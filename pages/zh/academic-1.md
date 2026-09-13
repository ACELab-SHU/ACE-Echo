---
title: 面向开放 6G 基带信号处理的领域专用计算架构
published: true
category: academic
permalink: zh_academic-1.html
summary: TURC 2023：RISC-V 扩展与多层数据流编程。
tags: []
content_status: 历史项目新闻
section: News
lang: zh-CN
translation_url: academic-1.html
keywords: ACE-Echo, Venus, 面向开放 6G 基带信号处理的领域专用计算架构
sidebar: home_sidebar_zh
layout: post
date: 2023-09-25 00:00:00 +0800
---

在 ACM 图灵奖庆祝大会中国站 2023（TURC ’23）上，我们介绍了一种面向未来 6G 基带信号处理的领域专用众核架构。设计基于 RISC-V 指令扩展和多层数据流编程模型，在提供软件编程灵活性的同时实现适当的性能。通过复数乘法、并行 shuffle 等定制指令，加速关键的信号处理任务，探索通用处理器（GPP）、数字信号处理器（DSP）和 FPGA 在能效与灵活性方面的改进空间。

[阅读论文](https://dl.acm.org/doi/10.1145/3603165.3607377)

{% include image.html file="news_2023_9_25_fig1.png" caption="所提出领域专用架构（DSA）的整体结构。" %}

{% include image.html file="news_2023_9_25_fig2.png" caption="编程模型，包括多层数据流调度。" %}
