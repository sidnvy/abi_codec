#!/usr/bin/env node

import { Interface } from 'ethers';

const iface = new Interface(['event Swap(address indexed sender, address indexed recipient, int256 amount0, int256 amount1, uint160 sqrtPriceX96, uint128 liquidity, int24 tick)']);
const event = iface.getEvent('Swap');

console.log('Swap Event Parameter Structure:');
event.inputs.forEach((input, i) => {
  console.log(`${i}: ${input.name} (${input.type}) indexed=${input.indexed}`);
});

console.log('\nNon-indexed parameters only:');
event.inputs.filter(input => !input.indexed).forEach((input, i) => {
  console.log(`${i}: ${input.name} (${input.type})`);
});
