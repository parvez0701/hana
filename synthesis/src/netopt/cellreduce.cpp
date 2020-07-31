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

#include <set>

#include "cellreduce.h"

#include "sutils2.h"
#include "sinst.h"
#include "sblock.h"
#include "spin.h"

using std::list;
using std::set;
using std::string;
using std::pair;
using std::make_pair;

using stview::SInst;
using stview::SBlock;
using stview::SPin;
using stview::SPN;
using stview::SCell;
using stview::SIxName;
namespace opt
{
    CellReduce::CellReduce() {
    }

    CellReduce::~CellReduce() {
    }

    bool
    CellReduce::operator()(SInst *inst)
    {
        if(!condition(inst)) { 
            return false;
        }
        this->addGateOfInterest(inst);
        return true;
    }

    bool
    CellReduce::optimize(SBlock *block)
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
            if(reduce(block, inst)) 
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
    CellReduce::condition(const SInst *inst) const
    {
        SCell::CellType type = inst->getCellType();

        if((type != SCell::AND) && (type != SCell::OR) &&
            (type != SCell::NAND) && (type != SCell::NOR) &&
            (type != SCell::XOR) && (type != SCell::XNOR)) {
            return false;
        }

        set<const SPN*> inacts;
        
        const list<SPin*>& inpins = inst->getInPins();
        for(list<SPin*>::const_iterator pin = inpins.begin();
            pin != inpins.end(); ++pin)
        {
            const SPN *act = (*pin)->getActual();
            if(!act) {
                continue;
            }

            if(inacts.find(act) != inacts.end()) {
                return true;
            }
            inacts.insert(act);
        }
        return false;
    }    

    bool
    CellReduce::reduce(SBlock *block, SInst *inst)
    {
        set<const SPN*> inacts;
        const SPN *act = 0;
        
        const list<SPin*>& inpins = inst->getInPins();
        for(list<SPin*>::const_iterator pin = inpins.begin();
            pin != inpins.end(); ++pin)
        {
            act = (*pin)->getActual();
            if(!act) {
                continue;
            }

            if(inacts.find(act) != inacts.end()) {
                break;
            }
            inacts.insert(act);
        }
        assert(act);
        const SPN *tact = act;
        list<const SPin*> pins;
        for(list<SPin*>::const_iterator pin = inpins.begin();
            pin != inpins.end(); ++pin)
        {
            const SPN *tmp = (*pin)->getActual();
            if(tact && (tmp == tact)) 
            {
                //skip the first occurence
                tact = 0;
                continue;
            }    
            pins.push_back(*pin);
        }    
        if(pins.size() == 1)
        {
            const list<SPin*>& outpins = inst->getOutPins();
            list<SPin*>::const_iterator pin = outpins.begin(); 
            this->reduce(block, inst, act, (*pin)->getActual());
        }
        else {
            this->reduce(block, inst, pins);
        }    
        return true;
    }

    void
    CellReduce::reduce(SBlock *block, SInst *oldinst, 
        const list<const SPin*>& oldpins)
    {
        pair<string, SCell::CellType> nameAndType = this->getNameAndType(oldinst);
        SInst *newinst = CreateInstance(block, nameAndType.second, 
            nameAndType.first, oldpins.size());
        

        int count = 0;
        for(list<const SPin*>::const_iterator pin = oldpins.begin(); 
            pin != oldpins.end(); ++pin)
        {
            SIxName name((*pin)->getIxName().getName(), count++);

            SPin *newpin  = newinst->getPin(name);
            newpin->setActual((*pin)->getActual());
        }    

        const list<SPin*>& outpins = oldinst->getOutPins();
        for(list<SPin*>::const_iterator pin = outpins.begin(); 
            pin != outpins.end(); ++pin)
        {
            SPin *newpin  = newinst->getPin((*pin)->getName());
            newpin->setActual((*pin)->getActual());
        }    

        const list<SPin*>& iopins = oldinst->getInOutPins();
        for(list<SPin*>::const_iterator pin = iopins.begin(); 
            pin != iopins.end(); ++pin)
        {
            SPin *newpin  = newinst->getPin((*pin)->getName());
            newpin->setActual((*pin)->getActual());
        }    

        block->deleteCellInst(oldinst);
    }

    void
    CellReduce::reduce(SBlock *block, SInst *oldinst, const SPN *in, const SPN *out)
    {
        SInst *newinst = 0;
        switch(oldinst->getCellType())
        {
            case SCell::AND:
            case SCell::OR:
                newinst = CreateInstance(block, SCell::BUF, "BUF", 1);
                newinst->getPin("in")->setActual(in);
                newinst->getPin("out")->setActual(out);
                break;
            case SCell::NAND:
            case SCell::NOR:
                newinst = CreateInstance(block, SCell::NOT, "NOT", 1);
                newinst->getPin("in")->setActual(in);
                newinst->getPin("out")->setActual(out);
                break;
            case SCell::XOR:    
                newinst = CreateInstance(block, SCell::GND, "GND", 0);
                newinst->getPin("out")->setActual(out);
                break;
            case SCell::XNOR:    
                newinst = CreateInstance(block, SCell::VCC, "VCC", 0);
                newinst->getPin("out")->setActual(out);
                break;
            default:
                return;
        }
        block->deleteCellInst(oldinst);
    }

    pair<string, SCell::CellType>
    CellReduce::getNameAndType(const SInst *inst) const
    {
        switch (inst->getCellType())
        {
            case SCell::NOT:
                return make_pair("NOT", SCell::NOT);
            case SCell::AND:
                return make_pair("AND", SCell::AND);
            case SCell::NAND:
                return make_pair("NAND", SCell::NAND);
            case SCell::OR:
                return make_pair("OR", SCell::OR);
            case SCell::NOR:
                return make_pair("NOR", SCell::NOR);
            case SCell::XOR:
                return make_pair("XOR", SCell::XOR);
            case SCell::XNOR:
                return make_pair("XNOR", SCell::XNOR);
            default:
                return make_pair("", SCell::CELLTYPE_BEGIN);
        }        
        return make_pair("", SCell::CELLTYPE_BEGIN);

    }
        
}


