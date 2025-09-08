/*
 * @Author: Yihao Shen shenyihao@shu.edu.cn
 * @Date: 2025-04-22 22:28:01
 * @LastEditors: Yihao Shen shenyihao@shu.edu.cn
 * @LastEditTime: 2025-06-19 11:54:41
 * @FilePath: /VEMU/AceEcho/tasks/lteSymProc/Task_lteConcatExtract.c
 * @Description: 
 */
#include "data_type.h"
#include "riscv_printf.h"
#include "venus.h"

typedef short __v2048i16 __attribute__((ext_vector_type(2048)));
typedef char __v4096i8 __attribute__((ext_vector_type(4096)));
typedef char __v8512i8 __attribute__((ext_vector_type(8512)));
typedef short __v8512i16 __attribute__((ext_vector_type(8512)));


  

int Task_lteConcatExtract(__v8512i8 rxSlot0_r, __v8512i8 rxSlot0_i, __v8512i8 rxSlot1_r, __v8512i8 rxSlot1_i, __v8512i16 RB6SigIndices,
     __v8512i16 RB15SigIndices, __v8512i16 RB25SigIndices, __v8512i16 RB50SigIndices, __v8512i16 RB75SigIndices, __v8512i16 RB100SigIndices, short_struct nRB)
{
    short NRB = nRB.data;

    __v8512i8 rxSlot0_r_o;
    __v8512i8 rxSlot0_i_o;
    __v8512i8 rxSlot1_r_o;
    __v8512i8 rxSlot1_i_o;
    vclaim(rxSlot0_r_o);
    vclaim(rxSlot0_i_o);
    vclaim(rxSlot1_r_o);
    vclaim(rxSlot1_i_o);
    rxSlot0_r = vsadd(rxSlot0_r, 0, MASKREAD_OFF, 8512);
    rxSlot0_i = vsadd(rxSlot0_i, 0, MASKREAD_OFF, 8512);
    rxSlot1_r = vsadd(rxSlot1_r, 0, MASKREAD_OFF, 8512);
    rxSlot1_i = vsadd(rxSlot1_i, 0, MASKREAD_OFF, 8512);
    if(NRB == 6){
        vshuffle(rxSlot0_r_o, RB6SigIndices, rxSlot0_r, SHUFFLE_GATHER, 6*12*7);
        vshuffle(rxSlot0_i_o, RB6SigIndices, rxSlot0_i, SHUFFLE_GATHER, 6*12*7);
        vshuffle(rxSlot1_r_o, RB6SigIndices, rxSlot1_r, SHUFFLE_GATHER, 6*12*7);
        vshuffle(rxSlot1_i_o, RB6SigIndices, rxSlot1_i, SHUFFLE_GATHER, 6*12*7);
    }
    else if(NRB == 15){
        vshuffle(rxSlot0_r_o, RB15SigIndices, rxSlot0_r, SHUFFLE_GATHER, 15*12*7);
        vshuffle(rxSlot0_i_o, RB15SigIndices, rxSlot0_i, SHUFFLE_GATHER, 15*12*7);
        vshuffle(rxSlot1_r_o, RB15SigIndices, rxSlot1_r, SHUFFLE_GATHER, 15*12*7);
        vshuffle(rxSlot1_i_o, RB15SigIndices, rxSlot1_i, SHUFFLE_GATHER, 15*12*7);
    }
    else if(NRB == 25){
        vshuffle(rxSlot0_r_o, RB25SigIndices, rxSlot0_r, SHUFFLE_GATHER, 25*12*7);
        vshuffle(rxSlot0_i_o, RB25SigIndices, rxSlot0_i, SHUFFLE_GATHER, 25*12*7);
        vshuffle(rxSlot1_r_o, RB25SigIndices, rxSlot1_r, SHUFFLE_GATHER, 25*12*7);
        vshuffle(rxSlot1_i_o, RB25SigIndices, rxSlot1_i, SHUFFLE_GATHER, 25*12*7);
    }
    else if(NRB == 50){
        vshuffle(rxSlot0_r_o, RB50SigIndices, rxSlot0_r, SHUFFLE_GATHER, 50*12*7);
        vshuffle(rxSlot0_i_o, RB50SigIndices, rxSlot0_i, SHUFFLE_GATHER, 50*12*7);
        vshuffle(rxSlot1_r_o, RB50SigIndices, rxSlot1_r, SHUFFLE_GATHER, 50*12*7);
        vshuffle(rxSlot1_i_o, RB50SigIndices, rxSlot1_i, SHUFFLE_GATHER, 50*12*7);
    }
    else if(NRB == 75){
        vshuffle(rxSlot0_r_o,RB75SigIndices ,rxSlot0_r ,SHUFFLE_GATHER ,75 * 12 * 7 );
        vshuffle(rxSlot0_i_o,RB75SigIndices ,rxSlot0_i ,SHUFFLE_GATHER ,75 * 12 * 7 );
        vshuffle(rxSlot1_r_o,RB75SigIndices ,rxSlot1_r ,SHUFFLE_GATHER ,75 * 12 * 7 );
        vshuffle(rxSlot1_i_o,RB75SigIndices ,rxSlot1_i ,SHUFFLE_GATHER ,75 * 12 * 7 );
    }
    else if(NRB == 100){
        vshuffle(rxSlot0_r_o,RB100SigIndices ,rxSlot0_r ,SHUFFLE_GATHER ,100 * 12 * 7 );
        vshuffle(rxSlot0_i_o,RB100SigIndices ,rxSlot0_i ,SHUFFLE_GATHER ,100 * 12 * 7 );
        vshuffle(rxSlot1_r_o,RB100SigIndices ,rxSlot1_r ,SHUFFLE_GATHER ,100 * 12 * 7 );
        vshuffle(rxSlot1_i_o,RB100SigIndices ,rxSlot1_i ,SHUFFLE_GATHER ,100 * 12 * 7 );
    }
    vreturn(rxSlot0_r_o, NRB*12*7, rxSlot0_i_o, NRB*12*7, rxSlot1_r_o, NRB*12*7, rxSlot1_i_o, NRB*12*7);

}
