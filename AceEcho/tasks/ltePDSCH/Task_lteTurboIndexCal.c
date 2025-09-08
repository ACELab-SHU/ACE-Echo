// 计算lte Turbo译码时需要的一些index
/*
 * @input   
 * @input   
 * @output  
 *
 * Created by wangqianli
*/


#include "venus.h"
#include <stdint.h>
#include "vmath.h"
#include "riscv_printf.h"


// typedef short __v2048i16 __attribute__((ext_vector_type(4096)));        //index变量用__v2048i16
// typedef char  __v4096i8 __attribute__((ext_vector_type(8192)));         //数据用4096i8
typedef short __v2048i16 __attribute__((ext_vector_type(5000)));        //index变量用__v2048i16
typedef char  __v4096i8 __attribute__((ext_vector_type(5000)));         //数据用4096i8



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


int Task_lteTurboIndexCal(
    // __v4096i8 softin1_x_B,__v4096i8 softin1_y_B,
    // __v4096i8 softin2_x_B,__v4096i8 softin2_y_B,
    __v4096i8 state0_8,
    // __v2048i16 alphain_N,
    __v2048i16 a_add_index1,__v2048i16 a_add_index2,__v2048i16 a_add_index3,__v2048i16 a_add_index4,
    __v2048i16 b_add_index1,__v2048i16 b_add_index2,
    __v2048i16 atemp1_index,__v2048i16 atemp2_index,__v2048i16 btemp1_index,__v2048i16 btemp2_index,
    short_struct trblklength,short_struct SW_value ){


    short reg  = 4;                         //寄存器个数
    short trblklen = trblklength.data;      //原始信息长度
    short N  = trblklen + 24;               //码块长度
    short B  = reg+N;   
    int fac = 5000;
    printf("B = %hd\n",&B);    

    short ii = 3 ;                  //迭代次数
    short SW = SW_value.data;       //分的段数
    printf("trblklen = %hd\n",&trblklen);
    printf("SW = %hd\n",&SW);


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


    // short d_bias;
    // short l2=2*length1;
    // short l3=3*length1;
    // short l4=4*length1;
    // short l5=5*length1;
    // short l6=6*length1;
    // short l7=7*length1;

    // short j=0;
    // int sum =  length7 * 8 * SW * 2;
    // // int fac = 8192;
    // // int flag = sum*10 /fac;
    // // if(flag %10 ==0){
    // //     flag = sum/fac;
    // // }else{
    // //     flag = sum/fac + 1;
    // // }
    // // printf("flag = %d\n",&flag);



    /**************** 分支度量 变量申明 *****************/
    // __v4096i8 d1_B;     //分支量度
    // __v4096i8 d2_B;
    // __v4096i8 d3_B;
    // __v4096i8 d4_B;
    // vclaim(d1_B);
    // vclaim(d2_B);
    // vclaim(d3_B);
    // vclaim(d4_B);

    // __v4096i8 d1_B_ksub1;
    // __v4096i8 d2_B_ksub1;
    // __v4096i8 d3_B_ksub1;
    // __v4096i8 d4_B_ksub1;
    // vclaim(d1_B_ksub1);
    // vclaim(d2_B_ksub1);
    // vclaim(d3_B_ksub1);
    // vclaim(d4_B_ksub1);

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

    __v2048i16 atemp1_index_8;              //加分支度量前，需要变换前向度量的顺序
    __v2048i16 atemp2_index_8;              //加分支度量前，需要变换前向度量的顺序
    vclaim(atemp1_index_8);
    vclaim(atemp2_index_8);


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
    vclaim(SW_temp);


    /****************************************************************** */
    //拼接传入的向量
    atemp1_index_8 = vsadd(atemp1_index,0,MASKREAD_OFF,half_length);
    atemp2_index_8 = vsadd(atemp2_index,0,MASKREAD_OFF,half_length);
    vrange(SW_tempindex2,half_length);
    vshuffle(SW_temp,SW_tempindex2,state0_8,SHUFFLE_GATHER,half_length);    //初始状态拼接

    SW_tempindex2 = vsadd(SW_tempindex2,half_length,MASKREAD_OFF,half_length);
    btemp1_index = vsadd(btemp1_index,half_length,MASKREAD_OFF,half_length);
    btemp2_index = vsadd(btemp2_index,half_length,MASKREAD_OFF,half_length);
    vshuffle(atemp1_index_8,SW_tempindex2,btemp1_index,SHUFFLE_SCATTER,half_length);
    vshuffle(atemp2_index_8,SW_tempindex2,btemp2_index,SHUFFLE_SCATTER,half_length);

    //初始状态拼接
    vshuffle(state0_8,SW_tempindex2,SW_temp,SHUFFLE_SCATTER,half_length);
    state0_8 = vsadd(state0_8,0,MASKREAD_OFF,length);

    /********************************************************************* */
    //分支度量index
    vrange(SW_tempindex,8);
    SW_tempindex2 = vadd(a_add_index1,0,MASKREAD_OFF,8);
    SW_tempindex3 = vadd(a_add_index2,0,MASKREAD_OFF,8);
    SW_tempindex4 = vadd(a_add_index3,0,MASKREAD_OFF,8);
    SW_tempindex5 = vadd(a_add_index4,0,MASKREAD_OFF,8);
    for(short i=1;i<SW;i++){
        SW_tempindex = vadd(SW_tempindex,8,MASKREAD_OFF,8);
        SW_tempindex2 = vadd(SW_tempindex2,kminus,MASKREAD_OFF,8);
        SW_tempindex3 = vadd(SW_tempindex3,kminus,MASKREAD_OFF,8);
        SW_tempindex4 = vadd(SW_tempindex4,kminus,MASKREAD_OFF,8);
        SW_tempindex5 = vadd(SW_tempindex5,kminus,MASKREAD_OFF,8);
        vshuffle(a_add_index1,SW_tempindex,SW_tempindex2,SHUFFLE_SCATTER,8);
        vshuffle(a_add_index2,SW_tempindex,SW_tempindex3,SHUFFLE_SCATTER,8);
        vshuffle(a_add_index3,SW_tempindex,SW_tempindex4,SHUFFLE_SCATTER,8);
        vshuffle(a_add_index4,SW_tempindex,SW_tempindex5,SHUFFLE_SCATTER,8);
    }
    // a_add_index1 = vsadd(a_add_index1, 0,MASKREAD_OFF,length);  //
    // a_add_index2 = vsadd(a_add_index2, 0,MASKREAD_OFF,length);
    // a_add_index3 = vsadd(a_add_index3, 0,MASKREAD_OFF,length);
    // a_add_index4 = vsadd(a_add_index4, 0,MASKREAD_OFF,length);  //63

    SW_tempindex2 = vadd(b_add_index1,B-1,MASKREAD_OFF,8);
    SW_tempindex3 = vadd(b_add_index2,B-1,MASKREAD_OFF,8);
    SW_tempindex = vadd(SW_tempindex,8,MASKREAD_OFF,8);
    vshuffle(a_add_index1,SW_tempindex,SW_tempindex2,SHUFFLE_SCATTER,8);
    vshuffle(a_add_index2,SW_tempindex,SW_tempindex3,SHUFFLE_SCATTER,8);
    // a_add_index1 = vsadd(a_add_index1, 0,MASKREAD_OFF,length);  //69
    // a_add_index2 = vsadd(a_add_index2, 0,MASKREAD_OFF,length);

    vbrdcst(b_add_index3,2000,MASKREAD_OFF,half_length);
    for(short i=1;i<SW;i++){
        SW_tempindex = vadd(SW_tempindex,8,MASKREAD_OFF,8);
        SW_tempindex2 = vrsub(SW_tempindex2,kminus,MASKREAD_OFF,8);
        SW_tempindex3 = vrsub(SW_tempindex3,kminus,MASKREAD_OFF,8);
        vshuffle(a_add_index1,SW_tempindex,SW_tempindex2,SHUFFLE_SCATTER,8);
        vshuffle(a_add_index2,SW_tempindex,SW_tempindex3,SHUFFLE_SCATTER,8);
    }
    vrange(SW_tempindex4,half_length);
    SW_tempindex4 = vadd(SW_tempindex4,half_length,MASKREAD_OFF,half_length);//
    vshuffle(a_add_index3,SW_tempindex4,b_add_index3,SHUFFLE_SCATTER,half_length);
    vshuffle(a_add_index4,SW_tempindex4,b_add_index3,SHUFFLE_SCATTER,half_length);
    a_add_index5 = vsadd(a_add_index1, 0,MASKREAD_OFF,length);      //
    a_add_index6 = vsadd(a_add_index2, 0,MASKREAD_OFF,length);
    a_add_index7 = vsadd(a_add_index3, 0,MASKREAD_OFF,length);
    a_add_index8 = vsadd(a_add_index4, 0,MASKREAD_OFF,length);








    vreturn(a_add_index5,sizeof(a_add_index5), a_add_index6,sizeof(a_add_index6), a_add_index7,sizeof(a_add_index7), a_add_index8,sizeof(a_add_index8), state0_8,sizeof(state0_8) , atemp1_index_8,sizeof(atemp1_index_8), atemp2_index_8,sizeof(atemp2_index_8));        
}