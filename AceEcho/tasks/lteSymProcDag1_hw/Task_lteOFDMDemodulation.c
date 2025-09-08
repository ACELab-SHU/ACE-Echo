#include "riscv_printf.h"
#include "venus.h"


typedef short __v2048i16 __attribute__((ext_vector_type(2048)));
typedef char  __v2208i8 __attribute__((ext_vector_type(2208)));


// ------------------------- Set fixed point -------------------------
// // 2048点FFT的定点化 [6 6 5 5 5 5 4 3 2 1 0]
// short fft_fixed_vec[11] = {8, 7, 7, 7, 8, 7, 7, 8, 8, 8, 8};
// short fft_shift_vec[11] = {1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1};

// 2048点FFT的定点化 [6 5 5 4 4 3 3 2 2 1 1]
short fft_fixed_vec[11] = {8, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7};
short fft_shift_vec[11] = {1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

// 1024点fft的定点化 [6 6 6 5 5 4 4 3 2 2]
short fft1024_fixed_vec[10] = {8, 7, 7, 8, 7, 8, 7, 8, 8, 7};
short fft1024_shift_vec[10] = {1, 0, 0, 1, 0, 1, 0, 1, 1, 0};

// 512点的fft定点化 [6 5 5 5 4 4 4]///待确定
short fft512_fixed_vec[9] = {8, 8, 8, 7, 7, 8, 7, 7,7};
// short fft128_fixed_vec[7] = {8, 8, 7, 7, 8, 7, 7};
short fft512_shift_vec[9] = {1, 1, 1, 0, 0, 1, 0, 0,0};

// 256点的fft定点化 [6 5 5 5 4 4 4]///待确定
short fft256_fixed_vec[8] = {8, 8, 8, 7, 7, 8, 7, 7};
// short fft128_fixed_vec[7] = {8, 8, 7, 7, 8, 7, 7};
short fft256_shift_vec[8] = {1, 1, 1, 0, 0, 1, 0, 0};

// 128点的fft定点化 [6 5 5 5 4 4 4]
short fft128_fixed_vec[7] = {8, 8, 7, 7, 8, 7, 7};
// short fft128_fixed_vec[7] = {8, 8, 7, 7, 8, 7, 7};
short fft128_shift_vec[7] = {1, 1, 0, 0, 1, 0, 0};

// 输入定点化的点数，实现定点乘法的输出
VENUS_INLINE __v2208i8 MUL2048_8_FIXED(__v2208i8 a, __v2208i8 b, int fix_point, int length) {
  __v2208i8 result;
  vsetshamt(fix_point);
  result = vmul(a, b, MASKREAD_OFF, length);
  vsetshamt(0);
  return result;
};

typedef struct {
  short data;
} __attribute__((aligned(64))) short_struct;

int Task_lteOFDMDemodulation(__v2208i8 vin_real, __v2208i8 vin_imag,short_struct symbolNum, __v2208i8 cos_stage0, __v2208i8 cos_stage1, __v2208i8 cos_stage2, __v2208i8 cos_stage3, __v2208i8 cos_stage4, __v2208i8 cos_stage5, __v2208i8 cos_stage6, __v2208i8 cos_stage7, __v2208i8 cos_stage8, __v2208i8 cos_stage9, __v2208i8 cos_stage10, __v2208i8 sin_stage0, __v2208i8 sin_stage1, __v2208i8 sin_stage2, __v2208i8 sin_stage3, __v2208i8 sin_stage4, __v2208i8 sin_stage5, __v2208i8 sin_stage6, __v2208i8 sin_stage7, __v2208i8 sin_stage8, __v2208i8 sin_stage9, __v2208i8 sin_stage10, __v2048i16 shuffle_add_stage0, __v2048i16 shuffle_add_stage1, __v2048i16 shuffle_add_stage2, __v2048i16 shuffle_add_stage3, __v2048i16 shuffle_add_stage4, __v2048i16 shuffle_add_stage5, __v2048i16 shuffle_add_stage6, __v2048i16 shuffle_add_stage7, __v2048i16 shuffle_add_stage8, __v2048i16 shuffle_add_stage9, __v2048i16 shuffle_add_stage10, __v2048i16 shuffle_wn_stage0, __v2048i16 shuffle_wn_stage1, __v2048i16 shuffle_wn_stage2, __v2048i16 shuffle_wn_stage3, __v2048i16 shuffle_wn_stage4, __v2048i16 shuffle_wn_stage5, __v2048i16 shuffle_wn_stage6, __v2048i16 shuffle_wn_stage7, __v2048i16 shuffle_wn_stage8, __v2048i16 shuffle_wn_stage9, __v2048i16 shuffle_wn_stage10, short_struct in_nrb, __v2208i8 phase_r, __v2208i8 phase_i, __v2048i16 targetIndices) {

  // input parameter
  uint8_t  symbol_num = symbolNum.data;
  short    nrb        = in_nrb.data;


  if ((symbol_num <= 13) && (symbol_num >= 0)) {
    int N_FFT         = 0;
    int cp_length     = 0;
    int symbol_offset = 0;
    int shuffle_flag    = 0;

    if (nrb == 100) 
    {
      N_FFT = 2048;
      shuffle_flag = 0;
      if (symbol_num == 0 || symbol_num == 7) {
        cp_length     = 160;
        symbol_offset = 80;
      } else {
        cp_length     = 144;
        symbol_offset = 72;
      }
    } 
    else if (nrb == 75)
    {
      N_FFT = 2048;
      shuffle_flag = 0;
      if (symbol_num == 0 || symbol_num == 7) {
        cp_length     = 160;
        symbol_offset = 80;
      } else {
        cp_length     = 144;
        symbol_offset = 72;
      }
    }
    else if (nrb == 50) 
    {
      N_FFT = 1024;
      shuffle_flag = 1;
      if (symbol_num == 0 || symbol_num == 7) {
        cp_length     = 80;
        symbol_offset = 40;
      } else {
        cp_length     = 72;
        symbol_offset = 36;
      }
    }
    else if (nrb == 25) 
    {
      N_FFT = 512;
      shuffle_flag = 2;
      if (symbol_num == 0 || symbol_num == 7) {
        cp_length     = 40;
        symbol_offset = 20;
      } else {
        cp_length     = 36;
        symbol_offset = 18;
      }
    }
    else if (nrb == 15) 
    {
      N_FFT = 256;
      shuffle_flag = 3;
      if (symbol_num == 0 || symbol_num == 7) {
        cp_length     = 20;
        symbol_offset = 10;
      } else {
        cp_length     = 18;
        symbol_offset = 9;
      }
    }
    else if (nrb == 6) 
    {
      N_FFT = 128;
      shuffle_flag = 4;
      if (symbol_num == 0 || symbol_num == 7) {
        cp_length     = 10;
        symbol_offset = 5;
      } else {
        cp_length     = 9;
        symbol_offset = 4;
      }
    }


    // --------STEP 1 : Remove CP

    __v2048i16 Remove_CP_Index;
    vrange(Remove_CP_Index, N_FFT);
    Remove_CP_Index = vsadd(Remove_CP_Index, symbol_offset, MASKREAD_OFF, N_FFT);
    // __v2048i16 shift_CP;
    // vbrdcst(shift_CP, N_FFT, MASKREAD_OFF, N_FFT);
    // vbrdcst(shift_CP, 0, MASKREAD_OFF, N_FFT - symbol_offset);
    // Remove_CP_Index = vrsub(Remove_CP_Index, shift_CP, MASKREAD_OFF, N_FFT);

    __v2208i8 Data_without_CP_real;
    __v2208i8 Data_without_CP_imag;
    vclaim(Data_without_CP_real);
    vclaim(Data_without_CP_imag);

    __v2208i8 tempin_real;
    __v2208i8 tempin_imag;
    vclaim(tempin_real);
    vclaim(tempin_imag);
    __v2048i16 desample_index;
    vclaim(desample_index);
    vrange(desample_index, 2048);
    if(shuffle_flag==0){
      vshuffle(Data_without_CP_real, Remove_CP_Index, vin_real, SHUFFLE_GATHER, N_FFT);
      vshuffle(Data_without_CP_imag, Remove_CP_Index, vin_imag, SHUFFLE_GATHER, N_FFT);
    }
    else if (shuffle_flag==1)
    {
      desample_index = vsll(desample_index, 1, MASKREAD_OFF, 1104);
      vshuffle(tempin_real, desample_index, vin_real, SHUFFLE_GATHER, 1104);
      vshuffle(tempin_imag, desample_index, vin_imag, SHUFFLE_GATHER, 1104);
      vshuffle(Data_without_CP_real, Remove_CP_Index, tempin_real, SHUFFLE_GATHER, N_FFT);
      vshuffle(Data_without_CP_imag, Remove_CP_Index, tempin_imag, SHUFFLE_GATHER, N_FFT);
    }
    else if (shuffle_flag==2)
    {
      desample_index = vsll(desample_index, 2, MASKREAD_OFF, 552);
      vshuffle(tempin_real, desample_index, vin_real, SHUFFLE_GATHER, 552);
      vshuffle(tempin_imag, desample_index, vin_imag, SHUFFLE_GATHER, 552);
      vshuffle(Data_without_CP_real, Remove_CP_Index, tempin_real, SHUFFLE_GATHER, N_FFT);
      vshuffle(Data_without_CP_imag, Remove_CP_Index, tempin_imag, SHUFFLE_GATHER, N_FFT);
    }
    else if (shuffle_flag==3)
    {
      desample_index = vsll(desample_index, 3, MASKREAD_OFF, 276);
      vshuffle(tempin_real, desample_index, vin_real, SHUFFLE_GATHER, 276);
      vshuffle(tempin_imag, desample_index, vin_imag, SHUFFLE_GATHER, 276);
      vshuffle(Data_without_CP_real, Remove_CP_Index, tempin_real, SHUFFLE_GATHER, N_FFT);
      vshuffle(Data_without_CP_imag, Remove_CP_Index, tempin_imag, SHUFFLE_GATHER, N_FFT);
    }
    else if (shuffle_flag==4)
    {
      desample_index = vsll(desample_index, 4, MASKREAD_OFF, 138);
      vshuffle(tempin_real, desample_index, vin_real, SHUFFLE_GATHER, 138);
      vshuffle(tempin_imag, desample_index, vin_imag, SHUFFLE_GATHER, 138);
      vshuffle(Data_without_CP_real, Remove_CP_Index, tempin_real, SHUFFLE_GATHER, N_FFT);
      vshuffle(Data_without_CP_imag, Remove_CP_Index, tempin_imag, SHUFFLE_GATHER, N_FFT);
    }


    //  STEP 1 'END'---------

    //  --------STEP 2 : FFT

    __v2208i8 OFDM_OutReal;
    __v2208i8 OFDM_OutImag;
    vclaim(OFDM_OutReal);
    vclaim(OFDM_OutImag);
    short calculate_length = 0;
    if (N_FFT == 2048) 
    {
      calculate_length = 1024;
    } 
    else if (N_FFT == 1024) 
    {
      calculate_length = 512;
    }
    else if (N_FFT == 512)
    {
      calculate_length = 256;
    }
    else if (N_FFT == 256)
    {
      calculate_length = 128;
    }
    else if (N_FFT == 128)
    {
      calculate_length = 64;
    }
    else
    {
      calculate_length = 1024;
    }

    // 从2048点FFT的Wn中提取n点的Wn的shuffle_Index
    __v2048i16 shuffle_for_1024_Wn;
    vclaim(shuffle_for_1024_Wn);
    __v2048i16 shuffle_for_512_Wn;
    vclaim(shuffle_for_512_Wn);
    __v2048i16 shuffle_for_256_Wn;
    vclaim(shuffle_for_256_Wn);
    __v2048i16 shuffle_for_128_Wn;
    vclaim(shuffle_for_128_Wn);

    vrange(shuffle_for_1024_Wn, 2048);
    shuffle_for_1024_Wn = vsll(shuffle_for_1024_Wn, 1, MASKREAD_OFF, 2048);

    shuffle_for_512_Wn = vsll(shuffle_for_1024_Wn, 1, MASKREAD_OFF, 2048);

    shuffle_for_256_Wn = vsll(shuffle_for_512_Wn, 1, MASKREAD_OFF, 2048);

    shuffle_for_128_Wn = vsll(shuffle_for_256_Wn, 1, MASKREAD_OFF, 2048);


    __v2208i8 Wn_cos;
    vclaim(Wn_cos);
    __v2208i8 Wn_sin;
    vclaim(Wn_sin);


    // 向量搬移index
    __v2048i16 copy_2048; // copy_2048 = [0 1 2 ... 2047]
    vclaim(copy_2048);
    vrange(copy_2048, 2048);
    // __v2048i16 move_2048to1024; // move_2048to1024 = [1024 1025 1026 ... 3071]
    // move_2048to1024 = vsadd(copy_2048, 1024, MASKREAD_OFF, 2048);
    // __v2048i16 move_1024to512; // move_1024to512 = [512 513 514 ... 2559]
    // move_1024to512 = vsadd(copy_2048, 512, MASKREAD_OFF, 2048);

    __v2048i16 moveHalf2Front; // move_128to64 = [64 65 66 ... 127]
    moveHalf2Front = vadd(copy_2048, calculate_length, MASKREAD_OFF, calculate_length);

    //  进行计算的四个向量分别为    Data_without_CP_real（up）
    //  Data_without_CP_imag（up）    data_real_down data_imag_down
    __v2208i8 data_real_down;
    __v2208i8 data_imag_down;
    vclaim(data_real_down);
    vclaim(data_imag_down);


    // FFT每一级计算过程中的中间变量
    __v2208i8 tempAddResult_real;
    vclaim(tempAddResult_real);
    __v2208i8 tempAddResult_imag;
    vclaim(tempAddResult_imag);
    __v2208i8 tempWnResult_real;
    vclaim(tempWnResult_real);
    __v2208i8 tempWnResult_imag;
    vclaim(tempWnResult_imag);

    __v2208i8 a_sub_b_real;
    vclaim(a_sub_b_real);
    __v2208i8 a_sub_b_imag;
    vclaim(a_sub_b_imag);
    __v2208i8 cos_tempWnResult;
    __v2208i8 sin_tempWnResult;
    vclaim(cos_tempWnResult);
    vclaim(sin_tempWnResult);

    static int fraction = 0;

    if (N_FFT == 2048) {
      // 蝶形计算
      // a ------- (a + b)
      //      |
      // b ------- (a - b)Wn

      //  Stage 0------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft_shift_vec[0], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft_shift_vec[0], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, cos_stage0, fft_fixed_vec[0], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, sin_stage0, fft_fixed_vec[0], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, cos_stage0, fft_fixed_vec[0], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, sin_stage0, fft_fixed_vec[0], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage0, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage0, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage0, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage0, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 1------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft_shift_vec[1], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft_shift_vec[1], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, cos_stage1, fft_fixed_vec[1], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, sin_stage1, fft_fixed_vec[1], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, cos_stage1, fft_fixed_vec[1], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, sin_stage1, fft_fixed_vec[1], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET

      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage1, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage1, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage1, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage1, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 2------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft_shift_vec[2], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft_shift_vec[2], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, cos_stage2, fft_fixed_vec[2], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, sin_stage2, fft_fixed_vec[2], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, cos_stage2, fft_fixed_vec[2], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, sin_stage2, fft_fixed_vec[2], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage2, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage2, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage2, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage2, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 3------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft_shift_vec[3], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft_shift_vec[3], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, cos_stage3, fft_fixed_vec[3], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, sin_stage3, fft_fixed_vec[3], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, cos_stage3, fft_fixed_vec[3], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, sin_stage3, fft_fixed_vec[3], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage3, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage3, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage3, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage3, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 4------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft_shift_vec[4], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft_shift_vec[4], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, cos_stage4, fft_fixed_vec[4], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, sin_stage4, fft_fixed_vec[4], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, cos_stage4, fft_fixed_vec[4], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, sin_stage4, fft_fixed_vec[4], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage4, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage4, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage4, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage4, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 5------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft_shift_vec[5], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft_shift_vec[5], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, cos_stage5, fft_fixed_vec[5], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, sin_stage5, fft_fixed_vec[5], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, cos_stage5, fft_fixed_vec[5], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, sin_stage5, fft_fixed_vec[5], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage5, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage5, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage5, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage5, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 6------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft_shift_vec[6], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft_shift_vec[6], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, cos_stage6, fft_fixed_vec[6], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, sin_stage6, fft_fixed_vec[6], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, cos_stage6, fft_fixed_vec[6], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, sin_stage6, fft_fixed_vec[6], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage6, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage6, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage6, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage6, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 7------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft_shift_vec[7], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft_shift_vec[7], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, cos_stage7, fft_fixed_vec[7], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, sin_stage7, fft_fixed_vec[7], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, cos_stage7, fft_fixed_vec[7], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, sin_stage7, fft_fixed_vec[7], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET

      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage7, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage7, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage7, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage7, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 8------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft_shift_vec[8], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft_shift_vec[8], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, cos_stage8, fft_fixed_vec[8], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, sin_stage8, fft_fixed_vec[8], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, cos_stage8, fft_fixed_vec[8], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, sin_stage8, fft_fixed_vec[8], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET

      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage8, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage8, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage8, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage8, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 9------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft_shift_vec[9], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft_shift_vec[9], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, cos_stage9, fft_fixed_vec[9], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, sin_stage9, fft_fixed_vec[9], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, cos_stage9, fft_fixed_vec[9], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, sin_stage9, fft_fixed_vec[9], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage9, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage9, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage9, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage9, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 10------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft_shift_vec[10], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft_shift_vec[10], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, cos_stage10, fft_fixed_vec[10], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, sin_stage10, fft_fixed_vec[10], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, cos_stage10, fft_fixed_vec[10], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, sin_stage10, fft_fixed_vec[10], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);


      // 重排序
      vshuffle(OFDM_OutReal, shuffle_add_stage10, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(OFDM_OutReal, shuffle_wn_stage10, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(OFDM_OutImag, shuffle_add_stage10, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(OFDM_OutImag, shuffle_wn_stage10, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------
    }
    else if (N_FFT == 1024) {
      //  Stage 0------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_1024_Wn, cos_stage1, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_1024_Wn, sin_stage1, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft1024_shift_vec[0], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft1024_shift_vec[0], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_cos, fft1024_fixed_vec[0], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_sin, fft1024_fixed_vec[0], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_cos, fft1024_fixed_vec[0], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_sin, fft1024_fixed_vec[0], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage0, tempAddResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_real, shuffle_wn_stage0, tempWnResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_add_stage0, tempAddResult_imag, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage0, tempWnResult_imag, SHUFFLE_SCATTER, 1024);
      //---------------------------------------------

      //  Stage 1------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_1024_Wn, cos_stage2, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_1024_Wn, sin_stage2, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft1024_shift_vec[1], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft1024_shift_vec[1], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_cos, fft1024_fixed_vec[1], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_sin, fft1024_fixed_vec[1], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_cos, fft1024_fixed_vec[1], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_sin, fft1024_fixed_vec[1], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage1, tempAddResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_real, shuffle_wn_stage1, tempWnResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_add_stage1, tempAddResult_imag, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage1, tempWnResult_imag, SHUFFLE_SCATTER, 1024);
      //---------------------------------------------

      //  Stage 2------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_1024_Wn, cos_stage3, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_1024_Wn, sin_stage3, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft1024_shift_vec[2], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft1024_shift_vec[2], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_cos, fft1024_fixed_vec[2], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_sin, fft1024_fixed_vec[2], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_cos, fft1024_fixed_vec[2], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_sin, fft1024_fixed_vec[2], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage2, tempAddResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_real, shuffle_wn_stage2, tempWnResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_add_stage2, tempAddResult_imag, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage2, tempWnResult_imag, SHUFFLE_SCATTER, 1024);
      //---------------------------------------------

      //  Stage 3------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_1024_Wn, cos_stage4, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_1024_Wn, sin_stage4, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft1024_shift_vec[3], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft1024_shift_vec[3], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_cos, fft1024_fixed_vec[3], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_sin, fft1024_fixed_vec[3], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_cos, fft1024_fixed_vec[3], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_sin, fft1024_fixed_vec[3], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage3, tempAddResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_real, shuffle_wn_stage3, tempWnResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_add_stage3, tempAddResult_imag, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage3, tempWnResult_imag, SHUFFLE_SCATTER, 1024);
      //---------------------------------------------

      //  Stage 4------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_1024_Wn, cos_stage5, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_1024_Wn, sin_stage5, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft1024_shift_vec[4], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft1024_shift_vec[4], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_cos, fft1024_fixed_vec[4], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_sin, fft1024_fixed_vec[4], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_cos, fft1024_fixed_vec[4], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_sin, fft1024_fixed_vec[4], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage4, tempAddResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_real, shuffle_wn_stage4, tempWnResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_add_stage4, tempAddResult_imag, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage4, tempWnResult_imag, SHUFFLE_SCATTER, 1024);
      //---------------------------------------------

      //  Stage 5------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_1024_Wn, cos_stage6, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_1024_Wn, sin_stage6, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft1024_shift_vec[5], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft1024_shift_vec[5], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_cos, fft1024_fixed_vec[5], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_sin, fft1024_fixed_vec[5], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_cos, fft1024_fixed_vec[5], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_sin, fft1024_fixed_vec[5], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage5, tempAddResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_real, shuffle_wn_stage5, tempWnResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_add_stage5, tempAddResult_imag, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage5, tempWnResult_imag, SHUFFLE_SCATTER, 1024);
      //---------------------------------------------

      //  Stage 6------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_1024_Wn, cos_stage7, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_1024_Wn, sin_stage7, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft1024_shift_vec[6], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft1024_shift_vec[6], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_cos, fft1024_fixed_vec[6], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_sin, fft1024_fixed_vec[6], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_cos, fft1024_fixed_vec[6], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_sin, fft1024_fixed_vec[6], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage6, tempAddResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_real, shuffle_wn_stage6, tempWnResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_add_stage6, tempAddResult_imag, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage6, tempWnResult_imag, SHUFFLE_SCATTER, 1024);
      //---------------------------------------------

      //  Stage 7------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_1024_Wn, cos_stage8, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_1024_Wn, sin_stage8, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft1024_shift_vec[7], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft1024_shift_vec[7], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_cos, fft1024_fixed_vec[7], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_sin, fft1024_fixed_vec[7], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_cos, fft1024_fixed_vec[7], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_sin, fft1024_fixed_vec[7], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage7, tempAddResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_real, shuffle_wn_stage7, tempWnResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_add_stage7, tempAddResult_imag, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage7, tempWnResult_imag, SHUFFLE_SCATTER, 1024);
      //---------------------------------------------

      //  Stage 8------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_1024_Wn, cos_stage9, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_1024_Wn, sin_stage9, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft1024_shift_vec[8], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft1024_shift_vec[8], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_cos, fft1024_fixed_vec[8], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_sin, fft1024_fixed_vec[8], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_cos, fft1024_fixed_vec[8], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_sin, fft1024_fixed_vec[8], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage8, tempAddResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_real, shuffle_wn_stage8, tempWnResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_add_stage8, tempAddResult_imag, SHUFFLE_SCATTER, 1024);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage8, tempWnResult_imag, SHUFFLE_SCATTER, 1024);
      //---------------------------------------------

      //  Stage 9------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_1024_Wn, cos_stage10, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_1024_Wn, sin_stage10, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft1024_shift_vec[9], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft1024_shift_vec[9], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_cos, fft1024_fixed_vec[9], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_sin, fft1024_fixed_vec[9], calculate_length);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

      cos_tempWnResult  = MUL2048_8_FIXED(a_sub_b_imag, Wn_cos, fft1024_fixed_vec[9], calculate_length);
      sin_tempWnResult  = MUL2048_8_FIXED(a_sub_b_real, Wn_sin, fft1024_fixed_vec[9], calculate_length);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);


      // 重排序
      vshuffle(OFDM_OutReal, shuffle_wn_stage9, tempWnResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(OFDM_OutReal, shuffle_add_stage9, tempAddResult_real, SHUFFLE_SCATTER, 1024);
      vshuffle(OFDM_OutImag, shuffle_wn_stage9, tempWnResult_imag, SHUFFLE_SCATTER, 1024);
      vshuffle(OFDM_OutImag, shuffle_add_stage9, tempAddResult_imag, SHUFFLE_SCATTER, 1024);
      //---------------------------------------------

    }
    else if (N_FFT == 512) {

      //  Stage 0------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_512_Wn, cos_stage2, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_512_Wn, sin_stage2, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft512_shift_vec[0], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft512_shift_vec[0], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft512_fixed_vec[0];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage0, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage0, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage0, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage0, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 1------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_512_Wn, cos_stage3, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_512_Wn, sin_stage3, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft512_shift_vec[1], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft512_shift_vec[1], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft512_fixed_vec[1];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage1, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage1, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage1, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage1, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 2------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_512_Wn, cos_stage4, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_512_Wn, sin_stage4, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft512_shift_vec[2], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft512_shift_vec[2], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft512_fixed_vec[2];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage2, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage2, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage2, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage2, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 3------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_512_Wn, cos_stage5, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_512_Wn, sin_stage5, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft512_shift_vec[3], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft512_shift_vec[3], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft512_fixed_vec[3];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage3, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage3, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage3, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage3, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 4------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_512_Wn, cos_stage6, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_512_Wn, sin_stage6, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft512_shift_vec[4], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft512_shift_vec[4], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft512_fixed_vec[4];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage4, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage4, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage4, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage4, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 5------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_512_Wn, cos_stage7, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_512_Wn, sin_stage7, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft512_shift_vec[5], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft512_shift_vec[5], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft512_fixed_vec[5];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET

      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage5, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage5, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage5, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage5, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 6------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_512_Wn, cos_stage8, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_512_Wn, sin_stage8, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft512_shift_vec[6], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft512_shift_vec[6], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft512_fixed_vec[6];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET

      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage6, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage6, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage6, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage6, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 7------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_512_Wn, cos_stage9, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_512_Wn, sin_stage9, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft512_shift_vec[7], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft512_shift_vec[7], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft512_fixed_vec[7];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET

      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage7, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage7, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage7, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage7, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 8------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_512_Wn, cos_stage10, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_512_Wn, sin_stage10, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft512_shift_vec[8], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft512_shift_vec[8], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft512_fixed_vec[8];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);


      // 重排序
      vshuffle(OFDM_OutReal, shuffle_add_stage8, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(OFDM_OutReal, shuffle_wn_stage8, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(OFDM_OutImag, shuffle_add_stage8, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(OFDM_OutImag, shuffle_wn_stage8, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------
  
    }
    else if (N_FFT == 256) {

      //  Stage 0------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_256_Wn, cos_stage3, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_256_Wn, sin_stage3, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft256_shift_vec[0], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft256_shift_vec[0], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft256_fixed_vec[0];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage0, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage0, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage0, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage0, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 1------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_256_Wn, cos_stage4, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_256_Wn, sin_stage4, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft256_shift_vec[1], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft256_shift_vec[1], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft256_fixed_vec[1];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage1, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage1, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage1, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage1, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 2------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_256_Wn, cos_stage5, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_256_Wn, sin_stage5, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft256_shift_vec[2], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft256_shift_vec[2], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft256_fixed_vec[2];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage2, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage2, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage2, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage2, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 3------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_256_Wn, cos_stage6, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_256_Wn, sin_stage6, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft256_shift_vec[3], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft256_shift_vec[3], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft256_fixed_vec[3];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage3, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage3, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage3, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage3, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 4------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_256_Wn, cos_stage7, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_256_Wn, sin_stage7, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft256_shift_vec[4], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft256_shift_vec[4], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft256_fixed_vec[4];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage4, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage4, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage4, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage4, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 5------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_256_Wn, cos_stage8, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_256_Wn, sin_stage8, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft256_shift_vec[5], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft256_shift_vec[5], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft256_fixed_vec[5];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET

      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage5, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage5, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage5, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage5, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 6------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_256_Wn, cos_stage9, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_256_Wn, sin_stage9, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft256_shift_vec[6], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft256_shift_vec[6], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft256_fixed_vec[6];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
      tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


      // 重排序
      vshuffle(Data_without_CP_real, shuffle_add_stage6, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_real, shuffle_wn_stage6, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_add_stage6, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(Data_without_CP_imag, shuffle_wn_stage6, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------

      //  Stage 7------------------------------------
      vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
      vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_cos, shuffle_for_256_Wn, cos_stage10, SHUFFLE_GATHER, calculate_length);
      vshuffle(Wn_sin, shuffle_for_256_Wn, sin_stage10, SHUFFLE_GATHER, calculate_length);
      //  a + b
      tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
      tempAddResult_real = vsra(tempAddResult_real, fft256_shift_vec[7], MASKREAD_OFF, calculate_length);
      tempAddResult_imag = vsra(tempAddResult_imag, fft256_shift_vec[7], MASKREAD_OFF, calculate_length);

      // (a - b)Wn    (可用复数计算代替)
      a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
      a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
  
      fraction = fft256_fixed_vec[7];
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);
  
      vsetshamt(fraction);
      cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
      sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
      vsetshamt(0);
      tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);

      // fft shift
      vshuffle(OFDM_OutReal, shuffle_add_stage7, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(OFDM_OutReal, shuffle_wn_stage7, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
      vshuffle(OFDM_OutImag, shuffle_add_stage7, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
      vshuffle(OFDM_OutImag, shuffle_wn_stage7, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
      //---------------------------------------------
  
    }
    else if (N_FFT == 128) {
    //  Stage 0------------------------------------
    vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
    vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_cos, shuffle_for_128_Wn, cos_stage4, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_sin, shuffle_for_128_Wn, sin_stage4, SHUFFLE_GATHER, calculate_length);
    //  a + b
    tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
    tempAddResult_real = vsra(tempAddResult_real, fft128_shift_vec[0], MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsra(tempAddResult_imag, fft128_shift_vec[0], MASKREAD_OFF, calculate_length);

    // (a - b)Wn    (可用复数计算代替)
    a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
    a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);
    
    fraction = fft128_fixed_vec[0];
    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);

    tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
    tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


    // 重排序
    vshuffle(Data_without_CP_real, shuffle_add_stage0, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_real, shuffle_wn_stage0, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_add_stage0, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_wn_stage0, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
    //---------------------------------------------

    //  Stage 1------------------------------------
    vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
    vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_cos, shuffle_for_128_Wn, cos_stage5, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_sin, shuffle_for_128_Wn, sin_stage5, SHUFFLE_GATHER, calculate_length);
    //  a + b
    tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
    tempAddResult_real = vsra(tempAddResult_real, fft128_shift_vec[1], MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsra(tempAddResult_imag, fft128_shift_vec[1], MASKREAD_OFF, calculate_length);

    // (a - b)Wn    (可用复数计算代替)
    a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
    a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);


    fraction = fft128_fixed_vec[1];
    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
    tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET

    // 重排序
    vshuffle(Data_without_CP_real, shuffle_add_stage1, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_real, shuffle_wn_stage1, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_add_stage1, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_wn_stage1, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
    //---------------------------------------------

    //  Stage 2------------------------------------
    vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
    vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_cos, shuffle_for_128_Wn, cos_stage6, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_sin, shuffle_for_128_Wn, sin_stage6, SHUFFLE_GATHER, calculate_length);
    //  a + b
    tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
    tempAddResult_real = vsra(tempAddResult_real, fft128_shift_vec[2], MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsra(tempAddResult_imag, fft128_shift_vec[2], MASKREAD_OFF, calculate_length);

    // (a - b)Wn    (可用复数计算代替)
    a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
    a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

    fraction = fft128_fixed_vec[2];
    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
    tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


    // 重排序
    vshuffle(Data_without_CP_real, shuffle_add_stage2, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_real, shuffle_wn_stage2, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_add_stage2, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_wn_stage2, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
    //---------------------------------------------

    //  Stage 3------------------------------------
    vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
    vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_cos, shuffle_for_128_Wn, cos_stage7, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_sin, shuffle_for_128_Wn, sin_stage7, SHUFFLE_GATHER, calculate_length);
    //  a + b
    tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
    tempAddResult_real = vsra(tempAddResult_real, fft128_shift_vec[3], MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsra(tempAddResult_imag, fft128_shift_vec[3], MASKREAD_OFF, calculate_length);

    // (a - b)Wn    (可用复数计算代替)
    a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
    a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

    fraction = fft128_fixed_vec[3];
    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
    tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET

    // 重排序
    vshuffle(Data_without_CP_real, shuffle_add_stage3, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_real, shuffle_wn_stage3, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_add_stage3, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_wn_stage3, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
    //---------------------------------------------

    //  Stage 4------------------------------------
    vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
    vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_cos, shuffle_for_128_Wn, cos_stage8, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_sin, shuffle_for_128_Wn, sin_stage8, SHUFFLE_GATHER, calculate_length);
    //  a + b
    tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
    tempAddResult_real = vsra(tempAddResult_real, fft128_shift_vec[4], MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsra(tempAddResult_imag, fft128_shift_vec[4], MASKREAD_OFF, calculate_length);

    // (a - b)Wn    (可用复数计算代替)
    a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
    a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

    fraction = fft128_fixed_vec[4];
    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
    tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET

    // 重排序
    vshuffle(Data_without_CP_real, shuffle_add_stage4, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_real, shuffle_wn_stage4, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_add_stage4, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_wn_stage4, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
    //---------------------------------------------

    //  Stage 5------------------------------------
    vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
    vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_cos, shuffle_for_128_Wn, cos_stage9, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_sin, shuffle_for_128_Wn, sin_stage9, SHUFFLE_GATHER, calculate_length);
    //  a + b
    tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
    tempAddResult_real = vsra(tempAddResult_real, fft128_shift_vec[5], MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsra(tempAddResult_imag, fft128_shift_vec[5], MASKREAD_OFF, calculate_length);

    // (a - b)Wn    (可用复数计算代替)
    a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
    a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

    fraction = fft128_fixed_vec[5];
    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);


    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);
    tempWnResult_imag = vsadd(tempWnResult_imag, 1, MASKREAD_OFF, calculate_length); // Function FOR OFFSET


    // 重排序
    vshuffle(Data_without_CP_real, shuffle_add_stage5, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_real, shuffle_wn_stage5, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_add_stage5, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
    vshuffle(Data_without_CP_imag, shuffle_wn_stage5, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
    //---------------------------------------------

    //  Stage 6------------------------------------
    vshuffle(data_real_down, moveHalf2Front, Data_without_CP_real, SHUFFLE_GATHER, calculate_length);
    vshuffle(data_imag_down, moveHalf2Front, Data_without_CP_imag, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_cos, shuffle_for_128_Wn, cos_stage10, SHUFFLE_GATHER, calculate_length);
    vshuffle(Wn_sin, shuffle_for_128_Wn, sin_stage10, SHUFFLE_GATHER, calculate_length);
    //  a + b
    tempAddResult_real = vsadd(Data_without_CP_real, data_real_down, MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsadd(Data_without_CP_imag, data_imag_down, MASKREAD_OFF, calculate_length);
    tempAddResult_real = vsra(tempAddResult_real, fft128_shift_vec[6], MASKREAD_OFF, calculate_length);
    tempAddResult_imag = vsra(tempAddResult_imag, fft128_shift_vec[6], MASKREAD_OFF, calculate_length);

    // (a - b)Wn    (可用复数计算代替)
    a_sub_b_real = vssub(data_real_down, Data_without_CP_real, MASKREAD_OFF, calculate_length);
    a_sub_b_imag = vssub(data_imag_down, Data_without_CP_imag, MASKREAD_OFF, calculate_length);

    fraction = fft128_fixed_vec[6];
    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_real, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_imag, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_real = vssub(sin_tempWnResult, cos_tempWnResult, MASKREAD_OFF, calculate_length);

    vsetshamt(fraction);
    cos_tempWnResult = vmul(a_sub_b_imag, Wn_cos, MASKREAD_OFF, calculate_length);
    sin_tempWnResult = vmul(a_sub_b_real, Wn_sin, MASKREAD_OFF, calculate_length);
    vsetshamt(0);
    tempWnResult_imag = vsadd(cos_tempWnResult, sin_tempWnResult, MASKREAD_OFF, calculate_length);

    // fft shift
    vshuffle(OFDM_OutReal, shuffle_add_stage6, tempAddResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(OFDM_OutReal, shuffle_wn_stage6, tempWnResult_real, SHUFFLE_SCATTER, calculate_length);
    vshuffle(OFDM_OutImag, shuffle_add_stage6, tempAddResult_imag, SHUFFLE_SCATTER, calculate_length);
    vshuffle(OFDM_OutImag, shuffle_wn_stage6, tempWnResult_imag, SHUFFLE_SCATTER, calculate_length);
    //---------------------------------------------

    }
//////需要修改，先不做fft_shift,相位补偿完再shift


    __v2208i8 real_temp1;
    __v2208i8 real_temp2;
    __v2208i8 imag_temp1;
    __v2208i8 imag_temp2;

    vsetshamt(7);
    real_temp1 = vmul(OFDM_OutReal, phase_r, MASKREAD_OFF, N_FFT);
    real_temp2 = vmul(OFDM_OutImag, phase_i, MASKREAD_OFF, N_FFT);
    imag_temp1 = vmul(OFDM_OutReal, phase_i, MASKREAD_OFF, N_FFT);
    imag_temp2 = vmul(OFDM_OutImag, phase_r, MASKREAD_OFF, N_FFT);
    vsetshamt(0);

    OFDM_OutReal = vsadd(real_temp2, real_temp1, MASKREAD_OFF, N_FFT);
    OFDM_OutImag = vssub(imag_temp1, imag_temp2, MASKREAD_OFF, N_FFT);

    //fft_shift
    __v2048i16 shift_index;
    vclaim(shift_index);
    vrange(shift_index,2048);
    shift_index = vsadd(shift_index,calculate_length,MASKREAD_OFF,calculate_length);

    __v2208i8 out_real;
    __v2208i8 out_imag;
    vclaim(out_real);
    vclaim(out_imag);
    vshuffle(out_real, shift_index, OFDM_OutReal, SHUFFLE_GATHER,  calculate_length);
    vshuffle(out_real, shift_index, OFDM_OutReal, SHUFFLE_SCATTER, calculate_length);
    vshuffle(out_imag, shift_index, OFDM_OutImag, SHUFFLE_GATHER,  calculate_length);
    vshuffle(out_imag, shift_index, OFDM_OutImag, SHUFFLE_SCATTER, calculate_length);


    __v2208i8 out_r;
    __v2208i8 out_i;
    vclaim(out_r);
    vclaim(out_i);
    vshuffle(out_r,targetIndices,out_real, SHUFFLE_GATHER,nrb*12);
    vshuffle(out_i,targetIndices,out_imag, SHUFFLE_GATHER,nrb*12);

    vreturn(out_r, 1216, out_i, 1216);



  }
}