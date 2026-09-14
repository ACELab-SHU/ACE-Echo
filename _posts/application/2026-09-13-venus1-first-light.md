---
layout: echo-doc
title: Our first-generation chip has taped out and achieved first light
summary: The 5G-Lite project brings Venus to silicon, with successful bring-up and real-signal NR cell search.
section: News
hide_sidebar: true
date: 2026-09-13 00:00:00 +0800
published: true
category: application
permalink: venus1-first-light.html
tags:
- news
lang: en
translation_url: zh_venus1-first-light.html
---

<p class="document-date">Published 13 September 2026 · Silicon milestone</p>

**Our first-generation Venus chip has successfully taped out and achieved first light.** Developed through the 5G-Lite project, the chip has completed packaging and evaluation-board bring-up, and successfully demonstrated NR cell search using real over-the-air signals. Its detected physical cell identity matched the commercial-network reference measurement.

This milestone brings together years of work on Echo's communication software and the Venus programmable architecture—from algorithms and RTL to a working chip.

<figure class="news-photo"><img src="images/venus1-silicon.jpeg" width="1279" height="1706" alt="Packaged DT-VENUS-A1 AI-RAN SoC"><figcaption>The first-generation DT-VENUS-A1 AI-RAN SoC.</figcaption></figure>

## From design to packaged silicon

The team completed physical implementation, fabrication, wire bonding and packaging, then brought the chip up on its evaluation board. The resulting system runs application code and processes incoming radio signals.

| Chip and package | Details |
| --- | --- |
| Device | DT-VENUS-A1 AI-RAN SoC |
| Package | HLQFP128B |
| Package body | 14 × 14 mm |
| Pin count | 128 |
| Processing architecture | Configurable DFE, programmable Venus processor and task Scheduler |
| Clock and power organization | 3 PLLs and 4 power domains |

<div style="display:grid;grid-template-columns:repeat(auto-fit,minmax(min(100%,220px),1fr));gap:20px;align-items:start">
<figure class="doc-figure"><a class="figure-link" href="images/venus1-wire-bonding.webp"><img src="images/venus1-wire-bonding.webp" width="900" height="1600" loading="lazy" style="width:100%;height:300px;object-fit:contain" alt="Venus dies and their wire bonds before encapsulation"></a><figcaption>Die assembly and wire bonding.</figcaption></figure>
<figure class="doc-figure"><a class="figure-link" href="images/venus1-packaged-devices.webp"><img src="images/venus1-packaged-devices.webp" width="1600" height="900" loading="lazy" style="width:100%;height:300px;object-fit:contain" alt="A tray of packaged first-generation Venus devices"></a><figcaption>Packaged devices ready for board assembly and testing.</figcaption></figure>
<figure class="doc-figure"><a class="figure-link" href="images/venus1-evaluation-board.webp"><img src="images/venus1-evaluation-board.webp" width="900" height="1600" loading="lazy" style="width:100%;height:300px;object-fit:contain" alt="Venus evaluation board used for chip bring-up and testing"></a><figcaption>The evaluation board used for bring-up and silicon testing.</figcaption></figure>
</div>

## Real signals. A matching cell identity.
{: #a-chip-that-runs }

The silicon tests exercised the path from real radio input through DFE synchronization and Venus processing to cell-search results. The chip reported **PCI 800** and **SSB index 3**. A Cellular-Pro measurement of the commercial NR network independently showed **PCI 800**, confirming that the detected physical cell identity was correct.

| Field | Chip output | Network reference |
| --- | --- | --- |
| Physical cell identity (PCI) | 800 | 800 |
| SSB index | 3 | — |
| Network context | — | NR-SA · Band n1 · NR-ARFCN 426030 |

<figure class="doc-figure" style="max-width:420px"><a class="figure-link" href="images/venus1-nr-cell-search.png"><img src="images/venus1-nr-cell-search.png" width="612" height="912" loading="lazy" alt="Chip output showing cellid 800 and ssbIdx 3"></a><figcaption>Cell-search output captured during silicon testing. The cellid field reports the physical cell identity, PCI.</figcaption></figure>

Successful bring-up and this over-the-air result validate the implemented NR cell-search path on silicon, connecting the programmable architecture to a real communication workload.

## Low-power operation. Built for communication.
{: #measured-power-on-the-evaluation-board }

Venus combines configurable communication primitives with programmable processing and has demonstrated NR cell search on silicon. With its Cluster fully loaded, the evaluation board consumes **0.95 W excluding the DFE**, providing a low-power computing foundation for radio monitoring, synchronization and edge communication applications.

<section class="energy-showcase" aria-label="ENERGY COMPARISON · NORMALIZED TO 28 nm">
<div class="energy-power"><div><span class="energy-eyebrow">VENUS</span><p>Full Cluster load · Entire evaluation board, excluding DFE</p></div><strong class="energy-power-value">0.95 <small>W</small></strong></div>
<p class="energy-comparison-label">ENERGY COMPARISON · NORMALIZED TO 28 nm</p>
<div class="energy-duel">
<div class="energy-contender"><p class="energy-versus">VENUS VS ARM</p><h3 class="energy-opponent">NEON</h3><p class="energy-metric-label">ENERGY EFFICIENCY</p><strong class="energy-value"><small>≈</small>7.7<span>×</span><span class="energy-up" aria-hidden="true"><svg viewBox="0 0 32 40" focusable="false"><path d="M16 34V7M5 18 16 7 27 18" /></svg></span></strong><p class="energy-result">28 nm normalized calculation · Reference / Venus</p><p class="energy-platform">RK3588 · Cortex-A76</p></div>
<div class="energy-contender"><p class="energy-versus">VENUS VS INTEL</p><h3 class="energy-opponent">AVX-512</h3><p class="energy-metric-label">ENERGY EFFICIENCY</p><strong class="energy-value"><small>≈</small>25.6<span>×</span><span class="energy-up" aria-hidden="true"><svg viewBox="0 0 32 40" focusable="false"><path d="M16 34V7M5 18 16 7 27 18" /></svg></span></strong><p class="energy-result">28 nm normalized calculation · Reference / Venus</p><p class="energy-platform">Xeon 6434</p></div>
</div>
</section>

**Measurement scope: the entire development board for RK3588, CPU package power for Xeon, and the entire evaluation board with the Cluster fully loaded and DFE excluded for Venus.** Both ratios are calculated from the single-core comparison data below, normalized to 28 nm.

<details class="doc-figure"><summary>Measurement scope and energy calculation</summary>
<p><strong>Power measurement.</strong>The ARM Cortex-A76 reference runs on RK3588, with power measured across the entire development board. Xeon 6434 uses CPU package power. Venus uses 0.95 W for its entire evaluation board with the Cluster fully loaded, excluding the DFE. In the single-core comparison, CPU idle power is apportioned by physical core count.</p>
<p><strong>Workloads and data.</strong>The CPU reference data comes from LDPC workloads: the RK3588 Cortex-A76 baseline uses NEON, and the Xeon 6434 baseline uses AVX-512. Venus uses an SSB processing workload. The Venus calculation is 0.95 W / 8.448 Mbps, yielding approximately 112.45 nJ/bit. The comparison results apply to these workloads and measurement scopes.</p>
<p><strong>Process normalization.</strong>Energy is scaled using E₂₈ = E × (28 / L)², with process-node values of 8 nm for RK3588, 7 nm for Xeon 6434 and 40 nm for Venus. The normalized reference-to-Venus energy ratios are (34.58 × 12.25) / (112.45 × 0.49) ≈ 7.7 and (88.33 × 16) / (112.45 × 0.49) ≈ 25.6.</p>
</details>

### Bring your communication workload to Venus

Start with a cell-search or signal-processing example, then adapt its algorithm and dataflow to your application. **ACE-Echo 1.0 lets you compile and simulate Venus applications and quickly explore approximate cycle-level performance before deploying to hardware.**

[Start developing with ACE-Echo 1.0 →](venus1_get_started.html) · [Explore the NR cell-search example →](mydoc_5g_baseband.html)

## Echo and Venus come together

Configurable DFE hardware handles front-end communication primitives, while the Venus processor and Scheduler execute programmable algorithms and coordinate tasks. The 5G-Lite silicon milestone gives our communication–AI architecture research a hardware foundation for developing and testing applications.

## Opening application development

**ACE-Echo 1.0** opens this development path with a Gem5-based simulator for Venus applications, approximate cycle-level performance evaluation and an initial Agent-assisted development framework.

Our **ACE-Echo 2.0 roadmap** builds toward intent-driven development: describe an application, and an Agent helps generate an implementation deployable on Venus.

Thank you to every project member and partner who contributed to the design, verification, fabrication and silicon testing of this first chip.

[Explore the project](index.html) · [Develop with ACE-Echo 1.0](venus1_get_started.html) · [All news](news.html)
