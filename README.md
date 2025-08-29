# ABI Codec

A high-performance C++ library for Ethereum ABI encoding and decoding, with clean named structs for ABI types and flexible generic functions for custom data.

## Features

- **Header-only C++17 library** - No external dependencies except Boost.Multiprecision
- **Ethereum ABI compliant** - Full Ethereum ABI specification support
- **Validated against ethers.js** - All encodings tested for compatibility
- **Real RPC testing** - Live Ethereum mainnet validation
- **69 comprehensive tests** - Extensive test coverage
- **Named structs from ABI** - Clean, type-safe access to contract data
- **Generic functions** - Flexible encoding/decoding for any schema
- **Zero runtime overhead** - Template-based with compile-time optimization

## Supported Types

- `bool` - Boolean values
- `uintN` / `intN` - Integer types (8-256 bits)
- `address` - 20-byte Ethereum addresses
- `bytes` - Dynamic byte arrays
- `string` - UTF-8 strings
- `T[]` - Dynamic arrays
- `(T1, T2, ...)` - Tuples/structs
- Complex nested structures (arrays of tuples, nested arrays)

## Quick Start

```bash
git clone <repository-url>
cd abi_codec
npm install
npm test
```

## Usage

### Named Structs (Recommended for ABI data)

```cpp
#include "abi/abi.h"
#include "abi/protocols.h"

// Clean access to Uniswap V3 Pool data
IUniswapV3Pool_Slot0 slot0_result;
abi::BytesSpan slot0_data(encoded_bytes, size);

// Decode with meaningful field names
abi::decode_from<IUniswapV3Pool_Slot0>(slot0_data, slot0_result);

// Access fields by name (no index confusion!)
std::cout << "Price: " << slot0_result.sqrtPriceX96 << "\n";
std::cout << "Tick: " << slot0_result.tick << "\n";
std::cout << "Unlocked: " << slot0_result.unlocked << "\n";

// Encode back to bytes
const size_t size = abi::encoded_size_data<IUniswapV3Pool_Slot0>(slot0_result);
std::vector<uint8_t> buffer(size);
abi::encode_data_into<IUniswapV3Pool_Slot0>(buffer.data(), buffer.size(), slot0_result);
```

### Generic Functions (For custom data)

```cpp
#include "abi/abi.h"

// Calculate encoded size
boost::multiprecision::cpp_int value = boost::multiprecision::cpp_int("123456789");
const size_t size = abi::encoded_size_data<abi::uint_t<256>>(value);

// Encode to buffer
std::vector<uint8_t> buffer(size);
abi::Error err;
abi::encode_data_into<abi::uint_t<256>>(buffer.data(), buffer.size(), value, &err);

// Decode from buffer
boost::multiprecision::cpp_int decoded;
abi::BytesSpan span(buffer.data(), buffer.size());
abi::decode_from<abi::uint_t<256>>(span, decoded, &err);
```

### Function Calls (Contract interactions)

```cpp
// ERC20 transfer
using Transfer = abi::protocols::Transfer;
std::array<uint8_t, 20> recipient = {/* address bytes */};
boost::multiprecision::cpp_int amount = boost::multiprecision::cpp_int("1000000000000000000"); // 1 ETH

// Encode function call (includes 4-byte selector)
std::vector<uint8_t> call_data;
size_t call_size = Transfer::encoded_size(recipient, amount);
call_data.resize(call_size);
abi::Error err;
Transfer::encode_into(call_data.data(), call_data.size(), recipient, amount, &err);

// Decode return value
bool transfer_success;
abi::BytesSpan return_data(/* RPC response */, /* size */);
Transfer::decode(return_data, transfer_success, &err);
```

## API Reference

### Core Types
```cpp
namespace abi {
    // Primitive types
    struct bool_t {};                    // Boolean
    template<int N> struct uint_t {};    // Unsigned integer (8-256 bits)
    template<int N> struct int_t {};     // Signed integer (8-256 bits)
    struct address20 {};                 // 20-byte Ethereum address
    struct bytes {};                     // Dynamic byte array
    struct string_t {};                  // UTF-8 string

    // Composite types
    template<class T> struct dyn_array;  // Dynamic array: T[]
    template<class... Ts> struct tuple;  // Tuple: (T1, T2, ...)
}
```

### Generic Functions (For any schema)
```cpp
namespace abi {
    // Size calculation (no selector, for return data)
    template<class Schema, class V>
    size_t encoded_size_data(const V& value);

    // Data encoding (no selector, for return data)
    template<class Schema, class V>
    bool encode_data_into(uint8_t* out, size_t cap, const V& value, Error* e=nullptr);

    // Data decoding
    template<class Schema, class V>
    bool decode_from(BytesSpan in, V& out, Error* e=nullptr);
}
```

### Function Call Encoding (With selector)
```cpp
// Protocol-specific functions (auto-generated from ABI)
namespace abi::protocols {
    // Size calculation (includes 4-byte selector)
    template<class... Args>
    size_t encoded_size(const Args&... args);

    // Function call encoding (includes 4-byte selector)
    template<class... Args>
    bool encode_into(uint8_t* out, size_t cap, const Args&... args, Error* e=nullptr);

    // Return value decoding
    bool decode(BytesSpan in, ReturnType& out, Error* e=nullptr);
}
```

### Named Structs (Generated from ABI)
```cpp
// Example: Uniswap V3 Pool Slot0
struct IUniswapV3Pool_Slot0 {
    boost::multiprecision::cpp_int sqrtPriceX96;
    boost::multiprecision::cpp_int tick;
    boost::multiprecision::cpp_int observationIndex;
    boost::multiprecision::cpp_int observationCardinality;
    boost::multiprecision::cpp_int observationCardinalityNext;
    boost::multiprecision::cpp_int feeProtocol;
    bool unlocked;

    // Conversion to/from generic tuple
    static tuple_type to_tuple(const IUniswapV3Pool_Slot0& s);
    static IUniswapV3Pool_Slot0 from_tuple(const tuple_type& t);
};
```

## Named vs Generic: Choose Your Approach

### When to Use Named Structs 🏗️
```cpp
// ✅ BEST for ABI data with meaningful field names
IUniswapV3Pool_Slot0 slot0;
slot0.sqrtPriceX96 = boost::multiprecision::cpp_int("429512873912345678901234567890");
slot0.tick = 12345;
slot0.unlocked = true;

// Clean, readable access
std::cout << "Price: " << slot0.sqrtPriceX96 << "\n";
std::cout << "Tick: " << slot0.tick << "\n";
```

### When to Use Generic Tuples 🔧
```cpp
// ✅ BEST for unnamed types or internal processing
using SimpleReturn = abi::tuple<abi::uint_t<256>, abi::bool_t>;
auto result = std::make_tuple(boost::multiprecision::cpp_int("1000"), true);

// Index-based access (use comments!)
auto amount = std::get<0>(result);  // Amount
auto success = std::get<1>(result); // Success flag
```

### Best Practice: Wrapper for Unnamed Types
```cpp
struct TransferResult {
    boost::multiprecision::cpp_int amount;
    bool success;

    static TransferResult from_tuple(const std::tuple<boost::multiprecision::cpp_int, bool>& t) {
        return {std::get<0>(t), std::get<1>(t)};
    }
};

TransferResult clean = TransferResult::from_tuple(generic_tuple);
std::cout << "Amount: " << clean.amount << "\n";  // ✅ Clean access!
```

## Testing

```bash
npm test     # Run all tests
npm run test # Alternative test command
```

**Test Coverage (69 tests):**
- **Generic functions** - `encoded_size_data`, `encode_data_into` validation
- **Named structs** - ABI-generated types round-trip testing
- **Function calls** - Protocol-specific encoding/decoding
- **Edge cases** - Boundary conditions, error handling
- **Complex structures** - Nested arrays, tuples, dynamic types
- **Cross-validation** - Compare against ethers.js outputs
- **Real RPC data** - Live Ethereum mainnet validation

## Protocol Support

Auto-generated from ABI JSON files with clean named structs:

### ERC20 Protocol
```cpp
using BalanceOf = abi::protocols::BalanceOf;
using Transfer = abi::protocols::Transfer;
using Approve = abi::protocols::Approve;

// Clean function calls
std::array<uint8_t, 20> addr = /* address */;
boost::multiprecision::cpp_int amount = /* amount */;
bool success = Transfer::encode_into(buffer, size, addr, amount, &err);
```

### Multicall3 Protocol
```cpp
using Aggregate3 = abi::protocols::Aggregate3;
using TryAggregate = abi::protocols::TryAggregate;

// Handle complex multicall returns
std::vector<Multicall3_Result> results;
Aggregate3::decode(response_data, results, &err);
```

### UniswapV3 Protocol
```cpp
// Clean access to pool data
IUniswapV3Pool_Slot0 slot0;
abi::decode_from<IUniswapV3Pool_Slot0>(data, slot0);

// Meaningful field access
std::cout << "sqrtPriceX96: " << slot0.sqrtPriceX96 << "\n";
std::cout << "tick: " << slot0.tick << "\n";
std::cout << "unlocked: " << slot0.unlocked << "\n";
```

## Examples

Run the comprehensive example:
```bash
cd build && make user_usage_clean && ./examples/user_usage_clean
```

This demonstrates:
- ✅ Named struct usage (recommended for ABI data)
- ✅ Generic function usage (for custom data)
- ✅ Function call encoding/decoding
- ✅ Round-trip verification
- ✅ Named vs Generic comparison
- ✅ Best practices for unnamed types

## Project Structure

```
abi_codec/
├── include/abi/              # Core library
│   ├── abi.h                # Main API with generic functions
│   └── protocols.h          # Auto-generated named structs
├── abis/                     # ABI JSON files
│   ├── erc20.json           # ERC20 token interface
│   ├── multicall.json       # Multicall3 interface
│   └── uniswap_v3_pool.json # Uniswap V3 Pool interface
├── examples/                 # Usage examples
│   └── user_usage_clean.cpp # Comprehensive demonstration
├── tests/                    # Test suite (69 tests)
│   └── abi_tests.cpp        # All functionality tests
├── scripts/                  # Generation & testing scripts
│   ├── generate_from_abi_json.mjs # Generate protocols.h
│   └── encode.mjs           # ethers.js validation
└── CMakeLists.txt           # Build configuration
```

## Performance & Design

- **Zero runtime overhead** - All encoding/decoding is compile-time template-based
- **Header-only** - No linking required, just include and compile
- **Memory efficient** - Direct encoding to user-provided buffers
- **Type-safe** - Compile-time validation of all operations
- **ABI compliant** - Full Ethereum ABI specification support
- **Validated** - Cross-tested against ethers.js outputs

## Integration

### Header-only library - just include and compile:
```cpp
#include "abi/abi.h"           // Generic functions
#include "abi/protocols.h"     // Named structs (optional)

// Use generic functions
const size_t size = abi::encoded_size_data<abi::uint_t<256>>(value);
abi::encode_data_into<abi::uint_t<256>>(buffer, size, value);

// Or use named structs
IUniswapV3Pool_Slot0 slot0;
abi::decode_from<IUniswapV3Pool_Slot0>(data, slot0);
```

### CMake integration:
```cmake
add_subdirectory(abi_codec)
target_link_libraries(your_target abi_codec)
```

### Requirements:
- C++17 compiler
- Boost.Multiprecision (header-only)
- No other dependencies
