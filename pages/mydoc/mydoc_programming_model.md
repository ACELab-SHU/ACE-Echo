---
title: The Venus programming model
layout: echo-doc
summary: Define a task graph, compile for one backend, and evaluate the resulting application.
sidebar: mydoc_sidebar
permalink: mydoc_programming_model.html
section: Documentation
content_status: ACE-Echo 1.0
last_reviewed: '2026-09-13'
lang: en
translation_url: zh_mydoc_programming_model.html
---

In ACE-Echo, a **task** contains computation written in Venus C. A **DAG** connects tasks through their inputs and returns. The selected **backend** keeps compiler geometry, memory conventions and simulation settings consistent.

{% include echo-workflow.html %}

## Start with a working example

After [installing the release](venus1_get_started.html), inspect `workloads/5g_lite/tasks/forge_vector_smoke/`. It contains two C tasks, one BAS graph and a small host reference. The first task adds two signed vectors; the second uses the upstream result to restore the first input. The smoke checks the complete outputs for its declared cases.

```text
forge_vector_smoke/
├── Task_forgeAdd.c
├── Task_forgeRestore.c
├── forge_vector_smoke.bas
├── reference.py
└── run.py
```

## Separate the contracts

| Contract | What to decide |
|---|---|
| Numerical behavior | Element type, signedness, overflow, rounding and expected outputs |
| Task interface | Input order, returned values and actual returned byte counts |
| Graph | Dependencies and the values exposed as application outputs |
| Hardware | A single explicit backend for compilation and simulation |
| Evidence | The input cases, reference, output coverage and timing interval |

The [Venus guide](mydoc_venus_user_guide.html) introduces task code. The [DSL guide](mydoc_dsl_user_guide.html) describes BAS and compilation. The [Scheduler guide](mydoc_l1_scheduler_user_guide.html) explains application-contract execution.

## Run and inspect

```bash
make smoke
```

The installation smoke writes a new run directory. General CLI operations record commands, logs and artifact identities under `runs/`. Gem5 outputs and task/DAG traces are collected when the selected operation produces them. Use the generated run manifest to locate artifacts instead of assuming the old `Debug/emular_vins_result/` directory.

Fast mode is the normal development route. It reduces expensive observations while retaining the timing model; simulator agreement and independent algorithm correctness remain separate checks.

## Develop with an Agent

Forge initializes and records a request. An AI coding host reads the repository skill and drives compile, run and compare operations with developer guidance. See [the Forge onboarding instructions](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/GETTING_STARTED.md).

Earlier task-folder screenshots and VEMU instructions are retained in the [original programming guide source](https://github.com/ACELab-SHU/ACE-Echo/blob/8770c29996116dfd2bb791fdd5132a0949c294b3/pages/mydoc/mydoc_programming_model.md). They describe the legacy workflow.
