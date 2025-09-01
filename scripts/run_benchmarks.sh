#!/bin/bash

# ABI Codec Performance Benchmark Script
# This script provides consistent benchmark runs with proper environment setup

set -e

echo "🚀 ABI Codec Performance Benchmarks"
echo "=================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}✓${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

print_info() {
    echo -e "${BLUE}ℹ${NC} $1"
}

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    print_error "CMakeLists.txt not found. Please run this script from the project root."
    exit 1
fi

# Check dependencies
print_info "Checking dependencies..."
command -v cmake >/dev/null 2>&1 || { print_error "cmake is required but not installed."; exit 1; }
command -v make >/dev/null 2>&1 || { print_error "make is required but not installed."; exit 1; }

# Check for libethc dependencies
if ! pkg-config --exists libtommath; then
    print_warning "libtommath not found. libethc benchmarks will be skipped."
fi

if ! pkg-config --exists libsecp256k1; then
    print_warning "libsecp256k1 not found. libethc benchmarks will be skipped."
fi

# Clean and build
print_info "Building project..."
rm -rf build
mkdir -p build
cd build

# Configure with Release build for optimal performance
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

print_status "Build completed"

# Run benchmarks
# Save benchmark results for CI/CD
BENCHMARK_OUTPUT="../benchmarks/benchmark_output.txt"

print_info "Running performance benchmarks..."
echo ""

# Main benchmark
if [ -f "./benchmarks/balanceof_perf_comparison" ]; then
    print_info "Running ABI encoding performance comparison..."
    ./benchmarks/balanceof_perf_comparison | tee -a "$BENCHMARK_OUTPUT"
    echo ""
else
    print_error "Main benchmark executable not found"
fi

# Pure C benchmark (if available)
if [ -f "./benchmarks/simple_c_test" ]; then
    print_info "Running pure C libethc benchmark..."
    ./benchmarks/simple_c_test | tee -a "$BENCHMARK_OUTPUT"
    echo ""
fi

print_status "Benchmarks completed!"
print_info "Results are displayed above. For automated CI/CD results, see the GitHub Actions workflow."

# Generate summary
echo ""
echo "📊 Summary:"
echo "==========="
echo "- Manual encoding: Baseline performance"
echo "- Library (abi_codec): Template-optimized C++ implementation"
echo "- libethc: C library for comparison"
echo ""
echo "The library implementation should show excellent performance with minimal overhead"
echo "compared to manual encoding, while providing type safety and maintainability."

echo ""
echo "📁 Benchmark results saved to: $BENCHMARK_OUTPUT"
