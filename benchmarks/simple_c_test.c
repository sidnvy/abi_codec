#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ethc/abi.h>
#include <time.h>

#define ITERATIONS 10000

int main() {
    uint8_t buffer[36];

    printf("=== Pure C libethc Performance (Reuse Pattern + Pre-converted Address) ===\n\n");

    // Use reuse pattern with eth_abi_free for optimal performance
    static struct eth_abi abi;
    static int initialized = 0;

    if (!initialized) {
        memset(&abi, 0, sizeof(abi));
        initialized = 1;
    }

    // Pre-convert address to hex string (zero-copy optimization)
    char addr_hex[43] = "0x1234567890abcdef1234567890abcdef12345678";

    clock_t start = clock();
    for (int i = 0; i < ITERATIONS; ++i) {
        char *fn = "balanceOf(address)", *hex;
        char *addr = addr_hex;  // Use pre-converted address
        size_t hexlen;

        if (eth_abi_init(&abi, ETH_ABI_ENCODE) < 0) continue;
        if (eth_abi_call(&abi, &fn, NULL) < 0) continue;
        if (eth_abi_address(&abi, &addr) < 0) continue;
        if (eth_abi_call_end(&abi) < 0) continue;
        if (eth_abi_to_hex(&abi, &hex, &hexlen) < 0) continue;

        // Convert to bytes (same as C++ version)
        if (hexlen >= 72) {
            for (size_t j = 0; j < 72; j += 2) {
                char byte_str[3] = {hex[j], hex[j+1], '\0'};
                buffer[j/2] = (uint8_t)strtol(byte_str, NULL, 16);
            }
        }

        // Clean up with eth_abi_free for proper resource management
        eth_abi_free(&abi);
        free(hex);
    }
    clock_t end = clock();

    double total_time_seconds = (double)(end - start) / CLOCKS_PER_SEC;
    double total_time_ns = total_time_seconds * 1000000000.0;
    double per_call_ns = total_time_ns / ITERATIONS;

    printf("libethc with reuse pattern (eth_abi_free):\n");
    printf("- Per-call time: %.3f nanoseconds\n", per_call_ns);
    printf("- Iterations: %d\n", ITERATIONS);
    printf("- Pattern: Static struct + eth_abi_free cleanup\n");
    printf("- Compare with C++ version in balanceof_perf_comparison\n");

    return 0;
}
