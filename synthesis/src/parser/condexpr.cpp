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

#include "condexpr.h"
#include "omvisitor.h"
using std::string;
namespace Rtl
{
    void
    CondExpr::accept(OMVisitorBase* v) const {
        v->visit(this);
    }

    string
    CondExpr::toString() const {
        return string(" (") + cond->toString() + string (")") +
            string(" ? ") + 
            string(" (") + trueExpr->toString() + string(")") +
            string(" : ") + 
            string(" (") + falseExpr->toString() + string(")");
    }

    CondExpr*
    CondExpr::copy() const
    {
        Expr* ccond = cond->copy();
        Expr* ctrueExpr = trueExpr->copy();
        Expr* cfalseExpr = falseExpr->copy();
        return new CondExpr(ccond, ctrueExpr, cfalseExpr);
    }

    const Expr*
    CondExpr::repExpr(const string& str, const Expr *expr)
    {
        if(this->toString() == str) {
            return expr;
        }
        cond = const_cast<Expr*>(cond->repExpr(str, expr));
        trueExpr = const_cast<Expr*>(trueExpr->repExpr(str, expr));
        falseExpr = const_cast<Expr*>(falseExpr->repExpr(str, expr));
        return this;
    }

}
