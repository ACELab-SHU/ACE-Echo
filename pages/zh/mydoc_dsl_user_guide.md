---
title: DSL 用户指南
layout: echo-doc
guide_reference: true
summary: BAS 语法、数据声明、任务绑定、编译配置、产物与完整示例。
sidebar: mydoc_sidebar_zh
permalink: zh_mydoc_dsl_user_guide.html
section: Documentation
content_status: ACE-Echo 1.0
last_reviewed: '2026-09-13'
lang: zh-CN
translation_url: mydoc_dsl_user_guide.html
keywords: ACE-Echo, Venus, DSL 用户指南
---

BAS 用有向无环图描述任务：节点调用任务，边把任务返回的数据传给后续节点。发布版包含固定版本的 DSL 前端，任务编译使用单独安装的 Venus 定制 LLVM。

## 内置 smoke 中的图
{: #a-graph-from-the-bundled-smoke }

```basic
return_value short sum[32]
return_value short restored[32]
```

示例初始化两个各含 32 个有符号 16 位元素的向量 a、b，再连接两个任务：

```basic
dag dag1 = {
  [sum] = Task_forgeAdd(a, b)
  [restored] = Task_forgeRestore(sum, b)
}
END
```

这里是节选。编译时使用含完整参数数组的 [BAS 文件](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/workloads/5g_lite/tasks/forge_vector_smoke/forge_vector_smoke.bas)和配套任务源码。

## 声明与依赖
{: #declarations-and-dependencies }

| 结构 | 作用 |
|---|---|
| `parameter` | 声明图使用的示例数据 |
| `dag_input` | 声明由上层应用提供的输入 |
| `dfedata` | 按现有 BAS 约定声明数字前端输入 |
| `return_value` | 声明图输出及分配元数据 |
| `dag` | 连接任务调用与返回值 |
| `END` | 结束描述 |

使用 ASCII 标点和 `dfedata` 关键字，不要写成 `dfe_data`。下面给出完整声明形式与数据类型参考。

## 为已配置的 Venus1 后端编译
{: #compile-for-the-configured-venus1-backend }

完成安装后，在 platform/ 下运行：

```bash
./ace-echo --config .ace-echo/host/local.toml compile dag \
  --target forge_vector_smoke --backend .ace-echo/host/backend.json
```

编译在当前运行的隔离前端副本中进行。生成的 JSON、组合 BIN 和任务镜像收集到运行产物中，后续仿真使用这些记录路径。通过 `.ace-echo/host-tools.json` 配置本机工具，不要照抄其他开发者的绝对 LLVM_PATH，或手工混搭 lane/row 配置。

## 分配容量与传输数据
{: #allocation-size-and-transferred-data }

输出声明容量允许大于运行时真实返回量。对于 Venus1 普通任务依赖，RTL 通过 16 位 DMT 长度字段使用生产任务的运行时返回字节数。平台按这一行为处理；消费者声明容量用于提示，不会悄悄截短搬运。超过 65535 字节时必须考虑字段截断。

这不意味着重叠的存活内存区域也是安全的。精确行为见[传输长度约定](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/CONSUMER_DMA_LENGTH.md)和[可选外部 BAS 参数](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/BAS_PARAMETER_INPUTS.md)。

## 数据类型参考
{: #data-type-reference }

| BAS 类型 | 元素存储 | 典型用途 |
|---|---|---|
| `char` | 1 字节 | 比特、字节、8 位任务向量 |
| `short` | 2 字节 | 16 位任务向量及标量容器 |
| `int` | 4 字节 | 整数数据／应用元数据 |
| `float` | 4 字节 | 前端浮点数据 |
| `double` | 8 字节 | 前端双精度数据 |

这些类型描述 DSL 数据表示；Venus1 向量算术支持 8、16 位元素。声明 float 数组不会自动获得 Venus1 浮点向量指令。

BAS 标量参数通常写成单元素数组，如 `parameter short mode = {15}`。要匹配生成的任务接口：有些任务使用 64 字节对齐的 short_struct，而不是直接传 C short。元素数量和 ABI 容器大小是不同的量。

## 声明语法示例
{: #declaration-syntax-with-examples }

### 静态参数
{: #static-parameter }

```basic
parameter char bits = {1,0,1,1,0,0,1,0}
parameter short samples = {-3,-1,1,3}
parameter short mode = {15}
```

`parameter TYPE NAME = {VALUES}` 嵌入初始化数据，元素数由初始化列表决定。使用十进制字面量、ASCII 逗号，列表末尾不加逗号。负数在数字前加负号，声明末尾不加分号。

### 应用输入与数字前端输入
{: #application-and-digital-front-end-inputs }

```basic
dag_input short nCellid[1]
dfedata char iq[4420]
```

`dag_input TYPE NAME[CAPACITY]` 声明应用输入；`dfedata TYPE NAME[CAPACITY]` 声明数字前端输入。它们给出存储和绑定，不给出样本值。运行时通过匹配的启动程序／输入约定提供数据。参数顺序以生成元数据为准，不要假设就是声明顺序。

### 图返回值
{: #graph-returns }

```basic
return_value short subFrameNum[1]
return_value char decoded[256]
```

这些变量必须由图中的任务产生。方括号按元素预留容量，不要求任务返回同样多的字节。例如 decoded[256] 可承接更短的有效载荷，实际长度由 vreturn 决定。

### 全局变量
{: #globals }

```basic
global short mode = 1
```

前端也接受 `global TYPE NAME` 以及整数初始化形式。全局变量的分配和生命周期与任务结果不同。普通独立示例优先用 parameter 或任务结果；需要复现依赖全局变量的应用时再使用 global。

### 名称、注释与换行
{: #names-comments-and-line-endings }

```basic
' This whole line is a comment.
parameter short gain = {2} ' This is an end-of-line comment.
```

名称以字母或下划线开始，后接字母、数字或下划线，避免使用 data 等保留关键字。单引号到行尾为注释，支持整行与行尾注释；声明前的单引号会把声明本身注释掉。END 后保留最终换行。

## 任务调用与依赖绑定
{: #task-calls-and-dependency-bindings }

```basic
dag dag1 = {
    [sum] = Task_forgeAdd(a, b)
    [restored] = Task_forgeRestore(sum, b)
}
END
```

函数名选择任务。右侧参数顺序对应 C 任务签名，左侧名称顺序对应 `vreturn(buffer, bytes, ...)` 的返回对。后续任务引用已产生的名称便形成依赖。中间变量由任务结果引入，不必全部声明为图输出。

多个输出的**语法示意**如下：

```basic
dag dag1 = {
    [payload, status] = Task_decode(samples, mode)
    [result] = Task_consume(payload, status)
}
END
```

Task_decode、Task_consume 仅为示意名称，编译前需提供匹配源码。不要像旧快速入门那样重复声明同名 dag dag1。

解析器还支持其他后端使用的指针形式和 SPMD pragma。Venus1 清单并未确认指针输入描述符和 SPMD 任务容器字段的支持，入门应先使用普通值依赖；语法能解析不代表硬件能执行。

## 完整工作示例
{: #complete-working-example }

[Venus 用户指南](zh_mydoc_venus_user_guide.html#complete-two-task-example)包含两个 C 文件和完整 forge_vector_smoke.bas（含全部 32 元素输入）。预期输出为 `sum[i]=15-i`、`restored[i]=i-16`，对应发布版内置编译目标和参考文件。

## 编译配置参考
{: #compiler-configuration-reference }

原有底层配置参考仍可用于排查编译问题。ACE-Echo 1.0 根据本机与后端配置生成运行目录中的值，使用[快速开始](zh_venus1_get_started.html)中的命令统一选择。

| 变量／工具 | 含义 | Venus1 使用方式 |
|---|---|---|
| `LLVM_PATH` | 定制 LLVM 二进制目录 | 使用后端配套编译器 |
| `RVPATH` | RISC-V GCC 根目录 | 提供目标工具链与运行库前提 |
| `TARGET_DAG` | 待编译工作负载 | 由 CLI --target 选择 |
| `CC`、`OPT`、`LLC` | clang、opt、llc | C → IR → 优化 IR → 目标汇编 |
| `DUMP`、`CPY` | llvm-objdump、llvm-objcopy | 检查／提取产物 |
| `PYTHON` | Python 解释器 | 执行 DSL 各阶段 |
| `RM` | 删除命令 | Makefile 内部清理工具 |
| `VENUSLANE`、`VENUSROW` | 向量硬件几何 | 当前配置为 64 lane、512 row |
| `VENUS_VRFADDR` | 向量寄存器文件基地址 | 由后端决定，不是可移植应用常量 |
| `VenusInputStructAddr` | 输入描述符基地址 | 与 ABI 一致 |
| `TEMP_VRF_BASE_ADDR`、`TEMP_VRF_TOTAL_BYTES` | 临时分配区域 | 与内存布局一致 |
| `TEMP_ALLOC_ALIGN_BYTES` | 临时分配对齐 | 后端规定 |

### 编译与优化选项
{: #compiler-and-optimization-flags }

| 组别 | 关键选项 | 用途 |
|---|---|---|
| 目标 | `--target=riscv32-unknown-elf`、`-march=...zvenus` | 选择 Venus 定制目标 |
| 前端 | `-mllvm --venus`、`--venus-nr-row`、`--venus-nr-lane` | 提供几何信息 |
| IR 生成 | `-S -emit-llvm -Xclang -disable-O0-optnone` | 生成自定义 pass 所需 IR |
| 优化 | `-passes=mem2reg,venusplit` | 标量提升及 Venus 操作拆分 |
| ABI | `--venus-istruct-baseaddr` | 输入描述符位置 |
| 代码生成 | `-O3`、`-regalloc=basic`、`--venus-vrf-baseaddr` | 优化和向量分配 |
| 反汇编 | `-d --mattr=+m,+zvenus -M no-aliases` | 查看自定义指令 |

排查时使用成功运行记录中的准确命令。旧 config.mk 包含不同 lane 数和地址映射的历史默认值，整体照抄不等于选择 Venus1。原子扩展选项及附加 pass 也可能随版本改变。

## 生成产物与排查
{: #generated-artifacts-and-troubleshooting }

| 产物 | 用途 |
|---|---|
| DAG JSON | 图、变量绑定、输入输出描述符及调度信息 |
| 组合 DAG BIN | 运行时使用的图数据 |
| 各任务 IR／汇编／HEX | 检查编译结果和任务镜像 |
| 运行配置与 manifest | 复现编译器、源码和后端 |
| 参数元数据 | 追溯本次使用的输入 |

旧版路径为 final_output/&lt;dag&gt;.json、bin/&lt;dag&gt;.bin、venus_test/ir/&lt;task&gt;.hex。平台中应使用运行产物记录的路径，实际目录层级可能不同。

| 现象 | 检查方向 |
|---|---|
| 未知 builtin／向量类型 | 配套头文件、类型定义和 Venus 编译器 |
| 声明处解析失败 | ASCII 标点、关键字、注释及换行 |
| 输入输出不匹配 | C 参数、BAS 绑定、返回顺序和字节数 |
| 分配／重叠警告 | 存活缓冲区、真实返回量和内存布局 |
| 缺少运行时输入 | dag_input／dfedata 值和启动程序约定 |
| 尾部多出意外数据 | 容量、初始化字节与 vreturn 长度的区别 |

## 外部参数文件
{: #external-parameter-files }

不修改图结构即可用单独文件改变输入：

```basic
parameter short gain = {3}
```

对于实际引用 gain 的目标，使用：

```bash
./ace-echo --config .ace-echo/host/local.toml compile dag \
  --target YOUR_DAG --params /path/to/case.params \
  --backend .ace-echo/host/backend.json
```

外部值覆盖同名静态参数，类型需一致；显式 --params 优先于 dag-source.json 默认值。运行时输入仍保留 dag_input／dfedata 声明，启动程序需使用生成的输入约定。可用字面量范围、运行时完整输入要求和初始化行为见[参数文档](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/BAS_PARAMETER_INPUTS.md)。

源码依据：[词法分析器](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/toolchain/dsl/ply/basiclex.py)、[解析器](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/toolchain/dsl/ply/basparse.py)、[构建配置](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/toolchain/dsl/config.mk)、[Venus1 后端](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/configs/backends/venus1p0-64x512-300mhz.json)。
