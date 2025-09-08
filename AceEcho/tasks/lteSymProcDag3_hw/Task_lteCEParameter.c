/**
 * *****************************************************************************
 * @file        Task_lteCEParameter.c
 * @brief       
 * @author      Yihao Shen (shenyihao@shu.edu.cn)
 * @date        2025-07-28
 * @copyright   ACE_Lab (Shanghai University), All Rights Reserved.
 * *****************************************************************************
 */
#include "data_type.h"
#include "riscv_printf.h"
#include "venus.h"

typedef short __v2048i16 __attribute__((ext_vector_type(2048)));
typedef char __v4096i8 __attribute__((ext_vector_type(4096)));

// typedef struct {
//     short data;
//   } __attribute__((aligned(64))) short_struct;
  

int Task_lteCEParameter(short_struct nRB) 
{
    short NRB = nRB.data;

    short_struct dmrsRefLength;
    short_struct subCarrierLength;
    dmrsRefLength.data = NRB*2;
    subCarrierLength.data = NRB*12;
    vreturn(&dmrsRefLength, sizeof(dmrsRefLength), &subCarrierLength, sizeof(subCarrierLength));

}
