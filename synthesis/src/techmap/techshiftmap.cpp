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

#include "techshiftmap.h"

#include "sport.h"
#include "genmapmgr.h"
#include "techmapmgr.h"

using std::string;
using std::list;
using std::pair;
using std::make_pair;
using namespace stview;

namespace TechnologyMapping
{
    TechShiftMap::TechShiftMap(SCell::CellType type) : TechOne2OneMap(type) {
    }

    TechShiftMap::~TechShiftMap() {
    }


    TechOne2OneMap::PortMap
    TechShiftMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap portMap;
        TechOne2OneMap::getPortMap(gencell, techcell, "DATAIN", portMap);
        TechOne2OneMap::getPortMap(gencell, techcell, "DATAOUT", portMap);
        TechOne2OneMap::getPortMap(gencell, techcell, "SHIFT", portMap);

        string genshiftval = gencell->getSpecialPort("SHIFTVAL")->getIxName().getName();
        string techshiftval = gencell->getSpecialPort("SHIFTVAL")->getIxName().getName();
        portMap.insert(make_pair(genshiftval, techshiftval));
        return portMap;
    } 

}
