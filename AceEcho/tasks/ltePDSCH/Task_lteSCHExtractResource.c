//  lte 提取sch（subframe）
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
    dataSlot0: slot0数据
    dataSlot1: slot1数据
    schindex: SCH index
    schLength: SCH长度
    nDLRB: 总RB数

输出：
    fulldata: slot0和slot1数据拼接后提出的sch数据

*/


typedef struct {
    short data;
} __attribute__((aligned(64))) short_struct;


int Task_lteSCHExtractResource(__v4096i8 datain,
__v2048i16 schindex, short_struct schLength
){
    short length = schLength.data;

    datain = vsadd(datain,0,MASKREAD_OFF,8400);
    schindex = vsadd(schindex,0,MASKREAD_OFF,length);


    __v4096i8 pdschdata;
    vclaim(pdschdata);

    vshuffle(pdschdata,schindex,datain,SHUFFLE_GATHER,length);

    pdschdata =  vsadd(pdschdata,0,MASKREAD_OFF,length);
    


    // char word[16] = "extract finished";
    // printf("----------- %s -----------\n",&word);

    vreturn(pdschdata,sizeof(pdschdata)  );

}

