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

#include <cassert>
#include <algorithm>

#include "concat.h"
#include "omvisitor.h"
#include "utils.h"
using std::string;
using std::vector;
namespace Rtl
{
    void
    Concat::accept(OMVisitorBase* v) const{
        v->visit(this);
    }

    string
    Concat::toString() const
    {
        const int size = exprs.size();
        string str("{");
        if(size == 0){
            return str + string("}");
        }
        str = str + exprs[0]->toString();
        for(int i = 1; i < size; ++i){
            str = str + string(", ") + exprs[i]->toString();
        }

        str = str + string("}");
        return str;
    }

    long
    Concat::len() const
    {
        int retlen = 0;
        const int size = exprs.size();
        for(int i = 0; i < size; ++i){
            retlen += exprs[i]->len();
        }
        return retlen;
    }

    const Expr*
    Concat::getBit(const long bit) const
    {
        long tolookfor = bit;
        const int size = exprs.size();
        for(int i = 0; i < size; ++i)
        {
            const int len = exprs[i]->len();
            if(len == -1){
                return this;
            }
            if(tolookfor < len){
                return exprs[i]->getBit(tolookfor);
            }
            else{
                tolookfor -= len;
            }
        }
        return this;
    }

    Concat*
    Concat::copy() const
    {
        vector<Expr*> cexprs;
        const int size = exprs.size();
        for(int i = 0; i < size; ++i) {
            cexprs.push_back(exprs[i]->copy());
        }
        return new Concat(cexprs);
    }
    bool
    Concat::isConstExpr() const
    {
        const int size = exprs.size();
        for(int i = 0; i < size; ++i) {
            if(!exprs[i]->isConstExpr()){
                return false;
            }
        }
        return true;
    }

    const Expr*
    Concat::repExpr(const string& str, const Expr* expr)
    {
        if(this->toString() == str) {
            return expr;
        }

        for(unsigned int i = 0; i < exprs.size(); ++i) {
            exprs[i] = const_cast<Expr*>(exprs[i]->repExpr(str, expr));
        }
        return this;
    }

    int
    Concat::evaluate() const
    {
        if(!isConstExpr()) {
            return INT_MIN;
        }
        vector<char> concbits;
        for(int i = 0; i < exprs.size(); ++i)
        {
            int val = exprs[i]->evaluate();
            assert(val != INT_MIN);
            vector<char> bits;
            do
            {
                bits.push_back(val%2 ? '1' : '0');
                val /= 2;
            } while(val);
            std::reverse(bits.begin(), bits.end());
            std::copy(bits.begin(), bits.end(), std::back_inserter(concbits));
        }
        return Evaluate('b', concbits);
    }

}
