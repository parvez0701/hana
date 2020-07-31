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

#ifndef _FORCE_H_
#define _FORCE_H_

#include "stmt.h"
#include "expr.h"
#include "omvisitor.h"

namespace Rtl
{
    class Force: public Stmt
    {
        public:
            Force(const Expr* l, const Expr* r): Stmt(), 
                lhs(const_cast<Expr*>(l)), rhs(const_cast<Expr*>(r)){
            }

            virtual ~Force(){
            }

            const Expr* getLhs() const{
                return lhs;
            }
            const Expr* getRhs() const{
                return rhs;
            }
            virtual void accept(OMVisitorBase* v) const {
                v->visit(this);
            }
            virtual Force* copy() const {
                return new Force(lhs->copy(), rhs->copy());
            }

            virtual void repExpr(    const std::string& str,
                                    const Expr *expr)
            {
                lhs = const_cast<Expr*>(lhs->repExpr(str, expr));
                rhs = const_cast<Expr*>(rhs->repExpr(str, expr));
                //return this;
            }
        private:
            Expr* lhs;
            Expr* rhs;
    };

    class Release: public Stmt
    {
        public:
            Release(const Expr* v): Stmt(), var(const_cast<Expr*>(v)){
            }

            virtual void accept(OMVisitor* v) const {
                v->visit(this);
            }
            const Expr* getVar() const{
                return var;
            }

            virtual Release* copy() const {
                return new Release(var->copy());
            }
            virtual void repExpr(const std::string& str,
                                     const Expr *expr)
            {
                var = const_cast<Expr*>(var->repExpr(str, expr));
                //return this;
            }    
                
        private:
            Expr* var;
    };
}

#endif
