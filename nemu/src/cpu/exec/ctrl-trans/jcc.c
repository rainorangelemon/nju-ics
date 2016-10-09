#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

make_helper_v(ja)
make_helper_v(jae)
make_helper_v(jb)
make_helper_v(jbe)
make_helper_v(jc)
make_helper_v(je)
make_helper_v(jg)
make_helper_v(jge)
make_helper_v(jl)
make_helper_v(jle)
make_helper_v(jna)
make_helper_v(jnae)
make_helper_v(jnb)
make_helper_v(jnbe)
make_helper_v(jnc)
make_helper_v(jne)
make_helper_v(jng)
make_helper_v(jnge)
make_helper_v(jnl)
make_helper_v(jnle)
make_helper_v(jnp)
make_helper_v(jns)
make_helper_v(jnz)
make_helper_v(jo)
make_helper_v(jp)
make_helper_v(jpe)
make_helper_v(jpo)
make_helper_v(js)
make_helper_v(jz)

