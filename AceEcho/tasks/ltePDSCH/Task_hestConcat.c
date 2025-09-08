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


int Task_hestConcat( short_struct nDLRB,
    __v4096i8 hest_port0_slot0_real, __v4096i8 hest_port0_slot0_imag,__v4096i8 hest_port0_slot1_real, __v4096i8 hest_port0_slot1_imag,
    __v4096i8 hest_port1_slot0_real, __v4096i8 hest_port1_slot0_imag,__v4096i8 hest_port1_slot1_real, __v4096i8 hest_port1_slot1_imag,
    __v4096i8 hest_port2_slot0_real, __v4096i8 hest_port2_slot0_imag,__v4096i8 hest_port2_slot1_real, __v4096i8 hest_port2_slot1_imag,
    __v4096i8 hest_port3_slot0_real, __v4096i8 hest_port3_slot0_imag,__v4096i8 hest_port3_slot1_real, __v4096i8 hest_port3_slot1_imag
){
    short RB = nDLRB.data * 12 *7;  // 12个子载波，7个符号
    short length = RB * 2; 



    hest_port0_slot0_real = vsadd(hest_port0_slot0_real,0,MASKREAD_OFF,length);
    hest_port0_slot0_imag = vsadd(hest_port0_slot0_imag,0,MASKREAD_OFF,length);
    hest_port0_slot1_real = vsadd(hest_port0_slot1_real,0,MASKREAD_OFF,length);
    hest_port0_slot1_imag = vsadd(hest_port0_slot1_imag,0,MASKREAD_OFF,length);

    hest_port1_slot0_real = vsadd(hest_port1_slot0_real,0,MASKREAD_OFF,length);
    hest_port1_slot0_imag = vsadd(hest_port1_slot0_imag,0,MASKREAD_OFF,length);
    hest_port1_slot1_real = vsadd(hest_port1_slot1_real,0,MASKREAD_OFF,length);
    hest_port1_slot1_imag = vsadd(hest_port1_slot1_imag,0,MASKREAD_OFF,length);

    hest_port2_slot0_real = vsadd(hest_port2_slot0_real,0,MASKREAD_OFF,length);
    hest_port2_slot0_imag = vsadd(hest_port2_slot0_imag,0,MASKREAD_OFF,length);
    hest_port2_slot1_real = vsadd(hest_port2_slot1_real,0,MASKREAD_OFF,length);
    hest_port2_slot1_imag = vsadd(hest_port2_slot1_imag,0,MASKREAD_OFF,length);

    hest_port3_slot0_real = vsadd(hest_port3_slot0_real,0,MASKREAD_OFF,length);
    hest_port3_slot0_imag = vsadd(hest_port3_slot0_imag,0,MASKREAD_OFF,length);
    hest_port3_slot1_real = vsadd(hest_port3_slot1_real,0,MASKREAD_OFF,length);
    hest_port3_slot1_imag = vsadd(hest_port3_slot1_imag,0,MASKREAD_OFF,length);



    __v4096i8 hest_port0_real;
    __v4096i8 hest_port0_imag;
    __v4096i8 hest_port1_real;
    __v4096i8 hest_port1_imag;
    __v4096i8 hest_port2_real;
    __v4096i8 hest_port2_imag;
    __v4096i8 hest_port3_real;
    __v4096i8 hest_port3_imag;
    vclaim(hest_port0_real);
    vclaim(hest_port0_imag);
    vclaim(hest_port1_real);
    vclaim(hest_port1_imag);
    vclaim(hest_port2_real);
    vclaim(hest_port2_imag);
    vclaim(hest_port3_real);
    vclaim(hest_port3_imag);

    
    __v2048i16 index1;
    vclaim(index1);
    vrange(index1,RB);
    // index1 = vsadd(index1,0,MASKREAD_OFF,RB);

    vshuffle(hest_port0_real, index1, hest_port0_slot0_real, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port0_imag, index1, hest_port0_slot0_imag, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port1_real, index1, hest_port1_slot0_real, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port1_imag, index1, hest_port1_slot0_imag, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port2_real, index1, hest_port2_slot0_real, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port2_imag, index1, hest_port2_slot0_imag, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port3_real, index1, hest_port3_slot0_real, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port3_imag, index1, hest_port3_slot0_imag, SHUFFLE_SCATTER, RB);


    index1 = vsadd(index1,RB,MASKREAD_OFF,RB);          

    vshuffle(hest_port0_real, index1, hest_port0_slot1_real, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port0_imag, index1, hest_port0_slot1_imag, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port1_real, index1, hest_port1_slot1_real, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port1_imag, index1, hest_port1_slot1_imag, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port2_real, index1, hest_port2_slot1_real, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port2_imag, index1, hest_port2_slot1_imag, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port3_real, index1, hest_port3_slot1_real, SHUFFLE_SCATTER, RB);
    vshuffle(hest_port3_imag, index1, hest_port3_slot1_imag, SHUFFLE_SCATTER, RB);


    


    hest_port0_real = vsadd(hest_port0_real,0,MASKREAD_OFF,length);
    hest_port0_imag = vsadd(hest_port0_imag,0,MASKREAD_OFF,length);
    hest_port1_real = vsadd(hest_port1_real,0,MASKREAD_OFF,length);
    hest_port1_imag = vsadd(hest_port1_imag,0,MASKREAD_OFF,length);
    hest_port2_real = vsadd(hest_port2_real,0,MASKREAD_OFF,length);
    hest_port2_imag = vsadd(hest_port2_imag,0,MASKREAD_OFF,length);
    hest_port3_real = vsadd(hest_port3_real,0,MASKREAD_OFF,length);
    hest_port3_imag = vsadd(hest_port3_imag,0,MASKREAD_OFF,length);






    // char word[15] = "hestConcat finished";
    // printf("----------- %s -----------\n",&word);




    vreturn(hest_port0_real,sizeof(hest_port0_real), hest_port0_imag,sizeof(hest_port0_imag),
            hest_port1_real,sizeof(hest_port1_real), hest_port1_imag,sizeof(hest_port1_imag),
            hest_port2_real,sizeof(hest_port2_real), hest_port2_imag,sizeof(hest_port2_imag),
            hest_port3_real,sizeof(hest_port3_real), hest_port3_imag,sizeof(hest_port3_imag));


}

