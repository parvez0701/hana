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

#ifndef _ASSIGN_H_
#define _ASSIGN_H_

#include "stmt.h"
#include "expr.h"
namespace Rtl
{
    class Assign: public Stmt
    {
        friend class SemanticChecker;
        friend class GenUnroll;
        public:
            Assign(const Expr* l, const Expr* r): Stmt(),
            lexpr(const_cast<Expr*>(l)), rexpr(const_cast<Expr*>(r)){
            }
            virtual ~Assign() {
            }
            const Expr* getLhs() const {
                return lexpr;
            }

            const Expr* getRhs() const {
                return rexpr;
            }
            virtual Assign* copy() const = 0;//{
            //    return new Assign(lexpr->copy(), rexpr->copy());
            //}

            virtual void repExpr(const std::string& str,
                                    const Expr *expr)
            {
                lexpr = const_cast<Expr*>(lexpr->repExpr(str, expr));
                rexpr = const_cast<Expr*>(rexpr->repExpr(str, expr));
                //return this;
            }
        protected:
            std::pair<Expr*, Expr*> getExprCopy() const {
                return std::make_pair(lexpr->copy(), rexpr->copy());
            }
            void setLhs(const Expr *l) {
                lexpr = const_cast<Expr*>(l);
            }
            void setRhs(const Expr *r) {
                rexpr = const_cast<Expr*>(r);
            }    
        private:
            Expr* lexpr;
            Expr* rexpr;
    };

}
#endif
