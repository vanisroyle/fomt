# Boot/system batch 8: blocked matches

Base: `930729d`. Nonzero candidates remain in assembly.

## 08010024 — item collection clear — score 25

Blocked after approximately 25 minutes: improved 40 → 30 → 25, then a
follow-up search and context matrix made no further improvement.
There are no stack, branch, instruction-count or literal differences.
The target copies `r0 → r2` before `r5 → r1`; the candidate reverses
those copies. Its initial comparison uses `r0` instead of `r2`.
The loop-back comparison and final subtraction/store already match.
These account for five register-operand differences.

Tried parameter/reference and return-type matrices, pointer/word and iterator
representations, nested clear/erase helpers, copying/destruction algorithms,
and six-worker random searches of both caller and erase body.
The validated candidate and exact diagnostic are preserved at
`/home/ubuntu/boot-system3/08010024/seed25-batch8/`
(`base.c`, `debug25.log`, `seed25.log`).
