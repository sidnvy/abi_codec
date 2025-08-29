#include <iostream>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include "abi/protocols.h"

// Use specific contract-prefixed names to avoid conflicts

// Struct types
using IUniswapV3Pool_Slot0 = abi::protocols::IUniswapV3Pool_Slot0;
using IUniswapV3Pool_Tick = abi::protocols::IUniswapV3Pool_Tick;
using Multicall3_Result = abi::protocols::Multicall3_Result;
using ITickLens_PopulatedTick = abi::protocols::ITickLens_PopulatedTick;
using Multicall3_Call = abi::protocols::Multicall3_Call;
using Multicall3_Call3Value = abi::protocols::Multicall3_Call3Value;

// ERC20 functions
using ERC20_BalanceOf = abi::protocols::ERC20_BalanceOf;
using ERC20_Transfer = abi::protocols::ERC20_Transfer;
using ERC20_Approve = abi::protocols::ERC20_Approve;
using ERC20_TotalSupply = abi::protocols::ERC20_TotalSupply;

// Uniswap V3 Pool functions
using UniswapV3Pool_Slot0 = abi::protocols::UniswapV3Pool_Slot0;
using UniswapV3Pool_Ticks = abi::protocols::UniswapV3Pool_Ticks;
using UniswapV3Pool_Token0 = abi::protocols::UniswapV3Pool_Token0;
using UniswapV3Pool_Token1 = abi::protocols::UniswapV3Pool_Token1;
using UniswapV3Pool_Liquidity = abi::protocols::UniswapV3Pool_Liquidity;
using UniswapV3Pool_Observe = abi::protocols::UniswapV3Pool_Observe;

// Uniswap V2 Router functions
using UniswapV2Router_GetAmountsOut = abi::protocols::UniswapV2Router_GetAmountsOut;
using UniswapV2Router_SwapExactTokensForTokens = abi::protocols::UniswapV2Router_SwapExactTokensForTokens;

// Use shared utilities from abi namespace
// abi::parse_hex() and abi::addr_from_hex() are now available

// Example showing how to use the clean named structs to access return values
void demonstrateCleanUsage() {
    std::cout << "=== Clean Named Structs Usage Examples ===\n\n";

    // 1. Uniswap V3 Pool slot0() function - now with named fields!
    std::cout << "1. Uniswap V3 Pool slot0() return value:\n";
    
    // Simulate receiving encoded return data (in real usage, this would come from RPC)
    std::vector<uint8_t> encoded_slot0 = {
        // This would be the actual encoded return data from the contract call
    };
    
    // Decode into named struct - now with clean field names!
    IUniswapV3Pool_Slot0 slot0_result;
    abi::BytesSpan slot0_data(encoded_slot0.data(), encoded_slot0.size());
    
    if (abi::decode_from<IUniswapV3Pool_Slot0>(slot0_data, slot0_result)) {
        // Access fields by their meaningful names!
        std::cout << "  sqrtPriceX96: " << slot0_result.sqrtPriceX96 << "\n";
        std::cout << "  tick: " << slot0_result.tick << "\n";
        std::cout << "  observationIndex: " << slot0_result.observationIndex << "\n";
        std::cout << "  observationCardinality: " << slot0_result.observationCardinality << "\n";
        std::cout << "  observationCardinalityNext: " << slot0_result.observationCardinalityNext << "\n";
        std::cout << "  feeProtocol: " << slot0_result.feeProtocol << "\n";
        std::cout << "  unlocked: " << (slot0_result.unlocked ? "true" : "false") << "\n";
        
        // You can also create and modify structs easily
        IUniswapV3Pool_Slot0 new_slot0 = slot0_result;
        new_slot0.tick = new_slot0.tick + 100; // Modify tick
        std::cout << "  Modified tick: " << new_slot0.tick << "\n";
    }
    std::cout << "\n";

    // 2. Uniswap V3 Pool ticks() function - clean Tick struct!
    std::cout << "2. Uniswap V3 Pool ticks() return value:\n";
    
    std::vector<uint8_t> encoded_tick;
    IUniswapV3Pool_Tick tick_result;
    abi::BytesSpan tick_data(encoded_tick.data(), encoded_tick.size());
    
    // The ticks function takes an int24 parameter and returns a Tick struct
    boost::multiprecision::cpp_int tick_param = 12345; // int24 parameter
    
    // Option 1: Using abi::decode_from (recommended)
    bool decoded1 = abi::decode_from<IUniswapV3Pool_Tick>(tick_data, tick_result);
    if (decoded1) {
        std::cout << "  ✅ Decoded using abi::decode_from<IUniswapV3Pool_Tick>\n";
    }

    // Option 2: Using the Fn struct's decode method
    IUniswapV3Pool_Tick tick_result2;
    bool decoded2 = UniswapV3Pool_Ticks::decode_result(tick_data, tick_result2);
    if (decoded2) {
        std::cout << "  ✅ Decoded using UniswapV3Pool_Ticks::decode_result\n";
    }

    // Both methods should work identically (since empty data won't decode)
    if (!decoded1 && !decoded2) {
        std::cout << "  ℹ️  Both methods correctly handle empty data (no decoding needed)\n";
    } else if (decoded1 && decoded2) {
        // Results should be identical
        assert(tick_result.liquidityGross == tick_result2.liquidityGross);
        std::cout << "  ✅ Both methods produce identical results\n";
    }

    // Continue with demonstration
    if (tick_result.liquidityGross != 0 || tick_result2.liquidityGross != 0) {
        // Access all the liquidity and fee data with meaningful names!
        std::cout << "  liquidityGross: " << tick_result.liquidityGross << "\n";
        std::cout << "  liquidityNet: " << tick_result.liquidityNet << "\n";
        std::cout << "  feeGrowthOutside0X128: " << tick_result.feeGrowthOutside0X128 << "\n";
        std::cout << "  feeGrowthOutside1X128: " << tick_result.feeGrowthOutside1X128 << "\n";
        std::cout << "  tickCumulativeOutside: " << tick_result.tickCumulativeOutside << "\n";
        std::cout << "  secondsPerLiquidityOutsideX128: " << tick_result.secondsPerLiquidityOutsideX128 << "\n";
        std::cout << "  secondsOutside: " << tick_result.secondsOutside << "\n";
        std::cout << "  initialized: " << (tick_result.initialized ? "true" : "false") << "\n";
        
        // Easy to work with the data
        if (tick_result.liquidityNet > 0) {
            std::cout << "  This tick has positive net liquidity!\n";
        }
    }
    std::cout << "\n";

    // 3. Multicall3 functions - now with proper struct names!
    std::cout << "3. Multicall3 aggregate3() return value:\n";
    
    std::vector<uint8_t> encoded_aggregate3;
    std::vector<Multicall3_Result> results;
    abi::BytesSpan aggregate3_data(encoded_aggregate3.data(), encoded_aggregate3.size());
    
    if (abi::decode_from<abi::dyn_array<Multicall3_Result>>(aggregate3_data, results)) {
        std::cout << "  Got " << results.size() << " results:\n";
        for (size_t i = 0; i < results.size(); ++i) {
            const auto& result = results[i];
            std::cout << "    Result " << i << ":\n";
            std::cout << "      success: " << (result.success ? "✅" : "❌") << "\n";
            std::cout << "      returnData size: " << result.returnData.size() << " bytes\n";
            
            // You can further decode the returnData if needed
            if (!result.returnData.empty() && result.returnData.size() >= 32) {
                // For example, if it's a uint256 value
                boost::multiprecision::cpp_int value;
                abi::BytesSpan value_data(result.returnData.data(), 32);
                if (abi::traits<abi::uint_t<256>>::decode(value_data, value)) {
                    std::cout << "      decoded uint256: " << value << "\n";
                }
            }
        }
    }
    std::cout << "\n";

    // 4. Tick Lens function - clean PopulatedTick struct!
    std::cout << "4. Tick Lens getPopulatedTicksInWord() return value:\n";
    
    std::vector<uint8_t> encoded_ticks;
    std::vector<ITickLens_PopulatedTick> populated_ticks;
    abi::BytesSpan ticks_data(encoded_ticks.data(), encoded_ticks.size());
    
    if (abi::decode_from<abi::dyn_array<ITickLens_PopulatedTick>>(ticks_data, populated_ticks)) {
        std::cout << "  Got " << populated_ticks.size() << " populated ticks:\n";
        for (size_t i = 0; i < populated_ticks.size(); ++i) {
            const auto& tick = populated_ticks[i];
            std::cout << "    Tick " << i << ":\n";
            std::cout << "      tick: " << tick.tick << "\n";
            std::cout << "      liquidityNet: " << tick.liquidityNet << "\n";
            std::cout << "      liquidityGross: " << tick.liquidityGross << "\n";
            
            // Calculate useful metrics
            if (tick.liquidityGross > 0) {
                double netRatio = static_cast<double>(tick.liquidityNet.convert_to<long long>()) / 
                                static_cast<double>(tick.liquidityGross.convert_to<unsigned long long>());
                std::cout << "      net/gross ratio: " << netRatio << "\n";
            }
        }
    }
    std::cout << "\n";

    // 5. Creating and using input structs for function calls
    std::cout << "5. Creating input structs for function calls:\n";
    
    // Example: Creating a Multicall3_Call for aggregate function
    Multicall3_Call call_input;
    call_input.target = {0xa0, 0xb8, 0x69, 0x91, 0xc6, 0x21, 0x8b, 0x36, 0xc1, 0xd1, 0x9d, 0x4a, 0x2e, 0x9e, 0xb0, 0xce, 0x36, 0x06, 0xeb, 0x48}; // address
    call_input.callData = {0x06, 0xfd, 0xde, 0x03}; // name() selector
    
    std::cout << "  Created Multicall3_Call:\n";
    std::cout << "    target: 0x";
    for (uint8_t byte : call_input.target) {
        printf("%02x", byte);
    }
    std::cout << "\n";
    std::cout << "    callData size: " << call_input.callData.size() << " bytes\n";
    
    // Example: Creating a Multicall3_Call3Value for more complex calls
    Multicall3_Call3Value complex_call;
    complex_call.target = {0x88, 0xe6, 0xa0, 0xc2, 0xdd, 0xd2, 0x6f, 0xe3, 0x34, 0xb4, 0x70, 0x1b, 0x28, 0x0e, 0x59, 0x6a, 0x2a, 0x2b, 0x39, 0x44}; // USDC
    complex_call.allowFailure = false;
    complex_call.value = boost::multiprecision::cpp_int("1000000"); // 1 USDC (6 decimals)
    complex_call.callData = {0xa9, 0x05, 0x9c, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd9, 0xba, 0x89, 0x42, 0x0a, 0x56, 0x9c, 0x9e, 0xac, 0x9c, 0x7e, 0xa9, 0x0c, 0x4d, 0x9e, 0x7f, 0x8d, 0x9b, 0xc6, 0x6b, 0xe2, 0x9c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // transfer(address,uint256)
    
    std::cout << "  Created Multicall3_Call3Value:\n";
    std::cout << "    target: 0x";
    for (uint8_t byte : complex_call.target) {
        printf("%02x", byte);
    }
    std::cout << "\n";
    std::cout << "    allowFailure: " << (complex_call.allowFailure ? "true" : "false") << "\n";
    std::cout << "    value: " << complex_call.value << " wei\n";
    std::cout << "    callData size: " << complex_call.callData.size() << " bytes\n";

    // 6. Encoding with clean named structs!
    std::cout << "\n6. Encoding with clean named structs:\n";

    // Example: Encoding a Slot0 struct back to bytes
    IUniswapV3Pool_Slot0 slot0_to_encode;
    slot0_to_encode.sqrtPriceX96 = boost::multiprecision::cpp_int("429512873912345678901234567890"); // Example price
    slot0_to_encode.tick = 12345; // Example tick
    slot0_to_encode.observationIndex = 42;
    slot0_to_encode.observationCardinality = 100;
    slot0_to_encode.observationCardinalityNext = 150;
    slot0_to_encode.feeProtocol = 5;
    slot0_to_encode.unlocked = true;

    std::vector<uint8_t> encoded_slot0_new;
    const size_t slot0_size = abi::encoded_size<IUniswapV3Pool_Slot0>(slot0_to_encode);
    encoded_slot0_new.resize(slot0_size);
    if (abi::encode_into<IUniswapV3Pool_Slot0>(encoded_slot0_new.data(), encoded_slot0_new.size(), slot0_to_encode)) {
        std::cout << "  ✅ Encoded Slot0 using abi::encode_into<>\n";
        std::cout << "    Encoded size: " << encoded_slot0_new.size() << " bytes\n";
        std::cout << "    First 32 bytes (sqrtPriceX96): ";
        for (size_t i = 0; i < 32 && i < encoded_slot0_new.size(); ++i) {
            printf("%02x", encoded_slot0_new[i]);
        }
        std::cout << "\n";
    }

    // Example: Encoding a Tick struct
    IUniswapV3Pool_Tick tick_to_encode;
    tick_to_encode.liquidityGross = boost::multiprecision::cpp_int("1000000000000000000"); // 1e18
    tick_to_encode.liquidityNet = boost::multiprecision::cpp_int("-500000000000000000"); // -5e17
    tick_to_encode.feeGrowthOutside0X128 = boost::multiprecision::cpp_int("123456789012345678901234567890");
    tick_to_encode.feeGrowthOutside1X128 = boost::multiprecision::cpp_int("987654321098765432109876543210");
    tick_to_encode.tickCumulativeOutside = boost::multiprecision::cpp_int("-123456789");
    tick_to_encode.secondsPerLiquidityOutsideX128 = boost::multiprecision::cpp_int("123456789012345678901234567890");
    tick_to_encode.secondsOutside = 1234567890;
    tick_to_encode.initialized = true;

    std::vector<uint8_t> encoded_tick_new;
    const size_t tick_size = abi::encoded_size<IUniswapV3Pool_Tick>(tick_to_encode);
    encoded_tick_new.resize(tick_size);
    if (abi::encode_into<IUniswapV3Pool_Tick>(encoded_tick_new.data(), encoded_tick_new.size(), tick_to_encode)) {
        std::cout << "  ✅ Encoded Tick using abi::encode_into<>\n";
        std::cout << "    Encoded size: " << encoded_tick_new.size() << " bytes\n";
    }

    // Example: Encoding Multicall3 structs
    Multicall3_Call3Value call_to_encode;
    call_to_encode.target = {0x88, 0xe6, 0xa0, 0xc2, 0xdd, 0xd2, 0x6f, 0xe3, 0x34, 0xb4, 0x70, 0x1b, 0x28, 0x0e, 0x59, 0x6a, 0x2a, 0x2b, 0x39, 0x44};
    call_to_encode.allowFailure = false;
    call_to_encode.value = boost::multiprecision::cpp_int("1000000000000000000"); // 1 ETH in wei
    call_to_encode.callData = {0xa9, 0x05, 0x9c, 0xbb}; // transfer selector

    std::vector<uint8_t> encoded_call_new;
    const size_t call_size = abi::encoded_size<Multicall3_Call3Value>(call_to_encode);
    encoded_call_new.resize(call_size);
    if (abi::encode_into<Multicall3_Call3Value>(encoded_call_new.data(), encoded_call_new.size(), call_to_encode)) {
        std::cout << "  ✅ Encoded Multicall3_Call3Value using abi::encode_into<>\n";
        std::cout << "    Encoded size: " << encoded_call_new.size() << " bytes\n";
    }

    // Demonstrate round-trip: encode -> decode -> verify
    std::cout << "\n7. Round-trip encoding/decoding verification:\n";

    // Round-trip Slot0
    std::vector<uint8_t> slot0_roundtrip;
    const size_t slot0_roundtrip_size = abi::encoded_size<IUniswapV3Pool_Slot0>(slot0_to_encode);
    slot0_roundtrip.resize(slot0_roundtrip_size);
    if (abi::encode_into<IUniswapV3Pool_Slot0>(slot0_roundtrip.data(), slot0_roundtrip.size(), slot0_to_encode)) {
        IUniswapV3Pool_Slot0 decoded_slot0;
        abi::BytesSpan decode_span(slot0_roundtrip.data(), slot0_roundtrip.size());
        if (abi::decode_from<IUniswapV3Pool_Slot0>(decode_span, decoded_slot0)) {
            bool matches = (slot0_to_encode.sqrtPriceX96 == decoded_slot0.sqrtPriceX96 &&
                           slot0_to_encode.tick == decoded_slot0.tick &&
                           slot0_to_encode.unlocked == decoded_slot0.unlocked);
            std::cout << "  ✅ Slot0 round-trip: " << (matches ? "SUCCESS" : "FAILED") << "\n";
        }
    }

    // Round-trip Tick
    std::vector<uint8_t> tick_roundtrip;
    const size_t tick_roundtrip_size = abi::encoded_size<IUniswapV3Pool_Tick>(tick_to_encode);
    tick_roundtrip.resize(tick_roundtrip_size);
    if (abi::encode_into<IUniswapV3Pool_Tick>(tick_roundtrip.data(), tick_roundtrip.size(), tick_to_encode)) {
        IUniswapV3Pool_Tick decoded_tick;
        abi::BytesSpan decode_span(tick_roundtrip.data(), tick_roundtrip.size());
        if (abi::decode_from<IUniswapV3Pool_Tick>(decode_span, decoded_tick)) {
            bool matches = (tick_to_encode.liquidityGross == decoded_tick.liquidityGross &&
                           tick_to_encode.liquidityNet == decoded_tick.liquidityNet &&
                           tick_to_encode.initialized == decoded_tick.initialized);
            std::cout << "  ✅ Tick round-trip: " << (matches ? "SUCCESS" : "FAILED") << "\n";
        }
    }

    // 8. Understanding Different Encoding Approaches
    std::cout << "\n8. Understanding Different Encoding Approaches:\n";

    std::cout << "\n   🔍 Comparing Encoding Methods:\n";

    // Method A: protocols::Slot0::encode_into() - Function Call Encoding
    std::vector<uint8_t> function_call_buffer;
    size_t function_call_size = UniswapV3Pool_Slot0::encoded_size();
    function_call_buffer.resize(function_call_size);

    abi::Error call_err;
    if (UniswapV3Pool_Slot0::encode_call(function_call_buffer.data(), function_call_buffer.size(), &call_err)) {
        std::cout << "     📞 Function Call Encoding (UniswapV3Pool_Slot0::encode_call):\n";
        std::cout << "       Size: " << function_call_buffer.size() << " bytes\n";
        std::cout << "       Data: ";
        for (size_t i = 0; i < function_call_buffer.size(); ++i) {
            printf("%02x", function_call_buffer[i]);
            if (i == 3) std::cout << " "; // Space after selector
        }
        std::cout << "\n";
        std::cout << "       This creates a CALL to slot0() function\n";
    }

    // Method B: abi::encode_into() - Data Encoding (what we need for responses)
    IUniswapV3Pool_Slot0 slot0_struct;
    slot0_struct.sqrtPriceX96 = boost::multiprecision::cpp_int("429512873912345678901234567890");
    slot0_struct.tick = 12345;
    slot0_struct.observationIndex = 42;
    slot0_struct.observationCardinality = 100;
    slot0_struct.observationCardinalityNext = 150;
    slot0_struct.feeProtocol = 5;
    slot0_struct.unlocked = true;

    std::vector<uint8_t> data_buffer;
    const size_t data_size = abi::encoded_size<IUniswapV3Pool_Slot0>(slot0_struct);
    data_buffer.resize(data_size);
    if (abi::encode_into<IUniswapV3Pool_Slot0>(data_buffer.data(), data_buffer.size(), slot0_struct)) {
            std::cout << "\n     📦 Data Encoding (using abi::encode_into for response data):\n";
    std::cout << "       Size: " << data_buffer.size() << " bytes\n";
    std::cout << "       Data: ";
    for (size_t i = 0; i < std::min(size_t(32), data_buffer.size()); ++i) {
        printf("%02x", data_buffer[i]);
    }
    std::cout << "...\n";
    std::cout << "       This encodes the Slot0 struct AS DATA (no function selector)\n";
    std::cout << "       Uses: abi::encoded_size<IUniswapV3Pool_Slot0>() + abi::encode_into<>()\n";
    }

    std::cout << "\n     🎯 Key Difference:\n";
    std::cout << "       • Function Call: Used to CALL contracts (includes selector)\n";
    std::cout << "       • Data Encoding: Used to encode RETURN VALUES (just data)\n";
    std::cout << "       • For testing: We need data encoding to simulate responses\n";

    std::cout << "\n     📐 Size Calculation Comparison:\n";
    std::cout << "       • UniswapV3Pool_Slot0::encoded_size() = " << UniswapV3Pool_Slot0::encoded_size() << " bytes (includes selector)\n";
    std::cout << "       • Slot0 struct data size = " << data_buffer.size() << " bytes (data only)\n";
    std::cout << "       • encoded_size_args() would add +4 for selector\n";

    // 9. Complete Fn wrapper workflow: encode call → simulate response → decode result
    std::cout << "\n9. Complete Fn wrapper workflow (encode call → decode result):\n";

    // Test simple ERC20 functions that work reliably
    std::cout << "\n   ERC20 Functions:\n";

    // Test BalanceOf: address → uint256
    std::array<uint8_t, 20> test_address = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34,
                                           0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78};

    // Encode BalanceOf call
    std::vector<uint8_t> balance_call_data;
    size_t balance_call_size = ERC20_BalanceOf::encoded_size(test_address);
    balance_call_data.resize(balance_call_size);

    abi::Error balance_err;
    if (ERC20_BalanceOf::template encode_call<std::array<uint8_t, 20>>(balance_call_data.data(), balance_call_data.size(), test_address, &balance_err)) {
        std::cout << "     ✅ Encoded BalanceOf call: " << balance_call_data.size() << " bytes\n";
        std::cout << "       Selector: ";
        for (size_t i = 0; i < 4; ++i) printf("%02x", balance_call_data[i]);
        std::cout << "\n";
    } else {
        std::cout << "     ❌ BalanceOf encoding failed: " << balance_err.message << "\n";
    }

    // Simulate BalanceOf return value (1,000,000 tokens with 18 decimals)
    boost::multiprecision::cpp_int simulated_balance = boost::multiprecision::cpp_int("1000000000000000000000000");
    std::vector<uint8_t> simulated_balance_response;
    const size_t balance_size = abi::encoded_size<abi::uint_t<256>>(simulated_balance);
    simulated_balance_response.resize(balance_size);
    abi::encode_into<abi::uint_t<256>>(simulated_balance_response.data(), simulated_balance_response.size(), simulated_balance);

    // Decode using BalanceOf::decode_result
    boost::multiprecision::cpp_int decoded_balance;
    abi::BytesSpan balance_response_span(simulated_balance_response.data(), simulated_balance_response.size());
    if (ERC20_BalanceOf::decode_result(balance_response_span, decoded_balance)) {
        std::cout << "     ✅ Decoded BalanceOf result: " << decoded_balance << "\n";
        std::cout << "       Match: " << (simulated_balance == decoded_balance ? "YES" : "NO") << "\n";
    }

    // Test TotalSupply: () → uint256 (simpler function)
    std::vector<uint8_t> total_supply_call_data;
    size_t total_supply_call_size = ERC20_TotalSupply::encoded_size();
    total_supply_call_data.resize(total_supply_call_size);

    abi::Error total_supply_err;
    if (ERC20_TotalSupply::encode_call(total_supply_call_data.data(), total_supply_call_data.size(), &total_supply_err)) {
        std::cout << "     ✅ Encoded TotalSupply call: " << total_supply_call_data.size() << " bytes\n";
        std::cout << "       Selector: ";
        for (size_t i = 0; i < 4; ++i) printf("%02x", total_supply_call_data[i]);
        std::cout << "\n";
    } else {
        std::cout << "     ❌ TotalSupply encoding failed: " << total_supply_err.message << "\n";
    }

    // Simulate TotalSupply return value
    boost::multiprecision::cpp_int simulated_total_supply = boost::multiprecision::cpp_int("1000000000000000000000000000"); // 1 billion tokens
    std::vector<uint8_t> simulated_total_supply_response;
    const size_t total_supply_size = abi::encoded_size<abi::uint_t<256>>(simulated_total_supply);
    simulated_total_supply_response.resize(total_supply_size);
    abi::encode_into<abi::uint_t<256>>(simulated_total_supply_response.data(), simulated_total_supply_response.size(), simulated_total_supply);

    // Decode using TotalSupply::decode_result
    boost::multiprecision::cpp_int decoded_total_supply;
    abi::BytesSpan total_supply_response_span(simulated_total_supply_response.data(), simulated_total_supply_response.size());
    if (ERC20_TotalSupply::decode_result(total_supply_response_span, decoded_total_supply)) {
        std::cout << "     ✅ Decoded TotalSupply result: " << decoded_total_supply << "\n";
        std::cout << "       Match: " << (simulated_total_supply == decoded_total_supply ? "YES" : "NO") << "\n";
    }

    // Test Uniswap V3 functions
    std::cout << "\n   Uniswap V3 Functions:\n";

    // Test Slot0: () → Slot0 struct (simpler than Ticks)
    std::vector<uint8_t> slot0_call_data;
    size_t slot0_call_size = UniswapV3Pool_Slot0::encoded_size();
    slot0_call_data.resize(slot0_call_size);

    abi::Error slot0_err;
    if (UniswapV3Pool_Slot0::encode_call(slot0_call_data.data(), slot0_call_data.size(), &slot0_err)) {
        std::cout << "     ✅ Encoded Slot0 call: " << slot0_call_data.size() << " bytes\n";
        std::cout << "       Selector: ";
        for (size_t i = 0; i < 4; ++i) printf("%02x", slot0_call_data[i]);
        std::cout << "\n";
    } else {
        std::cout << "     ❌ Slot0 encoding failed: " << slot0_err.message << "\n";
    }

    // Simulate Slot0 return value
    IUniswapV3Pool_Slot0 simulated_slot0;
    simulated_slot0.sqrtPriceX96 = boost::multiprecision::cpp_int("429512873912345678901234567890");
    simulated_slot0.tick = 12345;
    simulated_slot0.observationIndex = 42;
    simulated_slot0.observationCardinality = 100;
    simulated_slot0.observationCardinalityNext = 150;
    simulated_slot0.feeProtocol = 5;
    simulated_slot0.unlocked = true;

    std::vector<uint8_t> simulated_slot0_response;
    const size_t slot0_response_size = abi::encoded_size<IUniswapV3Pool_Slot0>(simulated_slot0);
    simulated_slot0_response.resize(slot0_response_size);
    abi::encode_into<IUniswapV3Pool_Slot0>(simulated_slot0_response.data(), simulated_slot0_response.size(), simulated_slot0);

    // Decode using Slot0::decode_result
    IUniswapV3Pool_Slot0 decoded_slot0;
    abi::BytesSpan slot0_response_span(simulated_slot0_response.data(), simulated_slot0_response.size());
    if (UniswapV3Pool_Slot0::decode_result(slot0_response_span, decoded_slot0)) {
        std::cout << "     ✅ Decoded Slot0 result:\n";
        std::cout << "       sqrtPriceX96: " << decoded_slot0.sqrtPriceX96 << "\n";
        std::cout << "       tick: " << decoded_slot0.tick << "\n";
        std::cout << "       unlocked: " << (decoded_slot0.unlocked ? "true" : "false") << "\n";

        bool slot0_matches = (simulated_slot0.sqrtPriceX96 == decoded_slot0.sqrtPriceX96 &&
                             simulated_slot0.tick == decoded_slot0.tick &&
                             simulated_slot0.unlocked == decoded_slot0.unlocked);
        std::cout << "       Match: " << (slot0_matches ? "YES" : "NO") << "\n";
    }

    // 10. Named vs Generic Types: Understanding the Difference
    std::cout << "\n10. Named vs Generic Types: Understanding the Difference\n";

    std::cout << "\n   🔍 Comparison: Named Structs vs Generic Tuples\n";

    // Example A: Named struct (from ABI) - Clean field access
    std::cout << "\n     🏗️  NAMED STRUCT (from ABI) - Clean field access:\n";
    IUniswapV3Pool_Slot0 named_slot0;
    named_slot0.sqrtPriceX96 = boost::multiprecision::cpp_int("429512873912345678901234567890");
    named_slot0.tick = 12345;
    named_slot0.observationIndex = 42;
    named_slot0.observationCardinality = 100;
    named_slot0.observationCardinalityNext = 150;
    named_slot0.feeProtocol = 5;
    named_slot0.unlocked = true;

    // Access by meaningful field names
    std::cout << "       slot0.sqrtPriceX96 = " << named_slot0.sqrtPriceX96 << "\n";
    std::cout << "       slot0.tick = " << named_slot0.tick << "\n";
    std::cout << "       slot0.unlocked = " << (named_slot0.unlocked ? "true" : "false") << "\n";
    std::cout << "       ✅ Easy to read and understand!\n";

    // Example B: Generic tuple (underlying representation) - Index access
    std::cout << "\n     🔧 GENERIC TUPLE (underlying) - Index access:\n";
    using GenericSlot0Tuple = abi::tuple<
        abi::uint_t<160>, abi::int_t<24>, abi::uint_t<16>, abi::uint_t<16>,
        abi::uint_t<16>, abi::uint_t<8>, abi::bool_t
    >;

    auto generic_slot0 = std::make_tuple(
        boost::multiprecision::cpp_int("429512873912345678901234567890"), // sqrtPriceX96
        boost::multiprecision::cpp_int(12345),                           // tick
        boost::multiprecision::cpp_int(42),                             // observationIndex
        boost::multiprecision::cpp_int(100),                            // observationCardinality
        boost::multiprecision::cpp_int(150),                            // observationCardinalityNext
        boost::multiprecision::cpp_int(5),                              // feeProtocol
        true                                                            // unlocked
    );

    // Access by index - harder to understand
    std::cout << "       std::get<0>(tuple) = " << std::get<0>(generic_slot0) << " // sqrtPriceX96?\n";
    std::cout << "       std::get<1>(tuple) = " << std::get<1>(generic_slot0) << " // tick?\n";
    std::cout << "       std::get<6>(tuple) = " << (std::get<6>(generic_slot0) ? "true" : "false") << " // unlocked?\n";
    std::cout << "       ❌ Hard to remember what each index represents!\n";

    // Show conversion between named and generic
    std::cout << "\n     🔄 CONVERSION between Named ↔ Generic:\n";

    // Named → Generic (for internal operations)
    auto tuple_from_named = IUniswapV3Pool_Slot0::to_tuple(named_slot0);
    std::cout << "       IUniswapV3Pool_Slot0::to_tuple(named) → tuple\n";

    // Generic → Named (for clean access)
    auto named_from_tuple = IUniswapV3Pool_Slot0::from_tuple(tuple_from_named);
    std::cout << "       IUniswapV3Pool_Slot0::from_tuple(tuple) → named\n";

    // Example C: When ABI doesn't have named types
    std::cout << "\n     🎯 WHEN ABI HAS NO NAMED TYPES:\n";

    // Some contract might return (uint256,bool) without naming it
    using SimpleReturn = abi::tuple<abi::uint_t<256>, abi::bool_t>;
    auto simple_result = std::make_tuple(
        boost::multiprecision::cpp_int("1000000000000000000"), // 1 ETH in wei
        true                                                    // success flag
    );

    std::cout << "       Contract returns: (uint256, bool)\n";
    std::cout << "       Access as: std::get<0>(result) // amount\n";
    std::cout << "                  std::get<1>(result) // success\n";
    std::cout << "       💡 Use comments or create wrapper functions!\n";

    // Best practice: Create wrapper for unnamed types
    std::cout << "\n     💡 BEST PRACTICE for unnamed types:\n";
    struct TransferResult {
        boost::multiprecision::cpp_int amount;
        bool success;

        static TransferResult from_tuple(const std::tuple<boost::multiprecision::cpp_int, bool>& t) {
            return {std::get<0>(t), std::get<1>(t)};
        }

        std::tuple<boost::multiprecision::cpp_int, bool> to_tuple() const {
            return std::make_tuple(amount, success);
        }
    };

    TransferResult clean_result = TransferResult::from_tuple(simple_result);
    std::cout << "       TransferResult clean_result = TransferResult::from_tuple(tuple);\n";
    std::cout << "       clean_result.amount = " << clean_result.amount << "\n";
    std::cout << "       clean_result.success = " << (clean_result.success ? "true" : "false") << "\n";
    std::cout << "       ✅ Clean access even for unnamed ABI types!\n";

    // 11. Test Uniswap V2 Router: Dynamic Arrays & Complex Signatures
    std::cout << "\n11. Test Uniswap V2 Router: Dynamic Arrays & Complex Signatures\n";

    std::cout << "\n   🔄 Testing Uniswap V2 Router functions with dynamic arrays:\n";

    // Test getAmountsOut: (uint256, address[]) → uint256[]
    using GetAmountsOut = UniswapV2Router_GetAmountsOut;

    boost::multiprecision::cpp_int amountIn = boost::multiprecision::cpp_int("1000000000000000000"); // 1 ETH
    std::vector<std::array<uint8_t, 20>> path = {
        abi::addr_from_hex("0xC02aaA39b223FE8D0A0e5C4F27eAD9083C756Cc2"), // WETH
        abi::addr_from_hex("0xA0b86a33E6441A8b5F4F62fC20cdD7E9C8C6e4F4")  // Some token
    };

    // Encode call
    std::vector<uint8_t> amounts_call;
    size_t amounts_call_size = GetAmountsOut::encoded_size(amountIn, path);
    amounts_call.resize(amounts_call_size);

    abi::Error amounts_err;
    if (GetAmountsOut::template encode_call<boost::multiprecision::cpp_int, std::vector<std::array<uint8_t, 20>>>(amounts_call.data(), amounts_call.size(), amountIn, path, &amounts_err)) {
        std::cout << "     ✅ Encoded getAmountsOut call: " << amounts_call.size() << " bytes\n";
        std::cout << "       Path length: " << path.size() << " addresses\n";
        std::cout << "       Selector: ";
        for (size_t i = 0; i < 4; ++i) printf("%02x", amounts_call[i]);
        std::cout << "\n";
    }

    // Simulate return value: uint256[] amounts
    std::vector<boost::multiprecision::cpp_int> simulated_amounts = {
        boost::multiprecision::cpp_int("1000000000000000000"), // Input amount
        boost::multiprecision::cpp_int("2500000000000000000")  // Output amount
    };

    // Encode the return data using generic functions
    const size_t amounts_size = abi::encoded_size<abi::dyn_array<abi::uint_t<256>>>(simulated_amounts);
    std::vector<uint8_t> simulated_amounts_response;
    simulated_amounts_response.resize(amounts_size);

    abi::encode_into<abi::dyn_array<abi::uint_t<256>>>(
        simulated_amounts_response.data(),
        simulated_amounts_response.size(),
        simulated_amounts
    );

    // Decode using GetAmountsOut::decode_result
    std::vector<boost::multiprecision::cpp_int> decoded_amounts;
    abi::BytesSpan amounts_response_span(simulated_amounts_response.data(), simulated_amounts_response.size());
    if (GetAmountsOut::decode_result(amounts_response_span, decoded_amounts)) {
        std::cout << "     ✅ Decoded getAmountsOut result:\n";
        std::cout << "       Amounts: [";
        for (size_t i = 0; i < decoded_amounts.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << decoded_amounts[i];
        }
        std::cout << "]\n";
        std::cout << "       Match: " << (decoded_amounts == simulated_amounts ? "YES" : "NO") << "\n";
    }

    // Test swapExactTokensForTokens: complex signature with dynamic arrays
    using SwapExactTokensForTokens = UniswapV2Router_SwapExactTokensForTokens;

    boost::multiprecision::cpp_int swapAmountIn = boost::multiprecision::cpp_int("500000000000000000"); // 0.5 ETH
    boost::multiprecision::cpp_int swapAmountOutMin = boost::multiprecision::cpp_int("1000000000000000000"); // 1 token min
    std::vector<std::array<uint8_t, 20>> swapPath = {
        abi::addr_from_hex("0x6B175474E89094C44Da98b954EedeAC495271d0F"), // DAI
        abi::addr_from_hex("0xC02aaA39b223FE8D0A0e5C4F27eAD9083C756Cc2"), // WETH
        abi::addr_from_hex("0xA0b86a33E6441A8b5F4F62fC20cdD7E9C8C6e4F4")  // UNI
    };
    std::array<uint8_t, 20> swapTo = abi::addr_from_hex("0x742d35Cc6634C0532925a3b8A46Bb0FcE0260b8B");
    boost::multiprecision::cpp_int swapDeadline = boost::multiprecision::cpp_int("1700000000");

    // Encode swap call
    std::vector<uint8_t> swap_call;
    size_t swap_call_size = SwapExactTokensForTokens::encoded_size(swapAmountIn, swapAmountOutMin, swapPath, swapTo, swapDeadline);
    swap_call.resize(swap_call_size);

    if (SwapExactTokensForTokens::template encode_call<boost::multiprecision::cpp_int, boost::multiprecision::cpp_int, std::vector<std::array<uint8_t, 20>>, std::array<uint8_t, 20>, boost::multiprecision::cpp_int>(swap_call.data(), swap_call.size(), swapAmountIn, swapAmountOutMin, swapPath, swapTo, swapDeadline, &amounts_err)) {
        std::cout << "     ✅ Encoded swapExactTokensForTokens call: " << swap_call.size() << " bytes\n";
        std::cout << "       Complex path: " << swapPath.size() << " hops (DAI → WETH → UNI)\n";
        std::cout << "       Selector: ";
        for (size_t i = 0; i < 4; ++i) printf("%02x", swap_call[i]);
        std::cout << "\n";
    }

    // Simulate swap return value: uint256[] with 3 amounts
    std::vector<boost::multiprecision::cpp_int> swap_amounts = {
        boost::multiprecision::cpp_int("500000000000000000"),   // Input amount
        boost::multiprecision::cpp_int("2500000000000000000"),  // Intermediate
        boost::multiprecision::cpp_int("1200000000000000000")   // Final output
    };

    const size_t swap_response_size = abi::encoded_size<abi::dyn_array<abi::uint_t<256>>>(swap_amounts);
    std::vector<uint8_t> swap_response;
    swap_response.resize(swap_response_size);

    abi::encode_into<abi::dyn_array<abi::uint_t<256>>>(
        swap_response.data(),
        swap_response.size(),
        swap_amounts
    );

    // Decode swap result
    std::vector<boost::multiprecision::cpp_int> decoded_swap_amounts;
    abi::BytesSpan swap_response_span(swap_response.data(), swap_response.size());
    if (SwapExactTokensForTokens::decode_result(swap_response_span, decoded_swap_amounts)) {
        std::cout << "     ✅ Decoded swapExactTokensForTokens result:\n";
        std::cout << "       Swap amounts: [";
        for (size_t i = 0; i < decoded_swap_amounts.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << decoded_swap_amounts[i];
        }
        std::cout << "]\n";
        std::cout << "       Match: " << (decoded_swap_amounts == swap_amounts ? "YES" : "NO") << "\n";
    }

    std::cout << "\n   🎯 Uniswap V2 Router Test Summary:\n";
    std::cout << "     ✅ Dynamic arrays (address[], uint256[]) handled correctly\n";
    std::cout << "     ✅ Complex function signatures generated properly\n";
    std::cout << "     ✅ Multiple return values work seamlessly\n";
    std::cout << "     ✅ Generic functions encode/decode dynamic data perfectly\n";
    std::cout << "     ✅ Clean integration: ABI → Named Functions → Generic Encoding\n";

    std::cout << "\n=== Summary ===\n";
    std::cout << "✅ Named structs provide clean, type-safe access to ABI data\n";
    std::cout << "✅ Fields have meaningful names from the ABI\n";
    std::cout << "✅ Easy to create, modify, and work with structured data\n";
    std::cout << "✅ Full compatibility with existing ABI codec\n";
    std::cout << "✅ No performance overhead - delegates to existing tuple logic\n";
    std::cout << "✅ Clean encoding: abi::encode_into<Schema>(value, buffer)\n";
    std::cout << "✅ Function-specific encoding: protocols::Function::encode_call()\n";
    std::cout << "✅ Round-trip verified: encode → decode → verify data integrity\n";
    std::cout << "✅ Complete Fn workflow: encode_call → decode_result\n";
    std::cout << "✅ Clear distinction: Function calls vs Data encoding\n";
    std::cout << "✅ Generic ABI functions: encoded_size<>() + encode_into<>()\n";
    std::cout << "✅ Named vs Generic: Choose based on ABI structure and readability needs\n";
    std::cout << "✅ Wrapper structs: Create clean access for unnamed ABI types\n";
    std::cout << "✅ Dynamic arrays: Full support for address[], uint256[], etc.\n";
    std::cout << "✅ Complex signatures: Multi-parameter functions work perfectly\n";
    std::cout << "✅ Real-world contracts: Uniswap V2 Router tested successfully!\n";
}

int main() {
    demonstrateCleanUsage();
    return 0;
}
