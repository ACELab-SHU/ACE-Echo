---
title: Venus 用户指南
layout: echo-doc
guide_reference: true
summary: 完整向量类型、原有指令分类、掩码、内存访问和任务示例参考。
sidebar: mydoc_sidebar_zh
permalink: zh_mydoc_venus_user_guide.html
section: Documentation
content_status: Venus1 编程参考
last_reviewed: '2026-09-13'
lang: zh-CN
translation_url: mydoc_venus_user_guide.html
keywords: ACE-Echo, Venus, Venus 用户指南
---

这份指南供编写 Venus C 任务时随时查阅，包含原有指令分类、调用形式、参数说明与示例，以及完整的任务和 BAS 文件。安装环境请看[快速开始](zh_venus1_get_started.html)，图编译请看 [DSL 用户指南](zh_mydoc_dsl_user_guide.html)。

[基本指令](#base-intrinsics) · [扩展指令](#extended-intrinsics) · [完整示例](#complete-two-task-example) · [原版 CRC 示例](#crc-example-from-the-original-guide)

本指南面向 Venus1 的 64 lane / 512 row 配置及配套 LLVM 15.0.7 编译器（版本 `27fdd688`）。调用形式已对照发布版头文件与编译器，运算说明以公开 Gem5 实现为依据。已有 DAG 回归并不等于对每条指令及所有边界情况都完成了芯片验证。

## 数据类型与长度
{: #data-types-and-lengths }

```c
#include "venus.h"

__v1024i16 samples;  // 1024 elements, 16 bits each: 2048 bytes
__v512i8 bits;      // 512 elements, 8 bits each: 512 bytes
__v32i16 small;     // 32 elements, 16 bits each: 64 bytes
```

`__vNiW` 表示 **N 个元素，每个元素 W 位**。配套 `venustype.h` 提供向量类型定义。若需要的名称未生成，可使用编译器的向量扩展定义：

```c
typedef short __v96i16 __attribute__((ext_vector_type(96)));
```

| 数量 | 单位 | 示例 |
|---|---|---|
| 向量类型长度 | 元素 | `__v32i16` 包含 32 个元素 |
| 指令 `length` / AVL | 参与运算的元素数 | `vadd(a, b, MASKREAD_OFF, 32)` |
| BAS `[capacity]` | 声明类型的元素数 | `return_value short sum[32]` |
| `vreturn` 长度 | **字节** | `vreturn(sum, 64)` |
| 地址增量 | 字节 | 第 i 个 16 位元素位于 `base + 2*i` |

Venus1 的向量算术使用 8 位、16 位元素。较大的 C 向量类型仍需与其他存活向量一起满足硬件存储限制。旧文档“任意长度，最大 65535”的表述并不代表一定能分配相应容量。

## 指令调用约定
{: #intrinsic-call-conventions }

```c
out = vadd(a, b, MASKREAD_OFF, length);
out = vseq(a, b, MASKREAD_OFF, MASKWRITE_OFF, length);
out = vmuladd(a, b, c, MASKREAD_OFF, length);
```

- `a` 是向量。普通二元运算中的 `b` 可为兼容向量，或该 builtin 支持的标量形式。
- `length` 的单位是元素。上述算术 builtin 省略长度时，配套编译器使用第一个向量声明的元素数。局部向量运算建议显式填写长度。
- `MASKREAD_OFF` / `MASKREAD_ON` 控制是否读取掩码；比较指令还需指定 `MASKWRITE_OFF` / `MASKWRITE_ON`。
- 普通算术返回向量值。`vclaim`、`vrange`、`vbrdcst`、`vshuffle` 宏直接更新第一个参数，应作为独立语句调用。

**操作数顺序很重要。** 配套 Venus1 编译器把第一个 C 向量参数映射为 `vs2`，第二个映射为 `vs1` 或标量操作数。命名不总是遵循常见 C 运算顺序：`vrsub(a,b)` 计算 `a-b`，`vsub(a,b)` 计算 `b-a`。下表统一以 C 参数名说明。

参考章节中的代码片段假定 `a`、`b`、`c`、`out` 类型兼容，且 `length` 有效。它们是用法片段；页面后部提供完整可编译任务。

## 基本指令
{: #base-intrinsics }

### 加法 — vadd、vsadd、vsaddu
{: #addition--vadd-vsadd-vsaddu }

| 调用 | 逐元素运算 |
|---|---|
| `vadd(a,b,mr,n)` | `a + b`，保留元素位宽 |
| `vsadd(a,b,mr,n)` | 有符号饱和加法 |
| `vsaddu(a,b,mr,n)` | 无符号饱和加法 |

```c
out = vadd(a, b, MASKREAD_OFF, length);
out = vsadd(a, 10, MASKREAD_OFF, length);
out = vsaddu(a, b, MASKREAD_OFF, length);
```

8 位有符号数 `120 + 20` 经 `vsadd` 饱和为 `127`；8 位无符号数 `250 + 10` 经 `vsaddu` 饱和为 `255`。普通加法保留结果的低位，不做饱和处理。

### 减法 — vrsub、vsub、vssub、vssubu
{: #subtraction--vrsub-vsub-vssub-vssubu }

| 调用 | 逐元素运算 |
|---|---|
| `vrsub(a,b,mr,n)` | `a - b` |
| `vsub(a,b,mr,n)` | `b - a` |
| `vssub(a,b,mr,n)` | 有符号饱和 `b - a` |
| `vssubu(a,b,mr,n)` | 无符号饱和 `b - a` |

```c
out = vrsub(a, b, MASKREAD_OFF, length);  // a - b
out = vsub(a, b, MASKREAD_OFF, length);   // b - a
out = vssub(a, b, MASKREAD_OFF, length);
out = vssubu(a, b, MASKREAD_OFF, length);
```

当 `a=7`、`b=2` 时，`vrsub` 得到 `5`，`vsub` 得到 `-5`。发布版恢复任务使用 `vrsub(sum,b)`，从 `sum=a+b` 恢复输入 `a`。

### 乘法 — vmul、vmulh、vmulhu、vmulhsu
{: #multiplication--vmul-vmulh-vmulhu-vmulhsu }

移位配置为零时，令 W 为元素位宽：

| 调用 | 乘积及截取部分 |
|---|---|
| `vmul(a,b,mr,n)` | 乘积的低 W 位 |
| `vmulh(a,b,mr,n)` | 有符号 × 有符号乘积的高 W 位 |
| `vmulhu(a,b,mr,n)` | 无符号 × 无符号乘积的高 W 位 |
| `vmulhsu(a,b,mr,n)` | **有符号 a × 无符号 b** 乘积的高 W 位 |

```c
vsetshamt(0);
out = vmul(a, b, MASKREAD_OFF, length);
out = vmulh(a, b, MASKREAD_OFF, length);
out = vmulhu(a, b, MASKREAD_OFF, length);
out = vmulhsu(a, b, MASKREAD_OFF, length);
```

旧指南把 `vmulhu`、`vmulhsu` 错写为取低位。高位截取和混合符号语义会影响定点计算。移位与饱和设置也会影响结果，详见下方配置章节。

### 除法与余数 — vdiv、vdivu、vrem、vremu
{: #division-and-remainder--vdiv-vdivu-vrem-vremu }

移位配置为零时：

| 调用 | 逐元素运算 |
|---|---|
| `vdiv(a,b,mr,n)` | 有符号 `b / a` |
| `vdivu(a,b,mr,n)` | 无符号 `b / a` |
| `vrem(a,b,mr,n)` | 有符号余数 `b % a` |
| `vremu(a,b,mr,n)` | 无符号余数 `b % a` |

```c
vsetshamt(0);
out = vdiv(a, b, MASKREAD_OFF, length);  // a is the denominator
out = vrem(a, b, MASKREAD_OFF, length);
```

正数 `a=3`、`b=10` 的商和余数分别为 `3`、`1`。模型中的除法路径也使用移位配置，并将商限制在元素范围内。入门样例使用非零分母；算法若依赖除零、溢出行为，应专门验证相应情况。

### 位运算 — vand、vor、vxor
{: #bitwise-operations--vand-vor-vxor }

```c
out = vand(a, b, MASKREAD_OFF, length);  // a & b
out = vor(a, b, MASKREAD_OFF, length);   // a | b
out = vxor(a, b, MASKREAD_OFF, length);  // a ^ b
out = vand(a, 15, MASKREAD_OFF, length); // retain the low four bits
```

各元素分别按位运算。例如 `12 & 10 = 8`、`12 | 10 = 14`、`12 ^ 10 = 6`。

### 移位 — vsll、vsrl、vsra
{: #shifts--vsll-vsrl-vsra }

```c
out = vsll(a, 2, MASKREAD_OFF, length);  // left shift
out = vsrl(a, 2, MASKREAD_OFF, length);  // logical right shift, fill with zero
out = vsra(a, 2, MASKREAD_OFF, length);  // arithmetic right shift, extend sign
```

第二个参数提供移位量。W 位元素通常使用 `[0,W-1]` 内的移位量，超范围行为需要单独验证。`vsrl` 逻辑右移、补零；`vsra` 算术右移、保留符号；`vsll` 左移。

### 比较 — vseq、vsne、vslt、vsle、vsgt
{: #comparisons--vseq-vsne-vslt-vsle-vsgt }

比较调用必须包含两个掩码控制参数。无符号变体对无符号元素执行同样的关系判断：

| 指令 | 以 C 参数 a、b 表示的关系 |
|---|---|
| `vseq` | `b == a` |
| `vsne` | `b != a` |
| `vslt`、`vsltu` | `b < a` |
| `vsle`、`vsleu` | `b <= a` |
| `vsgt`、`vsgtu` | `b > a`，是**严格大于**，不是 `>=` |

```c
out = vseq(a, b, MASKREAD_OFF, MASKWRITE_OFF, length);
out = vslt(a, b, MASKREAD_OFF, MASKWRITE_OFF, length);
out = vsgtu(a, b, MASKREAD_OFF, MASKWRITE_OFF, length);
```

`MASKWRITE_OFF` 时，比较位置产生向量谓词；`MASKWRITE_ON` 时，使用调用写入掩码，不消费普通向量返回值：

```c
vseq(a, b, MASKREAD_OFF, MASKWRITE_ON, length);
out = vadd(a, 1, MASKREAD_ON, length);
```

这里仅对 a 与 b 相等的位置加一。上述普通二元算术路径在读取掩码时，未选中位置沿用第一个向量 `a`。归约和 shuffle 有自己的数据流行为，不应直接套用这一规则。

### 复合运算 — vmuladd、vmulsub、vaddmul、vsubmul
{: #composite-operations--vmuladd-vmulsub-vaddmul-vsubmul }

| 指令 | 截位、移位与饱和处理前的代数形式 |
|---|---|
| `vmuladd(a,b,c,mr,n)` | `(a*b) + c` |
| `vmulsub(a,b,c,mr,n)` | `(a*b) - c` |
| `vaddmul(a,b,c,mr,n)` | `(b+a) * c` |
| `vsubmul(a,b,c,mr,n)` | `(b-a) * c` |

```c
out = vmuladd(a, b, c, MASKREAD_OFF, length);
out = vmulsub(a, b, c, MASKREAD_OFF, length);
out = vaddmul(a, b, c, MASKREAD_OFF, length);
out = vsubmul(a, b, c, MASKREAD_OFF, length);
```

这些是固定位宽数据通路操作。中间结果的截位及各级独立控制，意味着它们不一定等价于无限精度 C 表达式计算完毕后仅做一次强制类型转换。

### 复数乘法 — vcmxmul
{: #complex-multiplication--vcmxmul }

```c
__v32i16 result_re, result_im;
vclaim(result_re, 32);
vclaim(result_im, 32);

vcmxmul(&result_re, &result_im,
        a_re, a_im, b_re, b_im, MASKREAD_OFF, 32);
```

前两个参数按顺序为**实部输出指针、虚部输出指针**，接下来依次为 **a 实部、a 虚部、b 实部、b 虚部**。它表示 `(a_re + i*a_im) * (b_re + i*b_im)`，同时受定点配置影响。各分量使用相同元素位宽与兼容容量。旧示例的实虚部名称顺序有误，新代码应使用上述顺序。

## 扩展指令
{: #extended-intrinsics }

### 聚集与分散 — vshuffle
{: #gather-and-scatter--vshuffle }

```c
__v32i16 index;
vclaim(index, 32);
vrange(index, 32);

vshuffle(out, index, a, SHUFFLE_GATHER, 32);
vshuffle(out, index, a, SHUFFLE_SCATTER, 32);
```

| 模式 | 每个有效索引 i 对应的数据搬移 |
|---|---|
| `SHUFFLE_GATHER` | `out[i] = a[index[i]]` |
| `SHUFFLE_SCATTER` | `out[index[i]] = a[i]` |

索引为 **16 位元素索引**，即使数据向量本身是 8 位也如此。长度表示要处理的索引条目数。源和目标索引都应落在各自分配区域内；未写入但后续会读取的目标位置需先初始化。scatter 尽量避免重复目标索引，除非已经验证所需冲突行为。

当前头文件将 `vshuffle` 映射到 `__Venus_shuffle_test`，应用仍使用公开宏 `vshuffle`。

### 索引生成 — vrange
{: #index-generation--vrange }

```c
__v32i16 index;
vclaim(index, 32);
vrange(index, 32);                         // 0,1,...,31
index = vadd(index, 8, MASKREAD_OFF, 32); // 8,9,...,39
```

`vrange` 在前 length 个位置生成从零开始的连续索引，应使用 16 位向量。上例加 8 后得到 8…39；随后访问的数据分配必须覆盖索引 39。

### 广播 — vbrdcst
{: #broadcast--vbrdcst }

```c
vbrdcst(out, 0, MASKREAD_OFF, length);
vbrdcst(out, 7, MASKREAD_ON, length);
```

广播将标量写入选中的向量元素。第二个参数必须是标量，不能是另一向量。宏直接更新 out，不能写成 `out = vbrdcst(...)`。

### 定点配置 — vsetshamt 与 vsetcsr
{: #fixed-point-configuration--vsetshamt-and-vsetcsr }

```c
vsetshamt(7);
out = vmul(a, b, MASKREAD_OFF, length);
vsetshamt(0);  // restore the setting expected by subsequent operations
```

对于 `vmul`，乘法模型先把乘积右移，再截取低位；高位乘法则在配置的左移后截取高 W 位。以 8 位乘积和移位量 s 为例，可理解为 `low8(product >> s)` 与 `high8(product << s)`。除法同样使用该设置，因此它不只影响一条乘法指令。

| 辅助宏 | 头文件映射 | 用途 |
|---|---|---|
| `vsetshamt(value)` | `vsetcsr(VCSR_MULSHAMT,value)` | 算术移位配置 |
| `vsetsaturate(value)` | `vsetcsr(VCSR_MULSATURATE,value)` | 算术饱和控制位 |
| `vsetmsbhead(value)` | `vsetcsr(VCSR_MSBHEAD,value)` | 向量地址／头部配置 |
| `vsetLSUmsb(value)` | `vsetcsr(VCSR_MSBLSU,value)` | LSU 地址配置 |
| `vsetEXTENSIONcfg(value)` | `vsetcsr(VCSR_EXTCFG,value)` | 扩展配置 |

这些配置有状态。后几项需使用与硬件及后端匹配的示例值，不能都当成简单布尔开关。头文件列出了寄存器标识，并未给出完整位域规范。

### 存储声明 — vclaim
{: #storage-declaration--vclaim }

```c
__v32i16 scratch;
vclaim(scratch, 32);
vbrdcst(scratch, 0, MASKREAD_OFF, 32);
```

`vclaim` 告知编译器保留向量存储及存活关系，**不初始化数据**，也不是运行时堆分配。向量用于就地更新或标量访问时，通常在声明后调用。长度单位为元素；旧形式 `vclaim(scratch)` 使用完整声明长度。

### 地址与标量访问 — vaddr、vbarrier、VSPM
{: #address-and-scalar-access--vaddr-vbarrier-vspm }

```c
#include "venus.h"
#include "riscv_printf.h"

// Fragment inside a task, after samples has been produced.
unsigned int address = (unsigned int)vaddr(samples);
vbarrier();
VSPM_OPEN();
short first = *(volatile short *)address;
*(volatile short *)(address + 2) = first; // second 16-bit element
VSPM_CLOSE();
```

`vaddr` 返回向量基地址。`vbarrier()` 提供标量访问前使用的编译器／硬件同步操作。配套 `riscv_printf.h` 定义访问窗口宏 `VSPM_OPEN()`、`VSPM_CLOSE()`。将打开、访问、关闭操作保持在一起，使用元素宽度正确的 volatile 访问。

8 位向量使用 `volatile unsigned char *` 和 `base+i`；16 位向量使用 `volatile short *` 和 `base+2*i`。不要像旧 CRC 示例那样在读取前把 OPEN 错写成 CLOSE。这些宏依赖硬件地址映射，不能作为普通主机 C 代码运行。

### 掩码取反 — vmnot
{: #mask-inversion--vmnot }

旧接口形式为 `vmnot(mask_reg)`，可选长度参数。发布的 `venus.h` 中该包装宏已被注释，但 Venus1 编译器仍列有底层 `__Venus_mnot` builtin。因此，配套头文件**并未提供可直接使用的 vmnot 公开宏**。此处保留说明便于阅读旧代码；新代码可直接构造所需比较谓词，或使用另行验证的头文件／builtin 集成。

### 任务返回 — vreturn
{: #task-returns--vreturn }

```c
vreturn(out, length * sizeof(short)); // length 16-bit elements
```

参数按**缓冲区、字节数**成对排列。多个返回值的例子：

```c
typedef struct { short data; } __attribute__((aligned(64))) short_struct;
short_struct status = {0};
vreturn(out, 64, &status, sizeof(status));
```

先从 out 返回 64 字节，再返回对齐的标量容器。`sizeof(status)` 包含对齐填充，并不等于 `sizeof(short)`。BAS 中的输出绑定顺序需与这些返回对一致。

类型容量、BAS 分配和运行时返回长度是不同量。BAS 容量可以有意大于实际数据。普通 Venus1 依赖搬运把运行时字节数放入 16 位 DMT 长度字段；超长数据及存活内存重叠仍需正确规划。详见[完整传输约定](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/CONSUMER_DMA_LENGTH.md)。

### 归约 — 最小值、最大值、求和及逻辑归约
{: #reductions--minimum-maximum-sum-and-logic }

当前头文件为有符号 min/max 提供了区分位宽的辅助宏：

| 调用 | 结果 |
|---|---|
| `vredmin8(a,mr,n)`、`vredmin16(a,mr,n)` | 有符号最小值，使用对应的 8/16 位初值 |
| `vredmax8(a,mr,n)`、`vredmax16(a,mr,n)` | 有符号最大值，使用对应初值 |
| `vredminu(a,mr,n)`、`vredmaxu(a,mr,n)` | 无符号最小值／最大值 |
| `vredsum(a,mr,n)` | 求和 |
| `vredand(a,mr,n)`、`vredor(a,mr,n)`、`vredxor(a,mr,n)` | 按位归约 |

```c
__v32i16 minimum, maximum, total;
minimum = vredmin16(a, MASKREAD_OFF, 32);
maximum = vredmax16(a, MASKREAD_OFF, 32);
total = vredsum(a, MASKREAD_OFF, 32);
```

应读取归约结果缓冲区，不能假设每个 lane 都包含答案。min/max 是位于输出前部的标量结果；sum 使用更宽的累加器与结果表示。字节布局需按配套后端读取，不能笼统套用旧文档“前四个元素”的规则，因为四个 8 位元素与四个 16 位元素的字节数不同。当前头文件不提供通用 `vredmin`、`vredmax` 包装宏，应选择上述位宽对应形式。正确掩码常量是 `MASKREAD_OFF`，不是 `MASKRED_OFF`。

### 其他头文件条目与后端可用性
{: #other-header-entries-and-backend-availability }

共享宏生成器还列有 `vsignset`、`vmin`、`vmax`，头文件也包含 `vload` / `vstore`。这些 builtin 名称不在配套 Venus1 编译器表中，不能仅因宏存在就认为 Venus1 可用。`vpseudo` 面向编译器内部用途，本文不提供通用应用写法。`vsync` 将同步与 `wfi` 组合，使用者必须具备匹配的唤醒协议。

## 完整双任务示例
{: #complete-two-task-example }

以下来源于发布版 `forge_vector_smoke`。每个 C 文件包含 venus.h，并按标题所示名称保存。

### Task_forgeAdd.c
{: #task_forgeaddc }

```c
#include "venus.h"

int Task_forgeAdd(__v32i16 a, __v32i16 b) {
    __v32i16 sum;
    vclaim(sum, 32);
    sum = vadd(a, b, MASKREAD_OFF, 32);
    vreturn(sum, 64);
    return 0;
}
```

### Task_forgeRestore.c
{: #task_forgerestorec }

```c
#include "venus.h"

int Task_forgeRestore(__v32i16 sum, __v32i16 b) {
    __v32i16 restored;
    vclaim(restored, 32);
    restored = vrsub(sum, b, MASKREAD_OFF, 32);
    vreturn(restored, 64);
    return 0;
}
```

### forge_vector_smoke.bas
{: #forge_vector_smokebas }

```basic
return_value short sum[32]
return_value short restored[32]
parameter short a = {-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}
parameter short b = {31,29,27,25,23,21,19,17,15,13,11,9,7,5,3,1,-1,-3,-5,-7,-9,-11,-13,-15,-17,-19,-21,-23,-25,-27,-29,-31}
dag dag1 = {
    [sum] = Task_forgeAdd(a, b)
    [restored] = Task_forgeRestore(sum, b)
}
END
```

对 i=0…31，`a[i]=i-16`、`b[i]=31-2*i`，因此 `sum[i]=15-i`，`restored[i]=a[i]`。这种不对称输入能识别减法参数反向。两项返回均含 32 个有符号 16 位元素，即 64 字节。

[配置好发布环境](zh_venus1_get_started.html)后，在 platform/ 下编译内置目标：

```bash
./ace-echo --config .ace-echo/host/local.toml compile dag \
  --target forge_vector_smoke --backend .ace-echo/host/backend.json
```

运行步骤见[编程模型](zh_mydoc_programming_model.html)。完整源码目录还包括参考输出相关文件，复现时请一并保留。

## 原版指南中的 CRC 示例
{: #crc-example-from-the-original-guide }

原来的 Task_nrCRC 展示了索引生成、gather/scatter、异或、比较、归约和标量访问。但输入准备、RNTI 掩码依赖具体算法，不能作为通用 CRC 标准参考。完整旧示例仍保留在本页，迁移说明列于代码之前。

<details class="guide-archive" markdown="1">
<summary>展开原版 CRC 任务及迁移说明</summary>

- 原示例读取结果前用了 `VSPM_CLOSE()`，应先修正为正确的访问窗口序列。
- 在配套类型头文件中定义 `short_struct`，并确认归约结果位宽后再转换为状态值。
- `sizeof(buf)` 返回完整声明容量，可能包含未初始化尾部。新实现应返回消费者实际需要的已初始化字节。
- 下方是原始算法例子，不是当前通过的 smoke，也不能证明任意多项式与输入的 CRC 正确性。

```c
#include "data_type.h"
#include "riscv_printf.h"
#include "venus.h"

typedef short __v2048i16 __attribute__((ext_vector_type(2048)));
typedef char  __v4096i8 __attribute__((ext_vector_type(4096)));

/**
 * @section DESCRIPTION
 * This Task checks the input data vector for a CRC error for 5G New Radio (NR)
 * physical channels as specified in 3GPP TS 38.212 .
 *
 * Features
 * - Supports all 5G NR CRC polynomials (CRC24A, CRC24B, CRC24C, CRC16, CRC11, CRC6)
 *
 * @param[in]     tmp_vin            : A 4096i8 vector for storing the input bit sequence.
 * @param[in]     in_fullLen         : A short struct for storing length of input data in bits.
 * @param[in]     in_pariLen         : A short struct for storing length of CRC data in bits.
 * @param[in]     poly               : A 4096i8 vector (stored table in bas) for storing the CRC generation polynomial.
 * @param[out]    out_crc_result     : A short struct for storing error detection result.
 * @param[out]    buf                : A 4096i8 vector for storing calculated CRC value (optional).
 *
 */

int Task_nrCRC(__v4096i8 tmp_vin, short_struct in_fullLen, short_struct in_pariLen, __v4096i8 poly) {
  int fullLen = in_fullLen.data + 24;
  int pariLen = in_pariLen.data + 1;
  int msgLen  = fullLen - pariLen + 1;
  int tmp;

  __v4096i8 vin;
  vclaim(vin);
  vbrdcst(vin, 1, MASKREAD_OFF, fullLen);

  __v2048i16 vin_shuffle_index;
  vclaim(vin_shuffle_index);
  vrange(vin_shuffle_index, fullLen);
  vin_shuffle_index = vsadd(vin_shuffle_index, 24, MASKREAD_OFF, fullLen);

  vshuffle(vin, vin_shuffle_index, tmp_vin, SHUFFLE_SCATTER, fullLen);

  __v4096i8  buf;
  __v4096i8  msg;
  __v2048i16 index;
  vclaim(buf);
  vclaim(msg);
  vclaim(index);

  vrange(index, msgLen);
  vbrdcst(msg, 0, MASKREAD_OFF, fullLen);
  vshuffle(msg, index, vin, SHUFFLE_GATHER, msgLen);

  for (int i = 0; i < msgLen; i++) {
    int m_addr = vaddr(msg);
    vbarrier();
    VSPM_OPEN();
    unsigned int addr = m_addr + i;
    tmp               = *(volatile unsigned char *)(addr);
    VSPM_CLOSE();

    if (tmp == 1) {
      vrange(index, pariLen);
      index = vsadd(index, i, MASKREAD_OFF, pariLen);
      vshuffle(buf, index, msg, SHUFFLE_GATHER, pariLen);
      buf = vxor(buf, poly, MASKREAD_OFF, pariLen);
      vshuffle(msg, index, buf, SHUFFLE_SCATTER, pariLen);
    }
  }

  vrange(index, pariLen - 1);
  index = vsadd(index, msgLen, MASKREAD_OFF, pariLen - 1);
  vshuffle(buf, index, msg, SHUFFLE_GATHER, pariLen - 1);

  __v4096i8 si_rnti;
  vclaim(si_rnti);
  vbrdcst(si_rnti, 1, MASKREAD_OFF, pariLen - 1);
  vbrdcst(si_rnti, 0, MASKREAD_OFF, 8);
  buf = vxor(buf, si_rnti, MASKREAD_OFF, pariLen - 1);

  __v2048i16 shuffle_index;
  vclaim(shuffle_index);
  vrange(shuffle_index, pariLen);
  shuffle_index = vsadd(shuffle_index, fullLen - pariLen + 1, MASKREAD_OFF, pariLen - 1);

  __v4096i8 compare;
  vclaim(compare);
  vshuffle(compare, shuffle_index, vin, SHUFFLE_GATHER, pariLen - 1);

  __v4096i8 compare_result;
  compare_result = vsne(buf, compare, MASKREAD_OFF, MASKWRITE_OFF, pariLen - 1);

  compare_result          = vredsum(compare_result, MASKREAD_OFF, pariLen - 1);
  int compare_result_addr = vaddr(compare_result);
  vbarrier();
  VSPM_CLOSE();
  int crc_result = *(volatile unsigned char *)(compare_result_addr);
  VSPM_CLOSE();

  short_struct out_crc_result;
  out_crc_result.data = crc_result;

  vreturn(buf, sizeof(buf), &out_crc_result, sizeof(out_crc_result));
}
```

</details>

## 源码参考与迁移检查
{: #source-reference-and-migration-checklist }

- [公开宏与控制常量](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/toolchain/dsl/venus_test/venus.h)
- [算术／比较宏生成表](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/toolchain/dsl/venus_test/gen_venusbuiltin_h.py)
- [发布版 Gem5 算术模型](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/gem5/src/venus/venus_vfu.cc)
- [Venus1 后端与编译器身份](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/configs/backends/venus1p0-64x512-300mhz.json)
- [完整 smoke 应用](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/workloads/5g_lite/tasks/forge_vector_smoke)

迁移旧任务时，应核对参数顺序、元素长度、返回字节数、实虚部顺序、归约宏名称和标量访问时序。先用有代表性的不对称输入比较输出，再使用时序指标选择优化方案。
