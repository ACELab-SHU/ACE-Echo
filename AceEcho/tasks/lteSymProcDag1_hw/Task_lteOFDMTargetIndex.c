#include "riscv_printf.h"
#include "venus.h"


typedef short __v2048i16 __attribute__((ext_vector_type(2048)));
typedef char  __v4096i8 __attribute__((ext_vector_type(4096)));

typedef struct {
    short data;
  } __attribute__((aligned(64))) short_struct;

int Task_lteOFDMTargetIndex(__v2048i16 TargetIndex6, __v2048i16 TargetIndex15, __v2048i16 TargetIndex25,
                            __v2048i16 TargetIndex50, __v2048i16 TargetIndex75, __v2048i16 TargetIndex100,
                            short_struct nRB)
{
    __v2048i16 TargetIndex;
    vclaim(TargetIndex);
    __v2048i16 CopyIndex;
    vclaim(CopyIndex);
    vrange(CopyIndex, 2048);

    short nRB_data = nRB.data;
    short length = nRB_data * 12;
    if (nRB_data == 6)
    {
        vshuffle(TargetIndex, CopyIndex, TargetIndex6, SHUFFLE_GATHER, length);
    }
    else if(nRB_data == 15){
        vshuffle(TargetIndex, CopyIndex, TargetIndex15, SHUFFLE_GATHER, length);
    }
    else if (nRB_data == 25)
    {
        vshuffle(TargetIndex, CopyIndex, TargetIndex25, SHUFFLE_GATHER, length);
    }
    else if (nRB_data == 50)
    {
        vshuffle(TargetIndex, CopyIndex, TargetIndex50, SHUFFLE_GATHER, length);
    }
    else if (nRB_data == 75)
    {
        vshuffle(TargetIndex, CopyIndex, TargetIndex75, SHUFFLE_GATHER, length);
    }
    else if (nRB_data == 100)
    {
        vshuffle(TargetIndex, CopyIndex, TargetIndex100, SHUFFLE_GATHER, length);
    }

    vreturn(TargetIndex, sizeof(TargetIndex));
}
