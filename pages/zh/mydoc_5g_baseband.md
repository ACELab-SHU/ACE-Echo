---
title: Venus 上的 5G / NR
layout: echo-doc
summary: 连接 NR 处理流程、芯片里程碑与当前仿真用例。
sidebar: mydoc_sidebar_zh
permalink: zh_mydoc_5g_baseband.html
section: Applications
content_status: 应用范围
last_reviewed: '2026-09-13'
lang: zh-CN
translation_url: mydoc_5g_baseband.html
keywords: ACE-Echo, Venus, Venus 上的 5G / NR
---

5G NR 是 Venus 通信–AI 架构的重要应用方向。本页分别介绍处理流程、原有硬件演示，以及 ACE-Echo 1.0 已测量的用例。

## 从同步到系统信息
{: #from-synchronization-to-system-information }

<ol class="process-cards"><li><span>01</span><h3>发现小区</h3><p>检测同步信号并确定物理小区标识。PSS 与 SSS 共同参与 NR 小区标识的计算。</p></li><li><span>02</span><h3>解码广播</h3><p>处理 PBCH 及其参考信号，恢复广播载荷和 MIB 信息。</p></li><li><span>03</span><h3>获取系统信息</h3><p>通过控制信道调度，定位承载 SIB1 等系统信息的共享信道传输。</p></li></ol>

这是概念流程，不是固定时延保证。同步精度、捕获时间与解码成功率取决于波形、信道、配置和实现。物理信号见 [3GPP TS 38.211，Release 16](https://www.etsi.org/deliver/etsi_ts/138200_138299/138211/16.04.00_60/ts_138211v160400p.pdf)，广播和系统信息定义见 [TS 38.331](https://www.etsi.org/deliver/etsi_ts/138300_138399/138331/15.24.01_60/ts_138331v152401p.pdf)。

## 项目已展示的进展
{: #what-the-project-has-demonstrated }

| 依据 | 说明 |
|---|---|
| [2025 年 1 月 FPGA 演示](zh_application-1.html) | 评估板上的早期 UVP PBCH/MIB 实验 |
| [第一代芯片](zh_venus1-first-light.html) | 2026 年项目评审记录的芯片点亮与真实信号 NR 小区搜索 |
| [ACE-Echo 1.0 回归](zh_venus1_validation.html) | 八个软硬件时序比较 DAG 中的 `nrPBCH` 和 `nrPDCCH` |

首颗芯片里程碑不代表完整 PBCH/MIB/SIB1、长期稳定性或端到端吞吐率已通过验证。同样，仿真器与 RTL 已知输出位一致，也不能替代独立的算法参考。

## 开发 NR 应用
{: #develop-an-nr-application }

先运行[小型安装 smoke](zh_venus1_get_started.html)，再查看配套的通信 workload 源码及所需输入。比较实现前应固定输入矩阵和预期输出。通过[编程模型](zh_mydoc_programming_model.html)划分任务，并参考[验证报告](zh_venus1_validation.html)解读时序。
