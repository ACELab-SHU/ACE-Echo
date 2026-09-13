---
title: Meet ACE-Echo
layout: echo-doc
toc: false
summary: The open development platform for our Venus communication–AI processor.
sidebar: mydoc_sidebar
permalink: mydoc_introduction.html
section: Project
content_status: ACE-Echo 1.0
last_reviewed: '2026-09-13'
lang: en
translation_url: zh_mydoc_introduction.html
---

ACE-Echo connects our communication–AI chip research with application development. **Venus** is the programmable RISC-V processor. **AURA** is the architecture and programming foundation. **ACE-Echo** is the open platform used to develop applications for that hardware.

## Architecture, silicon and software

AURA brings together Venus Language, the Zoozve compiler and mathematical libraries. Its dataflow approach connects task-level computation with the movement of data between tasks.

{% include image.html file="aura-architecture.png" caption="AURA groups the programming language, compiler and mathematical libraries. These are components, not sequential pipeline stages." %}

Venus combines programmable Tiles, a Scheduler and a digital front end. Our research explores communication processing and AI workloads on this shared hardware foundation.

{% include image.html file="venus-architecture.png" caption="Venus hardware components: Tile, Scheduler and digital front end." %}

Our first-generation chip has completed tape-out and achieved first light. Bring-up and real-signal NR cell search are documented in the [silicon announcement](venus1-first-light.html). That milestone is distinct from the validation scope of the open simulator.

## Build applications with 1.0

ACE-Echo 1.0 provides a usable Gem5-based simulator and an initial Agent-assisted framework. Developers can write Venus applications, check outputs and quickly estimate their approximate performance. Custom Venus LLVM and other host tools remain separate prerequisites.

{% include echo-workflow.html %}

Start with the [installation guide](venus1_get_started.html), then explore the [programming model](mydoc_programming_model.html) and [supported scope](mydoc_supported_features.html).

## From assistance to intent

{% include echo-roadmap.html %}

The [roadmap](roadmap.html) replaces earlier beta dates and feature forecasts. ACE-Echo software versions and Venus hardware generations are separate version names.

## Built for collaboration

Researchers can explore hardware-aware algorithms; application developers can compare implementations before hardware evaluation. Contributions to examples, documentation and reproducible experiments help extend the shared platform.

[Meet the team](mydoc_developers.html) · [Contributing organizations](mydoc_contributing_organizations.html) · [Contact ACE Lab](mailto:shenyihao@shu.edu.cn)
