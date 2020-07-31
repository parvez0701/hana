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

#include "techffmap.h"

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
    TechFFMap::TechFFMap(SCell::CellType type) : TechSeqMap(type) {
    }

    TechFFMap::~TechFFMap() {
    }

    string 
    TechFFMap::getNameHint() const {
        return "FF";
    }

    TechOne2OneMap::PortMap
    TechFFMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap portMap = TechSeqMap::getPortMap(gencell, techcell);
        string genclk = gencell->getSpecialPort("CLOCK")->getIxName().getName();
        string techclk = techcell->getSpecialPort("CLOCK")->getIxName().getName();
        portMap.insert(make_pair(genclk, techclk));
        return portMap;
    } 

    bool
    TechFFMap::isNegEdgeClock(const SInst *inst) const {
        return this->isNegEdge(inst, "CLOCK");
    }

    const SCell*
    TechFFMap::getTechCell(const SInst *inst) const
    {
        vector<const SCell*> cells = SCellMgr::instance()->getTechCells(SCell::FF, 2);
        bool negedge = this->isNegEdgeClock(inst);
        for(int i = 0; i < cells.size(); ++i)
        {
            if((negedge && (cells[i]->getSpecialPort("CLOCK")->
                getAttribute("CLOCK") == (void *)0) ) || 
                ((!negedge && (cells[i]->getSpecialPort("CLOCK")->
                getAttribute("CLOCK") == (void *)1) ))) {
                    return cells[i];
            }
        }
        return 0;
    }

    bool
    TechFFMap::doMapping(SBlock *block, SInst* inst)
    {
        //Use cases:
        //1.    posedge/negedge FF infered, no FF cell definition exists in the lib     
        //2.    posedge FF infered, posedge FF exists in the lib    
        //3.    posedge FF infered, negedge FF exists in the lib    
        //4.    negedge FF infered, posedge FF exists in the lib    
        //5.    negedge FF infered, negedge FF exists in the lib    


        bool negedge = this->isNegEdgeClock(inst);
        const  SCell *cell = negedge ? this->getNegEdgeTechCell(SCell::FF, 2, CLOCK) : 
        this->getPosEdgeTechCell(SCell::FF, 2, CLOCK);
        if(negedge)
        {
            if(cell)
            {
                SInst *newinst = CreateInstance(block, cell, this->getNameHint());
                SInst *invinst = const_cast<SInst*>(this->getNegEdgeInverter(inst, "CLOCK"));
                this->connectUsingPortMap(inst, newinst);
                newinst->getSpecialPin("CLOCK")->setActual(
                invinst->getSpecialPin("DATAIN")->getActual());
        
                block->deleteCellInst(inst);
                block->deleteCellInst(invinst);
                return true;
            }
            cell = this->getPosEdgeTechCell(SCell::FF, 2, CLOCK);
            if(!cell) {
                return false;
            }    
            return TechOne2OneMap::doMapping(block, inst);
        }    
        else
        {
            if(cell) {
                return TechOne2OneMap::doMapping(block, inst);
            }    
            cell = this->getNegEdgeTechCell(SCell::FF, 2, CLOCK);
            if(!cell) {
                return false;
            }    
            //need to insert an inverter
            const SCell *invcell = SCellMgr::instance()->getCell(SCell::NOT, 1);
            SInst *invinst = CreateInstance(block, invcell, "NOT");
            SPin *clk = inst->getSpecialPin("CLOCK");
            invinst->getSpecialPin("DATAIN")->setActual(clk->getActual());
            SInst *newinst = CreateInstance(block, cell, this->getNameHint());
            this->connectUsingPortMap(inst, newinst);
            newinst->getSpecialPin("CLOCK")->removeActual();
            block->connect(invinst->getSpecialPin("DATAOUT"), newinst->getSpecialPin("CLOCK"));
            block->deleteCellInst(inst);
            return true;
            
        }
        return false;
        #if 0 
        const  SCell *cell = this->getTechCell(inst);
        bool negedge = this->isNegEdgeClock(inst);

        if(!negedge || !cell) {
            return TechOne2OneMap::doMapping(block, inst);
        }    
        SInst *newinst = CreateInstance(block, cell, this->getNameHint());
        SInst *invinst = const_cast<SInst*>(this->getNegEdgeInverter(inst, "CLOCK"));
        this->connectUsingPortMap(inst, newinst);
        newinst->getSpecialPin("CLOCK")->setActual(
            invinst->getSpecialPin("DATAIN")->getActual());
        
        block->deleteCellInst(inst);
        block->deleteCellInst(invinst);
        return true;
        #endif
    }

    //----------------------------------------------------------------------
    TechROrSFFMapBase::TechROrSFFMapBase(SCell::CellType type) :
        TechFFMap(type) {
    }

    TechROrSFFMapBase::~TechROrSFFMapBase() {
    }
    const SCell*
    TechROrSFFMapBase::getTechCell(const SInst *inst) const
    {
        const bool negedgeclk = this->isNegEdgeClock(inst);
        const bool negedgectrl  = this->isNegEdgeCtrl(inst);
        vector<const SCell*> cells = SCellMgr::instance()->getTechCells(this->getType(), 3);
        string attrib = this->getCtrlAttrib();
        for(int i = 0; i < cells.size(); ++i)
        {
            if((negedgeclk == this->isNegEdge(cells[i], CLOCK)) &&
                (negedgectrl == this->isNegEdge(cells[i], attrib))) {
                return cells[i];
            }    
        }    
        return 0;
    }
    bool
    TechROrSFFMapBase::doMapping(SBlock *block, SInst* inst)
    {
        //Use cases:
        //1: posedge clk, posedge set/reset, cell available
        //2: posedge clk, posedge set/reset, cell not available
        //3: posedge clk, negedge set/reset, cell available
        //4: posedge clk, negedge set/reset, cell not available
        //5: negedge clk, posedge set/reset, cell available
        //6: negedge clk, posedge set/reset, cell not available
        //7: negedge clk, negedge set/reset, cell available
        //8: negedge clk, negedge set/reset, cell not available
        const  SCell *cell = this->getTechCell(inst);
        const bool negedgeclk = this->isNegEdgeClock(inst);
        const bool negedgectrl = this->isNegEdgeCtrl(inst);


        if(cell)
        {
            if((!negedgeclk && !negedgectrl)) {
                return TechOne2OneMap::doMapping(block, inst);
            }    
            SInst *newinst = CreateInstance(block, cell, this->getNameHint());
            this->connectUsingPortMap(inst, newinst);
            if(negedgeclk) {
                if(!this->removeInverter(block, newinst, CLOCK)) {
                    return false;
                }    
            }
            if(negedgectrl) {
                if(!this->removeInverter(block, newinst, this->getCtrlAttrib())) {
                    return false;
                }    
            }
            block->deleteCellInst(inst);
            return true;
        }

        return this->doMappingNoMatch(block, inst);
    }    

    bool
    TechROrSFFMapBase::doMappingNoMatch(SBlock *block, SInst* inst)
    {
        const bool negedgeclk = this->isNegEdgeClock(inst);
        const bool negedgectrl = this->isNegEdgeCtrl(inst);
        //Cell does not exist, let us try to get a cell with same
        //clock poplarity as it is there in the input design
        string attrib = this->getCtrlAttrib();

        const SCell *cell = negedgeclk ? this->getNegEdgeTechCell(
            this->getType(), 3, CLOCK) :
            this->getPosEdgeTechCell(this->getType(), 3, CLOCK);
        if(cell)
        {
            SInst *newinst = CreateInstance(block, cell, this->getNameHint());
            this->connectUsingPortMap(inst, newinst);
            if(negedgeclk)
            {
                if(!this->removeInverter(block, newinst, CLOCK)) {
                    return false;
                }    
            }    
            if(!negedgectrl) {
                //Surely cell has posedge reset/set, otherwise it would
                //have resulted in complete match
                //if(!this->removeInverter(block, newinst, attrib)) {
                //    return false;
                //}    
            //}
            //else {
                //Cell surely has negedge set/reset
                if(!this->addInverter(block, newinst, attrib)) {
                    return false;
                }    
            }
            block->deleteCellInst(inst);
            return true;
        }


        cell = this->getNegEdgeTechCell(this->getType(), 3, CLOCK);
        if(!cell) {
            cell = this->getPosEdgeTechCell(this->getType(), 3, CLOCK);
        }
        if(!cell) {
            return false;
        }    

        SInst *newinst = CreateInstance(block, cell, this->getNameHint());
        this->connectUsingPortMap(inst, newinst);
        if(!negedgeclk)
        {
            if(!this->addInverter(block, newinst, CLOCK)) {
                return false;
            }    
        }    
        if(this->getNegEdgeTechCell(this->getType(), 3, attrib)/*this->isNegEdgeCtrl(newinst)*/) {
            if(negedgectrl) {
                if(!this->removeInverter(block, newinst, attrib)) {
                    return false;
                }    
            }    
            else {
                if(!this->addInverter(block, newinst, attrib)) {
                    return false;
                }    
            }
        }    
        block->deleteCellInst(inst);
        return true;
    }    

    //----------------------------------------------------------------------
    TechRFFMap::TechRFFMap() : TechROrSFFMapBase(SCell::RFF) {
    }

    TechRFFMap::~TechRFFMap() {
    }

    string 
    TechRFFMap::getNameHint() const {
        return "RFF";
    }

    TechOne2OneMap::PortMap
    TechRFFMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap portMap = TechFFMap::getPortMap(gencell, techcell);
        string genrst = gencell->getSpecialPort(RESET)->getIxName().getName();
        string techrst = techcell->getSpecialPort(RESET)->getIxName().getName();
        portMap.insert(make_pair(genrst, techrst));
        return portMap;
    } 

    bool
    TechRFFMap::isNegEdgeReset(const SInst *inst) const {
        return this->isNegEdge(inst, RESET);
    }

    bool
    TechRFFMap::isNegEdgeCtrl(const SInst *inst) const {
        return this->isNegEdgeReset(inst);
    }    

    string
    TechRFFMap::getCtrlAttrib() const {
        return RESET;
    }    


    //----------------------------------------------------------------------

    TechSFFMap::TechSFFMap() : TechROrSFFMapBase(SCell::SFF) {
    }

    TechSFFMap::~TechSFFMap() {
    }

    string 
    TechSFFMap::getNameHint() const {
        return "SFF";
    }

    TechOne2OneMap::PortMap
    TechSFFMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap portMap = TechFFMap::getPortMap(gencell, techcell);
        string genset = gencell->getSpecialPort("SET")->getIxName().getName();
        string techset = techcell->getSpecialPort("SET")->getIxName().getName();
        portMap.insert(make_pair(genset, techset));
        return portMap;
    } 

    bool
    TechSFFMap::isNegEdgeSet(const SInst *inst) const {
        return this->isNegEdge(inst, SET);
    }

    bool
    TechSFFMap::isNegEdgeCtrl(const SInst *inst) const {
        return this->isNegEdgeSet(inst);
    }

    string
    TechSFFMap::getCtrlAttrib() const {
        return SET;
    }    

    //----------------------------------------------------------------------

    TechRSFFMapBase::TechRSFFMapBase(SCell::CellType type) :
        TechFFMap(type) {
    }

    TechRSFFMapBase::~TechRSFFMapBase() {
    }

    bool
    TechRSFFMapBase::doMapping(SBlock *block, SInst* inst)
    {
        //Use cases:
        //1:    posedge clk, posedge set, posedge reset, cell available
        //2:    posedge clk, posedge set, posedge reset, cell not available
        //3:    posedge clk, posedge set, negedge reset, cell available
        //4:    posedge clk, posedge set, negedge reset, cell not available
        //5:    posedge clk, negedge set, posedge reset, cell available
        //6:    posedge clk, negedge set, posedge reset, cell not available
        //7:    posedge clk, negedge set, negedge reset, cell available
        //8:    posedge clk, negedge set, negedge reset, cell not available
        //9:    negedge clk, posedge set, posedge reset, cell available
        //10:    negedge clk, posedge set, posedge reset, cell not available
        //11:    negedge clk, posedge set, negedge reset, cell available
        //12:    negedge clk, posedge set, negedge reset, cell not available
        //13:    negedge clk, negedge set, posedge reset, cell available
        //14:    negedge clk, negedge set, posedge reset, cell not available
        //15:    negedge clk, negedge set, negedge reset, cell available
        //16:    negedge clk, negedge set, negedge reset, cell not available

        const  SCell *cell = this->getTechCell(inst);
        const bool negedgeclk = this->isNegEdgeClock(inst);
        const bool negedgeset = this->isNegEdge(inst, SET);
        const bool negedgereset = this->isNegEdge(inst, RESET);


        if(cell)
        {
            if((!negedgeclk && !negedgeset && !negedgereset)) {
                return TechOne2OneMap::doMapping(block, inst);
            }    
            SInst *newinst = CreateInstance(block, cell, this->getNameHint());
            this->connectUsingPortMap(inst, newinst);
            if(negedgeclk) {
                if(!this->removeInverter(block, newinst, CLOCK)) {
                    return false;
                }    
            } 
            if(negedgeset) {
                if(!this->removeInverter(block, newinst, SET)) {
                    return false;
                }    
            }
            if(negedgereset) {
                if(!this->removeInverter(block, newinst, RESET)) {
                    return false;
                }    
            }
            block->deleteCellInst(inst);
            return true;
        }

        return this->doMappingNoMatch(block, inst);
    }    

    const SCell*
    TechRSFFMapBase::getTechCell(const SInst *inst) const
    {
        const bool negedgeclk = this->isNegEdgeClock(inst);
        const bool negedgeset  = this->isNegEdge(inst, SET);
        const bool negedgereset  = this->isNegEdge(inst, RESET);

        vector<const SCell*> cells = SCellMgr::instance()->getTechCells(this->getType(), 4);
        for(int i = 0; i < cells.size(); ++i)
        {
            if((negedgeclk == this->isNegEdge(cells[i], CLOCK)) &&
                (negedgeset == this->isNegEdge(cells[i], SET)) &&
                (negedgereset == this->isNegEdge(cells[i], RESET))) {
                return cells[i];
            }
        }
        return 0;
    }

    bool
    TechRSFFMapBase::doMappingNoMatch(SBlock *block, SInst* inst)
    {
        const bool negedgeclk = this->isNegEdgeClock(inst);
        const bool negedgeset = this->isNegEdge(inst, SET);
        const bool negedgereset = this->isNegEdge(inst, RESET);
        //Cell does not exist, let us try to get a cell with same
        //clock poplarity as it is there in the input design

        const SCell *cell = negedgeclk ? this->getNegEdgeTechCell(
            this->getType(), 4, CLOCK) :
            this->getPosEdgeTechCell(this->getType(), 4, CLOCK);
        if(!cell) {
            cell = this->getPosEdgeTechCell(this->getType(), 4, CLOCK);
        }
        if(!cell) {
            cell = this->getNegEdgeTechCell(this->getType(), 4, CLOCK);
        }
        if(!cell) {
            return false;
        }    

        SInst *newinst = CreateInstance(block, cell, this->getNameHint());
        this->connectUsingPortMap(inst, newinst);

        if(!this->addRemoveInverter(block, cell, newinst, negedgeclk, CLOCK)) {
            return false;
        }    
        if(!this->addRemoveInverter(block, cell, newinst, negedgeset, SET)) {
            return false;
        }    
        if(!this->addRemoveInverter(block, cell, newinst, negedgereset, RESET)) {
            return false;
        }    
        block->deleteCellInst(inst);
        return true;
    }

    bool
    TechRSFFMapBase::addRemoveInverter(SBlock *block, const SCell *cell, 
        SInst *inst, bool negedge, const string& attrib) 
    {
        if(negedge) {
            if(this->isNegEdge(cell, attrib)) {
                return this->removeInverter(block, inst, attrib); 
            }    
        }
        else {
            if(this->isNegEdge(cell, attrib)) {
                return this->addInverter(block, inst, attrib);
            }    
        }
        //no action => true
        return true;
    }

    //----------------------------------------------------------------------
    TechRSFFMap::TechRSFFMap() : TechRSFFMapBase(SCell::RSFF) {
    }

    TechRSFFMap::~TechRSFFMap() {
    }

    string 
    TechRSFFMap::getNameHint() const {
        return "RSFF";
    }

    TechOne2OneMap::PortMap
    TechRSFFMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap portMap = TechFFMap::getPortMap(gencell, techcell);
        string genset = gencell->getSpecialPort("SET")->getIxName().getName();
        string techset = techcell->getSpecialPort("SET")->getIxName().getName();
        string genrst = gencell->getSpecialPort("RESET")->getIxName().getName();
        string techrst = techcell->getSpecialPort("RESET")->getIxName().getName();
        portMap.insert(make_pair(genset, techset));
        portMap.insert(make_pair(genrst, techrst));
        return portMap;
    } 
    //----------------------------------------------------------------------

    TechSRFFMap::TechSRFFMap() : TechRSFFMapBase(SCell::SRFF) {
    }

    TechSRFFMap::~TechSRFFMap() {
    }

    string 
    TechSRFFMap::getNameHint() const {
        return "SRFF";
    }

    TechOne2OneMap::PortMap
    TechSRFFMap::getPortMap(const SCell *gencell, const SCell *techcell) const
    {
        PortMap portMap = TechFFMap::getPortMap(gencell, techcell);
        string genset = gencell->getSpecialPort("SET")->getIxName().getName();
        string techset = techcell->getSpecialPort("SET")->getIxName().getName();
        string genrst = gencell->getSpecialPort("RESET")->getIxName().getName();
        string techrst = techcell->getSpecialPort("RESET")->getIxName().getName();
        portMap.insert(make_pair(genset, techset));
        portMap.insert(make_pair(genrst, techrst));
        return portMap;
    } 
}
