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

#include "vecpin.h"
#include "intconst.h"
#include "omvisitor.h"

namespace Rtl
{
    VecPin::VecPin(const string& n, const Dimension* d,
        const Dir dir, const Expr* a):
         VecCntv(n, d), actual(const_cast<Expr*>(a))
    {
        this->createPinBits(n, d, dir, a);
        
    }

    void
    VecPin::createPinBits(const string& name, const Dimension* dim,
        const Dir dir, const Expr* expr)
    {
        pins = this->flatten<Pin>(name, dim);
        bool isbehav = !expr;
        if(expr){
            isbehav = (expr->len() == -1);
        }
        const int size = pins.size();
        for(int i = 0; i < size; ++i)
        {
            if(isbehav){
                pins[i]->setActual(expr);
            }
            else {
                pins[i]->setActual(expr->getBit(i));
            }
            pins[i]->setDir(dir);
        }
        
    }

    const Pin*
    VecPin::getBit(const long bit) const{
        return static_cast<const Pin*>(VecCntv::getBit(bit));
    }

    void
    VecPin::accept(OMVisitorBase* visitor) const{
        visitor->visit(this);
    }

    VecPin*
    VecPin::copy() const
    {
        Dimension dim;
        Range *range = new Range(
            new IntConst(pins[0]->getIxName().getIndex()),
            new IntConst(pins[pins.size()-1]->getIxName().getIndex()));
        dim.packDim.push_back(range);
        
        return new VecPin(this->getName(), &dim, this->getDir(),
            actual ? actual->copy() : 0);
    }
    

}
