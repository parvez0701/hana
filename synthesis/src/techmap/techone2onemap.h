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

#ifndef _TECHONE2ONEMAP_H_
#define _TECHONE2ONEMAP_H_


#include "techmapbase.h"
#include "scell.h"

namespace TechnologyMapping
{
    class TechOne2OneMap: public TechMapBase
    {
        public:
            virtual ~TechOne2OneMap();
        protected:
            TechOne2OneMap(stview::SCell::CellType);

            virtual bool condition(const stview::SInst*) const;
            virtual bool doMapping(stview::SBlock *, stview::SInst*);

            typedef std::map<std::string, std::string, stview::cmp_str > PortMap;

            virtual PortMap getPortMap(const stview::SCell*, 
                const stview::SCell*) const;
            stview::SCell::CellType getType() const;

            void getPortMap(const stview::SCell*, const stview::SCell*,
                const std::string&, PortMap&) const;
            
            void connectUsingPortMap(stview::SInst*, stview::SInst*) const;
        private:
            TechOne2OneMap();
            TechOne2OneMap(const TechOne2OneMap&);
            TechOne2OneMap& operator=(const TechOne2OneMap&);

            stview::SCell::CellType type;

    };
}
#endif
