---
title: "BAS and DAG compilation"
layout: echo-doc
summary: "Connect Venus tasks through explicit inputs, dependencies and returns."
sidebar: mydoc_sidebar
permalink: mydoc_dsl_user_guide.html
section: Documentation
content_status: "ACE-Echo 1.0"
last_reviewed: 2026-09-13
---

BAS describes a directed acyclic graph: nodes invoke tasks, and edges carry returned data to later tasks. The release bundles a revision-pinned DSL frontend and uses an external Venus-custom LLVM installation for task compilation.

## A graph from the bundled smoke

```basic
return_value short sum[32]
return_value short restored[32]
```

The sample initializes two vectors, `a` and `b`, with 32 signed 16-bit elements each, then connects its two tasks:

```basic
dag dag1 = {
  [sum] = Task_forgeAdd(a, b)
  [restored] = Task_forgeRestore(sum, b)
}
END
```

These are excerpts. Use the [complete BAS file](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/workloads/5g_lite/tasks/forge_vector_smoke/forge_vector_smoke.bas) with its parameter arrays and matching task sources.

## Declarations and dependencies

| Construct | Role |
|---|---|
| `parameter` | Declares sample data used by the graph |
| `dag_input` | Declares an input supplied by the enclosing application |
| `dfedata` | Declares digital-front-end input in the existing BAS convention |
| `return_value` | Declares a graph output and its allocation metadata |
| `dag` | Connects named task calls and their returned values |
| `END` | Terminates the description |

Use the spelling and types accepted by the paired frontend. The older page's `dfe_data`, duplicated DAG and full-width comma examples have been removed. Scalar/vector type support in the frontend is not a promise that every type is supported by every hardware backend.

## Compile for the configured Venus1 backend

Run from `platform/` after setup:

```bash
./ace-echo --config .ace-echo/host/local.toml compile dag \
  --target forge_vector_smoke --backend .ace-echo/host/backend.json
```

Compilation uses a run-local frontend copy. The generated JSON, combined BIN and task images are collected in the run artifacts. Use those recorded paths for later simulation. Configure host paths through `.ace-echo/host-tools.json`; do not copy another developer's absolute `LLVM_PATH` or mix lane/row settings manually.

## Allocation size and transferred data

An output declaration's allocation capacity can exceed the data returned at runtime. For Venus1 ordinary task dependencies, RTL transport uses the producer's runtime return byte count through the 16-bit DMT length field. The platform follows that behavior; a receiver's declared capacity is advisory and does not silently shorten the transfer. Above 65535 bytes, the 16-bit field truncation must be considered.

This does not make an overlapping live memory region safe. See [the transfer-length contract](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/CONSUMER_DMA_LENGTH.md) and [optional external BAS parameters](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/BAS_PARAMETER_INPUTS.md) for precise behavior.
