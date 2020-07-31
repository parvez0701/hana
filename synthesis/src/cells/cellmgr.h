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

#ifndef _CELLMGR_H_
#define _CELLMGR_H_

#include <map>
#include <string>
#include <typeinfo>
#include "syutils.h"

namespace Synth
{
    class SynthCELL;
    class CellMgr
    {
        public:
            typedef enum {UNSET, GND, VCC, BUF, NOT, AND, NAND, OR, NOR, XOR,
                XNOR, MUX, FF, FFR, LD, LDR, ADD, SUB, MUL, DIV, CMP, LSH,
                RSH, ENC, DEC, INC } CellType;
            static CellMgr* instance();
            virtual ~CellMgr();

            const SynthCELL* create(
                    CellType,
                    const std::vector<int>&);

        private:
            CellMgr();
            CellMgr(const CellMgr&);
            CellMgr& operator=(const CellMgr&);

            static CellMgr *cellmgr;
            std::map<std::string, const SynthCELL*, cmp_str> existCells;
            std::map<CellType, const SynthCELL*> synthCells;
    };
}

#endif
