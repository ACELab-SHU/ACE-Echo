---
title: Performance and evidence
layout: echo-doc
summary: Separate 1.0 simulation accuracy from earlier architecture benchmarks.
sidebar: mydoc_sidebar
permalink: mydoc_performance.html
section: Evaluation
content_status: Measured scope + research archive
last_reviewed: '2026-09-13'
lang: en
translation_url: zh_mydoc_performance.html
---

ACE-Echo 1.0 helps developers estimate the performance of Venus applications. Its cycle-level model is evaluated against RTL for specific cases. That simulator comparison is separate from comparisons between hardware architectures.

## ACE-Echo 1.0 timing measurements

The recorded eight-DAG regression used nominal 300 MHz Tile and 150 MHz AXI clocks. The largest absolute DAG-interval error was **0.4633%**; the largest individual-task error was **2.2834%**. See the [complete results and interval definitions](venus1_validation.html).

These measurements are tied to the listed input matrix and model. A small timing error does not establish all-output algorithm correctness, universal cycle-for-cycle equivalence or measured chip energy consumption.

## Earlier architecture research

The figures below are retained from the original project website. Their data and visual encodings are unchanged. They describe earlier experimental configurations and should be read with the corresponding research source, rather than interpreted as new 1.0 measurements.

### Tile-level comparison

{% include image.html file="performance2.png" caption="Historical tile-level comparison with AVX, Neon and TI DSP. Original workload choices, compiler settings and lane configurations apply." %}

### Earlier hardware platforms

{% include image.html file="performance3.png" caption="Historical table comparing earlier implementations. This is not a same-process, same-clock measurement of the first-generation silicon." %}

### Vector architecture experiments

{% include image.html file="performance4.png" caption="Historical UVP/Ara kernel comparison. The original figure pairs L-lane Ara with 4L-lane UVP; lane labels are not interchangeable." %}

### PBCH latency breakdown

{% include image.html file="performance5.png" caption="Historical BCH procedure breakdown at 50 MHz, Lane32Reg1024. The 1.0 regression uses a different clock and hardware profile." %}

## Read the original context

The [academic archive](academic_achievements.html) links the project's architecture publications. The [original performance page](https://github.com/ACELab-SHU/ACE-Echo/blob/8770c29996116dfd2bb791fdd5132a0949c294b3/pages/mydoc/mydoc_Performance.md) preserves its full experimental descriptions. Earlier broad claims about being faster or more energy-efficient than all CPU/GPU solutions are not carried forward as release guarantees.
