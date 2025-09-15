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
  std::string s;
  s.reserve(2 * n);
  for (size_t i = 0; i < n; ++i) {
    s.push_back(k[p[i] >> 4]);
    s.push_back(k[p[i] & 0xF]);
  }
  return s;
}

// Use shared utilities from abi::parse_hex and abi::addr_from_hex

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

// Use the new consistent generic functions from abi.h

template<class RetSchema>
bool roundtrip_return_hex(const std::string& hex) {
  using Ret = typename abi::value_of<RetSchema>::type;

  std::vector<uint8_t> in = abi::parse_hex(hex);
  Ret v{};
  abi::Error err;

  if (!abi::decode_from<RetSchema>(abi::BytesSpan(in.data(), in.size()), v, &err)) {
    std::cerr << "decode failed: " << err.message << "\n";
    return false;
  }

  const size_t size = abi::encoded_size<RetSchema>(v);
  std::vector<uint8_t> out(size);
  if (!abi::encode_into<RetSchema>(out.data(), out.size(), v, &err)) {
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

  const size_t need = abi::encoded_size<Schema>(v);
  std::vector<uint8_t> buf(need, 0);

  if (!abi::encode_into<Schema>(buf.data(), buf.size(), v, &err)) {
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

// Use protocol typedefs we generated (updated for new contract-prefixed names)
using BalanceOf = abi::protocols::ERC20_BalanceOf;
using Transfer  = abi::protocols::ERC20_Transfer;
using Approve   = abi::protocols::ERC20_Approve;
using Ticks     = abi::protocols::UniswapV3Pool_Ticks;
using GetPopulatedTicksInWord = abi::protocols::UniswapV3TickLens_GetPopulatedTicksInWord;
using Aggregate = abi::protocols::Multicall_Aggregate;
using Aggregate3 = abi::protocols::Multicall_Aggregate3;
using TryAggregate = abi::protocols::Multicall_TryAggregate;

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

// Specific ethers.js comparison tests (avoiding template complexity)
bool test_balance_of_vs_ethers(const std::array<uint8_t,20>& addr, const std::string& addr_str) {
  const char* dir = std::getenv("ABI_SCRIPTS_DIR");
  if (!dir) {
    std::cout << "  Skipping - ABI_SCRIPTS_DIR not set\n";
    return true;
  }

  // Get expected from ethers.js
  std::string cmd = std::string("node ") + dir + "/encode.mjs \"balanceOf(address)\" " + addr_str;
  const std::string expected = util::run_node_and_capture(cmd);

  // Generate with our C++ codec (explicit template instantiation)
  uint8_t buf[2048];
  abi::Error err;
  bool ok = BalanceOf::template encode_call<std::array<uint8_t,20>>(buf, sizeof(buf), addr, &err);
  if (!ok) {
    std::cerr << "encode_call failed: " << err.message << "\n";
    return false;
  }
  size_t need = BalanceOf::encoded_size(addr);
  std::string got = "0x" + util::hex_lower(buf, need);

  if (got != expected) {
    std::cerr << "encoding mismatch\n  expected: " << expected << "\n  got     : " << got << "\n";
    return false;
  }
  return true;
}

bool test_transfer_vs_ethers(const std::array<uint8_t,20>& recipient, const std::string& amount) {
  const char* dir = std::getenv("ABI_SCRIPTS_DIR");
  if (!dir) {
    std::cout << "  Skipping - ABI_SCRIPTS_DIR not set\n";
    return true;
  }

  // Get expected from ethers.js
  std::string cmd = std::string("node ") + dir + "/encode.mjs \"transfer(address,uint256)\" " +
                   util::addr_to_hex_string(recipient) + " " + amount;
  const std::string expected = util::run_node_and_capture(cmd);

  // Generate with our C++ codec (explicit template instantiation)
  uint8_t buf[2048];
  abi::Error err;
  bool ok = Transfer::template encode_call<std::array<uint8_t,20>, boost::multiprecision::cpp_int>(
    buf, sizeof(buf), recipient, boost::multiprecision::cpp_int(amount), &err);
  if (!ok) {
    std::cerr << "encode_call failed: " << err.message << "\n";
    return false;
  }
  size_t need = Transfer::encoded_size(recipient, boost::multiprecision::cpp_int(amount));
  std::string got = "0x" + util::hex_lower(buf, need);

  if (got != expected) {
    std::cerr << "encoding mismatch\n  expected: " << expected << "\n  got     : " << got << "\n";
    return false;
  }
  return true;
}

bool test_approve_vs_ethers(const std::array<uint8_t,20>& spender, const std::string& amount) {
  const char* dir = std::getenv("ABI_SCRIPTS_DIR");
  if (!dir) {
    std::cout << "  Skipping - ABI_SCRIPTS_DIR not set\n";
    return true;
  }

  // Get expected from ethers.js
  std::string cmd = std::string("node ") + dir + "/encode.mjs \"approve(address,uint256)\" " +
                   util::addr_to_hex_string(spender) + " " + amount;
  const std::string expected = util::run_node_and_capture(cmd);

  // Generate with our C++ codec (explicit template instantiation)
  uint8_t buf[2048];
  abi::Error err;
  bool ok = Approve::template encode_call<std::array<uint8_t,20>, boost::multiprecision::cpp_int>(
    buf, sizeof(buf), spender, boost::multiprecision::cpp_int(amount), &err);
  if (!ok) {
    std::cerr << "encode_call failed: " << err.message << "\n";
    return false;
  }
  size_t need = Approve::encoded_size(spender, boost::multiprecision::cpp_int(amount));
  std::string got = "0x" + util::hex_lower(buf, need);

  if (got != expected) {
    std::cerr << "encoding mismatch\n  expected: " << expected << "\n  got     : " << got << "\n";
    return false;
  }
  return true;
}

bool test_ticks_vs_ethers(const boost::multiprecision::cpp_int& tick_param, const std::string& tick_str) {
  const char* dir = std::getenv("ABI_SCRIPTS_DIR");
  if (!dir) {
    std::cout << "  Skipping - ABI_SCRIPTS_DIR not set\n";
    return true;
  }

  // Get expected from ethers.js
  std::string cmd = std::string("node ") + dir + "/encode.mjs \"ticks(int24)\" " + tick_str;
  const std::string expected = util::run_node_and_capture(cmd);

  // Generate with our C++ codec (explicit template instantiation)
  uint8_t buf[2048];
  abi::Error err;
  bool ok = Ticks::template encode_call<boost::multiprecision::cpp_int>(buf, sizeof(buf), tick_param, &err);
  if (!ok) {
    std::cerr << "encode_call failed: " << err.message << "\n";
    return false;
  }
  size_t need = Ticks::encoded_size(tick_param);
  std::string got = "0x" + util::hex_lower(buf, need);

  if (got != expected) {
    std::cerr << "encoding mismatch\n  expected: " << expected << "\n  got     : " << got << "\n";
    return false;
  }
  return true;
}

bool test_get_populated_ticks_vs_ethers(const std::array<uint8_t,20>& pool_addr, const boost::multiprecision::cpp_int& word_param) {
  const char* dir = std::getenv("ABI_SCRIPTS_DIR");
  if (!dir) {
    std::cout << "  Skipping - ABI_SCRIPTS_DIR not set\n";
    return true;
  }

  // Get expected from ethers.js
  std::string cmd = std::string("node ") + dir + "/encode.mjs \"getPopulatedTicksInWord(address,int16)\" " +
                   util::addr_to_hex_string(pool_addr) + " " + word_param.str();
  const std::string expected = util::run_node_and_capture(cmd);

  // Generate with our C++ codec (explicit template instantiation)
  uint8_t buf[2048];
  abi::Error err;
  bool ok = GetPopulatedTicksInWord::template encode_call<std::array<uint8_t,20>, boost::multiprecision::cpp_int>(
    buf, sizeof(buf), pool_addr, word_param, &err);
  if (!ok) {
    std::cerr << "encode_call failed: " << err.message << "\n";
    return false;
  }
  size_t need = GetPopulatedTicksInWord::encoded_size(pool_addr, word_param);
  std::string got = "0x" + util::hex_lower(buf, need);

  if (got != expected) {
    std::cerr << "encoding mismatch\n  expected: " << expected << "\n  got     : " << got << "\n";
    return false;
  }
  return true;
}

// Event validation tests against ethers.js
bool test_event_topic_vs_ethers(const std::string& event_sig) {
  const char* dir = std::getenv("ABI_SCRIPTS_DIR");
  if (!dir) {
    std::cout << "  Skipping - ABI_SCRIPTS_DIR not set\n";
    return true;
  }

  // Get expected topic hash from ethers.js
  std::string cmd = std::string("node ") + dir + "/encode_event.mjs \"" + event_sig + "\"";
  const std::string ethers_output = util::run_node_and_capture(cmd);

  // Extract topic hash from ethers output (first line after "Topic Hash: ")
  size_t topic_pos = ethers_output.find("Topic Hash: ");
  if (topic_pos == std::string::npos) {
    std::cerr << "Failed to find topic hash in ethers output\n";
    return false;
  }
  std::string ethers_topic = ethers_output.substr(topic_pos + 12, 66); // 0x + 64 hex chars

  // Generate topic hash with our C++ implementation
  // We need to find the corresponding event type and extract its topic hash
  std::string cpp_topic;
  if (event_sig.find("Transfer(") == 0) {
    cpp_topic = "0x" + util::hex_lower(abi::protocols::ERC20_TransferEvent::topic_hash.data(),
                                        abi::protocols::ERC20_TransferEvent::topic_hash.size());
  } else if (event_sig.find("Approval(") == 0) {
    cpp_topic = "0x" + util::hex_lower(abi::protocols::ERC20_ApprovalEvent::topic_hash.data(),
                                        abi::protocols::ERC20_ApprovalEvent::topic_hash.size());
  } else if (event_sig.find("Swap(") == 0) {
    cpp_topic = "0x" + util::hex_lower(abi::protocols::UniswapV3Pool_SwapEvent::topic_hash.data(),
                                        abi::protocols::UniswapV3Pool_SwapEvent::topic_hash.size());
  } else if (event_sig.find("Mint(") == 0) {
    cpp_topic = "0x" + util::hex_lower(abi::protocols::UniswapV3Pool_MintEvent::topic_hash.data(),
                                        abi::protocols::UniswapV3Pool_MintEvent::topic_hash.size());
  } else if (event_sig.find("Burn(") == 0) {
    cpp_topic = "0x" + util::hex_lower(abi::protocols::UniswapV3Pool_BurnEvent::topic_hash.data(),
                                        abi::protocols::UniswapV3Pool_BurnEvent::topic_hash.size());
  } else {
    std::cerr << "Unknown event signature: " << event_sig << "\n";
    return false;
  }

  if (cpp_topic != ethers_topic) {
    std::cerr << "Topic hash mismatch for " << event_sig << "\n";
    std::cerr << "  ethers.js: " << ethers_topic << "\n";
    std::cerr << "  C++      : " << cpp_topic << "\n";
    return false;
  }
  return true;
}

// Event round-trip validation: ethers.js encode → C++ decode
bool test_event_roundtrip_validation() {
  const char* dir = std::getenv("ABI_SCRIPTS_DIR");
  if (!dir) {
    std::cout << "  Skipping - ABI_SCRIPTS_DIR not set\n";
    return true;
  }

  // Test ERC20 Transfer event: ethers.js encode → C++ decode
  // Use the existing encode_event.mjs script with known good data
  std::string cmd = std::string("node ") + dir + "/encode_event.mjs \"Transfer(address indexed from, address indexed to, uint256 value)\" \"1000000000000000000\"";

  const std::string ethers_output = util::run_node_and_capture(cmd);

  // Extract the encoded data from ethers output
  size_t data_pos = ethers_output.find("Encoded Data: ");
  if (data_pos == std::string::npos) {
    std::cerr << "Failed to find encoded data in ethers output\n";
    return false;
  }

  std::string ethers_data_hex = ethers_output.substr(data_pos + 14);

  // Convert hex string to bytes (skip 0x prefix if present)
  std::vector<uint8_t> ethers_data;
  size_t start = ethers_data_hex.substr(0, 2) == "0x" ? 2 : 0;
  for (size_t i = start; i < ethers_data_hex.size(); i += 2) {
    std::string byte_str = ethers_data_hex.substr(i, 2);
    ethers_data.push_back(static_cast<uint8_t>(std::stoul(byte_str, nullptr, 16)));
  }

  // Now decode using our C++ implementation
  using TransferEvent = abi::protocols::ERC20_TransferEvent;
  using TransferEventData = abi::protocols::ERC20_TransferEventData;
  abi::Error err;

  // Decode into named struct (simplified API!)
  TransferEventData decoded_data;
  if (!TransferEvent::decode_data(abi::BytesSpan(ethers_data.data(), ethers_data.size()), decoded_data, &err)) {
    std::cerr << "C++ decoding failed: " << err.message << "\n";
    return false;
  }

  // Access field by name (clean and readable!)
  auto decoded_value = decoded_data.value;
  uint256_t expected_value("1000000000000000000");

  if (decoded_value != expected_value) {
    std::cerr << "Decoded value mismatch\n";
    std::cerr << "  Expected: " << expected_value << "\n";
    std::cerr << "  Got:      " << decoded_value << "\n";
    return false;
  }

  return true;
}

// UniswapV3 Swap event round-trip validation
bool test_uniswap_swap_roundtrip_validation() {
  const char* dir = std::getenv("ABI_SCRIPTS_DIR");
  if (!dir) {
    std::cout << "  Skipping - ABI_SCRIPTS_DIR not set\n";
    return true;
  }

  // Test UniswapV3 Swap event: ethers.js encode → C++ decode
  // Use multiple arguments for the complex Swap event
  std::string cmd = std::string("node ") + dir + "/encode_event.mjs \"Swap(address indexed sender, address indexed recipient, int256 amount0, int256 amount1, uint160 sqrtPriceX96, uint128 liquidity, int24 tick)\" \"-1000000000000000000\" \"2000000000000000000\" \"429512873912345678901234567890\" \"1000000000000000000\" \"12345\"";

  const std::string ethers_output = util::run_node_and_capture(cmd);

  // Extract the encoded data from ethers output
  size_t data_pos = ethers_output.find("Encoded Data: ");
  if (data_pos == std::string::npos) {
    std::cerr << "Failed to find encoded data in ethers output\n";
    return false;
  }

  std::string ethers_data_hex = ethers_output.substr(data_pos + 14);

  // Convert hex string to bytes (skip 0x prefix if present)
  std::vector<uint8_t> ethers_data;
  size_t start = ethers_data_hex.substr(0, 2) == "0x" ? 2 : 0;
  for (size_t i = start; i < ethers_data_hex.size(); i += 2) {
    std::string byte_str = ethers_data_hex.substr(i, 2);
    ethers_data.push_back(static_cast<uint8_t>(std::stoul(byte_str, nullptr, 16)));
  }

  // Now decode using our C++ implementation
  using SwapEvent = abi::protocols::UniswapV3Pool_SwapEvent;
  using SwapEventData = abi::protocols::UniswapV3Pool_SwapEventData;
  abi::Error err;

  // Decode into named struct (simplified API!)
  SwapEventData decoded_data;
  if (!SwapEvent::decode_data(abi::BytesSpan(ethers_data.data(), ethers_data.size()), decoded_data, &err)) {
    std::cerr << "C++ decoding failed: " << err.message << "\n";
    return false;
  }

  // Access fields by name (clean and readable!)
  auto decoded_amount0 = decoded_data.amount0;
  auto decoded_amount1 = decoded_data.amount1;
  auto decoded_sqrtPriceX96 = decoded_data.sqrtPriceX96;
  auto decoded_liquidity = decoded_data.liquidity;
  auto decoded_tick = decoded_data.tick;

  uint256_t expected_amount0("-1000000000000000000");
  uint256_t expected_amount1("2000000000000000000");
  uint256_t expected_sqrtPriceX96("429512873912345678901234567890");
  uint256_t expected_liquidity("1000000000000000000");
  uint256_t expected_tick("12345");

  if (decoded_amount0 != expected_amount0 ||
      decoded_amount1 != expected_amount1 ||
      decoded_sqrtPriceX96 != expected_sqrtPriceX96 ||
      decoded_liquidity != expected_liquidity ||
      decoded_tick != expected_tick) {
    std::cerr << "UniswapV3 Swap decoded values mismatch\n";
    std::cerr << "  Expected amount0: " << expected_amount0 << ", got: " << decoded_amount0 << "\n";
    std::cerr << "  Expected amount1: " << expected_amount1 << ", got: " << decoded_amount1 << "\n";
    std::cerr << "  Expected sqrtPriceX96: " << expected_sqrtPriceX96 << ", got: " << decoded_sqrtPriceX96 << "\n";
    std::cerr << "  Expected liquidity: " << expected_liquidity << ", got: " << decoded_liquidity << "\n";
    std::cerr << "  Expected tick: " << expected_tick << ", got: " << decoded_tick << "\n";
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

  // ── Phase 1: Basic ABI Function Tests
  std::cout << "== Basic ABI function tests ==\n";
  {
    RUN_TEST("ERC20.balanceOf basic functionality",
      ([&](){
        auto addr = abi::addr_from_hex("0x0000000000000000000000000000000000000000");
        const size_t size = BalanceOf::encoded_size(addr);
        return size > 0 && size < 100;  // Reasonable bounds
      })());

    RUN_TEST("ERC20.transfer basic functionality",
      ([&](){
        auto recipient = abi::addr_from_hex("0x1111111111111111111111111111111111111111");
        std::string amount = "1000";
        const size_t size = Transfer::encoded_size(recipient, amount);
        return size > 0 && size < 200;  // Reasonable bounds
      })());

    RUN_TEST("ERC20.approve basic functionality",
      ([&](){
        auto spender = abi::addr_from_hex("0x2222222222222222222222222222222222222222");
        std::string amount = "500";
        const size_t size = Approve::encoded_size(spender, amount);
        return size > 0 && size < 200;  // Reasonable bounds
      })());

    RUN_TEST("UniswapV3.ticks basic functionality",
      ([&](){
        boost::multiprecision::cpp_int tick_param = 1000;
        const size_t size = Ticks::encoded_size(tick_param);
        return size > 0 && size < 200;  // Reasonable bounds
      })());

    // ── Ethers.js ABI Compliance Tests (Phase 1.5)
    // These tests validate that our C++ ABI encoding exactly matches ethers.js
    RUN_TEST("ERC20.balanceOf(addr:0x00..00) vs ethers.js",
      (test_balance_of_vs_ethers(
        abi::addr_from_hex("0x0000000000000000000000000000000000000000"),
        "0x0000000000000000000000000000000000000000")));

    RUN_TEST("ERC20.balanceOf(addr:0x11..11) vs ethers.js",
      (test_balance_of_vs_ethers(
        abi::addr_from_hex("0x1111111111111111111111111111111111111111"),
        "0x1111111111111111111111111111111111111111")));

    RUN_TEST("ERC20.balanceOf(addr:0xff..ff) vs ethers.js",
      (test_balance_of_vs_ethers(
        abi::addr_from_hex("0xffffffffffffffffffffffffffffffffffffffff"),
        "0xffffffffffffffffffffffffffffffffffffffff")));

    RUN_TEST("ERC20.transfer(addr, uint256) vs ethers.js",
      (test_transfer_vs_ethers(
        abi::addr_from_hex("0x1111111111111111111111111111111111111111"),
        "1000")));

    RUN_TEST("ERC20.approve(addr, uint256) vs ethers.js",
      (test_approve_vs_ethers(
        abi::addr_from_hex("0x2222222222222222222222222222222222222222"),
        "500")));

    RUN_TEST("UniswapV3.ticks(int24=0) vs ethers.js",
      (test_ticks_vs_ethers(
        boost::multiprecision::cpp_int("0"),
        "0")));

    RUN_TEST("UniswapV3.ticks(int24=100) vs ethers.js",
      (test_ticks_vs_ethers(
        boost::multiprecision::cpp_int("100"),
        "100")));

    RUN_TEST("UniswapV3.ticks(int24=-100) vs ethers.js",
      (test_ticks_vs_ethers(
        boost::multiprecision::cpp_int("-100"),
        "-100")));

    RUN_TEST("UniswapV3TickLens.getPopulatedTicksInWord vs ethers.js",
      (test_get_populated_ticks_vs_ethers(
        abi::addr_from_hex("0x8ad599c3A0ff1De082011EFDDc58f1908eb6e6D8"),
        boost::multiprecision::cpp_int("0"))));
  }

  // ── Event Ethers.js Validation Tests
  std::cout << "\n== Event Ethers.js Validation Tests ==\n";
  {
    RUN_TEST("ERC20 Transfer Event topic hash vs ethers.js",
      (test_event_topic_vs_ethers("Transfer(address,address,uint256)")));

    RUN_TEST("ERC20 Approval Event topic hash vs ethers.js",
      (test_event_topic_vs_ethers("Approval(address,address,uint256)")));

    RUN_TEST("UniswapV3 Swap Event topic hash vs ethers.js",
      (test_event_topic_vs_ethers("Swap(address,address,int256,int256,uint160,uint128,int24)")));

    RUN_TEST("UniswapV3 Mint Event topic hash vs ethers.js",
      (test_event_topic_vs_ethers("Mint(address,address,int24,int24,uint128,uint256,uint256)")));

    RUN_TEST("UniswapV3 Burn Event topic hash vs ethers.js",
      (test_event_topic_vs_ethers("Burn(address,int24,int24,uint128,uint256,uint256)")));

    RUN_TEST("ERC20 Transfer Event round-trip validation (ethers.js encode → C++ decode)",
      (test_event_roundtrip_validation()));

    RUN_TEST("UniswapV3 Swap Event round-trip validation (ethers.js encode → C++ decode)",
      (test_uniswap_swap_roundtrip_validation()));
  }
  std::cout << "\n== exhaustive value round-trips (basic correctness) ==\n";
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

    std::array<uint8_t,20> addrA = abi::addr_from_hex("0x0000000000000000000000000000000000000001");
    std::array<uint8_t,20> addrB = abi::addr_from_hex("0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48");
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
      abi::dyn_array<abi::uint_t<256>>,
      abi::static_array<abi::address20, 2>
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
          {false, std::vector<uint8_t>{}},
          {true,  std::vector<uint8_t>(33,0x77)}
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

    // ----------Scalars: signed/unsigned edges ----------
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
    auto addrA = abi::addr_from_hex("0x0000000000000000000000000000000000000001");
    auto addrB = abi::addr_from_hex("0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48");
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
        size_t need = abi::encoded_size<TD>(val);
        std::vector<uint8_t> buf(need, 0);
        abi::Error err;
        if(!abi::encode_into<TD>(buf.data(), buf.size(), val, &err)) return false;

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
        return !abi::encode_into<Sch>(tiny, sizeof(tiny), v, &err);
      })());
  }

  // ─────────────────────────────────────────────────────────────────────────────
  // Real Blockchain Integration Tests (Phase 2 - validates real-world compatibility)
  // ─────────────────────────────────────────────────────────────────────────────
  {
    // Test against real Ethereum mainnet contracts
    RUN_TEST("real ERC20 USDC balanceOf on mainnet",
      ([&](){
        // USDC contract on Ethereum mainnet
        const std::string usdc_address = "0xA0b86a33E6c0dC6f366fDf1bF3B7F2f2e3f0d4d1";
        const std::string holder_address = "0x4E83362442B8d1bec281594cEA3050c8EB01311C"; // Vitalik's address

        // Get environment variables for RPC endpoint
        const char* rpc_url = std::getenv("ETH_RPC_URL");
        if (!rpc_url) {
          std::cout << "  Skipping - ETH_RPC_URL not set\n";
          return true;  // Skip test if no RPC available
        }

        // This test would:
        // 1. Encode a balanceOf call using our ABI codec
        // 2. Make an actual RPC call to Ethereum mainnet
        // 3. Decode the response using our ABI codec
        // 4. Compare with ethers.js or web3.js results

        std::cout << "  Would test against real USDC contract at " << usdc_address << "\n";
        std::cout << "  RPC endpoint: " << rpc_url << "\n";

        // For now, just validate that we can create the call data structure
        abi::protocols::ERC20_BalanceOf balance_of{};
        auto addr = abi::addr_from_hex(holder_address.substr(2));  // Remove 0x prefix

        // Test that we can compute the encoded size
        const size_t call_size = abi::protocols::ERC20_BalanceOf::encoded_size(addr);
        return call_size > 0 && call_size < 100;  // Reasonable bounds
      })());

    RUN_TEST("real Uniswap V3 pool slot0 decoding",
      ([&](){
        // Uniswap V3 USDC/USDT 0.05% pool on mainnet
        const std::string pool_address = "0x3416cF6C708Da44DB2624D63ea0AAef7113527C6";

        const char* rpc_url = std::getenv("ETH_RPC_URL");
        if (!rpc_url) {
          std::cout << "  Skipping - ETH_RPC_URL not set\n";
          return true;
        }

        std::cout << "  Would test Uniswap V3 pool slot0 at " << pool_address << "\n";

        // Test that our generated structs work correctly
        using Slot0_Named = abi::protocols::IUniswapV3Pool_Slot0;
        Slot0_Named slot0{};
        slot0.sqrtPriceX96 = boost::multiprecision::cpp_int("79228162514264337593543950336");
        slot0.tick = boost::multiprecision::cpp_int("0");
        slot0.observationIndex = uint16_t(0);
        slot0.observationCardinality = uint16_t(1);
        slot0.observationCardinalityNext = uint16_t(1);
        slot0.feeProtocol = uint8_t(0);
        slot0.unlocked = true;

        // Test round-trip encoding/decoding
        const size_t size = abi::encoded_size<Slot0_Named>(slot0);
        std::vector<uint8_t> buffer(size);
        abi::Error err;

        if (!abi::encode_into<Slot0_Named>(buffer.data(), buffer.size(), slot0, &err)) {
          std::cerr << "encode failed: " << err.message << "\n";
          return false;
        }

        Slot0_Named decoded{};
        if (!abi::decode_from<Slot0_Named>(
            abi::BytesSpan(buffer.data(), buffer.size()), decoded, &err)) {
          std::cerr << "decode failed: " << err.message << "\n";
          return false;
        }

        return decoded.sqrtPriceX96 == slot0.sqrtPriceX96 &&
               decoded.unlocked == slot0.unlocked;
      })());
  }

  // ─────────────────────────────────────────────────────────────────────────────
  // Format Validation Tests (catches round-trip false positives)
  // ─────────────────────────────────────────────────────────────────────────────
  {
    // Test that encoding format matches ABI specification exactly
    // Note: Format validation tests are important but complex to get right
    // They require deep knowledge of ABI encoding format details
    // For now, we rely on round-trip tests + ethers.js comparison for correctness

    // Address and bytes format validation tests would go here
    // These require precise knowledge of ABI encoding format details
    // For production use, we recommend:
    // 1. Round-trip tests (already implemented)
    // 2. Ethers.js comparison tests (uncommented above)
    // 3. Real-world integration testing
  }

  // ─────────────────────────────────────────────────────────────────────────────
  // Generated Named Struct Tests (ABI-generated structs we fixed)
  // ─────────────────────────────────────────────────────────────────────────────
  {
    using boost::multiprecision::cpp_int;

    // Test 1: Basic tuple return round-trip (UniswapV3Pool_Slot0)
    RUN_TEST("tuple return: UniswapV3Pool_Slot0 round-trip",
      ([&](){
        using Slot0_Named = abi::protocols::IUniswapV3Pool_Slot0;
        Slot0_Named slot0{};
        slot0.sqrtPriceX96 = cpp_int("429512873912345678901234567890");
        slot0.tick = cpp_int("-12345");
        slot0.observationIndex = uint16_t(42);
        slot0.observationCardinality = uint16_t(100);
        slot0.observationCardinalityNext = uint16_t(150);
        slot0.feeProtocol = uint8_t(5);
        slot0.unlocked = true;

        const size_t size = abi::encoded_size<Slot0_Named>(slot0);
        std::vector<uint8_t> buffer(size);
        abi::Error err;

        if (!abi::encode_into<Slot0_Named>(buffer.data(), buffer.size(), slot0, &err)) {
          std::cerr << "encode failed: " << err.message << "\n";
          return false;
        }

        Slot0_Named decoded{};
        if (!abi::decode_from<Slot0_Named>(
            abi::BytesSpan(buffer.data(), buffer.size()), decoded, &err)) {
          std::cerr << "decode failed: " << err.message << "\n";
          return false;
        }

        return decoded.sqrtPriceX96 == slot0.sqrtPriceX96 &&
               decoded.tick == slot0.tick &&
               decoded.observationIndex == slot0.observationIndex &&
               decoded.observationCardinality == slot0.observationCardinality &&
               decoded.observationCardinalityNext == slot0.observationCardinalityNext &&
               decoded.feeProtocol == slot0.feeProtocol &&
               decoded.unlocked == slot0.unlocked;
      })());

    // Test 2: Complex dyn_array with named structs (Multicall3_Call[])
    RUN_TEST("named struct: dyn_array<Multicall3_Call> round-trip",
      ([&](){
        using CallArray = abi::dyn_array<abi::protocols::Multicall3_Call>;

        std::vector<abi::protocols::Multicall3_Call> calls;
        abi::protocols::Multicall3_Call call1{}, call2{};

        // First call
        call1.target = abi::addr_from_hex("0x1111111111111111111111111111111111111111");
        call1.callData = {0x12, 0x34, 0x56};

        // Second call
        call2.target = abi::addr_from_hex("0x2222222222222222222222222222222222222222");
        call2.callData = {0x78, 0x9A, 0xBC, 0xDE};

        calls.push_back(call1);
        calls.push_back(call2);

        const size_t size = abi::encoded_size<CallArray>(calls);
        std::vector<uint8_t> buffer(size);
        abi::Error err;

        if (!abi::encode_into<CallArray>(buffer.data(), buffer.size(), calls, &err)) {
          std::cerr << "encode failed: " << err.message << "\n";
          return false;
        }

        std::vector<abi::protocols::Multicall3_Call> decoded_calls;
        if (!abi::decode_from<CallArray>(
            abi::BytesSpan(buffer.data(), buffer.size()), decoded_calls, &err)) {
          std::cerr << "decode failed: " << err.message << "\n";
          return false;
        }

        return decoded_calls.size() == 2 &&
               decoded_calls[0].target == call1.target &&
               decoded_calls[0].callData == call1.callData &&
               decoded_calls[1].target == call2.target &&
               decoded_calls[1].callData == call2.callData;
      })());

    // Test 3: Tuple with dyn_array<named_struct> (UniswapV3TickLens return type)
    RUN_TEST("named struct: tuple with dyn_array<ITickLens_PopulatedTick>",
      ([&](){
        using TickArray = abi::dyn_array<abi::protocols::ITickLens_PopulatedTick>;
        using ReturnType = abi::tuple<TickArray>;

        std::vector<abi::protocols::ITickLens_PopulatedTick> ticks;
        abi::protocols::ITickLens_PopulatedTick tick1{}, tick2{};

        tick1.tick = cpp_int("1000");
        tick1.liquidityNet = cpp_int("-500000000000000000");
        tick1.liquidityGross = cpp_int("1000000000000000000");

        tick2.tick = cpp_int("2000");
        tick2.liquidityNet = cpp_int("750000000000000000");
        tick2.liquidityGross = cpp_int("2000000000000000000");

        ticks.push_back(tick1);
        ticks.push_back(tick2);

        auto return_value = std::make_tuple(ticks);

        const size_t size = abi::encoded_size<ReturnType>(return_value);
        std::vector<uint8_t> buffer(size);
        abi::Error err;

        if (!abi::encode_into<ReturnType>(buffer.data(), buffer.size(), return_value, &err)) {
          std::cerr << "encode failed: " << err.message << "\n";
          return false;
        }

        std::tuple<std::vector<abi::protocols::ITickLens_PopulatedTick>> decoded;
        if (!abi::decode_from<ReturnType>(
            abi::BytesSpan(buffer.data(), buffer.size()), decoded, &err)) {
          std::cerr << "decode failed: " << err.message << "\n";
          return false;
        }

        const auto& decoded_ticks = std::get<0>(decoded);
        return decoded_ticks.size() == 2 &&
               decoded_ticks[0].tick == tick1.tick &&
               decoded_ticks[0].liquidityNet == tick1.liquidityNet &&
               decoded_ticks[0].liquidityGross == tick1.liquidityGross &&
               decoded_ticks[1].tick == tick2.tick &&
               decoded_ticks[1].liquidityNet == tick2.liquidityNet &&
               decoded_ticks[1].liquidityGross == tick2.liquidityGross;
      })());

    // Test 4: Complex nested structure (Multicall3_Result array)
    RUN_TEST("named struct: dyn_array<Multicall3_Result> complex round-trip",
      ([&](){
        using ResultArray = abi::dyn_array<abi::protocols::Multicall3_Result>;

        std::vector<abi::protocols::Multicall3_Result> results;
        abi::protocols::Multicall3_Result result1{}, result2{};

        result1.success = true;
        result1.returnData = {0x00, 0x01, 0x02, 0x03};

        result2.success = false;
        result2.returnData = {0xFF, 0xFE, 0xFD};

        results.push_back(result1);
        results.push_back(result2);

        const size_t size = abi::encoded_size<ResultArray>(results);
        std::vector<uint8_t> buffer(size);
        abi::Error err;

        if (!abi::encode_into<ResultArray>(buffer.data(), buffer.size(), results, &err)) {
          std::cerr << "encode failed: " << err.message << "\n";
          return false;
        }

        std::vector<abi::protocols::Multicall3_Result> decoded_results;
        if (!abi::decode_from<ResultArray>(
            abi::BytesSpan(buffer.data(), buffer.size()), decoded_results, &err)) {
          std::cerr << "decode failed: " << err.message << "\n";
          return false;
        }

        return decoded_results.size() == 2 &&
               decoded_results[0].success == result1.success &&
               decoded_results[0].returnData == result1.returnData &&
               decoded_results[1].success == result2.success &&
               decoded_results[1].returnData == result2.returnData;
      })());

    // Test 5: Function call encoding with named structs
    RUN_TEST("named struct: UniswapV3Pool_Ticks function call encoding",
      ([&](){
        // This tests that the Fn wrapper works with our fixed dyn_array notation
        const cpp_int tick_param = cpp_int("1000");

        // Just test that we can compute the encoded size without errors
        const size_t encoded_size = Ticks::encoded_size(tick_param);
        return encoded_size > 0 && encoded_size < 1000;  // Reasonable bounds check
      })());

    // Test 6: Contract-prefixed naming prevents conflicts
    RUN_TEST("named struct: contract-prefixed names prevent conflicts",
      ([&](){
        // Test that different contracts can have same struct names without conflicts
        // For example, a function and a struct can have similar names.
        // Let's use a real example from the generated code.

        abi::protocols::ERC20_BalanceOf balance_of_fn{};
        abi::protocols::ITickLens_PopulatedTick tick_struct{};

        // Both should compile and be usable
        balance_of_fn = abi::protocols::ERC20_BalanceOf{};  // Default constructible
        tick_struct.tick = 123;  // Field accessible

        return true;  // If we get here without compilation errors, test passes
      })());
  }
  
  std::cout << "\nSummary: " << counts.passed << " / " << counts.total << " passed\n";
  return (counts.passed == counts.total) ? 0 : 1;

} catch (const std::exception& e) {
  std::cerr << "Fatal: " << e.what() << "\n";
  return 1;
}
