---
title: 性能与验证依据
layout: echo-doc
summary: 区分 1.0 仿真精度与早期架构基准测试。
sidebar: mydoc_sidebar_zh
permalink: zh_mydoc_performance.html
section: Evaluation
content_status: 测量范围与研究归档
last_reviewed: '2026-09-13'
lang: zh-CN
translation_url: mydoc_performance.html
keywords: ACE-Echo, Venus, 性能与验证依据
---

ACE-Echo 1.0 帮助开发者估计 Venus 应用的性能。我们通过指定用例将周期级模型与 RTL 比较。仿真器与 RTL 的比较，和不同硬件架构之间的比较，是两类独立的证据。

## ACE-Echo 1.0 时序测量
{: #ace-echo-10-timing-measurements }

记录的八 DAG 回归采用标称 300 MHz Tile 与 150 MHz AXI 时钟。DAG 区间的最大绝对误差为 **0.4633%**，单任务最大绝对误差为 **2.2834%**。见[完整结果与区间定义](zh_venus1_validation.html)。

这些结果对应列出的输入矩阵和模型。时序误差小，不等于所有输出的算法正确性通过，也不等于任意场景逐周期等价或已测量芯片能耗。

## 早期架构研究
{: #earlier-architecture-research }

以下基准测试评估早期架构配置，需结合对应的实验设置与论文解读。ACE-Echo 1.0 仿真器的测量结果单独列于上文。

### Tile 级比较
{: #tile-level-comparison }

{% include image.html file="performance2.png" caption="历史 Tile 级比较：AVX、Neon 与 TI DSP。应同时参考原始工作负载、编译设置与 lane 配置。" %}

### 早期硬件平台
{: #earlier-hardware-platforms }

{% include image.html file="performance3.png" caption="早期实现的历史对比表，并非第一代芯片在相同工艺、相同时钟下的测量。" %}

### 向量架构实验
{: #vector-architecture-experiments }

{% include image.html file="performance4.png" caption="历史 UVP/Ara 算子比较。原图将 L-lane Ara 与 4L-lane UVP 配对，二者 lane 标签不能混用。" %}

### PBCH 时延分解
{: #pbch-latency-breakdown }

{% include image.html file="performance5.png" caption="历史 BCH 流程分解：50 MHz，Lane32Reg1024。1.0 回归采用不同的时钟和硬件配置。" %}

## 阅读原始背景
{: #read-the-original-context }

[学术成果](zh_academic_achievements.html)收录项目的架构论文。[原性能页面](https://github.com/ACELab-SHU/ACE-Echo/blob/8770c29996116dfd2bb791fdd5132a0949c294b3/pages/mydoc/mydoc_Performance.md)保留完整实验描述。
