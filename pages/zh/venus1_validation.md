---
title: Venus 1.0 验证报告
layout: echo-doc
content_status: 已记录回归
last_reviewed: '2026-09-13'
section: Evaluation
summary: Gem5 与 RTL 的测量一致性，以及明确的时序和正确性边界。
sidebar: mydoc_sidebar_zh
permalink: zh_venus1_validation.html
folder: mydoc
lang: zh-CN
translation_url: venus1_validation.html
keywords: ACE-Echo, Venus, Venus 1.0 验证报告
---

2026 年 9 月 13 日的回归完成了八个 DAG，涵盖 **202 个任务、557 个返回值**，使用 Gem5 fast 模式、L1 契约时序引擎及重新执行的完整 SoC RTL 运行。标称 Tile 时钟为 300 MHz，AXI 时钟为 150 MHz。

## 时序测量
{: #measured-timing }

| DAG | 任务数 | DAG 区间误差 | 任务执行时间之和误差 |
|---|---:|---:|---:|
| nrPBCH | 19 | −0.0355% | −0.0579% |
| nrPDCCH | 24 | −0.1604% | −0.1007% |
| ltePBCHDag1_hw | 27 | +0.0021% | −0.0520% |
| ltePBCHDag2_hw | 58 | +0.0584% | +0.0207% |
| ltePCFICH | 7 | −0.4026% | +0.0006% |
| ltePDCCHDag1_hw | 9 | +0.4633% | +0.5316% |
| ltePDCCHDag2_hw | 6 | −0.1422% | −0.1125% |
| ltePDSCH | 52 | −0.0347% | −0.0782% |

误差定义为 `(Gem5 − RTL) / RTL`。DAG 区间从首个任务开始执行，到最后一个任务完成，包括任务间 DMA 与调度，不包含启动、初始加载及最终返回 DMA。DAG 区间最大绝对误差为 **0.4633%**，单个任务最大绝对误差为 **2.2834%**。这些结果适用于本次输入矩阵，不能推广为所有工作负载的保证。

## 输出一致性
{: #output-agreement }

已知输出位未发现差异。但 RTL 返回缓冲区中存在 **607,984 个未知位**，这些位仍未验证，也未被直接视为填充位。因此不宣称完整逐位正确性 PASS。

该比较不能证明相对于 MATLAB 或认可 golden 的独立算法正确性。特别是 PDSCH 样例的 CRC 问题，与仿真器一致性仍是两个独立问题。

## 传输与模型边界
{: #transfer-and-model-boundaries }

计入 Gem5 额外的 82 次代码加载后，全部 3,128 次 RTL 传输在有序方向、Tile 本地地址与长度上匹配。RTL 可在代码 CRC 命中后跳过加载，当前 Gem5 路径仍保留该加载。RTL 运行时 DMT 分配也不同于 Gem5 的静态槽位复用。这些结果不代表 AXI 握手逐拍等价。

L1 契约引擎解释固件执行计划并建模传输，不执行完整的 L1 标量 Scheduler CPU。ACE-Echo 提供周期级建模，并在声明范围内取得测量一致性；本报告不宣称任意场景逐周期等价。

## 源码版本
{: #source-identities }

- 回归平台：`be19d6a782d80360dfd89d37524a8277f6c5c339`
- DSL：`91f4579a187890e58f2ee225bb35b29762967632`
- 八个 DAG workload：`2c0feaf8b5e773553711cf4431ada59e9b434329`
- RTL 参考：`a34a99aeb9245e01178d7a357c226e87a4249aef`

发布包增加了打包、文档和入门示例，未改变这八个 DAG 的实现。公开发行的来源记录见 `platform/PUBLIC_https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/_MANIFEST.json`。

[开始使用 Venus 1.0](zh_venus1_get_started.html)或[返回平台概览](zh_index.html)。
