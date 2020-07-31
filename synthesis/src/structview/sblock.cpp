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

#include "sblock.h"
#include "sport.h"
#include "snet.h"
#include "sinst.h"
#include "sblockinst.h"
#include "netlistvisitorbase.h"
#include "namemgr.h"

using std::map;
using std::list;
using std::vector;
using std::string;
using std::make_pair;

namespace stview
{
    SBlock::SBlock(const string& name): SBase(name) {
    }

    SBlock::~SBlock() {
    }

    const SPort*
    SBlock::getPort(const string& pname) const
    {
        PortSymTable::const_iterator iter =
            portSymTable.find(pname);
        if(iter == portSymTable.end()){
            return 0;
        }

        return iter->second;
    }

    const SNet*
    SBlock::getNet(const string& nname) const
    {
        NetSymTable::const_iterator iter =
            netSymTable.find(nname);
        if(iter == netSymTable.end()){
            return 0;
        }
        return iter->second;

    }

    const SInst*
    SBlock::getInst(const string& iname) const
    {
        CellSymTable::const_iterator iter =
            cellSymTable.find(iname);
        if(iter == cellSymTable.end()){
            return 0;
        }

        return iter->second;
    }

    const SBlockInst*
    SBlock::getBlockInst(const string& iname) const
    {
        BlockSymTable::const_iterator iter =
            blockSymTable.find(iname);
        if(iter == blockSymTable.end()){
            return 0;
        }

        return iter->second;
    }

    const SBase*
    SBlock::findObject(const string& name) const
    {
        const SBase *obj = NULL;
        if(obj = getPort(name)) {
            return obj;
        }

        if(obj = getNet(name)) {
            return obj;
        }

        if(obj = getInst(name)) {
            return obj;
        }

        return getBlockInst(name);
    }
    void
    SBlock::addPort(const SPort *port)
    {
        if(!port) {
            return;
        }
        switch(port->getDir())
        {
            case SPort::IN:
                inports.push_back(const_cast<SPort*>(port));
                break;
            case SPort::OUT:
                outports.push_back(const_cast<SPort*>(port));
                break;
            case SPort::INOUT:
                inoutports.push_back(const_cast<SPort*>(port));
                break;
            default:
                break;
        }
        portSymTable.insert(make_pair(port->getName(), 
            const_cast<SPort*>(port)));
    }

    void
    SBlock::addNet(const SNet *net)
    {
        if(!net) {
            return;
        }
        nets.push_back(const_cast<SNet*>(net));
        netSymTable.insert(make_pair(net->getName(), const_cast<SNet*>(net)));
    }


    void
    SBlock::addInst(const SInst *inst)
    {
        if(!inst) {
            return;
        }
        insts.push_back(const_cast<SInst*>(inst));
        cellSymTable.insert(make_pair(inst->getName(), const_cast<SInst*>(inst)));
    }
    void
    SBlock::addBlockInst(const SBlockInst *inst)
    {
        if(!inst) {
            return;
        }
        blockinsts.push_back(const_cast<SBlockInst*>(inst));
        blockSymTable.insert(make_pair(inst->getName(), const_cast<SBlockInst*>(inst)));
    }

	void
	SBlock::addAssign(const SAssign *assign) {
	    if(assign) {
		    assigns.push_back(const_cast<SAssign*>(assign));
		}
	}	

    void
    SBlock::accept(NetlistVisitorBase* nvb) {
        nvb->visit(this);
    }    

    void
    SBlock::deleteCellInst(SInst *sinst)
    {
        if(!sinst) {
            return;
        }    
        insts.remove(sinst);
        const string& name = sinst->getName();
        //std::cout << "Removed instance " << name << std::endl;

        blockSymTable.erase(name);
        cellSymTable.erase(name);
        delete sinst;
    }

    void
    SBlock::deleteNet(SNet *net)
    {
        if(!net) {
            return;
        }    
        //remove(nets.begin(), nets.end(), net);
        nets.remove(net);
        const string& name = net->getName();

        netSymTable.erase(name);
        blockSymTable.erase(name);
        delete net;
    }

    bool
    SBlock::connect(SPin *p1, SPin *p2)
    {
        if(!p1 || !p2) {
            return false;
        }    

        const SPN *spn1 = p1->getActual();
        const SPN *spn2 = p2->getActual();

        if(spn1 && spn2) {
            //which one to use ?
            return false;
        }

        if(spn1) 
        {
            p2->setActual(spn1);
            return true;
        }    
        if(spn2) 
        {
            p1->setActual(spn2);
            return true;
        }    
        string name = Synth::BlockNameManager::instance()->createName(this, "net");
        SNet *net = new SNet(name);
        this->addNet(net);

        p1->setActual(net);
        p2->setActual(net);
        return true;
    }

    void
    SBlock::resetVisitCount()
    {
        ResetVisitCountPort()(inports);
        ResetVisitCountPort()(outports);
        ResetVisitCountPort()(inoutports);

        ResetVisitCountNet()(nets);

        ResetVisitCountInst()(insts);
        ResetVisitCountBlockInst()(blockinsts);
    }
}
