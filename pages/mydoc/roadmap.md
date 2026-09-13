---
title: ACE-Echo roadmap
layout: echo-doc
toc: false
summary: From practical Venus application development to intent-driven generation.
sidebar: mydoc_sidebar
permalink: roadmap.html
section: Project
content_status: Current direction
last_reviewed: '2026-09-13'
lang: en
translation_url: zh_roadmap.html
---

Our direction is to make communication–AI hardware easier to program. The first step is a useful simulator and a guided development workflow; the next is a more complete Agent-assisted path from intent to an application for Venus.

{% include echo-roadmap.html %}

## What changes between releases

| | ACE-Echo 1.0 | ACE-Echo 2.0 direction |
|---|---|---|
| Starting point | A developer-defined task, DAG and reference | A user-defined application intent |
| Development | Venus C/BAS with initial Agent assistance | Agent-led generation and iterative improvement |
| Evaluation | Gem5 output checks and approximate timing | Validation integrated into the generation loop |
| Result | An implementation the developer reviews and evaluates | An application intended for deployment on Venus |

The 2.0 column describes planned behavior. Its release date and final supported application scope have not been announced here.

## Software versions and hardware generations

**ACE-Echo 1.0 / 2.0** name platform releases. **Venus 1.0 / 2.0** name hardware backends. A Venus2 backend in the source tree does not mean the ACE-Echo 2.0 Agent workflow has shipped. The 1.0 website's reported regression targets Venus1.

## Previous plans

Explore the platform's early development in the [v0.1 release record](mydoc_beta_release_notes.html). The roadmap above describes the current direction.
