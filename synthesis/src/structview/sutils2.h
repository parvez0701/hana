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

#include "sycntvs.h"

#include "sport.h"
#include "scell.h"

using std::string;
using std::list;

extern const char *DATAIN;
extern const char *DATAOUT;
extern const char *ENABLE;
extern const char *SELECT;
extern const char *CIN;
extern const char *COUT;
extern const char *CLOCK;
extern const char *RESET;
extern const char *SET;
extern const char *SHIFT;
extern const char *SHIFTVAL;
extern const char *EQ;
extern const char *UEQ;
extern const char *GT;
extern const char *LT;

namespace stview
{
    class SInst;
    class SBlock;
    class SNet;
    SPort::Dir    GetDir(Synth::SyScPort::SyDirType dir);

    //Used only for simple combinational gates instance
    SInst*        CreateInstance(SBlock*, SCell::CellType, const std::string&, int=2, bool=true); 
    SInst*        CreateInstance(SBlock*, const SCell*, const std::string&); 
    SNet*        CreateNet(SBlock*, const std::string&);

    std::string CellType2String(SCell::CellType);

}
