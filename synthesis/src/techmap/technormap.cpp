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

#include "technormap.h"
#include "techmapmgr.h"

#include "sblock.h"
#include "sinst.h"

#include "sutils2.h"
using namespace stview;

using std::list;
using std::string;

namespace TechnologyMapping
{
    TechNorMap::TechNorMap(bool merge) : 
        TechNXNXMap(stview::SCell::NOR, merge) {
    }

    TechNorMap::~TechNorMap() {
    }

    string
    TechNorMap::getNameHint() const {
        return "NOR";
    }

    bool
    TechNorMap::doGeneralMapping(SBlock *block, SInst *inst)
    {
        if(this->doOrNotMap(block, inst)) {
            return true;
        }

        if(this->doAndNotMap(block, inst)) {
            return true;
        }
        return this->doNorVccMap(block, inst);
    }

    bool
    TechNorMap::doOrNotMap(SBlock *block, SInst *inst)
    {
        if(!TechMapMgr::instance()->cellExists(SCell::NOT, 1)) {
            return false;
        }    
        const int insize = TechMapMgr::instance()->getInputSize(inst);
        if(!TechMapMgr::instance()->cellExists(SCell::OR, insize)) {
            return false;
        }    

        SInst *invinst = CreateInstance(block, SCell::NOT, "NOT", 1, false);
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
            pin->setActual(act);
        }

        block->connect(orinst->getSpecialPin(DATAOUT), invinst->getSpecialPin(DATAIN));
        block->connect(invinst->getSpecialPin(DATAOUT), inst->getSpecialPin(DATAOUT));
        block->deleteCellInst(inst);
        return true;
            
    }

    bool
    TechNorMap::doAndNotMap(SBlock *block, SInst *inst)
    {
        if(!TechMapMgr::instance()->cellExists(SCell::NOT, 1)) {
            return false;
        }    
        const int insize = TechMapMgr::instance()->getInputSize(inst);
        if(TechMapMgr::instance()->cellExists(SCell::AND, insize)) 
        {
            //mapping with AND of the same input size and NOR

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
                SInst *invinst = CreateInstance(
                    block, SCell::NOT, "NOT", 1, false);
                SPin *invin = invinst->getSpecialPin(DATAIN);
                invin->setActual(act);
                block->connect(invinst->getSpecialPin(DATAOUT), pin);
            }
            block->connect(andinst->getSpecialPin(DATAOUT), inst->getSpecialPin(DATAOUT));
            block->deleteCellInst(inst);
            return true;
        }    
        //Trying with AND with input size 2
        if(!TechMapMgr::instance()->cellExists(SCell::AND, 2)) {
            return false;
        }    

        SPin *lastout = 0;
        list<const SPin*> oldinpins = inst->getSpecialPins(DATAIN);
        list<const SPin*>::iterator oldinpin = oldinpins.begin();

        while(oldinpin != oldinpins.end())    
        {
            SInst *andinst = CreateInstance(block, SCell::AND, "AND", 2, false);
            if(!lastout) {
                lastout = const_cast<SPin*>(*oldinpin++);
            }    
            list<const SPin*> newinpins = andinst->getSpecialPins(DATAIN);
            block->connect(const_cast<SPin*>(*oldinpin++), 
                const_cast<SPin*>(newinpins.front()));
            block->connect(lastout, const_cast<SPin*>(newinpins.back()));    
            lastout = andinst->getSpecialPin(DATAOUT);
        }
        return true;
    }    

    bool
    TechNorMap::doNorVccMap(SBlock *block, SInst *inst)
    {
        if(!TechMapMgr::instance()->cellExists(SCell::VCC, 1)) {
            return false;
        }    
        const int insize = TechMapMgr::instance()->getInputSize(inst);
        int newsize = insize + 1;
        if(!TechMapMgr::instance()->cellExists(SCell::NOR, newsize)) {
            ++newsize;
        }
        //max upto 2 more inputs
        if(!TechMapMgr::instance()->cellExists(SCell::NOR, newsize)) {
            return false;
        }    
        SInst *norinst = CreateInstance(block, SCell::NOR, "NOR", newsize, false);

        list<const SPin*> oldinpins = inst->getSpecialPins(DATAIN);
        list<const SPin*> newinpins = norinst->getSpecialPins(DATAIN);

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
        block->connect(norinst->getSpecialPin(DATAOUT), inst->getSpecialPin(DATAOUT));
        block->deleteCellInst(inst);
        return true;
    }    



}
