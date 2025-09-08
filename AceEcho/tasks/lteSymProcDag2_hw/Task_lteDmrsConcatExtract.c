/*
 * @Author: Yihao Shen shenyihao@shu.edu.cn
 * @Date: 2025-04-23 00:42:15
 * @LastEditors: Yihao Shen shenyihao@shu.edu.cn
 * @LastEditTime: 2025-04-23 21:48:04
 * @FilePath: /VEMU/AceEcho/tasks/lteSymProc/Task_lteDmrsConcatExtract.c
 * @Description: 
 */
#include "data_type.h"
#include "riscv_printf.h"
#include "venus.h"

typedef short __v2048i16 __attribute__((ext_vector_type(2048)));
typedef char __v4096i8 __attribute__((ext_vector_type(4096)));

// typedef struct {
//     short data;
//   } __attribute__((aligned(64))) short_struct;
  

int Task_lteDmrsConcatExtract(__v4096i8 dmrs_subframe_r, __v4096i8 dmrs_subframe_i,  __v2048i16 RB6DmrsIndices, __v2048i16 RB15DmrsIndices, __v2048i16 RB25DmrsIndices,
     __v2048i16 RB50DmrsIndices, __v2048i16 RB75DmrsIndices, __v2048i16 RB100DmrsIndices, short_struct nRB) 
{
    short NRB = nRB.data;
    __v4096i8 dmrsSlot0_r;
    __v4096i8 dmrsSlot0_i;
    __v4096i8 dmrsSlot1_r;
    __v4096i8 dmrsSlot1_i;
    vclaim(dmrsSlot0_r);
    vclaim(dmrsSlot0_i);
    vclaim(dmrsSlot1_r);
    vclaim(dmrsSlot1_i);
    if(NRB == 6){
        vshuffle(dmrsSlot0_r, RB6DmrsIndices, dmrs_subframe_r, SHUFFLE_GATHER, 6*2*2);
        vshuffle(dmrsSlot0_i, RB6DmrsIndices, dmrs_subframe_i, SHUFFLE_GATHER, 6*2*2);
        RB6DmrsIndices = vsadd(RB6DmrsIndices, 512, MASKREAD_OFF, 6*2*2);
        vshuffle(dmrsSlot1_r, RB6DmrsIndices, dmrs_subframe_r, SHUFFLE_GATHER, 6*2*2);
        vshuffle(dmrsSlot1_i, RB6DmrsIndices, dmrs_subframe_i, SHUFFLE_GATHER, 6*2*2);
    }
    else if(NRB == 15){
        vshuffle(dmrsSlot0_r, RB15DmrsIndices, dmrs_subframe_r, SHUFFLE_GATHER, 15*2*2);
        vshuffle(dmrsSlot0_i, RB15DmrsIndices, dmrs_subframe_i, SHUFFLE_GATHER, 15*2*2);
        RB15DmrsIndices = vsadd(RB15DmrsIndices, 512, MASKREAD_OFF, 15*2*2);
        vshuffle(dmrsSlot1_r, RB15DmrsIndices, dmrs_subframe_r, SHUFFLE_GATHER, 15*2*2);
        vshuffle(dmrsSlot1_i, RB15DmrsIndices, dmrs_subframe_i, SHUFFLE_GATHER, 15*2*2);
    }
    else if(NRB == 25){
        vshuffle(dmrsSlot0_r, RB25DmrsIndices, dmrs_subframe_r, SHUFFLE_GATHER, 25*2*2);
        vshuffle(dmrsSlot0_i, RB25DmrsIndices, dmrs_subframe_i, SHUFFLE_GATHER, 25*2*2);
        RB25DmrsIndices = vsadd(RB25DmrsIndices, 512, MASKREAD_OFF, 25*2*2);
        vshuffle(dmrsSlot1_r, RB25DmrsIndices, dmrs_subframe_r, SHUFFLE_GATHER, 25*2*2);
        vshuffle(dmrsSlot1_i, RB25DmrsIndices, dmrs_subframe_i, SHUFFLE_GATHER, 25*2*2);
    }
    else if(NRB == 50){
        vshuffle(dmrsSlot0_r,RB50DmrsIndices ,dmrs_subframe_r ,SHUFFLE_GATHER ,50 * 2 * 2 );
        vshuffle(dmrsSlot0_i,RB50DmrsIndices ,dmrs_subframe_i ,SHUFFLE_GATHER ,50 * 2 * 2 );
        RB50DmrsIndices = vsadd(RB50DmrsIndices ,512 ,MASKREAD_OFF ,50 * 2 * 2 );
        vshuffle(dmrsSlot1_r,RB50DmrsIndices ,dmrs_subframe_r ,SHUFFLE_GATHER ,50 * 2 * 2 );
        vshuffle(dmrsSlot1_i,RB50DmrsIndices ,dmrs_subframe_i ,SHUFFLE_GATHER ,50 * 2 * 2 );
    }
    else if(NRB == 75){
        vshuffle(dmrsSlot0_r,RB75DmrsIndices ,dmrs_subframe_r ,SHUFFLE_GATHER ,75 * 2 * 2 );
        vshuffle(dmrsSlot0_i,RB75DmrsIndices ,dmrs_subframe_i ,SHUFFLE_GATHER ,75 * 2 * 2 );
        RB75DmrsIndices = vsadd(RB75DmrsIndices ,512 ,MASKREAD_OFF ,75 * 2 * 2 );
        vshuffle(dmrsSlot1_r,RB75DmrsIndices ,dmrs_subframe_r ,SHUFFLE_GATHER ,75 * 2 * 2 );
        vshuffle(dmrsSlot1_i,RB75DmrsIndices ,dmrs_subframe_i ,SHUFFLE_GATHER ,75 * 2 * 2 );
    }
    else if(NRB == 100){
        vshuffle(dmrsSlot0_r,RB100DmrsIndices ,dmrs_subframe_r ,SHUFFLE_GATHER ,100 * 2 * 2 );
        vshuffle(dmrsSlot0_i,RB100DmrsIndices ,dmrs_subframe_i ,SHUFFLE_GATHER ,100 * 2 * 2 );
        RB100DmrsIndices = vsadd(RB100DmrsIndices ,512 ,MASKREAD_OFF ,100 * 2 * 2 );
        vshuffle(dmrsSlot1_r,RB100DmrsIndices ,dmrs_subframe_r ,SHUFFLE_GATHER ,100 * 2 * 2 );
        vshuffle(dmrsSlot1_i,RB100DmrsIndices ,dmrs_subframe_i ,SHUFFLE_GATHER ,100 * 2 * 2 );
    }


    // __v4096i8 dmrsPort0_r;
    // __v4096i8 dmrsPort0_i;
    // vclaim(dmrsPort0_r);
    // vclaim(dmrsPort0_i);
    // __v2048i16 dmrsPort0Index;
    // vclaim(dmrsPort0Index);
    // vrange(dmrsPort0Index, 2*NRB*2);
    // vshuffle(dmrsPort0_r, dmrsPort0Index, dmrsSlot0_r, SHUFFLE_GATHER, 2*NRB*2);
    // vshuffle(dmrsPort0_i, dmrsPort0Index, dmrsSlot0_i, SHUFFLE_GATHER, 2*NRB*2);
    // dmrsPort0Index = vsadd(dmrsPort0Index, 2*NRB*2, MASKREAD_OFF, 2*NRB*2);
    // vshuffle(dmrsPort0_r, dmrsPort0Index, dmrsSlot1_r, SHUFFLE_SCATTER, 2*NRB*2);
    // vshuffle(dmrsPort0_i, dmrsPort0Index, dmrsSlot1_i, SHUFFLE_SCATTER, 2*NRB*2);



    short_struct dmrsRefLength;
    short_struct subCarrierLength;
    dmrsRefLength.data = NRB*2;
    subCarrierLength.data = NRB*12;
    vreturn(dmrsSlot0_r,NRB*2*2,dmrsSlot0_i,NRB*2*2,dmrsSlot1_r,NRB*2*2,dmrsSlot1_i,NRB*2*2,
        // dmrsPort0_r,NRB*2*4, dmrsPort0_i, NRB*2*4, 
        &dmrsRefLength, sizeof(dmrsRefLength), &subCarrierLength, sizeof(subCarrierLength));

}
