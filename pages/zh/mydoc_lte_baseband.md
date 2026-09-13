---
title: Venus 上的 LTE
layout: echo-doc
toc: false
summary: 可用 LTE DAG 用例及其 1.0 评估范围。
sidebar: mydoc_sidebar_zh
permalink: zh_mydoc_lte_baseband.html
section: Applications
content_status: 应用范围
last_reviewed: '2026-09-13'
lang: zh-CN
translation_url: mydoc_lte_baseband.html
keywords: ACE-Echo, Venus, Venus 上的 LTE
---

LTE 工作负载属于 Venus 开发和回归集合。下文介绍支持的用例与评估结果。

## 已发布回归中的 DAG
{: #dags-in-the-published-regression }

| 处理模块 | DAG 用例 |
|---|---|
| 广播信道 | `ltePBCHDag1_hw`、`ltePBCHDag2_hw` |
| 控制格式 | `ltePCFICH` |
| 控制信道 | `ltePDCCHDag1_hw`、`ltePDCCHDag2_hw` |
| 共享信道 | `ltePDSCH` |

这六个 DAG 完成了 2026 年 9 月的 Gem5 fast / RTL 时序回归。测量仅适用于指定输入和计时边界，不代表完整的空口 LTE 小区搜索应用或所有协议配置已通过验证。

## 准确解读结果
{: #interpret-results-carefully }

各 DAG 时序误差、RTL 输出未知位及独立的 PDSCH CRC 问题见[验证报告](zh_venus1_validation.html)。运行完成、仿真器一致与算法正确，是不同的结果。

## 开始开发
{: #start-developing }

按照 [1.0 指南](zh_venus1_get_started.html)安装配套源码与工具，统一选择 Venus1 后端，记录波形和参数，并用该用例的参考检查每项必要输出。
