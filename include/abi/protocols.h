#pragma once

#include "abi.h"

namespace abi {
namespace protocols {

// Auto-generated from ABI JSON files on 2025-08-29T09:41:13.704Z
// Run: node scripts/generate_from_abi_json.mjs

// ==============================
// ERC20
// ==============================

struct Sel_name { static constexpr std::array<uint8_t,4> value{{0x06,0xfd,0xde,0x03}}; }; // "name()"
using Name = Fn<Sel_name, string_t>;

struct Sel_approve { static constexpr std::array<uint8_t,4> value{{0x09,0x5e,0xa7,0xb3}}; }; // "approve(address,uint256)"
using Approve = Fn<Sel_approve, bool_t, address20, uint_t<256>>;

struct Sel_totalSupply { static constexpr std::array<uint8_t,4> value{{0x18,0x16,0x0d,0xdd}}; }; // "totalSupply()"
using TotalSupply = Fn<Sel_totalSupply, uint_t<256>>;

struct Sel_transferFrom { static constexpr std::array<uint8_t,4> value{{0x23,0xb8,0x72,0xdd}}; }; // "transferFrom(address,address,uint256)"
using TransferFrom = Fn<Sel_transferFrom, bool_t, address20, address20, uint_t<256>>;

struct Sel_decimals { static constexpr std::array<uint8_t,4> value{{0x31,0x3c,0xe5,0x67}}; }; // "decimals()"
using Decimals = Fn<Sel_decimals, uint_t<8>>;

struct Sel_balanceOf { static constexpr std::array<uint8_t,4> value{{0x70,0xa0,0x82,0x31}}; }; // "balanceOf(address)"
using BalanceOf = Fn<Sel_balanceOf, uint_t<256>, address20>;

struct Sel_symbol { static constexpr std::array<uint8_t,4> value{{0x95,0xd8,0x9b,0x41}}; }; // "symbol()"
using Symbol = Fn<Sel_symbol, string_t>;

struct Sel_transfer { static constexpr std::array<uint8_t,4> value{{0xa9,0x05,0x9c,0xbb}}; }; // "transfer(address,uint256)"
using Transfer = Fn<Sel_transfer, bool_t, address20, uint_t<256>>;

struct Sel_allowance { static constexpr std::array<uint8_t,4> value{{0xdd,0x62,0xed,0x3e}}; }; // "allowance(address,address)"
using Allowance = Fn<Sel_allowance, uint_t<256>, address20, address20>;

// ==============================
// UniswapV3Pool
// ==============================

struct Sel_token0 { static constexpr std::array<uint8_t,4> value{{0x0d,0xfe,0x16,0x81}}; }; // "token0()"
using Token0 = Fn<Sel_token0, address20>;

struct Sel_token1 { static constexpr std::array<uint8_t,4> value{{0xd2,0x12,0x20,0xa7}}; }; // "token1()"
using Token1 = Fn<Sel_token1, address20>;

struct Sel_slot0 { static constexpr std::array<uint8_t,4> value{{0x38,0x50,0xc7,0xbd}}; }; // "slot0()"
using Slot0 = Fn<Sel_slot0, tuple<uint_t<160>, int_t<24>, uint_t<16>, uint_t<16>, uint_t<16>, uint_t<8>, bool_t>>;

struct Sel_liquidity { static constexpr std::array<uint8_t,4> value{{0x1a,0x68,0x65,0x02}}; }; // "liquidity()"
using Liquidity = Fn<Sel_liquidity, uint_t<128>>;

struct Sel_ticks { static constexpr std::array<uint8_t,4> value{{0xf3,0x0d,0xba,0x93}}; }; // "ticks(int24)"
using Ticks = Fn<Sel_ticks, tuple<uint_t<128>, int_t<128>, uint_t<256>, uint_t<256>, int_t<56>, uint_t<160>, uint_t<32>, bool_t>, int_t<24>>;

struct Sel_observe { static constexpr std::array<uint8_t,4> value{{0x88,0x3b,0xdb,0xfd}}; }; // "observe(uint32[])"
using Observe = Fn<Sel_observe, tuple<dyn_array<int_t<56>>, dyn_array<uint_t<160>>>, dyn_array<uint_t<32>>>;

// ==============================
// UniswapV3TickLens
// ==============================

struct Sel_getPopulatedTicksInWord { static constexpr std::array<uint8_t,4> value{{0x35,0x1f,0xb4,0x78}}; }; // "getPopulatedTicksInWord(address,int16)"
using GetPopulatedTicksInWord = Fn<Sel_getPopulatedTicksInWord, dyn_array<tuple<int_t<24>, int_t<128>, uint_t<128>>>, address20, int_t<16>>;

// ==============================
// Multicall
// ==============================

struct Sel_aggregate { static constexpr std::array<uint8_t,4> value{{0x25,0x2d,0xba,0x42}}; }; // "aggregate((address,bytes)[])"
using Aggregate = Fn<Sel_aggregate, tuple<uint_t<256>, dyn_array<bytes>>, dyn_array<tuple<address20, bytes>>>;

struct Sel_aggregate3 { static constexpr std::array<uint8_t,4> value{{0x82,0xad,0x56,0xcb}}; }; // "aggregate3((address,bool,bytes)[])"
using Aggregate3 = Fn<Sel_aggregate3, dyn_array<tuple<bool_t, bytes>>, dyn_array<tuple<address20, bool_t, bytes>>>;

struct Sel_aggregate3Value { static constexpr std::array<uint8_t,4> value{{0x17,0x4d,0xea,0x71}}; }; // "aggregate3Value((address,bool,uint256,bytes)[])"
using Aggregate3Value = Fn<Sel_aggregate3Value, dyn_array<tuple<bool_t, bytes>>, dyn_array<tuple<address20, bool_t, uint_t<256>, bytes>>>;

struct Sel_blockAndAggregate { static constexpr std::array<uint8_t,4> value{{0xc3,0x07,0x7f,0xa9}}; }; // "blockAndAggregate((address,bytes)[])"
using BlockAndAggregate = Fn<Sel_blockAndAggregate, tuple<uint_t<256>, uint_t<256>, dyn_array<tuple<bool_t, bytes>>>, dyn_array<tuple<address20, bytes>>>;

struct Sel_getBasefee { static constexpr std::array<uint8_t,4> value{{0x3e,0x64,0xa6,0x96}}; }; // "getBasefee()"
using GetBasefee = Fn<Sel_getBasefee, uint_t<256>>;

struct Sel_getBlockHash { static constexpr std::array<uint8_t,4> value{{0xee,0x82,0xac,0x5e}}; }; // "getBlockHash(uint256)"
using GetBlockHash = Fn<Sel_getBlockHash, uint_t<256>, uint_t<256>>;

struct Sel_getBlockNumber { static constexpr std::array<uint8_t,4> value{{0x42,0xcb,0xb1,0x5c}}; }; // "getBlockNumber()"
using GetBlockNumber = Fn<Sel_getBlockNumber, uint_t<256>>;

struct Sel_getChainId { static constexpr std::array<uint8_t,4> value{{0x34,0x08,0xe4,0x70}}; }; // "getChainId()"
using GetChainId = Fn<Sel_getChainId, uint_t<256>>;

struct Sel_getCurrentBlockCoinbase { static constexpr std::array<uint8_t,4> value{{0xa8,0xb0,0x57,0x4e}}; }; // "getCurrentBlockCoinbase()"
using GetCurrentBlockCoinbase = Fn<Sel_getCurrentBlockCoinbase, address20>;

struct Sel_getCurrentBlockDifficulty { static constexpr std::array<uint8_t,4> value{{0x72,0x42,0x5d,0x9d}}; }; // "getCurrentBlockDifficulty()"
using GetCurrentBlockDifficulty = Fn<Sel_getCurrentBlockDifficulty, uint_t<256>>;

struct Sel_getCurrentBlockGasLimit { static constexpr std::array<uint8_t,4> value{{0x86,0xd5,0x16,0xe8}}; }; // "getCurrentBlockGasLimit()"
using GetCurrentBlockGasLimit = Fn<Sel_getCurrentBlockGasLimit, uint_t<256>>;

struct Sel_getCurrentBlockTimestamp { static constexpr std::array<uint8_t,4> value{{0x0f,0x28,0xc9,0x7d}}; }; // "getCurrentBlockTimestamp()"
using GetCurrentBlockTimestamp = Fn<Sel_getCurrentBlockTimestamp, uint_t<256>>;

struct Sel_getEthBalance { static constexpr std::array<uint8_t,4> value{{0x4d,0x23,0x01,0xcc}}; }; // "getEthBalance(address)"
using GetEthBalance = Fn<Sel_getEthBalance, uint_t<256>, address20>;

struct Sel_getLastBlockHash { static constexpr std::array<uint8_t,4> value{{0x27,0xe8,0x6d,0x6e}}; }; // "getLastBlockHash()"
using GetLastBlockHash = Fn<Sel_getLastBlockHash, uint_t<256>>;

struct Sel_tryAggregate { static constexpr std::array<uint8_t,4> value{{0xbc,0xe3,0x8b,0xd7}}; }; // "tryAggregate(bool,(address,bytes)[])"
using TryAggregate = Fn<Sel_tryAggregate, dyn_array<tuple<bool_t, bytes>>, bool_t, dyn_array<tuple<address20, bytes>>>;

struct Sel_tryBlockAndAggregate { static constexpr std::array<uint8_t,4> value{{0x39,0x95,0x42,0xe9}}; }; // "tryBlockAndAggregate(bool,(address,bytes)[])"
using TryBlockAndAggregate = Fn<Sel_tryBlockAndAggregate, tuple<uint_t<256>, uint_t<256>, dyn_array<tuple<bool_t, bytes>>>, bool_t, dyn_array<tuple<address20, bytes>>>;

} // namespace protocols
} // namespace abi
