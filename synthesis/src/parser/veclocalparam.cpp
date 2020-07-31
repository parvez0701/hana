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

#include "veclocalparam.h"
#include "omvisitor.h"

namespace Rtl
{
    VecLocalParam::VecLocalParam(const string& n, const Dimension* d,
        const bool is): VecCntv(n, d)
    {
        this->createLocalParamBits(n, d, is);
    }

    void
    VecLocalParam::createLocalParamBits(const string& name, const Dimension* dim,
        const bool isSig)
    {
        lparams = this->flatten<LocalParam>(name, dim);
        const int size = lparams.size();
        for(int i = 0; i < size; ++i){
            lparams[i]->setSign(isSig);
        }

    }
    void
    VecLocalParam::accept(OMVisitorBase* visitor) const {
        visitor->visit(this);
    }

    const LocalParam*
    VecLocalParam::getBit(const long bit) const
    {
        if(bit < 0 || bit > (long)lparams.size()){
            return NULL;
        }
        //return lparams[bit];
        return dynamic_cast<const LocalParam*>(VecCntv::getBit(bit));
    }

    VecLocalParam*
    VecLocalParam::copy() const
    {
        Dimension dim;
        Range* range = new Range(new IntConst(
            lparams[0]->evaluate()), new IntConst(
            lparams[lparams.size()-1]->evaluate()));
        dim.packDim.push_back(range);
        return new VecLocalParam(this->getName(), &dim, this->isSigned());
    }
    

}
