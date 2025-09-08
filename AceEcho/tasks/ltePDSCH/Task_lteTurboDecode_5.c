// lte Turbo译码,采用max-log-MAP算法
/*
 * @input   soft_in 软输入
 * @input   alphain 交织序列
 * @output  hard_out 译码输出
 *
 * Created by wangqianli
*/
/* 拆分的Turbodecode 2：
    SISO2 拆分出计算得到的 前向度量和后向度量
*/

#include "venus.h"
#include <stdint.h>
#include "vmath.h"
#include "riscv_printf.h"


typedef short __v2048i16 __attribute__((ext_vector_type(5000)));        //index变量用__v2048i16
typedef char  __v4096i8 __attribute__((ext_vector_type(5000)));         //数据用4096i8
// typedef short __v2048i16 __attribute__((ext_vector_type(2048)));        //index变量用__v2048i16
// typedef char  __v4096i8 __attribute__((ext_vector_type(4096)));         //数据用4096i8


//打印向量
#define VENUS_PRINTVEC_CHAR(name, len)                                                                                 \
  do {                                                                                                                 \
    short array_##name[len];                                                                                           \
    short   vecaddr_##name = vaddr(name);                                                                                \
    VSPM_OPEN();                                                                                                       \
    vbarrier();                                                                                                        \
    for (short _____ = 0; _____ < len; _____++) {                                                                        \
      array_##name[_____] = *(volatile unsigned char *)(vecaddr_##name + _____);                                       \
      printf("%hd\n", &array_##name[_____]);                                                                           \
    }                                                                                                                  \
    VSPM_CLOSE();                                                                                                      \
  } while (0)






typedef struct {
    short data;
} __attribute__((aligned(64))) short_struct;


int Task_lteTurboDecode_5(__v4096i8 atemp1_8B,short_struct trblklength,short_struct SW_value){
    
    short reg  = 4;                         //寄存器个数
    short trblklen = trblklength.data;      //原始信息长度
    short N  = trblklen + 24;               //码块长度
    short B  = reg+N;   
    // printf("B = %hd\n",&B);    

    short ii = 3 ;                  //迭代次数
    short SW = SW_value.data;       //分的段数
    // printf("trblklen = %hd\n",&trblklen);
    // printf("SW = %hd\n",&SW);


    short kminus = B*10/SW;
    short kplus = 0;
    if(kminus%10 == 0){
        kminus = B/SW;
        kplus = kminus;
    }else{
        kminus = B/SW;
        kplus = kminus+1;
    }
    // printf("kminus = %hd\n",&kminus);
    // printf("kplus = %hd\n",&kplus);


    short half_length = 8*SW;
    short length = half_length*2;
    short length1= 512/SW;               //(8192/(8*SW*2))
    // short length1= 256/SW;                  //(4096/(8*SW*2))
    short length2= length1*SW;
    short length3;
    short length4;
    short length5;
    short length6;
    short length7 = B-((SW-1)*kminus+1) +1 + 1;     
    // printf("half_length=%hd\n",&half_length);
    // printf("length1 = (4096/(8*SW*2)) = %hd\n",&length1);
    // printf("length2 = length1*SW = %hd\n",&length2);
    // printf("length7 = B-((SW-1)*kminus+1) +1 +1 = %hd\n",&length7);
    //length7补充说明
    //前向循环次数：B-((SW-1)*kminus+1) +2次，实际计算为B-((SW-1)*kminus+1) +1次；
    //后向循环次数：循环和实际计算都为B-((SW-1)*kminus+1) +1 次，
    //本程序中i=0不参与实际计算，
    //所以，维纳斯中需要做 length7 = = B-((SW-1)*kminus+1) +1 + 1 次循环 

    short d_bias;
    short l2=2*length1;
    short l3=3*length1;
    short l4=4*length1;
    short l5=5*length1;
    short l6=6*length1;
    short l7=7*length1;

    short j=0;
    int sum =  length7 * 8 * SW * 2;
    int fac = 5000;
    int flag = sum*10 /fac;
    if(flag %10 ==0){
        flag = sum/fac;
    }else{
        flag = sum/fac + 1;
    }
    // printf("flag = %d\n",&flag);




    /**************** 前向度量 变量申明 *****************/
    __v4096i8 a1_B;         //前向度量
    __v4096i8 a2_B;
    __v4096i8 a3_B;
    __v4096i8 a4_B;
    __v4096i8 a5_B;
    __v4096i8 a6_B;
    __v4096i8 a7_B;
    __v4096i8 a8_B;
    vclaim(a1_B);
    vclaim(a2_B);
    vclaim(a3_B);
    vclaim(a4_B);
    vclaim(a5_B);
    vclaim(a6_B);
    vclaim(a7_B);
    vclaim(a8_B);

    /**************** 后向度量 变量申明 *****************/
    __v4096i8 b1_reverse_B;         //b1_B的倒序
    __v4096i8 b2_reverse_B;
    __v4096i8 b3_reverse_B;
    __v4096i8 b4_reverse_B;
    __v4096i8 b5_reverse_B;
    __v4096i8 b6_reverse_B;
    __v4096i8 b7_reverse_B;
    __v4096i8 b8_reverse_B;
    vclaim(b1_reverse_B);
    vclaim(b2_reverse_B);
    vclaim(b3_reverse_B);
    vclaim(b4_reverse_B);
    vclaim(b5_reverse_B);
    vclaim(b6_reverse_B);
    vclaim(b7_reverse_B);
    vclaim(b8_reverse_B);

    __v4096i8 b1_B;     //后向度量
    __v4096i8 b2_B;
    __v4096i8 b3_B;
    __v4096i8 b4_B;
    __v4096i8 b5_B;
    __v4096i8 b6_B;
    __v4096i8 b7_B;
    __v4096i8 b8_B;
    vclaim(b1_B);
    vclaim(b2_B);
    vclaim(b3_B);
    vclaim(b4_B);
    vclaim(b5_B);
    vclaim(b6_B);
    vclaim(b7_B);
    vclaim(b8_B);

    /**************** 拆分index 变量申明 *****************/
    __v2048i16 split_index1;
    __v2048i16 split_index2;
    // __v2048i16 split_index3;
    // __v2048i16 split_index4;
    // __v2048i16 split_index5;
    // __v2048i16 split_index6;
    // __v2048i16 split_index7;
    // __v2048i16 split_index8;
    vclaim(split_index1);
    vclaim(split_index2);
    // vclaim(split_index3);
    // vclaim(split_index4);
    // vclaim(split_index5);
    // vclaim(split_index6);
    // vclaim(split_index7);
    // vclaim(split_index8);


    /**************** 其他变量申明 *****************/
    __v2048i16 SW_tempindex;
    __v2048i16 SW_tempindex2;
    __v2048i16 SW_tempindex3;
    __v2048i16 SW_tempindex4;
    __v2048i16 SW_tempindex5;
    __v2048i16 SW_tempindex6;
    __v2048i16 SW_tempindex7;
    // __v2048i16 SW_tempindex8;
    // __v2048i16 SW_tempindex9;
    // __v2048i16 SW_tempindex10;
    // __v2048i16 SW_tempindex11;
    // __v2048i16 SW_tempindex12;
    // __v2048i16 SW_tempindex13;
    // __v2048i16 SW_tempindex14;
    // __v2048i16 SW_tempindex15;
    // __v2048i16 SW_tempindex16;
    // __v2048i16 SW_tempindex17;
    vclaim(SW_tempindex);
    vclaim(SW_tempindex2);
    vclaim(SW_tempindex3);
    vclaim(SW_tempindex4);
    vclaim(SW_tempindex5);
    vclaim(SW_tempindex6);
    vclaim(SW_tempindex7);
    // vclaim(SW_tempindex8);
    // vclaim(SW_tempindex9);
    // vclaim(SW_tempindex10);
    // vclaim(SW_tempindex11);
    // vclaim(SW_tempindex12);
    // vclaim(SW_tempindex13);
    // vclaim(SW_tempindex14);
    // vclaim(SW_tempindex15);
    // vclaim(SW_tempindex16);
    // vclaim(SW_tempindex17);

    __v4096i8 SW_temp;
    // __v4096i8 SW_temp2;
    // __v4096i8 SW_temp3;
    // __v4096i8 SW_temp4;
    // __v4096i8 SW_temp5;
    // __v4096i8 SW_temp6;
    // __v4096i8 SW_temp7;
    // __v4096i8 SW_temp8;
    vclaim(SW_temp);
    // vclaim(SW_temp2);
    // vclaim(SW_temp3);
    // vclaim(SW_temp4);
    // vclaim(SW_temp5);
    // vclaim(SW_temp6);
    // vclaim(SW_temp7);
    // vclaim(SW_temp8);












    /******************************* 计算拆分前向后向结果的index ************************************** */
    //判断前向后向循环length7次 计算得到的数据，需要放几个4096
    //假设最长的情况（6144bit）要分70段 ，length7 =146, 计算得到 146*8*70*2 = 163520 个数据 ，需要放40段
    // int sum =  length7 * 8 * SW * 2;
    // flag = sum*10 /4096;
    // if(flag %10 ==0){
    //     flag = sum/4096;
    // }else{
    //     flag = sum/4096 + 1;
    // }
    // printf("flag = %d\n",&flag);

    if(flag ==1){
        //计算前几段搬移时需要的index
        vbrdcst(SW_tempindex6,8,MASKREAD_OFF,length7);
        vbrdcst(SW_tempindex7,kminus,MASKREAD_OFF,length7);

        vrange(split_index1,length7);     //0,1,2,....,8*B/(length)-1
        vrange(split_index2,length7);     //0,1,2,....,8*B/(length)-1
        split_index1 = vmul(split_index1,length,MASKREAD_OFF,length7);  //0,length,2*length,...,
        SW_tempindex3 = vadd(split_index2,0,MASKREAD_OFF,length7);
        //从最后一个分段开始
        split_index1 = vadd(split_index1,8*(SW-1),MASKREAD_OFF,length7);
        split_index2 = vadd(split_index2,kminus*(SW-1),MASKREAD_OFF,length7);
        SW_tempindex2 = vadd(split_index1,0,MASKREAD_OFF,length7);    
        SW_tempindex5 = vadd(split_index2,0,MASKREAD_OFF,length7);
        for(short i=1;i<SW;i++){
            SW_tempindex2 = vrsub(SW_tempindex2,SW_tempindex6,MASKREAD_OFF,length7);           //16,length+16,2*length+16,...,
            SW_tempindex3 = vadd(SW_tempindex3,length7,MASKREAD_OFF,length7);      //移位拼接的index
            SW_tempindex5 = vrsub(SW_tempindex5,SW_tempindex7,MASKREAD_OFF,length7);       //
            vshuffle(split_index1,SW_tempindex3,SW_tempindex2,SHUFFLE_SCATTER,length7);
            vshuffle(split_index2,SW_tempindex3,SW_tempindex5,SHUFFLE_SCATTER,length7);
        }
        length2 = length7*SW;
        
    }
    // else if(flag ==2){

    // }else if(flag ==3){
    //     //计算前几段搬移时需要的index
    //     vrange(split_index1,length1);     //0,1,2,....,8*B/(length)-1
    //     vrange(split_index2,length1);     //0,1,2,....,8*B/(length)-1
    //     split_index1 = vmul(split_index1,length,MASKREAD_OFF,length1);  //0,length,2*length,...,
    //     SW_tempindex2 = vadd(split_index1,0,MASKREAD_OFF,length1);
    //     SW_tempindex3 = vadd(split_index2,0,MASKREAD_OFF,length1);
    //     SW_tempindex5 = vadd(split_index2,0,MASKREAD_OFF,length1);
    //     for(short i=1;i<SW;i++){
    //         SW_tempindex2 = vadd(SW_tempindex2,8,MASKREAD_OFF,length1);           //16,length+16,2*length+16,...,
    //         SW_tempindex3 = vadd(SW_tempindex3,length1,MASKREAD_OFF,length1);      //移位拼接的index
    //         SW_tempindex5 = vadd(SW_tempindex5,kminus,MASKREAD_OFF,length1);       //
    //         vshuffle(split_index1,SW_tempindex3,SW_tempindex2,SHUFFLE_SCATTER,length1);
    //         vshuffle(split_index2,SW_tempindex3,SW_tempindex5,SHUFFLE_SCATTER,length1);
    //     }
    //     //计算最后一段搬移需要的index
    //     //前向
    //     length3 = (kplus-2*length1);
    //     length4 = length3*SW;
    //     vrange(split_index3,length3);     //0,1,2,....
    //     vrange(split_index4,length3);     //0,1,2,....
    //     split_index3 = vmul(split_index3,length,MASKREAD_OFF,length3);  //0,length,2*length,...,
    //     SW_tempindex2 = vadd(split_index3,0,MASKREAD_OFF,length3);
    //     SW_tempindex3 = vadd(split_index4,0,MASKREAD_OFF,length3);
    //     SW_tempindex5 = vadd(split_index4,0,MASKREAD_OFF,length3);
    //     for(short i=1;i<SW;i++){
    //         SW_tempindex2 = vadd(SW_tempindex2,8,MASKREAD_OFF,length3);
    //         SW_tempindex3 = vadd(SW_tempindex3,length3,MASKREAD_OFF,length3);
    //         SW_tempindex5 = vadd(SW_tempindex5,kminus,MASKREAD_OFF,length3);
    //         vshuffle(split_index3,SW_tempindex3,SW_tempindex2,SHUFFLE_SCATTER,length3);
    //         vshuffle(split_index4,SW_tempindex3,SW_tempindex5,SHUFFLE_SCATTER,length3);
    //     }
    //     split_index4 = vadd(split_index4,length1*2,MASKREAD_OFF,length4);

    //     //后向
    //     length5 = (length7)-2*length1;
    //     length6 = length5*SW;
    //     vrange(split_index5,length5);     //0,1,2,....
    //     vrange(split_index6,length5);     //0,1,2,....
    //     split_index5 = vmul(split_index5,length,MASKREAD_OFF,length5);  //0,length,2*length,...,
    //     SW_tempindex2 = vadd(split_index5,0,MASKREAD_OFF,length5);
    //     SW_tempindex3 = vadd(split_index6,0,MASKREAD_OFF,length5);
    //     SW_tempindex5 = vadd(split_index6,0,MASKREAD_OFF,length5);
    //     for(short i=1;i<SW;i++){
    //         SW_tempindex2 = vadd(SW_tempindex2,8,MASKREAD_OFF,length5);
    //         SW_tempindex3 = vadd(SW_tempindex3,length5,MASKREAD_OFF,length5);
    //         SW_tempindex5 = vadd(SW_tempindex5,kminus,MASKREAD_OFF,length5);
    //         vshuffle(split_index5,SW_tempindex3,SW_tempindex2,SHUFFLE_SCATTER,length5);
    //         vshuffle(split_index6,SW_tempindex3,SW_tempindex5,SHUFFLE_SCATTER,length5);
    //     }
    //     split_index6 = vadd(split_index6,length1*2,MASKREAD_OFF,length6);
    // }else if(flag ==4){

    // }else if(flag ==5){

    // }else if(flag ==6){


    split_index1 = vadd(split_index1,0,MASKREAD_OFF,length2);
    split_index2 = vadd(split_index2,0,MASKREAD_OFF,length2);
    // // split_index3 = vadd(split_index3,0,MASKREAD_OFF,length4);
    // // split_index4 = vadd(split_index4,0,MASKREAD_OFF,length4);
    // split_index5 = vadd(split_index5,0,MASKREAD_OFF,length6);
    // split_index6 = vadd(split_index6,0,MASKREAD_OFF,length6);

    //
    vbrdcst(a1_B,176,MASKREAD_OFF,B);
    vbrdcst(a2_B,176,MASKREAD_OFF,B);
    vbrdcst(a3_B,176,MASKREAD_OFF,B);
    vbrdcst(a4_B,176,MASKREAD_OFF,B);
    vbrdcst(a5_B,176,MASKREAD_OFF,B);
    vbrdcst(a6_B,176,MASKREAD_OFF,B);
    vbrdcst(a7_B,176,MASKREAD_OFF,B);
    vbrdcst(a8_B,176,MASKREAD_OFF,B);
    vbrdcst(b1_reverse_B,176,MASKREAD_OFF,B);
    vbrdcst(b2_reverse_B,176,MASKREAD_OFF,B);
    vbrdcst(b3_reverse_B,176,MASKREAD_OFF,B);
    vbrdcst(b4_reverse_B,176,MASKREAD_OFF,B);
    vbrdcst(b5_reverse_B,176,MASKREAD_OFF,B);
    vbrdcst(b6_reverse_B,176,MASKREAD_OFF,B);
    vbrdcst(b7_reverse_B,176,MASKREAD_OFF,B);
    vbrdcst(b8_reverse_B,176,MASKREAD_OFF,B);

    /******* 把计算出的atemp_8B拆分为a1_B,a2_B,...,a8_B; b1_B,b2_B,...,b8_B******/
    // if(flag==1){           //只有一段
    //拆第一段
    vshuffle(SW_temp,split_index1,atemp1_8B,SHUFFLE_GATHER,length2);   //拆出atemp1_8B中的所有a1放入a1中对应的位置
    vshuffle(a1_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(split_index1,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(a2_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(a3_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(a4_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(a5_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(a6_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(a7_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(a8_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    //后向
    SW_tempindex = vadd(SW_tempindex,half_length-7,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(b1_reverse_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(b2_reverse_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(b3_reverse_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(b4_reverse_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(b5_reverse_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(b6_reverse_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(b7_reverse_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

    SW_tempindex = vadd(SW_tempindex,1,MASKREAD_OFF,length2);
    vshuffle(SW_temp,SW_tempindex,atemp1_8B,SHUFFLE_GATHER,length2);
    vshuffle(b8_reverse_B,split_index2,SW_temp,SHUFFLE_SCATTER,length2);

//     }
// }else if(flag==2){     //分两段

// }else if(flag==3){     //分三段

    //将btemp_B倒序
    vrange(SW_tempindex,B);
    vbrdcst(SW_tempindex2,B-1,MASKREAD_OFF,B);
    SW_tempindex2 = vrsub(SW_tempindex2,SW_tempindex,MASKREAD_OFF,B);      //(B-1),B-2,.....,0
    vshuffle(b1_B,SW_tempindex2,b1_reverse_B,SHUFFLE_SCATTER,B);
    vshuffle(b2_B,SW_tempindex2,b2_reverse_B,SHUFFLE_SCATTER,B);
    vshuffle(b3_B,SW_tempindex2,b3_reverse_B,SHUFFLE_SCATTER,B);
    vshuffle(b4_B,SW_tempindex2,b4_reverse_B,SHUFFLE_SCATTER,B);
    vshuffle(b5_B,SW_tempindex2,b5_reverse_B,SHUFFLE_SCATTER,B);
    vshuffle(b6_B,SW_tempindex2,b6_reverse_B,SHUFFLE_SCATTER,B);
    vshuffle(b7_B,SW_tempindex2,b7_reverse_B,SHUFFLE_SCATTER,B);
    vshuffle(b8_B,SW_tempindex2,b8_reverse_B,SHUFFLE_SCATTER,B);

    // b1_reverse_B = vsadd(b1_reverse_B,0,MASKREAD_OFF,B);
    // b2_reverse_B = vsadd(b2_reverse_B,0,MASKREAD_OFF,B);
    // b3_reverse_B = vsadd(b3_reverse_B,0,MASKREAD_OFF,B);
    // b4_reverse_B = vsadd(b4_reverse_B,0,MASKREAD_OFF,B);
    // b5_reverse_B = vsadd(b5_reverse_B,0,MASKREAD_OFF,B);
    // b6_reverse_B = vsadd(b6_reverse_B,0,MASKREAD_OFF,B);
    // b7_reverse_B = vsadd(b7_reverse_B,0,MASKREAD_OFF,B);
    // b8_reverse_B = vsadd(b8_reverse_B,0,MASKREAD_OFF,B);


    a1_B = vsadd(a1_B,0,MASKREAD_OFF,B);
    a2_B = vsadd(a2_B,0,MASKREAD_OFF,B);
    a3_B = vsadd(a3_B,0,MASKREAD_OFF,B);
    a4_B = vsadd(a4_B,0,MASKREAD_OFF,B);
    a5_B = vsadd(a5_B,0,MASKREAD_OFF,B);
    a6_B = vsadd(a6_B,0,MASKREAD_OFF,B);
    a7_B = vsadd(a7_B,0,MASKREAD_OFF,B);
    a8_B = vsadd(a8_B,0,MASKREAD_OFF,B);


    b1_B = vsadd(b1_B,0,MASKREAD_OFF,B);
    b2_B = vsadd(b2_B,0,MASKREAD_OFF,B);
    b3_B = vsadd(b3_B,0,MASKREAD_OFF,B);
    b4_B = vsadd(b4_B,0,MASKREAD_OFF,B);
    b5_B = vsadd(b5_B,0,MASKREAD_OFF,B);
    b6_B = vsadd(b6_B,0,MASKREAD_OFF,B);
    b7_B = vsadd(b7_B,0,MASKREAD_OFF,B);
    b8_B = vsadd(b8_B,0,MASKREAD_OFF,B);



    // 缩放，防止溢出 5/16=0.3125
    a1_B = vsra(a1_B,2,MASKREAD_OFF,B);
    a2_B = vsra(a2_B,2,MASKREAD_OFF,B);
    a3_B = vsra(a3_B,2,MASKREAD_OFF,B);
    a4_B = vsra(a4_B,2,MASKREAD_OFF,B);
    a5_B = vsra(a5_B,2,MASKREAD_OFF,B);
    a6_B = vsra(a6_B,2,MASKREAD_OFF,B);
    a7_B = vsra(a7_B,2,MASKREAD_OFF,B);
    a8_B = vsra(a8_B,2,MASKREAD_OFF,B);

    b1_B = vsra(b1_B,2,MASKREAD_OFF,B);
    b2_B = vsra(b2_B,2,MASKREAD_OFF,B);
    b3_B = vsra(b3_B,2,MASKREAD_OFF,B);
    b4_B = vsra(b4_B,2,MASKREAD_OFF,B);
    b5_B = vsra(b5_B,2,MASKREAD_OFF,B);
    b6_B = vsra(b6_B,2,MASKREAD_OFF,B);
    b7_B = vsra(b7_B,2,MASKREAD_OFF,B);
    b8_B = vsra(b8_B,2,MASKREAD_OFF,B);

    a1_B = vmul(a1_B,5,MASKREAD_OFF,B);
    a2_B = vmul(a2_B,5,MASKREAD_OFF,B);
    a3_B = vmul(a3_B,5,MASKREAD_OFF,B);
    a4_B = vmul(a4_B,5,MASKREAD_OFF,B);
    a5_B = vmul(a5_B,5,MASKREAD_OFF,B);
    a6_B = vmul(a6_B,5,MASKREAD_OFF,B);
    a7_B = vmul(a7_B,5,MASKREAD_OFF,B);
    a8_B = vmul(a8_B,5,MASKREAD_OFF,B);

    b1_B = vmul(b1_B,5,MASKREAD_OFF,B);
    b2_B = vmul(b2_B,5,MASKREAD_OFF,B);
    b3_B = vmul(b3_B,5,MASKREAD_OFF,B);
    b4_B = vmul(b4_B,5,MASKREAD_OFF,B);
    b5_B = vmul(b5_B,5,MASKREAD_OFF,B);
    b6_B = vmul(b6_B,5,MASKREAD_OFF,B);
    b7_B = vmul(b7_B,5,MASKREAD_OFF,B);
    b8_B = vmul(b8_B,5,MASKREAD_OFF,B);


    a1_B = vsra(a1_B,2,MASKREAD_OFF,B);
    a2_B = vsra(a2_B,2,MASKREAD_OFF,B);
    a3_B = vsra(a3_B,2,MASKREAD_OFF,B);
    a4_B = vsra(a4_B,2,MASKREAD_OFF,B);
    a5_B = vsra(a5_B,2,MASKREAD_OFF,B);
    a6_B = vsra(a6_B,2,MASKREAD_OFF,B);
    a7_B = vsra(a7_B,2,MASKREAD_OFF,B);
    a8_B = vsra(a8_B,2,MASKREAD_OFF,B);

    b1_B = vsra(b1_B,2,MASKREAD_OFF,B);
    b2_B = vsra(b2_B,2,MASKREAD_OFF,B);
    b3_B = vsra(b3_B,2,MASKREAD_OFF,B);
    b4_B = vsra(b4_B,2,MASKREAD_OFF,B);
    b5_B = vsra(b5_B,2,MASKREAD_OFF,B);
    b6_B = vsra(b6_B,2,MASKREAD_OFF,B);
    b7_B = vsra(b7_B,2,MASKREAD_OFF,B);
    b8_B = vsra(b8_B,2,MASKREAD_OFF,B);





    // char words[30] = "SISO2 data split finished";
    // printf("-------%s---------\n",&words);

    // __v4096i8 hard_out_N;
    // vclaim(hard_out_N);
    // vbrdcst(hard_out_N,0,MASKREAD_OFF,10);

    vreturn(a1_B,sizeof(a1_B), a2_B,sizeof(a2_B), a3_B,sizeof(a3_B), a4_B,sizeof(a4_B), a5_B,sizeof(a5_B), a6_B,sizeof(a6_B), a7_B,sizeof(a7_B), a8_B,sizeof(a8_B), b1_B,sizeof(b1_B), b2_B,sizeof(b2_B), b3_B,sizeof(b3_B), b4_B,sizeof(b4_B), b5_B,sizeof(b5_B), b6_B,sizeof(b6_B), b7_B,sizeof(b7_B), b8_B,sizeof(b8_B));

}