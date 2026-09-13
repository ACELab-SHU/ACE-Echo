---
title: Venus 编程模型
layout: echo-doc
summary: 定义任务图，面向同一后端编译，并评估生成的应用。
sidebar: mydoc_sidebar_zh
permalink: zh_mydoc_programming_model.html
section: Documentation
content_status: ACE-Echo 1.0
last_reviewed: '2026-09-13'
lang: zh-CN
translation_url: mydoc_programming_model.html
keywords: ACE-Echo, Venus, Venus 编程模型
---

在 ACE-Echo 中，**任务（task）**包含用 Venus C 编写的计算，**DAG**通过输入和返回值连接任务。选择的**后端（backend）**保证编译器几何参数、内存约定和仿真设置一致。

{% include echo-workflow.html %}

## 从可运行示例开始
{: #start-with-a-working-example }

[完成安装](zh_venus1_get_started.html)后，查看 `workloads/5g_lite/tasks/forge_vector_smoke/`。其中包含两个 C 任务、一个 BAS 图和一个小型主机参考。第一个任务将两个有符号向量相加，第二个任务使用上游结果还原第一个输入。smoke 会检查其声明用例的完整输出。

```text
forge_vector_smoke/
├── Task_forgeAdd.c
├── Task_forgeRestore.c
├── forge_vector_smoke.bas
├── reference.py
└── run.py
```

## 分清各项约定
{: #separate-the-contracts }

| 约定 | 需要明确的内容 |
|---|---|
| 数值行为 | 元素类型、有无符号、溢出、舍入与预期输出 |
| 任务接口 | 输入顺序、返回值与实际返回字节数 |
| 图 | 任务依赖与对外暴露的应用输出 |
| 硬件 | 编译和仿真共同使用的唯一明确后端 |
| 验证依据 | 输入用例、参考结果、输出覆盖范围与计时间隔 |

[Venus 指南](zh_mydoc_venus_user_guide.html)介绍任务代码，[DSL 指南](zh_mydoc_dsl_user_guide.html)介绍 BAS 和编译，[Scheduler 指南](zh_mydoc_l1_scheduler_user_guide.html)介绍应用契约执行。

## 运行与检查
{: #run-and-inspect }

```bash
make smoke
```

安装 smoke 会创建新的运行目录。常规 CLI 操作在 `runs/` 下记录命令、日志与产物身份；当所选操作产生 Gem5 输出及任务/DAG 跟踪时，也会收集这些文件。请通过生成的运行清单查找产物，不要假定仍使用旧版 `Debug/emular_vins_result/` 目录。

Fast 模式是常规开发路径。它保留时序模型，同时减少昂贵的观测；仿真器一致性与独立算法正确性仍需要分别检查。

## 使用 Agent 辅助开发
{: #develop-with-an-agent }

Forge 初始化并记录请求。AI 编程宿主读取仓库技能，在开发者指导下驱动编译、运行与比较操作。见 [Forge 入门说明](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/GETTING_STARTED.md)。

早期任务目录截图和 VEMU 操作说明保存在[原编程指南源码](https://github.com/ACELab-SHU/ACE-Echo/blob/8770c29996116dfd2bb791fdd5132a0949c294b3/pages/mydoc/mydoc_programming_model.md)中，描述的是旧版工作流。
