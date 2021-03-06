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

#ifndef _SOPNORTRANS_H_
#define _SOPNORTRANS_H_

#include "soptransbase.h"

namespace SOP
{
    class SopNorTrans : public SopTransBase
    {
        public:
            virtual ~SopNorTrans() { }
            virtual SopNorTrans* copy() const { return new SopNorTrans(); }
            virtual void transform(const SInst *inst)
            {
                const std::list<SPin*>& inpins = inst->getInPins();
                std::list<SPin*>::const_iterator piter = inpins.begin();
                
                SExpr *expr = this->getExpr(*piter);
                for(++piter; piter != inpins.end(); ++piter)
                {
                    SExpr *other = this->getExpr(*piter);
                    expr = SExprTree::createNode(
                        SExpr::OR, 0, expr, other);
                }

                expr = SExprTree::createNode(SExpr::NOT, 0, expr);
                
                const SPin *pin = inst->getPin("out");
                this->addExpr(pin, expr);
            }    
    };
}
#endif
