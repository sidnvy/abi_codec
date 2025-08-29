#pragma once

#include "abi.h"

// Auto-generated from ABI JSON files on 2025-08-29T17:15:32.636Z
// Run: node scripts/generate_from_abi_json.mjs

// ==============================
// Named Structs (in abi::protocols namespace)
// ==============================

namespace abi {
namespace protocols {

// ---------- Named schema for IUniswapV3Pool.Slot0 ----------
struct IUniswapV3Pool_Slot0 {
  abi::cpp_t<uint_t<160>> sqrtPriceX96;
  abi::cpp_t<int_t<24>> tick;
  abi::cpp_t<uint_t<16>> observationIndex;
  abi::cpp_t<uint_t<16>> observationCardinality;
  abi::cpp_t<uint_t<16>> observationCardinalityNext;
  abi::cpp_t<uint_t<8>> feeProtocol;
  abi::cpp_t<bool_t> unlocked;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<160>, int_t<24>, uint_t<16>, uint_t<16>, uint_t<16>, uint_t<8>, bool_t>;

  // Conversion to underlying tuple
  static abi::value_of<schema>::type to_tuple(const IUniswapV3Pool_Slot0& s) {
    return std::make_tuple(
      s.sqrtPriceX96,
      s.tick,
      s.observationIndex,
      s.observationCardinality,
      s.observationCardinalityNext,
      s.feeProtocol,
      s.unlocked
    );
  }

  // Conversion from underlying tuple
  static IUniswapV3Pool_Slot0 from_tuple(const abi::value_of<schema>::type& t) {
    IUniswapV3Pool_Slot0 s{};
    s.sqrtPriceX96 = std::get<0>(t);
    s.tick = std::get<1>(t);
    s.observationIndex = std::get<2>(t);
    s.observationCardinality = std::get<3>(t);
    s.observationCardinalityNext = std::get<4>(t);
    s.feeProtocol = std::get<5>(t);
    s.unlocked = std::get<6>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV3Pool.Tick ----------
struct IUniswapV3Pool_Tick {
  abi::cpp_t<uint_t<128>> liquidityGross;
  abi::cpp_t<int_t<128>> liquidityNet;
  abi::cpp_t<uint_t<256>> feeGrowthOutside0X128;
  abi::cpp_t<uint_t<256>> feeGrowthOutside1X128;
  abi::cpp_t<int_t<56>> tickCumulativeOutside;
  abi::cpp_t<uint_t<160>> secondsPerLiquidityOutsideX128;
  abi::cpp_t<uint_t<32>> secondsOutside;
  abi::cpp_t<bool_t> initialized;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<128>, int_t<128>, uint_t<256>, uint_t<256>, int_t<56>, uint_t<160>, uint_t<32>, bool_t>;

  // Conversion to underlying tuple
  static abi::value_of<schema>::type to_tuple(const IUniswapV3Pool_Tick& s) {
    return std::make_tuple(
      s.liquidityGross,
      s.liquidityNet,
      s.feeGrowthOutside0X128,
      s.feeGrowthOutside1X128,
      s.tickCumulativeOutside,
      s.secondsPerLiquidityOutsideX128,
      s.secondsOutside,
      s.initialized
    );
  }

  // Conversion from underlying tuple
  static IUniswapV3Pool_Tick from_tuple(const abi::value_of<schema>::type& t) {
    IUniswapV3Pool_Tick s{};
    s.liquidityGross = std::get<0>(t);
    s.liquidityNet = std::get<1>(t);
    s.feeGrowthOutside0X128 = std::get<2>(t);
    s.feeGrowthOutside1X128 = std::get<3>(t);
    s.tickCumulativeOutside = std::get<4>(t);
    s.secondsPerLiquidityOutsideX128 = std::get<5>(t);
    s.secondsOutside = std::get<6>(t);
    s.initialized = std::get<7>(t);
    return s;
  }
};

// ---------- Named schema for ITickLens.PopulatedTick ----------
struct ITickLens_PopulatedTick {
  abi::cpp_t<int_t<24>> tick;
  abi::cpp_t<int_t<128>> liquidityNet;
  abi::cpp_t<uint_t<128>> liquidityGross;

  // Underlying ABI schema
  using schema = abi::tuple<int_t<24>, int_t<128>, uint_t<128>>;

  // Conversion to underlying tuple
  static abi::value_of<schema>::type to_tuple(const ITickLens_PopulatedTick& s) {
    return std::make_tuple(
      s.tick,
      s.liquidityNet,
      s.liquidityGross
    );
  }

  // Conversion from underlying tuple
  static ITickLens_PopulatedTick from_tuple(const abi::value_of<schema>::type& t) {
    ITickLens_PopulatedTick s{};
    s.tick = std::get<0>(t);
    s.liquidityNet = std::get<1>(t);
    s.liquidityGross = std::get<2>(t);
    return s;
  }
};

// ---------- Named schema for Multicall3.Call ----------
struct Multicall3_Call {
  abi::cpp_t<address20> target;
  abi::cpp_t<bytes> callData;

  // Underlying ABI schema
  using schema = abi::tuple<address20, bytes>;

  // Conversion to underlying tuple
  static abi::value_of<schema>::type to_tuple(const Multicall3_Call& s) {
    return std::make_tuple(
      s.target,
      s.callData
    );
  }

  // Conversion from underlying tuple
  static Multicall3_Call from_tuple(const abi::value_of<schema>::type& t) {
    Multicall3_Call s{};
    s.target = std::get<0>(t);
    s.callData = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for Multicall3.Call3 ----------
struct Multicall3_Call3 {
  abi::cpp_t<address20> target;
  abi::cpp_t<bool_t> allowFailure;
  abi::cpp_t<bytes> callData;

  // Underlying ABI schema
  using schema = abi::tuple<address20, bool_t, bytes>;

  // Conversion to underlying tuple
  static abi::value_of<schema>::type to_tuple(const Multicall3_Call3& s) {
    return std::make_tuple(
      s.target,
      s.allowFailure,
      s.callData
    );
  }

  // Conversion from underlying tuple
  static Multicall3_Call3 from_tuple(const abi::value_of<schema>::type& t) {
    Multicall3_Call3 s{};
    s.target = std::get<0>(t);
    s.allowFailure = std::get<1>(t);
    s.callData = std::get<2>(t);
    return s;
  }
};

// ---------- Named schema for Multicall3.Result ----------
struct Multicall3_Result {
  abi::cpp_t<bool_t> success;
  abi::cpp_t<bytes> returnData;

  // Underlying ABI schema
  using schema = abi::tuple<bool_t, bytes>;

  // Conversion to underlying tuple
  static abi::value_of<schema>::type to_tuple(const Multicall3_Result& s) {
    return std::make_tuple(
      s.success,
      s.returnData
    );
  }

  // Conversion from underlying tuple
  static Multicall3_Result from_tuple(const abi::value_of<schema>::type& t) {
    Multicall3_Result s{};
    s.success = std::get<0>(t);
    s.returnData = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for Multicall3.Call3Value ----------
struct Multicall3_Call3Value {
  abi::cpp_t<address20> target;
  abi::cpp_t<bool_t> allowFailure;
  abi::cpp_t<uint_t<256>> value;
  abi::cpp_t<bytes> callData;

  // Underlying ABI schema
  using schema = abi::tuple<address20, bool_t, uint_t<256>, bytes>;

  // Conversion to underlying tuple
  static abi::value_of<schema>::type to_tuple(const Multicall3_Call3Value& s) {
    return std::make_tuple(
      s.target,
      s.allowFailure,
      s.value,
      s.callData
    );
  }

  // Conversion from underlying tuple
  static Multicall3_Call3Value from_tuple(const abi::value_of<schema>::type& t) {
    Multicall3_Call3Value s{};
    s.target = std::get<0>(t);
    s.allowFailure = std::get<1>(t);
    s.value = std::get<2>(t);
    s.callData = std::get<3>(t);
    return s;
  }
};



} // namespace protocols

// ==============================
// Traits specializations for named structs (in abi namespace)
// ==============================

// value_of specializations for named structs
template<> struct value_of<protocols::IUniswapV3Pool_Slot0> { using type = protocols::IUniswapV3Pool_Slot0; };
template<> struct value_of<protocols::IUniswapV3Pool_Tick> { using type = protocols::IUniswapV3Pool_Tick; };
template<> struct value_of<protocols::ITickLens_PopulatedTick> { using type = protocols::ITickLens_PopulatedTick; };
template<> struct value_of<protocols::Multicall3_Call> { using type = protocols::Multicall3_Call; };
template<> struct value_of<protocols::Multicall3_Call3> { using type = protocols::Multicall3_Call3; };
template<> struct value_of<protocols::Multicall3_Result> { using type = protocols::Multicall3_Result; };
template<> struct value_of<protocols::Multicall3_Call3Value> { using type = protocols::Multicall3_Call3Value; };

// traits specializations that delegate to underlying tuple schema
template<> struct traits<protocols::IUniswapV3Pool_Slot0> {
  using S = typename protocols::IUniswapV3Pool_Slot0::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_Slot0& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_Slot0::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_Slot0& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_Slot0::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_Slot0& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_Slot0::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_Slot0& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_Slot0::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV3Pool_Tick> {
  using S = typename protocols::IUniswapV3Pool_Tick::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_Tick& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_Tick::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_Tick& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_Tick::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_Tick& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_Tick::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_Tick& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_Tick::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::ITickLens_PopulatedTick> {
  using S = typename protocols::ITickLens_PopulatedTick::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::ITickLens_PopulatedTick& v) {
    return traits<S>::tail_size( protocols::ITickLens_PopulatedTick::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::ITickLens_PopulatedTick& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::ITickLens_PopulatedTick::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::ITickLens_PopulatedTick& v) {
    traits<S>::encode_tail(out, base, protocols::ITickLens_PopulatedTick::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::ITickLens_PopulatedTick& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::ITickLens_PopulatedTick::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::Multicall3_Call> {
  using S = typename protocols::Multicall3_Call::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::Multicall3_Call& v) {
    return traits<S>::tail_size( protocols::Multicall3_Call::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::Multicall3_Call& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::Multicall3_Call::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::Multicall3_Call& v) {
    traits<S>::encode_tail(out, base, protocols::Multicall3_Call::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::Multicall3_Call& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::Multicall3_Call::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::Multicall3_Call3> {
  using S = typename protocols::Multicall3_Call3::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::Multicall3_Call3& v) {
    return traits<S>::tail_size( protocols::Multicall3_Call3::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::Multicall3_Call3& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::Multicall3_Call3::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::Multicall3_Call3& v) {
    traits<S>::encode_tail(out, base, protocols::Multicall3_Call3::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::Multicall3_Call3& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::Multicall3_Call3::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::Multicall3_Result> {
  using S = typename protocols::Multicall3_Result::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::Multicall3_Result& v) {
    return traits<S>::tail_size( protocols::Multicall3_Result::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::Multicall3_Result& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::Multicall3_Result::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::Multicall3_Result& v) {
    traits<S>::encode_tail(out, base, protocols::Multicall3_Result::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::Multicall3_Result& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::Multicall3_Result::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::Multicall3_Call3Value> {
  using S = typename protocols::Multicall3_Call3Value::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::Multicall3_Call3Value& v) {
    return traits<S>::tail_size( protocols::Multicall3_Call3Value::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::Multicall3_Call3Value& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::Multicall3_Call3Value::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::Multicall3_Call3Value& v) {
    traits<S>::encode_tail(out, base, protocols::Multicall3_Call3Value::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::Multicall3_Call3Value& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::Multicall3_Call3Value::from_tuple(tmp);
    return true;
  }
};



} // namespace abi

namespace abi {
namespace protocols {
// ==============================
// ERC20
// ==============================

struct Sel_ERC20_name { static constexpr std::array<uint8_t,4> value{{0x06,0xfd,0xde,0x03}}; }; // "name()"
using ERC20_Name = Fn<Sel_ERC20_name, string_t>;

struct Sel_ERC20_approve { static constexpr std::array<uint8_t,4> value{{0x09,0x5e,0xa7,0xb3}}; }; // "approve(address,uint256)"
using ERC20_Approve = Fn<Sel_ERC20_approve, bool_t, address20, uint_t<256>>;

struct Sel_ERC20_totalSupply { static constexpr std::array<uint8_t,4> value{{0x18,0x16,0x0d,0xdd}}; }; // "totalSupply()"
using ERC20_TotalSupply = Fn<Sel_ERC20_totalSupply, uint_t<256>>;

struct Sel_ERC20_transferFrom { static constexpr std::array<uint8_t,4> value{{0x23,0xb8,0x72,0xdd}}; }; // "transferFrom(address,address,uint256)"
using ERC20_TransferFrom = Fn<Sel_ERC20_transferFrom, bool_t, address20, address20, uint_t<256>>;

struct Sel_ERC20_decimals { static constexpr std::array<uint8_t,4> value{{0x31,0x3c,0xe5,0x67}}; }; // "decimals()"
using ERC20_Decimals = Fn<Sel_ERC20_decimals, uint_t<8>>;

struct Sel_ERC20_balanceOf { static constexpr std::array<uint8_t,4> value{{0x70,0xa0,0x82,0x31}}; }; // "balanceOf(address)"
using ERC20_BalanceOf = Fn<Sel_ERC20_balanceOf, uint_t<256>, address20>;

struct Sel_ERC20_symbol { static constexpr std::array<uint8_t,4> value{{0x95,0xd8,0x9b,0x41}}; }; // "symbol()"
using ERC20_Symbol = Fn<Sel_ERC20_symbol, string_t>;

struct Sel_ERC20_transfer { static constexpr std::array<uint8_t,4> value{{0xa9,0x05,0x9c,0xbb}}; }; // "transfer(address,uint256)"
using ERC20_Transfer = Fn<Sel_ERC20_transfer, bool_t, address20, uint_t<256>>;

struct Sel_ERC20_allowance { static constexpr std::array<uint8_t,4> value{{0xdd,0x62,0xed,0x3e}}; }; // "allowance(address,address)"
using ERC20_Allowance = Fn<Sel_ERC20_allowance, uint_t<256>, address20, address20>;

// ==============================
// UniswapV3Pool
// ==============================

struct Sel_UniswapV3Pool_token0 { static constexpr std::array<uint8_t,4> value{{0x0d,0xfe,0x16,0x81}}; }; // "token0()"
using UniswapV3Pool_Token0 = Fn<Sel_UniswapV3Pool_token0, address20>;

struct Sel_UniswapV3Pool_token1 { static constexpr std::array<uint8_t,4> value{{0xd2,0x12,0x20,0xa7}}; }; // "token1()"
using UniswapV3Pool_Token1 = Fn<Sel_UniswapV3Pool_token1, address20>;

struct Sel_UniswapV3Pool_slot0 { static constexpr std::array<uint8_t,4> value{{0x38,0x50,0xc7,0xbd}}; }; // "slot0()"
using UniswapV3Pool_Slot0 = Fn<Sel_UniswapV3Pool_slot0, IUniswapV3Pool_Slot0>;

struct Sel_UniswapV3Pool_liquidity { static constexpr std::array<uint8_t,4> value{{0x1a,0x68,0x65,0x02}}; }; // "liquidity()"
using UniswapV3Pool_Liquidity = Fn<Sel_UniswapV3Pool_liquidity, uint_t<128>>;

struct Sel_UniswapV3Pool_ticks { static constexpr std::array<uint8_t,4> value{{0xf3,0x0d,0xba,0x93}}; }; // "ticks(int24)"
using UniswapV3Pool_Ticks = Fn<Sel_UniswapV3Pool_ticks, IUniswapV3Pool_Tick, int_t<24>>;

struct Sel_UniswapV3Pool_observe { static constexpr std::array<uint8_t,4> value{{0x88,0x3b,0xdb,0xfd}}; }; // "observe(uint32[])"
using UniswapV3Pool_Observe = Fn<Sel_UniswapV3Pool_observe, tuple<dyn_array<int_t<56>>, dyn_array<uint_t<160>>>, dyn_array<uint_t<32>>>;

// ==============================
// UniswapV3TickLens
// ==============================

struct Sel_UniswapV3TickLens_getPopulatedTicksInWord { static constexpr std::array<uint8_t,4> value{{0x35,0x1f,0xb4,0x78}}; }; // "getPopulatedTicksInWord(address,int16)"
using UniswapV3TickLens_GetPopulatedTicksInWord = Fn<Sel_UniswapV3TickLens_getPopulatedTicksInWord, dyn_array<ITickLens_PopulatedTick>, address20, int_t<16>>;

// ==============================
// Multicall
// ==============================

struct Sel_Multicall_aggregate { static constexpr std::array<uint8_t,4> value{{0x25,0x2d,0xba,0x42}}; }; // "aggregate((address,bytes)[])"
using Multicall_Aggregate = Fn<Sel_Multicall_aggregate, tuple<uint_t<256>, dyn_array<bytes>>, dyn_array<Multicall3_Call>>;

struct Sel_Multicall_aggregate3 { static constexpr std::array<uint8_t,4> value{{0x82,0xad,0x56,0xcb}}; }; // "aggregate3((address,bool,bytes)[])"
using Multicall_Aggregate3 = Fn<Sel_Multicall_aggregate3, dyn_array<Multicall3_Result>, dyn_array<Multicall3_Call3>>;

struct Sel_Multicall_aggregate3Value { static constexpr std::array<uint8_t,4> value{{0x17,0x4d,0xea,0x71}}; }; // "aggregate3Value((address,bool,uint256,bytes)[])"
using Multicall_Aggregate3Value = Fn<Sel_Multicall_aggregate3Value, dyn_array<Multicall3_Result>, dyn_array<Multicall3_Call3Value>>;

struct Sel_Multicall_blockAndAggregate { static constexpr std::array<uint8_t,4> value{{0xc3,0x07,0x7f,0xa9}}; }; // "blockAndAggregate((address,bytes)[])"
using Multicall_BlockAndAggregate = Fn<Sel_Multicall_blockAndAggregate, tuple<uint_t<256>, uint_t<256>, dyn_array<Multicall3_Result>>, dyn_array<Multicall3_Call>>;

struct Sel_Multicall_getBasefee { static constexpr std::array<uint8_t,4> value{{0x3e,0x64,0xa6,0x96}}; }; // "getBasefee()"
using Multicall_GetBasefee = Fn<Sel_Multicall_getBasefee, uint_t<256>>;

struct Sel_Multicall_getBlockHash { static constexpr std::array<uint8_t,4> value{{0xee,0x82,0xac,0x5e}}; }; // "getBlockHash(uint256)"
using Multicall_GetBlockHash = Fn<Sel_Multicall_getBlockHash, uint_t<256>, uint_t<256>>;

struct Sel_Multicall_getBlockNumber { static constexpr std::array<uint8_t,4> value{{0x42,0xcb,0xb1,0x5c}}; }; // "getBlockNumber()"
using Multicall_GetBlockNumber = Fn<Sel_Multicall_getBlockNumber, uint_t<256>>;

struct Sel_Multicall_getChainId { static constexpr std::array<uint8_t,4> value{{0x34,0x08,0xe4,0x70}}; }; // "getChainId()"
using Multicall_GetChainId = Fn<Sel_Multicall_getChainId, uint_t<256>>;

struct Sel_Multicall_getCurrentBlockCoinbase { static constexpr std::array<uint8_t,4> value{{0xa8,0xb0,0x57,0x4e}}; }; // "getCurrentBlockCoinbase()"
using Multicall_GetCurrentBlockCoinbase = Fn<Sel_Multicall_getCurrentBlockCoinbase, address20>;

struct Sel_Multicall_getCurrentBlockDifficulty { static constexpr std::array<uint8_t,4> value{{0x72,0x42,0x5d,0x9d}}; }; // "getCurrentBlockDifficulty()"
using Multicall_GetCurrentBlockDifficulty = Fn<Sel_Multicall_getCurrentBlockDifficulty, uint_t<256>>;

struct Sel_Multicall_getCurrentBlockGasLimit { static constexpr std::array<uint8_t,4> value{{0x86,0xd5,0x16,0xe8}}; }; // "getCurrentBlockGasLimit()"
using Multicall_GetCurrentBlockGasLimit = Fn<Sel_Multicall_getCurrentBlockGasLimit, uint_t<256>>;

struct Sel_Multicall_getCurrentBlockTimestamp { static constexpr std::array<uint8_t,4> value{{0x0f,0x28,0xc9,0x7d}}; }; // "getCurrentBlockTimestamp()"
using Multicall_GetCurrentBlockTimestamp = Fn<Sel_Multicall_getCurrentBlockTimestamp, uint_t<256>>;

struct Sel_Multicall_getEthBalance { static constexpr std::array<uint8_t,4> value{{0x4d,0x23,0x01,0xcc}}; }; // "getEthBalance(address)"
using Multicall_GetEthBalance = Fn<Sel_Multicall_getEthBalance, uint_t<256>, address20>;

struct Sel_Multicall_getLastBlockHash { static constexpr std::array<uint8_t,4> value{{0x27,0xe8,0x6d,0x6e}}; }; // "getLastBlockHash()"
using Multicall_GetLastBlockHash = Fn<Sel_Multicall_getLastBlockHash, uint_t<256>>;

struct Sel_Multicall_tryAggregate { static constexpr std::array<uint8_t,4> value{{0xbc,0xe3,0x8b,0xd7}}; }; // "tryAggregate(bool,(address,bytes)[])"
using Multicall_TryAggregate = Fn<Sel_Multicall_tryAggregate, dyn_array<Multicall3_Result>, bool_t, dyn_array<Multicall3_Call>>;

struct Sel_Multicall_tryBlockAndAggregate { static constexpr std::array<uint8_t,4> value{{0x39,0x95,0x42,0xe9}}; }; // "tryBlockAndAggregate(bool,(address,bytes)[])"
using Multicall_TryBlockAndAggregate = Fn<Sel_Multicall_tryBlockAndAggregate, tuple<uint_t<256>, uint_t<256>, dyn_array<Multicall3_Result>>, bool_t, dyn_array<Multicall3_Call>>;

// ==============================
// UniswapV2Router
// ==============================

struct Sel_UniswapV2Router_addLiquidity { static constexpr std::array<uint8_t,4> value{{0xe8,0xe3,0x37,0x00}}; }; // "addLiquidity(address,address,uint256,uint256,uint256,uint256,address,uint256)"
using UniswapV2Router_AddLiquidity = Fn<Sel_UniswapV2Router_addLiquidity, tuple<uint_t<256>, uint_t<256>, uint_t<256>>, address20, address20, uint_t<256>, uint_t<256>, uint_t<256>, uint_t<256>, address20, uint_t<256>>;

struct Sel_UniswapV2Router_swapExactETHForTokens { static constexpr std::array<uint8_t,4> value{{0x7f,0xf3,0x6a,0xb5}}; }; // "swapExactETHForTokens(uint256,address[],address,uint256)"
using UniswapV2Router_SwapExactETHForTokens = Fn<Sel_UniswapV2Router_swapExactETHForTokens, dyn_array<uint_t<256>>, uint_t<256>, dyn_array<address20>, address20, uint_t<256>>;

struct Sel_UniswapV2Router_swapExactTokensForTokens { static constexpr std::array<uint8_t,4> value{{0x38,0xed,0x17,0x39}}; }; // "swapExactTokensForTokens(uint256,uint256,address[],address,uint256)"
using UniswapV2Router_SwapExactTokensForTokens = Fn<Sel_UniswapV2Router_swapExactTokensForTokens, dyn_array<uint_t<256>>, uint_t<256>, uint_t<256>, dyn_array<address20>, address20, uint_t<256>>;

struct Sel_UniswapV2Router_swapTokensForExactTokens { static constexpr std::array<uint8_t,4> value{{0x88,0x03,0xdb,0xee}}; }; // "swapTokensForExactTokens(uint256,uint256,address[],address,uint256)"
using UniswapV2Router_SwapTokensForExactTokens = Fn<Sel_UniswapV2Router_swapTokensForExactTokens, dyn_array<uint_t<256>>, uint_t<256>, uint_t<256>, dyn_array<address20>, address20, uint_t<256>>;

struct Sel_UniswapV2Router_getAmountsOut { static constexpr std::array<uint8_t,4> value{{0xd0,0x6c,0xa6,0x1f}}; }; // "getAmountsOut(uint256,address[])"
using UniswapV2Router_GetAmountsOut = Fn<Sel_UniswapV2Router_getAmountsOut, dyn_array<uint_t<256>>, uint_t<256>, dyn_array<address20>>;

struct Sel_UniswapV2Router_getAmountsIn { static constexpr std::array<uint8_t,4> value{{0x1f,0x00,0xca,0x74}}; }; // "getAmountsIn(uint256,address[])"
using UniswapV2Router_GetAmountsIn = Fn<Sel_UniswapV2Router_getAmountsIn, dyn_array<uint_t<256>>, uint_t<256>, dyn_array<address20>>;

} // namespace protocols
} // namespace abi
