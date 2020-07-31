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

#ifndef _SOPINVTRANS_H_
#define _SOPINVTRANS_H_

#include "soptransbase.h"
//#include "sopsop.h"

namespace SOP
{
    class SopInvTrans : public SopTransBase
    {
        public:
            virtual ~SopInvTrans() { }
            virtual SopInvTrans* copy() const { return new SopInvTrans(); }
            virtual void transform(const SInst *inst)
            {
                const SPin *spin = inst->getPin("in");
                SExpr *expr = this->getExpr(spin);
                this->addExpr(inst->getPin("out"), 
                    SExprTree::createNode(SExpr::NOT, 0, expr));
            }
    };
}
#endif
