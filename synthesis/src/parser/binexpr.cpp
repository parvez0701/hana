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

#include "binexpr.h"
#include "omvisitor.h"
#include "utils.h"
using std::string;
namespace Rtl
{
    void
    BinExpr::accept(OMVisitorBase* v) const{
        v->visit(this);
    }

    string
    BinExpr::toString() const {
        return string ("(") + 
            lExpr->toString() + 
            GetOpString(op) + 
            rExpr->toString() +
            string(")");
    }

    int
    BinExpr::evaluate() const 
    {
        if(!lExpr->isConstExpr() || !rExpr->isConstExpr()) {
            return INT_MIN;
        }    
        switch (op)
        {
            case LOGAND:
            {
                if(lExpr->evaluate() && rExpr->evaluate()){
                    return 1;
                }
                else{
                    return 0;
                }
            }
            case LOGOR:
            {
                if(lExpr->evaluate() || rExpr->evaluate()){
                    return 1;
                }
                else {
                    return 0;
                }
            }
            case GTHAN:
            {
                if(lExpr->evaluate() > rExpr->evaluate()){
                    return 1;
                }
                else {
                    return 0;
                }
            }
            case GTHANEQ:
            {
                if(lExpr->evaluate() >= rExpr->evaluate()){
                    return 1;
                }
                else {
                    return 0;
                }
            }
            
            case LTHAN:
            {
                if(lExpr->evaluate() < rExpr->evaluate()){
                    return 1;
                }
                else {
                    return 0;
                }
            }
            case LTHANEQ:
            {
                if(lExpr->evaluate() <= rExpr->evaluate()){
                    return 1;
                }
                else {
                    return 0;
                }
            }

            case LOGEQ:
            {
                if(lExpr->evaluate() == rExpr->evaluate()){
                    return 1;
                }
                else {
                    return 0;
                }
            }
                    
            case LOGENEQ:
            {
                if(lExpr->evaluate() != rExpr->evaluate()){
                    return 1;
                }
                else {
                    return 0;
                }
            }

            case LSHIFT: return lExpr->evaluate() << rExpr->evaluate();
            case RSHIFT:
            case RRSHIFT: return lExpr->evaluate() >> rExpr->evaluate();
            case PLUS: return lExpr->evaluate() + rExpr->evaluate();
            case MINUS: return lExpr->evaluate() - rExpr->evaluate();
            case MULT: return lExpr->evaluate() * rExpr->evaluate();
            case DIV: return lExpr->evaluate()/rExpr->evaluate();
            case MOD: return lExpr->evaluate()%rExpr->evaluate();
            case BITXOR: return lExpr->evaluate() ^ rExpr->evaluate();
            case BITNXOR: return ~(lExpr->evaluate() ^ rExpr->evaluate());
            case BITAND: return ~(lExpr->evaluate() & rExpr->evaluate());
            case BITOR: return ~(lExpr->evaluate() & rExpr->evaluate());
            default: return INT_MIN;
        }
        return INT_MIN;
    }

    long
    BinExpr::len() const
    {
        return -1;
/*
        const long siz1 = lExpr->size();
        const long siz2 = rExpr->size();
        if(size1 == -1 || size2 == -1){
            return -1;
        }
        return size1 > size2 ? size1 : size2;
*/
    }

    const Expr*
    BinExpr::repExpr(const string& str, const Expr *expr)
    {
        if(this->toString() == str) {
            return expr;
        }

        lExpr = const_cast<Expr*>(lExpr->repExpr(str, expr));
        rExpr = const_cast<Expr*>(rExpr->repExpr(str, expr));
        return this;
    }
}
