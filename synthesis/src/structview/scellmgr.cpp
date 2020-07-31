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

#include <assert.h>
#include "scellmgr.h"

#include "libraryreader.h"
#include "scell.h"
#include "sport.h"

using std::map;
using std::vector;
using std::string;
using std::list;
using std::make_pair;
namespace stview
{
    SCellMgr *SCellMgr::cellMgr = 0;

    SCellMgr::SCellMgr() {
        status = init();
    }

    SCellMgr::~SCellMgr() {
    }

    SCellMgr*
    SCellMgr::instance() {
        if(SCellMgr::cellMgr == 0){
            SCellMgr::cellMgr = new SCellMgr();
        }

        return SCellMgr::cellMgr;
    }

    bool
    SCellMgr::init()
    {
        for(SCell::CellType i = SCell::CELLTYPE_BEGIN; i <= SCell::CELLTYPE_END; ++i) {
            cellInfoTable.push_back(CellInfoVector());
        }    
        for(SCell::CellType i = SCell::CELLTYPE_BEGIN; i < SCell::CELLTYPE_END; ++i) 
        {
            cellInfoTable[i].push_back(CellInfo());
            cellInfoTable[i][0].type = i;
        }

        bool generic = true;
        do {
            if(LibraryReader::instance()->readLibrary(generic)) {
                return false;
            }
            if(!doChecksAndAdd(generic)) {
                return false;
            }
            generic = !generic;
        }while(!generic);    
        return false;
    }

    bool
    SCellMgr::doChecksAndAdd(bool generic)
    {
        bool status = true;
        list<SCell*>* cells = LibraryReader::instance()->getCells();
        if(!cells){
            //nothing to be done
            return true;
        }
        list<SCell*>::iterator cellBegin = cells->begin();
        list<SCell*>::iterator cellEnd = cells->end();

        for(; cellBegin != cellEnd; ++cellBegin) 
        {
            (*cellBegin)->generic = generic;
            if(!doChecksAndAdd(*cellBegin, generic)) {
                status = false;
            }    
        }
        return status;
    }

    bool
    SCellMgr::doChecksAndAdd(SCell *cell, bool generic)
    {
        //no checks done at this moment
        bool status = true;
        
        //CellBitBlaster::instance()->doBlast(cell);
        const list<SPort*>& ports = cell->getPorts();
        int count = 0;
        for(list<SPort*>::const_iterator port = ports.begin();
            port != ports.end(); ++port){
            if((*port)->getDir() == SPort::IN) {
                ++count;
            }
        }    
        CellInfo cellInfo(cell->getName(), cell->getCellType(), count,
            generic ? 1 : 2) ;
        addCell(cell, cellInfo);
        return status;
    }

    bool
    SCellMgr::addGenericCell(SCell *cell)
    {
        return genericCellTable.insert(
            make_pair(cell->getName(), cell)).second;
    }        
    bool
    SCellMgr::addTechCell(SCell *cell)
    {
        return techCellTable.insert(
            make_pair(cell->getName(), cell)).second;
    }        

    void
    SCellMgr::addCellInfo(CellInfo& cellInfo) {
        cellInfoTable[cellInfo.type].push_back(cellInfo);
    }    

    bool
    SCellMgr::addCell(SCell *cell, CellInfo& cellInfo)
    {
        assert(cell->getCellType() == cellInfo.type);

        addCellInfo(cellInfo);
        return cell->isGeneric() ? addGenericCell(cell) :
            addTechCell(cell);
    }        


    const SCell*
    SCellMgr::getGenericCell(const string& name) const
    {
        CellTable::const_iterator iter = genericCellTable.find(name);
        if(iter == genericCellTable.end()) {
            return 0;
        }
        return iter->second;
    }

    const SCell*
    SCellMgr::getTechCell(const string& name) const
    {
        CellTable::const_iterator iter = techCellTable.find(name);
        if(iter == techCellTable.end()) {
            return 0;
        }
        return iter->second;
    }    
    const SCell*
    SCellMgr::getCell(const string& name) const 
    {
        if(const SCell *cell = this->getTechCell(name)) {
            return cell;
        }
        return this->getGenericCell(name);
    }

    const SCell*
    SCellMgr::getCell(SCell::CellType type, short inputSize) const
    {
        const SCell *cell = this->getTechCell(type, inputSize);
        return cell ? cell : this->getGenericCell(type, inputSize);
    }

    const SCell*
    SCellMgr::getTechCell(SCell::CellType type, short inputSize) const
    {
        assert((type > (short)SCell::CELLTYPE_BEGIN) &&
            (type < (short) SCell::CELLTYPE_END));
        
        CellInfoVector cellInfos = cellInfoTable[type];
        for(int i = 0; i < cellInfos.size(); ++i) {
            if((cellInfos[i].inputSize == inputSize) &&
                (cellInfos[i].flag == 2)) {
                return this->getTechCell(cellInfos[i].name);
            }
        }
        return NULL;
    }

    vector<const SCell*>
    SCellMgr::getTechCells(SCell::CellType type, short inputSize) const
    {
        assert((type > (short)SCell::CELLTYPE_BEGIN) &&
            (type < (short) SCell::CELLTYPE_END));

        vector<const SCell*> cells;    
        
        CellInfoVector cellInfos = cellInfoTable[type];
        for(int i = 0; i < cellInfos.size(); ++i) {
            if((cellInfos[i].inputSize == inputSize) &&
                (cellInfos[i].flag == 2)) {
                cells.push_back(this->getTechCell(cellInfos[i].name));
            }
        }
        return cells;
    }

    const SCell*
    SCellMgr::getGenericCell(SCell::CellType type, short inputSize) const
    {
        assert((type > (short)SCell::CELLTYPE_BEGIN) &&
            (type < (short) SCell::CELLTYPE_END));
        
        CellInfoVector cellInfos = cellInfoTable[type];
        for(int i = 0; i < cellInfos.size(); ++i) {
            if((cellInfos[i].inputSize == inputSize) &&
                (cellInfos[i].flag == 1)) {
                return this->getGenericCell(cellInfos[i].name);
            }
        }
        return NULL;
    }

}
