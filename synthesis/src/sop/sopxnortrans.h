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

#ifndef _SOPXNORTRANS_H_
#define _SOPXNORTRANS_H_

#include <cassert>

#include "soptransbase.h"
//#include "sopsop.h"

namespace SOP
{
    class SopXnorTrans : public SopTransBase
    {
        public:
            virtual ~SopXnorTrans() { }
            virtual SopXnorTrans* copy() const { return new SopXnorTrans(); }
            virtual void transform(const SInst *inst)
            {
                const std::list<SPin*>& inpins = inst->getInPins();

                assert(inpins.size() == 2);
                std::list<SPin*>::const_iterator piter = inpins.begin();
                
                SExpr *expr1 = this->getExpr(*piter++);
                SExpr *expr2 = this->getExpr(*piter);

                SExpr *expr = SExprTree::createNode(
                    SExpr::NOT, 0, SExprTree::createNode(
                        SExpr::OR, 0, SExprTree::createNode(
                            SExpr::AND, 0, 
                                SExprTree::subTreeCopy(expr1), 
                                   SExprTree::createNode(SExpr::NOT, 0, 
                                       SExprTree::subTreeCopy(expr2))), 
                    SExprTree::createNode(SExpr::AND, 0, 
                        SExprTree::createNode(SExpr::NOT, 0, expr1), expr2)));
                
                const SPin *pin = inst->getPin("out");
                this->addExpr(pin, expr);
            }    
    };
}
#endif
