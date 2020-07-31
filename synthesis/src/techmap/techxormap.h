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

#ifndef _TECHXORMAP_H_
#define _TECHXORMAP_H_

#include "technxnxmap.h"

namespace TechnologyMapping
{
    class TechXorMap : public TechNXNXMap
    {
        public:
            TechXorMap(bool=false);
            virtual ~TechXorMap();
            virtual std::string getNameHint() const;
        private:
            TechXorMap(const TechXorMap&);
            TechXorMap& operator=(const TechXorMap&);
    };


    TechXorMap::TechXorMap(bool merge) : 
        TechNXNXMap(stview::SCell::XOR, merge) {
    }

    TechXorMap::~TechXorMap() {
    }

    std::string
    TechXorMap::getNameHint() const {
        return "XOR";
    }    
}

#endif
