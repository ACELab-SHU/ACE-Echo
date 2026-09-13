---
layout: echo-doc
title: 第一代芯片流片成功，顺利点亮
summary: 5G-Lite 推动 Venus 成为真实芯片，完成上电调试与真实信号 NR 小区搜索。
section: News
date: 2026-09-13 00:00:00 +0800
published: true
category: application
permalink: zh_venus1-first-light.html
tags:
- news
lang: zh-CN
translation_url: venus1-first-light.html
keywords: ACE-Echo, Venus, 第一代芯片流片成功，顺利点亮
content_status: ''
sidebar: home_sidebar_zh
---

<p class="document-date">发布于 2026 年 9 月 13 日 · 芯片里程碑</p>

我们的第一代芯片已完成流片并成功点亮。5G-Lite 项目从软件与 RTL 出发，完成物理实现、制造、封装和评估板上电调试。Venus 已在真实芯片上运行。

<figure class="news-photo"><img src="images/venus1-silicon.jpeg" width="1279" height="1706" alt="封装后的第一代 DT-VENUS-A1 AI-RAN SoC"><figcaption>第一代 Venus 芯片实物，图片来自 5G-Lite 结题报告。</figcaption></figure>

## 一颗真正运行的芯片
{: #a-chip-that-runs }

上电调试期间，芯片执行了目标程序，并通过 UART 持续输出：

<blockquote class="news-quote">DT AI-RAN!</blockquote>

这验证了基本时钟、复位、代码执行与串口输出路径。随后，项目展示了真实空口信号处理和 NR 小区搜索，其报告的小区标识与商用网络参考工具观测到的 PCI 一致。

这些进展记录于团队 **2026 年 9 月 2 日的项目结题评审**。该日期是评审日期，不作为流片或首次点亮的具体日期。

## Echo 与 Venus 汇聚
{: #echo-and-venus-come-together }

Echo 从真实通信流程出发，Venus 发展可编程处理架构及工具链。5G-Lite 将两者结合在面向通信的 SoC 中。

可配置 DFE 硬件处理通信前端基本操作，Venus 处理器与 Scheduler 支持可编程算法和任务执行。首颗芯片为通信–AI 架构研究提供了实物基础，也为下一代设计提供反馈。

本次里程碑是芯片点亮与 NR 小区搜索。完整端到端吞吐率、长期稳定性及 PBCH/MIB/SIB1 的完整资格验证仍是后续工作。

## 开放应用开发
{: #opening-application-development }

我们通过 **ACE-Echo 1.0** 开放开发平台：提供基于 Gem5 的仿真器，用于开发 Venus 应用并快速估计性能，同时提供初版 Agent 辅助开发框架。

**ACE-Echo 2.0 路线图**将在此基础上推进意图驱动开发：用户描述应用意图，由 Agent 生成可部署到 Venus 的实现。

[了解项目](zh_index.html) · [使用 ACE-Echo 1.0 开发](zh_venus1_get_started.html) · [全部新闻](zh_news.html)
