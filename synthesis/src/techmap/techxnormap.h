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

#ifndef _TECHXNORMAP_H_
#define _TECHXNORMAP_H_

#include "technxnxmap.h"

namespace TechnologyMapping
{
    class TechXnorMap : public TechNXNXMap
    {
        public:
            TechXnorMap(bool=false);
            virtual ~TechXnorMap();
            virtual std::string getNameHint() const;
        private:
            TechXnorMap(const TechXnorMap&);
            TechXnorMap& operator=(const TechXnorMap&);
    };


    TechXnorMap::TechXnorMap(bool merge) : 
        TechNXNXMap(stview::SCell::XNOR, merge) {
    }

    TechXnorMap::~TechXnorMap() {
    }

    std::string
    TechXnorMap::getNameHint() const {
        return "XNOR";
    }    
}

#endif
