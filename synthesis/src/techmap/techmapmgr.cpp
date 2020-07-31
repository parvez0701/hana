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

#include "techmapmgr.h"
#include "scellmgr.h"
#include "sroot.h"
#include "sblock.h"
#include "sinst.h"

#include "techandmap.h"
#include "techormap.h"
#include "technandmap.h"
#include "technormap.h"
#include "techxormap.h"
#include "techxnormap.h"
#include "techaddmap.h"
#include "techsubmap.h"
#include "techbufmap.h"
#include "techinvmap.h"
#include "techgndmap.h"
#include "techvccmap.h"
#include "techincmap.h"
#include "techmulmap.h"
#include "techmuxmap.h"
#include "techlshiftmap.h"
#include "techrshiftmap.h"
#include "techffmap.h"
#include "techldmap.h"
#include "techtribufmap.h"
#include "techcmpmap.h"

#include "cellsemcheckmgr.h"

using std::list;

using namespace stview;
static const int MAX_IN_SIZE = 1024;
namespace TechnologyMapping
{
    TechMapMgr* TechMapMgr::techMapMgr = 0;

    TechMapMgr::TechMapMgr() {
        init();
    }

    TechMapMgr::~TechMapMgr() {
        techMapMgr = 0;
    }

    TechMapMgr*
    TechMapMgr::instance()
    {
        if(!techMapMgr) {
            techMapMgr = new TechMapMgr();
        }
        return techMapMgr;
    }

    void
    TechMapMgr::init()
    {
        mappers.push_back(new TechAndMap());
        mappers.push_back(new TechOrMap());
        mappers.push_back(new TechNandMap());
        mappers.push_back(new TechNorMap());
        mappers.push_back(new TechXorMap());
        mappers.push_back(new TechXnorMap());
        mappers.push_back(new TechAddMap());
        mappers.push_back(new TechSubMap());
        mappers.push_back(new TechBufMap());
        mappers.push_back(new TechInvMap());
        mappers.push_back(new TechGndMap());
        mappers.push_back(new TechVccMap());
        mappers.push_back(new TechIncMap());
        mappers.push_back(new TechMulMap());
        mappers.push_back(new TechMuxMap());
        mappers.push_back(new TechLShiftMap());
        mappers.push_back(new TechRShiftMap());
        mappers.push_back(new TechFFMap());
        mappers.push_back(new TechRFFMap());
        mappers.push_back(new TechSFFMap());
        mappers.push_back(new TechRSFFMap());
        mappers.push_back(new TechSRFFMap());
        mappers.push_back(new TechLDMap());
        mappers.push_back(new TechTribufMap());
        mappers.push_back(new TechCmpMap());

        for(SCell::CellType t = SCell::CELLTYPE_BEGIN; t <= SCell::CELLTYPE_END; ++t) {
            cellTable.push_back(SCells());
        }    

        for(SCell::CellType t = SCell::GND; t < SCell::CELLTYPE_END; ++t) 
        {
            const SCell *max = 0;
            for(int i = 0; i < MAX_IN_SIZE; ++i) 
            {
                const SCell *cell = SCellMgr::instance()->getTechCell(t, i);
                cellTable[t].push_back(cell);

                if(cell) {
                    max = cell;
                }    
            }    
            cellMaxTable.push_back(max);
        }    
    }

    void
    TechMapMgr::doMapping()
    {
        if(!CellSemCheckMgr::instance()->isOk()) {
            return;
        }    
        const list<SBlock*>& blocks = SRoot::instance()->getBlocks();
        list<SBlock*>::const_iterator blockIter = blocks.begin();
        list<SBlock*>::const_iterator blockIterEnd = blocks.end();

        for(; blockIter != blockIterEnd; ++blockIter)
        {
            SBlock *block = *blockIter;
            bool netlistChanged = true;
            while(netlistChanged)
            {
                //keep doing until there is no change in the netlist
                netlistChanged = false;
                list<TechMapBase*>::iterator mapper = mappers.begin();
                for(; mapper != mappers.end(); ++mapper) 
                {
                    (*mapper)->clear();
                    (*mapper)->init();
                    const list<SInst*>& insts = block->getInsts();
                    list<SInst*>::const_iterator inst = insts.begin();
                    for(; inst != insts.end(); ++inst) {
                        (**mapper)(*inst);
                    }
                    (*mapper)->init();
                    if((*mapper)->doMapping(block)) {
                        netlistChanged = true;
                    }
                }    
            }
        }

    }

    bool
    TechMapMgr::cellExists(SCell::CellType type, int input) const {
        return cellTable[type][input] != 0;
    }

    const SCell*
    TechMapMgr::getCell(SCell::CellType type, int input) const {  
        return cellTable[type][input];
    }
    const SCell*
    TechMapMgr::getCell(SCell::CellType type) const 
    {
        for(int i = 0; i < MAX_IN_SIZE; ++i) {
            if(cellTable[type][i]) {
                return cellTable[type][i];
            }
        }    
        return 0;
    }    

    const SCell*
    TechMapMgr::getCellMinInput(SCell::CellType type) const {
        return this->getCell(type);
    }

    const SCell*
    TechMapMgr::getCellMaxInput(SCell::CellType type) const {
        return cellMaxTable[type];
    }

    int
    TechMapMgr::getInputSize(const SCell *cell) const {
        return cell->getSpecialPorts("DATAIN").size();
    }

    int
    TechMapMgr::getInputSize(const SInst *inst) const {
        return this->getInputSize(inst->getCell());
    }    

}
