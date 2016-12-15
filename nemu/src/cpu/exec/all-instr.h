#include "prefix/prefix.h"

#include "ctrl-trans/call.h"
#include "ctrl-trans/jcc.h"
#include "ctrl-trans/ret.h"
#include "ctrl-trans/leave.h"
#include "ctrl-trans/jmp.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/movsx.h"
#include "data-mov/movzx.h"
#include "data-mov/cwtl_cltd.h"
#include "data-mov/pusha.h"
#include "data-mov/popa.h"

#include "data-trans/push.h"
#include "data-trans/pop.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/cmp.h"
#include "arith/add.h"
#include "arith/adc.h"
#include "arith/sub.h"
#include "arith/sbb.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/shld.h"
#include "logic/test.h"
#include "logic/setcc.h"
#include "logic/cld.h"
#include "logic/std.h"

#include "string/rep.h"
#include "string/movs.h"
#include "string/stos.h"
#include "string/lods.h"
#include "string/scas.h"

#include "misc/misc.h"

#include "special/special.h"

#include "system/lgdt.h"
#include "system/lidt.h"
#include "system/int.h"
#include "system/iret.h"
#include "system/hlt.h"

#include "io/in.h"
#include "io/out.h"
