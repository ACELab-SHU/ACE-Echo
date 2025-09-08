//  lte 输入数据拼接（输入为单个slot）
//  Created by wangqianli

#include "venus.h"
#include <stdint.h>
#include <string.h> 
#include "vmath.h"
#include "riscv_printf.h" 

// typedef short __v2048i16 __attribute__((ext_vector_type(2048)));        //index变量用__v2048i16
// typedef char  __v4096i8 __attribute__((ext_vector_type(4096)));         //数据用4096i8
typedef short __v2048i16 __attribute__((ext_vector_type(4200)));
typedef char __v4096i8 __attribute__((ext_vector_type(8400)));

/* 
输入：


输出：

*/


typedef struct {
    short data;
} __attribute__((aligned(64))) short_struct;


int Task_rxdataConcat( short_struct nDLRB,
    __v4096i8 rxSlot0_real, __v4096i8 rxSlot0_imag,__v4096i8 rxSlot1_real, __v4096i8 rxSlot1_imag
){
    short RB = nDLRB.data * 12 *7;  // 12个子载波，7个符号
    short length = RB * 2; 


    rxSlot0_real = vsadd(rxSlot0_real,0,MASKREAD_OFF,length);
    rxSlot0_imag = vsadd(rxSlot0_imag,0,MASKREAD_OFF,length);
    rxSlot1_real = vsadd(rxSlot1_real,0,MASKREAD_OFF,length);
    rxSlot1_imag = vsadd(rxSlot1_imag,0,MASKREAD_OFF,length);

    




    __v4096i8 rxdata_real;
    __v4096i8 rxdata_imag;
    vclaim(rxdata_real);
    vclaim(rxdata_imag);

    
    __v2048i16 index1;
    vclaim(index1);
    vrange(index1,RB);
    // index1 = vsadd(index1,0,MASKREAD_OFF,RB);

    vshuffle(rxdata_real, index1, rxSlot0_real, SHUFFLE_SCATTER, RB);       
    vshuffle(rxdata_imag, index1, rxSlot0_imag, SHUFFLE_SCATTER, RB);


    index1 = vsadd(index1,RB,MASKREAD_OFF,RB);          
    vshuffle(rxdata_real, index1, rxSlot1_real, SHUFFLE_SCATTER, RB);   
    vshuffle(rxdata_imag, index1, rxSlot1_imag, SHUFFLE_SCATTER, RB);


    rxdata_real = vsadd(rxdata_real,0,MASKREAD_OFF,length);         
    rxdata_imag = vsadd(rxdata_imag,0,MASKREAD_OFF,length);





    // char word[15] = "rxdataConcat finished";
    // printf("----------- %s -----------\n",&word);




    vreturn(rxdata_real,sizeof(rxdata_real), rxdata_imag,sizeof(rxdata_imag));


}

