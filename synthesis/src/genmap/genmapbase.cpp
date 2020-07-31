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


#include "genmapbase.h"
#include "sycontainer.h"
#include "syexpr.h"
#include "syroot.h"
#include "namemgr.h"

#include "scell.h"
#include "scellmgr.h"
#include "sinst.h"
#include "spin.h"
#include "sport.h"
#include "sroot.h"
#include "sblock.h"
#include "snet.h"
#include "sutils2.h"


using namespace stview;

using namespace Synth;

using std::list;

namespace GenericMapping
{

    void
    GenericMapperBase::createCellInstances(const InstancePredicate& pred)
    {
        this->setPinMapList();
        const SyModule *mod = SyRoot::instance()->getModule(
            this->getBlock()->getName());
        
        assert(mod);

        const SCell *cell = this->getCell();
        //assert(cell); //TBD
        if(!cell) { return; }
        const SyList<SyModInst*>& minsts = mod->getModInsts();

        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next)
        {
            if(!pred(node->val)) {
                continue;
            }
            this->createCellInstance(node->val, cell);
        }
    }


    
    SInst*
    GenericMapperBase::createCellInstance(    const SyModInst *minst,
                                            const SCell *cell)
    {
        SInst *cinst = new SInst(minst->getIxName().toString(), cell);

        for(PinMapListConstIterator iter = pinMapList.begin(); 
            iter != pinMapList.end(); ++iter)
        {
            SyScPin *opin = minst->getPin(iter->first);
            assert(opin);
            SPin *npin = new SPin(    iter->second,
                                    const_cast<SPN*>(
                                    getMapCntv(opin->getActual())),
                                    cinst );
            cinst->addPin(npin);                        
        }
        block->addInst(cinst);
        return cinst;
    }

    const SPN*
    GenericMapperBase::getMapCntv(const SyExpr *expr) const
    {
        if(!expr) {
            return 0;
        }    
        const SyScCntv *ocntv = dynamic_cast<const SyScCntv*>(expr);
        assert(ocntv);
        const SPN *ncntv = dynamic_cast<const SPN*>(
            block->findObject(getMapName(ocntv->getIxName())));
        assert(ncntv);
        return ncntv;
    }


    SNet*
    GenericMapperBase::createNet(const string& hint)
    {
        string name = BlockNameManager::instance()->createName(block, hint);
        SNet *net = new SNet(name);
        block->addNet(net);
        return net;
    }

    SInst*
    GenericMapperBase::createInverter(
        const string& hint,
        const SPN *in,
        const SPN *out)
    {
        string name = BlockNameManager::instance()->createName(block, hint);
        const SCell *inv = SCellMgr::instance()->getGenericCell(SCell::NOT, 1);
        assert(inv);
        SInst *inst = new SInst(name, inv);
        SPin *inpin = new SPin("in", in, inst);
        SPin *outpin = new SPin("out", out ? out : createNet(), inst);

        inst->addPin(inpin);
        inst->addPin(outpin);
        block->addInst(inst);
        return inst;
    }
        
    SInst*
    GenericMapperBase::createBuffer(
        const string& hint,
        const SPN *in,
        const SPN *out)
    {
        string name = BlockNameManager::instance()->createName(block, hint);
        const SCell *buf = SCellMgr::instance()->getGenericCell(SCell::BUF, 1);
        assert(buf);
        SInst *inst = new SInst(name, buf);
        SPin *inpin = new SPin("in", in, inst);
        SPin *outpin = new SPin("out", out ? out : createNet(), inst);

        inst->addPin(inpin);
        inst->addPin(outpin);
        block->addInst(inst);
        return inst;
    }

    SInst*
    GenericMapperBase::create2InputGate(const string& hint,
        SCell::CellType type,
        const SPN *in1,
        const SPN *in2,
        const SPN *out)
    {
        string name = BlockNameManager::instance()->createName(block, hint);
        const SCell *cell = SCellMgr::instance()->getGenericCell(type, 2);

        assert(cell);
        SInst *inst = new SInst(name, cell);
        const list<SPort*>& ports = cell->getPorts();
        list<SPort*>::const_iterator iter = ports.begin();

        bool first = true;
        for(; iter != ports.end(); ++iter)
        {
            SPin *pin = 0;
            SPort *port = *iter;
            if(port->getDir() == SPort::OUT) {
                pin = new SPin(port->getName(), out, inst);
            }
            else if(first) 
            {
                first = false;
                pin = new SPin(port->getName(), in1, inst);
            }
            else {
                pin = new SPin(port->getName(), in2, inst);
            }
            inst->addPin(pin);
        }    
        block->addInst(inst);
        return inst;

    }

    void
    GenericMapperBase::create2InputGateTree(
        const string& hint,
        SCell::CellType type,
        const vector<SPN*>& inputs,
        const SPN *out)
    {
        assert(inputs.size() > 1);

        SInst *inst = this->create2InputGate(hint, type,
            inputs[0], inputs[1], 0);
        SPin *outpin = const_cast<SPin*>(inst->getPin("out"));
        if(inputs.size() == 2) 
        {
            outpin->setActual(out);
            return;
        }    
        SNet *net = this->createNet();
        outpin->setActual(net);
        for(int i = 2; i < inputs.size(); ++i)
        {
            if(i == (inputs.size()-1)) {
                inst = this->create2InputGate(hint, type,
                    net, inputs[i], out);
            }
            else
            {
                inst = this->create2InputGate(hint, type,
                    net, inputs[i], 0);
                net = this->createNet();    
                outpin = const_cast<SPin*>(inst->getPin("out"));
                outpin->setActual(net);
            }
            
        }
    }
}
