#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/movzx.h"
#include "data-mov/movsx.h"
#include "data-mov/xchg.h"
#include "data-mov/push.h"
#include "data-mov/pop.h"

#include "jump/call.h"
#include "jump/call2.h"
#include "jump/je.h"
#include "jump/jne.h"
#include "jump/jbe.h"
#include "jump/ja.h"
#include "jump/jae.h"
#include "jump/jl.h"
#include "jump/jle.h"
#include "jump/jg.h"
#include "jump/jge.h"
#include "jump/jmp.h"
#include "jump/jmp2.h"
#include "jump/leave.h"
#include "jump/ret.h"


#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/sub.h"
#include "arith/test.h"
#include "arith/cmp.h"
#include "arith/add.h"
#include "arith/adc.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/shld.h"

#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"
