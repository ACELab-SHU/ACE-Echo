---
title: Zoozve：支持任意寄存器分组、无需 strip-mining 的 RISC-V 向量扩展（WIP）
published: true
category: academic
permalink: zh_academic-3.html
summary: LCTES 2025：向量扩展与 LLVM 编译工具链研究。
tags:
- RISC-V
- vector processing
- LLVM
- hardware implementation
content_status: 历史项目新闻
section: News
lang: zh-CN
translation_url: academic-3.html
keywords: ACE-Echo, Venus, Zoozve：支持任意寄存器分组、无需 strip-mining 的 RISC-V 向量扩展（WIP）
sidebar: home_sidebar_zh
layout: post
date: 2025-06-13 00:00:00 +0800
---

在第 26 届 ACM SIGPLAN/SIGBED 嵌入式系统语言、编译器与工具会议（LCTES ’25）上，我们提出了 Zoozve：一种让长向量处理无需 strip-mining 的 RISC-V 向量扩展。Zoozve 支持任意向量寄存器分组，并采用数据自适应的寄存器分配策略，降低寄存器压力。结合定制 LLVM 编译工具链和 SystemVerilog 硬件实现，论文在 FFT 工作负载中报告动态指令数最多减少至原来的 1/344，面积开销为 5.2%。该研究探索了无线与 AI 领域灵活、高效的向量计算方向。

[阅读论文](https://dl.acm.org/doi/abs/10.1145/3735452.3735526)

{% include image.html file="news_2025_6_13.png" caption="所提出的编译工作流。" %}
