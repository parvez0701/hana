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

#ifndef _DEFPARAM_H_
#define _DEFPARAM_H_

#include "assign.h"
namespace Rtl
{
    class Defparam: public Assign
    {
        friend class OMCreator;
        public:
            Defparam(const Expr* le, const Expr* re):
                Assign(le, re), range(0){
            }
        
            virtual ~Defparam(){
            }

            const Range* getRange() const{
                return range;
            }

            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }

            void setRange(const Range* r){
                range = const_cast<Range*>(r);
            }
            virtual Defparam* copy() const {
                std::pair<Expr*, Expr*> ep = this->getExprCopy();
                Defparam* dp = new Defparam(ep.first, ep.second);
                dp->setRange(range->copy());
                return dp;
            }

            virtual void repExpr(    const std::string& str,
                                    const Expr *expr)
            {
                Assign::repExpr(str, expr);
                range = range->repExpr(str, expr);
                //return this;
            }
        private:
            Range* range;
    };
}
#endif
