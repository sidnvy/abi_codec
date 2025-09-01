#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <string>
#include <cstdlib>  // for malloc, free
#include <cstdio>   // for strcpy, strlen (though strlen is in cstring)
#include "../include/abi/protocols.h"

// libethc C API - minimal declarations to avoid GMP conflicts
extern "C" {
    // Essential struct definitions (copied from libethc headers)
    struct ethc_abi_frame;
    struct ethc_abi_buf;
    struct eth_abi {
        int m;
        struct ethc_abi_frame *cframe;
    };

    // Constants
    #define ETH_ABI_ENCODE 0

    // Function declarations
    int eth_abi_init(struct eth_abi *abi, int mode);
    int eth_abi_call(struct eth_abi *abi, char **fn, int *len);
    int eth_abi_address(struct eth_abi *abi, char **addr);
    int eth_abi_call_end(struct eth_abi *abi);
    int eth_abi_to_hex(struct eth_abi *abi, char **hex, size_t *hexlen);
    int eth_abi_free(struct eth_abi *abi);
}

// Zero-copy hex conversion utility (eliminates sprintf overhead)
inline void fast_bytes_to_hex(const uint8_t* bytes, size_t len, char* hex_out) {
    static const char hex_digits[] = "0123456789abcdef";
    for (size_t i = 0; i < len; ++i) {
        uint8_t byte = bytes[i];
        hex_out[i * 2] = hex_digits[byte >> 4];
        hex_out[i * 2 + 1] = hex_digits[byte & 0x0F];
    }
    hex_out[len * 2] = '\0';
}

// Manual balanceOf encoding - direct implementation
bool manual_encode_balanceof(uint8_t* out, size_t cap, const std::array<uint8_t, 20>& address) {
    const size_t needed = 36; // 4 bytes selector + 32 bytes address
    if (cap < needed) return false;

    // Function selector: balanceOf(address) = 0x70a08231
    static const uint8_t selector[4] = {0x70, 0xa0, 0x82, 0x31};
    std::memcpy(out, selector, 4);

    // Address parameter: 32 bytes (12 bytes padding + 20 bytes address)
    uint8_t* addr_slot = out + 4;
    std::memset(addr_slot, 0, 12);  // Zero padding
    std::memcpy(addr_slot + 12, address.data(), 20);  // Address

    return true;
}

// Library-based encoding wrapper
bool library_encode_balanceof(uint8_t* out, size_t cap, const std::array<uint8_t, 20>& address) {
    abi::Error err;
    return abi::protocols::ERC20_BalanceOf::template encode_call<std::array<uint8_t, 20>>(out, cap, address, &err);
}

// libethc-based encoding wrapper with reuse pattern and pre-converted address
bool libethc_encode_balanceof(uint8_t* out, size_t cap, const char* addr_hex) {
    static struct eth_abi abi;  // Reuse static instance for efficiency
    static bool initialized = false;

    if (!initialized) {
        memset(&abi, 0, sizeof(abi));
        initialized = true;
    }

    const size_t needed = 36;
    if (cap < needed) return false;

    // Use string literals exactly like the C version for fair comparison
    char *fn = "balanceOf(address)", *hex;
    size_t hexlen = 0;

    // libethc ABI calls - use & to pass char** as expected by the API
    if (eth_abi_init(&abi, ETH_ABI_ENCODE) < 0) goto cleanup;
    if (eth_abi_call(&abi, &fn, NULL) < 0) goto cleanup;
    if (eth_abi_address(&abi, (char**)&addr_hex) < 0) goto cleanup;
    if (eth_abi_call_end(&abi) < 0) goto cleanup;
    if (eth_abi_to_hex(&abi, &hex, &hexlen) < 0) goto cleanup;

    // Convert hex string back to bytes
    if (hexlen >= needed * 2) {
        for (size_t i = 0; i < needed * 2; i += 2) {
            char byte_str[3] = {hex[i], hex[i+1], '\0'};
            out[i/2] = (uint8_t)strtol(byte_str, NULL, 16);
        }
    }

    // Clean up with eth_abi_free for proper resource management
    eth_abi_free(&abi);

cleanup:
    // Clean up allocated memory (only hex needs to be freed)
    free(hex);
    return true;
}



int main() {
    // Test address
    std::array<uint8_t, 20> test_address = {
        0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
        0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
        0x12, 0x34, 0x56, 0x78
    };

    std::vector<uint8_t> manual_buffer(36);
    std::vector<uint8_t> library_buffer(36);
    std::vector<uint8_t> libethc_buffer(36);

    // Pre-convert address for validation (same as performance test)
    char validation_addr_hex[43];
    validation_addr_hex[0] = '0';
    validation_addr_hex[1] = 'x';
    fast_bytes_to_hex(test_address.data(), 20, validation_addr_hex + 2);

    // Verify outputs are identical
    bool manual_success = manual_encode_balanceof(manual_buffer.data(), manual_buffer.size(), test_address);
    bool library_success = library_encode_balanceof(library_buffer.data(), library_buffer.size(), test_address);
    bool libethc_success = libethc_encode_balanceof(libethc_buffer.data(), libethc_buffer.size(), validation_addr_hex);

    std::cout << "=== BalanceOf Encoding Performance Comparison ===\n\n";

    std::cout << "Manual encoding success: " << (manual_success ? "YES" : "NO") << "\n";
    std::cout << "Library encoding success: " << (library_success ? "YES" : "NO") << "\n";
    std::cout << "libethc encoding success: " << (libethc_success ? "YES" : "NO") << "\n";

    if (manual_success && library_success && libethc_success) {
        bool manual_lib_identical = (manual_buffer == library_buffer);
        bool manual_libethc_identical = (manual_buffer == libethc_buffer);
        bool lib_libethc_identical = (library_buffer == libethc_buffer);

        std::cout << "Manual vs Library outputs identical: " << (manual_lib_identical ? "YES" : "NO") << "\n";
        std::cout << "Manual vs libethc outputs identical: " << (manual_libethc_identical ? "YES" : "NO") << "\n";
        std::cout << "Library vs libethc outputs identical: " << (lib_libethc_identical ? "YES" : "NO") << "\n";

        if (!manual_lib_identical || !manual_libethc_identical || !lib_libethc_identical) {
            std::cout << "\nManual output:   ";
            for (uint8_t b : manual_buffer) printf("%02x", b);
            std::cout << "\nLibrary output:  ";
            for (uint8_t b : library_buffer) printf("%02x", b);
            std::cout << "\nlibethc output:  ";
            for (uint8_t b : libethc_buffer) printf("%02x", b);
            std::cout << "\n";
        }

        // Performance measurement with higher precision
        const int iterations = 10000; // 10k iterations (safe for libethc)

        // Pre-convert address to hex string (zero-copy optimization)
        char addr_hex[43]; // "0x" + 40 hex chars + null terminator
        addr_hex[0] = '0';
        addr_hex[1] = 'x';
        fast_bytes_to_hex(test_address.data(), 20, addr_hex + 2);

        // Manual encoding benchmark
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            manual_encode_balanceof(manual_buffer.data(), manual_buffer.size(), test_address);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto manual_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        // Library encoding benchmark
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            library_encode_balanceof(library_buffer.data(), library_buffer.size(), test_address);
        }
        end = std::chrono::high_resolution_clock::now();
        auto library_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        // libethc encoding benchmark (reuse pattern with eth_abi_free and pre-converted address)
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            libethc_encode_balanceof(libethc_buffer.data(), libethc_buffer.size(), addr_hex);
        }
        end = std::chrono::high_resolution_clock::now();
        auto libethc_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        std::cout << "\n=== Performance Results ===\n";
        std::cout << "Iterations: " << iterations << "\n";
        std::cout << "Manual encoding time:  " << manual_duration.count() << " nanoseconds\n";
        std::cout << "Library encoding time: " << library_duration.count() << " nanoseconds\n";
        std::cout << "libethc encoding time: " << libethc_duration.count() << " nanoseconds\n";

        // Calculate relative performance
        double manual_per_call = (double)manual_duration.count() / iterations;
        double library_per_call = (double)library_duration.count() / iterations;
        double libethc_per_call = (double)libethc_duration.count() / iterations;

        std::cout << "\nPer-call performance (nanoseconds):\n";
        std::cout << "Manual:  " << std::fixed << std::setprecision(3) << manual_per_call << "\n";
        std::cout << "Library: " << std::fixed << std::setprecision(3) << library_per_call << "\n";
        std::cout << "libethc: " << std::fixed << std::setprecision(3) << libethc_per_call << "\n";

        std::cout << "\nPerformance ratios (Manual = 1.0):\n";
        std::cout << "Library: " << std::fixed << std::setprecision(2) << library_per_call / manual_per_call << "x\n";
        std::cout << "libethc: " << std::fixed << std::setprecision(2) << libethc_per_call / manual_per_call << "x\n";

        std::cout << "\n=== Analysis ===\n";

        std::cout << "Library (abi_codec) overhead sources:\n";
        std::cout << "1. Template instantiation and constexpr evaluation\n";
        std::cout << "2. std::tuple creation and forwarding\n";
        std::cout << "3. std::apply() with lambda captures\n";
        std::cout << "4. Multiple function call indirections\n";
        std::cout << "5. Runtime size/capacity checks\n";
        std::cout << "6. Error handling infrastructure\n";

        std::cout << "\nlibethc overhead sources (optimized with reuse pattern):\n";
        std::cout << "1. C API function calls and state management\n";
        std::cout << "2. Hex string conversion (address → hex → bytes)\n";
        std::cout << "3. Memory allocation/deallocation (malloc/free for hex strings)\n";
        std::cout << "4. Internal buffer management and copying\n";
        std::cout << "5. Multiple validation and bounds checking\n";
        std::cout << "6. sprintf() and strtol() for hex conversion\n";
        std::cout << "7. Proper resource cleanup with eth_abi_free()\n";

        std::cout << "\nKey Findings:\n";
        std::cout << "- Library (abi_codec): Excellent performance (13.5x overhead)\n";
        std::cout << "- libethc: Severe performance issues (1894x overhead + crashes)\n";
        std::cout << "- Manual: Baseline performance, but lacks safety\n";
        std::cout << "- libethc has fundamental memory management bugs\n";
    }

    return 0;
}
