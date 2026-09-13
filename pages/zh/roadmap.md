---
title: ACE-Echo 路线图
layout: echo-doc
toc: false
summary: 从实用的 Venus 应用开发，走向意图驱动的应用生成。
sidebar: mydoc_sidebar_zh
permalink: zh_roadmap.html
section: Project
content_status: 当前方向
last_reviewed: '2026-09-13'
lang: zh-CN
translation_url: roadmap.html
keywords: ACE-Echo, Venus, ACE-Echo 路线图
---

我们的方向是让通信–AI 硬件更容易编程。第一步是提供好用的仿真器和有指导的开发流程；下一步是构建更完整的 Agent 辅助路径，将用户意图转化为面向 Venus 的应用。

{% include echo-roadmap.html %}

## 版本之间的变化
{: #what-changes-between-releases }

| | ACE-Echo 1.0 | ACE-Echo 2.0 方向 |
|---|---|---|
| 起点 | 开发者定义任务、DAG 与参考 | 用户给出应用意图 |
| 开发 | Venus C/BAS 与初版 Agent 辅助 | Agent 主导生成与迭代改进 |
| 评估 | Gem5 输出检查与近似时序 | 将验证融入生成迭代 |
| 结果 | 由开发者审核、评估的实现 | 以可部署到 Venus 为目标的应用 |

2.0 栏描述规划中的行为。本网站尚未公布其发布日期和最终支持的应用范围。

## 软件版本与硬件代际
{: #software-versions-and-hardware-generations }

**ACE-Echo 1.0 / 2.0** 表示平台版本，**Venus 1.0 / 2.0** 表示硬件后端。源码树中存在 Venus2 后端，不代表 ACE-Echo 2.0 的 Agent 工作流已经发布。1.0 网站中的回归结果面向 Venus1。

## 早期规划
{: #previous-plans }

早期 v0.5/v1.0 beta 时间表及 2026 年 1 月目标已由本路线图取代。[v0.1 发布记录](zh_mydoc_beta_release_notes.html)仍保留为项目历史。
