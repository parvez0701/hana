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

#include "symemory.h"
#include "sycontainer.h"
#include "namemgr.h"

using std::map;
using std::string;
using std::make_pair ;
namespace Synth
{
    SyMemoryHandler* SyMemoryHandler::memory = 0;

    SyMemoryHandler::SyMemoryHandler() {
    }

    SyMemoryHandler::~SyMemoryHandler() {
    }

    SyMemoryHandler*
    SyMemoryHandler::instance() 
    {
        if(!SyMemoryHandler::memory) {
            SyMemoryHandler::memory = new SyMemoryHandler();
        }
        return memory;
    }    

    string
    SyMemoryHandler::getAddrPortName() const {
        return "ADDR";
    }
    string
    SyMemoryHandler::getDataPortName() const {
        return "DATA";
    }

    string
    SyMemoryHandler::getChipSelectName() const {
        return "CS";
    }

    string
    SyMemoryHandler::getReadEnableName() const {
        return "RE";
    }

    string
    SyMemoryHandler::getWriteEnableName() const {
        return "WE";
    }    

    SyModInst*
    SyMemoryHandler::getMemInst(const SyModule *mod, const string& name) const
    {
        ModMemElemInstMap::const_iterator iter = 
            modMemInsts.find(mod->getName());
        if(iter == modMemInsts.end()) {
            return NULL;
        }
        MemElemInst::const_iterator memInst = iter->second.find(name);
        if(memInst == iter->second.end()) {
            return NULL;
        }

        return memInst->second;
    }    

    string
    SyMemoryHandler::getNameHint(const SyModule *mod, const string& reg) const
    {
        string hint = mod->getName() + string("_") + reg;
        return ModuleNameManager::instance()->createName(mod, hint);
        
    }

    SyModule*
    SyMemoryHandler::createMemoryModule(const SyModule *mod, const string& reg) const
    {
        string memName = string("MEM_") + mod->getName();
        return new SyModule(memName);
    }

    SyVPort*
    SyMemoryHandler::addPort(    SyModule *mod,
                                const string& name, int msb, int lsb) const
    {
        SyScPort::SyDirType dir = SyScPort::INOUT;
        SyScNet::SyNetType ntype = SyScNet::REG;
        SyVPort *port = new SyVPort(    name, msb, lsb, false, 
                                        dir, ntype);
        
        mod->addObject(port->getName(), port);
        const SyList<SyScCntv*>& bits = port->getBits();

        for(SyNode<SyScCntv*>* node = bits.head; node; node = node->next) {
            mod->addPort(dynamic_cast<SyScPort*>(node->val));
        }    
        return port;
    }    
        

    SyModInst*
    SyMemoryHandler::createMemory(    SyModule *mod, const string& reg,
                            int addr, int data)
    {
        SyModInst *memInst = this->getMemInst(mod, reg);
        if(memInst) {
            return memInst;
        }    

        SyModule *memMod = this->createMemoryModule(mod, reg);
        SyVPort *vport = this->addPort(
            memMod, string("ADDR"), addr-1, 0);
        vport = this->addPort(
            memMod, string("DATA"), data-1, 0);
        SyScPort *cs = new SyScPort(SyIxName("CS"), false, 
            SyScPort::IN, SyScNet::WIRE);
        
        memMod->addPort(cs);
        memMod->addObject("CS", cs);
        SyScPort *we = new SyScPort(SyIxName("WE"), false, 
            SyScPort::IN, SyScNet::WIRE);
        
        memMod->addPort(we);
        memMod->addObject("WE", we);

        SyScPort *re = new SyScPort(SyIxName("RE"), false, 
            SyScPort::IN, SyScNet::WIRE);
        
        memMod->addPort(re);
        memMod->addObject("RE", re);

        string instName("MEM_" + reg + "_INST");
        instName = ModuleNameManager::instance()->createName(memMod, instName);

        memInst = new SyModInst(instName, memMod, memMod->getName());
        SyMemory *vmem = new SyMemory(reg, data-1, 0, false);
        mod->addObject(vmem->getName(), vmem);
        const SyList<SyScCntv*>& bits = vmem->getBits();

        for(SyNode<SyScCntv*>* node = bits.head; node; node = node->next) {
            mod->addReg(dynamic_cast<SyScReg*>(node->val));
        }    
        SyVPin *vpin = new SyVPin(vport->getName(), data-1, 0, vmem, memInst);
        memInst->addPin(vpin);
        memInst->addVecPin(vpin);
        SyScPin *pin = new SyScPin(SyIxName("CS"), NULL, memInst);
        memInst->addPin(pin);

        pin = new SyScPin(SyIxName("WE"), NULL, memInst);
        memInst->addPin(pin);
        pin = new SyScPin(SyIxName("RE"), NULL, memInst);
        memInst->addPin(pin);

        ModMemElemInstMap::iterator iter = modMemInsts.find(mod->getName());
        if(iter == modMemInsts.end()) {
            map<string, SyModInst*, cmp_str> tmap;
            tmap.insert(make_pair(reg, memInst));
            modMemInsts.insert    (make_pair(
                const_cast<string&>(mod->getName()), tmap));
        }
        else {
            iter->second.insert(make_pair(reg, memInst));
        }
        mod->addModInst(memInst);

        return memInst;
    }    


        
}
