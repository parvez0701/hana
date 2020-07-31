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

#include "techmuxmap.h"

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
    TechMuxMap::TechMuxMap() : TechOne2OneMap(SCell::MUX) {
    }

    TechMuxMap::~TechMuxMap() {
    }

    string 
    TechMuxMap::getNameHint() const {
        return "MUX";
    }

    TechOne2OneMap::PortMap
    TechMuxMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap portMap = TechOne2OneMap::getPortMap(gencell, techcell);
        TechOne2OneMap::getPortMap(gencell, techcell, "SELECT", portMap);
        return portMap;
    } 
    pair<string, string>
    TechMuxMap::getInputsBaseName(const SCell *cell) const
    {
        pair<string, string> inputs;
        const list<SPort*>& ports = cell->getPorts();
        for(list<SPort*>::const_iterator port = ports.begin();
            port != ports.end(); ++port) 
        {
            if((*port)->getDir() != SPort::IN) {
                continue;
            }
            if(!(*port)->getAttribute("DATAIN")) {
                continue;
            }
            string name = (*port)->getIxName().getName();
            if(inputs.first.empty()) {
                inputs.first = name;
            }
            else if(inputs.first != name) 
            {
                inputs.second = name;
                break;
            }
        }    
        return inputs;
    }    

}
