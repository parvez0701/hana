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

#ifndef _SOPANDTRANS_H_
#define _SOPANDTRANS_H_

#include "soptransbase.h"
//#include "soputils.h"
//#include "sopsop.h"

namespace SOP
{
    class SopAndTrans : public SopTransBase
    {
        public:
            virtual ~SopAndTrans() { }
            virtual SopAndTrans* copy() const { return new SopAndTrans(); }
            virtual void transform(const SInst *inst)
            {
                const std::list<SPin*>& inpins = inst->getInPins();
                std::list<SPin*>::const_iterator piter = inpins.begin();
                
                SExpr *expr = this->getExpr(*piter);
                for(++piter; piter != inpins.end(); ++piter)
                {
                    SExpr *other = this->getExpr(*piter);
                    expr = SExprTree::createNode(SExpr::AND, 0, expr, other);
                }
                
                const SPin *pin = inst->getPin("out");
                this->addExpr(pin, expr);
            }    
    };
}
#endif
