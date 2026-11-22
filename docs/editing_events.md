# Editing event bytecode

Run `make extract` with your original US ROM. Each of the 1328 scripts now has
`assets/scripts/NNNN.evt`: readable CODE instructions and, where present, its
JUMP switch tables. Edit that file, then run `make`. Text itself remains in
`assets/text/NNNN.txt`; `STR_0` means string ID 0 in that file, not a ROM address.
No extracted scripts or ROM-derived binaries belong in git.

Existing `.evt` files are never overwritten without `make extract FORCE=1`.
When migrating an existing asset tree, newly created `.evt` files use the
current `.code.bin`/`.jump.bin` contents, preserving prior binary edits.
An `.evt` takes precedence over those binaries. The builder still supports
the original binaries when an `.evt` is absent; automatic extraction can
regenerate the missing `.evt` from them. Unknown chunks, original chunk order,
CODE trailers and unchanged STR payload layout remain in the JSON metadata.
Keep that metadata and preserve its JUMP chunk presence.

## Source format

```
.code
    EVT_PUSH8 STR_0
    EVT_CALL 33
    EVT_BRANCH finished
finished:
    EVT_END
```

Comments begin with `#`. Labels stand on their own lines. Decimal, hexadecimal
(`0x...`) and signed integer operands are accepted, subject to their encoded
width. This is a bounded event assembler, not GAS or Python: no expressions,
includes or macros are executed. Errors identify source file/line where possible.
Operands describe encoded bits: PUSH8/PUSH16 zero-extend at runtime, so their
`-1` bit patterns mean 255/65535. Use PUSH32 for negative stack values.

The 37 VM handlers `0x00..0x24` are defined by `AScriptEngine::NextInstruction`
in `src/script_engine.cc`:

| Hex | Mnemonics (`EVT_` prefix on each) | Operand |
|---|---|---|
| 00..06 | NOP, STORE_IND, ADD_STORE, SUB_STORE, MUL_STORE, DIV_STORE, MOD_STORE | none |
| 07..12 | ADD, SUB, MUL, DIV, MOD, AND, OR, INC, DEC, NEG, NOT, CMP | none |
| 13..14 | LOAD, STORE | 32-bit VM memory index |
| 15..16 | DUP, POP | none |
| 17 | PUSH32 | 32-bit integer |
| 18..1E | BRANCH, BRANCH_LT, BRANCH_LE, BRANCH_EQ, BRANCH_NE, BRANCH_GE, BRANCH_GT | 32-bit CODE offset/label |
| 1F..20 | BRANCH_STACK, END | none |
| 21 | CALL | 32-bit native handler ID |
| 22..23 | PUSH16, PUSH8 | 16/8-bit integer |
| 24 | SWITCH | 32-bit switch table **index**, not offset |

`STORE_IND` and arithmetic-store operations consume an address and value on
the stack, updating VM memory and leaving the result. Conditional branches
pop a signed comparison value; CMP produces -1/0/+1. END returns 2 to stop
execution. CALL invokes `OnCall(id)`; it is **not** a jump to a CODE offset.
Native calls retain numeric IDs rather than guessing undocumented semantics.

`Opcode` masks bit 7; `Operand8/16/32` add `unk_348` when it is set. The `.BASE`
suffix preserves this bit, e.g. `EVT_LOAD.BASE 2`. Such operands are dynamic,
so branch bounds/switch IDs cannot be statically resolved. No original script
uses this flag. Unknown low-seven-bit opcodes are preserved as `OP_xx`
(one byte: the engine's default handler returns -1); they are not usable new
commands. Labels are recomputed when instruction sizes change. Static jumps
must land on instruction boundaries, never inside operands.

## Strings and native calls

There is no dedicated string opcode: native handlers pop STR IDs from the
stack. Disassembly writes `STR_n` for contiguous immediate pushes consumed by
verified string readers in `func_0803F8DC` (`ScriptEngine::OnCall`):
33/34/35 (dialogue), 36..43 (prompt/choice strings), 45 (speaker name),
58 (text variable), 251 (default-speed dialogue), and 293 (a string consumer).
Their string-table reads use fields +0x10/+0x14/+0x18, matching `GetString`.
Computed IDs and arguments not provably immediate remain numeric. You can
use `STR_n` explicitly with any PUSH instruction; assembly checks that the
ID exists and fits the original operand width. It rejects `.BASE STR_n`.
Do not change small integers into string references merely because their
values happen to match a string ID.

For example, swap the first two `EVT_PUSH8 STR_0` / `STR_1` operands in
`0867.evt` to swap Thomas's first two spoken strings without modifying text.
Keep CALLs and surrounding scene operations in place.

## Switch tables

```
.code
    EVT_PUSH8 1
    EVT_SWITCH 0
first:
    EVT_END
other:
    EVT_END
.jump
    .word 1          # number of tables
    .word JT_0000    # offset measured from JUMP payload + 4
JT_0000:
    .word 1          # number of cases
    .word other      # default target; -1 or 0 means fall through
    .case 1, first   # signed value, CODE target
```

This follows the actual SWITCH handler: offsets are relative to the bytes
**after** the count, not relative to the RIFF file or the ROM. CODE labels
are relative to the first instruction. Case values must stay sorted in signed
order for `std::lower_bound`; update the explicit count when adding cases.
Table aliases, padding words and unused trailing words are preserved. Default
targets <= 0 fall through (the runtime cannot use offset 0 as a default).

## Checks and limits

```
make assets-test
python3 tools/assets/check_event_roundtrip.py
make clean && make -j8 compare
python3 tools/assets/check_event_rom.py
```

The roundtrip verifies all CODE/JUMP bytes and complete script containers.
The last command builds the Thomas string-reference swap, reparses the linked
ROM, saves `build/assets/event-thomas-swapped.gba`, and restores the source
and vanilla build in `finally`. It starts only from a compare-OK ROM.

Edited scripts still use the existing symbolic script table and script overflow
placement. The tooling validates byte structure, labels, string IDs and switch
tables, not scene semantics, stack balance or whether an event will terminate.
Review edited control flow and test gameplay in an emulator separately.
