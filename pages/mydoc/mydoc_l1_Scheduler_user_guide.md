---
title: "L1 Scheduler User Guide"
layout: echo-doc
guide_reference: true
summary: "Understand DAG execution, Scheduler packaging and the 1.0 application-contract model."
sidebar: mydoc_sidebar
permalink: mydoc_l1_scheduler_user_guide.html
section: Documentation
content_status: "ACE-Echo 1.0"
last_reviewed: 2026-09-13
---

The Scheduler connects task execution with dependencies and data movement. ACE-Echo 1.0 distinguishes a Gem5 DAG run, a compiled Scheduler package and an application-contract run.

| Operation | Scope |
|---|---|
| `run dag` | Execute a supplied graph with the selected Gem5 Scheduler model |
| `scheduler build` | Build the platform's Scheduler sources into `l1.elf` / `l1.bin` |
| `run application --scheduler-engine contract` | Decode the firmware's application plan and execute it with the model |
| Full Scheduler firmware execution | The scalar CPU/peripheral backend is not implemented as a qualified 1.0 mode |

## Work from generated artifacts

Compile the DAG first and use the JSON, combined BIN and task-image paths recorded by that run. The example below shows the command shape; replace every `/path/to/` entry with your own generated artifact.

```bash
./ace-echo --config .ace-echo/host/local.toml scheduler build \
  --backend .ace-echo/host/backend.json \
  --dag-name my_dag --dag-json /path/to/dag.json \
  --dag-bin /path/to/dag.bin
```

A fully static DAG with no runtime inputs can use the backend's `--auto-static-main` option. Dynamic application inputs require an appropriate launcher and input contract.

```bash
./ace-echo --config .ace-echo/host/local.toml run application \
  --backend .ace-echo/host/backend.json \
  --l1-elf /path/to/l1.elf --scheduler-engine contract --mode fast
```

## What a successful run establishes

Read the run's coverage fields, output comparisons and timing boundaries. Application-contract completion is not execution of the complete scalar Scheduler firmware and does not establish a fresh RTL pass. Hardware and RTL evaluation need the matching separately supplied sources, toolchains and test inputs.

See [dynamic input handling](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/DYNAMIC_APPLICATION_INPUTS.md), [RTL evidence](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/RTL_EVIDENCE_AND_LIFECYCLE.md) and the [published validation scope](venus1_validation.html).

The old `dagInfoPaser.py` / `l1_main` instructions describe the [legacy integration path](https://github.com/ACELab-SHU/ACE-Echo/blob/8770c29996116dfd2bb791fdd5132a0949c294b3/pages/mydoc/mydoc_l1_Scheduler_user_guide.md).


## Preparing a DAG for L1

Keep the compiled JSON, combined BIN and per-task images from the **same compile run**. The JSON describes the input/output order and sizes; the binary and task images implement that contract. Recompiling a BAS file while retaining older scheduler metadata can create a mismatched package even when each component builds independently.

The platform's packaging step generates the C data symbols consumed by `fire_dag`. Do not handwrite the embedded DAG image or reorder generated input descriptors. For a graph with no `dag_input` / `dfedata`, `--auto-static-main` can supply the static launcher; otherwise select a main source that provides the runtime inputs.

## fire_dag API reference

The current Scheduler exposes the macro in `dagfire.h`:

```c
fire_dag(DAGname, DAGInputNum, DAGOutputNum,
         /* ordered input pointers, then ordered output pointers */);
```

This is a signature sketch; real calls need the matching generated symbols and pointer arguments.

| Argument | Meaning | How to select it |
|---|---|---|
| `DAGname` | Token prefix for generated graph symbols | Match the packaged DAG name; it is not a runtime string |
| `DAGInputNum` | Number of runtime input pointers | Match generated metadata |
| `DAGOutputNum` | Number of graph output pointers | Match generated metadata |
| Remaining input pointers | Buffers bound to runtime inputs | Follow the recorded input order |
| Remaining output pointers | Storage for returned graph outputs | Follow the recorded output order |

The macro registers the graph, transfers graph/task/parameter metadata and supplies runtime input/output bindings to the scheduler. A macro invocation is not a portable blocking C function call. Keep the completion/FIFO protocol from the matching launcher before consuming outputs or starting a dependent graph.

### Example with two runtime inputs

Suppose the graph declares:

```basic
dfedata char dfe_input[4388]
dag_input short nCellid[1]
return_value short subFrameNum[1]
```

If the **generated metadata** lists inputs as `nCellid`, then `dfe_input`, use that order even though the declarations above are reversed:

```c
#include "dagfire.h"
// Include the generated DAG symbols supplied by the packaging step.

static short nCellid[1] __attribute__((aligned(64))) = {2};
static char dfe_input[4388] __attribute__((aligned(64)));
// Example allocation: leave space for the task's aligned scalar return.
static unsigned char subFrameNum_storage[64] __attribute__((aligned(64)));

void launch_example(void) {
    // Populate dfe_input before launch.
    fire_dag(dag_name, 2, 1, nCellid, dfe_input, subFrameNum_storage);
    // Use the matching launcher's completion protocol before reading output.
}
```

This is an integration fragment: `dag_name` and its generated symbols stand for your packaged graph. The old example listed `nCellid` first in metadata but passed `dfe_input` first in the call. The example above makes the order consistent. Allocate output storage for the actual byte count returned by the producer, including ABI padding when relevant; a BAS `short[1]` declaration does not by itself prove the runtime writes only two bytes.

## Connecting one DAG to another

1. Compile and package each graph with the same selected backend.
2. Bind inputs and output buffers according to each graph's metadata.
3. Launch the producer graph through the matching runtime protocol.
4. Wait for its documented completion signal before reusing its output as another graph's input.
5. Preserve the buffer until the consumer has finished; verify type, byte count and lifetime across the boundary.

The application-contract model decodes the supported plan and bindings. It does not emulate arbitrary C control flow in an L1 main. A new launcher that builds successfully may still use behavior the contract decoder does not support; inspect the application coverage report.

## Legacy dagInfoPaser workflow

For users maintaining the original `l1_main` layout, the old sequence remains:

```bash
cd /path/to/legacy-project/l1_main
python3 ./dagInfoPaser.py YOUR_DAG
```

Read `daginfo/YOUR_DAG.txt` for input/output ordering, then update the matching `source/l1.cpp` and its buffer declarations. The legacy example used `l1_api.h`; the current bundled Scheduler uses `dagfire.h` and generated packaging data. Follow one complete version's layout and headers when reproducing an older application.

The old direct paths were `dsl/bin/<dag>.bin`, `dsl/final_output/<dag>.json` and `dsl/venus_test/ir/<task>.hex`. The current platform collects these under run artifacts and builds its own isolated Scheduler source copy.

## Diagnosing integration problems

| Symptom | Check |
|---|---|
| Generated symbol not found | DAG token prefix and generated header/image selection |
| Input data swapped | Metadata order versus `fire_dag` pointer order |
| Output buffer overwritten | Runtime returned byte count, padding and allocation lifetime |
| Graph launches but application output is stale | Completion handling and dependent-graph sequencing |
| Scheduler build succeeds but contract run fails | Supported plan format, runtime inputs and coverage report |
| Different results after a recompile | JSON, BIN, task images and L1 package must come from one matching build |

Read the published [fire_dag macro](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/scheduler/include/dagfire.h) and [dynamic input contract](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/DYNAMIC_APPLICATION_INPUTS.md) alongside the generated artifacts when adding an application.
