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


#include <map>
#include "invabs.h"

#include "sutils2.h"
#include "sinst.h"
#include "sblock.h"
#include "spin.h"

using std::list;
using std::string;
using std::pair;
using std::make_pair;

using stview::SInst;
using stview::SBlock;
using stview::SPin;
using stview::SPN;
using stview::SCell;
using stview::Driver;

namespace opt
{
    InvAbs::InvAbs() {
    }

    InvAbs::~InvAbs() {
    }

    bool
    InvAbs::operator()(SInst *inst)
    {
        if(!condition(inst)) { 
            return false;
        }
        this->addGateOfInterest(inst);
        return true;
    }

    bool
    InvAbs::optimize(SBlock *block)
    {
        list<string> rmlist;
        bool status = false;
        for(string name = this->getNextGateName(); !name.empty();
            name = this->getNextGateName())
        {
            SInst *inst = const_cast<SInst*>(block->getInst(name));
            if(!inst) {
                //deleted by some other processing
                continue;
            }
            if(!condition(inst)) {
                continue;
            }
            if(absorbInverter(block, inst)) 
            {
                status = true;
                rmlist.push_back(name);

            }
        }
        for(list<string>::const_iterator iter = rmlist.begin();
            iter != rmlist.end(); ++iter) {
            this->removeGateOfInterest(*iter);
        }
        return status;
    }    
 
    bool
    InvAbs::condition(const SInst *inst) const
    {
        if(inst->getCellType() != SCell::NOT) {
            return false;
        }
        SPN *in = const_cast<SPN*>(inst->getPin("in")->getActual());
        if(!in) {
            return false;
        }
        in->reset();
        if((in->dsize() != 1) || (in->ssize() != 1)) {
            return false;
        }    
        Driver d = in->getDriver();
        const SInst *dinst = (*d)->getCellInst();
        if(!dinst) {
            return false;
        }
        return this->getNameAndType(dinst).second != SCell::CELLTYPE_BEGIN;
    }

    pair<string, SCell::CellType>
    InvAbs::getNameAndType(const SInst *inst) const
    {
        switch (inst->getCellType())
        {
            case SCell::NOT:
                return make_pair("BUF", SCell::BUF);
            case SCell::AND:
                return make_pair("NAND", SCell::NAND);
            case SCell::NAND:
                return make_pair("AND", SCell::AND);
            case SCell::OR:
                return make_pair("NOR", SCell::NOR);
            case SCell::NOR:
                return make_pair("OR", SCell::OR);
            case SCell::XOR:
                return make_pair("XNOR", SCell::XNOR);
            case SCell::XNOR:
                return make_pair("XOR", SCell::XOR);
            default:
                return make_pair("", SCell::CELLTYPE_BEGIN);
        }        
        return make_pair("", SCell::CELLTYPE_BEGIN);

    }

    bool
    InvAbs::absorbInverter(SBlock *block, SInst *inst)
    {
        const SPN *out = inst->getPin("out")->getActual();
        SPN *in = const_cast<SPN*>(inst->getPin("in")->getActual());

        in->reset();
        Driver d = in->getDriver();

        SInst *otherInst = const_cast<SInst*>((*d)->getCellInst());
        assert(otherInst);

        pair<string, SCell::CellType> nameAndType = 
            this->getNameAndType(otherInst);
        assert(nameAndType.second != SCell::CELLTYPE_BEGIN);

        const std::list<SPin*>& inpins = otherInst->getInPins();
        SInst *newInst = CreateInstance(block, nameAndType.second, 
            nameAndType.first, inpins.size());
        
        for(list<SPin*>::const_iterator pin = inpins.begin();
            pin != inpins.end(); ++pin) {
            const_cast<SPin*>(newInst->getPin((*pin)->getName()))->
                setActual((*pin)->getActual());
        }        

        const_cast<SPin*>(newInst->getPin("out"))->setActual(out);

        block->deleteCellInst(inst);
        block->deleteCellInst(otherInst);

        return true;

    }
}
