#!/usr/bin/env node

import { Interface } from 'ethers';

// Usage: node scripts/encode.mjs "functionName(type1,type2,...)" arg1 arg2 ...
const [sig, ...args] = process.argv.slice(2);

if (!sig || args.length === 0) {
  console.error('Usage: node scripts/encode.mjs "functionName(type1,type2,...)" arg1 arg2 ...');
  console.error('Example: node scripts/encode.mjs "balanceOf(address)" 0x000000000000000000000000000000000000dEaD');
  process.exit(1);
}

try {
  const iface = new Interface([`function ${sig}`]);
  const functionName = sig.split('(')[0];
  const encoded = iface.encodeFunctionData(functionName, args);
  process.stdout.write(encoded);
} catch (error) {
  console.error('Encoding error:', error.message);
  process.exit(1);
}
