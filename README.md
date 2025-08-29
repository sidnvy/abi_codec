# ABI Codec

A high-performance C++ library for Ethereum ABI encoding and decoding, validated against ethers.js and real RPC calls.

## Features

- Header-only C++17 library
- Ethereum ABI compliant
- Validated against ethers.js
- Real RPC testing with live Ethereum mainnet
- 23 comprehensive tests
- Zero dependencies (except Boost.Multiprecision)

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

```cpp
#include "abi/abi.h"
#include "abi/protocols.h"

// Basic encoding/decoding
uint256_t value = uint256_t("123456789");
uint8_t buf[32];
abi::Error err;
bool ok = abi::encode_into<abi::uint_t<256>>(buf, sizeof(buf), value, &err);

// Function call encoding
using Transfer = abi::protocols::Transfer;
std::array<uint8_t, 20> to = util::addr_from_hex("0x1111111111111111111111111111111111111111");
uint256_t amount = uint256_t("1000");
bool success = Transfer::encode_into<decltype(to), decltype(amount)>(buffer, sizeof(buffer), to, amount, &err);
```

## API

```cpp
namespace abi {
    // Core types
    struct bool_t, uint_t<N>, int_t<N>, address20, bytes, string_t;
    template<class T> struct dyn_array;
    template<class... Ts> struct tuple;
    
    // Encoding/decoding
    template<class Schema, class V>
    bool encode_into(uint8_t* out, size_t cap, const V& v, Error* e=nullptr);
    
    template<class Schema, class V>
    bool decode_from(BytesSpan in, V& out, Error* e=nullptr);
}
```

## Testing

```bash
npm test  # Run all 23 tests
```

**Test Coverage:**
- Encode vs ethers.js (11 tests)
- Return-value round trips (5 tests)  
- Real RPC tests (4 tests)
- Basic types, complex structures, edge cases (3 tests)

## Protocol Support

Auto-generated from ABI JSON files:
- **ERC20**: `balanceOf`, `transfer`, `approve`, etc.
- **Multicall3**: `tryAggregate`, `aggregate3`, etc.
- **UniswapV3**: `slot0`, `ticks`, `token0`, `token1`, etc.

## Project Structure

```
abi_codec/
├── include/abi/          # Core library
├── abis/                 # ABI JSON files
├── tests/                # Test suite
├── scripts/              # Generation & testing scripts
└── CMakeLists.txt        # Build configuration
```

## Integration

Header-only library - just include `abi/abi.h` and compile with C++17.
