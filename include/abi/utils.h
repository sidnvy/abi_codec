#pragma once
#include <array>
#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>

namespace abi {

// ----------------- hex parsing utilities -----------------

/**
 * Parse hex string to byte vector
 * Supports both "0x" prefixed and raw hex strings
 */
inline std::vector<uint8_t> parse_hex(const std::string& h) {
  size_t i = (h.size() >= 2 && (h[0] == '0') && (h[1] == 'x' || h[1] == 'X')) ? 2 : 0;
  std::vector<uint8_t> out; out.reserve((h.size() - i + 1) / 2);

  // Handle odd-length hex strings
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

/**
 * Parse hex string to 20-byte Ethereum address
 * Throws std::runtime_error if address is not exactly 20 bytes
 */
inline std::array<uint8_t, 20> addr_from_hex(const std::string& addr) {
  auto b = parse_hex(addr);
  if (b.size() != 20) {
    throw std::runtime_error("Invalid address length: " + addr + " (expected 20 bytes, got " + std::to_string(b.size()) + ")");
  }
  std::array<uint8_t, 20> a{};
  std::copy(b.begin(), b.end(), a.begin());
  return a;
}

/**
 * Convert address to hex string with 0x prefix
 */
inline std::string addr_to_hex_string(const std::array<uint8_t, 20>& addr) {
  std::string result = "0x";
  for (uint8_t byte : addr) {
    char buf[3];
    std::sprintf(buf, "%02x", byte);
    result += buf;
  }
  return result;
}

} // namespace abi
