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

#include "sycontainer.h"
#include "sycntvs.h"
#include "syconst.h"
#include "syvisitor.h"
#include "tvisitor.h"

namespace Synth
{

    SyList<SyCntv*>*
        SyScope::getMixPorts() const
        {
            SyList<SyCntv*> *mixPorts = new SyList<SyCntv*>();
            string pname;
            for(SyNode<SyScPort*>* node = ports.head; node; node = node->next)
            {
                if(node->val->isVecBit())
                {
                    if(!pname.empty() && 
                        (pname == node->val->getIxName().getName())) {
                            continue;
                    }
                    pname = node->val->getIxName().getName();
                    const SyVPort *vport = dynamic_cast<const SyVPort*>(
                        this->findObject(pname));
                    assert(vport);
                    mixPorts->push_back(const_cast<SyVPort*>(vport));
                }
                else {
                    mixPorts->push_back(node->val);
                }
            }
            return mixPorts;
        }

    const SyBase*
        SyScope::findObject(const std::string& n) const
        {
            std::map<std::string, SyBase*, cmp_str>::const_iterator iter =
                symbolTable.find(n);
            if(iter == symbolTable.end()){
                return 0;
            }
            return iter->second;
        }

    const SyBase*
        SyScope::findObject(const SyIxName& n) const {
            return this->findObject(n.toString());
        }    

    void
        SyScope::resetCntvs()
        {
            ResetCntvs<SyScPort, SyVPort>(this->getName(),
                    this->getPorts());
            ResetCntvs<SyScReg, SyVReg>(this->getName(),
                    this->getRegs());
        }
    void
        SyModule::resetCntvs()
        {
            SyScope::resetCntvs();
            ResetCntvs<SyScNet, SyVNet>(this->getName(), nets);
        }

    bool
        SyModule::connect(
                SyScCntv *c1,
                SyCntv *c2,
                string hint,
                const bool force)
        {
            const SyScCntv *sccntv = dynamic_cast<const SyScCntv*>(
                            this->findObject(c1->getIxName().toString()));
            if(!sccntv) {
                return false;
            }
            if(SyScCntv *sccntv = dynamic_cast<SyScCntv*>(c2)){
                if(!dynamic_cast<const SyScCntv*>(
                            this->findObject(sccntv->getIxName().toString()))){
                    return false;
                }
            }
            string newname = CreateUniqueName(this, hint);
            if(SyScPin *p1 = dynamic_cast<SyScPin*>(c1))
            {
                const SyExpr *act1 = p1->getActual();
                if(SyScPin *p2 = dynamic_cast<SyScPin*>(c2))
                {
                    const SyExpr *act2 = p2->getActual();
                    if(act1 && act2) {
                        return false;
                    }
                    if(act1) {
                        p2->setActual(act1);
                    }
                    else if(act2) {
                        p1->setActual(act2);
                    }
                    else
                    {
                        SyScNet *net = new SyScNet( newname, SyScNet::WIRE,
                                false);
                        this->addNet(net);
                        this->addObject(newname, net);
                        p1->setActual(net);
                        p2->setActual(net);
                    }
                    return true;
                }
                else
                {
                    if(!act1 || (act1 && force))
                    {
                        if(SyConst *const_ = dynamic_cast<SyConst*>(c2))
                        {
                            SyConst cbit = (*const_)[const_->size() - 1];
                            p1->setActual(cbit.copy());
                            return true;
                        }
                        else if(SyScCntv *sccntv = dynamic_cast<SyScCntv*>(
                                    c2)){
                            p1->setActual(sccntv);
                            return true;
                        }
                        return false;
                    }
                }
            }
            else if(SyScPin *p2 = dynamic_cast<SyScPin*>(c2))
            {
                const SyExpr *act2 = p2->getActual();
                if(!act2 || (act2 && force))
                {
                    if(SyConst *const_ = dynamic_cast<SyConst*>(c1))
                    {
                        SyConst cbit = (*const_)[const_->size() - 1];
                        p2->setActual(cbit.copy());
                        return true;
                    }
                    else if(SyScCntv *sccntv = dynamic_cast<SyScCntv*>(c1)){
                        p1->setActual(sccntv);
                        return true;
                    }
                    return false;
                }
            }
            return false;
        }

    void
        SyModule::addTfPort(const string& name, const SyCntv *port)
        {
            tfPortTable.insert(make_pair(name, port));
        }
    const SyCntv*
        SyModule::getTfPort(const string& name) const
        {
            TfPortTable::const_iterator iter = tfPortTable.find(name);
            if(iter == tfPortTable.end()) {
                return 0;
            }
            return iter->second;
        }

    const SyVPort*
        SyModule::getTfVPort(const string &name) const {
            return dynamic_cast<const SyVPort*>(this->getTfPort(name));
        }
    
    const SyScPort*
        SyModule::getTfSPort(const string& name) const {
            return dynamic_cast<const SyScPort*>(this->getTfSPort(name));
        }    
    const SyModule::TfPortTable&
        SyModule::getTfPorts() const {
            return tfPortTable;
        }    

    const SyBase*
        SyModule::findObject(const string& n) const
        {
            const SyCntv *obj = this->getTfPort(n);
            if(obj) {
                return obj;
            }
            return SyScope::findObject(n);
        }

    const SyBase*
        SyModule::findObject(const SyIxName& n) const {
            return this->findObject(n.toString());
        }    
        

    void
        SyBModule::resetCntvs()
        {
            SyModule::resetCntvs();
            for(SyNode<SyTask*>* node = tasks.head; node; node = node->next){
                ResetCntvs<SyScPort, SyVPort>(this->getName(),
                        node->val->getPorts(), node->val->getName());
            }
            for(SyNode<SyFunc*>* node = funcs.head; node; node = node->next){
                ResetCntvs<SyScPort, SyVPort>(this->getName(),
                        node->val->getPorts(), node->val->getName());
            }
        }

    void
        SyModule::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyModule::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }

    void
        SyBModule::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyBModule::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }

    void
        SyPrimInst::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    void
        SyPrimInst::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }

    void
        SyModInst::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    void
        SyModInst::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }

    void
        SyTask::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    void
        SyTask::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyFunc::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    void
        SyFunc::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    template<class T1, class T2> void ResetCntvs(
            const std::string& modname,
            const SyList<T1*>& cntvlist,
            const string& tfname)
    {
        SyList<T1*>& tlist = const_cast<SyList<T1*>&>(cntvlist);
        for(SyNode<T1*>* node = tlist.head; node; node = node->next)
        {
            if(node->val->isVecBit()){
                //const T2 *t2 = dynamic_cast<const T2*>(
                //        node->val->getVecCntv(modname));
                const T2 *t2 = GetVecCntv<T1, T2>(
                    node->val->getIxName().getName(),modname, tfname); 
                vector<SyExpr*>& val = const_cast<vector<SyExpr*>&>(
                        t2->getVal());
                for(unsigned int i = 0; i < val.size(); ++i){
                    val[i] = 0;
                }
            }
            vector<SyExpr*>& val = const_cast<vector<SyExpr*>&>(
                    node->val->getVal());
            for(unsigned int i = 0; i < val.size(); ++i){
                val[i] = 0;
            }
        }
    }

}
