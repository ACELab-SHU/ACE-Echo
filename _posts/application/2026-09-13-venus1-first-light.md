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

## Measured power on the evaluation board

Power measurements covered nine operating scenarios, from the always-on baseline to the SSB decode-loop workload. **The evaluation board and Venus SoC together consumed approximately 1.06 W while running the SSB decode loop.**

| Test scenario | Measured board + SoC power |
| --- | --- |
| Always-on baseline | 0.2877 W |
| DFE computation scenario | 0.7683 W |
| Cluster computation scenario | 0.8923 W |
| SSB decode loop | 1.0597 W |

These are measured system power values for the stated test configurations, including the evaluation board's power-supply path.

<figure class="doc-figure"><a class="figure-link" href="images/venus1-power-measurements.png"><img src="images/venus1-power-measurements.png" width="1684" height="353" loading="lazy" alt="Original nine-scenario power measurement matrix for the development board and Venus SoC"></a><figcaption>Power measurements and domain configurations across nine test scenarios. Select the image to view the full-resolution record.</figcaption></figure>

## Echo and Venus come together

Configurable DFE hardware handles front-end communication primitives, while the Venus processor and Scheduler execute programmable algorithms and coordinate tasks. The 5G-Lite silicon milestone gives our communication–AI architecture research a hardware foundation for developing and testing applications.

## Opening application development

**ACE-Echo 1.0** opens this development path with a Gem5-based simulator for Venus applications, approximate cycle-level performance evaluation and an initial Agent-assisted development framework.

Our **ACE-Echo 2.0 roadmap** builds toward intent-driven development: describe an application, and an Agent helps generate an implementation deployable on Venus.

Thank you to every project member and partner who contributed to the design, verification, fabrication and silicon testing of this first chip.

[Explore the project](index.html) · [Develop with ACE-Echo 1.0](venus1_get_started.html) · [All news](news.html)
