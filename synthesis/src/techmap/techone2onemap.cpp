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

#include "techone2onemap.h"

#include "techmapmgr.h"
#include "genmapmgr.h"
#include "sutils2.h"

#include "sinst.h"
#include "sblock.h"

using std::list;

using namespace stview;
namespace TechnologyMapping
{
    TechOne2OneMap::TechOne2OneMap(SCell::CellType t) : type(t) {
    }


    TechOne2OneMap::~TechOne2OneMap() {
    }

    SCell::CellType
    TechOne2OneMap::getType() const {
        return type;
    }    

    bool
    TechOne2OneMap::condition(const SInst *inst) const
    {
        if(inst->getCellType() != type) {
            return false;
        }
        int input = inst->getInPins().size();
        if(inst->isGeneric())
        {
            if(TechMapMgr::instance()->cellExists(type, input)) {
                return true;
            }
        }
        return false;
    }

    bool
    TechOne2OneMap::doMapping(SBlock *block, SInst *inst)
    {
        const list<SPin*>& inpins = inst->getInPins();
        const list<SPin*>& outpins = inst->getOutPins();
        const list<SPin*>& iopins = inst->getInOutPins();

        SInst *newinst = CreateInstance(block, type, this->getNameHint(),
            inpins.size(), false);

        this->connectUsingPortMap(inst, newinst);
        block->deleteCellInst(inst);
        return true;
    }

    void
    TechOne2OneMap::connectUsingPortMap(SInst *oldinst, SInst *newinst) const
    {
        PortMap portMap = this->getPortMap(oldinst->getCell(), newinst->getCell());

        for(PortMap::const_iterator iter = portMap.begin(); 
            iter != portMap.end(); ++iter) {
            newinst->getPin(iter->second)->setActual(
                oldinst->getPin(iter->first)->getActual());
        }
    }

    TechOne2OneMap::PortMap
    TechOne2OneMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap portMap;
        this->getPortMap(gencell, techcell, "DATAIN", portMap);
        this->getPortMap(gencell, techcell, "DATAOUT", portMap);
        return portMap;
    }
    void
    TechOne2OneMap::getPortMap(const SCell *gencell, const SCell *techcell,
        const string& attrib, PortMap& portMap) const
    {
        list<const SPort*> gports = gencell->getSpecialPorts(attrib);
        list<const SPort*> tports = techcell->getSpecialPorts(attrib);

        assert(gports.size() == tports.size());
        list<const SPort*>::const_iterator gport = gports.begin();
        list<const SPort*>::const_iterator tport = tports.begin();

        for(; (gport != gports.end()) && (tport != tports.end()); ++gport, ++tport) {
            portMap.insert(make_pair((*gport)->getName(), (*tport)->getName()));
        }
    }    


/*    TechOne2OneMap::PortMap
    TechOne2OneMap::getPortMap() const
    {
        const SCell *gencell = GenericMapping::GenMapMgr::instance()->
            getCell(this->getType(), 1);
        const SCell *techcell = TechMapMgr::instance()->getCell(this->getType(), 1);
        string genin = gencell->getSpecialPort("DATAIN")->getName();
        string techin = techcell->getSpecialPort("DATAIN")->getName();
        string genout = gencell->getSpecialPort("DATAOUT")->getName();
        string techout = techcell->getSpecialPort("DATAOUT")->getName();
        PortMap pmap;
        pmap.insert(make_pair(genin, techin));
        pmap.insert(make_pair(genout, techout));
        return pmap;

    }
    */
}
