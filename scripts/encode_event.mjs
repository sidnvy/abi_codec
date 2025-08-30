#!/usr/bin/env node

import { Interface, keccak256, toUtf8Bytes } from 'ethers';

// Usage: node scripts/encode_event.mjs "EventName(type1,type2,...)" [arg1 arg2 ...]
const [sig, ...args] = process.argv.slice(2);

if (!sig) {
  console.error('Usage: node scripts/encode_event.mjs "EventName(type1,type2,...)" [arg1 arg2 ...]');
  console.error('Examples:');
  console.error('  node scripts/encode_event.mjs "Transfer(address,address,uint256)"');
  console.error('  node scripts/encode_event.mjs "Swap(address,address,int256,int256,uint160,uint128,int24)" "-1000000000000000000" "2000000000000000000" "429512873912345678901234567890" "1000000000000000000" "12345"');
  process.exit(1);
}

try {
  const iface = new Interface([`event ${sig}`]);
  const eventName = sig.split('(')[0];

  // Get the event topic hash
  const eventFragment = iface.getEvent(eventName);
  const topicHash = keccak256(toUtf8Bytes(eventFragment.format()));

  console.log(`Topic Hash: ${topicHash}`);

  if (args.length > 0) {
    // For ethers.js, we need to provide ALL parameters in order
    let argIndex = 0;
    const allArgs = eventFragment.inputs.map((input, paramIndex) => {
      const isIndexed = input.indexed === true;

      if (isIndexed) {
        // For indexed parameters, provide dummy values since they're not in the data
        if (input.type === 'address') {
          return '0x0000000000000000000000000000000000000000';
        } else if (input.type.startsWith('uint') || input.type.startsWith('int')) {
          return 0;
        } else {
          return null;
        }
      } else {
        // Non-indexed parameter - use provided arguments
        if (argIndex < args.length) {
          const arg = args[argIndex++];
          // Use string representation for large integers, ethers.js can handle it
          return arg;
        }
        return 0; // Default value
      }
    });

    try {
      // Encode the event log with dummy indexed values
      const encodedLog = iface.encodeEventLog(eventName, allArgs);

      // Extract only the data portion (non-indexed parameters)
      if (encodedLog.data && encodedLog.data !== '0x') {
        const dataOnly = encodedLog.data.slice(2); // Remove 0x prefix
        console.log(`Encoded Data: ${dataOnly}`);
        process.stdout.write(dataOnly);
      } else {
        console.log(`Encoded Data: (empty)`);
      }
    } catch (error) {
      console.error('Failed to encode event log:', error.message);

      // Fallback: manually encode just the non-indexed parameters
      const nonIndexedParams = eventFragment.inputs.filter(input => !input.indexed);
      const nonIndexedArgs = args.map(arg => arg); // Keep as strings for large numbers

      if (nonIndexedArgs.length > 0 && nonIndexedArgs.length === nonIndexedParams.length) {
        try {
          const encoded = iface._encodeParams(nonIndexedParams, nonIndexedArgs);
          const dataOnly = encoded.slice(2); // Remove 0x prefix
          console.log(`Encoded Data (fallback): ${dataOnly}`);
          process.stdout.write(dataOnly);
        } catch (fallbackError) {
          console.error('Fallback encoding also failed:', fallbackError.message);
        }
      }
    }
  }

} catch (error) {
  console.error('Event encoding error:', error.message);
  process.exit(1);
}
