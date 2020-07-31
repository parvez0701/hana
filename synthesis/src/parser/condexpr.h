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

#ifndef _CONDEXPR_H_
#define _CONDEXPR_H_

#include "expr.h"
namespace Rtl
{
    class OMVisitorBase;
    class CondExpr : public Expr
    {
        friend class Elaborator;
        friend class SemanticChecker;
        friend Expr* ReplaceSubExpression(Expr*, const std::string&, const Expr*);
        public:
            CondExpr(const Expr* c, const Expr* te, const Expr* fe):
                Expr(), cond(const_cast<Expr*>(c)),
                trueExpr(const_cast<Expr*>(te)), 
                falseExpr(const_cast<Expr*>(fe)){
            }
            virtual ~CondExpr() {
            }

            const Expr* getCond() const{
                return cond;
            }

            const Expr* getTExpr() const{
                return trueExpr;
            }

            const Expr* getFExpr() const{
                return falseExpr;
            }
            virtual int evaluate() const {
                return 0;
            }
            virtual long len() const {
                return -1;
            }
            virtual void accept(OMVisitorBase*) const;
            virtual std::string toString() const;
            virtual CondExpr* copy() const;
            virtual bool isConstExpr() const {
                return cond->isConstExpr() && trueExpr->isConstExpr() &&
                    falseExpr->isConstExpr();
            }

            virtual const Expr* repExpr(const std::string&, const Expr*);
            virtual bool isSigned() const {
                return cond->isSigned() && trueExpr->isSigned() &&
                    falseExpr->isSigned();
            }        
        private:
            void setCond(const Expr* e){
                cond = const_cast<Expr*>(e);
            }
            void setTExpr(const Expr* e){
                trueExpr = const_cast<Expr*>(e);
            }
            void setFExpr(const Expr* e){
                falseExpr = const_cast<Expr*>(e);
            }
            Expr* cond;
            Expr* trueExpr;
            Expr* falseExpr;
    };
}
#endif
