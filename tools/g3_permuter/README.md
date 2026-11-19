# agbcp / decomp-permuter

This adapter compiles candidate C++ with the Makefile's preprocessing, agbcp,
section-alignment, and ARM7TDMI assembly flags. It does not replace the normal
build or change its configuration.

## Install and prepare

```sh
git clone https://github.com/simonlindholm/decomp-permuter.git "$HOME/decomp-permuter"
python3 -m venv "$HOME/permuter-venv"
"$HOME/permuter-venv/bin/pip" install toml Levenshtein
```

Create a persistent directory for each function. From the FoMT repository:

```sh
python3 tools/g3_permuter/prepare_target.py asm/code_809E804.s \
    func_080B36B4 "$HOME/permutations/080B36B4"
```

Use an original assembly snapshot after splitting a function. Check the entry,
preceding function, and external branches before treating an assembly label as a
separate function. An internal label belongs to its parent, not a separate job.

Each directory needs `base.c`, `compile.sh`, `settings.toml`, and `target.o`.
The `.c` extension is required by the permuter; this adapter always compiles C++.
For example, create this **per-function** `compile.sh` using absolute paths:

```sh
#!/bin/bash
export FOMT_ROOT=/absolute/path/to/fomt
export FOMT_PERM_CONTEXT=/absolute/path/to/function/context.hh
export FOMT_PERM_SECTION=.text.g3_080B36B4
exec bash "$FOMT_ROOT/tools/g3_permuter/compile.sh" "$@"
```

`FOMT_PERM_CONTEXT` optionally injects a header containing the real C++ types,
inline methods, templates, and declarations. `FOMT_PERM_SECTION` optionally keeps
only the function's assembly section before assembly, excluding unrelated
constructors, inline-function emissions, and debug sections from scoring.
Keep the literal pools and jump tables belonging to the selected function.

Example `settings.toml`:

```toml
compiler_type = "gcc"
func_name = "UpdateChickenFeedingChickens"
```

## C++ parser bridge

The permuter parses C, not C++. Use `PERM_PRETEND(...)` for C-compatible type
views/prototypes needed by its type inferencer; they disappear before compiling.
Put actual definitions in `context.hh`. Use `PERM_IGNORE(...)` around C++-only
declarations such as reference bindings, with a corresponding pretend C local.
Function attributes can also be protected:

```c
PERM_IGNORE(EC SECTION(".text.g3_080B36B4"))
void UpdateChickenFeedingChickens(ChickenFeedingState *state) { /* candidate */ }
```

Templates and constructor expressions can be represented by ordinary-looking
function calls to the C parser, then expanded by macros in `context.hh` back to
the unchanged C++ expressions. Do not replace them with out-of-line helpers:
that changes inlining, lifetime, and register allocation. Include all called
function prototypes and correct struct members in the parser views.

Use `0` rather than the `nullptr` macro in parser input. With the upstream
permuter version used for batch 5, declarations in `for` initializers trigger
`DeclList` errors in some randomizations. Move each declaration just before its
loop inside an enclosing block, preserving the original scope.

## Run and validate

```sh
"$HOME/permuter-venv/bin/python" "$HOME/decomp-permuter/permuter.py" \
    "$HOME/permutations/080B36B4" --debug --stack-diffs --algorithm levenshtein

"$HOME/permuter-venv/bin/python" -u "$HOME/decomp-permuter/permuter.py" \
    "$HOME/permutations/080B36B4" "$HOME/permutations/another-function" \
    -j 4 --stack-diffs --algorithm levenshtein --best-only --stop-on-zero \
    > "$HOME/permutations/run.log" 2>&1
```

Launch independent groups in separate background shell sessions if desired.
`--stop-on-zero` stops the whole group after one zero; restart without that
function to keep searching the others. Improved candidates are saved in
`output-SCORE-N/source.c`, with their score and diff.

Always enable `--stack-diffs` for stack-layout near-misses. A score of zero is
only a lead: branch targets and some assembly details are normalized by the
scorer. Port the result into readable C++, update all callers and linker splits,
then require `make -j$(nproc) compare` to print `fomt.gba: OK` before retaining it.
