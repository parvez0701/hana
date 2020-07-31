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

#include <math.h>

#include "bitsel.h"
#include "omvisitor.h"
#include "veccntv.h"
#include "integer.h"

using std::string;
using std::vector;
namespace Rtl
{
    string
    BitSel::toString() const
    {
        string str(var->toString());
        const int size = dim.size();
        for(int i = 0; i < size; ++i) {
            str = str + string("[") + dim[i]->toString() + string("]");
        }
        return str;
    }

    void
    BitSel::accept(OMVisitorBase* v) const {
        v->visit(this);
    }

    BitSel*
    BitSel::copy() const {
        vector<Expr*> cdim;
        const int size = dim.size();
        for(int i = 0; i < size; ++i){
            cdim.push_back(dim[i]->copy());
        }
        Expr* cvar = var->copy();
        return new BitSel(cvar, cdim);
    }

    bool 
    BitSel::isConstExpr() const
    {
        const int size = dim.size();
        for(int i = 0; i < size; ++i){
            if(!dim[i]->isConstExpr()){
                return false;
            }
        }
        return var->isConstExpr();
    }

    const Expr*
    BitSel::getActual() const 
    {
        if(!this->hasConstDim()){
            //this function is defined only for constant bit selects.
            return 0;
        }

        Integer *int_ = dynamic_cast<Integer*>(var);
        const VecCntv *vcntv = dynamic_cast<VecCntv*>(var);
        if(dim.size() == 1) {
            if(vcntv) {
                return vcntv->getBit(dim[0]->evaluate());
            }
            return int_->getIntBit(dim[0]->evaluate());
        }    
            
        
        const Dimension *vdim = vcntv->getDim();
        vector<Range*> ranges = vdim->packDim;
        std::copy(vdim->unpackDim.begin(), vdim->unpackDim.end(), std::back_inserter(ranges));
        int bit = dim[dim.size()-1]->evaluate();
            

        int j = 0;
        for(unsigned int i = 0; i < ranges.size()-1; ++i){
            bit += abs(ranges[i]->getLExpr()->evaluate() - 
                    ranges[i]->getRExpr()->evaluate())*
                dim[j++]->evaluate();
        }

        return vcntv->getBit(bit);
    }

    bool
    BitSel::hasConstDim() const {
        for(unsigned int i = 0; i < dim.size(); ++i){
            if(!dim[i]->isConstExpr()){
                return false;
            }
        }
        return true;
    }    

    const Expr*
    BitSel::repExpr(const string& str, const Expr *expr)
    {
        if(this->toString() == str) {
            return expr;
        }
        for(unsigned int i = 0; i < dim.size(); ++i) {
            dim[i] = const_cast<Expr*>(dim[i]->repExpr(str, expr));
        }    
        return this;
    }    

}
