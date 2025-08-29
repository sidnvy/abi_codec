#!/usr/bin/env node

import { ethers } from 'ethers';

// Multicall3 contract address on Ethereum mainnet
const MULTICALL3_ADDRESS = '0xcA11bde05977b3631167028862bE2a173976CA11';

// Uniswap V3 ETH-USDC pool address
const UNISWAP_V3_POOL_ADDRESS = '0x8ad599c3A0ff1De082011EFDDc58f1908eb6e6D8';

// Function selectors for token0() and token1()
const TOKEN0_SELECTOR = '0x0dfe1681'; // token0()
const TOKEN1_SELECTOR = '0xd21220a7'; // token1()

// Multicall3 tryAggregate function signature
const TRY_AGGREGATE_SIGNATURE = 'tryAggregate(bool,(address,bytes)[])';

async function main() {
    console.log('🚀 Multicall3 RPC Test - ETH-USDC Pool\n');
    
    // Connect to Ethereum mainnet (you can change this to your preferred RPC)
    const provider = new ethers.JsonRpcProvider('https://eth.llamarpc.com');
    
    // Multicall3 contract interface
    const multicall3Interface = new ethers.Interface([
        `function tryAggregate(bool requireSuccess, tuple(address target, bytes callData)[] calls) returns (tuple(bool success, bytes returnData)[])`
    ]);
    
    // Prepare the calls
    const calls = [
        {
            target: UNISWAP_V3_POOL_ADDRESS,
            callData: TOKEN0_SELECTOR
        },
        {
            target: UNISWAP_V3_POOL_ADDRESS,
            callData: TOKEN1_SELECTOR
        }
    ];
    
    console.log('📋 Call Details:');
    console.log(`  Multicall3 Contract: ${MULTICALL3_ADDRESS}`);
    console.log(`  Uniswap V3 Pool: ${UNISWAP_V3_POOL_ADDRESS}`);
    console.log(`  Function 1: token0() (selector: ${TOKEN0_SELECTOR})`);
    console.log(`  Function 2: token1() (selector: ${TOKEN1_SELECTOR})`);
    console.log(`  requireSuccess: false (continue even if calls fail)\n`);
    
    try {
        // Encode the multicall
        const encodedData = multicall3Interface.encodeFunctionData('tryAggregate', [false, calls]);
        console.log('🔧 Encoded Multicall Data:');
        console.log(`  ${encodedData}\n`);
        
        // Send the RPC call
        console.log('📡 Sending RPC call...');
        const result = await provider.call({
            to: MULTICALL3_ADDRESS,
            data: encodedData
        });
        
        console.log('✅ RPC call successful!');
        console.log(`  Raw result: ${result}\n`);
        
        // Decode the result
        const decodedResult = multicall3Interface.decodeFunctionResult('tryAggregate', result);
        const [successResults] = decodedResult;
        
        console.log('🔍 Decoded Results:');
        
        // Decode each result
        for (let i = 0; i < successResults.length; i++) {
            const { success, returnData } = successResults[i];
            const functionName = i === 0 ? 'token0()' : 'token1()';
            
            console.log(`\n  ${functionName}:`);
            console.log(`    Success: ${success}`);
            
            if (success && returnData !== '0x') {
                // Decode the address result - addresses are padded to 32 bytes
                const paddedAddress = returnData.slice(0, 66); // Remove 0x and take 32 bytes
                const address = '0x' + paddedAddress.slice(26); // Take last 20 bytes (40 chars)
                const normalizedAddress = ethers.getAddress(address);
                console.log(`    Token Address: ${normalizedAddress}`);
                
                // Try to get token info if it's a known token
                if (normalizedAddress === '0xC02aaA39b223FE8D0A0e5C4F27eAD9083C756Cc2') {
                    console.log(`    Token: WETH (Wrapped Ether)`);
                } else if (normalizedAddress === '0xA0b86991c6218b36c1d19d4a2e9eb0ce3606eb48') {
                    console.log(`    Token: USDC (USD Coin)`);
                } else {
                    console.log(`    Token: Unknown (${normalizedAddress})`);
                }
            } else {
                console.log(`    Error: ${returnData === '0x' ? 'No data returned' : returnData}`);
            }
        }
        
        // Also show the C++ codec would decode this
        console.log('\n🔧 For C++ Codec Testing:');
        console.log(`  Raw result bytes: ${result.slice(2)}`); // Remove 0x prefix
        console.log(`  Expected decoded addresses: [${successResults.map(r => {
            if (!r.success) return 'ERROR';
            const paddedAddress = r.returnData.slice(0, 66);
            const address = '0x' + paddedAddress.slice(26);
            return ethers.getAddress(address);
        }).join(', ')}]`);
        
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
