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

#include "vecport.h"
#include "omvisitor.h"
#include "intconst.h"

namespace Rtl
{
    VecPort::VecPort(const string& n, const Dimension* d,
        const Dir dir): VecCntv(n, d)
    {
        this->createPortBits(n, d, dir);
    }

    void
    VecPort::createPortBits(const string& name, const Dimension* dim,
        const Dir dir)
    {
        ports = this->flatten<Port>(name, dim);
        const int size = ports.size();
        for(int i = 0; i < size; ++i){
            ports[i]->setDir(dir);
        }

    }
/*
    Port*
    VecPort::getCntvBit(const long bit) const throw(Error){
        if(bit < 0 || bit > ports.size()){
            Error err = {__LINE__, __FILE__, "Bit out of range"};
            throw err;
        }

        return ports[bit];
    }
*/
    void
    VecPort::accept(OMVisitorBase* visitor) const {
        visitor->visit(this);
    }

    const Port*
    VecPort::getBit(const long bit) const {
        return static_cast<const Port*>(VecCntv::getBit(bit));
    }

    void
    VecPort::setVarType(const string& vt)
    {
        const int size = ports.size();
        for(int i = 0; i < size; ++i){
            ports[i]->setVarType(vt);
        }
    }

    VecPort*
    VecPort::copy() const
    {
        //no deep copy
        return const_cast<VecPort*>(this);
    /*
        Dimension dim;
        Range* range = new Range(new IntConst(
            ports[0]->getIxName().getIndex()), new IntConst(
            ports[ports.size()-1]->getIxName().getIndex()));
        dim.packDim.push_back(range);    
        VecPort *vport = new VecPort(this->getName(), &dim, this->getDir());
        vport->setVarType(this->getVarType());
        return vport;
    */    
    }
        
    

}
