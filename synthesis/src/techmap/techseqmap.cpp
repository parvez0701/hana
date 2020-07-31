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

#include "techseqmap.h"

#include "techmapmgr.h"
#include "scellmgr.h"
#include "genmapmgr.h"
#include "sport.h"
#include "sinst.h"
#include "sblock.h"

#include "sutils2.h"

using std::string;
using std::list;
using std::vector;
using std::pair;
using std::make_pair;
using namespace stview;

namespace TechnologyMapping
{
    TechSeqMap::TechSeqMap(SCell::CellType type) : TechOne2OneMap(type) {
    }

    TechSeqMap::~TechSeqMap() {
    }

    TechOne2OneMap::PortMap
    TechSeqMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap portMap;
        string genq = gencell->getSpecialPort("DATAOUT")->getIxName().getName();
        string techq = techcell->getSpecialPort("DATAOUT")->getIxName().getName();
        string gend = gencell->getSpecialPort("DATAIN")->getIxName().getName();
        string techd = gencell->getSpecialPort("DATAIN")->getIxName().getName();
        portMap.insert(make_pair(genq, techq));
        portMap.insert(make_pair(gend, techd));
        return portMap;
    } 

    const SInst*
    TechSeqMap::getNegEdgeInverter(const SInst *inst, const string& attrib) const
    {
        const SPin *spin = inst->getSpecialPin(attrib);
        const SPN *act = spin->getActual();

        if(act->dsize() != 1) {
            return 0;
        }

        SPin *pin = *(const_cast<SPN*>(act)->getDriver());
        const SInst *notinst = pin->getCellInst();
        const_cast<SPN*>(act)->reset();
        return notinst;
    }
    bool
    TechSeqMap::isNegEdge(const SInst *inst, const string& attrib) const {
        return this->getNegEdgeInverter(inst, attrib) != 0;
    }
    bool
    TechSeqMap::isNegEdge(const SCell *cell, const string& attrib) const
    {
        //negedge is marked by attribute val 0
        if(const SPort *port = cell->getSpecialPort(attrib)) {
            return port->getAttribute(attrib) == (void*)0;
        }
        return false;
    }

    const SCell*
    TechSeqMap::getEdgeTechCell(SCell::CellType type, int insize, 
        const string& attrib, bool negedge) const
    {
        vector<const SCell*> cells = SCellMgr::instance()->getTechCells(type, insize);
        for(int i = 0; i < cells.size(); ++i)
        {
            const SPort *p = cells[i]->getSpecialPort(attrib);
            if(p) {
                if((negedge && (p->getAttribute(attrib) == (void*)0)) ||
                    (!negedge && (p->getAttribute(attrib) == (void*)1))) {
                    return cells[i];
                }    
            }
        }
        return  0;
    }
    const SCell*
    TechSeqMap::getNegEdgeTechCell(SCell::CellType type, int insize, 
        const string& attrib) const {
        return this->getEdgeTechCell(type, insize, attrib, true);
    }    
    const SCell*
    TechSeqMap::getPosEdgeTechCell(SCell::CellType type, int insize, 
        const string& attrib) const {
        return this->getEdgeTechCell(type, insize, attrib, false);
    }    

    bool
    TechSeqMap::addInverter(SBlock *block, SInst *inst, const string& attrib) const
    {
        //if(!TechMapMgr::instance()->cellExists(SCell::NOT, 1)) {
        //    return false;
        //}    
        bool generic = !TechMapMgr::instance()->cellExists(SCell::NOT, 1);
        SInst *invinst = CreateInstance(block, SCell::NOT,
            "NOT", 1, generic);
        
        SPin *pin = inst->getSpecialPin(attrib);
        invinst->getSpecialPin(DATAIN)->setActual(pin->getActual());
        pin->removeActual();
        block->connect(pin, invinst->getSpecialPin(DATAOUT));
        return true;
    }    
        
    bool
    TechSeqMap::removeInverter(SBlock *block, SInst *inst, const string& attrib) const
    {
        SInst *invinst = const_cast<SInst*>(this->getNegEdgeInverter(inst, attrib));
        if(!invinst) {
            return false;
        }
        
        inst->getSpecialPin(attrib)->removeActual();

        block->connect(invinst->getSpecialPin(DATAIN), 
            inst->getSpecialPin(attrib));
        
        block->deleteCellInst(invinst);
        return true;
    }    

        
}
