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

#include "technandmap.h"
#include "techmapmgr.h"

#include "sblock.h"
#include "sinst.h"

#include "sutils2.h"
using namespace stview;

using std::list;

namespace TechnologyMapping
{
    TechNandMap::TechNandMap(bool merge) : 
        TechNXNXMap(stview::SCell::NAND, merge) {
    }

    TechNandMap::~TechNandMap() {
    }

    std::string
    TechNandMap::getNameHint() const {
        return "NAND";
    }    

    bool
    TechNandMap::doGeneralMapping(SBlock *block, SInst *inst)
    {
        if(this->doAndNotMap(block, inst)) {
            return true;
        }

        if(this->doOrNotMap(block, inst)) {
            return true;
        }
        return this->doNandVccMap(block, inst);
    }

    bool
    TechNandMap::doAndNotMap(SBlock *block, SInst *inst)
    {
        if(!TechMapMgr::instance()->cellExists(SCell::NOT, 1)) {
            return false;
        }    
        const int insize = TechMapMgr::instance()->getInputSize(inst);
        if(!TechMapMgr::instance()->cellExists(SCell::AND, insize)) {
            return false;
        }    

        SInst *invinst = CreateInstance(block, SCell::NOT, "NOT", 1, false);
        SInst *andinst = CreateInstance(block, SCell::AND, "AND", insize, false);

        list<const SPin*> oldinpins = inst->getSpecialPins(DATAIN);
        list<const SPin*> newinpins = andinst->getSpecialPins(DATAIN);
        assert(oldinpins.size() == newinpins.size());

        list<const SPin*>::iterator oldinpin = oldinpins.begin();
        list<const SPin*>::iterator newinpin = newinpins.begin();

        for(; (oldinpin != oldinpins.end()) && (newinpin != newinpins.end());
            ++oldinpin, ++newinpin) 
        {
            SPin *pin = const_cast<SPin*>(*newinpin);
            SPN *act = const_cast<SPin*>((*oldinpin))->getActual();
            pin->setActual(act);
        }

        block->connect(andinst->getSpecialPin(DATAOUT), invinst->getSpecialPin(DATAIN));
        block->connect(invinst->getSpecialPin(DATAOUT), inst->getSpecialPin(DATAOUT));
        block->deleteCellInst(inst);
        return true;
            
    }

    bool
    TechNandMap::doOrNotMap(SBlock *block, SInst *inst)
    {
        if(!TechMapMgr::instance()->cellExists(SCell::NOT, 1)) {
            return false;
        }    
        const int insize = TechMapMgr::instance()->getInputSize(inst);
        if(TechMapMgr::instance()->cellExists(SCell::OR, insize)) 
        {
            //mapping with OR of the same input size and NAND

            SInst *orinst = CreateInstance(block, SCell::OR, "OR", insize, false);

            list<const SPin*> oldinpins = inst->getSpecialPins(DATAIN);
            list<const SPin*> newinpins = orinst->getSpecialPins(DATAIN);
            assert(oldinpins.size() == newinpins.size());

            list<const SPin*>::iterator oldinpin = oldinpins.begin();
            list<const SPin*>::iterator newinpin = newinpins.begin();

            for(; (oldinpin != oldinpins.end()) && (newinpin != newinpins.end());
                ++oldinpin, ++newinpin) 
            {
                SPin *pin = const_cast<SPin*>(*newinpin);
                SPN *act = const_cast<SPin*>((*oldinpin))->getActual();
                SInst *invinst = CreateInstance(
                    block, SCell::NOT, "NOT", 1, false);
                SPin *invin = invinst->getSpecialPin(DATAIN);
                invin->setActual(act);
                block->connect(invinst->getSpecialPin(DATAOUT), pin);
            }
            block->connect(orinst->getSpecialPin(DATAOUT), inst->getSpecialPin(DATAOUT));
            block->deleteCellInst(inst);
            return true;
        }    
        //Trying with OR with input size 2
        if(!TechMapMgr::instance()->cellExists(SCell::OR, 2)) {
            return false;
        }    

        SPin *lastout = 0;
        list<const SPin*> oldinpins = inst->getSpecialPins(DATAIN);
        list<const SPin*>::iterator oldinpin = oldinpins.begin();

        while(oldinpin != oldinpins.end())    
        {
            SInst *orinst = CreateInstance(block, SCell::OR, "OR", 2, false);
            if(!lastout) {
                lastout = const_cast<SPin*>(*oldinpin++);
            }    
            list<const SPin*> newinpins = orinst->getSpecialPins(DATAIN);
            block->connect(const_cast<SPin*>(*oldinpin++), 
                const_cast<SPin*>(newinpins.front()));
            block->connect(lastout, const_cast<SPin*>(newinpins.back()));    
            lastout = orinst->getSpecialPin(DATAOUT);
        }
        return true;
    }    
    bool
    TechNandMap::doNandVccMap(SBlock *block, SInst *inst)
    {
        if(!TechMapMgr::instance()->cellExists(SCell::VCC, 1)) {
            return false;
        }    
        const int insize = TechMapMgr::instance()->getInputSize(inst);
        int newsize = insize + 1;
        if(!TechMapMgr::instance()->cellExists(SCell::NAND, newsize)) {
            ++newsize;
        }
        //max upto 2 more inputs
        if(!TechMapMgr::instance()->cellExists(SCell::NAND, newsize)) {
            return false;
        }    
        SInst *nandinst = CreateInstance(block, SCell::NAND, "NAND", newsize, false);

        list<const SPin*> oldinpins = inst->getSpecialPins(DATAIN);
        list<const SPin*> newinpins = nandinst->getSpecialPins(DATAIN);

        list<const SPin*>::iterator oldinpin = oldinpins.begin();
        list<const SPin*>::iterator newinpin = newinpins.begin();
        for(; (oldinpin != oldinpins.end()) && (newinpin != newinpins.end());
                ++oldinpin, ++newinpin) {
            block->connect(const_cast<SPin*>(*oldinpin), 
                const_cast<SPin*>(*newinpin));
        }        
        SInst *vccinst = CreateInstance(block, SCell::VCC, "VCC", 0, false);
        block->connect(vccinst->getSpecialPin(DATAOUT), 
            const_cast<SPin*>(*newinpin++));
        block->connect(vccinst->getSpecialPin(DATAOUT), 
            const_cast<SPin*>(*newinpin++));
        block->connect(nandinst->getSpecialPin(DATAOUT), inst->getSpecialPin(DATAOUT));
        block->deleteCellInst(inst);
        return true;
    }    
}
