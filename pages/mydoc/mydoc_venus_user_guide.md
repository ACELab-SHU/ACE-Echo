---
title: "Writing a Venus task"
layout: echo-doc
summary: "Vector types, explicit task returns and a small example from the release."
sidebar: mydoc_sidebar
permalink: mydoc_venus_user_guide.html
section: Documentation
content_status: "ACE-Echo 1.0"
last_reviewed: 2026-09-13
---

Venus C uses the custom `venus.h` interface and Venus LLVM extensions. Stock LLVM cannot compile the same target by simply adding a RISC-V flag. Use the compiler identity paired with the selected backend.

## Vectors and element widths

In `__v32i16`, `32` is the number of elements and `16` is the width of each element in bits. The documented examples use 8-bit and 16-bit elements. Vector storage and legal operations depend on the compiler and backend; an arbitrary type spelling does not imply unlimited hardware capacity.

## A complete small task

The following task is copied from the bundled software smoke:

```c
#include "venus.h"

int Task_forgeAdd(__v32i16 a, __v32i16 b) {
    __v32i16 sum;
    vclaim(sum, 32);
    sum = vadd(a, b, MASKREAD_OFF, 32);
    vreturn(sum, 64);
    return 0;
}
```

It operates on 32 signed 16-bit elements and returns 64 bytes. The sample uses bounded inputs without arithmetic overflow. Its `vreturn` byte count describes the actual task output; it is separate from the BAS allocation capacity.

## Check the task interface

| Item | Why it matters |
|---|---|
| Input order and element type | BAS bindings must match the compiled task signature |
| Intrinsic semantics | Operand order, signedness and saturation are operation-specific |
| Allocation and liveness | Vector capacity and overlapping live values affect correctness |
| Runtime return size | This drives Venus1 output and dependency transport |
| Reference cases | Asymmetric inputs expose operand reversals that equal-input tests can miss |

The paired restore task is a useful example of an operation whose operand order needs care. Read [its compiler-specific explanation](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/workloads/5g_lite/tasks/forge_vector_smoke/Task_forgeRestore.c) instead of inferring semantics from the intrinsic name.

## Where to go next

- [Task source and provenance](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/workloads/5g_lite/tasks/forge_vector_smoke/Task_forgeAdd.c)
- [Connect the tasks with BAS](mydoc_dsl_user_guide.html)
- [Select and configure the compiler](venus1_get_started.html)

The [original intrinsic catalog](https://github.com/ACELab-SHU/ACE-Echo/blob/8770c29996116dfd2bb791fdd5132a0949c294b3/pages/mydoc/mydoc_venus_user_guide.md) is retained as a historical reference. Its signatures, operation comments and memory assumptions are not a versioned ISA specification for the 1.0 backend.
