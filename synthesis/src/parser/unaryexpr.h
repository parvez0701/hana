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

#ifndef _UNARYEXPR_H_
#define _UNARYEXPR_H_
#include "defines.h"
#include "expr.h"
namespace Rtl
{
    class OMVisitorBase;
    class UnaryExpr: public Expr
    {
        friend class Elaborator;
        friend class SemanticChecker;
        friend Expr* ReplaceSubExpression(Expr*, const std::string&, const Expr*);
        public:
            UnaryExpr(const Expr* e, OpType o): Expr(), 
            expr(const_cast<Expr*>(e)), op(o) {
            }
            virtual ~UnaryExpr(){
            }
            const Expr* getExpr() const{
                return expr;
            }
            OpType getOp() const {
                return op;
            }
            virtual std::string toString() const;
            virtual void accept(OMVisitorBase*) const;
            virtual int evaluate() const;
            virtual long len() const {
                return -1;
            }
            virtual bool isConstExpr() const {
                return expr->isConstExpr();
            }

            virtual const Expr* repExpr(const std::string&, const Expr*);

            virtual UnaryExpr* copy() const {
                return new UnaryExpr(expr->copy(), op);
            }    

            virtual bool isSigned() const {
                return expr->isSigned();
            }    
        private:
            void setExpr(const Expr* e){
                expr = const_cast<Expr*>(e);
            }

            int evalRedXor() const;
            int evalRedXnor() const;
            Expr* expr;
            OpType op;
    };
}
#endif
