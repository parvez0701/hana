/* 
Copyright (C) 2009-2010 Parvez Ahmad
Written by Parvez Ahmad <parvez_ahmad@yahoo.co.uk>.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef _PARSEHELP_H_
#define _PARSEHELP_H_
#include <vector>
#include <string>
#include <algorithm>
#include "omcreator.h"
#include "module.h"
#include "param.h"
#include "localparam.h"
#include "port.h"
//#include "range.h"
#include "parseinfo.h"
#include "basednum.h"
#include "num.h"
#include "hierid.h"
#include "vstring.h"
#include "bitsel.h"
#include "partsel.h"
#include "concat.h"
#include "multiconcat.h"
#include "binexpr.h"
#include "unaryexpr.h"
#include "condexpr.h"
#include "defines.h"
#include "modinst.h"
#include "delay.h"
#include "semcheck.h"
#include "file.h"
#include "contassign.h"
#include "proccontassign.h"
#include "dassign.h"
#include "namedblock.h"
#include "block.h"
#include "force.h"
#include "fork.h"
#include "disable.h"
#include "trigger.h"
#include "loopstmts.h"
#include "case.h"
#include "if.h"
#include "event.h"
#include "procassign.h"
#include "compassign.h"
#include "taskfunc.h"
#include "defparam.h"
#include "specify.h"
#include "udp.h"
#include "generate.h"
#include "realconst.h"
#include "implicitnet.h"

using namespace Rtl;
bool skipMod();
void skipUpto(const std::vector<std::string>&);
void handleGenIf();
int processIfdef(int);
void processUndef();
bool isSystemTimingCheckCommand(const char*);
void setNumString();
#endif
