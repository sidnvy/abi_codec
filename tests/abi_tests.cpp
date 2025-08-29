#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <iostream>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

#include "abi/abi.h"

// Type aliases for convenience
using uint256_t = boost::multiprecision::cpp_int;
#include "abi/protocols.h"

// ─────────────────────────────────────────────────────────────────────────────
// Small, focused utilities
// ─────────────────────────────────────────────────────────────────────────────
namespace util {

inline std::string run_node_and_capture(const std::string& cmd) {
#if defined(_WIN32)
  FILE* pipe = _popen(cmd.c_str(), "rb");
#else
  FILE* pipe = popen(cmd.c_str(), "r");
#endif
  if (!pipe) throw std::runtime_error("Failed to start node: " + cmd);

  std::string out;
  std::array<char, 4096> buf{};
  while (true) {
    size_t n = fread(buf.data(), 1, buf.size(), pipe);
    if (n == 0) break;
    out.append(buf.data(), n);
  }

#if defined(_WIN32)
  _pclose(pipe);
#else
  pclose(pipe);
#endif

  // trim single trailing newline / CR
  if (!out.empty() && (out.back() == '\n' || out.back() == '\r')) out.pop_back();
  return out;
}

inline std::string hex_lower(const uint8_t* p, size_t n) {
  static constexpr char k[] = "0123456789abcdef";
  std::string s; s.reserve(2 * n);
  for (size_t i = 0; i < n; ++i) {
    s.push_back(k[p[i] >> 4]);
    s.push_back(k[p[i] & 0xF]);
  }
  return s;
}

inline std::vector<uint8_t> parse_hex(const std::string& h) {
  size_t i = (h.size() >= 2 && (h[0] == '0') && (h[1] == 'x' || h[1] == 'X')) ? 2 : 0;
  std::vector<uint8_t> out; out.reserve((h.size() - i + 1) / 2);

  // handle odd-length payloads
  if (((h.size() - i) & 1) != 0) {
    auto c = h[i];
    auto nib = (c >= '0' && c <= '9') ? (c - '0') : (10 + ((c | 32) - 'a'));
    out.push_back(static_cast<uint8_t>(nib));
    ++i;
  }
  auto nib = [](char c)->uint8_t {
    if (c >= '0' && c <= '9') return static_cast<uint8_t>(c - '0');
    c |= 32; return static_cast<uint8_t>(10 + (c - 'a'));
  };
  for (; i + 1 < h.size(); i += 2) {
    out.push_back(static_cast<uint8_t>((nib(h[i]) << 4) | nib(h[i+1])));
  }
  return out;
}

inline std::array<uint8_t, 20> addr_from_hex(const std::string& addr) {
  auto b = parse_hex(addr);
  if (b.size() != 20) throw std::runtime_error("Invalid address length: " + addr);
  std::array<uint8_t,20> a{};
  std::copy(b.begin(), b.end(), a.begin());
  return a;
}

inline boost::multiprecision::cpp_int uint_from_str(const std::string& s) {
  return boost::multiprecision::cpp_int(s);
}
inline boost::multiprecision::cpp_int int_from_i64(int64_t v) {
  return boost::multiprecision::cpp_int(v);
}

inline std::string addr_to_hex_string(const std::array<uint8_t, 20>& addr) {
  return "0x" + hex_lower(addr.data(), addr.size());
}

} // namespace util

// ─────────────────────────────────────────────────────────────────────────────
// Round-trip helpers using generic ABI functions from abi.h
// ─────────────────────────────────────────────────────────────────────────────
namespace abi_test {

// Use the new generic functions from abi.h instead of custom implementations

template<class RetSchema>
bool roundtrip_return_hex(const std::string& hex) {
  using Ret = typename abi::value_of<RetSchema>::type;

  std::vector<uint8_t> in = util::parse_hex(hex);
  Ret v{};
  abi::Error err;

  if (!abi::decode_from<RetSchema>(abi::BytesSpan(in.data(), in.size()), v, &err)) {
    std::cerr << "decode failed: " << err.message << "\n";
    return false;
  }

  const size_t size = abi::encoded_size_data<RetSchema>(v);
  std::vector<uint8_t> out(size);
  if (!abi::encode_data_into<RetSchema>(out.data(), out.size(), v, &err)) {
    std::cerr << "re-encode failed: " << err.message << "\n";
    return false;
  }

  if (out != in) {
    std::cerr << "round-trip mismatch\n"
              << "  input : " << util::hex_lower(in.data(), in.size()) << "\n"
              << "  output: " << util::hex_lower(out.data(), out.size()) << "\n"
              << "  sizes : in=" << in.size() << " out=" << out.size() << "\n";
    return false;
  }
  return true;
}

// Generic round-trip (value -> encode -> decode -> ==)
template<class Schema>
bool roundtrip_value(const typename abi::value_of<Schema>::type& v){
  using V = typename abi::value_of<Schema>::type;
  abi::Error err;

  const size_t need = abi::encoded_size_data<Schema>(v);
  std::vector<uint8_t> buf(need, 0);

  if (!abi::encode_data_into<Schema>(buf.data(), buf.size(), v, &err)) {
    std::cerr << "encode failed: " << err.message << "\n";
    return false;
  }

  V out{};
  if (!abi::decode_from<Schema>(abi::BytesSpan(buf.data(), buf.size()), out, &err)) {
    std::cerr << "decode failed: " << err.message << "\n";
    return false;
  }

  if (!(out == v)) {
    std::cerr << "mismatch after roundtrip\n";
    return false;
  }
  return true;
}

} // namespace abi_test

// ─────────────────────────────────────────────────────────────────────────────
// Test harness plumbing
// ─────────────────────────────────────────────────────────────────────────────
struct Counters { int total=0, passed=0; };

#define RUN_TEST(desc, expr)                                                    \
  do {                                                                          \
    ++counts.total;                                                             \
    std::cout << "• " << (desc) << std::endl;                                   \
    if ((expr)) {                                                               \
      ++counts.passed;                                                          \
      std::cout << "  ✓ passed\n";                                              \
    } else {                                                                    \
      std::cout << "  ✗ failed\n";                                              \
    }                                                                           \
  } while(0)

// Use protocol typedefs we generated
using BalanceOf = abi::protocols::BalanceOf;
using Transfer  = abi::protocols::Transfer;
using Approve   = abi::protocols::Approve;
using Ticks     = abi::protocols::Ticks;
using GetPopulatedTicksInWord = abi::protocols::GetPopulatedTicksInWord;
using Aggregate = abi::protocols::Aggregate;
using Aggregate3 = abi::protocols::Aggregate3;
using TryAggregate = abi::protocols::TryAggregate;

// Helper function to convert arguments to strings for command line
template<typename T>
std::string arg_to_string(const T& arg) {
  if constexpr (std::is_same_v<T, std::array<uint8_t, 20>>) {
    return util::addr_to_hex_string(arg);
  } else if constexpr (std::is_same_v<T, bool>) {
    return arg ? "true" : "false";
  } else if constexpr (std::is_same_v<T, std::vector<std::tuple<std::array<uint8_t,20>, std::vector<uint8_t>>>>) {
    // Convert to JSON format for ethers.js
    std::string result = "[";
    for (size_t i = 0; i < arg.size(); ++i) {
      if (i > 0) result += ",";
      result += "[\"" + util::addr_to_hex_string(std::get<0>(arg[i])) + "\",\"0x";
      for (auto byte : std::get<1>(arg[i])) {
        result += util::hex_lower(&byte, 1);
      }
      result += "\"]";
    }
    result += "]";
    return result;
  } else if constexpr (std::is_same_v<T, std::vector<std::tuple<std::array<uint8_t,20>, bool, std::vector<uint8_t>>>>) {
    // Convert to JSON format for ethers.js
    std::string result = "[";
    for (size_t i = 0; i < arg.size(); ++i) {
      if (i > 0) result += ",";
      result += "[\"" + util::addr_to_hex_string(std::get<0>(arg[i])) + "\"," + 
                (std::get<1>(arg[i]) ? "true" : "false") + ",\"0x";
      for (auto byte : std::get<2>(arg[i])) {
        result += util::hex_lower(&byte, 1);
      }
      result += "\"]";
    }
    result += "]";
    return result;
  } else {
    return std::string(arg);
  }
}

// Generic encoding check against ethers.js (via encode.mjs)
template<class FnType, class... Args>
bool encode_matches_ethers(const std::string& signature, Args&&... args) {
  const char* dir = std::getenv("ABI_SCRIPTS_DIR");
  if (!dir) throw std::runtime_error("ABI_SCRIPTS_DIR not set");

  // Check if this is a multicall function that needs special handling
  bool is_multicall = (signature.find("tryAggregate") != std::string::npos) || (signature.find("aggregate3") != std::string::npos);
  
  std::string cmd;
  if (is_multicall) {
    cmd = std::string("node ") + dir + "/encode_multicall.mjs";
    if (signature.find("tryAggregate") != std::string::npos) {
      cmd += " tryAggregate";
      if constexpr (sizeof...(Args) > 0) {
        ((cmd += " " + arg_to_string(args)), ...);
      }
    } else if (signature.find("aggregate3") != std::string::npos) {
      cmd += " aggregate3";
      if constexpr (sizeof...(Args) > 0) {
        ((cmd += " " + arg_to_string(args)), ...);
      }
    }
  } else {
    cmd = std::string("node ") + dir + "/encode.mjs \"" + signature + "\"";
    if constexpr (sizeof...(Args) > 0) {
      ((cmd += " " + arg_to_string(args)), ...); // append argv as strings for node script
    }
  }

  const std::string expected = util::run_node_and_capture(cmd);

  uint8_t buf[2048];
  abi::Error err;
  bool ok = FnType::template encode_into<Args...>(buf, sizeof(buf), std::forward<Args>(args)..., &err);
  if (!ok) {
    std::cerr << "encode_into failed: " << err.message << "\n";
    return false;
  }
  size_t need = FnType::encoded_size(std::forward<Args>(args)...);
  std::string got = "0x" + util::hex_lower(buf, need);

  if (got != expected) {
    std::cerr << "encoding mismatch\n  expected: " << expected << "\n  got     : " << got << "\n";
    return false;
  }
  return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Main: clear, grouped tests
// ─────────────────────────────────────────────────────────────────────────────
int main() try {
  Counters counts;
  std::cout << "ABI encode/decode test suite\n\n";

/*   // ── 1) Call-data encoding vs ethers.js
  std::cout << "== encode vs ethers.js ==\n";
  {
    RUN_TEST("balanceOf(addr:0x00..00)",
      (encode_matches_ethers<BalanceOf>("balanceOf(address)",
        util::addr_from_hex("0x0000000000000000000000000000000000000000"))));

    RUN_TEST("balanceOf(addr:0x11..11)",
      (encode_matches_ethers<BalanceOf>("balanceOf(address)",
        util::addr_from_hex("0x1111111111111111111111111111111111111111"))));

    RUN_TEST("balanceOf(addr:0xff..ff)",
      (encode_matches_ethers<BalanceOf>("balanceOf(address)",
        util::addr_from_hex("0xffffffffffffffffffffffffffffffffffffffff"))));

    RUN_TEST("transfer(addr, uint256)",
      (encode_matches_ethers<Transfer>("transfer(address,uint256)",
        util::addr_from_hex("0x1111111111111111111111111111111111111111"),
        std::string("1000"))));

    RUN_TEST("approve(addr, uint256)",
      (encode_matches_ethers<Approve>("approve(address,uint256)",
        util::addr_from_hex("0x2222222222222222222222222222222222222222"),
        std::string("500"))));

    RUN_TEST("ticks(int24=0)",
      (encode_matches_ethers<Ticks>("ticks(int24)", std::string("0"))));

    RUN_TEST("ticks(int24=100)",
      (encode_matches_ethers<Ticks>("ticks(int24)", std::string("100"))));

    RUN_TEST("TickLens.getPopulatedTicksInWord(pool, word=0)",
      (encode_matches_ethers<GetPopulatedTicksInWord>("getPopulatedTicksInWord(address,int16)",
        util::addr_from_hex("0x8ad599c3A0ff1De082011EFDDc58f1908eb6e6D8"),
        std::string("0"))));

    // Multicall tests - dynamic arguments encoding
    RUN_TEST("multicall.tryAggregate encoding vs ethers.js",
      ([&](){
        // Test data: requireSuccess=false, single call with address and bytes
        bool requireSuccess = false;
        std::vector<std::tuple<std::array<uint8_t,20>, std::vector<uint8_t>>> calls{
          {util::addr_from_hex("0x1111111111111111111111111111111111111111"), {0x12, 0x34}}
        };
        
        // Get expected encoding from ethers.js
        const char* dir = std::getenv("ABI_SCRIPTS_DIR");
        if (!dir) throw std::runtime_error("ABI_SCRIPTS_DIR not set");
        
        std::string cmd = std::string("node ") + dir + "/encode_multicall.mjs tryAggregate false \"[[\\\"0x1111111111111111111111111111111111111111\\\",\\\"0x1234\\\"]]\"";
        std::string expected = util::run_node_and_capture(cmd);
        
        // Encode with our C++ codec
        uint8_t buf[2048];
        abi::Error err;
        bool ok = TryAggregate::template encode_into<decltype(requireSuccess), decltype(calls)>(buf, sizeof(buf), requireSuccess, calls, &err);
        if (!ok) {
          std::cerr << "encode_into failed: " << err.message << "\n";
          return false;
        }
        
        size_t encoded_size = TryAggregate::encoded_size(requireSuccess, calls);
        std::string encoded = "0x" + util::hex_lower(buf, encoded_size);
        
        // Compare with ethers.js output
        if (encoded != expected) {
          std::cerr << "encoding mismatch\n  expected: " << expected << "\n  got     : " << encoded << "\n";
          return false;
        }
        
        return true;
      })());

    RUN_TEST("multicall.aggregate3 encoding vs ethers.js",
      ([&](){
        // Test data: single call with address, bool, and bytes
        std::vector<std::tuple<std::array<uint8_t,20>, bool, std::vector<uint8_t>>> calls{
          {util::addr_from_hex("0x2222222222222222222222222222222222222222"), true, {0x56, 0x78}}
        };
        
        // Get expected encoding from ethers.js
        const char* dir = std::getenv("ABI_SCRIPTS_DIR");
        if (!dir) throw std::runtime_error("ABI_SCRIPTS_DIR not set");
        
        std::string cmd = std::string("node ") + dir + "/encode_multicall.mjs aggregate3 \"[[\\\"0x2222222222222222222222222222222222222222\\\",true,\\\"0x5678\\\"]]\"";
        std::string expected = util::run_node_and_capture(cmd);
        
        // Encode with our C++ codec
        uint8_t buf[2048];
        abi::Error err;
        bool ok = Aggregate3::template encode_into<decltype(calls)>(buf, sizeof(buf), calls, &err);
        if (!ok) {
          std::cerr << "encode_into failed: " << err.message << "\n";
          return false;
        }
        
        size_t encoded_size = Aggregate3::encoded_size(calls);
        std::string encoded = "0x" + util::hex_lower(buf, encoded_size);
        
        // Compare with ethers.js output
        if (encoded != expected) {
          std::cerr << "encoding mismatch\n  expected: " << expected << "\n  got     : " << encoded << "\n";
          return false;
        }
        
        return true;
      })());
  }
 */
  std::cout << "\n== exhaustive value round-trips ==\n";
  {

    using abi_test::roundtrip_value;
    using boost::multiprecision::cpp_int;

    // 1) Scalars
    RUN_TEST("bool true",  (roundtrip_value<abi::bool_t>(true)));
    RUN_TEST("bool false", (roundtrip_value<abi::bool_t>(false)));

    RUN_TEST("uint256 small", (roundtrip_value<abi::uint_t<256>>(cpp_int("12345678901234567890"))));
    RUN_TEST("uint256 max-64bit", (roundtrip_value<abi::uint_t<256>>(cpp_int("18446744073709551615"))));
    RUN_TEST("int24 zero",  (roundtrip_value<abi::int_t<24>>(cpp_int(0))));
    RUN_TEST("int24 positive", (roundtrip_value<abi::int_t<24>>(cpp_int(0x7FFFFF))));
    RUN_TEST("int24 negative", (roundtrip_value<abi::int_t<24>>(cpp_int(-1)))); // sign-extend path

    std::array<uint8_t,20> addrA = util::addr_from_hex("0x0000000000000000000000000000000000000001");
    std::array<uint8_t,20> addrB = util::addr_from_hex("0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48");
    RUN_TEST("address #1", (roundtrip_value<abi::address20>(addrA)));
    RUN_TEST("address USDC", (roundtrip_value<abi::address20>(addrB)));

    RUN_TEST("bytes empty", (roundtrip_value<abi::bytes>(std::vector<uint8_t>{})));
    RUN_TEST("bytes non-32 multiple", (roundtrip_value<abi::bytes>(std::vector<uint8_t>{0xAA,0xBB,0xCC,0xDD,0xEE})));

    RUN_TEST("string empty", (roundtrip_value<abi::string_t>(std::string{})));
    RUN_TEST("string ascii", (roundtrip_value<abi::string_t>(std::string("hello, abi"))));
    RUN_TEST("string utf8",  (roundtrip_value<abi::string_t>(std::string(u8"καλημέρα 🚀"))));

    // 2) Static arrays
    using UA3 = abi::static_array<abi::uint_t<256>, 3>;
    RUN_TEST("static_array<uint256,3>",
      (roundtrip_value<UA3>(std::array<cpp_int,3>{cpp_int(1), cpp_int(2), cpp_int(3)})));

    using BA2 = abi::static_array<abi::bytes, 2>; // array of dynamic → overall dynamic
    RUN_TEST("static_array<bytes,2>",
      (roundtrip_value<BA2>(std::array<std::vector<uint8_t>,2>{
        std::vector<uint8_t>{}, std::vector<uint8_t>{0x01,0x02,0x03}
      })));

    // 3) Dynamic arrays (static element type)
    using DU = abi::dyn_array<abi::uint_t<256>>;
    RUN_TEST("dyn_array<uint256> len=0", (roundtrip_value<DU>(std::vector<cpp_int>{})));
    RUN_TEST("dyn_array<uint256> len=1", (roundtrip_value<DU>(std::vector<cpp_int>{cpp_int("42")})));
    RUN_TEST("dyn_array<uint256> len=3",
      (roundtrip_value<DU>(std::vector<cpp_int>{cpp_int("100"), cpp_int("200"), cpp_int("300")})));

    // 4) Dynamic arrays (dynamic element type)
    using DB = abi::dyn_array<abi::bytes>;
    RUN_TEST("dyn_array<bytes> len=0", (roundtrip_value<DB>(std::vector<std::vector<uint8_t>>{})));
    RUN_TEST("dyn_array<bytes> len=2",
      (roundtrip_value<DB>(std::vector<std::vector<uint8_t>>{
        std::vector<uint8_t>{0xAA,0xBB},
        std::vector<uint8_t>{}
      })));

    // 5) Tuples (static-only)
    using TupStatic = abi::tuple<abi::uint_t<256>, abi::bool_t, abi::address20>;
    RUN_TEST("tuple<uint256,bool,address>",
      (roundtrip_value<TupStatic>(std::make_tuple(
        cpp_int("123456789"),
        true,
        addrB
      ))));

    // 6) Tuples (contains dynamic)
    using TupDyn1 = abi::tuple<abi::uint_t<256>, abi::address20, abi::bool_t, abi::bytes>;
    RUN_TEST("tuple<uint256,address,bool,bytes>",
      (roundtrip_value<TupDyn1>(std::make_tuple(
        cpp_int("123456789"),
        addrA,
        true,
        std::vector<uint8_t>{0xAA,0xBB,0xCC}
      ))));

    using TupDyn2 = abi::tuple<abi::bool_t, abi::bytes>; // classic (bool,bytes)
    RUN_TEST("tuple<bool,bytes> (short bytes)",
      (roundtrip_value<TupDyn2>(std::make_tuple(true, std::vector<uint8_t>{0x01}))));

    // 7) Arrays of tuples (static tuple)
    using AT_StaticTup = abi::dyn_array<abi::tuple<abi::uint_t<256>, abi::bool_t>>;
    RUN_TEST("dyn_array<tuple<uint256,bool>> len=3",
      (roundtrip_value<AT_StaticTup>(std::vector<std::tuple<cpp_int,bool>>{
        {cpp_int("100"), true},
        {cpp_int("200"), false},
        {cpp_int("300"), true}
      })));

    // 8) Arrays of tuples (dynamic tuple)
    using AT_DynTup = abi::dyn_array<abi::tuple<abi::bool_t, abi::bytes>>;
    RUN_TEST("dyn_array<tuple<bool,bytes>> len=2",
      (roundtrip_value<AT_DynTup>(std::vector<std::tuple<bool,std::vector<uint8_t>>>{
        {true,  std::vector<uint8_t>{0xDE,0xAD,0xBE,0xEF}},
        {false, std::vector<uint8_t>{}}
      })));

    // 9) Tuple containing arrays
    using TupWithArrays = abi::tuple<
      abi::dyn_array<abi::uint_t<256>>,            // dynamic
      abi::static_array<abi::address20, 2>         // static (but overall tuple dynamic)
    >;
    RUN_TEST("tuple<dyn_array<uint256>, static_array<address,2>>",
      (roundtrip_value<TupWithArrays>(std::make_tuple(
        std::vector<cpp_int>{cpp_int(1), cpp_int(2), cpp_int(3)},
        std::array<std::array<uint8_t,20>,2>{ addrA, addrB }
      ))));

    // 10) Nested tuples
    using Inner = abi::tuple<abi::uint_t<256>, abi::bool_t>;
    using Outer = abi::tuple<Inner, abi::bytes>;  // dynamic overall
    RUN_TEST("tuple<tuple<uint256,bool>, bytes>",
      (roundtrip_value<Outer>(std::make_tuple(
        std::make_tuple(cpp_int("424242"), true),
        std::vector<uint8_t>{0x11,0x22,0x33,0x44}
      ))));

    // 11) Tuple of dynamic array of dynamic tuples (stress typical multicall3 return)
    using CallRes  = abi::tuple<abi::bool_t, abi::bytes>;
    using CallsRes = abi::dyn_array<CallRes>;
    RUN_TEST("dyn_array<tuple<bool,bytes>> empty",
      (roundtrip_value<CallsRes>(std::vector<std::tuple<bool,std::vector<uint8_t>>>{})));
    RUN_TEST("dyn_array<tuple<bool,bytes>> typical",
      (roundtrip_value<CallsRes>(std::vector<std::tuple<bool,std::vector<uint8_t>>>{
        {true,  std::vector<uint8_t>(32, 0)},                   // 32-byte zero
        {true,  std::vector<uint8_t>{ std::vector<uint8_t>(12,0).begin(), std::vector<uint8_t>(12,0).end() }}, // trivial
        {false, std::vector<uint8_t>{0x01,0x02}}
      })));

    // 12) Static array of dynamic tuples
    using SADT = abi::static_array<abi::tuple<abi::bool_t, abi::bytes>, 2>;
    RUN_TEST("static_array<tuple<bool,bytes>,2>",
      (roundtrip_value<SADT>(std::array<std::tuple<bool,std::vector<uint8_t>>,2>{
        std::make_tuple(true,  std::vector<uint8_t>{0xAA}),
        std::make_tuple(false, std::vector<uint8_t>{})
      })));

    // 13) Tuples mixing string & bytes
    using MixSB = abi::tuple<abi::string_t, abi::bytes, abi::bool_t>;
    RUN_TEST("tuple<string,bytes,bool>",
      (roundtrip_value<MixSB>(std::make_tuple(
        std::string("hello"),
        std::vector<uint8_t>{0,1,2,3,4,5,6},
        true
      ))));

    // 14) Deep nesting: tuple<dyn_array<tuple<bool,bytes>>, string>
    using Deep = abi::tuple<abi::dyn_array<abi::tuple<abi::bool_t, abi::bytes>>, abi::string_t>;
    RUN_TEST("tuple<dyn_array<tuple<bool,bytes>>,string>",
      (roundtrip_value<Deep>(std::make_tuple(
        std::vector<std::tuple<bool,std::vector<uint8_t>>>{
          {true,  std::vector<uint8_t>{0xCA,0xFE}},
          {false, std::vector<uint8_t>{}}
        },
        std::string("ok")
      ))));
  }

  
  // ─────────────────────────────────────────────────────────────────────────────
  // Edge-case value round-trips & failure-path tests
  // ─────────────────────────────────────────────────────────────────────────────
  {
    using abi_test::roundtrip_value;
    using boost::multiprecision::cpp_int;

    // ---------- Scalars: signed/unsigned edges ----------
    RUN_TEST("int8 min",  (roundtrip_value<abi::int_t<8>>(cpp_int(-128))));
    RUN_TEST("int8 max",  (roundtrip_value<abi::int_t<8>>(cpp_int(127))));
    RUN_TEST("int24 min", (roundtrip_value<abi::int_t<24>>(cpp_int(- (1<<23) ))));
    RUN_TEST("int24 max", (roundtrip_value<abi::int_t<24>>(cpp_int(  (1<<23)-1 ))));
    RUN_TEST("int256 min-ish", (roundtrip_value<abi::int_t<256>>(cpp_int(1) << 200)));   // still fits; sign handled
    RUN_TEST("uint256 topbit", (roundtrip_value<abi::uint_t<256>>((cpp_int(1) << 255) + 123)));

    // Zero/one sanity across widths
    RUN_TEST("uint256 zero", (roundtrip_value<abi::uint_t<256>>(cpp_int(0))));
    RUN_TEST("uint256 one",  (roundtrip_value<abi::uint_t<256>>(cpp_int(1))));
    RUN_TEST("int256 -1",    (roundtrip_value<abi::int_t<256>>(cpp_int(-1))));

    // ---------- bytes/string 32-byte boundaries ----------
    RUN_TEST("bytes len=0",  (roundtrip_value<abi::bytes>(std::vector<uint8_t>{})));
    RUN_TEST("bytes len=31", (roundtrip_value<abi::bytes>(std::vector<uint8_t>(31, 0xAB))));
    RUN_TEST("bytes len=32", (roundtrip_value<abi::bytes>(std::vector<uint8_t>(32, 0xCD))));
    RUN_TEST("bytes len=33", (roundtrip_value<abi::bytes>(std::vector<uint8_t>(33, 0xEF))));
    RUN_TEST("bytes len=64", (roundtrip_value<abi::bytes>(std::vector<uint8_t>(64, 0x11))));

    RUN_TEST("string len=0",  (roundtrip_value<abi::string_t>(std::string{})));
    RUN_TEST("string len=31", (roundtrip_value<abi::string_t>(std::string(31, 'x'))));
    RUN_TEST("string len=32", (roundtrip_value<abi::string_t>(std::string(32, 'y'))));
    RUN_TEST("string len=33", (roundtrip_value<abi::string_t>(std::string(33, 'z'))));
    RUN_TEST("string utf8 32/33", (roundtrip_value<abi::string_t>(std::string(u8"🚀") + std::string(31, 'a'))));

    // ---------- Tuples static/dynamic shapes ----------
    using TupStatic = abi::tuple<abi::uint_t<256>, abi::uint_t<256>, abi::bool_t>;
    RUN_TEST("tuple(static multi-word + bool)",
      (roundtrip_value<TupStatic>(std::make_tuple(cpp_int(1), cpp_int(2), true))));

    using TupDynFirst = abi::tuple<abi::bytes, abi::uint_t<256>>;
    using TupDynLast  = abi::tuple<abi::uint_t<256>, abi::bytes>;
    RUN_TEST("tuple(bytes,uint256)",
      (roundtrip_value<TupDynFirst>(std::make_tuple(std::vector<uint8_t>{0xAA,0xBB}, cpp_int(42)))));
    RUN_TEST("tuple(uint256,bytes)",
      (roundtrip_value<TupDynLast>(std::make_tuple(cpp_int(42), std::vector<uint8_t>{0xCC}))));

    using TupAllDyn = abi::tuple<abi::bytes, abi::string_t>;
    RUN_TEST("tuple(bytes,string) top-level dynamic",
      (roundtrip_value<TupAllDyn>(std::make_tuple(std::vector<uint8_t>{1,2,3}, std::string("hi")))));

    // ---------- Arrays: static element w/ stride > 1 ----------
    using AT_Static = abi::dyn_array<abi::tuple<abi::uint_t<256>, abi::bool_t>>; // stride=64
    RUN_TEST("dyn_array<tuple<uint256,bool>> len=1",
      (roundtrip_value<AT_Static>(std::vector<std::tuple<cpp_int,bool>>{ {cpp_int(7), true} })));
    RUN_TEST("dyn_array<tuple<uint256,bool>> len=5",
      (roundtrip_value<AT_Static>(std::vector<std::tuple<cpp_int,bool>>{
        {cpp_int(1),false},{cpp_int(2),true},{cpp_int(3),false},{cpp_int(4),true},{cpp_int(5),false}
      })));

    // ---------- Arrays: dynamic elements (ragged) ----------
    using AD_bytes = abi::dyn_array<abi::bytes>;
    RUN_TEST("dyn_array<bytes> ragged",
      (roundtrip_value<AD_bytes>(std::vector<std::vector<uint8_t>>{
        {}, {0x01}, std::vector<uint8_t>(33,0xEE), std::vector<uint8_t>(64,0xFF)
      })));

    using AD_dynT = abi::dyn_array<abi::tuple<abi::bool_t, abi::bytes>>;
    RUN_TEST("dyn_array<tuple<bool,bytes>> ragged",
      (roundtrip_value<AD_dynT>(std::vector<std::tuple<bool,std::vector<uint8_t>>>{
        {true, {}}, {true, std::vector<uint8_t>(32,0)}, {false, std::vector<uint8_t>(33,0xAB)}
      })));

    // ---------- static_array with dynamic element (head=offsets) ----------
    using SA_bytes2 = abi::static_array<abi::bytes, 3>;
    RUN_TEST("static_array<bytes,3> ragged",
      (roundtrip_value<SA_bytes2>(std::array<std::vector<uint8_t>,3>{
        std::vector<uint8_t>{}, std::vector<uint8_t>{0x01,0x02}, std::vector<uint8_t>(33,0xCC)
      })));

    using SADT2 = abi::static_array<abi::tuple<abi::bool_t, abi::bytes>, 3>;
    RUN_TEST("static_array<tuple<bool,bytes>,3>",
      (roundtrip_value<SADT2>(std::array<std::tuple<bool,std::vector<uint8_t>>,3>{
        std::make_tuple(true,  std::vector<uint8_t>{0xAA}),
        std::make_tuple(false, std::vector<uint8_t>{}),
        std::make_tuple(true,  std::vector<uint8_t>(64,0x11))
      })));

    // ---------- Tuple containing arrays (mixed) ----------
    using TupWithArrays = abi::tuple<
      abi::dyn_array<abi::uint_t<256>>,
      abi::static_array<abi::address20, 2>
    >;
    auto addrA = util::addr_from_hex("0x0000000000000000000000000000000000000001");
    auto addrB = util::addr_from_hex("0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48");
    RUN_TEST("tuple<dyn_array<uint256>, static_array<address,2>>",
      (roundtrip_value<TupWithArrays>(std::make_tuple(
        std::vector<cpp_int>{ cpp_int(1), cpp_int(2), cpp_int(3) },
        std::array<std::array<uint8_t,20>,2>{ addrA, addrB }
      ))));

    // ---------- Deep nesting ----------
    using Inner = abi::tuple<abi::uint_t<256>, abi::bool_t>;
    using Outer = abi::tuple<Inner, abi::bytes>;
    RUN_TEST("tuple<tuple<uint256,bool>,bytes>",
      (roundtrip_value<Outer>(std::make_tuple(
        std::make_tuple(cpp_int("424242"), true),
        std::vector<uint8_t>{0x11,0x22,0x33,0x44}
      ))));

    using Deep = abi::tuple<abi::dyn_array<abi::tuple<abi::bool_t, abi::bytes>>, abi::string_t>;
    RUN_TEST("tuple<dyn_array<tuple<bool,bytes>>,string> ragged",
      (roundtrip_value<Deep>(std::make_tuple(
        std::vector<std::tuple<bool,std::vector<uint8_t>>>{
          {true,  std::vector<uint8_t>{0xCA,0xFE}},
          {false, std::vector<uint8_t>{}},
          {true,  std::vector<uint8_t>(33,0x77)}
        },
        std::string("ok")
      ))));

    // ---------- Failure-path: corrupted offsets (should not crash) ----------
    // 1) dyn_array<dynamic>: off1 < off0 (non-monotonic)
    RUN_TEST("dyn_array<dynamic> corrupted (non-monotonic offsets) fails gracefully",
      ([&](){
        // Construct: len=2, offs [0x60, 0x40] (backwards). Minimal well-formed heads/tails otherwise.
        // Layout: [len][off0][off1]  then tails blob (unused here)
        std::vector<uint8_t> blob(32 + 32*2, 0);
        // len = 2
        abi::write_u256_imm(blob.data()+0, 2);
        // off0 = 0x60, off1 = 0x40  (relative to +32)
        abi::write_u256_imm(blob.data()+32, 0x60);
        abi::write_u256_imm(blob.data()+64, 0x40);
        using Bad = abi::dyn_array<abi::bytes>;
        typename abi::value_of<Bad>::type out;
        abi::Error err;
        return !abi::traits<Bad>::decode(abi::BytesSpan(blob.data(), blob.size()), out, &err);
      })());

    // 2) tuple(dynamic) with off < head (invalid)
    RUN_TEST("tuple<dynamic> corrupted (off < head) fails gracefully",
      ([&](){
        // tuple<bool,bytes> head is 0x40; we set offset to 0x20
        std::vector<uint8_t> tup(64, 0); // head only
        // bool = 1 at [31]
        tup[31] = 1;
        // offset=0x20 at [32..63]
        abi::write_u256_imm(tup.data()+32, 0x20);
        using TD = abi::tuple<abi::bool_t, abi::bytes>;
        typename abi::value_of<TD>::type out;
        abi::Error err;
        return !abi::traits<TD>::decode(abi::BytesSpan(tup.data(), tup.size()), out, &err);
      })());

    // 3) top-level dynamic tuple: missing deref (simulate caller mistake)
    RUN_TEST("top-level dynamic tuple must deref (decode_from handles it)",
      ([&](){
        using TD = abi::tuple<abi::bool_t, abi::bytes>;
        auto val = std::make_tuple(true, std::vector<uint8_t>{0xAA,0xBB});
        // encode as value (will produce top pointer + body)
        size_t need = abi::encoded_size_data<TD>(val);
        std::vector<uint8_t> buf(need, 0);
        abi::Error err;
        if(!abi::encode_data_into<TD>(buf.data(), buf.size(), val, &err)) return false;

        // 3a) Proper path: decode_from (should succeed)
        typename abi::value_of<TD>::type out{};
        if(!abi::decode_from<TD>(abi::BytesSpan(buf.data(), buf.size()), out, &err)) return false;
        if(!(out==val)) return false;

        // 3b) Improper path: call traits<TD>::decode directly on whole blob (should fail)
        typename abi::value_of<TD>::type out_bad{};
        return !abi::traits<TD>::decode(abi::BytesSpan(buf.data(), buf.size()), out_bad, &err);
      })());

    // ---------- Encode undersized buffer must fail ----------
    RUN_TEST("encode buffer too small (bytes 33)",
      ([&](){
        using Sch = abi::bytes;
        std::vector<uint8_t> v(33, 0xEE);
        abi::Error err;
        uint8_t tiny[64]; // definitely too small (needs 32 + pad(33)=64 → actually 96 total with head)
        return !abi::encode_data_into<Sch>(tiny, sizeof(tiny), v, &err);
      })());
  }
  
  std::cout << "\nSummary: " << counts.passed << " / " << counts.total << " passed\n";
  return (counts.passed == counts.total) ? 0 : 1;

} catch (const std::exception& e) {
  std::cerr << "Fatal: " << e.what() << "\n";
  return 1;
}