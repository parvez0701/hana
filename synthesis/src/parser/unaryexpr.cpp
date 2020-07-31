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

#include <cmath>
#include "unaryexpr.h"
#include "omvisitor.h"
#include "utils.h"

using std::string;
namespace Rtl
{
    void
    UnaryExpr::accept(OMVisitorBase* v) const {
        v->visit(this);
    }

    string
    UnaryExpr::toString() const {
        return string(" (") + 
            GetOpString(op) + 
            expr->toString() +
            string(")");
    }

    int
    UnaryExpr::evaluate() const
    {
        if(!expr->isConstExpr()) {
            return INT_MIN;
        }    
        switch (op)
        {
            case UNPLUS: return expr->evaluate();
            case UNMINUS: return -1*expr->evaluate();
            case BITNEG: return expr->evaluate() ^ (int)pow(2, 32);
            case LOGNEG: return !expr->evaluate();
            case REDXOR: return evalRedXor();
            case REDXNOR: return evalRedXnor();
            default: return INT_MIN;
        }
    }

    const Expr*
    UnaryExpr::repExpr(const string& str, const Expr *_expr) 
    {
        if(this->toString() == str) {
            return _expr;
        }
        expr = const_cast<Expr*>(expr->repExpr(str, _expr));
        return this;
    }

    int
    UnaryExpr::evalRedXor() const
    {
        int val = expr->evaluate();
        int ret = val << 1;
        ret ^= val;
        val = val << 1;
        int count = 0;
        do
        {
            ret ^= val;
            val = val << 1;
            ++count;
        }while (count < 30);
        return ret >> 31;
    }
    
    int
    UnaryExpr::evalRedXnor() const
    {
        int val = expr->evaluate();
        int ret = val << 1;
        ret = ~(ret ^ val);
        val = val << 1;
        int count = 0;
        do
        {
            ret = ~(ret ^ val);
            val = val << 1;
            ++count;
        }while (count < 30);
        return ret >> 31;
    }

        
}
