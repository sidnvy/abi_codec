#!/usr/bin/env node

import { ethers } from 'ethers';

// Uniswap V3 Tick Lens contract address on Ethereum mainnet
const TICKLENS_ADDRESS = '0xbfd8137f7d1516D3ea5cA83523914859ec47F573';

// Uniswap V3 ETH-USDC pool address
const UNISWAP_V3_POOL_ADDRESS = '0x8ad599c3A0ff1De082011EFDDc58f1908eb6e6D8';

// Function selector for getPopulatedTicksInWord(address,int16)
const GET_POPULATED_TICKS_SELECTOR = '0x351fb478';

async function main() {
    console.log('🚀 Uniswap V3 TickLens RPC Test - getPopulatedTicksInWord\n');

    // Connect to Ethereum mainnet (you can change this to your preferred RPC)
    const provider = new ethers.JsonRpcProvider('https://eth.llamarpc.com');

    // TickLens contract interface
    const tickLensInterface = new ethers.Interface([
        `function getPopulatedTicksInWord(address pool, int16 tickBitmapIndex) returns (tuple(int24 tick, int128 liquidityNet, uint128 liquidityGross)[] populatedTicks)`
    ]);

    // Test word position (this should be a valid tick bitmap index)
    const wordPosition = 12345; // Same as C++ test

    console.log('📋 Call Details:');
    console.log(`  TickLens Contract: ${TICKLENS_ADDRESS}`);
    console.log(`  Uniswap V3 Pool: ${UNISWAP_V3_POOL_ADDRESS}`);
    console.log(`  Function: getPopulatedTicksInWord(${UNISWAP_V3_POOL_ADDRESS}, ${wordPosition})`);
    console.log(`  Selector: ${GET_POPULATED_TICKS_SELECTOR}\n`);

    try {
        // Encode the function call
        const encodedData = tickLensInterface.encodeFunctionData('getPopulatedTicksInWord', [
            UNISWAP_V3_POOL_ADDRESS,
            wordPosition
        ]);
        console.log('🔧 Encoded TickLens Call Data:');
        console.log(`  ${encodedData}\n`);

        // Send the RPC call to get the raw response
        console.log('📡 Sending RPC call...');
        const result = await provider.call({
            to: TICKLENS_ADDRESS,
            data: encodedData
        });

        console.log('✅ RPC call successful!');
        console.log(`  Raw result: ${result}\n`);

        // Decode the result
        const decodedResult = tickLensInterface.decodeFunctionResult('getPopulatedTicksInWord', result);
        const [populatedTicks] = decodedResult;

        console.log('🔍 Decoded Results:');
        console.log(`  Number of ticks: ${populatedTicks.length}`);

        if (populatedTicks.length > 0) {
            populatedTicks.forEach((tick, index) => {
                console.log(`\n  Tick ${index}:`);
                console.log(`    tick: ${tick.tick}`);
                console.log(`    liquidityNet: ${tick.liquidityNet}`);
                console.log(`    liquidityGross: ${tick.liquidityGross}`);
            });
        } else {
            console.log('  No populated ticks in this word');
        }

        // Show the raw bytes for C++ codec testing
        console.log('\n🔧 For C++ Codec Testing:');
        console.log(`  Encoded call bytes: ${encodedData.slice(2)}`); // Remove 0x prefix
        console.log(`  Expected response bytes: ${result.slice(2)}`); // Remove 0x prefix

        // Also show formatted versions for readability
        console.log('\n📝 Formatted for C++:');
        console.log('  Encoded call (32-byte chunks):');
        for (let i = 0; i < encodedData.length - 2; i += 64) {
            const chunk = encodedData.slice(2 + i, 2 + i + 64);
            const chunkNum = Math.floor(i / 64) + 1;
            console.log(`    Chunk ${chunkNum}: ${chunk}`);
        }

        console.log('\n  Response (32-byte chunks):');
        for (let i = 0; i < result.length - 2; i += 64) {
            const chunk = result.slice(2 + i, 2 + i + 64);
            const chunkNum = Math.floor(i / 64) + 1;
            console.log(`    Chunk ${chunkNum}: ${chunk}`);
        }

    } catch (error) {
        console.error('❌ RPC call failed:', error.message);

        if (error.message.includes('network')) {
            console.log('\n💡 Try using a different RPC endpoint:');
            console.log('   - https://eth.llamarpc.com');
            console.log('   - https://rpc.ankr.com/eth');
            console.log('   - https://cloudflare-eth.com');
        }
    }
}

main().catch(console.error);
