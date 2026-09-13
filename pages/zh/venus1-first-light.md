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

## 开发板实测功耗
{: #measured-power-on-the-evaluation-board }

团队对从 always-on 基线到 SSB 解码循环的九种运行场景进行了功耗测试。**运行 SSB 解码循环时，开发板与 Venus SoC 的合计实测功耗约为 1.06 W。**

| 测试场景 | 开发板与 SoC 合计实测功耗 |
| --- | --- |
| Always-on 基线 | 0.2877 W |
| DFE 计算场景 | 0.7683 W |
| Cluster 计算场景 | 0.8923 W |
| SSB 解码循环 | 1.0597 W |

以上为对应测试配置下的系统功耗，包含开发板供电链路。

<figure class="doc-figure"><a class="figure-link" href="images/venus1-power-measurements.png"><img src="images/venus1-power-measurements.png" width="1684" height="353" loading="lazy" alt="开发板与 Venus SoC 在九种场景下的原始功耗测量矩阵"></a><figcaption>九种测试场景的实测功耗与各域配置。点击图片可查看完整分辨率记录。</figcaption></figure>

## Echo 与 Venus 汇聚
{: #echo-and-venus-come-together }

可配置 DFE 负责前端通信原语，Venus 处理器与 Scheduler 承接可编程算法和任务调度。5G-Lite 的成功流片，为我们的通信与 AI 融合架构研究提供了硬件基础，也为后续应用开发与验证积累了实践经验。

## 开放应用开发
{: #opening-application-development }

**ACE-Echo 1.0** 将这一开发路径开放给更多开发者：通过基于 Gem5 的仿真器开发 Venus 应用、快速估计周期级性能，并使用初版 Agent 辅助开发框架。

**ACE-Echo 2.0** 将进一步向意图驱动开发推进：由用户描述应用意图，由 Agent 辅助生成可部署到 Venus 的实现。

感谢每一位参与芯片设计、验证、流片和硅后测试的项目成员与合作伙伴，共同推动第一代芯片成功落地。

[了解项目](zh_index.html) · [使用 ACE-Echo 1.0 开发](zh_venus1_get_started.html) · [全部新闻](zh_news.html)
