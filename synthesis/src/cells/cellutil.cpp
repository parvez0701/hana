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

#include "cellutil.h"

namespace Synth
{
    std::string CellType2String(CellType t)
    {
        switch (t)
        {
            case GND    : return "GND";
            case VCC    : return "VCC"; 
            case BUF    : return "BUF";
            case NOT    : return "NOT"; 
            case AND    : return "AND"; 
            case NAND    : return "NAND"; 
            case OR        : return "OR";
            case NOR    : return "NOR";
            case XOR    : return "XOR";
            case XNOR    : return "XNOR";
            case MUX    : return "MUX";
            case FF        : return "FF";
            case FFR    : return "FFR";
            case FFS    : return "FFS";
            case FFRS    : return "FFRS";
            case FFSR    : return "FFSR";
            case LD        : return "LD";
            case LDR    : return "LDR";
            case ADD    : return "ADD";
            case SUB    : return "SUB";
            case MUL    : return "MUL";
            case DIV    : return "DIV";
            case CMP    : return "CMP";
            case SHIFTER: return "SHIFTER";
            case ENC    : return "ENC";
            case DEC    : return "DEC";
            case INC    : return "INC";
            case TRIBUF    : return "TRIBUF";
            default        : return "";
        }    
    }    

}
