---
title: What you can build with 1.0
layout: echo-doc
summary: Venus application development, Gem5 simulation and initial Agent assistance.
sidebar: mydoc_sidebar
permalink: mydoc_supported_features.html
section: Documentation
content_status: ACE-Echo 1.0
last_reviewed: '2026-09-13'
lang: en
translation_url: zh_mydoc_supported_features.html
---

ACE-Echo 1.0 is the development platform for Venus applications. Its current scope is defined by the release source, selected hardware backend and recorded validation cases.

{% include echo-workflow.html %}

## Current capabilities

| Capability | Included in the 1.0 workflow |
|---|---|
| Application authoring | Venus C tasks and BAS descriptions of task dependencies |
| Compilation | Bundled DSL frontend connected to a separately installed Venus-custom LLVM toolchain |
| Simulation | Gem5 task, DAG and application-contract execution |
| Performance exploration | Cycle-level modelling and approximate timing on the selected backend |
| Output checking | Explicit comparison against a supplied reference; input and output identities recorded per run |
| Agent assistance | Forge requests and repository skills used by an external AI coding host |
| Reproducibility | Per-run commands, logs, configuration identities and generated artifacts |

## Application coverage

The published Venus1 regression covers `nrPBCH`, `nrPDCCH`, `ltePBCHDag1_hw`, `ltePBCHDag2_hw`, `ltePCFICH`, `ltePDCCHDag1_hw`, `ltePDCCHDag2_hw` and `ltePDSCH`. The [validation report](venus1_validation.html) explains timing agreement and remaining output-correctness limits.

The bundled two-task vector smoke is the starting point for installation checks. Broader 5G/LTE workflows and communication–AI research belong to the project ecosystem; they do not imply that every operator, input combination or end-to-end application is qualified by this release.

[5G/NR](mydoc_5g_baseband.html) · [LTE](mydoc_lte_baseband.html) · [GNSS](mydoc_gnss_baseband.html) · [LoRa](mydoc_lora_baseband.html)

## Scope boundaries

The release does not provide an automatic BAS-to-RTL exporter, a general CPU/GPU execution fallback, a visual DAG editor, or automatic ASIC power/resource reports.

The L1 application-contract engine models a Scheduler execution plan; it does not execute the complete scalar Scheduler firmware. RTL runs need separately supplied hardware sources and tools. Full intent-to-deployable-application generation belongs to the [2.0 roadmap](roadmap.html).

For exact command scope, see [the pinned platform README](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/README.md) and [coverage contract](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/COVERAGE.md).
