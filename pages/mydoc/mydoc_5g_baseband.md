---
title: 5G / NR on Venus
layout: echo-doc
summary: Connect the NR processing flow with the hardware milestone and current simulation cases.
sidebar: mydoc_sidebar
permalink: mydoc_5g_baseband.html
section: Applications
content_status: Application scope
last_reviewed: '2026-09-13'
lang: en
translation_url: zh_mydoc_5g_baseband.html
---

5G NR is a central application direction for the Venus communication–AI architecture. This page separates the processing flow, the original hardware demonstrations and the cases measured in ACE-Echo 1.0.

## From synchronization to system information

<ol class="process-cards"><li><span>01</span><h3>Find the cell</h3><p>Detect synchronization signals and determine the physical cell identity. PSS and SSS contribute to the NR cell-identity calculation.</p></li><li><span>02</span><h3>Decode the broadcast</h3><p>Process PBCH and its reference signals to recover the broadcast payload and MIB information.</p></li><li><span>03</span><h3>Acquire system information</h3><p>Use control-channel scheduling to locate the shared-channel transmission carrying system information such as SIB1.</p></li></ol>

This is a conceptual flow, not a fixed latency guarantee. Synchronization accuracy, acquisition time and decoding success depend on the waveform, channel, configuration and implementation. See [3GPP TS 38.211, release 16](https://www.etsi.org/deliver/etsi_ts/138200_138299/138211/16.04.00_60/ts_138211v160400p.pdf) for physical signals and [TS 38.331](https://www.etsi.org/deliver/etsi_ts/138300_138399/138331/15.24.01_60/ts_138331v152401p.pdf) for broadcast/system-information definitions.

## What the project has demonstrated

| Evidence | What it describes |
|---|---|
| [January 2025 FPGA demonstration](application-1.html) | A historical UVP PBCH/MIB experiment on an evaluation board |
| [First-generation silicon](venus1-first-light.html) | Chip bring-up and real-signal NR cell search, recorded in the 2026 project review |
| [ACE-Echo 1.0 regression](venus1_validation.html) | `nrPBCH` and `nrPDCCH` among eight software/RTL timing-comparison DAGs |

The first-silicon milestone does not certify complete PBCH/MIB/SIB1 operation, long-duration stability or end-to-end throughput. Likewise, matching known simulator/RTL output bits does not establish an independent algorithm reference.

## Develop an NR application

Begin with the [small installation smoke](venus1_get_started.html), then inspect the paired radio workload sources and their required inputs. Fix the input matrix and expected outputs before comparing implementations. Use the [programming model](mydoc_programming_model.html) to split work into tasks and the [validation report](venus1_validation.html) to interpret timing results.
