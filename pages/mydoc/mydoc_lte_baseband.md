---
title: "LTE on Venus"
layout: echo-doc
toc: false
summary: "Available LTE DAG cases and the scope of their 1.0 evaluation."
sidebar: mydoc_sidebar
permalink: mydoc_lte_baseband.html
section: Applications
content_status: "Application scope"
last_reviewed: 2026-09-13
---

LTE workloads are part of the Venus development and regression set. The old “available within two months” placeholder has been replaced with the recorded release scope.

## DAGs in the published regression

| Processing block | DAG cases |
|---|---|
| Broadcast channel | `ltePBCHDag1_hw`, `ltePBCHDag2_hw` |
| Control format | `ltePCFICH` |
| Control channel | `ltePDCCHDag1_hw`, `ltePDCCHDag2_hw` |
| Shared channel | `ltePDSCH` |

These six DAGs completed the September 2026 Gem5 fast / RTL timing regression. The measurement applies to the specified inputs and timing boundaries. It does not certify a complete over-the-air LTE cell-search application or every protocol configuration.

## Interpret results carefully

Use the [validation report](venus1_validation.html) for per-DAG timing error, unknown RTL output bits and the separate PDSCH CRC question. A completed run, simulator agreement and algorithm correctness are different results.

## Start developing

Install the paired source and tools using the [1.0 guide](venus1_get_started.html). Select the Venus1 backend consistently, record the waveform and parameters, and compare every required output against the reference for that exact case.
