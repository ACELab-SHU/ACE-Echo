---
title: 开始使用 ACE-Echo 1.0
layout: echo-doc
content_status: ACE-Echo 1.0
last_reviewed: '2026-09-13'
section: Documentation
summary: 开发 Venus 应用，用 Gem5 估计性能，并开始使用 Agent 辅助开发框架。
sidebar: mydoc_sidebar_zh
permalink: zh_venus1_get_started.html
folder: mydoc
lang: zh-CN
translation_url: venus1_get_started.html
keywords: ACE-Echo, Venus, 开始使用 ACE-Echo 1.0
---

ACE-Echo 是面向 Venus 通信–AI 融合处理器的开源开发平台。1.0 提供基于 Gem5 的仿真器和初版 Agent 辅助开发框架，帮助开发者构建 Venus 应用并快速了解大致性能。公开发行内容位于 `platform/`；仓库根目录中的早期工具属于旧版。

## 准备工具
{: #prepare-your-tools }

使用 Linux 主机，安装 Python 3.8 或更高版本、Git、Make、C/C++ 编译器，以及 `platform/docs/GETTING_STARTED.md` 列出的依赖。还需要 **Venus 定制 LLVM 工具链**和兼容的 RISC-V GCC。普通上游 LLVM 不实现 Venus 指令扩展。编译器、RTL 源码和商业 EDA 工具需要单独准备，不随本次发布提供。

## 获取源码
{: #clone-the-source }

当[上游 PR #5](https://github.com/ACELab-SHU/ACE-Echo/pull/5)仍在审核时，1.0 源码可从以下发布分支获取。本预览对应固定的 `db763e2` 发布快照；在 PR 合并前克隆上游默认分支，无法得到相同的 `platform/` 发行内容。

```bash
git clone --branch release/ace-echo-1.0 --single-branch \
  https://github.com/HorryShenYH/ACE-Echo.git
cd ACE-Echo/platform
```

公开源码包包含固定版本的 DSL 源码、八个 Venus1 回归 DAG 和一个小型入门 smoke 示例，无需访问内部子模块服务器。具体源码版本见 `PUBLIC_https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/_MANIFEST.json`。

## 选择 Venus 1.0
{: #select-venus-10 }

创建主机配置，并在 `.ace-echo/host-tools.json` 中修改外部工具路径：

```bash
mkdir -p .ace-echo
cp configs/host.example.json .ace-echo/host-tools.json
```

随后解析 64 lane、512 行的 Venus1 配置，其标称 Tile 时钟为 300 MHz，AXI 时钟为 150 MHz：

```bash
python3 scripts/bootstrap.py fetch-json
python3 scripts/bootstrap.py configure \
  --tools .ace-echo/host-tools.json \
  --backend configs/backends/venus1p0-64x512-300mhz.json
python3 scripts/bootstrap.py build-gem5 --jobs 4 --mode opt
./ace-echo --config .ace-echo/host/local.toml doctor \
  --scope fast --backend .ace-echo/host/backend.json
```

## 检查安装
{: #check-the-installation }

```bash
make test
make smoke
```

smoke 会重新编译一个双任务 DAG，在 Gem5 fast 模式下运行，并将两个任务的输出与软件参考比较。报告包含实际启用的时钟与计时边界。这是安装检查，不能替代八个通信算法的正确性验证或 RTL 回归。

接下来可查阅 `platform/docs/GETTING_STARTED.md`、`platform/docs/HARDWARE_SELECTION.md` 及 CLI `--help` 中的命令和后端约定。解读时序与正确性结果前，请阅读 [Venus1 验证报告](zh_venus1_validation.html)。

## Agent 辅助开发
{: #agent-assisted-development }

1.0 包含初版 Forge 开发框架，将 AI 编程宿主连接到平台的工具链、仿真器和验证流程。使用时请阅读仓库中的 Forge 文档与技能说明。CLI 负责初始化和记录工作，AI 编程宿主提供 Agent，开发者负责指导并审核结果。

完全自动化的“意图到可部署应用”生成是 **2.0 的规划目标**，并非 1.0 承诺的现有能力。见[平台路线图](zh_roadmap.html)。

## 旧版用户迁移
{: #existing-users }

将主机特定路径保存在 `.ace-echo/` 中，配套使用发布包的 DSL 与 workload 源码。[旧版安装指南](zh_mydoc_get_started.html)供早期实验参考，其中的仿真器步骤不适用于 ACE-Echo 1.0。
