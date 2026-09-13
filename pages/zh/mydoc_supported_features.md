---
title: 1.0 可以做什么
layout: echo-doc
summary: Venus 应用开发、Gem5 仿真与初版 Agent 辅助。
sidebar: mydoc_sidebar_zh
permalink: zh_mydoc_supported_features.html
section: Documentation
content_status: ACE-Echo 1.0
last_reviewed: '2026-09-13'
lang: zh-CN
translation_url: mydoc_supported_features.html
keywords: ACE-Echo, Venus, 1.0 可以做什么
---

ACE-Echo 1.0 是面向 Venus 应用的开发平台。当前支持范围由发布源码、选择的硬件后端和已有验证用例共同界定。

{% include echo-workflow.html %}

## 当前能力
{: #current-capabilities }

| 能力 | 1.0 工作流中的支持 |
|---|---|
| 应用编写 | Venus C 任务与描述任务依赖的 BAS 文件 |
| 编译 | 随包提供的 DSL 前端，连接单独安装的 Venus 定制 LLVM 工具链 |
| 仿真 | Gem5 任务、DAG 与应用契约执行 |
| 性能探索 | 所选后端下的周期级建模与近似时序评估 |
| 输出检查 | 与用户提供的参考显式比较，每次运行记录输入和输出身份 |
| Agent 辅助 | 由外部 AI 编程宿主使用 Forge 请求与仓库技能 |
| 可复现性 | 逐次记录命令、日志、配置身份与生成产物 |

## 应用覆盖
{: #application-coverage }

已发布的 Venus1 回归涵盖 `nrPBCH`、`nrPDCCH`、`ltePBCHDag1_hw`、`ltePBCHDag2_hw`、`ltePCFICH`、`ltePDCCHDag1_hw`、`ltePDCCHDag2_hw` 和 `ltePDSCH`。[验证报告](zh_venus1_validation.html)说明了时序一致性及输出正确性的剩余限制。

随包的双任务向量 smoke 示例适合作为安装检查的起点。更完整的 5G/LTE 流程和通信–AI 研究属于项目生态，并不意味着本次发布已经验证所有算子、输入组合或端到端应用。

[5G/NR](zh_mydoc_5g_baseband.html) · [LTE](zh_mydoc_lte_baseband.html) · [GNSS](zh_mydoc_gnss_baseband.html) · [LoRa](zh_mydoc_lora_baseband.html)

## 范围边界
{: #scope-boundaries }

本次发布不包含自动 BAS 转 RTL 导出器、通用 CPU/GPU 回退执行、可视化 DAG 编辑器或自动 ASIC 功耗与资源报告。旧网站对这些功能的描述超出了 1.0 已实现的范围。

L1 应用契约引擎对 Scheduler 的执行计划建模，不执行完整的标量 Scheduler 固件。RTL 运行需要另外提供硬件源码与工具。完整的“意图到可部署应用”生成属于 [2.0 路线图](zh_roadmap.html)。

精确的命令范围见[固定版本的平台 README](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/README.md)与[覆盖契约](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/COVERAGE.md)。
