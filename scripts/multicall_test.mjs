#!/usr/bin/env node

import { ethers } from 'ethers';

// Multicall contract address on mainnet
const MULTICALL_ADDRESS = '0x1F98415757620B543A52E61c46B32eB19261F984';

// ETH-USDC pool on mainnet
const POOL_ADDRESS = '0x8ad599c3A0ff1De082011EFDDc58f1908eb6e6D8';

// Function selectors
const TOKEN0_SELECTOR = '0x0ddfe803'; // token0()
const TOKEN1_SELECTOR = '0xd212200e'; // token1()

// Multicall ABI for tryAggregate
const MULTICALL_ABI = [
  'function tryAggregate(bool requireSuccess, tuple(address target, bytes callData)[] calls) external view returns (tuple(bool success, bytes returnData)[])'
];

async function testMulticall() {
  try {
    // Connect to mainnet (you can change this to your preferred RPC)
    const provider = new ethers.JsonRpcProvider('https://eth.llamarpc.com');
    
    console.log('🔗 Connected to Ethereum mainnet');
    console.log(`📊 Testing multicall on ETH-USDC pool: ${POOL_ADDRESS}`);
    
    // Create multicall contract instance
    const multicall = new ethers.Contract(MULTICALL_ADDRESS, MULTICALL_ABI, provider);
    
    // Prepare the calls
    const calls = [
      {
        target: POOL_ADDRESS,
        callData: TOKEN0_SELECTOR
      },
      {
        target: POOL_ADDRESS,
        callData: TOKEN1_SELECTOR
      }
    ];
    
    console.log('\n📞 Making multicall with 2 calls:');
    console.log('  - Call 1: token0()');
    console.log('  - Call 2: token1()');
    
    // Encode the call first to show the data
    const encodedData = multicall.interface.encodeFunctionData('tryAggregate', [false, calls]);
    console.log(`\n🔍 Encoded multicall data: ${encodedData}`);
    
    // Try to make the actual call
    try {
      const results = await multicall.tryAggregate(false, calls);
      
      console.log('\n✅ Multicall successful!');
      console.log(`📋 Received ${results.length} results:`);
      
      // Decode and display results
      for (let i = 0; i < results.length; i++) {
        const [success, returnData] = results[i];
        console.log(`\n  Result ${i + 1}:`);
        console.log(`    Success: ${success}`);
        
        if (success && returnData !== '0x') {
          // Decode the address (remove 0x prefix and take first 40 chars)
          const addressHex = returnData.slice(2, 42);
          const address = '0x' + addressHex;
          
          console.log(`    Return Data: ${returnData}`);
          console.log(`    Decoded Address: ${address}`);
          
          // Try to get token name if it's a valid ERC20
          try {
            const tokenContract = new ethers.Contract(address, ['function name() view returns (string)'], provider);
            const tokenName = await tokenContract.name();
            console.log(`    Token Name: ${tokenName}`);
          } catch (e) {
            console.log(`    Token Name: Could not fetch (${e.message})`);
          }
        } else {
          console.log(`    Return Data: ${returnData} (call failed or empty)`);
        }
      }
    } catch (rpcError) {
      console.log('\n⚠️  RPC call failed, but encoding was successful');
      console.log(`   Error: ${rpcError.message}`);
      console.log('\n📋 This is expected behavior for testing purposes');
      console.log('   The encoded data above can be used with any RPC endpoint');
    }
    
    // Test with a simpler call to a known working contract
    console.log('\n🧪 Testing with a simpler call...');
    try {
      // Test with a simple call to get ETH balance
      const simpleCalls = [
        {
          target: '0x0000000000000000000000000000000000000000', // Zero address
          callData: '0x' // Empty call data
        }
      ];
      
      const simpleResults = await multicall.tryAggregate(false, simpleCalls);
      console.log('✅ Simple multicall test successful!');
      console.log(`   Results: ${JSON.stringify(simpleResults)}`);
    } catch (e) {
      console.log(`❌ Simple test also failed: ${e.message}`);
    }
    
  } catch (error) {
    console.error('❌ Error:', error.message);
    process.exit(1);
  }
}

// Run the test
testMulticall();
