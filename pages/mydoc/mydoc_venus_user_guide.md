---
title: Venus User Guide
layout: echo-doc
guide_reference: true
summary: A practical reference for vector types, every original intrinsic family, masks, memory access and complete task examples.
sidebar: mydoc_sidebar
permalink: mydoc_venus_user_guide.html
section: Documentation
content_status: Venus1 programming reference
last_reviewed: '2026-09-13'
lang: en
translation_url: zh_mydoc_venus_user_guide.html
---

Use this guide while writing Venus C tasks. It contains the instruction families from the original guide, their call forms and examples, followed by task and BAS examples. For setup, see [Get started](venus1_get_started.html); for graph compilation, see the [DSL User Guide](mydoc_dsl_user_guide.html).

[Base intrinsics](#base-intrinsics) · [Extended intrinsics](#extended-intrinsics) · [Complete example](#complete-two-task-example) · [Original CRC example](#crc-example-from-the-original-guide)

The reference targets the Venus1 64-lane / 512-row profile and its LLVM 15.0.7 compiler revision `27fdd688`. Call forms are checked against the release's headers and paired compiler; operation descriptions use the published Gem5 implementation. The recorded DAG regression does not individually qualify every instruction or corner case against silicon.

## Data types and lengths

```c
#include "venus.h"

__v1024i16 samples;  // 1024 elements, 16 bits each: 2048 bytes
__v512i8 bits;      // 512 elements, 8 bits each: 512 bytes
__v32i16 small;     // 32 elements, 16 bits each: 64 bytes
```

`__vNiW` describes **N elements of W bits**. The paired `venustype.h` supplies vector typedefs. When a needed spelling is not generated, the underlying compiler extension is:

```c
typedef short __v96i16 __attribute__((ext_vector_type(96)));
```

| Quantity | Unit | Example |
|---|---|---|
| Vector type length | Elements | `__v32i16` holds 32 elements |
| Intrinsic `length` / AVL | Elements to operate on | `vadd(a, b, MASKREAD_OFF, 32)` |
| BAS `[capacity]` | Elements of the declared type | `return_value short sum[32]` |
| `vreturn` length | **Bytes** | `vreturn(sum, 64)` |
| Address increments | Bytes | A 16-bit element at index `i` is at `base + 2*i` |

Venus1 arithmetic uses 8-bit and 16-bit elements. A large C vector type still has to fit the selected backend's physical storage together with other live vectors. The old “arbitrary length up to 65535” description was not a guarantee of allocatable vector memory.

## Intrinsic call conventions

```c
out = vadd(a, b, MASKREAD_OFF, length);
out = vseq(a, b, MASKREAD_OFF, MASKWRITE_OFF, length);
out = vmuladd(a, b, c, MASKREAD_OFF, length);
```

- `a` is a vector. For ordinary binary operations, `b` can be a compatible vector or a scalar where the builtin supports that form.
- `length` counts elements. When omitted on these arithmetic builtins, the paired compiler uses the first vector's declared element count. Explicit lengths make partial-vector operations easier to review.
- `MASKREAD_OFF` / `MASKREAD_ON` select whether the mask participates. Comparison calls also need `MASKWRITE_OFF` / `MASKWRITE_ON`.
- Ordinary arithmetic returns a vector value. `vclaim`, `vrange`, `vbrdcst` and `vshuffle` macros update their first argument, so use them as statements.

**Operand order matters.** In the paired Venus1 compiler, the first C vector operand maps to `vs2`, and the second to `vs1` or the scalar operand. Venus names do not always follow ordinary C operand order: `vrsub(a,b)` computes `a-b`, while `vsub(a,b)` computes `b-a`. The tables below use C argument names throughout.

The snippets in the reference sections assume compatible vectors `a`, `b`, `c`, `out` and a valid `length`. They are usage fragments. A complete compilable task is provided near the end.

## Base intrinsics

### Addition — vadd, vsadd, vsaddu

| Call | Element operation |
|---|---|
| `vadd(a,b,mr,n)` | `a + b`, retaining the element width |
| `vsadd(a,b,mr,n)` | Signed saturating addition |
| `vsaddu(a,b,mr,n)` | Unsigned saturating addition |

```c
out = vadd(a, b, MASKREAD_OFF, length);
out = vsadd(a, 10, MASKREAD_OFF, length);
out = vsaddu(a, b, MASKREAD_OFF, length);
```

For signed 8-bit data, `120 + 20` saturates to `127` with `vsadd`. For unsigned 8-bit data, `250 + 10` saturates to `255` with `vsaddu`. Plain addition retains the low element-width bits instead of saturating.

### Subtraction — vrsub, vsub, vssub, vssubu

| Call | Element operation |
|---|---|
| `vrsub(a,b,mr,n)` | `a - b` |
| `vsub(a,b,mr,n)` | `b - a` |
| `vssub(a,b,mr,n)` | Signed saturating `b - a` |
| `vssubu(a,b,mr,n)` | Unsigned saturating `b - a` |

```c
out = vrsub(a, b, MASKREAD_OFF, length);  // a - b
out = vsub(a, b, MASKREAD_OFF, length);   // b - a
out = vssub(a, b, MASKREAD_OFF, length);
out = vssubu(a, b, MASKREAD_OFF, length);
```

With `a=7` and `b=2`, `vrsub` produces `5`; `vsub` produces `-5`. The bundled restore task uses `vrsub(sum,b)` to recover `a` from `sum=a+b`.

### Multiplication — vmul, vmulh, vmulhu, vmulhsu

With shift amount zero, `W` is the element width:

| Call | Product and selected part |
|---|---|
| `vmul(a,b,mr,n)` | Low W bits of the product |
| `vmulh(a,b,mr,n)` | High W bits of signed × signed product |
| `vmulhu(a,b,mr,n)` | High W bits of unsigned × unsigned product |
| `vmulhsu(a,b,mr,n)` | High W bits of **signed a × unsigned b** product |

```c
vsetshamt(0);
out = vmul(a, b, MASKREAD_OFF, length);
out = vmulh(a, b, MASKREAD_OFF, length);
out = vmulhu(a, b, MASKREAD_OFF, length);
out = vmulhsu(a, b, MASKREAD_OFF, length);
```

The old guide incorrectly called `vmulhu` and `vmulhsu` low-part operations. Their high-part behavior and mixed signedness matter for fixed-point code. Shift and saturation settings affect the result; see the configuration section below.

### Division and remainder — vdiv, vdivu, vrem, vremu

At shift amount zero:

| Call | Element operation |
|---|---|
| `vdiv(a,b,mr,n)` | Signed `b / a` |
| `vdivu(a,b,mr,n)` | Unsigned `b / a` |
| `vrem(a,b,mr,n)` | Signed remainder `b % a` |
| `vremu(a,b,mr,n)` | Unsigned remainder `b % a` |

```c
vsetshamt(0);
out = vdiv(a, b, MASKREAD_OFF, length);  // a is the denominator
out = vrem(a, b, MASKREAD_OFF, length);
```

For positive `a=3`, `b=10`, quotient and remainder are `3` and `1`. The model's division path also uses the shift configuration and clips the quotient to the element range. Keep denominators nonzero in an introductory example; validate divide-by-zero and overflow cases explicitly when your algorithm relies on them.

### Bitwise operations — vand, vor, vxor

```c
out = vand(a, b, MASKREAD_OFF, length);  // a & b
out = vor(a, b, MASKREAD_OFF, length);   // a | b
out = vxor(a, b, MASKREAD_OFF, length);  // a ^ b
out = vand(a, 15, MASKREAD_OFF, length); // retain the low four bits
```

The operation applies independently to each element's bit pattern. For example, `12 & 10 = 8`, `12 | 10 = 14`, and `12 ^ 10 = 6`.

### Shifts — vsll, vsrl, vsra

```c
out = vsll(a, 2, MASKREAD_OFF, length);  // left shift
out = vsrl(a, 2, MASKREAD_OFF, length);  // logical right shift, fill with zero
out = vsra(a, 2, MASKREAD_OFF, length);  // arithmetic right shift, extend sign
```

The second operand supplies the shift count. Use counts in `[0,W-1]` for W-bit elements unless you have tested the backend's out-of-range behavior. `vsrl` interprets the bit pattern as unsigned; `vsra` preserves the signed interpretation.

### Comparisons — vseq, vsne, vslt, vsle, vsgt

All comparison calls include both mask controls. The unsigned variants use the same relation on unsigned element values:

| Intrinsic | Relation using C arguments `a,b` |
|---|---|
| `vseq` | `b == a` |
| `vsne` | `b != a` |
| `vslt`, `vsltu` | `b < a` |
| `vsle`, `vsleu` | `b <= a` |
| `vsgt`, `vsgtu` | `b > a` — **strictly greater**, not `>=` |

```c
out = vseq(a, b, MASKREAD_OFF, MASKWRITE_OFF, length);
out = vslt(a, b, MASKREAD_OFF, MASKWRITE_OFF, length);
out = vsgtu(a, b, MASKREAD_OFF, MASKWRITE_OFF, length);
```

With `MASKWRITE_OFF`, compared positions produce vector predicates. With `MASKWRITE_ON`, use the call to populate the mask and do not consume an ordinary vector result:

```c
vseq(a, b, MASKREAD_OFF, MASKWRITE_ON, length);
out = vadd(a, 1, MASKREAD_ON, length);
```

Here matching positions of `a` and `b` are incremented. For the ordinary binary arithmetic paths described above, positions disabled by the read mask pass through the first vector `a`. Do not assume this pass-through rule for reductions or shuffle; they have their own dataflow behavior.

### Composite operations — vmuladd, vmulsub, vaddmul, vsubmul

| Intrinsic | Algebraic form before element-width truncation, shift and saturation |
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

These are fixed-width datapath operations. Intermediate narrowing and independently controlled arithmetic stages mean they need not equal an unlimited-precision C expression followed by a single final cast.

### Complex multiplication — vcmxmul

```c
__v32i16 result_re, result_im;
vclaim(result_re, 32);
vclaim(result_im, 32);

vcmxmul(&result_re, &result_im,
        a_re, a_im, b_re, b_im, MASKREAD_OFF, 32);
```

The first two parameters are **real-output pointer, imaginary-output pointer**. The next four are **a real, a imaginary, b real, b imaginary**. It represents `(a_re + i*a_im) * (b_re + i*b_im)`, subject to the configured fixed-point behavior. Use the same vector element width and compatible capacity for all components. The old example swapped real/imaginary names; do not copy that ordering into new code.

## Extended intrinsics

### Gather and scatter — vshuffle

```c
__v32i16 index;
vclaim(index, 32);
vrange(index, 32);

vshuffle(out, index, a, SHUFFLE_GATHER, 32);
vshuffle(out, index, a, SHUFFLE_SCATTER, 32);
```

| Mode | Data movement for each active index i |
|---|---|
| `SHUFFLE_GATHER` | `out[i] = a[index[i]]` |
| `SHUFFLE_SCATTER` | `out[index[i]] = a[i]` |

Indices are **16-bit element indices**, including when the data vector uses 8-bit elements. The length counts index entries being processed. Ensure every referenced source/destination index is within its allocation. Initialize destination positions that are not written but will later be read. For scatter, avoid duplicate destinations unless the required conflict behavior has been verified.

The current header routes `vshuffle` through `__Venus_shuffle_test`; this is the public macro to use, not a reason to rename application calls.

### Index generation — vrange

```c
__v32i16 index;
vclaim(index, 32);
vrange(index, 32);                         // 0,1,...,31
index = vadd(index, 8, MASKREAD_OFF, 32); // 8,9,...,39
```

`vrange` fills the first `length` positions with sequential indices starting at zero. Use a 16-bit vector. The destination in the second example must subsequently index a data allocation covering index 39.

### Broadcast — vbrdcst

```c
vbrdcst(out, 0, MASKREAD_OFF, length);
vbrdcst(out, 7, MASKREAD_ON, length);
```

Broadcast writes a scalar value to the selected vector elements. The second argument is a scalar, not another vector. It updates `out` directly; do not write `out = vbrdcst(...)`.

### Fixed-point configuration — vsetshamt and vsetcsr

```c
vsetshamt(7);
out = vmul(a, b, MASKREAD_OFF, length);
vsetshamt(0);  // restore the setting expected by subsequent operations
```

The multiplication model shifts a product right before selecting its low part for `vmul`. High-part operations select the high W bits after the configured left shift. For an 8-bit product and shift `s`, the conceptual slices are `low8(product >> s)` and `high8(product << s)`. Division also uses this setting, so do not treat it as local to one multiplication call.

| Helper | Header mapping | Purpose |
|---|---|---|
| `vsetshamt(value)` | `vsetcsr(VCSR_MULSHAMT,value)` | Arithmetic shift configuration |
| `vsetsaturate(value)` | `vsetcsr(VCSR_MULSATURATE,value)` | Arithmetic saturation control bits |
| `vsetmsbhead(value)` | `vsetcsr(VCSR_MSBHEAD,value)` | Vector address/head configuration |
| `vsetLSUmsb(value)` | `vsetcsr(VCSR_MSBLSU,value)` | LSU address configuration |
| `vsetEXTENSIONcfg(value)` | `vsetcsr(VCSR_EXTCFG,value)` | Extension configuration |

These are stateful settings. For the latter control fields, reuse the exact values from a matching hardware/backend example; they are not all simple Boolean flags. The header gives register identifiers, not a complete register bit-field specification.

### Storage declaration — vclaim

```c
__v32i16 scratch;
vclaim(scratch, 32);
vbrdcst(scratch, 0, MASKREAD_OFF, 32);
```

`vclaim` tells the compiler to retain vector storage/liveness. It does **not** initialize memory and is not a runtime heap allocation. Place it after the declaration when the storage is used by in-place intrinsics or scalar access. The length is in elements; the older `vclaim(scratch)` form uses the vector's full declared size.

### Address and scalar access — vaddr, vbarrier, VSPM

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

`vaddr` returns the vector's base address. `vbarrier()` provides the compiler/hardware synchronization operation used before scalar access. The paired `riscv_printf.h` defines `VSPM_OPEN()` and `VSPM_CLOSE()` for the access window. Keep the open/access/close sequence together and use a `volatile` access with the correct element width.

For an 8-bit vector use `volatile unsigned char *` and `base+i`; for a 16-bit vector use `volatile short *` and `base+2*i`. Do not replace an access-window **OPEN** with **CLOSE** before a read, as the old CRC example did. These macros are specific to the hardware memory map, not portable host C.

### Mask inversion — vmnot

The old API form was `vmnot(mask_reg)` with an optional length. In the published `venus.h`, the `vmnot` wrapper is commented out, although the Venus1 compiler lists the underlying `__Venus_mnot` builtin. Consequently this spelling is **not an available public macro in the paired header**. It remains documented here so existing code can be understood; for new code, express the needed predicate directly with a comparison, or use a separately verified header/builtin integration.

### Task returns — vreturn

```c
vreturn(out, length * sizeof(short)); // length 16-bit elements
```

The arguments are ordered **buffer, byte count** pairs. For multiple outputs:

```c
typedef struct { short data; } __attribute__((aligned(64))) short_struct;
short_struct status = {0};
vreturn(out, 64, &status, sizeof(status));
```

This returns 64 bytes from `out`, followed by the aligned scalar container. `sizeof(status)` includes alignment padding; it is not simply `sizeof(short)`. The BAS task call must bind outputs in the same order.

The type's capacity, the BAS allocation and the runtime return length are different quantities. A BAS capacity may deliberately exceed the actual result. For ordinary Venus1 dependencies, the runtime byte count passes through the 16-bit DMT length field; oversized or overlapping live regions still require correct application memory planning. See [the full transfer contract](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/docs/CONSUMER_DMA_LENGTH.md).

### Reductions — minimum, maximum, sum and logic

The current header supplies width-specific signed min/max helpers:

| Call | Result |
|---|---|
| `vredmin8(a,mr,n)`, `vredmin16(a,mr,n)` | Signed minimum, with the matching 8/16-bit seed |
| `vredmax8(a,mr,n)`, `vredmax16(a,mr,n)` | Signed maximum, with the matching 8/16-bit seed |
| `vredminu(a,mr,n)`, `vredmaxu(a,mr,n)` | Unsigned minimum / maximum |
| `vredsum(a,mr,n)` | Sum reduction |
| `vredand(a,mr,n)`, `vredor(a,mr,n)`, `vredxor(a,mr,n)` | Bitwise reduction |

```c
__v32i16 minimum, maximum, total;
minimum = vredmin16(a, MASKREAD_OFF, 32);
maximum = vredmax16(a, MASKREAD_OFF, 32);
total = vredsum(a, MASKREAD_OFF, 32);
```

Use the returned reduction buffer rather than assuming each lane contains the answer. Min/max are scalar results in the leading output; sum uses a wider accumulator/result representation. Its byte layout must be read according to the paired backend, not the old blanket rule “the first four elements” (four 8-bit elements and four 16-bit elements differ). The old names `vredmin` and `vredmax` are not generic wrappers in the current header; select the width-specific forms above. The correct mask constant is `MASKREAD_OFF`, not `MASKRED_OFF`.

### Other header entries and backend availability

The shared header generator also names `vsignset`, `vmin` and `vmax`, and the header includes `vload` / `vstore` wrappers. Those builtin names are absent from the paired Venus1 compiler's builtin table. They must not be advertised as usable Venus1 intrinsics merely because a shared macro exists. `vpseudo` is a compiler-oriented helper; it has no general application recipe in this guide. `vsync` combines synchronization with `wfi` and should only be used by code with the matching wake-up protocol.

## Complete two-task example

These task sources follow the release's `forge_vector_smoke` example. Each source includes `venus.h` and can be placed in its correspondingly named `.c` file.

### Task_forgeAdd.c

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

For index `i=0..31`, `a[i]=i-16`, `b[i]=31-2*i`, so `sum[i]=15-i` and `restored[i]=a[i]`. This gives an asymmetric reference that catches reversed subtraction. Both returns contain 32 signed 16-bit elements, or 64 bytes.

After [configuring the release](venus1_get_started.html), compile the bundled target from `platform/`:

```bash
./ace-echo --config .ace-echo/host/local.toml compile dag \
  --target forge_vector_smoke --backend .ace-echo/host/backend.json
```

Use the [programming model](mydoc_programming_model.html) for the run workflow. The complete source directory also contains the reference assets; retain them when reproducing the example.

## CRC example from the original guide

The original guide's larger `Task_nrCRC` demonstrates index generation, gather/scatter, XOR, comparison, reduction and scalar access. Its algorithm-specific input preparation and RNTI masking make it a poor generic CRC reference. The full original example is retained **inside this page** below for existing users; its corrections are called out before the code.

<details class="guide-archive" markdown="1">
<summary>Read the original CRC task and its migration notes</summary>

- The example originally read a result after `VSPM_CLOSE()` instead of opening the window. Correct that sequence before reuse.
- Define `short_struct` in the paired data-type header, and verify the reduction result's width before converting it into a status.
- `sizeof(buf)` returns the full declared buffer capacity, including any uninitialized tail. For a new implementation, return the actual initialized bytes required by its consumers.
- This is the original algorithm example, not the current passing smoke or proof of CRC correctness for arbitrary polynomials and inputs.

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

## Source reference and migration checklist

- [Public macros and control constants](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/toolchain/dsl/venus_test/venus.h)
- [Generated arithmetic/comparison macro catalog](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/toolchain/dsl/venus_test/gen_venusbuiltin_h.py)
- [Published Gem5 arithmetic model](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/components/gem5/src/venus/venus_vfu.cc)
- [Venus1 backend and compiler identity](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/configs/backends/venus1p0-64x512-300mhz.json)
- [Complete smoke application](https://github.com/HorryShenYH/ACE-Echo/blob/db763e2a0826ac4e8c1db3e1cdfc1f1dd3bf9a1e/platform/workloads/5g_lite/tasks/forge_vector_smoke)

When migrating an older task, check operand order, explicit element lengths, output byte counts, real/imaginary ordering, reduction helper names and scalar-access sequencing. Compare outputs with representative asymmetric inputs before using timing to choose an optimization.
