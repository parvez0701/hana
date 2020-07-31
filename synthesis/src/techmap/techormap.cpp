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

#include "techormap.h"
#include "techmapmgr.h"
#include "sblock.h"
#include "sinst.h"
#include "spin.h"
#include "snet.h"
#include "sutils2.h"

using std::list;
using std::string;
using namespace stview;
namespace TechnologyMapping
{
    
    bool
    TechOrMap::generalCondition(const SInst *inst) const
    {
        if(inst->getCellType() != SCell::OR) {
            return false;
        }

        if(!inst->isGeneric()) {
            return false;
        }
        return true;
    }

    bool
    TechOrMap::doGeneralMapping(SBlock *block, SInst *inst)
    {
        //Only two possibilities are explored, beyond this
        //it would be too expensive in terms of area
        if(this->doDemergedMapping(block, inst)) {
            return true;
        }
        return this->doDirectNorNotMapping(block, inst);
    }

    bool
    TechOrMap::doDemergedMapping(SBlock *block, SInst *inst)
    {
        const int insize = TechMapMgr::instance()->getInputSize(inst); 
        if(insize == 2) {
            return false;
        }    
        const SCell *cell3in = TechMapMgr::instance()->getCell(SCell::OR, 3);
        const SCell *cell2in = TechMapMgr::instance()->getCell(SCell::OR, 2);
        const SCell *cellgnd = TechMapMgr::instance()->getCell(SCell::GND, 0);
        list<const SPN*> acts = inst->getSpecialActuals("DATAIN");
        list<const SPN*>::iterator act = acts.begin();
        list<const SPN*>::iterator actEnd = acts.end();
        if(insize == 4)
        {
            //If exists, should have been mapped earlier
            assert(!TechMapMgr::instance()->cellExists(SCell::OR, 4));
            assert(acts.size() == 4);
            if(cell2in && cell3in)
            {
                SInst *inst3in = this->createInstance(block, 3, act, actEnd );    
                SInst *inst2in = this->createInstance(block, 2, act, actEnd );
                this->connectInstances(block, inst3in, inst2in);
                this->setOutActual(inst2in, inst);
                block->deleteCellInst(inst);
                return true;
            }
            else if(cell3in && cellgnd)
            {
                SInst *inst3in1 = this->createInstance(block, 3, act, actEnd );    
                SInst *inst3in2 = this->createInstance(block, 3, act, actEnd );    
                this->connectInstances(block, inst3in1, inst3in2);
                SInst *gndinst = CreateInstance(block, SCell::GND, "GND", 0, false);
                this->connectInstances(block, gndinst, inst3in2);
                this->setOutActual(inst3in2, inst);
                block->deleteCellInst(inst);
                return true;
            }
            else if(cell2in)
            {
                SInst *inst2in1 = this->createInstance(block, 2, act, actEnd );    
                SInst *inst2in2 = this->createInstance(block, 2, act, actEnd );    
                SInst *inst2in3 = CreateInstance(block, SCell::OR, 
                    this->getNameHint(), 2, false);
                this->connectInstances(block, inst2in1, inst2in3);    
                this->connectInstances(block, inst2in2, inst2in3);    
                this->setOutActual(inst2in3, inst);
                block->deleteCellInst(inst);
                return true;
            }
        }
        else if(insize == 3)
        {
            assert(!TechMapMgr::instance()->cellExists(SCell::OR, 3));
            assert(acts.size() == 3);
            if(cell2in)
            {
                SInst *inst2in1 = this->createInstance(block, 2, act, actEnd );    
                SInst *inst2in2 = this->createInstance(block, 2, act, actEnd );    
                this->connectInstances(block, inst2in1, inst2in2);
                this->setOutActual(inst2in2, inst);
                block->deleteCellInst(inst);
                return true;
            }    
        }
        return false;
    }


    bool
    TechOrMap::doDirectNorNotMapping(SBlock *block, SInst *inst)
    {
        const int insize = TechMapMgr::instance()->getInputSize(inst); 
        if(!TechMapMgr::instance()->cellExists(SCell::NOR, insize)) {
            return false;
        }    
        if(!TechMapMgr::instance()->cellExists(SCell::NOT, 1)) {
            return false;
        }    
        SInst *norinst = CreateInstance(block, SCell::NOR, "NOR", insize, false);
        SInst *notinst = CreateInstance(block, SCell::NOT, "NOT", 1, false);
        list<const SPN*> acts = inst->getSpecialActuals("DATAIN");
        list<const SPin*> inpins = norinst->getSpecialPins("DATAIN");
        assert(acts.size() == inpins.size());
        list<const SPN*>::iterator a = acts.begin();

        for(list<const SPin*>::iterator p = inpins.begin(); 
            (a != acts.end()) && (p != inpins.end()); ++a, ++p)
        {
            SPN *act = const_cast<SPN*>(*a);
            SPin *pin = const_cast<SPin*>(*p);
            pin->setActual(act);
        }
        this->connectInstances(block, norinst, notinst);
        this->setOutActual(inst, notinst);
        block->deleteCellInst(inst);
        return true;
    }
    
}
