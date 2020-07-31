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

#include "techldmap.h"

#include "techmapmgr.h"
#include "genmapmgr.h"
#include "sport.h"

using std::string;
using std::list;
using std::pair;
using std::make_pair;
using namespace stview;

namespace TechnologyMapping
{
    TechLDMap::TechLDMap() : TechSeqMap(SCell::LD) {
    }

    TechLDMap::~TechLDMap() {
    }

    string 
    TechLDMap::getNameHint() const {
        return "LATCH";
    }

    TechOne2OneMap::PortMap
    TechLDMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap portMap = TechSeqMap::getPortMap(gencell, techcell);
        string genenable = gencell->getSpecialPort("ENABLE")->getIxName().getName();
        string techenable = techcell->getSpecialPort("ENABLE")->getIxName().getName();
        portMap.insert(make_pair(genenable, techenable));
        return portMap;
    } 

    const SCell*
    TechLDMap::getTechCell(const SInst *inst) const {
        return 0;
    }    

}
