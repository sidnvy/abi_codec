#!/usr/bin/env node

import { keccak256, toUtf8Bytes } from 'ethers';
import fs from 'fs';
import path from 'path';

// ABI JSON file paths
const abiFiles = {
  "ERC20": "abis/erc20.json",
  "UniswapV3Pool": "abis/uniswap_v3_pool.json",
  "UniswapV3TickLens": "abis/uniswap_v3_tick_lens.json",
  "Multicall": "abis/multicall.json",
  "UniswapV2Router": "abis/uniswap_v2_router.json"
};

// ---------- Named struct extraction and management ----------

// Global registry of named structs found in ABI
const namedStructs = new Map();

// Extract named struct information from ABI
function extractNamedStructs(abi, contractName) {
  for (const item of abi) {
    if (item.type === 'function') {
      // Check inputs for named structs
      if (item.inputs) {
        for (const input of item.inputs) {
          extractNamedStructFromParam(input, contractName);
        }
      }
      // Check outputs for named structs
      if (item.outputs) {
        for (const output of item.outputs) {
          extractNamedStructFromParam(output, contractName);
        }
      }
    }
  }
}

function extractNamedStructFromParam(param, contractName) {
  // Handle array types that contain structs
  if (param.type && param.type.endsWith('[]') && param.internalType) {
    // Extract element type from array
    const elementType = param.type.slice(0, -2); // remove []
    if (elementType === 'tuple' && param.components && param.internalType.includes('struct')) {
      const match = param.internalType.match(/struct\s+([^.]+)\.(.+)\[\]/);
      if (match) {
        const [, contract, structName] = match;
        const fullName = `${contract}.${structName}`;

        if (!namedStructs.has(fullName)) {
          namedStructs.set(fullName, {
            contract,
            name: structName,
            fields: param.components.map(comp => ({
              name: comp.name,
              type: comp.type,
              internalType: comp.internalType,
              components: comp.components
            }))
          });
        }
      }
    }
  }

  if (param.type === 'tuple' && param.components && param.internalType) {
    // Extract struct name from internalType like "struct ContractName.StructName"
    const match = param.internalType.match(/struct\s+([^.]+)\.(.+)/);
    if (match) {
      const [, contract, structName] = match;
      const fullName = `${contract}.${structName}`;

      if (!namedStructs.has(fullName)) {
        namedStructs.set(fullName, {
          contract,
          name: structName,
          fields: param.components.map(comp => ({
            name: comp.name,
            type: comp.type,
            internalType: comp.internalType,
            components: comp.components
          }))
        });
      }
    }
  }

  // Recursively check components
  if (param.components) {
    for (const comp of param.components) {
      extractNamedStructFromParam(comp, contractName);
    }
  }
}

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

function canonicalEventSignature(abiItem) {
  // Event signature includes ALL parameters (both indexed and non-indexed)
  const inputs = (abiItem.inputs || []).map(canonicalType).join(',');
  return `${abiItem.name}(${inputs})`;
}

function calculateSelector(abiItem) {
  const sig = canonicalSignature(abiItem);
  const hash = keccak256(toUtf8Bytes(sig));
  return hash.slice(0, 10); // 0x + 8 hex chars
}

function calculateEventTopic(abiItem) {
  const sig = canonicalEventSignature(abiItem);
  const hash = keccak256(toUtf8Bytes(sig));
  return hash.slice(0, 66); // 0x + 64 hex chars (32 bytes)
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

function cppBaseTypeFrom(param) {
  const base = param.type.replace(/(\[[0-9]*\])*$/,'');
  if (base === 'tuple') {
    // Check if this is a named struct
    if (param.internalType) {
      // Strip array notation from internalType before extracting struct name
      const cleanInternalType = param.internalType.replace(/(\[[0-9]*\])*$/, '');
      const match = cleanInternalType.match(/struct\s+([^.]+)\.(.+)/);
      if (match) {
        const [, contract, structName] = match;
        // Use a simplified name for the struct
        return `${contract}_${structName}`;
      }
    }
    return parseTupleType(param.components || []);
  }
  return baseTypeMap[base] || 'uint_t<256>'; // conservative fallback
}

// ---------- Function/Event type generation ----------

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

function generateEventType(abiItem) {
  if (abiItem.type !== 'event') return null;

  // Separate indexed and non-indexed parameters
  const indexedParams = (abiItem.inputs || []).filter(p => p.indexed);
  const nonIndexedParams = (abiItem.inputs || []).filter(p => !p.indexed);

  // Data types (non-indexed parameters only)
  const dataTypes = nonIndexedParams.map(cppTypeForParam);

  return {
    indexedParams,
    nonIndexedParams,
    dataTypes,
    isAnonymous: abiItem.anonymous || false
  };
}

function selectorToHex(selector) {
  const hex = selector.slice(2);
  const bytes = [];
  for (let i = 0; i < 8; i += 2) bytes.push(`0x${hex.slice(i, i + 2)}`);
  return bytes.join(',');
}

function topicToHex(topic) {
  const hex = topic.slice(2);
  const bytes = [];
  for (let i = 0; i < 64; i += 2) bytes.push(`0x${hex.slice(i, i + 2)}`);
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

function generateNamedSchemas() {
  let output = '';

  for (const [fullName, structInfo] of namedStructs) {
    const structName = `${structInfo.contract}_${structInfo.name}`;
    const schemaTypes = structInfo.fields.map(field => cppTypeForParam(field));
    const schemaTuple = `abi::tuple<${schemaTypes.join(', ')}>`;

    output += `// ---------- Named schema for ${fullName} ----------\n`;
    output += `struct ${structName} {\n`;

    // Value type fields
    for (const field of structInfo.fields) {
      const fieldType = cppTypeForParam(field);
      const fieldName = toCamelCase(field.name || `f${structInfo.fields.indexOf(field)}`);
      output += `  abi::cpp_t<${fieldType}> ${fieldName};\n`;
    }

    // Underlying ABI schema
    output += `\n  // Underlying ABI schema\n`;
    output += `  using schema = ${schemaTuple};\n`;

    // to_tuple conversion
    output += `\n  // Conversion to underlying tuple\n`;
    output += `  static abi::value_of<schema>::type to_tuple(const ${structName}& s) {\n`;
    output += `    return std::make_tuple(\n`;
    structInfo.fields.forEach((field, index) => {
      const fieldName = toCamelCase(field.name || `f${index}`);
      const isLast = index === structInfo.fields.length - 1;
      output += `      s.${fieldName}${isLast ? '' : ','}\n`;
    });
    output += `    );\n`;
    output += `  }\n`;

    // from_tuple conversion
    output += `\n  // Conversion from underlying tuple\n`;
    output += `  static ${structName} from_tuple(const abi::value_of<schema>::type& t) {\n`;
    output += `    ${structName} s{};\n`;
    structInfo.fields.forEach((field, index) => {
      const fieldName = toCamelCase(field.name || `f${index}`);
      output += `    s.${fieldName} = std::get<${index}>(t);\n`;
    });
    output += `    return s;\n`;
    output += `  }\n`;

    // Add from_non_indexed method for complete structs (if this has indexed fields)
    if (structInfo.fields && structInfo.fields.some(f => f && f.indexed)) {
      // This is a complete struct with both indexed and non-indexed fields
      const contractName = structInfo.contract;
      const baseName = structInfo.name.replace('CompleteEventData', 'EventData');
      const nonIndexedStructName = `${contractName}_${baseName}`;

      output += `\n  // Initialize from non-indexed data (for complete event decoding)\n`;
      output += `  static ${structName} from_non_indexed(const ${nonIndexedStructName}& non_indexed) {\n`;
      output += `    ${structName} s{};\n`;

      // Copy non-indexed fields from the non-indexed struct
      const nonIndexedFields = structInfo.fields.filter(f => f && !f.indexed);
      nonIndexedFields.forEach((field) => {
        if (field) {
          const fieldName = toCamelCase(field.name || `f${structInfo.fields.indexOf(field)}`);
          output += `    s.${fieldName} = non_indexed.${fieldName};\n`;
        }
      });

      // Initialize indexed fields to default values
      const indexedFields = structInfo.fields.filter(f => f && f.indexed);
      indexedFields.forEach(field => {
        if (field) {
          const fieldName = toCamelCase(field.name || `f${structInfo.fields.indexOf(field)}`);
          if (field.type === 'address') {
            output += `    s.${fieldName} = {}; // Will be filled from topics\n`;
          } else if (field.type.startsWith('uint') || field.type.startsWith('int')) {
            output += `    s.${fieldName} = 0; // Will be filled from topics\n`;
          } else if (field.type === 'bool') {
            output += `    s.${fieldName} = false; // Will be filled from topics\n`;
          } else {
            output += `    // ${fieldName} will be filled from topics\n`;
          }
        }
      });

      output += `    return s;\n`;
      output += `  }\n`;
    }

    output += `};\n\n`;
  }

  return output;
}

function generateTraitsSpecializations() {
  let traits = '';

  console.log(`generateTraitsSpecializations called with ${namedStructs.size} named structs:`);
  for (const [key, value] of namedStructs) {
    console.log(`  - ${key}: ${value.contract}_${value.name}`);
  }

  // Generate value_of specializations first
  traits += `// value_of specializations for named structs\n`;
  for (const [fullName, structInfo] of namedStructs) {
    const structName = `protocols::${structInfo.contract}_${structInfo.name}`;
    traits += `template<> struct value_of<${structName}> { using type = ${structName}; };\n`;
  }
  traits += `\n`;

  // Generate traits specializations that delegate to underlying tuple schema
  traits += `// traits specializations that delegate to underlying tuple schema\n`;
  for (const [fullName, structInfo] of namedStructs) {
    const structName = `protocols::${structInfo.contract}_${structInfo.name}`;

    traits += `template<> struct traits<${structName}> {\n`;
    traits += `  using S = typename ${structName}::schema;\n`;
    traits += `  static constexpr bool is_dynamic  = traits<S>::is_dynamic;\n`;
    traits += `  static constexpr size_t head_words= traits<S>::head_words;\n\n`;

    traits += `  static size_t tail_size(const ${structName}& v) {\n`;
    traits += `    return traits<S>::tail_size( ${structName}::to_tuple(v) );\n`;
    traits += `  }\n\n`;

    traits += `  static void encode_head(uint8_t* out32, size_t hi, const ${structName}& v, size_t base) {\n`;
    traits += `    traits<S>::encode_head(out32, hi, ${structName}::to_tuple(v), base);\n`;
    traits += `  }\n\n`;

    traits += `  static void encode_tail(uint8_t* out, size_t base, const ${structName}& v) {\n`;
    traits += `    traits<S>::encode_tail(out, base, ${structName}::to_tuple(v));\n`;
    traits += `  }\n\n`;

    traits += `  static bool decode(BytesSpan in, ${structName}& out, Error* e=nullptr) {\n`;
    traits += `    abi::value_of<S>::type tmp;\n`;
    traits += `    if (!traits<S>::decode(in, tmp, e)) return false;\n`;
    traits += `    out = ${structName}::from_tuple(tmp);\n`;
    traits += `    return true;\n`;
    traits += `  }\n`;

    traits += `};\n\n`;
  }

  return traits;
}

async function generateProtocolsHeader() {
  // First pass: extract named structs from all ABI files
  for (const [contractName, abiPath] of Object.entries(abiFiles)) {
    try {
      const abiContent = fs.readFileSync(abiPath, 'utf8');
      const abi = JSON.parse(abiContent);
      extractNamedStructs(abi, contractName);
    } catch (error) {
      console.error(`Error processing ${abiPath}:`, error.message);
    }
  }

  let header = `#pragma once

#include "abi.h"

// Auto-generated from ABI JSON files on ${new Date().toISOString()}
// Run: node scripts/generate_from_abi_json.mjs

// ==============================
// Named Structs (in abi::protocols namespace)
// ==============================

namespace abi {
namespace protocols {

// NAMED_SCHEMAS_PLACEHOLDER

} // namespace protocols

// ==============================
// Traits specializations for named structs (in abi namespace)
// ==============================

// TRAITS_SPECIALIZATIONS_PLACEHOLDER

} // namespace abi

namespace abi {
namespace protocols {
`;

  // First pass: collect all named structs from all ABIs
  for (const [contractName, abiPath] of Object.entries(abiFiles)) {
    try {
      const abiContent = fs.readFileSync(abiPath, 'utf8');
      const abi = JSON.parse(abiContent);

      // Process events to collect named structs
      for (const item of abi) {
        if (item.type === 'event') {
          const eventType = generateEventType(item);
          if (eventType && !eventType.isAnonymous && eventType.dataTypes.length > 0) {
            const nonIndexedParams = item.inputs.filter(input => !input.indexed);
            if (nonIndexedParams.length > 0) {
              const fullName = `${contractName}.${toPascalCase(item.name)}EventData`;

              if (!namedStructs.has(fullName)) {
                console.log(`Adding event data struct: ${fullName}`);
                namedStructs.set(fullName, {
                  contract: contractName,
                  name: `${toPascalCase(item.name)}EventData`,
                  fields: nonIndexedParams.map(param => ({
                    name: param.name,
                    type: param.type,
                    internalType: param.internalType,
                    components: param.components
                  }))
                });
                console.log(`Event data fields:`, nonIndexedParams.map(p => p.name));
              }
            }
          }
        }
      }
    } catch (error) {
      console.error(`Error processing ${abiPath}:`, error.message);
    }
  }

  // Second pass: process each ABI file for function generation
  for (const [contractName, abiPath] of Object.entries(abiFiles)) {
    try {
      const abiContent = fs.readFileSync(abiPath, 'utf8');
      const abi = JSON.parse(abiContent);

      header += `// ==============================\n`;
      header += `// ${contractName}\n`;
      header += `// ==============================\n\n`;

      for (const item of abi) {
        if (item.type === 'function') {
          const selectorName = `Sel_${contractName}_${toCamelCase(item.name)}`;

          // Calculate selector using the canonical signature
          const selector = calculateSelector(item);

          // Build the function signature for the comment
          const signature = buildFunctionSignature(item.name, item.inputs);

          const hexBytes = selectorToHex(selector);
          header += `struct ${selectorName} { static constexpr std::array<uint8_t,4> value{{${hexBytes}}}; }; // "${signature}"\n`;

          // Generate function typedef
          const cppType = generateCppType(item);
          if (cppType) {
            const functionName = `${contractName}_${toPascalCase(item.name)}`;

            // Store function info - return type first, then arguments
            const args = [cppType.returnType, ...cppType.inputTypes].join(', ');
            header += `using ${functionName} = Fn<${selectorName}, ${args}>;\n`;
          }

          header += '\n';
        } else if (item.type === 'event') {
          const topicName = `Topic_${contractName}_${toPascalCase(item.name)}`;

          // Calculate topic hash using event signature (ALL params)
          const topic = calculateEventTopic(item);

          // Build the event signature for the comment
          const signature = canonicalEventSignature(item);

          const hexBytes = topicToHex(topic);
          header += `struct ${topicName} { static constexpr std::array<uint8_t,32> value{{${hexBytes}}}; }; // "${signature}"\n`;

          // Generate event typedef
          const eventType = generateEventType(item);
          if (eventType && !eventType.isAnonymous) {
            const eventName = `${contractName}_${toPascalCase(item.name)}Event`;
            const eventDataName = `${contractName}_${toPascalCase(item.name)}EventData`;

            // Collect non-indexed parameters only (simpler approach)
            const nonIndexedParams = item.inputs.filter(input => !input.indexed);

            if (nonIndexedParams.length > 0) {
              // Event typedef for non-indexed parameters
              header += `using ${eventName} = Event<${topicName}, ${eventDataName}>;\n`;
            } else {
              // Events with no non-indexed parameters are not supported
              console.log(`Skipping ${eventName}: no non-indexed parameters`);
            }
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

  // Replace the placeholder with actual named schemas
  const namedSchemasOutput = generateNamedSchemas();
  header = header.replace('// NAMED_SCHEMAS_PLACEHOLDER', namedSchemasOutput);

  // Replace the traits specializations placeholder
  const traitsOutput = generateTraitsSpecializations();
  header = header.replace('// TRAITS_SPECIALIZATIONS_PLACEHOLDER', traitsOutput);

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

