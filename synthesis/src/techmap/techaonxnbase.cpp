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

#include "techaonxnbase.h"
#include "techmapmgr.h"

#include "sinst.h"
#include "snet.h"
#include "sblock.h"
#include "sutils2.h"

using std::vector;
using std::list;
using std::set;

using namespace stview;
namespace TechnologyMapping
{
    TechAONXNBase::TechAONXNBase(SCell::CellType t) : type(t) {
    }

    TechAONXNBase::~TechAONXNBase() {
    }

    SCell::CellType
    TechAONXNBase::getType() const {
        return type;
    }    

    bool
    TechAONXNBase::condition(const SInst *inst) const
    {
        if(inst->getCellType() != type) {
            return false;
        }
        int input = inst->getInPins().size();
        if(inst->isGeneric())
        {
            if(TechMapMgr::instance()->cellExists(type, input)) {
                return true;
            }
        }    

        //See if we have opportunity to merge this cell with others of
        //same type
        return this->condition(inst, input);
    }

    bool
    TechAONXNBase::condition(const SInst *inst, int input) const
    {
        if(inst->getCellType() != type) {
            return false;
        }    
        if(inst->getCell()->isGeneric() && 
            TechMapMgr::instance()->cellExists(type, input)) {
            return true;
        }

        const list<SPin*>& pins = inst->getInPins();

        for(list<SPin*>::const_iterator pin = pins.begin(); 
            pin != pins.end(); ++pin)
        {
            SPN *act = (*pin)->getActual();
            if(!act) {
                continue;
            }    
            act->reset();
            if(act->dsize() > 1) {
                continue;
            }    
            Driver d = act->getDriver();
            SPin *newpin = dynamic_cast<SPin*>(*d);
            //assert(newpin);
            if(!newpin) {
                continue;
            }    

            const SInst *newinst = newpin->getCellInst();
            if(!newinst || (newinst == inst)) {
                continue;
            }    
            if(this->condition(newinst, input+1)) {
                return true;
            }    
        }
        //Notice that we are missing the opportunity to merge
        //gates at the same level. TBD

        return false;
    }

    bool
    TechAONXNBase::doMapping(SBlock *block, SInst *inst)
    {
        vector<const SInst*> insts;
        this->getInsts2BeMerged(inst, insts);
        if(insts.empty()) {
            return false;
        }    
        this->merge(block, insts);
        return true;
    }

    void
    TechAONXNBase::getInsts2BeMerged(const SInst *inst, 
        vector<const SInst*>& insts) const
    {
        if(inst->getCellType() != type) {
            return;
        }    
        insts.push_back(inst);

        const list<SPin*>& pins = inst->getInPins();

        for(list<SPin*>::const_iterator pin = pins.begin(); 
            pin != pins.end(); ++pin)
        {
            SPN *act = (*pin)->getActual();
            if(!act) {
                continue;
            }    
            act->reset();
            if(act->dsize() > 1) {
                continue;
            }    
            Driver d = act->getDriver();
            SPin *newpin = dynamic_cast<SPin*>(*d);
            if(!newpin) {
                continue;
            }    
            //assert(newpin);

            const SInst *newinst = newpin->getCellInst();
            if(!newinst || (newinst == inst)) {
                continue;
            }    
            this->getInsts2BeMerged(newinst, insts);
        }
    }
    void
    TechAONXNBase::merge(SBlock* block, vector<const SInst*>& insts) const
    {
        int total = 0; 
        int size = 0;
        int count = 0;
        vector<const SInst*> insts2bmerged;

        for(; count < insts.size(); ++count)
        {
            total += insts[count]->getInPins().size() - ((count == 0) ? 0 :1);
            if(TechMapMgr::instance()->cellExists(type, total)) 
            {
                size = total;
                insts2bmerged.push_back(insts[count]);
            }    
        }

        //assert(!insts2bmerged.empty());
        if(insts2bmerged.empty()) {
            return;
        }    
        set<const SPN*> oldouts;
        vector<const SPN*> oldins;

        for(int i = 1; i < insts2bmerged.size(); ++i) 
        {
            const SPN *out = insts2bmerged[i]->getPin("out")->getActual();
            assert(out);
            oldouts.insert(out);
        }    
        const SPN *out = insts2bmerged[0]->getPin("out")->getActual();

        for(int i = 0; i < insts2bmerged.size(); ++i)
        {
            const list<SPin*> & inpins = insts2bmerged[i]->getInPins();

            for(list<SPin*>::const_iterator pin = inpins.begin();
                pin != inpins.end(); ++pin) 
            {
                const SPN *act = (*pin)->getActual();
                if(!act) {
                    continue;
                }
                if(oldouts.find(act) == oldouts.end()) {
                    oldins.push_back(act);
                }
            }    
            block->deleteCellInst(const_cast<SInst*>(insts2bmerged[i]));
        }

        SInst *newinst = CreateInstance(block, type, this->getNameHint(), size, false);
        newinst->getPin("out")->setActual(out);

        for(count = 0; count < oldins.size(); ++count) {
            newinst->getPin(SIxName("in", count))->setActual(oldins[count]);
        }
    }    
    SInst*
    TechAONXNBase::createInstance(SBlock *block, int insize,
        list<const SPN*>::iterator& start,
        list<const SPN*>::iterator& end)
    {
        SInst *inst = CreateInstance(block, type,
            this->getNameHint(), insize, false);    
        
        list<const SPin*> inpins = inst->getSpecialPins("DATAIN");
        
        for(list<const SPin*>::iterator p = inpins.begin();
            (p != inpins.end()) && (start != end); ++p, ++start ) {
            const_cast<SPin*>(*p)->setActual(*start);
        }
        return inst;
    }

    void
    TechAONXNBase::connectInstances(SBlock* block, SInst *inst1, SInst *inst2)
    {
        SPin *out = const_cast<SPin*>(inst1->getSpecialPin("DATAOUT"));
        SNet *net = CreateNet(block, "synth");
        out->setActual(net);
        list<const SPin*> pins = inst2->getSpecialPins("DATAIN");
        for(list<const SPin*>::iterator p = pins.begin(); 
            p != pins.end(); ++p)
        {
            SPin *pin = const_cast<SPin*>(*p);
            if(!pin->getActual()) 
            {
                pin->setActual(net);
                return;
            }
        }    
    }

    void
    TechAONXNBase::setOutActual(SInst *inst1, SInst *inst2)
    {
        const SPin* pin1 = inst1->getSpecialPin("DATAOUT");
        const SPin* pin2 = inst2->getSpecialPin("DATAOUT");
        const_cast<SPin*>(pin1)->setActual(pin2->getActual());
    }    

}
