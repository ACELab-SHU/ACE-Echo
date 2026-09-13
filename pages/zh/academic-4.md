---
title: Venus：面向 6G 边缘智能、融合 AI 与无线基带处理的 RISC-V 领域专用架构
published: true
category: academic
permalink: zh_academic-4.html
summary: Venus 架构研究发表于 IEEE Wireless Communications。
tags: []
content_status: 历史项目新闻
section: News
lang: zh-CN
translation_url: academic-4.html
keywords: ACE-Echo, Venus, Venus：面向 6G 边缘智能、融合 AI 与无线基带处理的 RISC-V 领域专用架构
sidebar: home_sidebar_zh
layout: post
date: 2025-07-09 00:00:00 +0800
---

团队关于 Venus 架构的研究论文已被 **IEEE Wireless Communications Magazine** 接收。Venus 是基于 RISC-V 的领域专用架构（DSA），面向 6G 边缘智能，探索人工智能与无线基带处理的高效融合。

## 6G 边缘计算的挑战与 Venus 的设计思路
{: #pain-points-in-6g-edge-computing-and-venus-breakthrough-solutions }

6G 局域网要求边缘设备在实时 AI 推理的同时高效处理复杂无线基带信号。现有架构在效率与灵活性之间存在权衡：通用处理器面临能效限制，专用芯片编程灵活性不足，GPU 也可能因无线信号处理的数据依赖而难以充分利用资源。

Venus 利用开放 RISC-V 生态与领域专用优化，探索可编程性和效率的平衡。智能工厂等 6G 场景（图 1）展示了这一架构的目标应用。

{% include image.html file="news_2025_7_9_fig1.png" caption="图 1：智能工厂中基于 6G 局域网的边缘 AI 与不同计算架构。" max-width="450" %}

## Venus 架构的核心设计
{: #core-design-and-advantages-of-venus-architecture }

硬件采用多层数据流驱动的众核结构（图 2），包含主调度器、集群和 Tile 三级调度，使用非统一内存访问（NUMA）促进局部处理，减少数据搬运能耗。每个 Tile 配备向量处理 lane，支持灵活的寄存器重组与动态向量长度调整。

{% include image.html file="news_2025_7_9_fig2.png" caption="图 2：Venus 硬件架构。" %}

软件侧提供基于 LLVM 的工具链，并通过领域专用语言（DSL）简化任务依赖定义和硬件资源映射。论文以表 1 所列架构进行面积、功耗和能效比较，展示 16-lane Tile 在所选 5G 物理层任务中的表现，并讨论向 6G 大带宽场景扩展的潜力。具体结论应结合原论文配置阅读。

{% include image.html file="news_2025_7_9_fig3.png" caption="表 1：原论文中的架构比较。" %}

[阅读 IEEE Wireless Communications 论文](https://doi.org/10.1109/MWC.2025.3600950)

论文链接根据 [ACE Lab 发表记录](https://acelab-shu.github.io/authors/shan-cao/)更新。本文报道架构研究，配图不属于新的 ACE-Echo 1.0 基准测试。
