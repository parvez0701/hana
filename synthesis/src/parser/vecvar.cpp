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

#include "vecvar.h"
#include "omvisitor.h"
#include "omcreator.h"
#include "intconst.h"

namespace Rtl
{
    VecVar::VecVar(const string& n, const Dimension* d,
        const bool is): VecCntv(n, d)
    {
        this->setScope(OMCreator::instance()->getCurrScope());
//        this->createVarBits(n, d, is);
    }

    void
    VecVar::createVarBits(const string& name, const Dimension* dim,
        const bool isSig)
    {
/*        vars = this->flatten<Var>(name, dim);
        const int size = vars.size();
        for(int i = 0; i < size; ++i){
            vars[i]->setSign(isSig);
            vars[i]->setScope(this->getScope());
        }
        */

    }
//    void
//    VecVar::accept(OMVisitor* visitor) const {
//        visitor->visit(this);
//    }

    Dimension*
    VecVar::createDim() const
    {
        Dimension* dim = new Dimension();
        Range* range = new Range(new IntConst(
            vars[0]->getIxName().getIndex()), new IntConst(
            vars[vars.size()-1]->getIxName().getIndex()));
        dim->packDim.push_back(range);
        return dim;
    }
    

}
