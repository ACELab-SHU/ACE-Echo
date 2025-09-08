/**
 * ****************************************
 * @file        Task_lteDmrsIndices.c
 * @brief       LTE Dmrs Generation
 * @author      chenxiaoxiao
 * @date        2025.3.20
 * @copyright   ACE-Lab(Shanghai University)
 * ****************************************
 */

 #include "data_type.h"
 #include "riscv_printf.h"
 #include "venus.h"
 
 typedef short __v2048i16 __attribute__((ext_vector_type(2048)));
 typedef char __v4096i8 __attribute__((ext_vector_type(4096)));
 
 
 int Task_lteDmrsIndices(
  short_struct nCellID, short_struct nDLRB, short_struct CellRefPort, short_struct NslotNum, __v2048i16 dmrsindex_6, __v2048i16 dmrsindex_15, __v2048i16 dmrsindex_25, __v2048i16 dmrsindex_50, __v2048i16 dmrsindex_75, __v2048i16 dmrsindex_100)
 {

  int NID = nCellID.data;
  int RB = nDLRB.data;
  int port = CellRefPort.data;
  int Ns = NslotNum.data;
  int vshift = NID % 6;
  int subshift = RB * 12 * 4;
  int symshift = RB * 12;
  short_struct dmrs_symbol_length;

  __v2048i16 DmrsIndex_0;
  __v2048i16 DmrsIndex_4;
  __v2048i16 DmrsIndex;
  vclaim(DmrsIndex_0);
  vclaim(DmrsIndex_4);
  vclaim(DmrsIndex);

  __v2048i16 index12;
  __v2048i16 index30; 
  __v2048i16 index50;
  __v2048i16 index100;
  __v2048i16 index150;
  __v2048i16 index200;
  vclaim(index12);
  vclaim(index30);
  vclaim(index50);
  vclaim(index100);
  vclaim(index150);
  vclaim(index200);

  vrange(index12, 12);
  vrange(index30, 30);
  vrange(index50, 50);
  vrange(index100, 100);
  vrange(index150, 150);
  vrange(index200, 200);

  if(port == 0 && RB == 6){
    //v = 0;
    dmrs_symbol_length.data = 2;
    DmrsIndex_0 = vsadd(dmrsindex_6, vshift, MASKREAD_OFF, 12);
    vshift = (vshift + 3) % 6;
    DmrsIndex_4 = vsadd(dmrsindex_6, vshift + subshift, MASKREAD_OFF, 12);
    vshuffle(DmrsIndex, index12, DmrsIndex_0, SHUFFLE_SCATTER, 12);
    index12 = vsadd(index12, 12, MASKREAD_OFF, 12);
    vshuffle(DmrsIndex, index12, DmrsIndex_4, SHUFFLE_SCATTER, 12);
  }
  else if(port == 0 && RB == 15){
    //v = 0;
    dmrs_symbol_length.data = 2;
    DmrsIndex_0 = vsadd(dmrsindex_15, vshift, MASKREAD_OFF, 30);
    vshift = (vshift + 3) % 6;
    DmrsIndex_4 = vsadd(dmrsindex_15, vshift + subshift, MASKREAD_OFF, 30);
    vshuffle(DmrsIndex, index30, DmrsIndex_0, SHUFFLE_SCATTER, 30);
    index30 = vsadd(index30, 30, MASKREAD_OFF, 30);
    vshuffle(DmrsIndex, index30, DmrsIndex_4, SHUFFLE_SCATTER, 30);
  }
  else if(port == 0 && RB == 25){
    //v = 0;
    dmrs_symbol_length.data = 2;
    DmrsIndex_0 = vsadd(dmrsindex_25, vshift, MASKREAD_OFF, 50);
    vshift = (vshift + 3) % 6;
    DmrsIndex_4 = vsadd(dmrsindex_25, vshift + subshift, MASKREAD_OFF, 50);
    vshuffle(DmrsIndex, index50, DmrsIndex_0, SHUFFLE_SCATTER, 50);
    index50 = vsadd(index50, 50, MASKREAD_OFF, 50);
    vshuffle(DmrsIndex, index50, DmrsIndex_4, SHUFFLE_SCATTER, 50);
  }
  else if(port == 0 && RB == 50){
    //v = 0;
    dmrs_symbol_length.data = 2;
    DmrsIndex_0 = vsadd(dmrsindex_50, vshift, MASKREAD_OFF, 100);
    vshift = (vshift + 3) % 6;
    DmrsIndex_4 = vsadd(dmrsindex_50, vshift + subshift, MASKREAD_OFF, 100);
    vshuffle(DmrsIndex, index100, DmrsIndex_0, SHUFFLE_SCATTER, 100);
    index100 = vsadd(index100, 100, MASKREAD_OFF, 100);
    vshuffle(DmrsIndex, index100, DmrsIndex_4, SHUFFLE_SCATTER, 100);
  }
  else if(port == 0 && RB == 75){
    //v = 0;
    dmrs_symbol_length.data = 2;
    DmrsIndex_0 = vsadd(dmrsindex_75, vshift, MASKREAD_OFF, 150);
    vshift = (vshift + 3) % 6;
    DmrsIndex_4 = vsadd(dmrsindex_75, vshift + subshift, MASKREAD_OFF, 150);
    vshuffle(DmrsIndex, index150, DmrsIndex_0, SHUFFLE_SCATTER, 150);
    index150 = vsadd(index150, 150, MASKREAD_OFF, 150);
    vshuffle(DmrsIndex, index150, DmrsIndex_4, SHUFFLE_SCATTER, 150);
  }
  else if(port == 0 && RB == 100){
    //v = 0;
    dmrs_symbol_length.data = 2;
    DmrsIndex_0 = vsadd(dmrsindex_100, vshift, MASKREAD_OFF, 200);
    vshift = (vshift + 3) % 6;
    DmrsIndex_4 = vsadd(dmrsindex_100, vshift + subshift, MASKREAD_OFF, 200);
    vshuffle(DmrsIndex, index200, DmrsIndex_0, SHUFFLE_SCATTER, 200);
    index200 = vsadd(index200, 200, MASKREAD_OFF, 200);
    vshuffle(DmrsIndex, index200, DmrsIndex_4, SHUFFLE_SCATTER, 200);
  }
  else if(port == 1 && RB == 6){
    dmrs_symbol_length.data = 2;
    DmrsIndex_4 = vsadd(dmrsindex_6, vshift + subshift, MASKREAD_OFF, 12);
    vshift = (vshift + 3) % 6;
    DmrsIndex_0 = vsadd(dmrsindex_6, vshift, MASKREAD_OFF, 12);
    vshuffle(DmrsIndex, index12, DmrsIndex_0, SHUFFLE_SCATTER, 12);
    index12 = vsadd(index12, 12, MASKREAD_OFF, 12);
    vshuffle(DmrsIndex, index12, DmrsIndex_4, SHUFFLE_SCATTER, 12);
  }
  else if(port == 1 && RB == 15){
    dmrs_symbol_length.data = 2;
    DmrsIndex_4 = vsadd(dmrsindex_15, vshift + subshift, MASKREAD_OFF, 30);
    vshift = (vshift + 3) % 6;
    DmrsIndex_0 = vsadd(dmrsindex_15, vshift, MASKREAD_OFF, 30);
    vshuffle(DmrsIndex, index30, DmrsIndex_0, SHUFFLE_SCATTER, 30);
    index30 = vsadd(index30, 30, MASKREAD_OFF, 30);
    vshuffle(DmrsIndex, index30, DmrsIndex_4, SHUFFLE_SCATTER, 30);
  }
  else if(port == 1 && RB == 25){
    dmrs_symbol_length.data = 2;
    DmrsIndex_4 = vsadd(dmrsindex_25, vshift + subshift, MASKREAD_OFF, 50);
    vshift = (vshift + 3) % 6;
    DmrsIndex_0 = vsadd(dmrsindex_25, vshift, MASKREAD_OFF, 50);
    vshuffle(DmrsIndex, index50, DmrsIndex_0, SHUFFLE_SCATTER, 50);
    index50 = vsadd(index50, 50, MASKREAD_OFF, 50);
    vshuffle(DmrsIndex, index50, DmrsIndex_4, SHUFFLE_SCATTER, 50);
  }
  else if(port == 1 && RB == 50){
    dmrs_symbol_length.data = 2;
    DmrsIndex_4 = vsadd(dmrsindex_50, vshift + subshift, MASKREAD_OFF, 100);
    vshift = (vshift + 3) % 6;
    DmrsIndex_0 = vsadd(dmrsindex_50, vshift, MASKREAD_OFF, 100);
    vshuffle(DmrsIndex, index100, DmrsIndex_0, SHUFFLE_SCATTER, 100);
    index100 = vsadd(index100, 100, MASKREAD_OFF, 100);
    vshuffle(DmrsIndex, index100, DmrsIndex_4, SHUFFLE_SCATTER, 100);
  }
  else if(port == 1 && RB == 75){
    dmrs_symbol_length.data = 2;
    DmrsIndex_4 = vsadd(dmrsindex_75, vshift + subshift, MASKREAD_OFF, 150);
    vshift = (vshift + 3) % 6;
    DmrsIndex_0 = vsadd(dmrsindex_75, vshift, MASKREAD_OFF, 150);
    vshuffle(DmrsIndex, index150, DmrsIndex_0, SHUFFLE_SCATTER, 150);
    index150 = vsadd(index150, 150, MASKREAD_OFF, 150);
    vshuffle(DmrsIndex, index150, DmrsIndex_4, SHUFFLE_SCATTER, 150);
  }
  else if(port == 1 && RB == 100){
    dmrs_symbol_length.data = 2;
    DmrsIndex_4 = vsadd(dmrsindex_100, vshift + subshift, MASKREAD_OFF, 200);
    vshift = (vshift + 3) % 6;
    DmrsIndex_0 = vsadd(dmrsindex_100, vshift, MASKREAD_OFF, 200);
    vshuffle(DmrsIndex, index200, DmrsIndex_0, SHUFFLE_SCATTER, 200);
    index200 = vsadd(index200, 200, MASKREAD_OFF, 200);
    vshuffle(DmrsIndex, index200, DmrsIndex_4, SHUFFLE_SCATTER, 200);
  }
  else if(port == 2 && RB == 6 && Ns == 0){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_6, vshift + symshift, MASKREAD_OFF, 12);
  }
  else if(port == 2 && RB == 15 && Ns == 0){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_15, vshift + symshift, MASKREAD_OFF, 30);
  }
  else if(port == 2 && RB == 25 && Ns == 0){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_25, vshift + symshift, MASKREAD_OFF, 50);
  }
  else if(port == 2 && RB == 50 && Ns == 0){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_50, vshift + symshift, MASKREAD_OFF, 100);
  }
  else if(port == 2 && RB == 75 && Ns == 0){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_75, vshift + symshift, MASKREAD_OFF, 150);
  }
  else if(port == 2 && RB == 100 && Ns == 0){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_100, vshift + symshift, MASKREAD_OFF, 200);
  }
  else if(port == 2 && RB == 6 && Ns == 1){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_6, vshift + symshift, MASKREAD_OFF, 12);
  }
  else if(port == 2 && RB == 15 && Ns == 1){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_15, vshift + symshift, MASKREAD_OFF, 30);
  }
  else if(port == 2 && RB == 25 && Ns == 1){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_25, vshift + symshift, MASKREAD_OFF, 50);
  }
  else if(port == 2 && RB == 50 && Ns == 1){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_50, vshift + symshift, MASKREAD_OFF, 100);
  }
  else if(port == 2 && RB == 75 && Ns == 1){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_75, vshift + symshift, MASKREAD_OFF, 150);
  }
  else if(port == 2 && RB == 100 && Ns == 1){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_100, vshift + symshift, MASKREAD_OFF, 200);
  }
  else if(port == 3 && RB == 6 && Ns == 0){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_6, vshift + symshift, MASKREAD_OFF, 12);
  }
  else if(port == 3 && RB == 15 && Ns == 0){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_15, vshift + symshift, MASKREAD_OFF, 30);
  }
  else if(port == 3 && RB == 25 && Ns == 0){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_25, vshift + symshift, MASKREAD_OFF, 50);
  }
  else if(port == 3 && RB == 50 && Ns == 0){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_50, vshift + symshift, MASKREAD_OFF, 100);
  }
  else if(port == 3 && RB == 75 && Ns == 0){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_75, vshift + symshift, MASKREAD_OFF, 150);
  }
  else if(port == 3 && RB == 100 && Ns == 0){
    dmrs_symbol_length.data = 1;
    vshift = (vshift + 3) % 6;
    DmrsIndex = vsadd(dmrsindex_100, vshift + symshift, MASKREAD_OFF, 200);
  }
  else if(port == 3 && RB == 6 && Ns == 1){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_6, vshift + symshift, MASKREAD_OFF, 12);
  }
  else if(port == 3 && RB == 15 && Ns == 1){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_15, vshift + symshift, MASKREAD_OFF, 30);
  }
  else if(port == 3 && RB == 25 && Ns == 1){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_25, vshift + symshift, MASKREAD_OFF, 50);
  }
  else if(port == 3 && RB == 50 && Ns == 1){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_50, vshift + symshift, MASKREAD_OFF, 100);
  }
  else if(port == 3 && RB == 75 && Ns == 1){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_75, vshift + symshift, MASKREAD_OFF, 150);
  }
  else if(port == 3 && RB == 100 && Ns == 1){
    dmrs_symbol_length.data = 1;
    DmrsIndex = vsadd(dmrsindex_100, vshift + symshift, MASKREAD_OFF, 200);
  }  
  else{
    printf("input error",NULL);
  }

  DmrsIndex = vsadd(DmrsIndex, 0, MASKREAD_OFF);
  vreturn(DmrsIndex, sizeof(DmrsIndex), &dmrs_symbol_length, sizeof(dmrs_symbol_length));
}
 