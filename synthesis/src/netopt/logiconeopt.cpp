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


#include "logiconeopt.h"

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
    LogicOneOpt::LogicOneOpt() : gndnet(0){
    }

    LogicOneOpt::~LogicOneOpt() {
    }

    void
    LogicOneOpt::clear()
    {
        gndnet = 0;
        NetOptBase::clear();
    }

    SNet*
    LogicOneOpt::getGndNet(SBlock *block)
    {
        if(!gndnet)
        {
            SInst *gnd = CreateInstance(block, SCell::GND, "GND", 0);
            gndnet = CreateNet(block, "net");
            const_cast<SPin*>(gnd->getPin("out"))->setActual(gndnet);
        }
        return gndnet;
        
    }

    bool
    LogicOneOpt::operator()(SInst *inst) 
    {
        if(this->condition(inst)) 
        {
            this->addGateOfInterest(inst);
            return true;
        }    
        return false;

    }

    bool
    LogicOneOpt::condition(const SInst *inst) const
    {
        SCell::CellType type = inst->getCellType();
        if(type != SCell::VCC) {
            return false;
        }
        SPin *pin = const_cast<SPin*>(inst->getPin("out"));
        //Sinks& nets = pin->getSinks();
        //if(nets.empty()) {
        //    return false;
        //}    

        SPN *net = const_cast<SPN*>(pin->getActual());
        if(!net) {
            return false;
        }    
        //Sinks& pins = net->getSinks();

        //Sinks::const_iterator sinkEnd = pins.end();
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
                (t==SCell::NOR) || (t==SCell::XOR) || 
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
    LogicOneOpt::optimize(SBlock *block)
    {
        list<string> rmlist;
        for(string name = this->getNextGateName(); !name.empty();
            name = this->getNextGateName())
        {
            SInst *inst = const_cast<SInst*>(block->getInst(name));
            if(!inst) {
                //deleted by some other processing
                continue;
            }
            if(!this->condition(inst)) {
                continue;
            }    
            const SPin *pin = inst->getPin("out");
            const SPN *cntv = pin->getActual();
            if(!cntv ) {
                //deleted
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
    LogicOneOpt::compact(SBlock *block, SInst *inst)
    {
        const SPin *pin = inst->getPin("out");
        SPN *cntv = const_cast<SPN*>(pin->getActual());

        //Sinks& sinks = cntv->getSinks();
        //if(sinks.empty()) {
        //    return false;
        //}
        bool status = false;

        //Sinks::const_iterator sink = sinks.begin();
        //Sinks::const_iterator sinkEnd = sinks.end();
        cntv->reset();

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
                    if(compactNor(block, sinst)) {
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
    LogicOneOpt::compactAnd(SBlock *block, SInst *inst, SPN *cntv)
    {
        const list<SPin*>& inpins = inst->getInPins();
        bool status = true;

        switch(inpins.size())
        {
            case 2:
            {
                compactAnd2(block, inst, cntv);
                break;    
            }
            case 3:
            case 4:
            {
                SInst *ginst = CreateInstance(block, SCell::AND, "AND", inpins.size()-1);
                list<SPin*>& gpins = const_cast<list<SPin*>&>(ginst->getInPins());
                list<SPin*>::iterator gpin = gpins.begin();
                bool flag = true; //same net might be connected multiple times

                for(list<SPin*>::const_iterator iter = inpins.begin();
                    iter != inpins.end(); ++iter)
                {
                    const SPN *act = (*iter)->getActual();
                    if((act == cntv) && flag) 
                    {
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
    LogicOneOpt::compactAnd2(SBlock *block, SInst *inst, SPN *cntv)
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

//        if(in->isPort() && out->isPort()) 
//        {
            SInst *binst = CreateInstance(block, SCell::BUF, "BUF", 1);
            const_cast<SPin*>(binst->getPin("in"))->setActual(in);
            const_cast<SPin*>(binst->getPin("out"))->setActual(out);
//        }    
//        else if(in->isPort())
//        {
//            out->reset();
//            for(Sink s = out->getSink(); !out->sinkEnd(); 
//                s = out->getNextSink()) {
//                (*s)->setActual(in);
//            }
//            out->reset();
//        }
//        else if(out->isPort())
//        {
//            in->reset();
//            for(Driver d = in->getDriver(); !in->driverEnd();
//                d = in->getNextDriver()) {
//                (*d)->setActual(out);
//            }
//            in->reset();
//        }
//        else 
//        {
//            in->reset();
//            for(Driver d = in->getDriver(); !in->driverEnd();
//                d = in->getNextDriver()) {
//                (*d)->setActual(out);
//            }
//            in->reset();
//            for(Sink s = in->getSink(); !in->sinkEnd();
//                s = in->getNextSink()) {
//                (*s)->setActual(out);
//            }
//            in->reset();
//        }    
    }    

    bool
    LogicOneOpt::compactOr(SBlock *block, SInst *inst, SPN *vccnet)
    {
        bool status = false;
        SPin *outpin = inst->getOutPins().front();
        SPN *outcntv = outpin->getActual();

        //Sinks& sinks = const_cast<SCntv*>(outcntv)->getSinks();
        //Sinks::const_iterator sink = sinks.begin();
        //Sinks::const_iterator sinkEnd = sinks.end();
        outcntv->reset();

        for(Sink sink = outcntv->getSink(); !outcntv->sinkEnd(); 
            sink = outcntv->getNextSink())
        {
            SPin *pin = static_cast<SPin*>(*sink);
            pin->setActual(vccnet);
            status = true;
        }    
        if(status) { 
            block->deleteCellInst(inst);
        }    
        outcntv->reset();

        return status;
    }    
    bool
    LogicOneOpt::compactNor(SBlock *block, SInst *inst)
    {
        gndnet = this->getGndNet(block);

        SPin *outpin = inst->getOutPins().front();
        SPN *outcntv = const_cast<SPN*>(outpin->getActual());

        //Sinks& sinks = outcntv->getSinks();
        //Sinks::const_iterator sink = sinks.begin();
        //Sinks::const_iterator sinkEnd = sinks.end();
        outcntv->reset();

        for(Sink sink = outcntv->getSink(); !outcntv->sinkEnd(); 
            sink = outcntv->getNextSink())
        {
            SPin *pin = static_cast<SPin*>(*sink);
            pin->setActual(gndnet);
        }    

        block->deleteCellInst(inst);
        outcntv->reset();

        return true;
    }    
    bool
    LogicOneOpt::compactXor(SBlock *block, SInst *inst, SPN *vccnet)
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
            if(act != vccnet) 
            {
                const_cast<SPin*>(inv->getPin("in"))->setActual(act);
                break;
            }
        }    
        block->deleteCellInst(inst);
        return true;
    }
    bool
    LogicOneOpt::compactXnor(SBlock *block, SInst *inst, SPN *vccnet)
    {
        const list<SPin*>& inpins = inst->getInPins();
        if(inpins.size() != 2) {
            return false;
        }
        const SPin *outpin = inst->getPin("out");

        const SPN *othernet = 0;
        list<SPin*>::const_iterator iter = inpins.begin();

        for(; iter != inpins.end(); ++iter) {
            if((*iter)->getActual() != vccnet) 
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
    LogicOneOpt::compactMux(SBlock *block, SInst *inst, SPN *vccnet)
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
            //actual->reset();
            if(actual->dsize() != 1) {
                return false;
            }
            Driver d = actual->getDriver();
            const SInst *tinst = (*d)->getCellInst();
            if(!tinst || (tinst->getCellType() != (SCell::VCC))) 
            {
              //  actual->reset();
                return false;
            }
        }    
        SInst *binst = CreateInstance(block, SCell::BUF, "BUF", 1);
        const_cast<SPin*>(binst->getPin("in"))->setActual(vccnet);
        const_cast<SPin*>(binst->getPin("out"))->setActual(inst->getPin("out")->getActual());
        block->deleteCellInst(inst);
        return true;
    }

    bool
    LogicOneOpt::compactMux2(SBlock *block, SInst *inst, SPN *vccnet)
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

        if((in0act != vccnet) && (in1act != vccnet)) {
            return false;
        }

        SInst *orInst = CreateInstance(block, SCell::OR, "OR", 2);
        orInst->getPin("out")->setActual(out);
        if(in0act == vccnet)
        {
        
            SPN *net = CreateNet(block, "net");
            SInst *invInst = CreateInstance(block, SCell::NOT, "NOT", 1);
            invInst->getPin("in")->setActual(sel);
            invInst->getPin("out")->setActual(net);

            orInst->getPin(SIxName("in", 0))->setActual(net);
            orInst->getPin(SIxName("in", 1))->setActual(in1act);
        }
        else
        {
            orInst->getPin(SIxName("in", 0))->setActual(sel);
            orInst->getPin(SIxName("in", 1))->setActual(in0act);
        }
        block->deleteCellInst(inst);
        return true;
    }

    bool
    LogicOneOpt::compactBuf(SBlock *block, SInst *inst, SPN *vccnet)
    {
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
            (*s)->setActual(vccnet);
        }
        block->deleteCellInst(inst);
        return true;
    }
    bool
    LogicOneOpt::compactInv(SBlock *block, SInst *inst)
    {
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
            (*s)->setActual(getGndNet(block));
        }
        block->deleteCellInst(inst);
        return true;
            
    }
}

