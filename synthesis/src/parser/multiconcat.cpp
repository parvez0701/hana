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

#include "multiconcat.h"
#include "omvisitor.h"
using std::string;
using std::vector;
namespace Rtl
{
    void
    MultiConcat::accept(OMVisitorBase* v) const{
        v->visit(this);
    }

    string
    MultiConcat::toString() const
    {
        const vector<Expr*>& exprs = this->getExprs();
        string str("{ ");
        str = str + count->toString();
        str = str + string(" {");
        const int size = exprs.size();
        if( size == 0)
        {
            str = str + string("} }");
            return str;
        }

        str = str + exprs[0]->toString();
        for(int i = 1; i < size; ++i){
            str = str + string(", ") + exprs[i]->toString();
        }
        str = str + string("} }");
        return str;
    }

    long
    MultiConcat::len() const
    {
        const vector<Expr*>& exprs = this->getExprs();
        long retlen = 0;
        const int size = exprs.size();
        for(int i = 0; i < size; ++i){
            retlen += exprs[i]->len();
        }

        return count->evaluate() * retlen;
    }

    const Expr*
    MultiConcat::getBit(const long bit) const
    {
        long countlen = count->evaluate();
        const vector<Expr*>& exprs = this->getExprs();
        long tolookfor = bit;
        const int size = exprs.size();
        for(int i = 0; i < countlen; ++i)
        {
            for(int j = 0; j < size; ++j)
            {
                const int len = exprs[j]->len();
                if(len == -1){
                    return this;
                }
                if(tolookfor < len){
                    return exprs[j]->getBit(tolookfor);
                }
                else{
                    tolookfor -= len;
                }
            }
        }
        return this;
    }

    MultiConcat*
    MultiConcat::copy() const
    {
        const vector<Expr*>& oldexprs = this->getExprs();
        vector<Expr*> newexprs;

        for(int i = 0; i < oldexprs.size(); ++i) {
            newexprs.push_back(oldexprs[i]->copy());
        }    
        Expr *newcount = count->copy();
        return new MultiConcat(newcount, newexprs);
    }

    int
    MultiConcat::evaluate() const
    {
        if(!isConstExpr()) {
            return INT_MIN;
        }    
        vector<Expr*> texprs;
        const vector<Expr*>& exprs = this->getExprs();
        int count_ = count->evaluate();

        for(int i = 0; i < count_; ++i) {
            std::copy(exprs.begin(), exprs.end(), std::back_inserter(texprs));
        }

        Concat conc(texprs);
        return conc.evaluate();
    }

}
