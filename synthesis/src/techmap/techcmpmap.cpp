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

#include "techcmpmap.h"

#include "sport.h"

using std::string;
using std::make_pair;

using namespace stview;
namespace TechnologyMapping
{
    TechCmpMap::TechCmpMap() : TechOne2OneMap(SCell::CMP) {
    }

    TechCmpMap::~TechCmpMap() {
    }

    string
    TechCmpMap::getNameHint() const {
        return "CMP";
    }

    TechOne2OneMap::PortMap
    TechCmpMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap portMap = TechOne2OneMap::getPortMap(gencell, techcell);
        string geneq = gencell->getSpecialPort("EQ")->getName();
        string techeq = techcell->getSpecialPort("EQ")->getName();
        string genueq = gencell->getSpecialPort("UEQ")->getName();
        string techueq = techcell->getSpecialPort("UEQ")->getName();
        string genlt = gencell->getSpecialPort("LT")->getName();
        string techlt = techcell->getSpecialPort("LT")->getName();
        string gengt = gencell->getSpecialPort("GT")->getName();
        string techgt = techcell->getSpecialPort("GT")->getName();

        portMap.insert(make_pair(geneq, techeq));
        portMap.insert(make_pair(genueq, techueq));
        portMap.insert(make_pair(genlt, techlt));
        portMap.insert(make_pair(gengt, techgt));
        return portMap;
    }
}
