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

#include "vecnet.h"
#include "dimension.h"
#include "range.h"
#include "omvisitor.h"
#include "intconst.h"

namespace Rtl
{
    VecNet::VecNet(const string& n, const Dimension* d): VecCntv(n, d)
    {
        this->createNetBits(n, d);
    }

    void
    VecNet::createNetBits(const string& name, const Dimension* dim)
    {
        nets = this->flatten<Net>(name, dim);
    }

    const Net*
    VecNet::getBit(const long bit) const{
        if(bit < 0 || bit > (long)nets.size()){
            return NULL;
        }

        //return nets[bit];
        return static_cast<const Net*>(VecCntv::getBit(bit));
    }

    void
    VecNet::accept(OMVisitorBase* visitor) const {
        visitor->visit(this);
    }

    VecNet*
    VecNet::copy() const
    {
        //no deep copy
        return const_cast<VecNet*>(this);
    /*
        Dimension* dim = new Dimension();
        Range* range = new Range(new IntConst(
            nets[0]->getIxName().getIndex()), new IntConst(
            nets[nets.size()-1]->getIxName().getIndex()));
        dim->packDim.push_back(range);    
        return new VecNet(this->getName(), dim);
    */    
    }
    
    

}
