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

#ifndef _RANGE_H_
#define _RANGE_H_
#include "base.h"

namespace Rtl
{
    class Expr;
    class Range: public Base
    {
        friend class Elaborator;
        friend Expr* ReplaceSubExpression(Expr* orig, const std::string&, const Expr *subExpr);
        public:
            Range(): Base(){
            }
            
            Range(const Expr* lexpr, const Expr* rexpr):
                Base() , lrExpr(std::make_pair(const_cast<Expr*>(lexpr),
                const_cast<Expr*>(rexpr))){
            }

            virtual ~Range(){
            }

            const Expr* getLExpr()const {
                return lrExpr.first;
            }

            const Expr* getRExpr() const{
                return lrExpr.second;
            }
/*
            void setLExpr(const Expr *expr) {
                lrExpr.first = const_cast<Expr*>(expr);
            }    

            void setRExpr(const Expr *expr) {
                lrExpr.second = const_cast<Expr*>(expr);
            }    
*/            
            virtual Range* copy() const;
            Range *repExpr(const std::string& str, const Expr* expr);
            
        private:
            std::pair<Expr*, Expr*> lrExpr;
    };
}
#endif
