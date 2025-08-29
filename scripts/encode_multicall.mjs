#!/usr/bin/env node

import { Interface } from 'ethers';

// Usage: node scripts/encode_multicall.mjs "functionName" arg1 arg2 ...
const [sig, ...args] = process.argv.slice(2);

if (!sig || args.length === 0) {
  console.error('Usage: node scripts/encode_multicall.mjs "functionName" arg1 arg2 ...');
  console.error('Example: node scripts/encode_multicall.mjs "tryAggregate" false "[address,bytes][]"');
  process.exit(1);
}

try {
  // Handle multicall functions specifically
  let functionSignature;
  let functionArgs;
  
  if (sig === 'tryAggregate') {
    functionSignature = 'tryAggregate(bool,(address,bytes)[])';
    // args[0] should be the boolean, args[1] should be the JSON array string
    const requireSuccess = args[0] === 'true';
    const calls = JSON.parse(args[1]);
    functionArgs = [requireSuccess, calls];
  } else if (sig === 'aggregate3') {
    functionSignature = 'aggregate3((address,bool,bytes)[])';
    // args[0] should be the JSON array string
    const calls = JSON.parse(args[0]);
    functionArgs = [calls];
  } else {
    console.error('Unsupported function:', sig);
    process.exit(1);
  }
  
  const iface = new Interface([`function ${functionSignature}`]);
  const functionName = functionSignature.split('(')[0];
  const encoded = iface.encodeFunctionData(functionName, functionArgs);
  process.stdout.write(encoded);
} catch (error) {
  console.error('Encoding error:', error.message);
  process.exit(1);
}
