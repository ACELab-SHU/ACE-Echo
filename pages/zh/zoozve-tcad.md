---
layout: "echo-doc"
title: "Zoozve：从编译器到硬件的灵活向量寄存器"
summary: "任意寄存器分组连接 RISC-V 向量扩展、LLVM 后端与硬件实现。"
section: "News"
date: "2026-09-01 00:00:00 +0800"
published: true
category: "academic"
permalink: "zh_zoozve-tcad.html"
lang: "zh-CN"
translation_url: "zoozve-tcad.html"
hide_sidebar: true
toc: false
tags: ["news", "Venus"]
---

<p class="document-date">2026 年 9 月 · 期刊卷期 · IEEE TCAD</p>

Zoozve 通过任意寄存器分组处理长向量，减少反复进行软件 strip-mining 的需求。这项工作将指令集设计、LLVM 编译与硬件原型结合起来，协同研究寄存器分配与向量数据搬移。

{% include image.html file="news/zoozve-tcad.svg" caption="研究主题示意：Zoozve：从编译器到硬件的灵活向量寄存器" %}

## 论文信息

**An Arbitrary Register Grouping Scheme for RISC-V Vector Extension: Compilation Support and Hardware Implementation**

Limin Jiang, Siyi Xu, Yintao Liu, Yihao Shen, Yi Shi, Shan Cao, Zhiyuan Jiang

IEEE Transactions on Computer-Aided Design of Integrated Circuits and Systems, 45(9), 4450-4463

这项成果展示了 Venus 相关研究的进展。平台支持的应用和验证结果见 [ACE-Echo 1.0 验证范围](zh_venus1_validation.html)。

[阅读论文](https://doi.org/10.1109/tcad.2025.3649448) · [实验室发表记录](https://acelab-shu.github.io/publication/jiang-tcad-2025/) · [全部新闻](zh_news.html)
