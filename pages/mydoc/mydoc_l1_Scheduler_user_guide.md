---
title: "Scheduler and application execution"
layout: echo-doc
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
