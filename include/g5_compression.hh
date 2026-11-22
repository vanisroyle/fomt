#ifndef G5_COMPRESSION_HH
#define G5_COMPRESSION_HH

#include "prelude.h"

// Unpack's asset format starts with a little-endian word:
//     (uncompressed_byte_count << 8) | 0x70.
// Subsequent little-endian u32 words are consumed most-significant bit first.
// The first eight stream bits select:
//     bits 0..2: LZSS mode (0..3) or raw atoms (4);
//     bits 3..4: raw 8-bit atoms (0), Huffman nibbles (1), Huffman bytes (2);
//     bits 5..7: no difference filter (0), or prefix sums over nibbles (1),
//               bytes (2), little-endian halfwords (3), interleaved bytes (4).
// Nibble atoms are joined high nibble first. The nibble difference filter
// likewise processes each byte's high nibble before its low nibble.
//
// Huffman tables enumerate canonical codes of lengths 1..2*symbol_bits.
// Each length has a symbol_bits-bit count followed by that many symbols,
// each also symbol_bits bits wide. Codes are assigned in stream order.
//
// LZSS modes 0, 1, 2 and 3 read distance ladders of 2, 4, 7 and 3 entries.
// Each entry supplies a 4-bit width minus one; its base starts at one and
// increases by 1 << width after each entry. Backreferences may overlap.
// Mode 0 uses two-bit tags for ladder matches, literal runs and byte runs.
// Modes 1 and 2 use a literal/match bit; mode 2 adds variable-length runs.
// Mode 3 uses two-byte atoms and halfword-scaled backreferences. The final
// difference filter reconstructs the original values modulo the unit size.
// See tools/scripts/decompress.py for the corresponding host decoder.
//
// The ROM routine trusts its input: it does not check the signature, modes,
// references or destination capacity. Input is word-aligned and output is
// halfword-aligned. Helpers write completed halfwords; use nonzero, even
// output byte counts and word-padded compressed input.
//
// The retained assembly helpers share a non-C ABI: r0 is the input cursor,
// r1 the output cursor (initially destination - 2), r2/r3 the bit buffer/count,
// r4 the requested bit count/result, r7 the 0x420-byte scratch area,
// r8 destination + byte_count - 2 and r9 the atom reader. r10-r12 are private.
EC void Unpack(void const * source, void * destination); // 0x080D102C

#endif
