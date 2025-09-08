/**
 * *****************************************************************************
 * @file        Task_lteSubFrame5Split.c
 * @brief       preprocess the DFE input data
 * @author      Yihao Shen (shenyihao@shu.edu.cn)
 * @date        2025-07-25
 * @copyright   ACE_Lab (Shanghai University), All Rights Reserved.
 * *****************************************************************************
 */
#include "riscv_printf.h"
#include "venus.h"

typedef char __v8192i8 __attribute__((ext_vector_type(8192)));
typedef short __v4096i16 __attribute__((ext_vector_type(4096)));
typedef char __v4096i8 __attribute__((ext_vector_type(4096)));
typedef short __v2048i16 __attribute__((ext_vector_type(2048)));

int symbolLength = 2192;
int symbolLength_07 = 2208;


int Task_lteSubFrame5Split(__v8192i8 dfe_input_0, __v8192i8 dfe_input_1, __v8192i8 dfe_input_2, __v8192i8 dfe_input_3,
                           __v8192i8 dfe_input_4, __v8192i8 dfe_input_5, __v8192i8 dfe_input_6) {

  // generate shuffle index
  __v4096i16 shuffle_index;
  vclaim(shuffle_index);
  vrange(shuffle_index, symbolLength_07);
  shuffle_index = vmul(shuffle_index, 2, MASKREAD_OFF, symbolLength_07);
  shuffle_index = vadd(shuffle_index, 4, MASKREAD_OFF, symbolLength_07);

  __v8192i8 dfe_output_real_0;
  __v8192i8 dfe_output_real_1;
  __v8192i8 dfe_output_real_2;
  __v8192i8 dfe_output_real_3;
  __v8192i8 dfe_output_real_4;
  __v8192i8 dfe_output_real_5;
  __v8192i8 dfe_output_real_6;

  __v8192i8 dfe_output_imag_0;
  __v8192i8 dfe_output_imag_1;
  __v8192i8 dfe_output_imag_2;
  __v8192i8 dfe_output_imag_3;
  __v8192i8 dfe_output_imag_4;
  __v8192i8 dfe_output_imag_5;
  __v8192i8 dfe_output_imag_6;

  vclaim(dfe_output_real_0);
  vclaim(dfe_output_real_1);
  vclaim(dfe_output_real_2);
  vclaim(dfe_output_real_3);
  vclaim(dfe_output_real_4);
  vclaim(dfe_output_real_5);
  vclaim(dfe_output_real_6);


  vclaim(dfe_output_imag_0);
  vclaim(dfe_output_imag_1);
  vclaim(dfe_output_imag_2);
  vclaim(dfe_output_imag_3);
  vclaim(dfe_output_imag_4);
  vclaim(dfe_output_imag_5);
  vclaim(dfe_output_imag_6);


  // extract real data
  vshuffle(dfe_output_real_0, shuffle_index, dfe_input_0, SHUFFLE_GATHER, symbolLength_07);
  vshuffle(dfe_output_real_1, shuffle_index, dfe_input_1, SHUFFLE_GATHER, symbolLength);
  vshuffle(dfe_output_real_2, shuffle_index, dfe_input_2, SHUFFLE_GATHER, symbolLength);
  vshuffle(dfe_output_real_3, shuffle_index, dfe_input_3, SHUFFLE_GATHER, symbolLength);
  vshuffle(dfe_output_real_4, shuffle_index, dfe_input_4, SHUFFLE_GATHER, symbolLength);
  vshuffle(dfe_output_real_5, shuffle_index, dfe_input_5, SHUFFLE_GATHER, symbolLength);
  vshuffle(dfe_output_real_6, shuffle_index, dfe_input_6, SHUFFLE_GATHER, symbolLength);

  // extract imag data
  shuffle_index = vsadd(shuffle_index, 1, MASKREAD_OFF, symbolLength_07);
  vshuffle(dfe_output_imag_0, shuffle_index, dfe_input_0, SHUFFLE_GATHER, symbolLength_07);
  vshuffle(dfe_output_imag_1, shuffle_index, dfe_input_1, SHUFFLE_GATHER, symbolLength);
  vshuffle(dfe_output_imag_2, shuffle_index, dfe_input_2, SHUFFLE_GATHER, symbolLength);
  vshuffle(dfe_output_imag_3, shuffle_index, dfe_input_3, SHUFFLE_GATHER, symbolLength);
  vshuffle(dfe_output_imag_4, shuffle_index, dfe_input_4, SHUFFLE_GATHER, symbolLength);
  vshuffle(dfe_output_imag_5, shuffle_index, dfe_input_5, SHUFFLE_GATHER, symbolLength);
  vshuffle(dfe_output_imag_6, shuffle_index, dfe_input_6, SHUFFLE_GATHER, symbolLength);

  vreturn(dfe_output_real_0, symbolLength_07, dfe_output_imag_0, symbolLength_07, 
          dfe_output_real_1, symbolLength, dfe_output_imag_1, symbolLength,
          dfe_output_real_2, symbolLength, dfe_output_imag_2, symbolLength,
          dfe_output_real_3, symbolLength, dfe_output_imag_3, symbolLength,
          dfe_output_real_4, symbolLength, dfe_output_imag_4, symbolLength,
          dfe_output_real_5, symbolLength, dfe_output_imag_5, symbolLength,
          dfe_output_real_6, symbolLength, dfe_output_imag_6, symbolLength);

}
