    .include "six/asm/prelude.s"
    .include "six/asm/macros.s"
    .include "asm/m4a.s"

    rodata gSongTable global
    .include "build/assets/audio-table.s"
    endr
