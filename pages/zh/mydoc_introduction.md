---
title: 认识 ACE-Echo
layout: echo-doc
toc: false
summary: 面向 Venus 通信–AI 融合处理器的开源开发平台。
sidebar: mydoc_sidebar_zh
permalink: zh_mydoc_introduction.html
section: Project
content_status: ACE-Echo 1.0
last_reviewed: '2026-09-13'
lang: zh-CN
translation_url: mydoc_introduction.html
keywords: ACE-Echo, Venus, 认识 ACE-Echo
---

ACE-Echo 将通信与 AI 融合芯片研究连接到应用开发。**Venus** 是可编程 RISC-V 处理器，**AURA** 是架构与编程基础，**ACE-Echo** 则是面向这一硬件的开源开发平台。

## 架构、芯片与软件
{: #architecture-silicon-and-software }

AURA 由 Venus Language、Zoozve 编译器和数学库组成。其数据流方法将任务中的计算与任务间的数据搬运联系起来。

{% include image.html file="aura-architecture.png" caption="AURA 包含编程语言、编译器和数学库。三者是架构组成部分，不是依次执行的流水级。" %}

Venus 集成可编程 Tile、Scheduler 和数字前端。我们在这一共同的硬件基础上探索通信处理与 AI 工作负载的融合。

{% include image.html file="venus-architecture.png" caption="Venus 的硬件组成：Tile、Scheduler 和数字前端。" %}

第一代芯片已完成流片并成功点亮。[芯片新闻](zh_venus1-first-light.html)记录了上电调试和真实信号 NR 小区搜索进展。芯片里程碑与开源仿真器的验证范围分别说明。

## 用 1.0 开发应用
{: #build-applications-with-10 }

ACE-Echo 1.0 提供好用的 Gem5 仿真器和初版 Agent 辅助开发框架。开发者可以编写 Venus 应用、检查输出，并快速估计大致性能。Venus 定制 LLVM 等主机工具仍需单独准备。

{% include echo-workflow.html %}

从[安装指南](zh_venus1_get_started.html)开始，然后了解[编程模型](zh_mydoc_programming_model.html)与[支持范围](zh_mydoc_supported_features.html)。

## 从开发辅助走向意图驱动
{: #from-assistance-to-intent }

{% include echo-roadmap.html %}

[路线图](zh_roadmap.html)取代早期 beta 日期和功能预测。ACE-Echo 的软件版本与 Venus 的硬件代际独立命名。

## 一起建设平台
{: #built-for-collaboration }

研究者可以探索面向硬件的算法，应用开发者可以在硬件测试前比较不同实现。示例、文档和可复现实验的贡献，都能帮助平台扩展。

[开发团队](zh_mydoc_developers.html) · [参与单位](zh_mydoc_contributing_organizations.html) · [联系 ACE Lab](mailto:shenyihao@shu.edu.cn)
