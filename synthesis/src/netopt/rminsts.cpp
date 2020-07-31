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

#include "rminsts.h"

#include "sinst.h"
#include "sblock.h"
#include "spin.h"

using std::list;
using std::string;
using stview::SInst;
using stview::SBlock;
using stview::SPin;
using stview::SPN;
namespace opt
{
    RmInsts::RmInsts() {
    }

    RmInsts::~RmInsts() {
    }

    bool
    RmInsts::operator()(SInst *inst)
    {
        if(checkConnected(inst->getOutPins()) || 
            checkConnected(inst->getInOutPins())) {
            //atleast one output is connected with some logic, not
            //of our interest
            return false;
        }
        this->addGateOfInterest(inst);
        return true;
    }

    bool
    RmInsts::optimize(SBlock *block)
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
            if(checkConnected(inst->getOutPins()) || 
                checkConnected(inst->getInOutPins())) {
                //somehow got connected ?
                continue;
            }    
            status = true;
            rmlist.push_back(name);
            block->deleteCellInst(inst);
        }
        for(list<string>::const_iterator iter = rmlist.begin();
            iter != rmlist.end(); ++iter) {
            this->removeGateOfInterest(*iter);
        }    
        return status;
    }

    bool
    RmInsts::checkConnected(const list<SPin*>& pins) const
    {
        for(list<SPin*>::const_iterator pin = pins.begin(); 
            pin != pins.end(); ++pin)
        {
            if(SPN *netOrPort = (*pin)->getActual()) 
            {
                //ports sinks may be empty
                if(netOrPort->isPort()) {
                    return true;
                }    
                if(!netOrPort->sempty()){
                    return true;
                }    
            }    
        }
        return false;
    }
}
