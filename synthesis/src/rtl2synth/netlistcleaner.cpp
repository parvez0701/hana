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

#include <iostream>
#include "netlistcleaner.h"
#include "sycontainer.h"
#include "syroot.h"

using std::map;
using std::pair;
using std::make_pair;
using std::cout;

namespace Synth
{
    DriverSinkMgr::DriverSinkMgr(const SyModule *mod) : 
        currMod(mod) {
        init();
    }

    DriverSinkMgr::~DriverSinkMgr() {
        this->destroy();
    }

    const SyList<SyScPin*>*
    DriverSinkMgr::getDrivers(const SyScCntv *net) const
    {
        if(!net) {
            return 0;
        }    
        DriverSinkInfo::const_iterator iter = driverSinkInfo.find(net);
        if(iter == driverSinkInfo.end()) {
            return 0;
        }
        return iter->second.first;
    }
    const SyList<SyScPin*>*
    DriverSinkMgr::getSinks(const SyScCntv *net) const
    {
        if(!net) {
            return 0;
        }    
        DriverSinkInfo::const_iterator iter = driverSinkInfo.find(net);
        if(iter == driverSinkInfo.end()) {
            return 0;
        }
        return iter->second.second;
    }

    void
    DriverSinkMgr::addDriver(const SyScCntv *net, const SyScPin *pin)
    {
        if(!net || !pin) {
            return;
        }    
        DriverSinkInfo::iterator iter = driverSinkInfo.find(net);
        if(iter == driverSinkInfo.end())
        {
            DriversAndSinks ds;
            ds.first = new SyList<SyScPin*>();
            ds.second = new SyList<SyScPin*>();
            ds.first->push_back(const_cast<SyScPin*>(pin));
            driverSinkInfo.insert(make_pair(net, ds));
        }
        else {
            iter->second.first->push_back(const_cast<SyScPin*>(pin));
        }    
    }
        
    void
    DriverSinkMgr::addSink(const SyScCntv *net, const SyScPin *pin)
    {
        if(!net || !pin) {
            return;
        }    
        DriverSinkInfo::iterator iter = driverSinkInfo.find(net);
        if(iter == driverSinkInfo.end())
        {
            DriversAndSinks ds;
            ds.first = new SyList<SyScPin*>();
            ds.second = new SyList<SyScPin*>();
            ds.second->push_back(const_cast<SyScPin*>(pin));
            driverSinkInfo.insert(make_pair(net, ds));
        }
        else {
            iter->second.second->push_back(const_cast<SyScPin*>(pin));
        }    
    }

    void
    DriverSinkMgr::init()
    {
        assert(currMod);

        const SyList<SyModInst*>& minsts = currMod->getModInsts();
        const SyList<SyPrimInst*>& pinsts = currMod->getPrimInsts();

        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next) {
            this->addDriverSinkData(node->val);
        }    
        for(SyNode<SyPrimInst*>* node = pinsts.head; node; node = node->next) {
            this->addDriverSinkData(node->val);
        }    
    }

    void
    DriverSinkMgr::destroy()
    {
        DriverSinkInfo::iterator beginIter = driverSinkInfo.begin();
        DriverSinkInfo::iterator endIter = driverSinkInfo.end();

        for(; beginIter != endIter; ++beginIter) 
        {
            delete beginIter->second.first;
            delete beginIter->second.second;
        }
        driverSinkInfo.clear();
    }

    void
    DriverSinkMgr::addDriverSinkData(const SyInst *inst)
    {  
        const SyList<SyScPin*>& pins = inst->getPins();

        for(SyNode<SyScPin*>* node = pins.head; node; node = node->next) {
            if(const SyScCntv *actual = dynamic_cast<const SyScCntv*>(
                node->val->getActual())) 
            {
                SyScPin *pin = node->val;
                SyScPort::SyDirType dir = pin->getDir();
                if(dir == SyScPort::IN) {
                    this->addSink(actual, pin);
                }
                else if(dir == SyScPort::OUT) {
                    this->addDriver(actual, pin);
                }
                else
                {
                    this->addDriver(actual, pin);
                    this->addSink(actual, pin);
                }
            }
        }    
      
    }

    NetlistCleaner::NetlistCleaner() : currMod(0), dsMgr(0) {
    }

    NetlistCleaner::~NetlistCleaner() {
    }

    void
    NetlistCleaner::doCleanup()
    {
        SyRoot::SyStage oldStage = SyRoot::getStage();
        SyRoot::setStage(SyRoot::STAGE_2);
        const map<string, SyModule*, cmp_str> & synthMods =
            SyRoot::instance()->getSynthMods();

        map<string, SyModule*, cmp_str>::const_iterator beginIter = 
        synthMods.begin();    
        map<string, SyModule*, cmp_str>::const_iterator endIter = 
        synthMods.end();    

        for(; beginIter != endIter; ++beginIter) 
        {
            currMod = beginIter->second;
            this->init();
            //this->printUsage();
            this->removeBuffers();
            //this->printUsage();
            this->removeInsts();
            this->removeNets();
            this->destroy();
        }    
        SyRoot::setStage(oldStage);
    }

    void
    NetlistCleaner::init()
    {
        assert(currMod);

        dsMgr = new DriverSinkMgr(currMod);

        const SyList<SyScNet*>& nets = currMod->getNets();
        for(SyNode<SyScNet*>* node = nets.head; node; node = node->next) {
            netUsageTable.insert(make_pair(node->val, 0));
        }    

        const SyList<SyScPort*>& ports = currMod->getPorts();
        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next) {
            netUsageTable.insert(make_pair(node->val, 0));
        }    

        const SyList<SyScReg*>& regs = currMod->getRegs();
        for(SyNode<SyScReg*>* node = regs.head; node; node = node->next) {
            netUsageTable.insert(make_pair(node->val, 0));
        }    

        const SyModule::TfPortTable& tfPortTable = currMod->getTfPorts();
        for(SyModule::TfPortTable::const_iterator iter = tfPortTable.begin();
            iter != tfPortTable.end(); ++iter)
        {
            const SyScCntv *cntv = dynamic_cast<const SyScCntv*>(iter->second);
            if(cntv) {
                netUsageTable.insert(make_pair(cntv, 0));
            }    
        }

        this->computeUsage();
        
    }

    void
    NetlistCleaner::destroy() 
    {
        netUsageTable.clear();
        insts2bRemoved.clear();
        delete dsMgr;
        dsMgr = 0;
    }

    void
    NetlistCleaner::incUsage(const SyScCntv* net)
    {
        NetUsageTable::iterator iter = netUsageTable.find(net);
        assert(iter != netUsageTable.end());
        ++(iter->second);
    }
    void
    NetlistCleaner::decUsage(const SyScCntv* net)
    {
        NetUsageTable::iterator iter = netUsageTable.find(net);
        assert(iter != netUsageTable.end());
        --(iter->second);
    }

    int
    NetlistCleaner::getUsage(const SyScCntv* net) const
    {
        NetUsageTable::const_iterator iter = netUsageTable.find(net);
        assert(iter != netUsageTable.end());
        return iter->second;
    }    

    bool
    NetlistCleaner::isUnused(const SyScCntv *net) const {
        return this->getUsage(net) == 0;
    }

    void
    NetlistCleaner::computeUsage()
    {
        assert(currMod);

        const SyList<SyModInst*>& minsts = currMod->getModInsts();
        const SyList<SyPrimInst*>& pinsts = currMod->getPrimInsts();

        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next) {
            this->computeUsage(node->val);
        }    
        for(SyNode<SyPrimInst*>* node = pinsts.head; node; node = node->next) {
            this->computeUsage(node->val);
        }    
    }
    void
    NetlistCleaner::computeUsage(const SyInst *inst)
    {
        const SyList<SyScPin*>& pins = inst->getPins();

        for(SyNode<SyScPin*>* node = pins.head; node; node = node->next) {
            if(const SyScCntv *actual = dynamic_cast<const SyScCntv*>(
                node->val->getActual())) {
                this->incUsage(actual);
            }
        }
    }    

    bool
    NetlistCleaner::isBuffer(const SyModInst *minst) const
    {
        if(!minst) {
            return false;
        }    
        return minst->getMasterName() == string("BUF_1_1");
    }


    void
    NetlistCleaner::removeBuffers()
    {
        const SyList<SyScPort *>& ports = currMod->getPorts();
        visitedPins.clear();
        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next) {
            if(node->val->getDir() == SyScPort::IN) {
                this->removeBuffersForward(node->val);
            }
        }    
        visitedPins.clear();
        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next) {
            if(node->val->getDir() == SyScPort::OUT) {
                this->removeBuffersBackward(node->val);
            }
        }    

    }

    void
    NetlistCleaner::removeBuffersForward(SyScPort *port)
    {
        const SyList<SyScPin*> *sinks = dsMgr->getSinks(port);
        for(SyNode<SyScPin*>* node = sinks ? sinks->head : 0; node;
            node = node->next) {
            this->removeBuffersForward(port, node->val);
        }    
    }    

    void
    NetlistCleaner::removeBuffersBackward(SyScPort *port)
    {
        const SyList<SyScPin*> *drivers = dsMgr->getDrivers(port);
        for(SyNode<SyScPin*>* node = drivers ? drivers->head : 0; node;
            node = node->next) {
            this->removeBuffersBackward(port, node->val);
        }    
    }    

    void
    NetlistCleaner::removeBuffersForward(SyScCntv *src, SyScPin *dest)
    {
        if(visitedPins.find(dest) != visitedPins.end()) {
            return;
        }
        visitedPins.insert(dest);
        SyModInst *inst = const_cast<SyModInst*>(
            dynamic_cast<const SyModInst*>(dest->getInst()));
        
        if(!inst) {
            return;
        }    


        if(this->isBuffer(inst))
        {
            SyScPin *outPin = inst->getPin(SyIxName("OUT"));
            const SyScCntv *actual = dynamic_cast<const SyScCntv*>(
                outPin->getActual());
            if(dynamic_cast<const SyScPort*>(actual)) 
            {
                this->setActual(dest, src);
                return;
            }    
            if(actual)
            {
                const SyList<SyScPin*> *sinks = dsMgr->getSinks(actual);
                const SyList<SyScPin*> *drivers = dsMgr->getDrivers(actual);
                if((sinks && sinks->size() == 1) &&
                    (!drivers || (drivers->size() == 1)))
                {
                
                    if(insts2bRemoved.find(inst) == insts2bRemoved.end())
                    {
                        //We don't want to decrement the count,
                        //once during forwar removal and next
                        //time during backward removal
                        this->decUsage(actual);
                        const SyScPin *inPin = inst->getPin(SyIxName("IN"));
                        const SyScCntv *inCntv = dynamic_cast<const SyScCntv*>(
                        inPin->getActual());
                        this->decUsage(inCntv);    
                    }    

                    insts2bRemoved.insert(inst);
                    this->removeBuffersForward(const_cast<SyScCntv*>(src),
                    sinks->head->val);
                }    

                else 
                {
                    //new start
                    this->setActual(dest, src);

                    for(SyNode<SyScPin*>* node = sinks ? sinks->head : 0;
                        node; node = node->next) {
                        this->removeBuffersForward(const_cast<SyScCntv*>(actual),
                        node->val);
                    }    
                }    
            }
        }    
        else
        {
            //new start
            this->setActual(dest, src);
            const SyList<SyScPin*>& pins = inst->getPins();
            for(SyNode<SyScPin*>* node = pins.head; node; node = node->next)
            {
                if(node->val->getDir() != SyScPort::OUT) {
                    continue;
                }
                SyScPin *pin = node->val;
                SyScCntv *actual = const_cast<SyScCntv*>(
                    dynamic_cast<const SyScCntv*>(pin->getActual()));
                
                if(!actual) {
                    continue;
                }
                const SyList<SyScPin*>* sinks = dsMgr->getSinks(actual);
                for(SyNode<SyScPin*>* node1 = sinks ? sinks->head : 0;
                    node1; node1 = node1->next) {
                    removeBuffersForward(actual, node1->val);
                }
            }
        }
    }    
    void
    NetlistCleaner::removeBuffersBackward(SyScCntv *src, SyScPin *dest)
    {
        if(visitedPins.find(dest) != visitedPins.end()) {
            return;
        }
        visitedPins.insert(dest);

        SyModInst *inst = const_cast<SyModInst*>(
            dynamic_cast<const SyModInst*>(dest->getInst()));
        
        if(!inst) {
            return;
        }    

        if(this->isBuffer(inst))
        {
            SyScPin *inPin = inst->getPin(SyIxName("IN"));
            const SyScCntv *actual = dynamic_cast<const SyScCntv*>(
                inPin->getActual());
            if(dynamic_cast<const SyScPort*>(actual)) 
            {
                this->setActual(dest, src);
                return;
            }    
            if(actual)
            {
                const SyList<SyScPin*> *drivers = dsMgr->getDrivers(actual);
                const SyList<SyScPin*> *sinks = dsMgr->getSinks(actual);
                if((drivers && drivers->size() == 1) &&(
                    !sinks || (sinks->size() == 1)))
                {
                    if(insts2bRemoved.find(inst) == insts2bRemoved.end())
                    {
                        this->decUsage(actual);

                        const SyScPin *outPin = inst->getPin(SyIxName("OUT"));
                        const SyScCntv *outCntv = dynamic_cast<const SyScCntv*>(
                        outPin->getActual());
                        this->decUsage(outCntv);    
                    }    
                    insts2bRemoved.insert(inst);
                    this->removeBuffersBackward(const_cast<SyScCntv*>(src),
                    drivers->head->val);
                    //this->decUsage(src);
                }
                else
                {
                    //new start
                    this->setActual(dest, src);
                    for(SyNode<SyScPin*>* node = drivers ? drivers->head : 0;
                        node; node = node->next) {
                        this->removeBuffersBackward(const_cast<SyScCntv*>(actual),
                        node->val);
                    }    
                }    
            }
        }    
        else
        {
            //new start
            this->setActual(dest, src);
            const SyList<SyScPin*>& pins = inst->getPins();
            for(SyNode<SyScPin*>* node = pins.head; node; node = node->next)
            {
                if(node->val->getDir() != SyScPort::IN) {
                    continue;
                }
                SyScPin *pin = node->val;
                SyScCntv *actual = const_cast<SyScCntv*>(
                    dynamic_cast<const SyScCntv*>(pin->getActual()));
                
                if(!actual) {
                    continue;
                }
                const SyList<SyScPin*>* drivers = dsMgr->getDrivers(actual);
                for(SyNode<SyScPin*>* node1 = drivers ? drivers->head : 0;
                    node1; node1 = node1->next) {
                    removeBuffersBackward(actual, node1->val);
                }
            }
        }
    }    

    bool
    NetlistCleaner::is2bRemoved(const SyBase* obj) const
    {
        if(const SyModInst *minst = dynamic_cast<const SyModInst*>(obj)) { 
            return insts2bRemoved.find(const_cast<SyModInst*>(minst)) != 
            insts2bRemoved.end();
        }
        if(const SyScCntv *cntv = dynamic_cast<const SyScCntv*>(obj)) 
        {
            if(cntv->isVecBit()) {
                return false;
            }    
            return isUnused(cntv);
        }
        return false;
    }

    template <class T> void
    NetlistCleaner::removeObjects(SyList<T*>& objs)
    {
        assert(currMod);
        SyList<T*> tobjs;
        SyNode<T*>* node = objs.head;
        for(; node; node = node->next)
        {
            if(is2bRemoved(node->val)) {
                currMod->removeObject(node->val->getIxName().toString());
            }
            else {
                tobjs.push_back(node->val);
            }    
        }
        objs.clear();
        objs.merge(tobjs);
    }

    void
    NetlistCleaner::removeInsts()
    {
        SyList<SyModInst*>& insts = const_cast<SyList<SyModInst*>&>(
            currMod->getModInsts());

        removeObjects<SyModInst>(insts);    
        set<SyModInst*>::const_iterator beginIter = insts2bRemoved.begin();
        set<SyModInst*>::const_iterator endIter = insts2bRemoved.end();

        for(; beginIter != endIter; ++beginIter) {
            delete *beginIter;
        }    

    }

    void
    NetlistCleaner::removeNets()
    {
        assert(currMod);
        SyList<SyScNet*>& nets = const_cast<SyList<SyScNet*>&>(
            currMod->getNets());
        removeObjects<SyScNet>(nets);    
    }
    void
    NetlistCleaner::removeRegs()
    {
        assert(currMod);
        SyList<SyScReg*>& regs = const_cast<SyList<SyScReg*>&>(
            currMod->getRegs());
        removeObjects<SyScReg>(regs);    
    }

    void
    NetlistCleaner::setActual(SyScPin *pin, SyScCntv *cntv)
    {
        const SyScCntv *actual = dynamic_cast<const SyScCntv*>(
            pin->getActual());

        if(actual) {
            this->decUsage(actual);
        }    
        
        if(cntv) {
            this->incUsage(cntv);
        }
        pin->setActual(cntv);
    }

    void
    NetlistCleaner::printUsage() const
    {
        NetUsageTable::const_iterator beginIter    = netUsageTable.begin();
        NetUsageTable::const_iterator endIter    = netUsageTable.end();
        cout << "NET            USAGE\n";
        cout << "-----------------------------------------\n";

        for(; beginIter != endIter; ++beginIter) {
            cout << beginIter->first->toString() <<    "            " <<
            beginIter->second << "\n";
        }    
        cout << "-----------------------------------------\n";
        
    }
}
