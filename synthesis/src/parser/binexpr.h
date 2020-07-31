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

#ifndef _BINEXPR_H_
#define _BINEXPR_H_

#include "defines.h"
#include "expr.h"
namespace Rtl
{
    class OMVisitor;
    class BinExpr: public Expr
    {
        friend class Elaborator;
        friend class SemanticChecker;
        friend Expr* ReplaceSubExpression(Expr*, const std::string&, const Expr*);
        public:
            BinExpr(const Expr* le, const Expr* re, OpType o): Expr(),
                lExpr(const_cast<Expr*>(le)),
                rExpr(const_cast<Expr*>(re)),
                op(o){
            }
            virtual ~BinExpr(){
            }

            OpType getOp() const{
                return op;
            }
            const Expr* getLExpr() const {
                return lExpr;
            }
            const Expr* getRExpr() const {
                return rExpr;
            }
            virtual std::string toString() const;
            virtual void accept(OMVisitorBase*) const;
            virtual int evaluate() const;
            virtual long len() const;
            virtual BinExpr* copy() const {
                return new BinExpr(lExpr->copy(), rExpr->copy(), op);
            }
            virtual bool isConstExpr() const {
                return lExpr->isConstExpr() && rExpr->isConstExpr();
            }

            virtual const Expr *repExpr(const std::string&, const Expr*);
            virtual bool isSigned() const {
                return lExpr->isSigned() && rExpr->isSigned();
            }

        private:
            void setLExpr(const Expr* e){
                lExpr = const_cast<Expr*>(e);
            }
            void setRExpr(const Expr* e){
                rExpr = const_cast<Expr*>(e);
            }
            Expr* lExpr;
            Expr* rExpr;
            OpType op;
    };
            
}
#endif
