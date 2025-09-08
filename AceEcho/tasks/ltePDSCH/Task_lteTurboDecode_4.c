// lte Turbo译码,采用max-log-MAP算法
/*
 * @input   soft_in 软输入
 * @input   alphain 交织序列
 * @output  hard_out 译码输出
 *
 * Created by wangqianli
*/
/* 拆分的Turbodecode 1：
    SISO2:计算分支、前向、后向度量
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


int Task_lteTurboDecode_4(
    // __v4096i8 softin1_x_B,__v4096i8 softin1_y_B,
    __v4096i8 softin2_x_B,__v4096i8 softin2_y_B,__v4096i8 e_p_B,
    __v4096i8 state0_8,
    __v2048i16 alphain_N,
    // __v2048i16 a_add_index1,__v2048i16 a_add_index2,__v2048i16 a_add_index3,__v2048i16 a_add_index4,
    // __v2048i16 b_add_index1,__v2048i16 b_add_index2,
    __v2048i16 a_add_index1,__v2048i16 a_add_index2,__v2048i16 a_add_index3,__v2048i16 a_add_index4,
    __v2048i16 atemp1_index_8,__v2048i16 atemp2_index_8,
    // __v2048i16 atemp1_index,__v2048i16 atemp2_index,__v2048i16 btemp1_index,__v2048i16 btemp2_index,
    short_struct trblklength,short_struct SW_value ){
    
    short reg  = 4;                         //寄存器个数
    short trblklen = trblklength.data;      //原始信息长度
    short N  = trblklen + 24;               //码块长度
    short B  = reg+N;   
    // printf("B = %hd\n",&B);    

    short ii = 3 ;                  //迭代次数
    short SW = SW_value.data;       //分的段数
    // printf("trblklen = %hd\n",&trblklen);
    // printf("SW = %hd\n",&SW);
    int fac = 5000;    

    // short d1_ksub1;
    // short d2_ksub1;
    // short d3_ksub1;
    // short d4_ksub1; 


    // softin1_x_B = vsadd(softin1_x_B,0,MASKREAD_OFF,B);
    // softin1_y_B = vsadd(softin1_y_B,0,MASKREAD_OFF,B);
    softin2_x_B = vsadd(softin2_x_B,0,MASKREAD_OFF,B);
    softin2_y_B = vsadd(softin2_y_B,0,MASKREAD_OFF,B);
    state0_8 = vsadd(state0_8,0,MASKREAD_OFF,600);
    e_p_B = vsadd(e_p_B,0,MASKREAD_OFF,B);

    alphain_N = vsadd(alphain_N,0,MASKREAD_OFF,N);

    // a_add_index1 = vsadd(a_add_index1,0,MASKREAD_OFF,8);
    // a_add_index2 = vsadd(a_add_index2,0,MASKREAD_OFF,8);
    // a_add_index3 = vsadd(a_add_index3,0,MASKREAD_OFF,8);
    // a_add_index4 = vsadd(a_add_index4,0,MASKREAD_OFF,8);
    // b_add_index1 = vsadd(b_add_index1,0,MASKREAD_OFF,8);
    // b_add_index2 = vsadd(b_add_index2,0,MASKREAD_OFF,8);

    // atemp1_index = vsadd(atemp1_index,0,MASKREAD_OFF,816);
    // atemp2_index = vsadd(atemp2_index,0,MASKREAD_OFF,816);
    // btemp1_index = vsadd(btemp1_index,0,MASKREAD_OFF,816);
    // btemp2_index = vsadd(btemp2_index,0,MASKREAD_OFF,816);


    //以下用于保证除了传入参数有效数据之外的位置都为0
    __v4096i8 zerostemp1;   
    __v4096i8 zerostemp2;   
    vclaim(zerostemp1);
    vclaim(zerostemp2);
    __v2048i16 temp10;    
    // __v2048i16 temp11;
    // __v2048i16 temp12;   
    // __v2048i16 temp13;
    // __v2048i16 temp14; 
    vclaim(temp10);
    // vclaim(temp11);
    // vclaim(temp12);     
    // vclaim(temp13);
    // vclaim(temp14);   

    // vbrdcst(zerostemp1,0,MASKREAD_OFF,4096);
    vrange(temp10,B);
    // vshuffle(zerostemp1,temp10,softin1_x_B,SHUFFLE_GATHER,B);
    // vbrdcst(softin1_x_B,0,MASKREAD_OFF,4096);
    // vshuffle(softin1_x_B,temp10,zerostemp1,SHUFFLE_SCATTER,B);
    // softin1_x_B = vsadd(softin1_x_B,0,MASKREAD_OFF,4096);

    // vbrdcst(zerostemp1,0,MASKREAD_OFF,4096);
    // vshuffle(zerostemp1,temp10,softin1_y_B,SHUFFLE_GATHER,B);
    // vbrdcst(softin1_y_B,0,MASKREAD_OFF,4096);
    // vshuffle(softin1_y_B,temp10,zerostemp1,SHUFFLE_SCATTER,B);
    // softin1_y_B = vsadd(softin1_y_B,0,MASKREAD_OFF,4096);

    vbrdcst(zerostemp1,0,MASKREAD_OFF,fac);
    vshuffle(zerostemp1,temp10,softin2_x_B,SHUFFLE_GATHER,B);
    vbrdcst(softin2_x_B,0,MASKREAD_OFF,fac);
    vshuffle(softin2_x_B,temp10,zerostemp1,SHUFFLE_SCATTER,B);
    softin2_x_B = vsadd(softin2_x_B,0,MASKREAD_OFF,fac);

    vbrdcst(zerostemp1,0,MASKREAD_OFF,fac);
    vshuffle(zerostemp1,temp10,softin2_y_B,SHUFFLE_GATHER,B);
    vbrdcst(softin2_y_B,0,MASKREAD_OFF,fac);
    vshuffle(softin2_y_B,temp10,zerostemp1,SHUFFLE_SCATTER,B);
    softin2_y_B = vsadd(softin2_y_B,0,MASKREAD_OFF,fac);




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
    // int fac = 8192;
    // int flag = sum*10 /4096;
    // if(flag %10 ==0){
    //     flag = sum/4096;
    // }else{
    //     flag = sum/4096 + 1;
    // }
    // printf("flag = %d\n",&flag);



    /**************** 分支度量 变量申明 *****************/
    __v4096i8 d1_B;     //分支量度
    __v4096i8 d2_B;
    __v4096i8 d3_B;
    __v4096i8 d4_B;
    vclaim(d1_B);
    vclaim(d2_B);
    vclaim(d3_B);
    vclaim(d4_B);

    __v4096i8 d1_B_ksub1;
    __v4096i8 d2_B_ksub1;
    __v4096i8 d3_B_ksub1;
    __v4096i8 d4_B_ksub1;
    vclaim(d1_B_ksub1);
    vclaim(d2_B_ksub1);
    vclaim(d3_B_ksub1);
    vclaim(d4_B_ksub1);

    //加分支度量的位置
    __v2048i16 a_add_index5;        //a_add_index1的备份
    __v2048i16 a_add_index6;
    __v2048i16 a_add_index7;
    __v2048i16 a_add_index8;
    vclaim(a_add_index6);
    vclaim(a_add_index7);
    vclaim(a_add_index8);
    __v2048i16 b_add_index3;
    vclaim(b_add_index3);


    __v2048i16 ad_vseqindex;
    __v2048i16 bd_vseqindex;
    vclaim(ad_vseqindex);
    vclaim(bd_vseqindex);


    /**************** 前向度量 变量申明 *****************/
    //前向度量
    __v4096i8 atemp;
    __v4096i8 atemp1_8;         //左边的和
    __v4096i8 atemp2_8;         //右边的和
    vclaim(atemp);
    vclaim(atemp1_8);
    vclaim(atemp2_8);


    //数据过长需要分段放，创建存放位置
    __v4096i8 atemp1_8B;
    // __v4096i8 atemp2_8B;
    // __v4096i8 atemp3_8B;
    // __v4096i8 atemp4_8B;
    // __v4096i8 atemp5_8B;
    // __v4096i8 atemp6_8B;
    // __v4096i8 atemp7_8B;
    vclaim(atemp1_8B);
    // vclaim(atemp2_8B);
    // vclaim(atemp3_8B);
    // vclaim(atemp4_8B);
    // vclaim(atemp5_8B);
    // vclaim(atemp6_8B);
    // vclaim(atemp7_8B);

    // __v2048i16 atemp_index1_8;
    // __v2048i16 atemp_index2_8;
    // __v2048i16 atemp1_index_8;              //加分支度量前，需要变换前向度量的顺序
    // __v2048i16 atemp2_index_8;              //加分支度量前，需要变换前向度量的顺序
    // vclaim(atemp1_index_8);
    // vclaim(atemp2_index_8);

    // __v4096i8 a1_B;         //前向度量
    // __v4096i8 a2_B;
    // __v4096i8 a3_B;
    // __v4096i8 a4_B;
    // __v4096i8 a5_B;
    // __v4096i8 a6_B;
    // __v4096i8 a7_B;
    // __v4096i8 a8_B;
    // vclaim(a1_B);
    // vclaim(a2_B);
    // vclaim(a3_B);
    // vclaim(a4_B);
    // vclaim(a5_B);
    // vclaim(a6_B);
    // vclaim(a7_B);
    // vclaim(a8_B);

    // /**************** 后向度量 变量申明 *****************/
    // __v4096i8 b1_reverse_B;         //b1_B的倒序
    // __v4096i8 b2_reverse_B;
    // __v4096i8 b3_reverse_B;
    // __v4096i8 b4_reverse_B;
    // __v4096i8 b5_reverse_B;
    // __v4096i8 b6_reverse_B;
    // __v4096i8 b7_reverse_B;
    // __v4096i8 b8_reverse_B;
    // vclaim(b1_reverse_B);
    // vclaim(b2_reverse_B);
    // vclaim(b3_reverse_B);
    // vclaim(b4_reverse_B);
    // vclaim(b5_reverse_B);
    // vclaim(b6_reverse_B);
    // vclaim(b7_reverse_B);
    // vclaim(b8_reverse_B);

    // __v4096i8 b1_B;     //后向度量
    // __v4096i8 b2_B;
    // __v4096i8 b3_B;
    // __v4096i8 b4_B;
    // __v4096i8 b5_B;
    // __v4096i8 b6_B;
    // __v4096i8 b7_B;
    // __v4096i8 b8_B;
    // vclaim(b1_B);
    // vclaim(b2_B);
    // vclaim(b3_B);
    // vclaim(b4_B);
    // vclaim(b5_B);
    // vclaim(b6_B);
    // vclaim(b7_B);
    // vclaim(b8_B);



    // /**************** 拆分index 变量申明 *****************/
    // __v2048i16 split_index1;
    // __v2048i16 split_index2;
    // __v2048i16 split_index3;
    // __v2048i16 split_index4;
    // __v2048i16 split_index5;
    // __v2048i16 split_index6;
    // __v2048i16 split_index7;
    // __v2048i16 split_index8;
    // vclaim(split_index1);
    // vclaim(split_index2);
    // vclaim(split_index3);
    // vclaim(split_index4);
    // vclaim(split_index5);
    // vclaim(split_index6);
    // vclaim(split_index7);
    // vclaim(split_index8);

    // /**************** 似然比 变量申明 *****************/
    // __v4096i8 adb1_B;
    // __v4096i8 adb2_B;
    // __v4096i8 adb3_B;
    // __v4096i8 adb4_B;
    // __v4096i8 adb5_B;
    // __v4096i8 adb6_B;
    // __v4096i8 adb7_B;
    // __v4096i8 adb8_B;
    // vclaim(adb1_B);
    // vclaim(adb2_B);
    // vclaim(adb3_B);
    // vclaim(adb4_B);
    // vclaim(adb5_B);
    // vclaim(adb6_B);
    // vclaim(adb7_B);
    // vclaim(adb8_B);

    // __v4096i8 adb1_B_2;
    // __v4096i8 adb2_B_2;
    // __v4096i8 adb3_B_2;
    // __v4096i8 adb4_B_2;
    // __v4096i8 adb5_B_2;
    // __v4096i8 adb6_B_2;
    // __v4096i8 adb7_B_2;
    // __v4096i8 adb8_B_2;
    // vclaim(adb1_B_2);
    // vclaim(adb2_B_2);
    // vclaim(adb3_B_2);
    // vclaim(adb4_B_2);
    // vclaim(adb5_B_2);
    // vclaim(adb6_B_2);
    // vclaim(adb7_B_2);
    // vclaim(adb8_B_2);

    // __v4096i8 adbtemp1_B;
    // __v4096i8 adbtemp2_B;
    // vclaim(adbtemp1_B);
    // vclaim(adbtemp2_B);

    // __v4096i8 ltemp1_B;         //被减数
    // __v4096i8 ltemp2_B;         //减数
    // vclaim(ltemp1_B);
    // vclaim(ltemp2_B);



    /**************** 其他变量申明 *****************/
    __v4096i8 a_p_B;
    // __v4096i8 e_p_B;
    vclaim(a_p_B);
    // vclaim(e_p_B);


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
    __v4096i8 SW_temp2;
    // __v4096i8 SW_temp3;
    // __v4096i8 SW_temp4;
    // __v4096i8 SW_temp5;
    // __v4096i8 SW_temp6;
    // __v4096i8 SW_temp7;
    // __v4096i8 SW_temp8;
    vclaim(SW_temp);
    vclaim(SW_temp2);
    // vclaim(SW_temp3);
    // vclaim(SW_temp4);
    // vclaim(SW_temp5);
    // vclaim(SW_temp6);
    // vclaim(SW_temp7);
    // vclaim(SW_temp8);


    __v4096i8 pos64_8;      //阈值，2^{量化字长-2}=64，防上溢
    vclaim(pos64_8);

    __v4096i8 pos126_B;
    __v4096i8 pos64_B;
    vclaim(pos126_B);
    vclaim(pos64_B);

    __v4096i8 ones_B;
    vclaim(ones_B);

    __v4096i8 zero_8;
    vclaim(zero_8);
    __v2048i16 ones_8;
    __v2048i16 negones;;
    __v2048i16 zeros_8;
    vclaim(ones_8);
    vclaim(negones);
    vclaim(zeros_8);


    __v4096i8 m;
    __v4096i8 n;
    vclaim(m);
    vclaim(n);


    /**************************************************************** */
    /*************************************************************** */
    vbrdcst(zero_8,0,MASKREAD_OFF,8);
    vbrdcst(ones_8,1,MASKREAD_OFF,length);
    vbrdcst(negones,-1,MASKREAD_OFF,length);
    vbrdcst(zeros_8,0,MASKREAD_OFF,length);
    vbrdcst(pos64_8,64,MASKREAD_OFF,length);
    // vbrdcst(e_p_B,0,MASKREAD_OFF,B);


    vbrdcst(ad_vseqindex,1,MASKREAD_OFF,half_length);
    vbrdcst(bd_vseqindex,0,MASKREAD_OFF,half_length);
    vrange(SW_tempindex,half_length);
    SW_tempindex = vadd(SW_tempindex,half_length,MASKREAD_OFF,half_length);
    vshuffle(ad_vseqindex,SW_tempindex,bd_vseqindex,SHUFFLE_SCATTER,half_length);
    vshuffle(bd_vseqindex,SW_tempindex,ad_vseqindex,SHUFFLE_SCATTER,half_length);



    /****************************************************************** */
    // //拼接传入的向量
    // atemp1_index_8 = vsadd(atemp1_index,0,MASKREAD_OFF,half_length);
    // atemp2_index_8 = vsadd(atemp2_index,0,MASKREAD_OFF,half_length);
    // vrange(SW_tempindex2,half_length);
    // vshuffle(SW_temp,SW_tempindex2,state0_8,SHUFFLE_GATHER,half_length);    //初始状态拼接

    // SW_tempindex2 = vsadd(SW_tempindex2,half_length,MASKREAD_OFF,half_length);
    // btemp1_index = vsadd(btemp1_index,half_length,MASKREAD_OFF,half_length);
    // btemp2_index = vsadd(btemp2_index,half_length,MASKREAD_OFF,half_length);
    // vshuffle(atemp1_index_8,SW_tempindex2,btemp1_index,SHUFFLE_SCATTER,half_length);
    // vshuffle(atemp2_index_8,SW_tempindex2,btemp2_index,SHUFFLE_SCATTER,half_length);

    // //初始状态拼接
    // vshuffle(state0_8,SW_tempindex2,SW_temp,SHUFFLE_SCATTER,half_length);
    // state0_8 = vsadd(state0_8,0,MASKREAD_OFF,length);


    // /********************************************************************* */
    // //分支度量index
    // vrange(SW_tempindex,8);
    // SW_tempindex2 = vadd(a_add_index1,0,MASKREAD_OFF,8);
    // SW_tempindex3 = vadd(a_add_index2,0,MASKREAD_OFF,8);
    // SW_tempindex4 = vadd(a_add_index3,0,MASKREAD_OFF,8);
    // SW_tempindex5 = vadd(a_add_index4,0,MASKREAD_OFF,8);
    // for(short i=1;i<SW;i++){
    //     SW_tempindex = vadd(SW_tempindex,8,MASKREAD_OFF,8);
    //     SW_tempindex2 = vadd(SW_tempindex2,kminus,MASKREAD_OFF,8);
    //     SW_tempindex3 = vadd(SW_tempindex3,kminus,MASKREAD_OFF,8);
    //     SW_tempindex4 = vadd(SW_tempindex4,kminus,MASKREAD_OFF,8);
    //     SW_tempindex5 = vadd(SW_tempindex5,kminus,MASKREAD_OFF,8);
    //     vshuffle(a_add_index1,SW_tempindex,SW_tempindex2,SHUFFLE_SCATTER,8);
    //     vshuffle(a_add_index2,SW_tempindex,SW_tempindex3,SHUFFLE_SCATTER,8);
    //     vshuffle(a_add_index3,SW_tempindex,SW_tempindex4,SHUFFLE_SCATTER,8);
    //     vshuffle(a_add_index4,SW_tempindex,SW_tempindex5,SHUFFLE_SCATTER,8);
    // }
    // // a_add_index1 = vsadd(a_add_index1, 0,MASKREAD_OFF,length);  //
    // // a_add_index2 = vsadd(a_add_index2, 0,MASKREAD_OFF,length);
    // // a_add_index3 = vsadd(a_add_index3, 0,MASKREAD_OFF,length);
    // // a_add_index4 = vsadd(a_add_index4, 0,MASKREAD_OFF,length);  //63

    // SW_tempindex2 = vadd(b_add_index1,B-1,MASKREAD_OFF,8);
    // SW_tempindex3 = vadd(b_add_index2,B-1,MASKREAD_OFF,8);
    // SW_tempindex = vadd(SW_tempindex,8,MASKREAD_OFF,8);
    // vshuffle(a_add_index1,SW_tempindex,SW_tempindex2,SHUFFLE_SCATTER,8);
    // vshuffle(a_add_index2,SW_tempindex,SW_tempindex3,SHUFFLE_SCATTER,8);
    // // a_add_index1 = vsadd(a_add_index1, 0,MASKREAD_OFF,length);  //69
    // // a_add_index2 = vsadd(a_add_index2, 0,MASKREAD_OFF,length);

    // vbrdcst(b_add_index3,2000,MASKREAD_OFF,half_length);
    // for(short i=1;i<SW;i++){
    //     SW_tempindex = vadd(SW_tempindex,8,MASKREAD_OFF,8);
    //     SW_tempindex2 = vrsub(SW_tempindex2,kminus,MASKREAD_OFF,8);
    //     SW_tempindex3 = vrsub(SW_tempindex3,kminus,MASKREAD_OFF,8);
    //     vshuffle(a_add_index1,SW_tempindex,SW_tempindex2,SHUFFLE_SCATTER,8);
    //     vshuffle(a_add_index2,SW_tempindex,SW_tempindex3,SHUFFLE_SCATTER,8);
    // }
    // vrange(SW_tempindex4,half_length);
    // SW_tempindex4 = vadd(SW_tempindex4,half_length,MASKREAD_OFF,half_length);//
    // vshuffle(a_add_index3,SW_tempindex4,b_add_index3,SHUFFLE_SCATTER,half_length);
    // vshuffle(a_add_index4,SW_tempindex4,b_add_index3,SHUFFLE_SCATTER,half_length);
    // a_add_index5 = vsadd(a_add_index1, 0,MASKREAD_OFF,length);      //
    // a_add_index6 = vsadd(a_add_index2, 0,MASKREAD_OFF,length);
    // a_add_index7 = vsadd(a_add_index3, 0,MASKREAD_OFF,length);
    // a_add_index8 = vsadd(a_add_index4, 0,MASKREAD_OFF,length);




    /****************** 缩放数据 *******************/
    // softin1_x_B = vsra(softin1_x_B,3,MASKREAD_OFF,B);
    // softin1_y_B = vsra(softin1_y_B,3,MASKREAD_OFF,B);
    // softin2_x_B = vsra(softin2_x_B,3,MASKREAD_OFF,B);
    // softin2_y_B = vsra(softin2_y_B,3,MASKREAD_OFF,B);



    /******************************************************************************** */
    /******************************* SISO2 ************************************** */
    /*
        a_p(1:L_seq-m)=e_p(alphain);  %交织
        a_p(L_seq-m+1:L_seq)=0;  %尾比特部分不计算外部信息
    */
    vbrdcst(a_p_B,0,MASKREAD_OFF,B);
    vshuffle(a_p_B,alphain_N,e_p_B,SHUFFLE_GATHER,N);


    /***********************计算分支度量*********************** */
    /*
        d1(k) = -floor(0.75*(priori(k)+x(k)+y(k)));
        d2(k) = -floor(0.75*(priori(k)+x(k)-y(k)));
        d3(k) =  floor(0.75*(priori(k)+x(k)+y(k)));
        d4(k) =  floor(0.75*(priori(k)+x(k)-y(k)));
    */
    //初始化
    vbrdcst(d1_B,0,MASKREAD_OFF,fac);
    vbrdcst(d2_B,0,MASKREAD_OFF,fac);

    // softin2_x_B = vsra(softin2_x_B,2,MASKREAD_OFF,B);           //缩放数据
    // softin2_y_B = vsra(softin2_y_B,2,MASKREAD_OFF,B);

    d1_B = vsadd(softin2_x_B,softin2_y_B,MASKREAD_OFF,B);
    d1_B = vsadd(d1_B,a_p_B,MASKREAD_OFF,B);
    d1_B = vsra(d1_B,1,MASKREAD_OFF,B);
    d1_B = vmul(d1_B,3,MASKREAD_OFF,B);
    d1_B = vsra(d1_B,1,MASKREAD_OFF,B);
    //d3_B=d1_B; d1_B使用时用减号

    d2_B = vrsub(softin2_x_B,softin2_y_B,MASKREAD_OFF,B);
    d2_B = vsadd(d2_B,a_p_B,MASKREAD_OFF,B);
    d2_B = vsra(d2_B,1,MASKREAD_OFF,B);
    d2_B = vmul(d2_B,3,MASKREAD_OFF,B);
    d2_B = vsra(d2_B,1,MASKREAD_OFF,B);
    //d4_B=d2_B; d2_B使用时用减号


    // softin1_x_B = vsadd(softin1_x_B,0,MASKREAD_OFF,4096);
    // softin1_y_B = vsadd(softin1_y_B,0,MASKREAD_OFF,4096);
    softin2_x_B = vsadd(softin2_x_B,0,MASKREAD_OFF,fac);
    softin2_y_B = vsadd(softin2_y_B,0,MASKREAD_OFF,fac);



    vrange(SW_tempindex2,B);
    vshuffle(SW_temp2,SW_tempindex2,d1_B,SHUFFLE_GATHER,B);
    vbrdcst(d1_B,0,MASKREAD_OFF,fac);
    vshuffle(d1_B,SW_tempindex2,SW_temp2,SHUFFLE_GATHER,B);
    vshuffle(SW_temp2,SW_tempindex2,d2_B,SHUFFLE_GATHER,B);
    vbrdcst(d2_B,0,MASKREAD_OFF,fac);
    vshuffle(d2_B,SW_tempindex2,SW_temp2,SHUFFLE_GATHER,B);



    d1_B = vsadd(d1_B, 0,MASKREAD_OFF,fac);       //
    d2_B = vsadd(d2_B, 0,MASKREAD_OFF,fac);  




    // softin2_x_B = vsadd(softin2_x_B,0,MASKREAD_OFF,B);
    // softin2_y_B = vsadd(softin2_y_B,0,MASKREAD_OFF,B);

    // a_p_B = vsadd(a_p_B,0,MASKREAD_OFF,B);

    // d1_B = vsadd(d1_B, 0,MASKREAD_OFF,B);       //
    // d2_B = vsadd(d2_B, 0,MASKREAD_OFF,B);  




    /***********************计算前向后向度量*********************** */
    // //计算前向后向时，分支度量的index会被更新，所以在开头要重新初始化
    // a_add_index1 = vsadd(a_add_index5, 0,MASKREAD_OFF,length);  //
    // a_add_index2 = vsadd(a_add_index6, 0,MASKREAD_OFF,length);
    // a_add_index3 = vsadd(a_add_index7, 0,MASKREAD_OFF,length);
    // a_add_index4 = vsadd(a_add_index8, 0,MASKREAD_OFF,length);  //


    atemp = vsadd(state0_8, 0,MASKREAD_OFF,length);
    vrange(SW_tempindex2,length);         //用于把计算出来的值搬到atemp_8B上: 0,1,2,...,length-1
    

    // vbrdcst(d1_B_ksub1,0,MASKREAD_OFF,length);
    // vbrdcst(d2_B_ksub1,0,MASKREAD_OFF,length);
    // vbrdcst(d3_B_ksub1,0,MASKREAD_OFF,length);
    // vbrdcst(d4_B_ksub1,0,MASKREAD_OFF,length);

    // a_add_index1 = vsadd(a_add_index1, 0,MASKREAD_OFF,length);  //
    // a_add_index2 = vsadd(a_add_index2, 0,MASKREAD_OFF,length);
    // a_add_index3 = vsadd(a_add_index3, 0,MASKREAD_OFF,length);
    // a_add_index4 = vsadd(a_add_index4, 0,MASKREAD_OFF,length);  //


    for(short k=0; k<length7;k++){
    // for(short k=0; k<0;k++){
        //已有初始状态，只需要放入atemp
        if(k == 0){
            // vshuffle(atemp1_8B,SW_tempindex2,atemp,SHUFFLE_SCATTER,length);
            atemp1_8B = vsadd(atemp, 0,MASKREAD_OFF,length);
            SW_tempindex2 = vadd(SW_tempindex2,length,MASKREAD_OFF,length);     //length,length+1,...
        }


        if(k>0){
            /*  if k>1
                a(1,k)=max((a(1,k-1)+d1(k-1)),  (a(2,k-1)+d3(k-1))  );
                    a(2,k)=max((a(3,k-1)+d4(k-1)),  (a(4,k-1)+d2(k-1))  );
                    a(3,k)=max((a(5,k-1)+d2(k-1)),  (a(6,k-1)+d4(k-1))  );
                    a(4,k)=max((a(7,k-1)+d3(k-1)),  (a(8,k-1)+d1(k-1))  );
                    a(5,k)=max((a(1,k-1)+d3(k-1)),  (a(2,k-1)+d1(k-1))  );
                    a(6,k)=max((a(3,k-1)+d2(k-1)),  (a(4,k-1)+d4(k-1))  );
                    a(7,k)=max((a(5,k-1)+d4(k-1)),  (a(6,k-1)+d2(k-1))  );
                a(8,k)=max((a(7,k-1)+d1(k-1)),  (a(8,k-1)+d3(k-1))  );
            */
            /*
            for k=in_length:-1:1
                b(1,k)=max((b(1,k+1)+d1(k)),    (b(5,k+1)+d3(k))    );
                b(2,k)=max((b(5,k+1)+d1(k)),    (b(1,k+1)+d3(k))    );
                    b(3,k)=max((b(6,k+1)+d2(k)),    (b(2,k+1)+d4(k))    );
                    b(4,k)=max((b(2,k+1)+d2(k)),    (b(6,k+1)+d4(k))    );
                    b(5,k)=max((b(3,k+1)+d2(k)),    (b(7,k+1)+d4(k))    );
                    b(6,k)=max((b(7,k+1)+d2(k)),    (b(3,k+1)+d4(k))    );
                b(7,k)=max((b(8,k+1)+d1(k)),    (b(4,k+1)+d3(k))    );
                b(8,k)=max((b(4,k+1)+d1(k)),    (b(8,k+1)+d3(k))    );
            end
            */

            //step1:改变a[k-1]的顺序
            // atemp = vsadd(atemp, 0,MASKREAD_OFF,length);  //
            // atemp1_index_8 = vsadd(atemp1_index_8, 0,MASKREAD_OFF,length); 
            // atemp2_index_8 = vsadd(atemp2_index_8, 0,MASKREAD_OFF,length); 
            vshuffle(atemp1_8,atemp1_index_8,atemp,SHUFFLE_GATHER,length);      //97*
            // atemp1_8 = vsadd(atemp1_8, 0,MASKREAD_OFF,length);  //133
            vshuffle(atemp2_8,atemp2_index_8,atemp,SHUFFLE_GATHER,length);
            // atemp2_8 = vsadd(atemp2_8, 0,MASKREAD_OFF,length);  //



            // //step2:计算atemp1和atemp2 (用vsadd防止下溢出)
            // d1_B = vsadd(d1_B, 0,MASKREAD_OFF,B);   //136
            // d2_B = vsadd(d2_B, 0,MASKREAD_OFF,B); 
            // // // zeros_8 = vsadd(zeros_8, 0,MASKREAD_OFF,length); 
            a_add_index1 = vsadd(a_add_index1, 0,MASKREAD_OFF,length);  //65*  115*
            a_add_index2 = vsadd(a_add_index2, 0,MASKREAD_OFF,length);
            a_add_index3 = vsadd(a_add_index3, 0,MASKREAD_OFF,length);
            a_add_index4 = vsadd(a_add_index4, 0,MASKREAD_OFF,length);  //118*
            //atemp1
            vshuffle(d1_B_ksub1,a_add_index1,d1_B,SHUFFLE_GATHER,length);
            vshuffle(d2_B_ksub1,a_add_index2,d2_B,SHUFFLE_GATHER,length);
            vshuffle(d3_B_ksub1,a_add_index3,d1_B,SHUFFLE_GATHER,length);
            vshuffle(d4_B_ksub1,a_add_index4,d2_B,SHUFFLE_GATHER,length);   //122*
            // d3_B_ksub1 = vsadd(d3_B_ksub1, 0,MASKREAD_OFF,length);     //
            // d4_B_ksub1 = vsadd(d4_B_ksub1, 0,MASKREAD_OFF,length);  
            // vshuffle(d3_B_ksub1,SW_tempindex4,zeros_8,SHUFFLE_SCATTER,half_length);
            // vshuffle(d4_B_ksub1,SW_tempindex4,zeros_8,SHUFFLE_SCATTER,half_length);
            d1_B_ksub1 = vsadd(d1_B_ksub1, 0,MASKREAD_OFF,length);      //
            d2_B_ksub1 = vsadd(d2_B_ksub1, 0,MASKREAD_OFF,length);      //
            d3_B_ksub1 = vsadd(d3_B_ksub1, 0,MASKREAD_OFF,length);      //
            d4_B_ksub1 = vsadd(d4_B_ksub1, 0,MASKREAD_OFF,length);      //126*
            // atemp1_8 = vrsub(atemp1_8,d1_B_ksub1,MASKREAD_OFF,length);
            // atemp1_8 = vrsub(atemp1_8,d2_B_ksub1,MASKREAD_OFF,length);  //
            atemp1_8 = vsub(d1_B_ksub1,atemp1_8,MASKREAD_OFF,length);
            // atemp1_8 = vsadd(atemp1_8, 0,MASKREAD_OFF,length);  
            atemp1_8 = vsub(d2_B_ksub1,atemp1_8,MASKREAD_OFF,length);  //
            // atemp1_8 = vsadd(atemp1_8, 0,MASKREAD_OFF,length);  
            atemp1_8 = vsadd(atemp1_8,d3_B_ksub1,MASKREAD_OFF,length);  //
            // atemp1_8 = vsadd(atemp1_8, 0,MASKREAD_OFF,length);  
            atemp1_8 = vsadd(atemp1_8,d4_B_ksub1,MASKREAD_OFF,length);
            // atemp1_8 = vsadd(atemp1_8, 0,MASKREAD_OFF,length);  
            //atemp2
            vshuffle(d3_B_ksub1,a_add_index1,d1_B,SHUFFLE_GATHER,length);
            vshuffle(d4_B_ksub1,a_add_index2,d2_B,SHUFFLE_GATHER,length);
            vshuffle(d1_B_ksub1,a_add_index3,d1_B,SHUFFLE_GATHER,length);
            vshuffle(d2_B_ksub1,a_add_index4,d2_B,SHUFFLE_GATHER,length);
            // d1_B_ksub1 = vsadd(d1_B_ksub1, 0,MASKREAD_OFF,length);      //
            // d2_B_ksub1 = vsadd(d2_B_ksub1, 0,MASKREAD_OFF,length);      //
            // d3_B_ksub1 = vsadd(d3_B_ksub1, 0,MASKREAD_OFF,length);      //
            // d4_B_ksub1 = vsadd(d4_B_ksub1, 0,MASKREAD_OFF,length);      //
            atemp2_8 = vsub(d1_B_ksub1,atemp2_8,MASKREAD_OFF,length);
            atemp2_8 = vsub(d2_B_ksub1,atemp2_8,MASKREAD_OFF,length);
            atemp2_8 = vsadd(atemp2_8,d3_B_ksub1,MASKREAD_OFF,length);
            atemp2_8 = vsadd(atemp2_8,d4_B_ksub1,MASKREAD_OFF,length);      //138*
            //index++
            vsle(ad_vseqindex,bd_vseqindex,MASKREAD_OFF,MASKWRITE_ON,length);
            // a_add_index1 = vsadd(a_add_index1, 1,MASKREAD_ON,length);       //
            // a_add_index2 = vsadd(a_add_index2, 1,MASKREAD_ON,length);
            // a_add_index3 = vsadd(a_add_index3, 1,MASKREAD_ON,length);
            // a_add_index4 = vsadd(a_add_index4, 1,MASKREAD_ON,length);       //
            a_add_index1 = vsadd(a_add_index1,ones_8,MASKREAD_ON,length);       // 82* 140*
            a_add_index2 = vsadd(a_add_index2,ones_8,MASKREAD_ON,length);
            a_add_index3 = vsadd(a_add_index3,ones_8,MASKREAD_ON,length);
            a_add_index4 = vsadd(a_add_index4,ones_8,MASKREAD_ON,length);       //85* 143*
            //index--
            vsle(bd_vseqindex,ad_vseqindex,MASKREAD_OFF,MASKWRITE_ON,length);  //86
            // ones_8 = vsadd(ones_8,0,MASKREAD_OFF,length);                   // 87
            // a_add_index1 = vsub(ones_8,a_add_index1,MASKREAD_ON,length);   
            // a_add_index1 = vsadd(a_add_index1, 0,MASKREAD_OFF,length);  

            // a_add_index1 = vrsub(a_add_index1,ones_8,MASKREAD_ON,length);        // 87*/90       
            // a_add_index2 = vsub(a_add_index2,ones_8,MASKREAD_ON,length);
            // a_add_index3 = vsub(a_add_index3,ones_8,MASKREAD_ON,length);                                                                                                                                                                                                                                             
            // a_add_index4 = vsub(a_add_index4,ones_8,MASKREAD_ON,length);

            a_add_index1 = vsadd(a_add_index1,negones,MASKREAD_ON,length); 
            a_add_index2 = vsadd(a_add_index2,negones,MASKREAD_ON,length); 
            a_add_index3 = vsadd(a_add_index3,negones,MASKREAD_ON,length); 
            a_add_index4 = vsadd(a_add_index4,negones,MASKREAD_ON,length); 

            // a_add_index1 = vrsub(a_add_index1,ones_8,MASKREAD_ON,length);
            // a_add_index2 = vrsub(a_add_index2,ones_8,MASKREAD_ON,length);
            // a_add_index3 = vrsub(a_add_index3,ones_8,MASKREAD_ON,length);
            // a_add_index4 = vrsub(a_add_index4,ones_8,MASKREAD_ON,length);

            // ad_vseqindex = vsadd(ad_vseqindex,0,MASKREAD_OFF,length);       //
            // bd_vseqindex = vsadd(bd_vseqindex,0,MASKREAD_OFF,length);       //
            a_add_index1 = vsadd(a_add_index1, 0,MASKREAD_OFF,length);      //91* 150*
            a_add_index2 = vsadd(a_add_index2, 0,MASKREAD_OFF,length);
            a_add_index3 = vsadd(a_add_index3, 0,MASKREAD_OFF,length);
            a_add_index4 = vsadd(a_add_index4, 0,MASKREAD_OFF,length);      //153*
            // atemp1_8 = vsadd(atemp1_8, 0,MASKREAD_OFF,length);     //
            // atemp2_8 = vsadd(atemp2_8, 0,MASKREAD_OFF,length);     //



            //step3:max(atemp1_8,atemp2_8),作为a[k]
            vsgt(atemp1_8,atemp2_8,MASKREAD_OFF,MASKWRITE_ON,length);    //atemp1_8小于等于atemp2_8
            // vsgt(atemp2_8,atemp1_8,MASKREAD_OFF,MASKWRITE_ON,length);    //atemp1_8小于等于atemp2_8？？反的？？？？？
            // SW_temp2 = vsgt(atemp1_8,atemp2_8,MASKREAD_OFF,MASKWRITE_OFF,length); 
            atemp1_8 = vxor(atemp1_8, atemp1_8, MASKREAD_ON, length);
            atemp1_8 = vsadd(atemp1_8,0, MASKREAD_OFF, length); 
            // atemp1_8 = vsadd(atemp1_8, atemp2_8, MASKREAD_ON, length);   //得到a[k]
            atemp1_8 = vsadd(atemp1_8,atemp2_8, MASKREAD_ON, length);   //得到a[k]
            atemp = vsadd(atemp1_8,0, MASKREAD_OFF, length);            //1592  1465

            //step4:防上溢出,大于64则减去64
            // m = vslt(atemp1_8,pos64_8,MASKREAD_OFF,MASKWRITE_OFF,length);     //atemp1_8大于64
            m = vsle(pos64_8,atemp1_8,MASKREAD_OFF,MASKWRITE_OFF,length);     //atemp1_8小于等于64的位置为1
            n = vredmin8(m,MASKREAD_OFF,length);            // 101*   160*
            char cmp_temp_n;
            vbarrier();
            VSPM_OPEN();
            cmp_temp_n = *(volatile unsigned char *) (vaddr(n));
            VSPM_CLOSE();
            if(cmp_temp_n ==0){     //若m中的最小值为0，表明atemp1_8存在大于64的值
                //判定前向是否溢出
                vrange(SW_tempindex,half_length);
                vshuffle(SW_temp,SW_tempindex,atemp1_8,SHUFFLE_GATHER,half_length);
                m = vsle(pos64_8,SW_temp,MASKREAD_OFF,MASKWRITE_OFF,half_length);
                n = vredmin8(m,MASKREAD_OFF,half_length);
                vbarrier();
                VSPM_OPEN();
                cmp_temp_n = *(volatile unsigned char *) (vaddr(n));
                VSPM_CLOSE();
                if(cmp_temp_n ==0){
                    for(short i=0;i<SW;i++){
                        if(i==0){
                            vrange(SW_tempindex3,8);
                        }
                        vshuffle(SW_temp2,SW_tempindex3,atemp1_8,SHUFFLE_GATHER,8);
                        m = vsle(pos64_8,SW_temp2,MASKREAD_OFF,MASKWRITE_OFF,8);
                        n = vredmin8(m,MASKREAD_OFF,8);
                        vbarrier();
                        VSPM_OPEN();
                        cmp_temp_n = *(volatile unsigned char *) (vaddr(n));
                        VSPM_CLOSE();
                        if(cmp_temp_n ==0){
                            // printf("a_k=%d\t",&k);
                            // printf("a_i=%d\n",&i);
                            // SW_temp2 = vrsub(SW_temp2,pos64_8,MASKREAD_OFF,8);
                            // vshuffle(atemp1_8,SW_tempindex2,SW_temp2,SHUFFLE_SCATTER,8);
                            vshuffle(atemp1_8,SW_tempindex3,vrsub(SW_temp2,pos64_8,MASKREAD_OFF,8),SHUFFLE_SCATTER,8);
                        }
                        SW_tempindex3 = vadd(SW_tempindex3, 8,MASKREAD_OFF,8);
                    }
                }
                //判定后向是否溢出
                SW_tempindex = vsadd(SW_tempindex,half_length,MASKREAD_OFF,half_length);        //1639 1503
                vshuffle(SW_temp,SW_tempindex,atemp1_8,SHUFFLE_GATHER,half_length);
                m = vsle(pos64_8,SW_temp,MASKREAD_OFF,MASKWRITE_OFF,half_length);
                n = vredmin8(m,MASKREAD_OFF,half_length);
                vbarrier();
                VSPM_OPEN();
                cmp_temp_n = *(volatile unsigned char *) (vaddr(n));
                VSPM_CLOSE();
                if(cmp_temp_n ==0){
                    for(short i=0;i<SW;i++){
                        if(i==0){
                            vrange(SW_tempindex3,8);
                            SW_tempindex3 = vadd(SW_tempindex3,half_length,MASKREAD_OFF,8);
                        }
                        vshuffle(SW_temp2,SW_tempindex3,atemp1_8,SHUFFLE_GATHER,8);
                        m = vsle(pos64_8,SW_temp2,MASKREAD_OFF,MASKWRITE_OFF,8);
                        n = vredmin8(m,MASKREAD_OFF,8);
                        vbarrier();
                        VSPM_OPEN();
                        cmp_temp_n = *(volatile unsigned char *) (vaddr(n));
                        VSPM_CLOSE();
                        if(cmp_temp_n ==0){
                            // printf("b_k=%d\t",&k);
                            // printf("b_i=%d\n",&i);
                            // SW_temp2 = vrsub(SW_temp2,pos64_8,MASKREAD_OFF,8);
                            // vshuffle(atemp1_8,SW_tempindex2,SW_temp2,SHUFFLE_SCATTER,8);
                            vshuffle(atemp1_8,SW_tempindex3,vrsub(SW_temp2,pos64_8,MASKREAD_OFF,8),SHUFFLE_SCATTER,8);
                        }
                        SW_tempindex3 = vadd(SW_tempindex3, 8,MASKREAD_OFF,8);
                    }
                }

            }
            atemp = vsadd(atemp1_8, 0,MASKREAD_OFF,length); //1675  1545 
            // atemp1_index_8 = vsadd(atemp1_index_8, 0,MASKREAD_OFF,length);
            // atemp2_index_8 = vsadd(atemp2_index_8, 0,MASKREAD_OFF,length);



            //step5:移到atemp_8B上，index++
            //直接按顺序先放在temp上，后面再拆分(一段刚好能存1024bit*8个状态=8192)
            // if(k< length1 ){         //第一段
                vshuffle(atemp1_8B,SW_tempindex2,atemp1_8,SHUFFLE_SCATTER,length);
                // if(k < length1-1){
                    SW_tempindex2 = vadd(SW_tempindex2,length,MASKREAD_OFF,length);
            //     }else if(k== length1-1){
            //         vrange(SW_tempindex2,length);     //从头开始：0,1,2,...,length-1
            //     }
            //     // flag=1;
            // }else if(k> length1-1 && k< l2){     //第二段
            //     vshuffle(atemp2_8B,SW_tempindex2,atemp1_8,SHUFFLE_SCATTER,length);
            //     if(k< l2-1){
            //         SW_tempindex2 = vadd(SW_tempindex2,length,MASKREAD_OFF,length);
            //     }else if(k== l2-1){
            //         vrange(SW_tempindex2,length);     //从头开始：0,1,2,...,length-1
            //     }
            //     // flag=2;
            // }else if(k> l2-1 && k< l3 ){     //第三段
            //     vshuffle(atemp3_8B,SW_tempindex2,atemp1_8,SHUFFLE_SCATTER,length);
            //     if(k< l3-1){
            //         SW_tempindex2 = vadd(SW_tempindex2,length,MASKREAD_OFF,length);
            //     }else if(k== l3-1){
            //         vrange(SW_tempindex2,length);     //从头开始：0,1,2,...,length-1
            //     }
            //     // flag=3;
            // }else if(k> l3-1 && k< l4){     //第四段
            //     vshuffle(atemp4_8B,SW_tempindex2,atemp1_8,SHUFFLE_SCATTER,length);
            //     if(k< l4-1){
            //         SW_tempindex2 = vadd(SW_tempindex2,length,MASKREAD_OFF,length);
            //     }else if(k== l4-1){
            //         vrange(SW_tempindex2,length);     //从头开始：0,1,2,...,length-1
            //     }
            //     // flag=4;
            // }else if(k> l4-1 && k< l5){     //第五段
            //     vshuffle(atemp5_8B,SW_tempindex2,atemp1_8,SHUFFLE_SCATTER,length);
            //     if(k< l5-1){
            //         SW_tempindex2 = vadd(SW_tempindex2,length,MASKREAD_OFF,length);
            //     }else if(k== l5-1){
            //         vrange(SW_tempindex2,length);     //从头开始：0,1,2,...,length-1
            //     }
            //     // flag=5;
            // }else if(k> l5-1 && k< l6){     //第六段
            //     vshuffle(atemp6_8B,SW_tempindex2,atemp1_8,SHUFFLE_SCATTER,length);
            //     if(k< l6-1){
            //         SW_tempindex2 = vadd(SW_tempindex2,length,MASKREAD_OFF,length);
            //     }else if(k== l6-1){
            //         vrange(SW_tempindex2,length);     //从头开始：0,1,2,...,length-1
            //     }
            //     // flag=6;
            // }else if(k> l6-1 && k< l7){     //第七段
            //     vshuffle(atemp7_8B,SW_tempindex2,atemp1_8,SHUFFLE_SCATTER,length);
            //     SW_tempindex2 = vadd(SW_tempindex2,length,MASKREAD_OFF,length);
            //     // flag=7;
            // }


        }
    }
    atemp1_8B = vsadd(atemp1_8B,0,MASKREAD_OFF,sum);








    // char words[30] = "SISO2 a b computing finished";
    // printf("-------%s---------\n",&words);

    __v4096i8 softin2_x_B_out;
    vclaim(softin2_x_B_out);
    // vbrdcst(softin2_x_B_out,0,MASKREAD_OFF,10);
    softin2_x_B_out = vsadd(softin2_x_B,0,MASKREAD_OFF,B);
    
    // softin2_x_B = vsadd(softin2_x_B,0,MASKREAD_OFF,B);
    // softin2_y_B = vsadd(softin2_y_B,0,MASKREAD_OFF,B);



    // a_p_B =  vsadd(a_p_B,0,MASKREAD_OFF,B);
    // d1_B = vsadd(d1_B,0,MASKREAD_OFF,B);
    // d2_B = vsadd(d2_B,0,MASKREAD_OFF,B);

    vreturn(atemp1_8B,sizeof(atemp1_8B),softin2_x_B_out,sizeof(softin2_x_B_out), d1_B,sizeof(d1_B),d2_B,sizeof(d2_B) , a_p_B,sizeof(a_p_B), e_p_B,sizeof(e_p_B));

}