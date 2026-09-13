---
title: "DSL User Guide"
layout: echo-doc
guide_reference: true
summary: "BAS syntax, data declarations, task bindings, compiler configuration, artifacts and worked examples."
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

Use ASCII punctuation and the keyword `dfedata` (not `dfe_data`). The declaration examples and complete type reference below show the accepted forms.

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


## Data-type reference

| BAS type | Element storage | Typical use |
|---|---|---|
| `char` | 1 byte | Bits, bytes and 8-bit task vectors |
| `short` | 2 bytes | 16-bit task vectors and scalar containers |
| `int` | 4 bytes | Integer data / application metadata |
| `float` | 4 bytes | Floating-point data in the frontend |
| `double` | 8 bytes | Double-precision data in the frontend |

These types describe the DSL's data representation. The Venus1 vector arithmetic backend supports 8-bit and 16-bit elements; declaring a `float` array does not create a floating-point Venus1 vector instruction.

A BAS scalar parameter is conventionally represented by a one-element array, such as `parameter short mode = {15}`. Match the generated task interface: some task signatures use a 64-byte-aligned `short_struct`, rather than a raw C `short` argument. Array element count and ABI container size are separate.

## Declaration syntax with examples

### Static parameter

```basic
parameter char bits = {1,0,1,1,0,0,1,0}
parameter short samples = {-3,-1,1,3}
parameter short mode = {15}
```

`parameter TYPE NAME = {VALUES}` embeds initialization data. Its element count comes from the initializer. Use decimal numeric literals, ASCII commas and no trailing comma. Put a minus sign before a negative literal. A declaration has no terminating semicolon.

### Application and digital-front-end inputs

```basic
dag_input short nCellid[1]
dfedata char iq[4420]
```

`dag_input TYPE NAME[CAPACITY]` declares input from the application; `dfedata TYPE NAME[CAPACITY]` denotes digital-front-end input in the BAS convention. These declare storage and bindings, not sample values. Supply their runtime data through a matching launcher/input contract. Preserve the input order recorded in generated metadata rather than assuming declaration order.

### Graph returns

```basic
return_value short subFrameNum[1]
return_value char decoded[256]
```

The variable must be produced by a task call in the graph. The brackets reserve capacity in elements. They do not force the task to return that many bytes. For instance, `decoded[256]` may hold a shorter payload whose length is determined by `vreturn`.

### Globals

```basic
global short mode = 1
```

The frontend also accepts `global TYPE NAME` and an integer initializer form. Globals have different allocation/lifetime behavior from task results. For ordinary standalone examples, prefer a `parameter` or a task-produced value; use the global form when reproducing an application designed around it.

### Names, comments and line endings

```basic
' This whole line is a comment.
parameter short gain = {2} ' This is an end-of-line comment.
```

Names begin with a letter or underscore, followed by letters, digits or underscores. Avoid language keywords such as `data` as identifiers. The lexer treats `'` through the end of the line as a comment. Both whole-line and inline comments are accepted; a leading apostrophe comments out the declaration itself. Use a final newline after `END`.

## Task calls and dependency bindings

```basic
dag dag1 = {
    [sum] = Task_forgeAdd(a, b)
    [restored] = Task_forgeRestore(sum, b)
}
END
```

The function name selects the task. The right-hand argument list follows its C task signature; the left-hand list follows its ordered `vreturn(buffer, bytes, ...)` pairs. Reusing a produced name in another task creates a data dependency. Intermediate names are introduced by task results and need not all be graph returns.

A task with multiple outputs uses the following **syntax pattern**:

```basic
dag dag1 = {
    [payload, status] = Task_decode(samples, mode)
    [result] = Task_consume(payload, status)
}
END
```

Here `Task_decode` / `Task_consume` are illustrative names; supply matching task sources before compiling. Do not duplicate `dag dag1` as the old quick-start example did.

The parser also accepts pointer forms and SPMD pragmas used by other backend configurations. The Venus1 manifest does not qualify pointer input descriptors or SPMD task-container fields. Start with ordinary value dependencies on Venus1; syntax acceptance alone does not select hardware support.

## Complete working example

The [Venus User Guide](mydoc_venus_user_guide.html#complete-two-task-example) includes both C files and the entire `forge_vector_smoke.bas`, including all 32-element inputs. Its expected outputs are `sum[i]=15-i` and `restored[i]=i-16`. Unlike a syntax sketch, it corresponds to a bundled compilation target and reference assets.

## Compiler configuration reference

The original low-level configuration reference remains useful when diagnosing a build. ACE-Echo 1.0 generates the run-local values from the selected host and backend; use the setup commands in [Get started](venus1_get_started.html) to select them together.

| Variable / tool | Meaning | Venus1 usage |
|---|---|---|
| `LLVM_PATH` | Directory containing the custom LLVM binaries | Use the Venus1 compiler paired with the backend |
| `RVPATH` | RISC-V GCC toolchain root | Supplies the target toolchain/runtime prerequisites |
| `TARGET_DAG` | Workload selected for compilation | CLI `--target` selects this |
| `CC`, `OPT`, `LLC` | `clang`, `opt`, `llc` | C → LLVM IR → optimized IR → target assembly |
| `DUMP`, `CPY` | `llvm-objdump`, `llvm-objcopy` | Inspect / extract target artifacts |
| `PYTHON` | Python interpreter | Runs the DSL stages |
| `RM` | Build-system remove command | Internal Makefile cleanup utility |
| `VENUSLANE`, `VENUSROW` | Physical vector geometry | 64 lanes, 512 rows for this profile |
| `VENUS_VRFADDR` | Vector register-file address base | Backend-defined, not a portable application constant |
| `VenusInputStructAddr` | Task input-descriptor base | Must agree with the chosen ABI |
| `TEMP_VRF_BASE_ADDR`, `TEMP_VRF_TOTAL_BYTES` | Temporary allocation region | Must agree with the backend memory layout |
| `TEMP_ALLOC_ALIGN_BYTES` | Temporary allocation alignment | Backend-defined alignment |

### Compiler and optimization flags

| Group | Key options | Purpose |
|---|---|---|
| Target | `--target=riscv32-unknown-elf`, `-march=...zvenus` | Select the custom Venus RISC-V target |
| Frontend | `-mllvm --venus`, `--venus-nr-row`, `--venus-nr-lane` | Supply vector geometry to the compiler |
| IR generation | `-S -emit-llvm -Xclang -disable-O0-optnone` | Produce IR for the custom pass sequence |
| Optimization | `-passes=mem2reg,venusplit` | Promote scalar storage and split Venus operations |
| ABI | `--venus-istruct-baseaddr` | Select input-descriptor placement |
| Code generation | `-O3`, `-regalloc=basic`, `--venus-vrf-baseaddr` | Target optimization and vector allocation |
| Inspection | `-d --mattr=+m,+zvenus -M no-aliases` | Disassemble custom instructions |

Use the exact generated command line from a successful run when debugging. The original checkout's `config.mk` contains defaults for several historical configurations, including different lane counts and address maps. Copying that file wholesale is not equivalent to selecting Venus1. Atomic-extension flags and additional passes can also differ between compiler/backend revisions.

## Generated artifacts and troubleshooting

| Artifact | What it is for |
|---|---|
| Generated DAG JSON | Task graph, variable bindings, input/output descriptors and scheduling data |
| Combined DAG BIN | Packaged graph data consumed by the runtime |
| Per-task IR / assembly / HEX | Inspect compilation and the exact generated task image |
| Run-local configuration and manifest | Reproduce the compiler, source snapshot and backend selection |
| Parameter metadata | Trace which external/static inputs were used |

The older direct DSL layout used `final_output/<dag>.json`, `bin/<dag>.bin` and `venus_test/ir/<task>.hex`. In the platform, use the paths recorded under the run artifacts; their exact nesting can differ from the original checkout.

| Symptom | What to inspect |
|---|---|
| Unknown builtin or vector type | Paired `venus.h`, generated typedefs and Venus-custom compiler |
| Parser error at a declaration | ASCII punctuation, keyword spelling, comments and final newline |
| Task input/output mismatch | C argument order, BAS bindings, return order and byte counts |
| Allocation or overlap warning | Live buffers, actual returned bytes and backend memory layout |
| Missing runtime input | `dag_input` / `dfedata` values and launcher input contract |
| Result has unexpected extra tail data | Declared capacity versus initialized bytes and `vreturn` length |

## External parameter files

To vary inputs without editing the task graph, place values in a separate file:

```basic
parameter short gain = {3}
```

For a target that actually references `gain`, compile with:

```bash
./ace-echo --config .ace-echo/host/local.toml compile dag \
  --target YOUR_DAG --params /path/to/case.params \
  --backend .ace-echo/host/backend.json
```

External values override matching static parameters; their type must agree. Explicit `--params` takes precedence over a `dag-source.json` default. Runtime inputs keep their `dag_input` / `dfedata` declarations, and their launcher must use the generated input contract. See [external parameter details](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/BAS_PARAMETER_INPUTS.md) for supported literal ranges, complete runtime-input requirements and initialization behavior.

The source references for this guide are the [lexer](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/toolchain/dsl/ply/basiclex.py), [parser](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/toolchain/dsl/ply/basparse.py), [build configuration](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/toolchain/dsl/config.mk) and [Venus1 backend](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/configs/backends/venus1p0-64x512-300mhz.json).
