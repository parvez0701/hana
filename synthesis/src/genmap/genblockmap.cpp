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

#include <assert.h>

#include "genblockmap.h"
#include "sblock.h"
#include "sblockinst.h"
#include "sport.h"
#include "spin.h"
#include "snet.h"
#include "sroot.h"

#include "syroot.h"
#include "sycontainer.h"
#include "sutils2.h"

using std::string;
using std::ostringstream;

using namespace stview;

using namespace Synth;

namespace GenericMapping
{
    GenericBlockMapper::GenericBlockMapper(): module(0) {
    }

    GenericBlockMapper::GenericBlockMapper(SBlock *block) :
        GenericMapperBase(block) {
        module = SyRoot::instance()->getModule(block->getName());
        assert(module);
    }

    GenericBlockMapper::~GenericBlockMapper() {
    }

    GenericBlockMapper*
    GenericBlockMapper::create(SBlock *block) const{
        return new GenericBlockMapper(block);
    }    

    void
    GenericBlockMapper::doMapping()
    {
        createPorts();
        createTfPortNets();
        createNets();
    }


    void
    GenericBlockMapper::createPorts()
    {
        assert(module != NULL);

        const SyList<SyScPort*>& ports = module->getPorts();
        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next)
        {
            SyScPort *port = node->val;
            this->getBlock()->addPort(
                new SPort(getMapName(port->getIxName()),
                GetDir(port->getDir())));
        }        
                
    }

    void
    GenericBlockMapper::createTfPortNets()
    {
        assert(module != NULL);
        const SyModule::TfPortTable& tfPortTable = module->getTfPorts();
        for(SyModule::TfPortTable::const_iterator iter = tfPortTable.begin();
            iter != tfPortTable.end(); ++iter)
        {
            const SyScPort *port = dynamic_cast<const SyScPort*>(iter->second);
            if(!port) {
                continue;
            }    
            this->getBlock()->addNet(
                new SNet(getMapName(port->getIxName())));
                
        }        

    }

    void
    GenericBlockMapper::createNets()
    {
        assert(module != NULL);

        const SyList<SyScNet*>& nets = module->getNets();
        for(SyNode<SyScNet*>* node = nets.head; node; node = node->next) {
            this->getBlock()->addNet(new SNet(getMapName(node->val->getIxName())));
        }

        const SyList<SyScReg*>& regs = module->getRegs();
        for(SyNode<SyScReg*>* node = regs.head; node; node = node->next) {
            this->getBlock()->addNet(new SNet(getMapName(node->val->getIxName())));
        }    
    }

    void
    GenericBlockMapper::createBlockInsts()
    {
        assert(module != NULL);
        const SyList<SyModInst*>& modInsts = module->getModInsts();

        for(SyNode<SyModInst*>* node = modInsts.head; node; node = node->next)
        {
            const string& master = node->val->getMasterName();
            
            if(SyRoot::instance()->isCellModule(master)) {
                continue;
            }    
            SBlockInst* binst = new SBlockInst(
                getMapName(node->val->getIxName()), 
                SRoot::instance()->getBlock(node->val->getMasterName()));
            
            this->getBlock()->addBlockInst(binst);
            const SyList<SyScPin*> opins = node->val->getPins();
            for(SyNode<SyScPin*>* opin = opins.head; opin; opin = opin->next){
                binst->addPin(new SPin(
                    this->getMapName(opin->val->getIxName()),
                    this->getMapCntv(opin->val->getActual()), binst));
            }        
        }        
    }
}
