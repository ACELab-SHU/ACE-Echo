---
title: UVP 成功解码 5G PBCH MIB
published: true
category: application
permalink: zh_application-1.html
summary: 2025 年 FPGA 评估板上的广播信道解码演示。
tags:
- 5G
- PBCH
- MIB
- decode
content_status: 历史项目新闻
section: News
lang: zh-CN
translation_url: application-1.html
keywords: ACE-Echo, Venus, UVP 成功解码 5G PBCH MIB
sidebar: home_sidebar_zh
layout: post
date: 2025-01-01 00:00:00 +0800
---

2025 年 1 月 1 日，团队在实验室使用基于 UVP 架构的系统，成功解码 **5G 物理广播信道（PBCH）中的主信息块（MIB）**，验证了 UVP 在实际无线通信场景中的应用价值。

{% include image.html file="application1_1.png" caption="SoC 架构与评估板配置。" max-width="450" %}

{% include image.html file="application1_2.png" caption="评估板接收并处理基站 PBCH 信号的测试环境。" %}

当时部署在 VCU118 评估板上的 UVP 系统采用 32-lane、Reg2048 配置，与轻量 RISC-V 标量核紧耦合，通过总线互联协调各组件。数字前端（DFE）负责实时信号处理，存储器用于代码执行，串口支持调试和离线代码访问。

测试按 PBCH 流程执行：首先进行 OFDM 解调，完成 3 次 FFT 及相关前后处理，然后执行 SSS 搜索、DMRS 搜索等步骤。DMRS 搜索包含大量相关计算与伪随机序列生成，需要密集的排列和归约求和。之后依次进行信道估计、信道均衡与解调、比特解扰和信道译码。信道译码采用 Polar 码的置信传播算法；其时延受信号质量影响较大，信号质量越差通常需要越多迭代。

UVP 通过非 2 的幂次寄存器分组、更大的物理寄存器文件，以及具有定制加载/存储方案的对称和非对称向量指令，处理各环节的向量运算。系统最终成功恢复 MIB。原演示记录描述在 400MHz 下于毫秒级完成整个 PBCH 解码流程；该描述对应当时实验配置。

{% include image.html file="application1_3.png" caption="原 FPGA 演示的处理结果。" max-width="500" %}

这是 2025 年的 FPGA 演示。后续芯片进展见[第一代芯片新闻](zh_venus1-first-light.html)。
