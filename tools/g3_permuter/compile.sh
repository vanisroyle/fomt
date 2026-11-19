#!/bin/bash
set -euo pipefail

ROOT=${FOMT_ROOT:-$(realpath "$(dirname "${BASH_SOURCE[0]}")/../..")}
SOURCE=$(realpath "$1")
OUTPUT=$(realpath "$3")
ASM=$(mktemp "${OUTPUT}.XXXXXX.s")
trap 'rm -f "$ASM"' EXIT

CONTEXT=()
if [[ -n ${FOMT_PERM_CONTEXT:-} ]]; then
    CONTEXT=(-include "$FOMT_PERM_CONTEXT")
fi

cd "$ROOT"
arm-none-eabi-cpp -x c++ "${CONTEXT[@]}" \
    -I tools/agbcc/include -I tools/libagbc++ -I tools/libsix/include \
    -iquote . -iquote include -Wno-trigraphs -fno-exceptions "$SOURCE" |
    tools/agbcc/bin/agbcp -quiet -fno-exceptions -fno-rtti -fvtable-thunks \
        -g -mthumb-interwork -Wimplicit -Wparentheses -Werror -O2 -fhex-asm -o "$ASM"
tools/scripts/align_sections.sh "$ASM"

if [[ -n ${FOMT_PERM_SECTION:-} ]]; then
    python3 tools/g3_permuter/select_section.py "$ASM" "$FOMT_PERM_SECTION"
fi

arm-none-eabi-as -I tools/agbcc/include -I tools/libagbc++ -I tools/libsix/include \
    -I . -I include -mcpu=arm7tdmi "$ASM" -o "$OUTPUT"
