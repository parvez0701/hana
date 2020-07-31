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

#ifndef _PARTSEL_H_
#define _PARTSEL_H_
#include <stdlib.h>

#include "bitsel.h"
#include "range.h"
#include "concat.h"
#include "utils.h"

namespace Rtl
{
    class PartSel : public BitSel
    {
        public:
            PartSel(const Expr* e,const std::vector<Expr*>& d, 
                const Range* r, PolOp p):
                BitSel(e, d), range(const_cast<Range*>(r)), polop(p) {
            }

            virtual ~PartSel(){
            }
            const Range* getRange() const {
                return range;
            }
            PolOp getPolarityOperator() const {
                return polop;
            }    
            virtual std::string toString() const
            {
                std::string s = BitSel::toString();
                s = s + std::string("[") + range->getLExpr()->toString() +
                GetPolOpString(polop) + range->getRExpr()->
                toString() + string("]");
                return s;
            }
            virtual void accept(OMVisitorBase* v) const;

            bool isV2kStyle() const {
                return polop != POCOL;
            }

            virtual const Expr* repExpr(const std::string& str, const Expr* expr)
            {
                if(this->toString() == str) {
                    return expr;
                }
                range = range->repExpr(str, expr);
                return this;
            }    
            virtual bool isConstExpr() const {
                if(!BitSel::isConstExpr()){
                    return false;
                }
                return range->getLExpr()->isConstExpr() &&
                    range->getRExpr()->isConstExpr();
            }
            virtual const Concat* getActual() const;
            virtual long len() const 
            {
                //this functions is to be fixed for MD arrays
                if(!range->getLExpr()->isConstExpr() ||
                    !range->getRExpr()->isConstExpr()) {
                    return 0;
                }    
                const int msb = range->getLExpr()->evaluate();
                const int lsb = range->getRExpr()->evaluate();
                return abs(msb-lsb)+1;
            }

            virtual PartSel* copy() const
            {
                const std::vector<Expr*>& dim = this->getDim();
                std::vector<Expr*> newdim;
                for(int i = 0; i < dim.size(); ++i) {
                    newdim.push_back(dim[i]->copy());
                }    
                Expr *newvar = this->getVar() ? this->getVar()->copy() : 0;
                return new PartSel(newvar, newdim, range->copy(), polop);
            }

            virtual bool isSigned() const {
                return false;
            }    

        private:
            Range* range;
            PolOp polop;
    };

}
#endif
