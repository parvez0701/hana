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

#include "vecint.h"
using std::vector;
namespace Rtl
{

    const Integer*
    VecInt::getBit(const long bit) const
    {
        const vector<Var*>& vars = this->getVars();
        if(bit < 0 || bit > (long)vars.size()){
            return NULL;
        }
        return static_cast<const Integer*>(vars[bit]);
    }

    VecInt*
    VecInt::copy() const {
        return new VecInt(this->getName(), this->createDim(),
            this->isSigned());
    }        
    
/*
    void
    VecInt::flatten()
    {
        vector<Var*>& vars = const_cast<vector<Var*>&>(this->getVars());

        vector<Var*> tvars = vars;
        vars.empty();
        int size = tvars.size();
        long index = 0;

        for(int i = 0; i < size; ++i)
        {
            for(int j = 0; j < 32; ++j){
                vars.push_back(new Var(IxName(
                tvars[i]->getIxName().getName(), index++)));
            }
        }
    }
*/
}
