---
layout: echo-doc
title: 第一代芯片流片成功，顺利点亮
summary: 5G-Lite 推动 Venus 成为真实芯片，完成上电调试与真实信号 NR 小区搜索。
section: News
hide_sidebar: true
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

**我们的第一代 Venus 芯片流片成功，顺利点亮！** 通过 5G-Lite 项目，团队完成了芯片封装与评估板上电调试，并在真实空口信号下成功跑通 NR 小区搜索。芯片识别出的物理小区标识与商用网络现场测量结果一致。

从通信算法、软件与 RTL，到可以运行真实应用的芯片，Echo 的通信软件与 Venus 的可编程架构在这次流片中汇聚，多年的研究与工程积累成为了真实硅片。

<figure class="news-photo"><img src="images/venus1-silicon.jpeg" width="1279" height="1706" alt="封装后的 DT-VENUS-A1 AI-RAN SoC"><figcaption>第一代 DT-VENUS-A1 AI-RAN SoC 芯片实物。</figcaption></figure>

## 从设计到芯片实物
{: #from-design-to-packaged-silicon }

团队完成物理实现、制造、引线键合与封装，并在评估板上实现芯片启动、应用程序运行和无线信号处理，打通从设计到硅后测试的完整工程流程。

| 芯片与封装 | 参数 |
| --- | --- |
| 芯片型号 | DT-VENUS-A1 AI-RAN SoC |
| 封装型号 | HLQFP128B |
| 封装本体尺寸 | 14 × 14 mm |
| 引脚数量 | 128 |
| 处理架构 | 可配置 DFE、可编程 Venus 处理器与任务调度器 |
| 时钟与电源组织 | 3 个 PLL、4 个电源域 |

<div style="display:grid;grid-template-columns:repeat(auto-fit,minmax(min(100%,220px),1fr));gap:20px;align-items:start">
<figure class="doc-figure"><a class="figure-link" href="images/venus1-wire-bonding.webp"><img src="images/venus1-wire-bonding.webp" width="900" height="1600" loading="lazy" style="width:100%;height:300px;object-fit:contain" alt="塑封前的 Venus 裸片与引线键合实物"></a><figcaption>裸片装配与引线键合。</figcaption></figure>
<figure class="doc-figure"><a class="figure-link" href="images/venus1-packaged-devices.webp"><img src="images/venus1-packaged-devices.webp" width="1600" height="900" loading="lazy" style="width:100%;height:300px;object-fit:contain" alt="托盘中完成封装的第一代 Venus 芯片"></a><figcaption>完成封装、用于板级装配与测试的芯片。</figcaption></figure>
<figure class="doc-figure"><a class="figure-link" href="images/venus1-evaluation-board.webp"><img src="images/venus1-evaluation-board.webp" width="900" height="1600" loading="lazy" style="width:100%;height:300px;object-fit:contain" alt="用于 Venus 芯片上电调试与实测的评估板"></a><figcaption>用于芯片点亮与硅后测试的 EVB 评估板。</figcaption></figure>
</div>

## 真实空口验证，小区标识正确匹配
{: #a-chip-that-runs }

硅后测试跑通了真实无线信号输入、DFE 同步处理、Venus 计算和小区搜索结果输出的链路。芯片输出 **PCI 800、SSB 索引 3**，现场 Cellular-Pro 对商用 NR 网络的测量同样显示 **PCI 800**，验证了芯片对物理小区标识的正确识别。

| 对照项 | 芯片输出 | 现网参考 |
| --- | --- | --- |
| 物理小区标识（PCI） | 800 | 800 |
| SSB 索引 | 3 | — |
| 网络环境 | — | NR-SA · n1 频段 · NR-ARFCN 426030 |

<figure class="doc-figure" style="max-width:420px"><a class="figure-link" href="images/venus1-nr-cell-search.png"><img src="images/venus1-nr-cell-search.png" width="612" height="912" loading="lazy" alt="芯片实测输出 cellid 800、ssbIdx 3 的记录"></a><figcaption>芯片小区搜索实测输出。其中 cellid 字段对应物理小区标识 PCI。</figcaption></figure>

从成功点亮到真实空口小区搜索，芯片上的 NR 小区搜索链路通过了实际信号验证，可编程架构也由此承载起真实通信任务。

## 低功耗运行，专为通信计算而设计
{: #measured-power-on-the-evaluation-board }

Venus 将可配置通信原语与可编程计算结合，在真实芯片上跑通 NR 小区搜索。Cluster 满载时，评估板不计 DFE 的功耗为 **0.95 W**，为无线信号监测、同步处理和边缘通信应用提供低功耗计算基础。

<section class="energy-showcase" aria-label="28 nm 工艺归一化对比">
<div class="energy-power"><div><span class="energy-eyebrow">VENUS</span><p>Cluster 满载 · 整个评估板，不计 DFE</p></div><strong class="energy-power-value">0.95 <small>W</small></strong></div>
<p class="energy-comparison-label">28 nm 工艺归一化对比</p>
<div class="energy-duel">
<div class="energy-contender"><p class="energy-versus">VENUS 对比 ARM</p><h3 class="energy-opponent">NEON</h3><p class="energy-metric-label">能效对比</p><strong class="energy-value"><small>≈</small>7.7<span>×</span><span class="energy-up" aria-hidden="true"><svg viewBox="0 0 32 40" focusable="false"><path d="M16 34V7M5 18 16 7 27 18" /></svg></span></strong><p class="energy-result">28 nm 归一化算例 · 参考平台 / Venus</p><p class="energy-platform">RK3588 · Cortex-A76</p></div>
<div class="energy-contender"><p class="energy-versus">VENUS 对比 INTEL</p><h3 class="energy-opponent">AVX-512</h3><p class="energy-metric-label">能效对比</p><strong class="energy-value"><small>≈</small>25.6<span>×</span><span class="energy-up" aria-hidden="true"><svg viewBox="0 0 32 40" focusable="false"><path d="M16 34V7M5 18 16 7 27 18" /></svg></span></strong><p class="energy-result">28 nm 归一化算例 · 参考平台 / Venus</p><p class="energy-platform">Xeon 6434</p></div>
</div>
</section>

**测量范围：RK3588 采用整块开发板功耗，Xeon 采用 CPU package 功耗，Venus 采用 Cluster 满载时整个评估板不计 DFE 的功耗。** 两项倍数由下述单核比较数据按 28 nm 工艺归一化计算。

<details class="doc-figure"><summary>测量范围与能耗计算方法</summary>
<p><strong>功耗测量。</strong>ARM Cortex-A76 参考平台为 RK3588，测量覆盖整块开发板；Xeon 6434 测量 CPU package；Venus 测量整个评估板在 Cluster 满载时、不计 DFE 的功耗，数值为 0.95 W。单核比较中，CPU 空闲功耗按物理核心数分摊。</p>
<p><strong>工作负载与数据。</strong>CPU 参考数据来自 LDPC 工作负载：RK3588 的 Cortex-A76 基线启用 NEON，Xeon 6434 基线启用 AVX-512；Venus 数据来自 SSB 处理链路。Venus 采用 0.95 W / 8.448 Mbps 计算单位数据处理能耗，得到约 112.45 nJ/bit。比较结果对应这些工作负载和测量范围。</p>
<p><strong>工艺归一化。</strong>按 E₂₈ = E × (28 / L)² 折算，RK3588、Xeon 6434 与 Venus 的工艺节点分别按 8 nm、7 nm 和 40 nm 计入。参考平台与 Venus 的归一化能耗比为：(34.58 × 12.25) / (112.45 × 0.49) ≈ 7.7；(88.33 × 16) / (112.45 × 0.49) ≈ 25.6。</p>
</details>

### 让你的通信应用运行在 Venus 上

从小区搜索或信号处理示例出发，逐步替换算法、调整数据流，探索自己的应用。**通过 ACE-Echo 1.0 编译并仿真 Venus 应用，在部署到硬件之前，快速了解大致的周期级性能表现。**

[开始使用 ACE-Echo 1.0 →](zh_venus1_get_started.html) · [探索 NR 小区搜索示例 →](zh_mydoc_5g_baseband.html)

## Echo 与 Venus 汇聚
{: #echo-and-venus-come-together }

可配置 DFE 负责前端通信原语，Venus 处理器与 Scheduler 承接可编程算法和任务调度。5G-Lite 的成功流片，为我们的通信与 AI 融合架构研究提供了硬件基础，也为后续应用开发与验证积累了实践经验。

## 开放应用开发
{: #opening-application-development }

**ACE-Echo 1.0** 将这一开发路径开放给更多开发者：通过基于 Gem5 的仿真器开发 Venus 应用、快速估计周期级性能，并使用初版 Agent 辅助开发框架。

**ACE-Echo 2.0** 将进一步向意图驱动开发推进：由用户描述应用意图，由 Agent 辅助生成可部署到 Venus 的实现。

感谢每一位参与芯片设计、验证、流片和硅后测试的项目成员与合作伙伴，共同推动第一代芯片成功落地。

[了解项目](zh_index.html) · [使用 ACE-Echo 1.0 开发](zh_venus1_get_started.html) · [全部新闻](zh_news.html)
