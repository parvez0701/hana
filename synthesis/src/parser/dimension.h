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

#ifndef _DIMENSION_H_
#define _DIMENSION_H_
#include <vector>

#include "range.h"

namespace Rtl
{
    class Range;
    class Expr;
    struct Dimension{
        std::vector<Range*> packDim;
        std::vector<Range*> unpackDim;
        inline Dimension* copy() const;
    } ;

    Dimension*
    Dimension::copy() const {
        std::vector<Range*> cpackDim;
        std::vector<Range*> cunpackDim;
        int size = packDim.size();
        for(int i = 0; i < size; ++i){
            cpackDim.push_back(packDim[i]->copy());
        }
        size = unpackDim.size();
        for(int i = 0; i < size; ++i){
            cunpackDim.push_back(unpackDim[i]->copy());
        }
        struct Dimension* dim = new Dimension();
        dim->packDim = cpackDim;
        dim->unpackDim = cunpackDim;
        return dim;
    }
}
#endif
