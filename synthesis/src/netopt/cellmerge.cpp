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

#include "cellmerge.h"
#include "sutils2.h"
#include "sinst.h"
#include "sblock.h"
#include "spin.h"

using std::list;
using std::string;

using stview::SInst;
using stview::SBlock;
using stview::SPin;
using stview::SPN;
using stview::SCell;
using stview::Sink;
namespace opt
{
    CellMerge::CellMerge() : MAXSIZE(4) {
    }

    CellMerge::~CellMerge() {
    }

    bool
    CellMerge::operator()(SInst *inst)
    {
        if(!condition(inst)) { 
            return false;
        }
        this->addGateOfInterest(inst);
        return true;
    }

    bool
    CellMerge::optimize(SBlock *block)
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
            if(merge(block, inst)) 
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
    CellMerge::condition(const SInst *inst) const
    {
        SCell::CellType type = inst->getCellType();
        if((type != SCell::AND) && (type != SCell::OR)) {
            return false;
        }
        SPN *portOrNet = const_cast<SPN*>(inst->getPin("out")->getActual());
        if(!portOrNet) {
            return false;
        }
        if((portOrNet->dsize() != 1) || (portOrNet->ssize() != 1)) {
            return false;
        }
        Sink s = portOrNet->getSink();

        const SInst *otherInst = (*s)->getCellInst();
        if(!otherInst || (otherInst->getCellType() != type)) {
            return false;
        }
        const list<SPin*>& inpins1 = otherInst->getInPins();
        const list<SPin*>& inpins2 = inst->getInPins();
        if((inpins1.size()+inpins2.size()-1) > MAXSIZE) {
            //saturated
            return false;
        }
        return true;
    }

    bool
    CellMerge::merge(SBlock *block, SInst *inst)
    {
        SCell::CellType type = inst->getCellType();

        SPN *portOrNet = const_cast<SPN*>(inst->getPin("out")->getActual());
        Sink s = portOrNet->getSink();
        SInst *otherInst = const_cast<SInst*>((*s)->getCellInst());
        if(inst == otherInst) {
            return false;
        }    
        

        list<SPin*>& pins = const_cast<list<SPin*>&>(inst->getInPins());
        list<SPin*>& otherpins = const_cast<list<SPin*>&>(
            otherInst->getInPins());

        const int insize = pins.size() + otherpins.size();
        string hint((type == SCell::AND) ? "AND" : "OR");
        
        SInst* newinst = CreateInstance(block, type, hint, insize-1);
        list<SPin*>& newpins = const_cast<list<SPin*>&>(newinst->getInPins());
        list<SPin*>::iterator newpin = newpins.begin();
        bool once = true;
        for(list<SPin*>::const_iterator iter = otherpins.begin();
            iter != otherpins.end(); ++iter ) 
        {
            const SPN *spn = (*iter)->getActual();
            if((spn == portOrNet) && once) {
                once = false;
                continue;
            }    
            (*newpin)->setActual(spn);
            ++newpin;
        }    
        for(list<SPin*>::const_iterator iter = pins.begin();
            iter != pins.end(); ++iter, ++newpin) 
        {
            const SPN *spn = (*iter)->getActual();
            (*newpin)->setActual(spn);
        }    

        const_cast<SPin*>(newinst->getPin("out"))->setActual(
            otherInst->getPin("out")->getActual());
        block->deleteCellInst(inst);
        block->deleteCellInst(otherInst);
        return true;

    }

}
