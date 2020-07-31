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

#include "techandmap.h"
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
    TechAndMap::generalCondition(const SInst *inst) const
    {
        if(inst->getCellType() != SCell::AND) {
            return false;
        }

        if(!inst->isGeneric()) {
            return false;
        }
        return true;
    }

    bool
    TechAndMap::doGeneralMapping(SBlock *block, SInst *inst)
    {
        //Only two possibilities are explored, beyond this
        //it would be too expensive in terms of area
        if(this->doDemergedMapping(block, inst)) {
            return true;
        }
        return this->doDirectNandNotMapping(block, inst);
    }

    bool
    TechAndMap::doDemergedMapping(SBlock *block, SInst *inst)
    {
        const int insize = TechMapMgr::instance()->getInputSize(inst); 
        if(insize == 2) {
            return false;
        }    
        const SCell *cell3in = TechMapMgr::instance()->getCell(SCell::AND, 3);
        const SCell *cell2in = TechMapMgr::instance()->getCell(SCell::AND, 2);
        const SCell *cellvcc = TechMapMgr::instance()->getCell(SCell::VCC, 0);
        list<const SPN*> acts = inst->getSpecialActuals("DATAIN");
        list<const SPN*>::iterator act = acts.begin();
        list<const SPN*>::iterator actEnd = acts.end();
        if(insize == 4)
        {
            //If exists, should have been mapped earlier
            assert(!TechMapMgr::instance()->cellExists(SCell::AND, 4));
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
            else if(cell3in && cellvcc)
            {
                SInst *inst3in1 = this->createInstance(block, 3, act, actEnd );    
                SInst *inst3in2 = this->createInstance(block, 3, act, actEnd );    
                this->connectInstances(block, inst3in1, inst3in2);
                SInst *vccinst = CreateInstance(block, SCell::VCC, "VCC", 0, false);
                this->connectInstances(block, vccinst, inst3in2);
                this->setOutActual(inst3in2, inst);
                block->deleteCellInst(inst);
                return true;
            }
            else if(cell2in)
            {
                SInst *inst2in1 = this->createInstance(block, 2, act, actEnd );    
                SInst *inst2in2 = this->createInstance(block, 2, act, actEnd );    
                SInst *inst2in3 = CreateInstance(block, SCell::AND, 
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
            assert(!TechMapMgr::instance()->cellExists(SCell::AND, 3));
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
    TechAndMap::doNandNotMapping(SBlock *block, SInst *inst)
    {
        const int insize = TechMapMgr::instance()->getInputSize(inst); 
        const SCell *notcell = TechMapMgr::instance()->getCell(SCell::NOT, 1);
        if(!notcell) {
            return false;
        }    
        const SCell *nand4in = TechMapMgr::instance()->getCell(SCell::NAND, 4);
        const SCell *nand3in = TechMapMgr::instance()->getCell(SCell::NAND, 3);
        const SCell *nand2in = TechMapMgr::instance()->getCell(SCell::NAND, 2);
        const SCell *cellvcc = TechMapMgr::instance()->getCell(SCell::VCC, 0);
        if(((insize == 4) && nand4in) || 
            ((insize == 3) && nand3in) ||
            ((insize == 2) && nand2in)) {
            return this->doDirectNandNotMapping(block, inst);
        }    
#if 0
        if(insize == 4)
        {
            if(nand2in)
            {
                SInst *nand2inst1 = CreateInstance(block, SCell::NAND, "NAND", 2, false);
                SInst *nand2inst2 = CreateInstance(block, SCell::NAND, "NAND", 2, false);
                SInst *nand2inst3 = CreateInstance(block, SCell::NAND, "NAND", 2, false);

                SInst *notinst1 = CreateInstance(block, SCell::NOT, "NOT", 1, false);
                SInst *notinst2 = CreateInstance(block, SCell::NOT, "NOT", 1, false);
                SInst *notinst3 = CreateInstance(block, SCell::NOT, "NOT", 1, false);
                list<const SPN*> acts = inst->getSpecialActuals("DATAIN");
                list<const SPin*> inpins = nand2inst1->getSpecialPins("DATAIN");

                list<const SPN*>::iterator a = acts.begin();
                for(list<const SPin*>::iterator p = inpins.begin(); 
                    (p != inpins.end()) && (a != acts.end()); ++p, ++a){
                    const_cast<SPin*>(*p)->setActual(const_cast<SPN*>(*a));
                }    
                inpins = nand2inst2->getSpecialPins("DATAIN");
                for(list<const SPin*>::iterator p = inpins.begin(); 
                    (p != inpins.end()) && (a != acts.end()); ++p, ++a){
                    const_cast<SPin*>(*p)->setActual(const_cast<SPN*>(*a));
                }    

                this->connectInstances(block, nand2inst1, notinst1);
                this->connectInstances(block, nand2inst2, notinst2);
                SNet *net = CreateNet(block, "synth");
                const_cast<SPin*>(notinst1->getSpecialPin("DATAOUT"))->setActual(net);
                net = CreateNet(block, "synth");
                const_cast<SPin*>(notinst2->getSpecialPin("DATAOUT"))->setActual(net);
                this->connectInstances(block, notinst1, nand2inst3);
                this->connectInstances(block, notinst2, nand2inst3);
                net = CreateNet(block, "synth");
                const_cast<SPin*>(nand2inst3->getSpecialPin("DATAOUT"))->setActual(net);
                this->connectInstances(block, nand2inst3, notinst3);
                this->setOutActual(inst, notinst3);
            }
            else if(nand3in) {
                //TBD
            }
        }
#endif        
        return false;
    }

    bool
    TechAndMap::doDirectNandNotMapping(SBlock *block, SInst *inst)
    {
        const int insize = TechMapMgr::instance()->getInputSize(inst); 
        if(!TechMapMgr::instance()->cellExists(SCell::NAND, insize)) {
            return false;
        }    
        if(!TechMapMgr::instance()->cellExists(SCell::NOT, 1)) {
            return false;
        }    
        SInst *nandinst = CreateInstance(block, SCell::NAND, "NAND", insize, false);
        SInst *notinst = CreateInstance(block, SCell::NOT, "NOT", 1, false);
        list<const SPN*> acts = inst->getSpecialActuals("DATAIN");
        list<const SPin*> inpins = nandinst->getSpecialPins("DATAIN");
        assert(acts.size() == inpins.size());
        list<const SPN*>::iterator a = acts.begin();

        for(list<const SPin*>::iterator p = inpins.begin(); 
            (a != acts.end()) && (p != inpins.end()); ++a, ++p)
        {
            SPN *act = const_cast<SPN*>(*a);
            SPin *pin = const_cast<SPin*>(*p);
            pin->setActual(act);
        }
        this->connectInstances(block, nandinst, notinst);
        this->setOutActual(inst, notinst);
        block->deleteCellInst(inst);
        return true;
    }
    
}
