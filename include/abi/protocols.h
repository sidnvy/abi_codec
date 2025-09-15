#pragma once

#include "abi.h"
#include <vector>

// Auto-generated from ABI JSON files on 2025-09-15T11:56:32.879Z
// Run: node scripts/generate_from_abi_json.mjs

// ==============================
// Named Structs (in abi::protocols namespace)
// ==============================

namespace abi {
namespace protocols {

// ---------- Named schema for ITickLens.PopulatedTick ----------
struct ITickLens_PopulatedTick {
  abi::cpp_t<int_t<24>> tick;
  abi::cpp_t<int_t<128>> liquidityNet;
  abi::cpp_t<uint_t<128>> liquidityGross;

  // Underlying ABI schema
  using schema = abi::tuple<int_t<24>, int_t<128>, uint_t<128>>;
  using values = std::tuple<abi::cpp_t<int_t<24>>, abi::cpp_t<int_t<128>>, abi::cpp_t<uint_t<128>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const ITickLens_PopulatedTick& s) {
    return std::make_tuple(
      s.tick,
      s.liquidityNet,
      s.liquidityGross
    );
  }

  // Conversion from underlying tuple values
  static ITickLens_PopulatedTick from_tuple(const values& t) {
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
  using values = std::tuple<abi::cpp_t<address20>, abi::cpp_t<bytes>>;

  // Conversion to underlying tuple values
  static values to_tuple(const Multicall3_Call& s) {
    return std::make_tuple(
      s.target,
      s.callData
    );
  }

  // Conversion from underlying tuple values
  static Multicall3_Call from_tuple(const values& t) {
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
  using values = std::tuple<abi::cpp_t<address20>, abi::cpp_t<bool_t>, abi::cpp_t<bytes>>;

  // Conversion to underlying tuple values
  static values to_tuple(const Multicall3_Call3& s) {
    return std::make_tuple(
      s.target,
      s.allowFailure,
      s.callData
    );
  }

  // Conversion from underlying tuple values
  static Multicall3_Call3 from_tuple(const values& t) {
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
  using values = std::tuple<abi::cpp_t<bool_t>, abi::cpp_t<bytes>>;

  // Conversion to underlying tuple values
  static values to_tuple(const Multicall3_Result& s) {
    return std::make_tuple(
      s.success,
      s.returnData
    );
  }

  // Conversion from underlying tuple values
  static Multicall3_Result from_tuple(const values& t) {
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
  using values = std::tuple<abi::cpp_t<address20>, abi::cpp_t<bool_t>, abi::cpp_t<uint_t<256>>, abi::cpp_t<bytes>>;

  // Conversion to underlying tuple values
  static values to_tuple(const Multicall3_Call3Value& s) {
    return std::make_tuple(
      s.target,
      s.allowFailure,
      s.value,
      s.callData
    );
  }

  // Conversion from underlying tuple values
  static Multicall3_Call3Value from_tuple(const values& t) {
    Multicall3_Call3Value s{};
    s.target = std::get<0>(t);
    s.allowFailure = std::get<1>(t);
    s.value = std::get<2>(t);
    s.callData = std::get<3>(t);
    return s;
  }
};

// ---------- Named schema for ERC20.TransferEventData ----------
struct ERC20_TransferEventData {
  abi::cpp_t<uint_t<256>> value;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<256>>;
  using values = std::tuple<abi::cpp_t<uint_t<256>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const ERC20_TransferEventData& s) {
    return std::make_tuple(
      s.value
    );
  }

  // Conversion from underlying tuple values
  static ERC20_TransferEventData from_tuple(const values& t) {
    ERC20_TransferEventData s{};
    s.value = std::get<0>(t);
    return s;
  }
};

// ---------- Named schema for ERC20.ApprovalEventData ----------
struct ERC20_ApprovalEventData {
  abi::cpp_t<uint_t<256>> value;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<256>>;
  using values = std::tuple<abi::cpp_t<uint_t<256>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const ERC20_ApprovalEventData& s) {
    return std::make_tuple(
      s.value
    );
  }

  // Conversion from underlying tuple values
  static ERC20_ApprovalEventData from_tuple(const values& t) {
    ERC20_ApprovalEventData s{};
    s.value = std::get<0>(t);
    return s;
  }
};

// ---------- Named schema for UniswapV3Pool.BurnEventData ----------
struct UniswapV3Pool_BurnEventData {
  abi::cpp_t<uint_t<128>> amount;
  abi::cpp_t<uint_t<256>> amount0;
  abi::cpp_t<uint_t<256>> amount1;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<128>, uint_t<256>, uint_t<256>>;
  using values = std::tuple<abi::cpp_t<uint_t<128>>, abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const UniswapV3Pool_BurnEventData& s) {
    return std::make_tuple(
      s.amount,
      s.amount0,
      s.amount1
    );
  }

  // Conversion from underlying tuple values
  static UniswapV3Pool_BurnEventData from_tuple(const values& t) {
    UniswapV3Pool_BurnEventData s{};
    s.amount = std::get<0>(t);
    s.amount0 = std::get<1>(t);
    s.amount1 = std::get<2>(t);
    return s;
  }
};

// ---------- Named schema for UniswapV3Pool.CollectEventData ----------
struct UniswapV3Pool_CollectEventData {
  abi::cpp_t<address20> recipient;
  abi::cpp_t<uint_t<128>> amount0;
  abi::cpp_t<uint_t<128>> amount1;

  // Underlying ABI schema
  using schema = abi::tuple<address20, uint_t<128>, uint_t<128>>;
  using values = std::tuple<abi::cpp_t<address20>, abi::cpp_t<uint_t<128>>, abi::cpp_t<uint_t<128>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const UniswapV3Pool_CollectEventData& s) {
    return std::make_tuple(
      s.recipient,
      s.amount0,
      s.amount1
    );
  }

  // Conversion from underlying tuple values
  static UniswapV3Pool_CollectEventData from_tuple(const values& t) {
    UniswapV3Pool_CollectEventData s{};
    s.recipient = std::get<0>(t);
    s.amount0 = std::get<1>(t);
    s.amount1 = std::get<2>(t);
    return s;
  }
};

// ---------- Named schema for UniswapV3Pool.CollectProtocolEventData ----------
struct UniswapV3Pool_CollectProtocolEventData {
  abi::cpp_t<uint_t<128>> amount0;
  abi::cpp_t<uint_t<128>> amount1;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<128>, uint_t<128>>;
  using values = std::tuple<abi::cpp_t<uint_t<128>>, abi::cpp_t<uint_t<128>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const UniswapV3Pool_CollectProtocolEventData& s) {
    return std::make_tuple(
      s.amount0,
      s.amount1
    );
  }

  // Conversion from underlying tuple values
  static UniswapV3Pool_CollectProtocolEventData from_tuple(const values& t) {
    UniswapV3Pool_CollectProtocolEventData s{};
    s.amount0 = std::get<0>(t);
    s.amount1 = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for UniswapV3Pool.FlashEventData ----------
struct UniswapV3Pool_FlashEventData {
  abi::cpp_t<uint_t<256>> amount0;
  abi::cpp_t<uint_t<256>> amount1;
  abi::cpp_t<uint_t<256>> paid0;
  abi::cpp_t<uint_t<256>> paid1;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<256>, uint_t<256>, uint_t<256>, uint_t<256>>;
  using values = std::tuple<abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const UniswapV3Pool_FlashEventData& s) {
    return std::make_tuple(
      s.amount0,
      s.amount1,
      s.paid0,
      s.paid1
    );
  }

  // Conversion from underlying tuple values
  static UniswapV3Pool_FlashEventData from_tuple(const values& t) {
    UniswapV3Pool_FlashEventData s{};
    s.amount0 = std::get<0>(t);
    s.amount1 = std::get<1>(t);
    s.paid0 = std::get<2>(t);
    s.paid1 = std::get<3>(t);
    return s;
  }
};

// ---------- Named schema for UniswapV3Pool.IncreaseObservationCardinalityNextEventData ----------
struct UniswapV3Pool_IncreaseObservationCardinalityNextEventData {
  abi::cpp_t<uint_t<16>> observationCardinalityNextOld;
  abi::cpp_t<uint_t<16>> observationCardinalityNextNew;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<16>, uint_t<16>>;
  using values = std::tuple<abi::cpp_t<uint_t<16>>, abi::cpp_t<uint_t<16>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const UniswapV3Pool_IncreaseObservationCardinalityNextEventData& s) {
    return std::make_tuple(
      s.observationCardinalityNextOld,
      s.observationCardinalityNextNew
    );
  }

  // Conversion from underlying tuple values
  static UniswapV3Pool_IncreaseObservationCardinalityNextEventData from_tuple(const values& t) {
    UniswapV3Pool_IncreaseObservationCardinalityNextEventData s{};
    s.observationCardinalityNextOld = std::get<0>(t);
    s.observationCardinalityNextNew = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for UniswapV3Pool.InitializeEventData ----------
struct UniswapV3Pool_InitializeEventData {
  abi::cpp_t<uint_t<160>> sqrtPriceX96;
  abi::cpp_t<int_t<24>> tick;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<160>, int_t<24>>;
  using values = std::tuple<abi::cpp_t<uint_t<160>>, abi::cpp_t<int_t<24>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const UniswapV3Pool_InitializeEventData& s) {
    return std::make_tuple(
      s.sqrtPriceX96,
      s.tick
    );
  }

  // Conversion from underlying tuple values
  static UniswapV3Pool_InitializeEventData from_tuple(const values& t) {
    UniswapV3Pool_InitializeEventData s{};
    s.sqrtPriceX96 = std::get<0>(t);
    s.tick = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for UniswapV3Pool.MintEventData ----------
struct UniswapV3Pool_MintEventData {
  abi::cpp_t<address20> sender;
  abi::cpp_t<uint_t<128>> amount;
  abi::cpp_t<uint_t<256>> amount0;
  abi::cpp_t<uint_t<256>> amount1;

  // Underlying ABI schema
  using schema = abi::tuple<address20, uint_t<128>, uint_t<256>, uint_t<256>>;
  using values = std::tuple<abi::cpp_t<address20>, abi::cpp_t<uint_t<128>>, abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const UniswapV3Pool_MintEventData& s) {
    return std::make_tuple(
      s.sender,
      s.amount,
      s.amount0,
      s.amount1
    );
  }

  // Conversion from underlying tuple values
  static UniswapV3Pool_MintEventData from_tuple(const values& t) {
    UniswapV3Pool_MintEventData s{};
    s.sender = std::get<0>(t);
    s.amount = std::get<1>(t);
    s.amount0 = std::get<2>(t);
    s.amount1 = std::get<3>(t);
    return s;
  }
};

// ---------- Named schema for UniswapV3Pool.SetFeeProtocolEventData ----------
struct UniswapV3Pool_SetFeeProtocolEventData {
  abi::cpp_t<uint_t<8>> feeProtocol0Old;
  abi::cpp_t<uint_t<8>> feeProtocol1Old;
  abi::cpp_t<uint_t<8>> feeProtocol0New;
  abi::cpp_t<uint_t<8>> feeProtocol1New;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<8>, uint_t<8>, uint_t<8>, uint_t<8>>;
  using values = std::tuple<abi::cpp_t<uint_t<8>>, abi::cpp_t<uint_t<8>>, abi::cpp_t<uint_t<8>>, abi::cpp_t<uint_t<8>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const UniswapV3Pool_SetFeeProtocolEventData& s) {
    return std::make_tuple(
      s.feeProtocol0Old,
      s.feeProtocol1Old,
      s.feeProtocol0New,
      s.feeProtocol1New
    );
  }

  // Conversion from underlying tuple values
  static UniswapV3Pool_SetFeeProtocolEventData from_tuple(const values& t) {
    UniswapV3Pool_SetFeeProtocolEventData s{};
    s.feeProtocol0Old = std::get<0>(t);
    s.feeProtocol1Old = std::get<1>(t);
    s.feeProtocol0New = std::get<2>(t);
    s.feeProtocol1New = std::get<3>(t);
    return s;
  }
};

// ---------- Named schema for UniswapV3Pool.SwapEventData ----------
struct UniswapV3Pool_SwapEventData {
  abi::cpp_t<int_t<256>> amount0;
  abi::cpp_t<int_t<256>> amount1;
  abi::cpp_t<uint_t<160>> sqrtPriceX96;
  abi::cpp_t<uint_t<128>> liquidity;
  abi::cpp_t<int_t<24>> tick;

  // Underlying ABI schema
  using schema = abi::tuple<int_t<256>, int_t<256>, uint_t<160>, uint_t<128>, int_t<24>>;
  using values = std::tuple<abi::cpp_t<int_t<256>>, abi::cpp_t<int_t<256>>, abi::cpp_t<uint_t<160>>, abi::cpp_t<uint_t<128>>, abi::cpp_t<int_t<24>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const UniswapV3Pool_SwapEventData& s) {
    return std::make_tuple(
      s.amount0,
      s.amount1,
      s.sqrtPriceX96,
      s.liquidity,
      s.tick
    );
  }

  // Conversion from underlying tuple values
  static UniswapV3Pool_SwapEventData from_tuple(const values& t) {
    UniswapV3Pool_SwapEventData s{};
    s.amount0 = std::get<0>(t);
    s.amount1 = std::get<1>(t);
    s.sqrtPriceX96 = std::get<2>(t);
    s.liquidity = std::get<3>(t);
    s.tick = std::get<4>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV3Pool.Burn ----------
struct IUniswapV3Pool_Burn {
  abi::cpp_t<uint_t<256>> amount0;
  abi::cpp_t<uint_t<256>> amount1;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<256>, uint_t<256>>;
  using values = std::tuple<abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_Burn& s) {
    return std::make_tuple(
      s.amount0,
      s.amount1
    );
  }

  // Conversion from underlying tuple values
  static IUniswapV3Pool_Burn from_tuple(const values& t) {
    IUniswapV3Pool_Burn s{};
    s.amount0 = std::get<0>(t);
    s.amount1 = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV3Pool.Collect ----------
struct IUniswapV3Pool_Collect {
  abi::cpp_t<uint_t<128>> amount0;
  abi::cpp_t<uint_t<128>> amount1;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<128>, uint_t<128>>;
  using values = std::tuple<abi::cpp_t<uint_t<128>>, abi::cpp_t<uint_t<128>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_Collect& s) {
    return std::make_tuple(
      s.amount0,
      s.amount1
    );
  }

  // Conversion from underlying tuple values
  static IUniswapV3Pool_Collect from_tuple(const values& t) {
    IUniswapV3Pool_Collect s{};
    s.amount0 = std::get<0>(t);
    s.amount1 = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV3Pool.CollectProtocol ----------
struct IUniswapV3Pool_CollectProtocol {
  abi::cpp_t<uint_t<128>> amount0;
  abi::cpp_t<uint_t<128>> amount1;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<128>, uint_t<128>>;
  using values = std::tuple<abi::cpp_t<uint_t<128>>, abi::cpp_t<uint_t<128>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_CollectProtocol& s) {
    return std::make_tuple(
      s.amount0,
      s.amount1
    );
  }

  // Conversion from underlying tuple values
  static IUniswapV3Pool_CollectProtocol from_tuple(const values& t) {
    IUniswapV3Pool_CollectProtocol s{};
    s.amount0 = std::get<0>(t);
    s.amount1 = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV3Pool.Mint ----------
struct IUniswapV3Pool_Mint {
  abi::cpp_t<uint_t<256>> amount0;
  abi::cpp_t<uint_t<256>> amount1;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<256>, uint_t<256>>;
  using values = std::tuple<abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_Mint& s) {
    return std::make_tuple(
      s.amount0,
      s.amount1
    );
  }

  // Conversion from underlying tuple values
  static IUniswapV3Pool_Mint from_tuple(const values& t) {
    IUniswapV3Pool_Mint s{};
    s.amount0 = std::get<0>(t);
    s.amount1 = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV3Pool.Observations ----------
struct IUniswapV3Pool_Observations {
  abi::cpp_t<uint_t<32>> blockTimestamp;
  abi::cpp_t<int_t<56>> tickCumulative;
  abi::cpp_t<uint_t<160>> secondsPerLiquidityCumulativeX128;
  abi::cpp_t<bool_t> initialized;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<32>, int_t<56>, uint_t<160>, bool_t>;
  using values = std::tuple<abi::cpp_t<uint_t<32>>, abi::cpp_t<int_t<56>>, abi::cpp_t<uint_t<160>>, abi::cpp_t<bool_t>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_Observations& s) {
    return std::make_tuple(
      s.blockTimestamp,
      s.tickCumulative,
      s.secondsPerLiquidityCumulativeX128,
      s.initialized
    );
  }

  // Conversion from underlying tuple values
  static IUniswapV3Pool_Observations from_tuple(const values& t) {
    IUniswapV3Pool_Observations s{};
    s.blockTimestamp = std::get<0>(t);
    s.tickCumulative = std::get<1>(t);
    s.secondsPerLiquidityCumulativeX128 = std::get<2>(t);
    s.initialized = std::get<3>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV3Pool.Observe ----------
struct IUniswapV3Pool_Observe {
  abi::cpp_t<dyn_array<int_t<56>>> tickCumulatives;
  abi::cpp_t<dyn_array<uint_t<160>>> secondsPerLiquidityCumulativeX128s;

  // Underlying ABI schema
  using schema = abi::tuple<dyn_array<int_t<56>>, dyn_array<uint_t<160>>>;
  using values = std::tuple<abi::cpp_t<dyn_array<int_t<56>>>, abi::cpp_t<dyn_array<uint_t<160>>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_Observe& s) {
    return std::make_tuple(
      s.tickCumulatives,
      s.secondsPerLiquidityCumulativeX128s
    );
  }

  // Conversion from underlying tuple values
  static IUniswapV3Pool_Observe from_tuple(const values& t) {
    IUniswapV3Pool_Observe s{};
    s.tickCumulatives = std::get<0>(t);
    s.secondsPerLiquidityCumulativeX128s = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV3Pool.Positions ----------
struct IUniswapV3Pool_Positions {
  abi::cpp_t<uint_t<128>> liquidity;
  abi::cpp_t<uint_t<256>> feeGrowthInside0LastX128;
  abi::cpp_t<uint_t<256>> feeGrowthInside1LastX128;
  abi::cpp_t<uint_t<128>> tokensOwed0;
  abi::cpp_t<uint_t<128>> tokensOwed1;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<128>, uint_t<256>, uint_t<256>, uint_t<128>, uint_t<128>>;
  using values = std::tuple<abi::cpp_t<uint_t<128>>, abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<128>>, abi::cpp_t<uint_t<128>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_Positions& s) {
    return std::make_tuple(
      s.liquidity,
      s.feeGrowthInside0LastX128,
      s.feeGrowthInside1LastX128,
      s.tokensOwed0,
      s.tokensOwed1
    );
  }

  // Conversion from underlying tuple values
  static IUniswapV3Pool_Positions from_tuple(const values& t) {
    IUniswapV3Pool_Positions s{};
    s.liquidity = std::get<0>(t);
    s.feeGrowthInside0LastX128 = std::get<1>(t);
    s.feeGrowthInside1LastX128 = std::get<2>(t);
    s.tokensOwed0 = std::get<3>(t);
    s.tokensOwed1 = std::get<4>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV3Pool.ProtocolFees ----------
struct IUniswapV3Pool_ProtocolFees {
  abi::cpp_t<uint_t<128>> token0;
  abi::cpp_t<uint_t<128>> token1;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<128>, uint_t<128>>;
  using values = std::tuple<abi::cpp_t<uint_t<128>>, abi::cpp_t<uint_t<128>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_ProtocolFees& s) {
    return std::make_tuple(
      s.token0,
      s.token1
    );
  }

  // Conversion from underlying tuple values
  static IUniswapV3Pool_ProtocolFees from_tuple(const values& t) {
    IUniswapV3Pool_ProtocolFees s{};
    s.token0 = std::get<0>(t);
    s.token1 = std::get<1>(t);
    return s;
  }
};

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
  using values = std::tuple<abi::cpp_t<uint_t<160>>, abi::cpp_t<int_t<24>>, abi::cpp_t<uint_t<16>>, abi::cpp_t<uint_t<16>>, abi::cpp_t<uint_t<16>>, abi::cpp_t<uint_t<8>>, abi::cpp_t<bool_t>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_Slot0& s) {
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

  // Conversion from underlying tuple values
  static IUniswapV3Pool_Slot0 from_tuple(const values& t) {
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

// ---------- Named schema for IUniswapV3Pool.SnapshotCumulativesInside ----------
struct IUniswapV3Pool_SnapshotCumulativesInside {
  abi::cpp_t<int_t<56>> tickCumulativeInside;
  abi::cpp_t<uint_t<160>> secondsPerLiquidityInsideX128;
  abi::cpp_t<uint_t<32>> secondsInside;

  // Underlying ABI schema
  using schema = abi::tuple<int_t<56>, uint_t<160>, uint_t<32>>;
  using values = std::tuple<abi::cpp_t<int_t<56>>, abi::cpp_t<uint_t<160>>, abi::cpp_t<uint_t<32>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_SnapshotCumulativesInside& s) {
    return std::make_tuple(
      s.tickCumulativeInside,
      s.secondsPerLiquidityInsideX128,
      s.secondsInside
    );
  }

  // Conversion from underlying tuple values
  static IUniswapV3Pool_SnapshotCumulativesInside from_tuple(const values& t) {
    IUniswapV3Pool_SnapshotCumulativesInside s{};
    s.tickCumulativeInside = std::get<0>(t);
    s.secondsPerLiquidityInsideX128 = std::get<1>(t);
    s.secondsInside = std::get<2>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV3Pool.Swap ----------
struct IUniswapV3Pool_Swap {
  abi::cpp_t<int_t<256>> amount0;
  abi::cpp_t<int_t<256>> amount1;

  // Underlying ABI schema
  using schema = abi::tuple<int_t<256>, int_t<256>>;
  using values = std::tuple<abi::cpp_t<int_t<256>>, abi::cpp_t<int_t<256>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_Swap& s) {
    return std::make_tuple(
      s.amount0,
      s.amount1
    );
  }

  // Conversion from underlying tuple values
  static IUniswapV3Pool_Swap from_tuple(const values& t) {
    IUniswapV3Pool_Swap s{};
    s.amount0 = std::get<0>(t);
    s.amount1 = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV3Pool.Ticks ----------
struct IUniswapV3Pool_Ticks {
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
  using values = std::tuple<abi::cpp_t<uint_t<128>>, abi::cpp_t<int_t<128>>, abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>, abi::cpp_t<int_t<56>>, abi::cpp_t<uint_t<160>>, abi::cpp_t<uint_t<32>>, abi::cpp_t<bool_t>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV3Pool_Ticks& s) {
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

  // Conversion from underlying tuple values
  static IUniswapV3Pool_Ticks from_tuple(const values& t) {
    IUniswapV3Pool_Ticks s{};
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

// ---------- Named schema for IMulticall.Aggregate ----------
struct IMulticall_Aggregate {
  abi::cpp_t<uint_t<256>> blockNumber;
  abi::cpp_t<dyn_array<bytes>> returnData;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<256>, dyn_array<bytes>>;
  using values = std::tuple<abi::cpp_t<uint_t<256>>, abi::cpp_t<dyn_array<bytes>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IMulticall_Aggregate& s) {
    return std::make_tuple(
      s.blockNumber,
      s.returnData
    );
  }

  // Conversion from underlying tuple values
  static IMulticall_Aggregate from_tuple(const values& t) {
    IMulticall_Aggregate s{};
    s.blockNumber = std::get<0>(t);
    s.returnData = std::get<1>(t);
    return s;
  }
};

// ---------- Named schema for IMulticall.BlockAndAggregate ----------
struct IMulticall_BlockAndAggregate {
  abi::cpp_t<uint_t<256>> blockNumber;
  abi::cpp_t<uint_t<256>> blockHash;
  std::vector<Multicall3_Result> returnData;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<256>, uint_t<256>, dyn_array<Multicall3_Result>>;
  using values = std::tuple<abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>, std::vector<Multicall3_Result>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IMulticall_BlockAndAggregate& s) {
    return std::make_tuple(
      s.blockNumber,
      s.blockHash,
      s.returnData
    );
  }

  // Conversion from underlying tuple values
  static IMulticall_BlockAndAggregate from_tuple(const values& t) {
    IMulticall_BlockAndAggregate s{};
    s.blockNumber = std::get<0>(t);
    s.blockHash = std::get<1>(t);
    s.returnData = std::get<2>(t);
    return s;
  }
};

// ---------- Named schema for IMulticall.TryBlockAndAggregate ----------
struct IMulticall_TryBlockAndAggregate {
  abi::cpp_t<uint_t<256>> blockNumber;
  abi::cpp_t<uint_t<256>> blockHash;
  std::vector<Multicall3_Result> returnData;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<256>, uint_t<256>, dyn_array<Multicall3_Result>>;
  using values = std::tuple<abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>, std::vector<Multicall3_Result>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IMulticall_TryBlockAndAggregate& s) {
    return std::make_tuple(
      s.blockNumber,
      s.blockHash,
      s.returnData
    );
  }

  // Conversion from underlying tuple values
  static IMulticall_TryBlockAndAggregate from_tuple(const values& t) {
    IMulticall_TryBlockAndAggregate s{};
    s.blockNumber = std::get<0>(t);
    s.blockHash = std::get<1>(t);
    s.returnData = std::get<2>(t);
    return s;
  }
};

// ---------- Named schema for IUniswapV2Router.AddLiquidity ----------
struct IUniswapV2Router_AddLiquidity {
  abi::cpp_t<uint_t<256>> amountA;
  abi::cpp_t<uint_t<256>> amountB;
  abi::cpp_t<uint_t<256>> liquidity;

  // Underlying ABI schema
  using schema = abi::tuple<uint_t<256>, uint_t<256>, uint_t<256>>;
  using values = std::tuple<abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>, abi::cpp_t<uint_t<256>>>;

  // Conversion to underlying tuple values
  static values to_tuple(const IUniswapV2Router_AddLiquidity& s) {
    return std::make_tuple(
      s.amountA,
      s.amountB,
      s.liquidity
    );
  }

  // Conversion from underlying tuple values
  static IUniswapV2Router_AddLiquidity from_tuple(const values& t) {
    IUniswapV2Router_AddLiquidity s{};
    s.amountA = std::get<0>(t);
    s.amountB = std::get<1>(t);
    s.liquidity = std::get<2>(t);
    return s;
  }
};



} // namespace protocols

// ==============================
// Traits specializations for named structs (in abi namespace)
// ==============================

// value_of specializations for named structs
template<> struct value_of<protocols::ITickLens_PopulatedTick> { using type = protocols::ITickLens_PopulatedTick; };
template<> struct value_of<protocols::Multicall3_Call> { using type = protocols::Multicall3_Call; };
template<> struct value_of<protocols::Multicall3_Call3> { using type = protocols::Multicall3_Call3; };
template<> struct value_of<protocols::Multicall3_Result> { using type = protocols::Multicall3_Result; };
template<> struct value_of<protocols::Multicall3_Call3Value> { using type = protocols::Multicall3_Call3Value; };
template<> struct value_of<protocols::ERC20_TransferEventData> { using type = protocols::ERC20_TransferEventData; };
template<> struct value_of<protocols::ERC20_ApprovalEventData> { using type = protocols::ERC20_ApprovalEventData; };
template<> struct value_of<protocols::UniswapV3Pool_BurnEventData> { using type = protocols::UniswapV3Pool_BurnEventData; };
template<> struct value_of<protocols::UniswapV3Pool_CollectEventData> { using type = protocols::UniswapV3Pool_CollectEventData; };
template<> struct value_of<protocols::UniswapV3Pool_CollectProtocolEventData> { using type = protocols::UniswapV3Pool_CollectProtocolEventData; };
template<> struct value_of<protocols::UniswapV3Pool_FlashEventData> { using type = protocols::UniswapV3Pool_FlashEventData; };
template<> struct value_of<protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData> { using type = protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData; };
template<> struct value_of<protocols::UniswapV3Pool_InitializeEventData> { using type = protocols::UniswapV3Pool_InitializeEventData; };
template<> struct value_of<protocols::UniswapV3Pool_MintEventData> { using type = protocols::UniswapV3Pool_MintEventData; };
template<> struct value_of<protocols::UniswapV3Pool_SetFeeProtocolEventData> { using type = protocols::UniswapV3Pool_SetFeeProtocolEventData; };
template<> struct value_of<protocols::UniswapV3Pool_SwapEventData> { using type = protocols::UniswapV3Pool_SwapEventData; };
template<> struct value_of<protocols::IUniswapV3Pool_Burn> { using type = protocols::IUniswapV3Pool_Burn; };
template<> struct value_of<protocols::IUniswapV3Pool_Collect> { using type = protocols::IUniswapV3Pool_Collect; };
template<> struct value_of<protocols::IUniswapV3Pool_CollectProtocol> { using type = protocols::IUniswapV3Pool_CollectProtocol; };
template<> struct value_of<protocols::IUniswapV3Pool_Mint> { using type = protocols::IUniswapV3Pool_Mint; };
template<> struct value_of<protocols::IUniswapV3Pool_Observations> { using type = protocols::IUniswapV3Pool_Observations; };
template<> struct value_of<protocols::IUniswapV3Pool_Observe> { using type = protocols::IUniswapV3Pool_Observe; };
template<> struct value_of<protocols::IUniswapV3Pool_Positions> { using type = protocols::IUniswapV3Pool_Positions; };
template<> struct value_of<protocols::IUniswapV3Pool_ProtocolFees> { using type = protocols::IUniswapV3Pool_ProtocolFees; };
template<> struct value_of<protocols::IUniswapV3Pool_Slot0> { using type = protocols::IUniswapV3Pool_Slot0; };
template<> struct value_of<protocols::IUniswapV3Pool_SnapshotCumulativesInside> { using type = protocols::IUniswapV3Pool_SnapshotCumulativesInside; };
template<> struct value_of<protocols::IUniswapV3Pool_Swap> { using type = protocols::IUniswapV3Pool_Swap; };
template<> struct value_of<protocols::IUniswapV3Pool_Ticks> { using type = protocols::IUniswapV3Pool_Ticks; };
template<> struct value_of<protocols::IMulticall_Aggregate> { using type = protocols::IMulticall_Aggregate; };
template<> struct value_of<protocols::IMulticall_BlockAndAggregate> { using type = protocols::IMulticall_BlockAndAggregate; };
template<> struct value_of<protocols::IMulticall_TryBlockAndAggregate> { using type = protocols::IMulticall_TryBlockAndAggregate; };
template<> struct value_of<protocols::IUniswapV2Router_AddLiquidity> { using type = protocols::IUniswapV2Router_AddLiquidity; };

// traits specializations that delegate to underlying tuple schema
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

template<> struct traits<protocols::ERC20_TransferEventData> {
  using S = typename protocols::ERC20_TransferEventData::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::ERC20_TransferEventData& v) {
    return traits<S>::tail_size( protocols::ERC20_TransferEventData::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::ERC20_TransferEventData& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::ERC20_TransferEventData::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::ERC20_TransferEventData& v) {
    traits<S>::encode_tail(out, base, protocols::ERC20_TransferEventData::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::ERC20_TransferEventData& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::ERC20_TransferEventData::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::ERC20_ApprovalEventData> {
  using S = typename protocols::ERC20_ApprovalEventData::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::ERC20_ApprovalEventData& v) {
    return traits<S>::tail_size( protocols::ERC20_ApprovalEventData::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::ERC20_ApprovalEventData& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::ERC20_ApprovalEventData::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::ERC20_ApprovalEventData& v) {
    traits<S>::encode_tail(out, base, protocols::ERC20_ApprovalEventData::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::ERC20_ApprovalEventData& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::ERC20_ApprovalEventData::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::UniswapV3Pool_BurnEventData> {
  using S = typename protocols::UniswapV3Pool_BurnEventData::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::UniswapV3Pool_BurnEventData& v) {
    return traits<S>::tail_size( protocols::UniswapV3Pool_BurnEventData::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::UniswapV3Pool_BurnEventData& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::UniswapV3Pool_BurnEventData::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::UniswapV3Pool_BurnEventData& v) {
    traits<S>::encode_tail(out, base, protocols::UniswapV3Pool_BurnEventData::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::UniswapV3Pool_BurnEventData& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::UniswapV3Pool_BurnEventData::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::UniswapV3Pool_CollectEventData> {
  using S = typename protocols::UniswapV3Pool_CollectEventData::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::UniswapV3Pool_CollectEventData& v) {
    return traits<S>::tail_size( protocols::UniswapV3Pool_CollectEventData::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::UniswapV3Pool_CollectEventData& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::UniswapV3Pool_CollectEventData::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::UniswapV3Pool_CollectEventData& v) {
    traits<S>::encode_tail(out, base, protocols::UniswapV3Pool_CollectEventData::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::UniswapV3Pool_CollectEventData& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::UniswapV3Pool_CollectEventData::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::UniswapV3Pool_CollectProtocolEventData> {
  using S = typename protocols::UniswapV3Pool_CollectProtocolEventData::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::UniswapV3Pool_CollectProtocolEventData& v) {
    return traits<S>::tail_size( protocols::UniswapV3Pool_CollectProtocolEventData::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::UniswapV3Pool_CollectProtocolEventData& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::UniswapV3Pool_CollectProtocolEventData::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::UniswapV3Pool_CollectProtocolEventData& v) {
    traits<S>::encode_tail(out, base, protocols::UniswapV3Pool_CollectProtocolEventData::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::UniswapV3Pool_CollectProtocolEventData& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::UniswapV3Pool_CollectProtocolEventData::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::UniswapV3Pool_FlashEventData> {
  using S = typename protocols::UniswapV3Pool_FlashEventData::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::UniswapV3Pool_FlashEventData& v) {
    return traits<S>::tail_size( protocols::UniswapV3Pool_FlashEventData::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::UniswapV3Pool_FlashEventData& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::UniswapV3Pool_FlashEventData::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::UniswapV3Pool_FlashEventData& v) {
    traits<S>::encode_tail(out, base, protocols::UniswapV3Pool_FlashEventData::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::UniswapV3Pool_FlashEventData& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::UniswapV3Pool_FlashEventData::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData> {
  using S = typename protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData& v) {
    return traits<S>::tail_size( protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData& v) {
    traits<S>::encode_tail(out, base, protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::UniswapV3Pool_IncreaseObservationCardinalityNextEventData::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::UniswapV3Pool_InitializeEventData> {
  using S = typename protocols::UniswapV3Pool_InitializeEventData::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::UniswapV3Pool_InitializeEventData& v) {
    return traits<S>::tail_size( protocols::UniswapV3Pool_InitializeEventData::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::UniswapV3Pool_InitializeEventData& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::UniswapV3Pool_InitializeEventData::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::UniswapV3Pool_InitializeEventData& v) {
    traits<S>::encode_tail(out, base, protocols::UniswapV3Pool_InitializeEventData::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::UniswapV3Pool_InitializeEventData& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::UniswapV3Pool_InitializeEventData::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::UniswapV3Pool_MintEventData> {
  using S = typename protocols::UniswapV3Pool_MintEventData::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::UniswapV3Pool_MintEventData& v) {
    return traits<S>::tail_size( protocols::UniswapV3Pool_MintEventData::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::UniswapV3Pool_MintEventData& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::UniswapV3Pool_MintEventData::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::UniswapV3Pool_MintEventData& v) {
    traits<S>::encode_tail(out, base, protocols::UniswapV3Pool_MintEventData::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::UniswapV3Pool_MintEventData& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::UniswapV3Pool_MintEventData::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::UniswapV3Pool_SetFeeProtocolEventData> {
  using S = typename protocols::UniswapV3Pool_SetFeeProtocolEventData::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::UniswapV3Pool_SetFeeProtocolEventData& v) {
    return traits<S>::tail_size( protocols::UniswapV3Pool_SetFeeProtocolEventData::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::UniswapV3Pool_SetFeeProtocolEventData& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::UniswapV3Pool_SetFeeProtocolEventData::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::UniswapV3Pool_SetFeeProtocolEventData& v) {
    traits<S>::encode_tail(out, base, protocols::UniswapV3Pool_SetFeeProtocolEventData::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::UniswapV3Pool_SetFeeProtocolEventData& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::UniswapV3Pool_SetFeeProtocolEventData::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::UniswapV3Pool_SwapEventData> {
  using S = typename protocols::UniswapV3Pool_SwapEventData::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::UniswapV3Pool_SwapEventData& v) {
    return traits<S>::tail_size( protocols::UniswapV3Pool_SwapEventData::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::UniswapV3Pool_SwapEventData& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::UniswapV3Pool_SwapEventData::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::UniswapV3Pool_SwapEventData& v) {
    traits<S>::encode_tail(out, base, protocols::UniswapV3Pool_SwapEventData::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::UniswapV3Pool_SwapEventData& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::UniswapV3Pool_SwapEventData::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV3Pool_Burn> {
  using S = typename protocols::IUniswapV3Pool_Burn::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_Burn& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_Burn::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_Burn& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_Burn::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_Burn& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_Burn::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_Burn& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_Burn::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV3Pool_Collect> {
  using S = typename protocols::IUniswapV3Pool_Collect::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_Collect& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_Collect::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_Collect& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_Collect::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_Collect& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_Collect::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_Collect& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_Collect::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV3Pool_CollectProtocol> {
  using S = typename protocols::IUniswapV3Pool_CollectProtocol::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_CollectProtocol& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_CollectProtocol::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_CollectProtocol& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_CollectProtocol::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_CollectProtocol& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_CollectProtocol::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_CollectProtocol& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_CollectProtocol::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV3Pool_Mint> {
  using S = typename protocols::IUniswapV3Pool_Mint::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_Mint& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_Mint::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_Mint& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_Mint::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_Mint& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_Mint::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_Mint& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_Mint::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV3Pool_Observations> {
  using S = typename protocols::IUniswapV3Pool_Observations::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_Observations& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_Observations::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_Observations& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_Observations::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_Observations& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_Observations::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_Observations& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_Observations::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV3Pool_Observe> {
  using S = typename protocols::IUniswapV3Pool_Observe::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_Observe& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_Observe::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_Observe& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_Observe::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_Observe& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_Observe::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_Observe& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_Observe::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV3Pool_Positions> {
  using S = typename protocols::IUniswapV3Pool_Positions::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_Positions& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_Positions::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_Positions& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_Positions::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_Positions& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_Positions::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_Positions& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_Positions::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV3Pool_ProtocolFees> {
  using S = typename protocols::IUniswapV3Pool_ProtocolFees::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_ProtocolFees& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_ProtocolFees::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_ProtocolFees& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_ProtocolFees::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_ProtocolFees& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_ProtocolFees::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_ProtocolFees& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_ProtocolFees::from_tuple(tmp);
    return true;
  }
};

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

template<> struct traits<protocols::IUniswapV3Pool_SnapshotCumulativesInside> {
  using S = typename protocols::IUniswapV3Pool_SnapshotCumulativesInside::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_SnapshotCumulativesInside& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_SnapshotCumulativesInside::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_SnapshotCumulativesInside& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_SnapshotCumulativesInside::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_SnapshotCumulativesInside& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_SnapshotCumulativesInside::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_SnapshotCumulativesInside& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_SnapshotCumulativesInside::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV3Pool_Swap> {
  using S = typename protocols::IUniswapV3Pool_Swap::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_Swap& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_Swap::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_Swap& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_Swap::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_Swap& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_Swap::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_Swap& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_Swap::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV3Pool_Ticks> {
  using S = typename protocols::IUniswapV3Pool_Ticks::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV3Pool_Ticks& v) {
    return traits<S>::tail_size( protocols::IUniswapV3Pool_Ticks::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV3Pool_Ticks& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV3Pool_Ticks::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV3Pool_Ticks& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV3Pool_Ticks::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV3Pool_Ticks& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV3Pool_Ticks::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IMulticall_Aggregate> {
  using S = typename protocols::IMulticall_Aggregate::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IMulticall_Aggregate& v) {
    return traits<S>::tail_size( protocols::IMulticall_Aggregate::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IMulticall_Aggregate& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IMulticall_Aggregate::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IMulticall_Aggregate& v) {
    traits<S>::encode_tail(out, base, protocols::IMulticall_Aggregate::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IMulticall_Aggregate& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IMulticall_Aggregate::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IMulticall_BlockAndAggregate> {
  using S = typename protocols::IMulticall_BlockAndAggregate::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IMulticall_BlockAndAggregate& v) {
    return traits<S>::tail_size( protocols::IMulticall_BlockAndAggregate::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IMulticall_BlockAndAggregate& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IMulticall_BlockAndAggregate::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IMulticall_BlockAndAggregate& v) {
    traits<S>::encode_tail(out, base, protocols::IMulticall_BlockAndAggregate::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IMulticall_BlockAndAggregate& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IMulticall_BlockAndAggregate::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IMulticall_TryBlockAndAggregate> {
  using S = typename protocols::IMulticall_TryBlockAndAggregate::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IMulticall_TryBlockAndAggregate& v) {
    return traits<S>::tail_size( protocols::IMulticall_TryBlockAndAggregate::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IMulticall_TryBlockAndAggregate& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IMulticall_TryBlockAndAggregate::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IMulticall_TryBlockAndAggregate& v) {
    traits<S>::encode_tail(out, base, protocols::IMulticall_TryBlockAndAggregate::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IMulticall_TryBlockAndAggregate& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IMulticall_TryBlockAndAggregate::from_tuple(tmp);
    return true;
  }
};

template<> struct traits<protocols::IUniswapV2Router_AddLiquidity> {
  using S = typename protocols::IUniswapV2Router_AddLiquidity::schema;
  static constexpr bool is_dynamic  = traits<S>::is_dynamic;
  static constexpr size_t head_words= traits<S>::head_words;

  static size_t tail_size(const protocols::IUniswapV2Router_AddLiquidity& v) {
    return traits<S>::tail_size( protocols::IUniswapV2Router_AddLiquidity::to_tuple(v) );
  }

  static void encode_head(uint8_t* out32, size_t hi, const protocols::IUniswapV2Router_AddLiquidity& v, size_t base) {
    traits<S>::encode_head(out32, hi, protocols::IUniswapV2Router_AddLiquidity::to_tuple(v), base);
  }

  static void encode_tail(uint8_t* out, size_t base, const protocols::IUniswapV2Router_AddLiquidity& v) {
    traits<S>::encode_tail(out, base, protocols::IUniswapV2Router_AddLiquidity::to_tuple(v));
  }

  static bool decode(BytesSpan in, protocols::IUniswapV2Router_AddLiquidity& out, Error* e=nullptr) {
    abi::value_of<S>::type tmp;
    if (!traits<S>::decode(in, tmp, e)) return false;
    out = protocols::IUniswapV2Router_AddLiquidity::from_tuple(tmp);
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

struct Topic_ERC20_Transfer { static constexpr std::array<uint8_t,32> value{{0xdd,0xf2,0x52,0xad,0x1b,0xe2,0xc8,0x9b,0x69,0xc2,0xb0,0x68,0xfc,0x37,0x8d,0xaa,0x95,0x2b,0xa7,0xf1,0x63,0xc4,0xa1,0x16,0x28,0xf5,0x5a,0x4d,0xf5,0x23,0xb3,0xef}}; }; // "Transfer(address,address,uint256)"
using ERC20_TransferEvent = Event<Topic_ERC20_Transfer, ERC20_TransferEventData>;

struct Topic_ERC20_Approval { static constexpr std::array<uint8_t,32> value{{0x8c,0x5b,0xe1,0xe5,0xeb,0xec,0x7d,0x5b,0xd1,0x4f,0x71,0x42,0x7d,0x1e,0x84,0xf3,0xdd,0x03,0x14,0xc0,0xf7,0xb2,0x29,0x1e,0x5b,0x20,0x0a,0xc8,0xc7,0xc3,0xb9,0x25}}; }; // "Approval(address,address,uint256)"
using ERC20_ApprovalEvent = Event<Topic_ERC20_Approval, ERC20_ApprovalEventData>;

// ==============================
// UniswapV3Pool
// ==============================

struct Topic_UniswapV3Pool_Burn { static constexpr std::array<uint8_t,32> value{{0x0c,0x39,0x6c,0xd9,0x89,0xa3,0x9f,0x44,0x59,0xb5,0xfa,0x1a,0xed,0x6a,0x9a,0x8d,0xcd,0xbc,0x45,0x90,0x8a,0xcf,0xd6,0x7e,0x02,0x8c,0xd5,0x68,0xda,0x98,0x98,0x2c}}; }; // "Burn(address,int24,int24,uint128,uint256,uint256)"
using UniswapV3Pool_BurnEvent = Event<Topic_UniswapV3Pool_Burn, UniswapV3Pool_BurnEventData>;

struct Topic_UniswapV3Pool_Collect { static constexpr std::array<uint8_t,32> value{{0x70,0x93,0x53,0x38,0xe6,0x97,0x75,0x45,0x6a,0x85,0xdd,0xef,0x22,0x6c,0x39,0x5f,0xb6,0x68,0xb6,0x3f,0xa0,0x11,0x5f,0x5f,0x20,0x61,0x0b,0x38,0x8e,0x6c,0xa9,0xc0}}; }; // "Collect(address,address,int24,int24,uint128,uint128)"
using UniswapV3Pool_CollectEvent = Event<Topic_UniswapV3Pool_Collect, UniswapV3Pool_CollectEventData>;

struct Topic_UniswapV3Pool_CollectProtocol { static constexpr std::array<uint8_t,32> value{{0x59,0x6b,0x57,0x39,0x06,0x21,0x8d,0x34,0x11,0x85,0x0b,0x26,0xa6,0xb4,0x37,0xd6,0xc4,0x52,0x2f,0xdb,0x43,0xd2,0xd2,0x38,0x62,0x63,0xf8,0x6d,0x50,0xb8,0xb1,0x51}}; }; // "CollectProtocol(address,address,uint128,uint128)"
using UniswapV3Pool_CollectProtocolEvent = Event<Topic_UniswapV3Pool_CollectProtocol, UniswapV3Pool_CollectProtocolEventData>;

struct Topic_UniswapV3Pool_Flash { static constexpr std::array<uint8_t,32> value{{0xbd,0xbd,0xb7,0x1d,0x78,0x60,0x37,0x6b,0xa5,0x2b,0x25,0xa5,0x02,0x8b,0xee,0xa2,0x35,0x81,0x36,0x4a,0x40,0x52,0x2f,0x6b,0xcf,0xb8,0x6b,0xb1,0xf2,0xdc,0xa6,0x33}}; }; // "Flash(address,address,uint256,uint256,uint256,uint256)"
using UniswapV3Pool_FlashEvent = Event<Topic_UniswapV3Pool_Flash, UniswapV3Pool_FlashEventData>;

struct Topic_UniswapV3Pool_IncreaseObservationCardinalityNext { static constexpr std::array<uint8_t,32> value{{0xac,0x49,0xe5,0x18,0xf9,0x0a,0x35,0x8f,0x65,0x2e,0x44,0x00,0x16,0x4f,0x05,0xa5,0xd8,0xf7,0xe3,0x5e,0x77,0x47,0x27,0x9b,0xc3,0xa9,0x3d,0xbf,0x58,0x4e,0x12,0x5a}}; }; // "IncreaseObservationCardinalityNext(uint16,uint16)"
using UniswapV3Pool_IncreaseObservationCardinalityNextEvent = Event<Topic_UniswapV3Pool_IncreaseObservationCardinalityNext, UniswapV3Pool_IncreaseObservationCardinalityNextEventData>;

struct Topic_UniswapV3Pool_Initialize { static constexpr std::array<uint8_t,32> value{{0x98,0x63,0x60,0x36,0xcb,0x66,0xa9,0xc1,0x9a,0x37,0x43,0x5e,0xfc,0x1e,0x90,0x14,0x21,0x90,0x21,0x4e,0x8a,0xbe,0xb8,0x21,0xbd,0xba,0x3f,0x29,0x90,0xdd,0x4c,0x95}}; }; // "Initialize(uint160,int24)"
using UniswapV3Pool_InitializeEvent = Event<Topic_UniswapV3Pool_Initialize, UniswapV3Pool_InitializeEventData>;

struct Topic_UniswapV3Pool_Mint { static constexpr std::array<uint8_t,32> value{{0x7a,0x53,0x08,0x0b,0xa4,0x14,0x15,0x8b,0xe7,0xec,0x69,0xb9,0x87,0xb5,0xfb,0x7d,0x07,0xde,0xe1,0x01,0xfe,0x85,0x48,0x8f,0x08,0x53,0xae,0x16,0x23,0x9d,0x0b,0xde}}; }; // "Mint(address,address,int24,int24,uint128,uint256,uint256)"
using UniswapV3Pool_MintEvent = Event<Topic_UniswapV3Pool_Mint, UniswapV3Pool_MintEventData>;

struct Topic_UniswapV3Pool_SetFeeProtocol { static constexpr std::array<uint8_t,32> value{{0x97,0x3d,0x8d,0x92,0xbb,0x29,0x9f,0x4a,0xf6,0xce,0x49,0xb5,0x2a,0x8a,0xdb,0x85,0xae,0x46,0xb9,0xf2,0x14,0xc4,0xc4,0xfc,0x06,0xac,0x77,0x40,0x12,0x37,0xb1,0x33}}; }; // "SetFeeProtocol(uint8,uint8,uint8,uint8)"
using UniswapV3Pool_SetFeeProtocolEvent = Event<Topic_UniswapV3Pool_SetFeeProtocol, UniswapV3Pool_SetFeeProtocolEventData>;

struct Topic_UniswapV3Pool_Swap { static constexpr std::array<uint8_t,32> value{{0xc4,0x20,0x79,0xf9,0x4a,0x63,0x50,0xd7,0xe6,0x23,0x5f,0x29,0x17,0x49,0x24,0xf9,0x28,0xcc,0x2a,0xc8,0x18,0xeb,0x64,0xfe,0xd8,0x00,0x4e,0x11,0x5f,0xbc,0xca,0x67}}; }; // "Swap(address,address,int256,int256,uint160,uint128,int24)"
using UniswapV3Pool_SwapEvent = Event<Topic_UniswapV3Pool_Swap, UniswapV3Pool_SwapEventData>;

struct Sel_UniswapV3Pool_burn { static constexpr std::array<uint8_t,4> value{{0xa3,0x41,0x23,0xa7}}; }; // "burn(int24,int24,uint128)"
using UniswapV3Pool_Burn = Fn<Sel_UniswapV3Pool_burn, IUniswapV3Pool_Burn, int_t<24>, int_t<24>, uint_t<128>>;

struct Sel_UniswapV3Pool_collect { static constexpr std::array<uint8_t,4> value{{0x4f,0x1e,0xb3,0xd8}}; }; // "collect(address,int24,int24,uint128,uint128)"
using UniswapV3Pool_Collect = Fn<Sel_UniswapV3Pool_collect, IUniswapV3Pool_Collect, address20, int_t<24>, int_t<24>, uint_t<128>, uint_t<128>>;

struct Sel_UniswapV3Pool_collectProtocol { static constexpr std::array<uint8_t,4> value{{0x85,0xb6,0x67,0x29}}; }; // "collectProtocol(address,uint128,uint128)"
using UniswapV3Pool_CollectProtocol = Fn<Sel_UniswapV3Pool_collectProtocol, IUniswapV3Pool_CollectProtocol, address20, uint_t<128>, uint_t<128>>;

struct Sel_UniswapV3Pool_factory { static constexpr std::array<uint8_t,4> value{{0xc4,0x5a,0x01,0x55}}; }; // "factory()"
using UniswapV3Pool_Factory = Fn<Sel_UniswapV3Pool_factory, address20>;

struct Sel_UniswapV3Pool_fee { static constexpr std::array<uint8_t,4> value{{0xdd,0xca,0x3f,0x43}}; }; // "fee()"
using UniswapV3Pool_Fee = Fn<Sel_UniswapV3Pool_fee, uint_t<24>>;

struct Sel_UniswapV3Pool_feeGrowthGlobal0X128 { static constexpr std::array<uint8_t,4> value{{0xf3,0x05,0x83,0x99}}; }; // "feeGrowthGlobal0X128()"
using UniswapV3Pool_FeeGrowthGlobal0X128 = Fn<Sel_UniswapV3Pool_feeGrowthGlobal0X128, uint_t<256>>;

struct Sel_UniswapV3Pool_feeGrowthGlobal1X128 { static constexpr std::array<uint8_t,4> value{{0x46,0x14,0x13,0x19}}; }; // "feeGrowthGlobal1X128()"
using UniswapV3Pool_FeeGrowthGlobal1X128 = Fn<Sel_UniswapV3Pool_feeGrowthGlobal1X128, uint_t<256>>;

struct Sel_UniswapV3Pool_flash { static constexpr std::array<uint8_t,4> value{{0x49,0x0e,0x6c,0xbc}}; }; // "flash(address,uint256,uint256,bytes)"
using UniswapV3Pool_Flash = Fn<Sel_UniswapV3Pool_flash, void, address20, uint_t<256>, uint_t<256>, bytes>;

struct Sel_UniswapV3Pool_increaseObservationCardinalityNext { static constexpr std::array<uint8_t,4> value{{0x32,0x14,0x8f,0x67}}; }; // "increaseObservationCardinalityNext(uint16)"
using UniswapV3Pool_IncreaseObservationCardinalityNext = Fn<Sel_UniswapV3Pool_increaseObservationCardinalityNext, void, uint_t<16>>;

struct Sel_UniswapV3Pool_initialize { static constexpr std::array<uint8_t,4> value{{0xf6,0x37,0x73,0x1d}}; }; // "initialize(uint160)"
using UniswapV3Pool_Initialize = Fn<Sel_UniswapV3Pool_initialize, void, uint_t<160>>;

struct Sel_UniswapV3Pool_liquidity { static constexpr std::array<uint8_t,4> value{{0x1a,0x68,0x65,0x02}}; }; // "liquidity()"
using UniswapV3Pool_Liquidity = Fn<Sel_UniswapV3Pool_liquidity, uint_t<128>>;

struct Sel_UniswapV3Pool_maxLiquidityPerTick { static constexpr std::array<uint8_t,4> value{{0x70,0xcf,0x75,0x4a}}; }; // "maxLiquidityPerTick()"
using UniswapV3Pool_MaxLiquidityPerTick = Fn<Sel_UniswapV3Pool_maxLiquidityPerTick, uint_t<128>>;

struct Sel_UniswapV3Pool_mint { static constexpr std::array<uint8_t,4> value{{0x3c,0x8a,0x7d,0x8d}}; }; // "mint(address,int24,int24,uint128,bytes)"
using UniswapV3Pool_Mint = Fn<Sel_UniswapV3Pool_mint, IUniswapV3Pool_Mint, address20, int_t<24>, int_t<24>, uint_t<128>, bytes>;

struct Sel_UniswapV3Pool_observations { static constexpr std::array<uint8_t,4> value{{0x25,0x2c,0x09,0xd7}}; }; // "observations(uint256)"
using UniswapV3Pool_Observations = Fn<Sel_UniswapV3Pool_observations, IUniswapV3Pool_Observations, uint_t<256>>;

struct Sel_UniswapV3Pool_observe { static constexpr std::array<uint8_t,4> value{{0x88,0x3b,0xdb,0xfd}}; }; // "observe(uint32[])"
using UniswapV3Pool_Observe = Fn<Sel_UniswapV3Pool_observe, IUniswapV3Pool_Observe, dyn_array<uint_t<32>>>;

struct Sel_UniswapV3Pool_positions { static constexpr std::array<uint8_t,4> value{{0x51,0x4e,0xa4,0xbf}}; }; // "positions(bytes32)"
using UniswapV3Pool_Positions = Fn<Sel_UniswapV3Pool_positions, IUniswapV3Pool_Positions, uint_t<256>>;

struct Sel_UniswapV3Pool_protocolFees { static constexpr std::array<uint8_t,4> value{{0x1a,0xd8,0xb0,0x3b}}; }; // "protocolFees()"
using UniswapV3Pool_ProtocolFees = Fn<Sel_UniswapV3Pool_protocolFees, IUniswapV3Pool_ProtocolFees>;

struct Sel_UniswapV3Pool_setFeeProtocol { static constexpr std::array<uint8_t,4> value{{0x82,0x06,0xa4,0xd1}}; }; // "setFeeProtocol(uint8,uint8)"
using UniswapV3Pool_SetFeeProtocol = Fn<Sel_UniswapV3Pool_setFeeProtocol, void, uint_t<8>, uint_t<8>>;

struct Sel_UniswapV3Pool_slot0 { static constexpr std::array<uint8_t,4> value{{0x38,0x50,0xc7,0xbd}}; }; // "slot0()"
using UniswapV3Pool_Slot0 = Fn<Sel_UniswapV3Pool_slot0, IUniswapV3Pool_Slot0>;

struct Sel_UniswapV3Pool_snapshotCumulativesInside { static constexpr std::array<uint8_t,4> value{{0xa3,0x88,0x07,0xf2}}; }; // "snapshotCumulativesInside(int24,int24)"
using UniswapV3Pool_SnapshotCumulativesInside = Fn<Sel_UniswapV3Pool_snapshotCumulativesInside, IUniswapV3Pool_SnapshotCumulativesInside, int_t<24>, int_t<24>>;

struct Sel_UniswapV3Pool_swap { static constexpr std::array<uint8_t,4> value{{0x12,0x8a,0xcb,0x08}}; }; // "swap(address,bool,int256,uint160,bytes)"
using UniswapV3Pool_Swap = Fn<Sel_UniswapV3Pool_swap, IUniswapV3Pool_Swap, address20, bool_t, int_t<256>, uint_t<160>, bytes>;

struct Sel_UniswapV3Pool_tickBitmap { static constexpr std::array<uint8_t,4> value{{0x53,0x39,0xc2,0x96}}; }; // "tickBitmap(int16)"
using UniswapV3Pool_TickBitmap = Fn<Sel_UniswapV3Pool_tickBitmap, uint_t<256>, int_t<16>>;

struct Sel_UniswapV3Pool_tickSpacing { static constexpr std::array<uint8_t,4> value{{0xd0,0xc9,0x3a,0x7c}}; }; // "tickSpacing()"
using UniswapV3Pool_TickSpacing = Fn<Sel_UniswapV3Pool_tickSpacing, int_t<24>>;

struct Sel_UniswapV3Pool_ticks { static constexpr std::array<uint8_t,4> value{{0xf3,0x0d,0xba,0x93}}; }; // "ticks(int24)"
using UniswapV3Pool_Ticks = Fn<Sel_UniswapV3Pool_ticks, IUniswapV3Pool_Ticks, int_t<24>>;

struct Sel_UniswapV3Pool_token0 { static constexpr std::array<uint8_t,4> value{{0x0d,0xfe,0x16,0x81}}; }; // "token0()"
using UniswapV3Pool_Token0 = Fn<Sel_UniswapV3Pool_token0, address20>;

struct Sel_UniswapV3Pool_token1 { static constexpr std::array<uint8_t,4> value{{0xd2,0x12,0x20,0xa7}}; }; // "token1()"
using UniswapV3Pool_Token1 = Fn<Sel_UniswapV3Pool_token1, address20>;

// ==============================
// UniswapV3TickLens
// ==============================

struct Sel_UniswapV3TickLens_getPopulatedTicksInWord { static constexpr std::array<uint8_t,4> value{{0x35,0x1f,0xb4,0x78}}; }; // "getPopulatedTicksInWord(address,int16)"
using UniswapV3TickLens_GetPopulatedTicksInWord = Fn<Sel_UniswapV3TickLens_getPopulatedTicksInWord, dyn_array<ITickLens_PopulatedTick>, address20, int_t<16>>;

// ==============================
// Multicall
// ==============================

struct Sel_Multicall_aggregate { static constexpr std::array<uint8_t,4> value{{0x25,0x2d,0xba,0x42}}; }; // "aggregate((address,bytes)[])"
using Multicall_Aggregate = Fn<Sel_Multicall_aggregate, IMulticall_Aggregate, dyn_array<Multicall3_Call>>;

struct Sel_Multicall_aggregate3 { static constexpr std::array<uint8_t,4> value{{0x82,0xad,0x56,0xcb}}; }; // "aggregate3((address,bool,bytes)[])"
using Multicall_Aggregate3 = Fn<Sel_Multicall_aggregate3, dyn_array<Multicall3_Result>, dyn_array<Multicall3_Call3>>;

struct Sel_Multicall_aggregate3Value { static constexpr std::array<uint8_t,4> value{{0x17,0x4d,0xea,0x71}}; }; // "aggregate3Value((address,bool,uint256,bytes)[])"
using Multicall_Aggregate3Value = Fn<Sel_Multicall_aggregate3Value, dyn_array<Multicall3_Result>, dyn_array<Multicall3_Call3Value>>;

struct Sel_Multicall_blockAndAggregate { static constexpr std::array<uint8_t,4> value{{0xc3,0x07,0x7f,0xa9}}; }; // "blockAndAggregate((address,bytes)[])"
using Multicall_BlockAndAggregate = Fn<Sel_Multicall_blockAndAggregate, IMulticall_BlockAndAggregate, dyn_array<Multicall3_Call>>;

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
using Multicall_TryBlockAndAggregate = Fn<Sel_Multicall_tryBlockAndAggregate, IMulticall_TryBlockAndAggregate, bool_t, dyn_array<Multicall3_Call>>;

// ==============================
// UniswapV2Router
// ==============================

struct Sel_UniswapV2Router_addLiquidity { static constexpr std::array<uint8_t,4> value{{0xe8,0xe3,0x37,0x00}}; }; // "addLiquidity(address,address,uint256,uint256,uint256,uint256,address,uint256)"
using UniswapV2Router_AddLiquidity = Fn<Sel_UniswapV2Router_addLiquidity, IUniswapV2Router_AddLiquidity, address20, address20, uint_t<256>, uint_t<256>, uint_t<256>, uint_t<256>, address20, uint_t<256>>;

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
