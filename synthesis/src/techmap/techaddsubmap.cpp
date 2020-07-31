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

#include "techaddsubmap.h"
#include "scell.h"
#include "sport.h"

#include "genmapmgr.h"
#include "techmapmgr.h"

using std::pair;
using std::make_pair;
using std::string;
using std::list;

using stview::SCell;
using stview::SPort;
namespace TechnologyMapping
{
    TechAddSubMap::TechAddSubMap(SCell::CellType type): TechOne2OneMap(type) {
    }

    TechAddSubMap::~TechAddSubMap() {
    }

    TechOne2OneMap::PortMap
    TechAddSubMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap pmap = TechOne2OneMap::getPortMap(gencell, techcell);
        pair<string, string> gencinout = make_pair(gencell->getSpecialPort("CIN")->getName(),
            gencell->getSpecialPort("COUT")->getName());
        pair<string, string> techcinout = make_pair(techcell->getSpecialPort("CIN")->getName(),
            techcell->getSpecialPort("COUT")->getName());

        pmap.insert(make_pair(gencinout.first, techcinout.first));
        pmap.insert(make_pair(gencinout.second, techcinout.second));
        return pmap;

    }


}
