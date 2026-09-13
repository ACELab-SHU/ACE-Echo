---
title: 面向无线基带处理的分层数据流驱动异构架构
published: true
category: academic
permalink: zh_academic-2.html
summary: ASPDAC 2025：无缓存 NUMA 架构与 pack-and-ship 数据分发。
tags:
- Wireless baseband processing
- NUMA
- dataflow-driven
content_status: 历史项目新闻
section: News
lang: zh-CN
translation_url: academic-2.html
keywords: ACE-Echo, Venus, 面向无线基带处理的分层数据流驱动异构架构
sidebar: home_sidebar_zh
layout: post
date: 2025-03-04 00:00:00 +0800
---

在第 30 届亚太设计自动化会议（ASPDAC ’25）上，我们提出了面向无线基带处理（WBP）周期性和模块化特点的无缓存、基于 NUMA 的异构架构。通过“pack-and-ship”数据分发策略和多层数据流调度，改善数据局部性并降低访存时延。论文实验报告了最高 2.3× 的单 Tile 加速和 288 Mbps 的链路级吞吐率，展示了相对所选 GPU、DSP 基线的性能与可扩展性。上述数字适用于原论文实验配置。

[阅读论文](https://dl.acm.org/doi/abs/10.1145/3658617.3697558)

{% include image.html file="news_2025_3_4_fig1.png" caption="所提出设计的总体结构。" %}

{% include image.html file="news_2025_3_4_fig2.png" caption="Tile 级调度方案。" %}
