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

#include "module.h"
#include "omvisitor.h"
#include "net.h"
#include "param.h"
#include "vecnet.h"
#include "vecport.h"
#include "vecparam.h"
#include "localparam.h"
#include "taskfunc.h"
#include "udp.h"
#include "udpinst.h"
#include "generate.h"
#include "contassign.h"
#include "always.h"
#include "initial.h"
#include "specify.h"
#include "defparam.h"
#include "event.h"

namespace Rtl
{
    void
    Module::accept(OMVisitorBase* visitor) const{
        visitor->visit(this);
    }
    void 
    Module::addParams(const std::vector<Param*>& p)
    {
        const int size = p.size();
        for(int i = 0; i < size; ++i)
        {
            p[i]->setScope(this);
            if(!p[i]->isVecBit()){
                this->addObject(p[i]->getIxName().getName(), 
                p[i], false);
            }
            params.push_back(p[i]);
        }
    }

    void 
    Module::addLocalParams(const std::vector<LocalParam*>& lp)
    {
        const int size = lp.size();
        for(int i = 0; i < size; ++i)
        {
            lp[i]->setScope(this);
            if(!lp[i]->isVecBit()){
                this->addObject(lp[i]->getIxName().getName(), 
                lp[i], false);
            }
            lparams.push_back(lp[i]);
        }
    }
    void
    Module::addNets(const std::vector<Net*>& n)
    {
        const int size = n.size();
        for(int i = 0; i < size; ++i)
        {
            if(!n[i]->isVecBit()){
                this->addObject(n[i]->getIxName().getName(), 
                n[i], false);
            }
            n[i]->setScope(this);
            nets.push_back(n[i]);
        }
    }

    void
    Module::addPorts(const std::vector<Port*>& p)
    {
        const int size = p.size();
        for(int i = 0; i < size; ++i)
        {
            if(!p[i]->isVecBit()){
                this->addObject(p[i]->getIxName().getName(), 
                p[i], false);
            }
            p[i]->setScope(this);
            this->addPort(p[i]);
            //ports.push_back(p[i]);
        }
    }

    void
    Module::addTask(const Task* t)
    {
        if(!t){
            return;
        }
        tasks.push_back(const_cast<Task*>(t));
        this->addObject(t->getName(), t, false);
    }
    void
    Module::addFunc(const Func* f)
    {
        if(!f){
            return;
        }
        funcs.push_back(const_cast<Func*>(f));
        this->addObject(f->getName(), f, false);
    }
    
    void
    Module::addAlways(const Always* a){
        if(a){
            alwaysBlocks.push_back(const_cast<Always*>(a));
        }
    }
    void
    Module::addInitial(const Initial* i){
        if(i){
            initials.push_back(const_cast<Initial*>(i));
        }
    }

    void
    Module::addDefparam(const Defparam* d){
        if(d){
            defparams.push_back(const_cast<Defparam*>(d));
        }
    }

    void
    Module::addEvent(const EventStmt* es){
        if(es){
            events.push_back(const_cast<EventStmt*>(es));
        }
    }

    void
    Module::addEventVar(const EventVar* v) {
        if(v)
        {
            addObject(v->getName(), v, true);
            eventDecls.push_back(const_cast<EventVar*>(v));
        }
    }    

    void
    Module::addSpecifyBlock(const SpecifyBlock* specify){
        if(specify){
            specifyBlocks.push_back(const_cast<SpecifyBlock*>(specify));
        }
    }
    

    void
    Module::addUDPInst(const UDPInst* uinst){
        if(uinst)
        {
            uinsts.push_back(const_cast<UDPInst*>(uinst));
            this->addObject(uinst->getName().getName(),
                    uinst, false);
        }
    }

    void
    Module::addGenerate(const GenerateStmt* gs){
        if(gs){
            gstmts.push_back(const_cast<GenerateStmt*>(gs));
        }
    }
    void
    Module::addGenvar(const Genvar* g){
        if(g){
            genvars.push_back(const_cast<Genvar*>(g));
        }
    }
    const Base*
    Module::getPort(const IxName& ixname) const
    {
        if(ixname.getIndex() == IxName::nix){
            return this->getObject(ixname.getName());
        }

        const int size = ports.size();
        for(int i = 0; i < size; ++i){
            if(ports[i]->getIxName() == ixname){
                return ports[i];
            }
        }
        return NULL;
    }


    vector<Net*>
    Module::getScalarNets() const {
        return GetVSObjects<Net, Net>(this, nets);
    }

    vector<VecNet*>
    Module::getVectorNets() const {
        return GetVSObjects<Net, VecNet>(this, nets);
    }

    vector<Port*>
    Module::getScalarPorts() const {
        return GetVSObjects<Port, Port>(this, ports);
    }

    vector<VecPort*>
    Module::getVectorPorts() const {
        return GetVSObjects<Port, VecPort>(this, ports);
    }

    vector<Param*>
    Module::getScalarParams() const {
        return GetVSObjects<Param, Param>(this, params);
    }

    vector<VecParam*>
    Module::getVectorParams() const {
        return GetVSObjects<Param, VecParam>(this, params);
    }

    void
    Module::addStmt(const Stmt *stmt)
    {
        if(const ContAssign *ca = dynamic_cast<const ContAssign*>(stmt)) {
            contAssigns.push_back(const_cast<ContAssign*>(ca));
        }
        else if(const Always *always = dynamic_cast<const Always*>(stmt)) {
            alwaysBlocks.push_back(const_cast<Always*>(always));
        }
        else if(const Initial *init = dynamic_cast<const Initial*>(stmt)) {
            initials.push_back(const_cast<Initial*>(init));
        }
        else if(const SpecifyBlock *sb = 
            dynamic_cast<const SpecifyBlock*>(stmt)) {
            specifyBlocks.push_back(const_cast<SpecifyBlock*>(sb));
        }
        else if(const Func *func = dynamic_cast<const Func*>(stmt)) {
            funcs.push_back(const_cast<Func*>(func));
        }
        else if(const Task *task = dynamic_cast<const Task*>(stmt)) {
            tasks.push_back(const_cast<Task*>(task));
        }
        else if(const Defparam *dp = dynamic_cast<const Defparam*>(stmt)) {
            defparams.push_back(const_cast<Defparam*>(dp));
        }
        else if(const EventStmt* event = dynamic_cast<const EventStmt*>(stmt)){
            events.push_back(const_cast<EventStmt*>(event));
        }
        else if(const GenerateStmt *gs = dynamic_cast<const GenerateStmt*>(stmt)){
            gstmts.push_back(const_cast<GenerateStmt*>(gs));
        }
        else {
            Scope::addStmt(stmt);
        }    
    }

    void
    Module::addPort(Port *p)
    {
        if(portOrder.empty()) 
        {
            ports.push_back(p);
            return;
        }
        int loc = 0;
        for(; loc < portOrder.size(); ++loc) 
        {
            if(portOrder[loc] == p->getIxName().getName()) {
                break;
            }
        }
        ++loc;
        if(loc >= portOrder.size()) 
        {
            ports.push_back(p);
            return;
        }    
        string after(portOrder[loc]);

        vector<Port*>::iterator iter = ports.begin();
        for(; iter != ports.end(); ++iter) {
            if((*iter)->getIxName().getName() == after) {
                break;
            }
        }
        ports.insert(iter, p);
    }

}
