---
title: L1 Scheduler 用户指南
layout: echo-doc
guide_reference: true
summary: 了解 DAG 执行、Scheduler 打包与 1.0 应用契约模型。
sidebar: mydoc_sidebar_zh
permalink: zh_mydoc_l1_scheduler_user_guide.html
section: Documentation
content_status: ACE-Echo 1.0
last_reviewed: '2026-09-13'
lang: zh-CN
translation_url: mydoc_l1_scheduler_user_guide.html
keywords: ACE-Echo, Venus, L1 Scheduler 用户指南
---

Scheduler 负责连接任务执行、依赖关系与数据搬移。ACE-Echo 1.0 区分 Gem5 DAG 运行、已编译 Scheduler 包及应用契约运行。

| 操作 | 范围 |
|---|---|
| `run dag` | 通过所选 Gem5 Scheduler 模型执行图 |
| `scheduler build` | 编译 Scheduler，生成 l1.elf／l1.bin |
| `run application --scheduler-engine contract` | 解码固件中的应用计划并由模型执行 |
| 完整 Scheduler 固件执行 | 标量 CPU／外设后端尚未成为已验证的 1.0 模式 |

## 使用生成产物
{: #work-from-generated-artifacts }

先编译 DAG，再使用同次运行记录的 JSON、组合 BIN 和任务镜像路径。以下展示命令形式，所有 /path/to/ 路径需替换为实际产物。

```bash
./ace-echo --config .ace-echo/host/local.toml scheduler build \
  --backend .ace-echo/host/backend.json \
  --dag-name my_dag --dag-json /path/to/dag.json \
  --dag-bin /path/to/dag.bin
```

没有运行时输入的全静态 DAG 可使用 --auto-static-main。动态输入需要匹配的启动程序与输入约定。

```bash
./ace-echo --config .ace-echo/host/local.toml run application \
  --backend .ace-echo/host/backend.json \
  --l1-elf /path/to/l1.elf --scheduler-engine contract --mode fast
```

## 成功运行能说明什么
{: #what-a-successful-run-establishes }

读取覆盖范围、输出比较和时序边界。应用契约运行成功不等于完整标量 Scheduler 固件执行，也不代表新的 RTL 通过。硬件和 RTL 验证仍需配套源码、工具链和测试输入。

进一步阅读：[动态输入](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/DYNAMIC_APPLICATION_INPUTS.md)、[RTL 证据](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/RTL_EVIDENCE_AND_LIFECYCLE.md)、[发布验证范围](zh_venus1_validation.html)。旧 dagInfoPaser.py／l1_main 属于[历史集成路径](https://github.com/ACELab-SHU/ACE-Echo/blob/8770c29996116dfd2bb791fdd5132a0949c294b3/pages/mydoc/mydoc_l1_Scheduler_user_guide.md)。

## 为 L1 准备 DAG
{: #preparing-a-dag-for-l1 }

保留**同一次编译**的 JSON、组合 BIN 与任务镜像。JSON 描述接口顺序和大小，二进制实现该约定。只重编 BAS 却沿用旧调度元数据，即使各组件独立构建成功，也可能产生不匹配的包。

平台打包步骤生成 fire_dag 消费的 C 数据符号，不要手写嵌入图镜像或重排描述符。图中没有 dag_input／dfedata 时可使用静态自动启动程序；否则选用提供运行时输入的 main 源码。

## fire_dag API 参考
{: #fire_dag-api-reference }

当前 Scheduler 在 dagfire.h 中公开此宏：

```c
fire_dag(DAGname, DAGInputNum, DAGOutputNum,
         /* ordered input pointers, then ordered output pointers */);
```

这是签名示意；实际调用需要匹配的生成符号和指针参数。

| 参数 | 含义 | 选择依据 |
|---|---|---|
| `DAGname` | 生成符号的前缀 token | 对应打包图名称，不是运行时字符串 |
| `DAGInputNum` | 运行时输入指针数 | 生成元数据 |
| `DAGOutputNum` | 图输出指针数 | 生成元数据 |
| 后续输入指针 | 输入缓冲区 | 元数据记录顺序 |
| 后续输出指针 | 输出存储 | 元数据记录顺序 |

宏注册图、搬移图／任务／参数元数据，并提供输入输出绑定。它不是可移植的阻塞 C 函数；读取输出或启动依赖图之前，需要沿用匹配启动程序中的完成／FIFO 协议。

### 两个运行时输入的示例
{: #example-with-two-runtime-inputs }

假设图声明如下：

```basic
dfedata char dfe_input[4388]
dag_input short nCellid[1]
return_value short subFrameNum[1]
```

若**生成元数据**把输入排列为 nCellid、dfe_input，则即使上面声明顺序相反，调用也应遵循元数据顺序：

```c
#include "dagfire.h"
// Include the generated DAG symbols supplied by the packaging step.

static short nCellid[1] __attribute__((aligned(64))) = {2};
static char dfe_input[4388] __attribute__((aligned(64)));
// Example allocation: leave space for the task's aligned scalar return.
static unsigned char subFrameNum_storage[64] __attribute__((aligned(64)));

void launch_example(void) {
    // Populate dfe_input before launch.
    fire_dag(dag_name, 2, 1, nCellid, dfe_input, subFrameNum_storage);
    // Use the matching launcher's completion protocol before reading output.
}
```

这是集成片段，dag_name 及生成符号代表实际打包图。旧示例元数据把 nCellid 放前面，调用却先传 dfe_input，这里已统一。输出分配需覆盖生产任务实际返回的字节及 ABI 填充；BAS 的 short[1] 本身不能证明运行时只写两个字节。

## 连接多个 DAG
{: #connecting-one-dag-to-another }

1. 用相同后端编译并打包各图。
2. 按各自元数据绑定输入和输出存储。
3. 通过匹配的运行协议启动生产图。
4. 等待规定的完成信号，再将输出作为下一图的输入。
5. 消费者完成前保留缓冲区，核对跨图类型、字节数及生命周期。

应用契约模型解码受支持的计划和绑定，不模拟任意 L1 main C 控制流。新启动程序即便编译通过，也可能超出契约解码器支持范围，应检查应用覆盖报告。

## 旧版 dagInfoPaser 工作流
{: #legacy-daginfopaser-workflow }

维护原 l1_main 目录布局的用户仍可使用：

```bash
cd /path/to/legacy-project/l1_main
python3 ./dagInfoPaser.py YOUR_DAG
```

在 daginfo/YOUR_DAG.txt 查看接口顺序，再更新对应 source/l1.cpp 及缓冲区声明。旧例使用 l1_api.h；当前包使用 dagfire.h 和生成的打包数据。复现旧应用时，目录与头文件应完整匹配同一版本。

旧直接路径为 dsl/bin/&lt;dag&gt;.bin、dsl/final_output/&lt;dag&gt;.json、dsl/venus_test/ir/&lt;task&gt;.hex。当前平台统一收集到运行产物，并构建隔离的 Scheduler 源码副本。

## 集成问题排查
{: #diagnosing-integration-problems }

| 现象 | 检查项 |
|---|---|
| 生成符号不存在 | 图 token 前缀及生成头文件／镜像 |
| 输入交换 | 元数据与 fire_dag 指针顺序 |
| 输出存储被覆盖 | 实际返回字节、填充和生命周期 |
| 图启动但应用输出陈旧 | 完成处理及依赖图时序 |
| 构建成功但契约运行失败 | 计划格式、动态输入和覆盖报告 |
| 重编译后结果不同 | JSON、BIN、任务镜像与 L1 包来自同一匹配构建 |

新增应用时，结合生成产物阅读 [fire_dag 宏](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/scheduler/include/dagfire.h)和[动态输入约定](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/DYNAMIC_APPLICATION_INPUTS.md)。
