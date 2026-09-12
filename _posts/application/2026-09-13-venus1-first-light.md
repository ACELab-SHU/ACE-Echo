---
layout: echo-doc
title: "Our first-generation chip has taped out and achieved first light"
summary: "The 5G-Lite project brings Venus to silicon, with successful bring-up and real-signal NR cell search."
section: News
date: 2026-09-13 00:00:00 +0800
published: true
category: application
permalink: venus1-first-light.html
tags: [news]
---

<p class="document-date">Published 13 September 2026 · Silicon milestone</p>

Our first-generation chip has successfully completed tape-out and achieved first light. The 5G-Lite project has progressed from software and RTL through physical implementation, fabrication, packaging and evaluation-board bring-up. Venus now runs on real silicon.

<figure class="news-photo"><img src="images/venus1-silicon.jpeg" width="1279" height="1706" alt="The first-generation DT-VENUS-A1 AI-RAN SoC after packaging"><figcaption>First-generation Venus silicon. Project photograph from the 5G-Lite completion report.</figcaption></figure>

## A chip that runs

During bring-up, the chip executed the target program and continuously produced the UART message:

<blockquote class="news-quote">DT AI-RAN!</blockquote>

This established that the basic clock, reset, code execution and serial-output paths were working. The project then demonstrated real over-the-air signal processing and NR cell search. Its reported cell identifier matched the PCI observed by a commercial-network reference tool.

These milestones were documented in the team's **2 September 2026 project completion review**. That is the review date, rather than a claimed date for tape-out or first light.

## Echo and Venus come together

Echo began with real communication workflows. Venus developed the programmable processing architecture and its toolchain. The 5G-Lite project brought these efforts together in a communication-oriented SoC.

Configurable DFE hardware handles front-end communication primitives, while the Venus processor and Scheduler support programmable algorithms and task execution. This first silicon milestone gives our communication–AI architecture research a physical foundation and provides feedback for the next generation.

The demonstrated milestone is bring-up and NR cell search. Full end-to-end throughput, long-duration stability and complete PBCH/MIB/SIB1 qualification remain follow-on work.

## Opening application development

We are making the development platform available through **ACE-Echo 1.0**: a Gem5-based simulator for developing Venus applications and quickly estimating performance, together with an initial Agent-assisted development framework.

Our **ACE-Echo 2.0 roadmap** extends that foundation toward intent-driven development. The goal is for a user to describe an application and for an Agent to generate an implementation deployable on Venus.

[Explore the project](index.html) · [Develop with ACE-Echo 1.0](venus1_get_started.html) · [All news](news.html)
