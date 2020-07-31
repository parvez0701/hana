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

#include "logiczeroopt.h"

#include "scell.h"
#include "scntv.h"
#include "sblock.h"
#include "spin.h"
#include "sinst.h"
#include "snet.h"
#include "sutils2.h"

using stview::SInst;
using stview::SCell;
using stview::SCntv;
using stview::SBlock;
using stview::SPin;
using stview::SNet;
using stview::Drivers;
using stview::Sinks;
using stview::Driver;
using stview::Sink;
using stview::SPN;
using stview::SIxName;

using std::list;

namespace opt
{
    LogicZeroOpt::LogicZeroOpt() : vccnet(0) {
    }

    LogicZeroOpt::~LogicZeroOpt() {
    }

    void
    LogicZeroOpt::clear() 
    {
        vccnet = 0;
        NetOptBase::clear();
    }

    SNet*
    LogicZeroOpt::getVccNet(SBlock* block)
    {
        if(!vccnet)
        {
            SInst *vcc = CreateInstance(block, SCell::VCC, "VCC", 0);
            vccnet = CreateNet(block, "net");
            const_cast<SPin*>(vcc->getPin("out"))->setActual(vccnet);
        }
        return vccnet;
    }

    bool
    LogicZeroOpt::condition(const SInst *inst) const
    {
        SCell::CellType type = inst->getCellType();
        if(type != SCell::GND) {
            return false;
        }
        SPin *pin = const_cast<SPin*>(inst->getPin("out"));

        SPN *net = const_cast<SPN*>(pin->getActual());
        if(!net) {
            return false;
        }    
        net->reset();

        for(Sink sink = net->getSink(); !net->sinkEnd(); 
            sink = net->getNextSink())
        {
            SPin *p = *sink;
            const SInst *i = p->getCellInst();
            if(!i) {
                continue;
            }    
            SCell::CellType t = i->getCellType();
            if(( t==SCell::AND) || (t==SCell::OR) || 
                (t==SCell::NAND) || (t==SCell::XOR) || 
                (t==SCell::XNOR) || (t==SCell::MUX) ||
                (t==SCell::BUF) || (t==SCell::NOT)) 
            {
                net->reset();
                return true;
            }    
                
        }
        net->reset();
        return false;
    }    

    bool
    LogicZeroOpt::operator()(SInst *inst) 
    {
        if(this->condition(inst)) 
        {
            this->addGateOfInterest(inst);
            return true;
        }    
        return false;

    }

    bool
    LogicZeroOpt::optimize(SBlock *block)
    {
        list<string> rmlist;
        for(string name = this->getNextGateName(); !name.empty();
            name = this->getNextGateName())
        {
            SInst *inst = const_cast<SInst*>(block->getInst(name));
            if(!inst) {
                continue;
            }
            if(!this->condition(inst)) {
                continue;
            }    
            const SPin *pin = inst->getPin("out");
            const SPN *cntv = pin->getActual();
            if(!cntv ) {
                continue;
            }    
            if(this->compact(block, inst)) 
            {
                rmlist.push_back(name);
                this->changed(true);
            }    
        }

        for(list<string>::const_iterator iter = rmlist.begin();
            iter != rmlist.end(); ++iter) {
            this->removeGateOfInterest(*iter);
        }    

        return this->changed();
        
    }



    bool
    LogicZeroOpt::compact(SBlock *block, SInst *inst)
    {
        const SPin *pin = inst->getPin("out");
        SPN *cntv = const_cast<SPN*>(pin->getActual());
        cntv->reset();

        bool status = false;


        for(Sink sink = cntv->getSink(); !cntv->sinkEnd(); 
            sink = cntv->getNextSink())
        {
            cntv->lock();
            SPin *spin = *sink;
            SInst *sinst = const_cast<SInst*>(spin->getCellInst());
            if(!sinst) {
                continue;
            }
            SCell::CellType type = sinst->getCellType();
            switch (type)
            {
                case SCell::AND:
                {
                    if(compactAnd(block, sinst, cntv)) {
                        status = true;
                    }    
                    break;
                }
                case SCell::OR:
                {
                    if(compactOr(block, sinst, cntv)) {
                        status = true;
                    }    
                    break;
                }
                case SCell::NOR:
                {
                    if(compactNand(block, sinst)) {
                        status = true;
                    }    
                    break;
                }
                case SCell::XOR:
                {
                    if(compactXor(block, sinst, cntv)) {
                        status = true;
                    }    
                    break;
                }
                case SCell::XNOR:
                {
                    if(compactXnor(block, sinst, cntv)) {
                        status = true;
                    }    
                    break;
                }
                case SCell::MUX:
                {
                    if(compactMux(block, sinst, cntv)) {
                        status = true;
                    }    
                    else if(compactMux2(block, sinst, cntv)) {
                        status = true;
                    }    
                    break;
                }
                case SCell::BUF:
                {
                    if(compactBuf(block, sinst, cntv)) {
                        status = true;
                    }    
                    break;
                }
                case SCell::NOT:
                {
                    if(compactInv(block, sinst)) {
                        status = true;
                    }    
                    break;
                }
                default: {
                    break;
                }    
            }
            cntv->unlock();
            if(status) {
                break;
            }    
        }
        cntv->reset();
        return status;
    }

    bool
    LogicZeroOpt::compactOr(SBlock *block, SInst *inst, SPN *cntv)
    {
        const list<SPin*>& inpins = inst->getInPins();
        bool status = true;

        switch(inpins.size())
        {
            case 2:
            {
                compactOr2(block, inst, cntv);
                break;    
            }
            case 3:
            case 4:
            {
                SInst *ginst = CreateInstance(block, SCell::OR, "OR", inpins.size()-1);
                list<SPin*>& gpins = const_cast<list<SPin*>&>(ginst->getInPins());
                list<SPin*>::iterator gpin = gpins.begin();
                bool flag = true;

                for(list<SPin*>::const_iterator iter = inpins.begin();
                    iter != inpins.end(); ++iter)
                {
                    const SPN *act = (*iter)->getActual();
                    if((act == cntv) && flag) 
                    {
                        //skip only once, same net might be connected
                        //multiple times
                        flag = false;
                        continue;
                    }
                    (*gpin)->setActual(act);
                    ++gpin;
                }
                const_cast<SPin*>(ginst->getPin("out"))->setActual(
                    inst->getPin("out")->getActual());
                break;    
            }    
            default:
                status = false;

        }
        if(status) {
            block->deleteCellInst(inst);
        }    
        return status;
    }

    void
    LogicZeroOpt::compactOr2(SBlock *block, SInst *inst, SPN *cntv)
    {
        const list<SPin*>& inpins = inst->getInPins();
        SPN *in = 0;
        SPN *out = 0;
        list<SPin*>::const_iterator iter = inpins.begin();
        for(iter = inpins.begin(); iter != inpins.end(); ++iter) 
        {
            in = (*iter)->getActual();
            if(in != cntv) {
                break;
            }
        }
        assert(in);
        out = const_cast<SPN*>(inst->getPin("out")->getActual());

        SInst *binst = CreateInstance(block, SCell::BUF, "BUF", 1);
        const_cast<SPin*>(binst->getPin("in"))->setActual(in);
        const_cast<SPin*>(binst->getPin("out"))->setActual(out);
    }    

    bool
    LogicZeroOpt::compactAnd(SBlock *block, SInst *inst, SPN *gndnet)
    {
        bool status = false;
        SPin *outpin = inst->getOutPins().front();
        SPN *outcntv = outpin->getActual();
        outcntv->reset();


        for(Sink sink = outcntv->getSink(); !outcntv->sinkEnd(); 
            sink = outcntv->getNextSink())
        {
            SPin *pin = static_cast<SPin*>(*sink);
            pin->setActual(gndnet);
            status = true;
        }    
        if(status) {
            block->deleteCellInst(inst);
        }    

        outcntv->reset();

        return status;
    }    
    bool
    LogicZeroOpt::compactNand(SBlock *block, SInst *inst)
    {
        vccnet = this->getVccNet(block);

        SPin *outpin = inst->getOutPins().front();
        SPN *outcntv = const_cast<SPN*>(outpin->getActual());
        outcntv->reset();


        for(Sink sink = outcntv->getSink(); !outcntv->sinkEnd(); 
            sink = outcntv->getNextSink())
        {
            SPin *pin = static_cast<SPin*>(*sink);
            pin->setActual(vccnet);
        }    

        block->deleteCellInst(inst);
        outcntv->reset();

        return true;
    }    
    bool
    LogicZeroOpt::compactXnor(SBlock *block, SInst *inst, SPN *gndnet)
    {
        const list<SPin*>& inpins = inst->getInPins();
        if(inpins.size() != 2) {
            return false;
        }

        SInst *inv = CreateInstance(block, SCell::NOT, "NOT", 1);
        SPin *outpin = const_cast<SPin*>(inv->getPin("out"));
        outpin->setActual(inst->getPin("out")->getActual());

        list<SPin*>::const_iterator iter = inpins.begin();

        for(; iter != inpins.end(); ++iter) 
        {
            const SPN *act = (*iter)->getActual();
            if(act != gndnet) 
            {
                const_cast<SPin*>(inv->getPin("in"))->setActual(act);
                break;
            }
        }     
        block->deleteCellInst(inst);
        return true;
    }
    bool
    LogicZeroOpt::compactXor(SBlock *block, SInst *inst, SPN *gndnet)
    {
        const list<SPin*>& inpins = inst->getInPins();
        if(inpins.size() != 2) {
            return false;
        }
        const SPin *outpin = inst->getPin("out");

        const SPN *othernet = 0;
        list<SPin*>::const_iterator iter = inpins.begin();

        for(; iter != inpins.end(); ++iter) {
            if((*iter)->getActual() != gndnet) 
            {
                othernet = (*iter)->getActual();
                break;
            }
        }    
        SInst *binst = CreateInstance(block, SCell::BUF, "BUF", 1);
        binst->getPin("in")->setActual(othernet);
        binst->getPin("out")->setActual(outpin->getActual());
        block->deleteCellInst(inst);
        return true;
    }    

    bool
    LogicZeroOpt::compactMux(SBlock *block, SInst *inst, SPN *gndnet)
    {
        const list<SPin*>& inpins = inst->getInPins();

        for(list<SPin*>::const_iterator pin = inpins.begin();
            pin != inpins.end(); ++pin)
        {
            string name = (*pin)->getIxName().getName();
            if(name != "in") {
                continue;
            }
            SPN *actual = (*pin)->getActual();
            actual->reset();
            if(actual->dsize() != 1) {
                return false;
            }
            Driver d = actual->getDriver();
            const SInst *tinst = (*d)->getCellInst();
            if(!tinst || (tinst->getCellType() != (SCell::GND))) 
            {
                actual->reset();
                return false;
            }
        }    
        SInst *binst = CreateInstance(block, SCell::BUF, "BUF", 1);
        const_cast<SPin*>(binst->getPin("in"))->setActual(gndnet);
        const_cast<SPin*>(binst->getPin("out"))->setActual(inst->getPin("out")->getActual());
        block->deleteCellInst(inst);
        return true;
    }

    bool
    LogicZeroOpt::compactMux2(SBlock *block, SInst *inst, SPN *gndnet)
    {
        if((inst->getCellType() != SCell::MUX) ||
            (inst->getInPins().size() != 3)) {
            return false;
        }    
        SPN *in0act = inst->getPin(SIxName("in", 0))->getActual();
        SPN *in1act = inst->getPin(SIxName("in", 1))->getActual();
        SPN *sel = inst->getPin("select")->getActual();
        SPN *out = inst->getPin("out")->getActual();

        in0act->reset();
        in1act->reset();
        sel->reset();
        out->reset();

        if((in0act != gndnet) && (in1act != gndnet)) {
            return false;
        }

        SInst *andInst = CreateInstance(block, SCell::AND, "AND", 2);
        andInst->getPin("out")->setActual(out);
        if(in0act == gndnet)
        {
            andInst->getPin(SIxName("in", 0))->setActual(sel);
            andInst->getPin(SIxName("in", 1))->setActual(in1act);
        }
        else
        {
            SPN *net = CreateNet(block, "net");
            SInst *invInst = CreateInstance(block, SCell::NOT, "NOT", 1);
            invInst->getPin("in")->setActual(sel);
            invInst->getPin("out")->setActual(net);
            andInst->getPin(SIxName("in", 0))->setActual(in0act);
            andInst->getPin(SIxName("in", 1))->setActual(net);
        }
        block->deleteCellInst(inst);
        return true;
    }

    bool
    LogicZeroOpt::compactBuf(SBlock *block, SInst *inst, SPN *gndnet)
    {
        //return false;
        SPN *out = inst->getPin("out")->getActual();
        if(out->isPort()) {
            return false;
        }    
        out->reset();
        if(out->dsize() != 1) {
            return false;
        }    

        for(Sink s = out->getSink(); !out->sinkEnd(); 
            s = out->getNextSink()) {
            (*s)->setActual(gndnet);
        }
        block->deleteCellInst(inst);
        return true;
    }
    bool
    LogicZeroOpt::compactInv(SBlock *block, SInst *inst)
    {
        //return false;
        SPN *out = inst->getPin("out")->getActual();
        if(out->isPort()) {
            return false;
        }    
        out->reset();
        if(out->dsize() != 1) {
            return false;
        }    

        for(Sink s = out->getSink(); !out->sinkEnd(); 
            s = out->getNextSink()) {
            (*s)->setActual(getVccNet(block));
        }
        block->deleteCellInst(inst);
        return true;
            
    }



}
