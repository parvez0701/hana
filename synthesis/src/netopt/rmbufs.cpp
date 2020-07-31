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

#include <list>

#include "rmbufs.h"

#include "sblock.h"
#include "sinst.h"
#include "spin.h"
#include "scell.h"

using std::string;
using std::list;

using stview::SInst;
using stview::SBlock;
using stview::SPN;
using stview::SCell;
using stview::Sink;
using stview::Driver;

namespace opt
{
    RmBufs::RmBufs() {
    }

    RmBufs::~RmBufs() {
    }

    bool
    RmBufs::operator()(SInst *inst)
    {
        if(this->condition(inst)) 
        {
            this->addGateOfInterest(inst);
            return true;
        }    
        return false;
    }

    bool
    RmBufs::optimize(SBlock *block)
    {
        list<string> rmlist;
        bool status = false;
        int count = 0;
        for(string name = this->getNextGateName(); !name.empty();
            name = this->getNextGateName())
        {
            ++count;
            //block->resetVisitCount(); //Expensive ?
            SInst *inst = const_cast<SInst*>(block->getInst(name));
            if(!inst) {
                //deleted by some other processing
                continue;
            }
            if(!this->condition(inst)) {
                continue;
            }
            

            SPN *in = const_cast<SPN*>(inst->getPin("in")->getActual());
            SPN *out = const_cast<SPN*>(inst->getPin("out")->getActual());
            if(!in || !out) 
            {
                block->deleteCellInst(inst);
                return true;
            }    
            in->reset();
            if(!in->dempty())
            {
                for(Driver d = in->getDriver(); !in->driverEnd(); 
                    d = in->getNextDriver()) 
                {
                    if((*d)->getVisitCount() == count) {
                        continue;
                    }    
                    (*d)->setActual(out);
                    (*d)->incVisitCount();
                }
            }
            else
            {
                for(Sink s = out->getSink(); !out->sinkEnd(); 
                    s = out->getNextSink()) 
                {
                    if((*s)->getVisitCount() == count) {
                        continue;
                    }    

                    (*s)->setActual(in);
                    (*s)->incVisitCount();
                }
            }    
            in->reset();
            block->deleteCellInst(inst);
            status = true;
        }    
        block->resetVisitCount();
        return status;
    }

    bool
    RmBufs::condition(const SInst* inst) const
    {
        if(inst->getCellType() != SCell::BUF) {
            return false;
        }

        const SPN *in = inst->getPin("in")->getActual();
        const SPN *out = inst->getPin("out")->getActual();
        if(!in || !out) {
            return true;
        }    
        if((in->ssize() > 1) || (in->dsize() > 1)) {
            return false;
        }    
        if((out->ssize() > 1) || (out->dsize() > 1)) {
            return false;
        }    

        return !in->isPort() && !out->isPort();
    }
}
