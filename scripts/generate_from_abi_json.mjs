#!/usr/bin/env node

import { keccak256, toUtf8Bytes } from 'ethers';
import fs from 'fs';
import path from 'path';

// ABI JSON file paths
const abiFiles = {
  "ERC20": "abis/erc20.json",
  "UniswapV3Pool": "abis/uniswap_v3_pool.json", 
  "UniswapV3TickLens": "abis/uniswap_v3_tick_lens.json",
  "Multicall": "abis/multicall.json"
};

// ---------- Canonical type & signature helpers ----------

// canonical type for one ABI param (handles tuples, arrays, nested)
function canonicalType(param) {
  // Extract array suffix like "[][2][]" from the end
  const arraySuffix = (param.type.match(/(\[[0-9]*\])*$/) || [''])[0];
  const base = param.type.slice(0, param.type.length - arraySuffix.length);

  let head;
  if (base === 'tuple') {
    const inner = (param.components || []).map(canonicalType).join(',');
    head = `(${inner})`;
  } else {
    // basic solidity type strings in ABI JSON are already canonical
    head = base;
  }
  return `${head}${arraySuffix}`;
}

function canonicalSignature(abiItem) {
  const inputs = (abiItem.inputs || []).map(canonicalType).join(',');
  return `${abiItem.name}(${inputs})`;
}

function calculateSelector(abiItem) {
  const sig = canonicalSignature(abiItem);
  const hash = keccak256(toUtf8Bytes(sig));
  return hash.slice(0, 10); // 0x + 8 hex chars
}

// ---------- C++ type mapping from ABI param object ----------

const baseTypeMap = {
  // ints
  'bool': 'bool_t',
  'uint8': 'uint_t<8>', 'uint16': 'uint_t<16>', 'uint24': 'uint_t<24>', 'uint32': 'uint_t<32>',
  'uint56': 'uint_t<56>', 'uint64': 'uint_t<64>', 'uint96': 'uint_t<96>',
  'uint128': 'uint_t<128>', 'uint160': 'uint_t<160>', 'uint192': 'uint_t<192>', 'uint256': 'uint_t<256>',
  'int8': 'int_t<8>', 'int16': 'int_t<16>', 'int24': 'int_t<24>', 'int32': 'int_t<32>',
  'int56': 'int_t<56>', 'int64': 'int_t<64>', 'int96': 'int_t<96>',
  'int128': 'int_t<128>', 'int160': 'int_t<160>', 'int192': 'int_t<192>', 'int256': 'int_t<256>',
  'address': 'address20',
  'bytes': 'bytes',
  'string': 'string_t',
  // NOTE: bytes1..bytes32 are *fixed bytes*. If you want full correctness,
  // introduce a bytes_fixed<N>. As a pragmatic fallback, map to uint_t<8*N>.
  // e.g. 'bytes32': 'uint_t<256>'
};

// Add bytesN mappings
for (let n = 1; n <= 32; ++n) {
  baseTypeMap[`bytes${n}`] = `uint_t<${8*n}>`; // pragmatic fallback
}

function parseTupleType(components) {
  if (!components || components.length === 0) return 'tuple<>'; // empty tuple
  const ts = components.map(cppTypeForParam);
  return `tuple<${ts.join(', ')}>`;
}

function cppBaseTypeFrom(param) {
  const base = param.type.replace(/(\[[0-9]*\])*$/,'');
  if (base === 'tuple') return parseTupleType(param.components || []);
  return baseTypeMap[base] || 'uint_t<256>'; // conservative fallback
}

function wrapWithArrayDims(inner, arraySuffix) {
  // arraySuffix like "[][2][]"
  const dims = Array.from(arraySuffix.matchAll(/\[[0-9]*\]/g), m => m[0]);
  // Apply from right to left: the rightmost dimension is the innermost
  let t = inner;
  for (let i = dims.length - 1; i >= 0; --i) {
    const d = dims[i];
    if (d === '[]') t = `dyn_array<${t}>`;
    else {
      const n = d.slice(1, -1); // strip [ ]
      t = `static_array<${t}, ${n}>`;
    }
  }
  return t;
}

function cppTypeForParam(param) {
  const arraySuffix = (param.type.match(/(\[[0-9]*\])*$/) || [''])[0];
  const baseCpp = cppBaseTypeFrom(param);
  return wrapWithArrayDims(baseCpp, arraySuffix);
}

// ---------- Function type generation ----------

function generateCppType(abiItem) {
  if (abiItem.type !== 'function') return null;

  // Return type
  let returnType = 'void';
  if (abiItem.outputs && abiItem.outputs.length > 0) {
    if (abiItem.outputs.length === 1) {
      returnType = cppTypeForParam(abiItem.outputs[0]);
    } else {
      const outs = abiItem.outputs.map(cppTypeForParam);
      returnType = `tuple<${outs.join(', ')}>`;
    }
  }

  // Input types
  const inputTypes = (abiItem.inputs || []).map(cppTypeForParam);

  return { returnType, inputTypes };
}

function selectorToHex(selector) {
  const hex = selector.slice(2);
  const bytes = [];
  for (let i = 0; i < 8; i += 2) bytes.push(`0x${hex.slice(i, i + 2)}`);
  return bytes.join(',');
}

// keep buildFunctionSignature but make it canonical (for the comment)
function buildFunctionSignature(name, inputs) {
  const sig = `${name}(${(inputs||[]).map(canonicalType).join(',')})`;
  return sig;
}

function toPascalCase(str) {
  return str.charAt(0).toUpperCase() + str.slice(1);
}

function toCamelCase(str) {
  return str.charAt(0).toLowerCase() + str.slice(1);
}

async function generateProtocolsHeader() {
  let header = `#pragma once

#include "abi.h"

namespace abi {
namespace protocols {

// Auto-generated from ABI JSON files on ${new Date().toISOString()}
// Run: node scripts/generate_from_abi_json.mjs

`;

  // Process each ABI file
  for (const [contractName, abiPath] of Object.entries(abiFiles)) {
    try {
      const abiContent = fs.readFileSync(abiPath, 'utf8');
      const abi = JSON.parse(abiContent);
      
      header += `// ==============================\n`;
      header += `// ${contractName}\n`;
      header += `// ==============================\n\n`;
      
      for (const item of abi) {
        if (item.type === 'function') {
          const selectorName = `Sel_${toCamelCase(item.name)}`;
          
          // Calculate selector using the canonical signature
          const selector = calculateSelector(item);
          
          // Build the function signature for the comment
          const signature = buildFunctionSignature(item.name, item.inputs);
          
          const hexBytes = selectorToHex(selector);
          header += `struct ${selectorName} { static constexpr std::array<uint8_t,4> value{{${hexBytes}}}; }; // "${signature}"\n`;
          
          // Generate function typedef
          const cppType = generateCppType(item);
          if (cppType) {
            const functionName = toPascalCase(item.name);
            
            // Store function info - return type first, then arguments
            const args = [cppType.returnType, ...cppType.inputTypes].join(', ');
            header += `using ${functionName} = Fn<${selectorName}, ${args}>;\n`;
          }
          
          header += '\n';
        }
      }
      
    } catch (error) {
      console.error(`Error processing ${abiPath}:`, error.message);
    }
  }

  header += `} // namespace protocols\n`;
  header += `} // namespace abi\n`;
  
  return header;
}

// Main execution
try {
  generateProtocolsHeader().then(headerContent => {
    const outputPath = 'include/abi/protocols.h';
    
    // Ensure the directory exists
    const outputDir = path.dirname(outputPath);
    if (!fs.existsSync(outputDir)) {
      fs.mkdirSync(outputDir, { recursive: true });
    }
    
    // Write the generated header
    fs.writeFileSync(outputPath, headerContent);
    console.log(`Generated protocols header file: ${outputPath}`);
  });
  
} catch (error) {
  console.error('Error generating header:', error);
  process.exit(1);
}

