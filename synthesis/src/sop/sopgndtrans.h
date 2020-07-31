/* 
Copyright (C) 2009-2011 Parvez Ahmad
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

#ifndef _SOPGNDTRANS_H_
#define _SOPGNDTRANS_H_

#include "soptransbase.h"

namespace SOP
{
    class SopGndTrans : public SopTransBase
    {
        public:
            virtual ~SopGndTrans() { }
            virtual SopGndTrans* copy() const { return new SopGndTrans(); }
            virtual void transform(const SInst *inst)
            {
                const SPin *spin = inst->getPin("out");
                this->addExpr(spin, SExprTree::createNode(SExpr::ZERO));
            }
    };
}
#endif
