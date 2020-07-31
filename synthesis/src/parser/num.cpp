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

#include "num.h"
#include "omvisitor.h"

using std::string;
using std::vector;
namespace Rtl
{
    void
    Num::accept(OMVisitorBase* v) const {
        v->visit(this);
    }

    string
    Num::toString() const
    {
        char str[1024];
        sprintf(str, "%ld", num);
        return str;
    }
    int
    Num::evaluate() const{
        return num;
    }

    const BasedNum*
    Num::getBit(const long bit) const
    {
        if(bit < 0 || bit > this->len()){
            return NULL;
        }
        long val = num;
        val = val >> (bit-1);
        val = val & 1;
        vector<char> bits;
        bits.push_back( val ? '1': '0');
        return new BasedNum(1, 'b', false, bits);
    }

        

}
