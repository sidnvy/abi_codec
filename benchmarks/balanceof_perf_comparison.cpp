#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <string>
#include <cstdlib>  // for malloc, free
#include <cstdio>   // for strcpy, strlen (though strlen is in cstring)
#include <fstream>  // for file output
#include <algorithm> // for std::sort, std::shuffle
#include <random>    // for randomization
#include <numeric>   // for std::accumulate
#include <cmath>     // for statistical functions
#include <functional> // for std::function
#include <cctype>     // for std::tolower
#ifdef __linux__
#include <sched.h>    // for sched_getaffinity, CPU_*
#include <fstream>    // already included, but for governor file
#endif
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

// Volatile sink to prevent dead-code elimination
static volatile uint64_t global_checksum_sink = 0;

// Function to fold buffer contents into checksum (prevents DCE)
inline void fold_into_checksum(const uint8_t* data, size_t len) {
    uint64_t checksum = 0;
    for (size_t i = 0; i < len; ++i) {
        checksum = (checksum * 131) ^ data[i];  // Simple hash function
    }
    global_checksum_sink ^= checksum;
}

// Function to fold hex string into checksum (prevents DCE)
inline void fold_hex_into_checksum(const char* hex_data, size_t len) {
    uint64_t checksum = 0;
    for (size_t i = 0; i < len; ++i) {
        checksum = (checksum * 131) ^ static_cast<uint8_t>(hex_data[i]);  // Simple hash function
    }
    global_checksum_sink ^= checksum;
}

// Statistical utility functions
double calculate_median(std::vector<double>& values) {
    std::sort(values.begin(), values.end());
    size_t n = values.size();
    if (n % 2 == 0) {
        return (values[n/2 - 1] + values[n/2]) / 2.0;
    } else {
        return values[n/2];
    }
}

double calculate_percentile(const std::vector<double>& values, double percentile) {
    std::vector<double> sorted = values;
    std::sort(sorted.begin(), sorted.end());
    size_t n = sorted.size();
    double index = percentile * (n - 1);
    size_t lower = static_cast<size_t>(index);
    size_t upper = std::min(lower + 1, n - 1);
    double weight = index - lower;
    return sorted[lower] * (1 - weight) + sorted[upper] * weight;
}

double calculate_mean(const std::vector<double>& values) {
    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

double calculate_stddev(const std::vector<double>& values, double mean) {
    double sum_sq = 0.0;
    for (double val : values) {
        double diff = val - mean;
        sum_sq += diff * diff;
    }
    return std::sqrt(sum_sq / values.size());
}

// Benchmark configuration
struct BenchmarkConfig {
    int warmup_iterations = 1000;
    int measurement_iterations = 10000;
    int num_trials = 30;
    bool randomize_order = true;
};

// Benchmark result
struct BenchmarkResult {
    std::string name;
    double median_ns;
    double p95_ns;
    double mean_ns;
    double stddev_ns;
    std::vector<double> all_measurements;
};

// Function to run a single benchmark trial
template<typename Func>
double run_single_trial(Func&& benchmark_func, int iterations) {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        benchmark_func();
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / static_cast<double>(iterations);
}



// Helper function to find benchmark result by name
auto find_benchmark_by_name = [](const std::vector<BenchmarkResult>& results, const std::string& name) {
    return std::find_if(results.begin(), results.end(),
        [&name](const BenchmarkResult& r) { return r.name == name; });
};

// Helper function to compare hex strings as byte sequences (handles case differences)
auto hex_equal_as_bytes = [](const char* a, const char* b) -> bool {
    auto hex2n = [](char c) -> uint8_t {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return 0;
    };

    // Compare 36 bytes (72 hex chars)
    for (size_t i = 0; i < 72; i += 2) {
        uint8_t ab = (hex2n(a[i]) << 4) | hex2n(a[i + 1]);
        uint8_t bb = (hex2n(b[i]) << 4) | hex2n(b[i + 1]);
        if (ab != bb) return false;
    }
    return true;
};

// Function to run multiple benchmarks with randomized order
std::vector<BenchmarkResult> run_benchmark_suite(
    const std::vector<std::pair<std::string, std::function<void()>>>& benches,
    const BenchmarkConfig& cfg) {

    // Warm up each benchmark once
    for (auto& [_, f] : benches)
        for (int i = 0; i < cfg.warmup_iterations; ++i) f();

    std::vector<BenchmarkResult> res(benches.size());
    for (size_t i = 0; i < benches.size(); ++i) res[i].name = benches[i].first;
    for (auto& r : res) r.all_measurements.reserve(cfg.num_trials);

    std::vector<size_t> idx(benches.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::mt19937 gen(std::random_device{}());

    for (int t = 0; t < cfg.num_trials; ++t) {
        if (cfg.randomize_order) std::shuffle(idx.begin(), idx.end(), gen);
        for (size_t k : idx) {
            double m = run_single_trial(benches[k].second, cfg.measurement_iterations);
            res[k].all_measurements.push_back(m);
        }
    }

    for (auto& r : res) {
        r.median_ns = calculate_median(r.all_measurements);
        r.p95_ns = calculate_percentile(r.all_measurements, 0.95);
        r.mean_ns = calculate_mean(r.all_measurements);
        r.stddev_ns = calculate_stddev(r.all_measurements, r.mean_ns);
    }
    return res;
}

// Manual balanceOf encoding - direct implementation (bytes output)
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

// Manual balanceOf encoding - hex output
bool manual_encode_balanceof_hex(char* out, size_t cap, const std::array<uint8_t, 20>& address) {
    const size_t needed = 72; // 36 bytes * 2 hex chars per byte
    if (cap < needed + 1) return false; // +1 for null terminator

    uint8_t temp_buffer[36];
    if (!manual_encode_balanceof(temp_buffer, sizeof(temp_buffer), address)) return false;

    fast_bytes_to_hex(temp_buffer, 36, out);
    return true;
}

// Library-based encoding wrapper (bytes output)
bool library_encode_balanceof(uint8_t* out, size_t cap, const std::array<uint8_t, 20>& address) {
    abi::Error err;
    return abi::protocols::ERC20_BalanceOf::template encode_call<std::array<uint8_t, 20>>(out, cap, address, &err);
}

// Library-based encoding wrapper (hex output)
bool library_encode_balanceof_hex(char* out, size_t cap, const std::array<uint8_t, 20>& address) {
    uint8_t temp_buffer[36];
    if (!library_encode_balanceof(temp_buffer, sizeof(temp_buffer), address)) return false;
    fast_bytes_to_hex(temp_buffer, 36, out);
    return true;
}

// HFT-optimized library encoding wrapper (zero-allocation, bytes output)
uint8_t* hft_library_encode_balanceof(const std::array<uint8_t, 20>& address) {
    return abi::protocols::ERC20_BalanceOf::encode_call_hft(address);
}

// HFT-optimized library encoding wrapper (hex output)
bool hft_encode_balanceof_hex(char* out, size_t cap, const std::array<uint8_t, 20>& address) {
    uint8_t* temp_buffer = hft_library_encode_balanceof(address);
    if (!temp_buffer) return false;
    fast_bytes_to_hex(temp_buffer, 36, out);
    return true;
}

    // libethc-based encoding wrapper with proper error handling and memory management
bool libethc_encode_balanceof(uint8_t* out, size_t cap, const char* addr_hex) {
    struct eth_abi abi;  // Use fresh instance per call to avoid reuse issues
        memset(&abi, 0, sizeof(abi));

    const size_t needed = 36;
    if (cap < needed) return false;

    // Use string literals exactly like the C version for fair comparison
    char *fn = "balanceOf(address)", *hex = nullptr;
    size_t hexlen = 0;
    bool success = false;

    // libethc ABI calls - proper error handling
    do {
        if (eth_abi_init(&abi, ETH_ABI_ENCODE) < 0) break;
        if (eth_abi_call(&abi, &fn, NULL) < 0) break;
        // Create a copy of addr_hex since the API expects char** and may modify
        size_t addr_len = std::strlen(addr_hex);
        char* addr_copy = static_cast<char*>(std::malloc(addr_len + 1));
        if (!addr_copy) break;
        std::memcpy(addr_copy, addr_hex, addr_len + 1);
        if (eth_abi_address(&abi, &addr_copy) < 0) {
            std::free(addr_copy);
            break;
        }
        std::free(addr_copy); // eth_abi_address may have taken ownership or copied
        if (eth_abi_call_end(&abi) < 0) break;
        if (eth_abi_to_hex(&abi, &hex, &hexlen) < 0) break;

        // Handle optional "0x" prefix and convert hex string back to bytes
        const char* p = hex;
        size_t n = hexlen;
        if (n >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) { p += 2; n -= 2; }

        // Check we have exactly the expected payload
        if (n != needed * 2) break;

        // Normalize case and convert hex to bytes
        char normalized[73]; // Max possible hex length
        for (size_t i = 0; i < n; ++i) {
            normalized[i] = std::tolower(static_cast<unsigned char>(p[i]));
        }
        normalized[n] = '\0';

        for (size_t i = 0; i < n; i += 2) {
            char byte_str[3] = {normalized[i], normalized[i+1], '\0'};
            out[i/2] = static_cast<uint8_t>(strtol(byte_str, nullptr, 16));
        }
        success = true;
    } while (false);

    // Clean up - eth_abi_free should handle ABI cleanup, free hex if allocated
    eth_abi_free(&abi);
    if (hex) free(hex);

    return success;
}

    // libethc-based encoding wrapper - hex output only (no hex-to-bytes conversion)
bool libethc_encode_balanceof_hex(char* out, size_t cap, const char* addr_hex) {
    struct eth_abi abi;  // Use fresh instance per call to avoid reuse issues
    memset(&abi, 0, sizeof(abi));

    const size_t needed = 72; // 36 bytes * 2 hex chars per byte
    if (cap < needed + 1) return false; // +1 for null terminator

    // Use string literals exactly like the C version for fair comparison
    char *fn = "balanceOf(address)", *hex = nullptr;
    size_t hexlen = 0;
    bool success = false;

    // libethc ABI calls - proper error handling
    do {
        if (eth_abi_init(&abi, ETH_ABI_ENCODE) < 0) break;
        if (eth_abi_call(&abi, &fn, NULL) < 0) break;
        // Create a copy of addr_hex since the API expects char** and may modify
        size_t addr_len = std::strlen(addr_hex);
        char* addr_copy = static_cast<char*>(std::malloc(addr_len + 1));
        if (!addr_copy) break;
        std::memcpy(addr_copy, addr_hex, addr_len + 1);
        if (eth_abi_address(&abi, &addr_copy) < 0) {
            std::free(addr_copy);
            break;
        }
        std::free(addr_copy); // eth_abi_address may have taken ownership or copied
        if (eth_abi_call_end(&abi) < 0) break;
        if (eth_abi_to_hex(&abi, &hex, &hexlen) < 0) break;

        // Handle optional "0x" prefix and copy hex result directly
        const char* p = hex;
        size_t n = hexlen;
        if (n >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) { p += 2; n -= 2; }

        // Check we have exactly the expected payload
        if (n != needed) break;

        // Normalize case and copy hex result
        for (size_t i = 0; i < n; ++i) {
            out[i] = std::tolower(static_cast<unsigned char>(p[i]));
        }
        out[n] = '\0';
        success = true;
    } while (false);

    // Clean up - eth_abi_free should handle ABI cleanup, free hex if allocated
    eth_abi_free(&abi);
    if (hex) free(hex);

    return success;
}



int main() {
    // Open output file for GitHub Actions parsing
    std::ofstream output_file("benchmark_output.txt");
    if (!output_file.is_open()) {
        std::cerr << "Error: Could not open benchmark_output.txt for writing\n";
        return 1;
    }

    // Test address
    std::array<uint8_t, 20> test_address = {
        0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
        0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
        0x12, 0x34, 0x56, 0x78
    };

    // Buffers for bytes output
    std::vector<uint8_t> manual_buffer(36);
    std::vector<uint8_t> library_buffer(36);
    std::vector<uint8_t> libethc_buffer(36);
    std::vector<uint8_t> hft_buffer(36);

    // Buffers for hex output
    std::vector<char> manual_hex_buffer(73);
    std::vector<char> library_hex_buffer(73);
    std::vector<char> libethc_hex_buffer(73);
    std::vector<char> hft_hex_buffer(73);

    // Pre-convert address to hex string
    char addr_hex[43];
    addr_hex[0] = '0';
    addr_hex[1] = 'x';
    fast_bytes_to_hex(test_address.data(), 20, addr_hex + 2);

    // Helper function to write to both console and file
    auto write_output = [&](const std::string& line) {
        std::cout << line;
        output_file << line;
    };

    std::cout << "=== BalanceOf Encoding Performance Comparison ===\n\n";

    // ============================================================================
    // PROFILE 1: BYTES OUTPUT BENCHMARK
    // ============================================================================
    write_output("=== BYTES OUTPUT BENCHMARK ===\n");

    // Validate bytes outputs
    bool manual_bytes_success = manual_encode_balanceof(manual_buffer.data(), manual_buffer.size(), test_address);
    bool library_bytes_success = library_encode_balanceof(library_buffer.data(), library_buffer.size(), test_address);
    bool libethc_bytes_success = libethc_encode_balanceof(libethc_buffer.data(), libethc_buffer.size(), addr_hex);
    uint8_t* hft_result = hft_library_encode_balanceof(test_address);
    bool hft_bytes_success = (hft_result != nullptr);

    if (hft_bytes_success) {
        std::memcpy(hft_buffer.data(), hft_result, 36);
    }

    write_output("Bytes encoding success rates:\n");
    write_output("  Manual: " + std::string(manual_bytes_success ? "PASS" : "FAIL") + "\n");
    write_output("  Library: " + std::string(library_bytes_success ? "PASS" : "FAIL") + "\n");
    write_output("  HFT Library: " + std::string(hft_bytes_success ? "PASS" : "FAIL") + "\n");
    write_output("  libethc: " + std::string(libethc_bytes_success ? "PASS" : "FAIL") + " (excluded from bytes benchmark - hex→bytes overhead)\n");

    if (manual_bytes_success && library_bytes_success && hft_bytes_success) {
        // Configure comprehensive benchmarking
        BenchmarkConfig config;
        config.warmup_iterations = 1000;
        config.measurement_iterations = 1000;  // Smaller per-trial to allow more trials
        config.num_trials = 20;
        config.randomize_order = true;

        write_output("Running comprehensive bytes benchmark...\n");
        write_output("Configuration: " + std::to_string(config.num_trials) + " trials, " +
                    std::to_string(config.measurement_iterations) + " iterations each, randomized order\n");

        // Define benchmark functions (excluding libethc to avoid hex→bytes overhead unfairness)
        std::vector<std::pair<std::string, std::function<void()>>> bytes_benchmarks = {
            {"Manual (bytes)", [&]() {
                manual_encode_balanceof(manual_buffer.data(), manual_buffer.size(), test_address);
                fold_into_checksum(manual_buffer.data(), 36);
            }},
            {"Library (bytes)", [&]() {
                library_encode_balanceof(library_buffer.data(), library_buffer.size(), test_address);
                fold_into_checksum(library_buffer.data(), 36);
            }},
            {"HFT Library (bytes)", [&]() {
                uint8_t* result = hft_library_encode_balanceof(test_address);
                if (result) std::memcpy(hft_buffer.data(), result, 36);
                fold_into_checksum(hft_buffer.data(), 36);
            }}
        };

        // Run comprehensive benchmark
        auto bytes_results = run_benchmark_suite(bytes_benchmarks, config);

        // Report results
        write_output("\n=== BYTES OUTPUT - STATISTICAL RESULTS ===\n");
        write_output("All times in nanoseconds per call (median/p95/mean ± stddev)\n\n");

        for (const auto& result : bytes_results) {
            write_output(result.name + ":\n");
            write_output("  Median: " + std::to_string(result.median_ns) + " ns\n");
            write_output("  P95:    " + std::to_string(result.p95_ns) + " ns\n");
            write_output("  Mean:   " + std::to_string(result.mean_ns) + " ± " +
                        std::to_string(result.stddev_ns) + " ns\n");
            write_output("  Trials: " + std::to_string(result.all_measurements.size()) + "\n\n");
        }

        // Calculate relative performance ratios
        auto baseline_it = find_benchmark_by_name(bytes_results, "Manual (bytes)");
        if (baseline_it != bytes_results.end()) {
            double baseline_median = baseline_it->median_ns;
            write_output("Performance ratios (Manual = 1.0):\n");
            for (const auto& result : bytes_results) {
                double ratio = result.median_ns / baseline_median;
                write_output("  " + result.name + ": " + std::to_string(ratio) + "x\n");
            }
        }
    }

    // ============================================================================
    // PROFILE 2: HEX OUTPUT BENCHMARK
    // ============================================================================
    write_output("\n=== HEX OUTPUT BENCHMARK ===\n");

    // Validate hex outputs
    bool manual_hex_success = manual_encode_balanceof_hex(manual_hex_buffer.data(), manual_hex_buffer.size(), test_address);
    bool library_hex_success = library_encode_balanceof_hex(library_hex_buffer.data(), library_hex_buffer.size(), test_address);
    bool libethc_hex_success = libethc_encode_balanceof_hex(libethc_hex_buffer.data(), libethc_hex_buffer.size(), addr_hex);
    bool hft_hex_success = hft_encode_balanceof_hex(hft_hex_buffer.data(), hft_hex_buffer.size(), test_address);

    write_output("Hex encoding success rates:\n");
    write_output("  Manual: " + std::string(manual_hex_success ? "PASS" : "FAIL") + "\n");
    write_output("  Library: " + std::string(library_hex_success ? "PASS" : "FAIL") + "\n");
    write_output("  HFT Library: " + std::string(hft_hex_success ? "PASS" : "FAIL") + "\n");
    write_output("  libethc: " + std::string(libethc_hex_success ? "PASS" : "FAIL") + "\n");

    if (manual_hex_success && library_hex_success && hft_hex_success && libethc_hex_success) {
        // Configure comprehensive benchmarking for hex
        BenchmarkConfig config;
        config.warmup_iterations = 1000;
        config.measurement_iterations = 1000;  // Smaller per-trial to allow more trials
        config.num_trials = 20;
        config.randomize_order = true;

        write_output("Running comprehensive hex benchmark...\n");
        write_output("Configuration: " + std::to_string(config.num_trials) + " trials, " +
                    std::to_string(config.measurement_iterations) + " iterations each, randomized order\n");

        // Define hex benchmark functions
        std::vector<std::pair<std::string, std::function<void()>>> hex_benchmarks = {
            {"Manual (hex)", [&]() {
                manual_encode_balanceof_hex(manual_hex_buffer.data(), manual_hex_buffer.size(), test_address);
                fold_hex_into_checksum(manual_hex_buffer.data(), 72);
            }},
            {"Library (hex)", [&]() {
                library_encode_balanceof_hex(library_hex_buffer.data(), library_hex_buffer.size(), test_address);
                fold_hex_into_checksum(library_hex_buffer.data(), 72);
            }},
            {"HFT Library (hex)", [&]() {
                hft_encode_balanceof_hex(hft_hex_buffer.data(), hft_hex_buffer.size(), test_address);
                fold_hex_into_checksum(hft_hex_buffer.data(), 72);
            }},
            {"libethc (hex + alloc)", [&]() {
                libethc_encode_balanceof_hex(libethc_hex_buffer.data(), libethc_hex_buffer.size(), addr_hex);
                fold_hex_into_checksum(libethc_hex_buffer.data(), 72);
            }}
        };

        // Run comprehensive benchmark
        auto hex_results = run_benchmark_suite(hex_benchmarks, config);

        // Report results
        write_output("\n=== HEX OUTPUT - STATISTICAL RESULTS ===\n");
        write_output("All times in nanoseconds per call (median/p95/mean ± stddev)\n\n");

        for (const auto& result : hex_results) {
            write_output(result.name + ":\n");
            write_output("  Median: " + std::to_string(result.median_ns) + " ns\n");
            write_output("  P95:    " + std::to_string(result.p95_ns) + " ns\n");
            write_output("  Mean:   " + std::to_string(result.mean_ns) + " ± " +
                        std::to_string(result.stddev_ns) + " ns\n");
            write_output("  Trials: " + std::to_string(result.all_measurements.size()) + "\n\n");
        }

        // Calculate relative performance ratios
        auto baseline_it = find_benchmark_by_name(hex_results, "Manual (hex)");
        if (baseline_it != hex_results.end()) {
            double baseline_median = baseline_it->median_ns;
            write_output("Performance ratios (Manual = 1.0):\n");
            for (const auto& result : hex_results) {
                double ratio = result.median_ns / baseline_median;
                write_output("  " + result.name + ": " + std::to_string(ratio) + "x\n");
            }
            write_output("\nNote: libethc includes per-call malloc/free overhead\n");
        }
    }

    // ============================================================================
    // VALIDATION & ANALYSIS
    // ============================================================================
    write_output("\n=== VALIDATION ===\n");

    // Cross-validate outputs are equivalent (libethc excluded from bytes benchmark due to hex→bytes overhead)
    if (manual_bytes_success && library_bytes_success && hft_bytes_success) {
        bool bytes_consistent = (manual_buffer == library_buffer) &&
                               (manual_buffer == hft_buffer);
        write_output("Bytes outputs consistent: " + std::string(bytes_consistent ? "YES" : "NO") + "\n");
    }

    // libethc bytes validation (separate from timed benchmark)
    if (libethc_bytes_success) {
        write_output("libethc bytes validation: " + std::string((manual_buffer == libethc_buffer) ? "PASS" : "FAIL") + " (not timed)\n");
    }

    if (manual_hex_success && library_hex_success && hft_hex_success && libethc_hex_success) {
        bool hex_consistent = hex_equal_as_bytes(manual_hex_buffer.data(), library_hex_buffer.data()) &&
                             hex_equal_as_bytes(manual_hex_buffer.data(), hft_hex_buffer.data()) &&
                             hex_equal_as_bytes(manual_hex_buffer.data(), libethc_hex_buffer.data());
        write_output("Hex outputs consistent: " + std::string(hex_consistent ? "YES" : "NO") + "\n");
    }

    write_output("\n=== ANALYSIS ===\n");
    write_output("Note: Separate profiles prevent unfair comparisons between hex and bytes APIs\n");
    write_output("Benchmark methodology: 20 randomized trials, 1000 iterations each, with warm-up\n");
    write_output("Statistical analysis: median/p95/mean ± stddev reported\n");
    write_output("Checksum to prevent DCE: " + std::to_string(global_checksum_sink) + "\n");

    // ============================================================================
    // ENVIRONMENT & BUILD INFORMATION
    // ============================================================================
    write_output("\n=== ENVIRONMENT & BUILD INFORMATION ===\n");

    // Compiler information
#ifdef __clang__
    write_output("Compiler: Clang " + std::to_string(__clang_major__) + "." +
                std::to_string(__clang_minor__) + "." + std::to_string(__clang_patchlevel__) + "\n");
#elif defined(__GNUC__)
    write_output("Compiler: GCC " + std::to_string(__GNUC__) + "." +
                std::to_string(__GNUC_MINOR__) + "." + std::to_string(__GNUC_PATCHLEVEL__) + "\n");
#else
    write_output("Compiler: Unknown\n");
#endif

    // Build optimization level
#ifdef NDEBUG
    write_output("Build type: Release (NDEBUG defined)\n");
#else
    write_output("Build type: Debug\n");
#endif

    // CPU information (if available)
#ifdef __x86_64__
    write_output("Architecture: x86_64\n");
#elif defined(__aarch64__)
    write_output("Architecture: ARM64\n");
#else
    write_output("Architecture: Unknown\n");
#endif

    // Standard library
#if defined(_LIBCPP_VERSION)
    write_output("C++ Standard Library: libc++ " + std::to_string(_LIBCPP_VERSION) + "\n");
#elif defined(__GLIBCXX__)
    write_output("C++ Standard Library: libstdc++\n");
#else
    write_output("C++ Standard Library: Unknown\n");
#endif

    // C++ standard
#if __cplusplus >= 202002L
    write_output("C++ Standard: C++20 or later\n");
#elif __cplusplus >= 201703L
    write_output("C++ Standard: C++17\n");
#elif __cplusplus >= 201402L
    write_output("C++ Standard: C++14\n");
#else
    write_output("C++ Standard: C++11 or earlier\n");
#endif

    // Common optimization flags detection
    write_output("\nOptimization flags (detected):\n");
#ifdef __OPTIMIZE__
    write_output("  -O flag: Enabled\n");
#else
    write_output("  -O flag: Disabled (or -O0)\n");
#endif

#ifdef __OPTIMIZE_SIZE__
    write_output("  -Os: Enabled\n");
#endif

#ifdef __NO_INLINE__
    write_output("  -fno-inline: Enabled\n");
#endif

    // SIMD detection
#ifdef __SSE4_1__
    write_output("  SSE4.1: Available\n");
#endif
#ifdef __AVX__
    write_output("  AVX: Available\n");
#endif
#ifdef __AVX2__
    write_output("  AVX2: Available\n");
#endif
#ifdef __AVX512F__
    write_output("  AVX-512: Available\n");
#endif

    // Thread affinity and governor detection (Linux-specific)
#ifdef __linux__
    write_output("\nSystem configuration:\n");

    // Try to read CPU governor
    std::ifstream governor_file("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");
    if (governor_file.is_open()) {
        std::string governor;
        std::getline(governor_file, governor);
        write_output("  CPU governor: " + governor + "\n");
    }

    // Check if thread affinity is set (optional - not enforced)
    cpu_set_t cpu_set;
    if (sched_getaffinity(0, sizeof(cpu_set), &cpu_set) == 0) {
        int cpu_count = CPU_COUNT(&cpu_set);
        if (cpu_count == 1) {
            write_output("  Thread affinity: Single CPU (good for consistency)\n");
        } else {
            write_output("  Thread affinity: " + std::to_string(cpu_count) + " CPUs available\n");
        }
    }
#endif

    write_output("\nNote: For reproducible results, ensure:\n");
    write_output("- CPU frequency scaling is disabled (or set to performance governor)\n");
    write_output("- System is idle during benchmarking\n");
    write_output("- Consistent build flags across comparisons\n");
    write_output("- Same compiler version and optimization settings\n");

    // Add Linux-specific headers if needed
#ifdef __linux__
    write_output("- Consider pinning to single CPU core for consistency\n");
#endif

    // Close output file
    output_file.close();

    return 0;
}
