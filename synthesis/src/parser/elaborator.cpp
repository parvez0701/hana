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

#include <sstream>
#include <list>
#include <stack>

#include <assert.h>
#include "elaborator.h"
#include "module.h"
#include "modinst.h"
#include "param.h"
#include "vecparam.h"
#include "vecport.h"
#include "scope.h"
#include "vecnet.h"
#include "pin.h"
#include "vecpin.h"
#include "defparam.h"
#include "hierid.h"
#include "localparam.h"
#include "veclocalparam.h"
#include "unaryexpr.h"
#include "binexpr.h"
#include "condexpr.h"
#include "partsel.h"
#include "multiconcat.h"
#include "taskfunc.h"
#include "always.h"
#include "block.h"
#include "case.h"
#include "compassign.h"
#include "dassign.h"
#include "delay.h"
#include "disable.h"
#include "force.h"
#include "initial.h"
#include "loopstmts.h"
#include "namedblock.h"
#include "reg.h"
#include "vecreg.h"
#include "integer.h"
#include "vecint.h"
#include "vtime.h"
#include "vectime.h"
#include "real.h"
#include "vecreal.h"
#include "realtime.h"
#include "vecrealtime.h"
#include "namedblock.h"
#include "fork.h"
#include "specify.h"
#include "trigger.h"
#include "gateinst.h"
#include "modinst.h"
#include "udpinst.h"
#include "contassign.h"
#include "proccontassign.h"
#include "procassign.h"
#include "generate.h"
#include "num.h"

#include "msghandler.h"
using std::map;
using std::string;
using std::vector;
using std::list;
using std::ostringstream;
using std::make_pair;
using std::pair;
using std::stack;

namespace Rtl
{
    
    extern Root* GRoot;


    template<class T> const T*
    GetObject(const string& name, const T *oldObj, const Module *newmod)
    {
        stack<string> scopes;
        const Scope *sc = oldObj->getScope();

        while(sc)
        {
            scopes.push(sc->getName());
            sc = sc->getScope();
        }
        //pop off old module name
        if(!scopes.empty()) {
            scopes.pop();
        }    
        sc = newmod;
        while(!scopes.empty())
        {
            sc = dynamic_cast<const Scope*>(sc->getObject(scopes.top()));
            assert(sc);
            scopes.pop();
        }
        assert(sc);
        return dynamic_cast<const T*>(sc->getObject(name));
    }


    Elaborator* Elaborator::elaborator = 0;

    Elaborator::Elaborator(){
    }

    Elaborator::~Elaborator(){
        elabInfos.clear();
        elaborator = 0;
    }

    Elaborator*
        Elaborator::instance(){
            if(!elaborator){
                elaborator = new Elaborator();
            }
            return elaborator;
        }

    Expr*
        Elaborator::getElabExpr(const Expr* expr) const
        {
            map<Expr*, Expr*>::const_iterator iter = oldnewexprs.find(
                    const_cast<Expr*>(expr));
            if(iter == oldnewexprs.end()){
                return 0;
            }
            return iter->second;
        }


    void
        Elaborator::addElabExpr(const Expr* old, const Expr* new_) {
            oldnewexprs.insert(make_pair(const_cast<Expr*>(old),
                        const_cast<Expr*>(new_)));
        }

    void
        Elaborator::populateDefparamInfo()
        {
            const vector<Module*>& topmods = GRoot->getTopModules();
            for(unsigned int i = 0; i < topmods.size(); ++i){
                this->populateDefparamInfo(topmods[i], 
                    Path(topmods[i]->getName()));
            }
        }

    void
        Elaborator::populateDefparamInfo(
                const Module* mod,
                const Path& path)
        {
            const vector<Defparam*>& defparams = mod->getDefparams();
            for(unsigned int k = 0; k < defparams.size(); ++k)
            {
                Defparam* defparam = defparams[k];
                const HierId* hid = dynamic_cast<const HierId*>(
                        defparam->getLhs());
                Path dpath = hid ? hid->getResolvedPath() : path + Path(defparam->getLhs()->toString());
                string param = dpath.ascend();
                DefparamInfo::iterator iter = defparamInfo.find(dpath);
                if(iter != defparamInfo.end()){
                    iter->second.insert(make_pair( param, 
                                const_cast<Expr*>(defparam->getRhs())));
                }
                else
                {
                    map<string, Expr*, compare_str> tmap;
                    tmap.insert(make_pair(param,
                                const_cast<Expr*>(defparam->getRhs())));
                    defparamInfo.insert(make_pair(dpath, tmap));
                }
            }

            const list<ModInst*>& minsts = mod->getModInsts();
            for(list<ModInst*>::const_iterator iter = minsts.begin();
                iter != minsts.end(); ++iter)
            {
                const ModInst* minst = *iter;
                const Module* master = minst->getMaster();
                if(!master){
                    continue;
                }
                Path newpath(path);
                newpath.descend(minst->getName().toString());
                this->populateDefparamInfo(master, newpath);
            }
        }



    /* Function: getElabModName
       * Returns name for the module to be elaborated with respect to
       * the instances.
       * Returns null string if elaboration is not required.
    */
    string
        Elaborator::getElabModName(
                const Module* mod,
                const ModInst* inst,
                const vector<NameConn*>& defparams
                ) const
        {
            if(!mod || !inst){
                return string();
            }

            //const vector<NameConn*>& instparams = inst->getParams();
            string name = mod->getName();
            name = name + string("___");

            map<string, int, compare_str> paramvals;

            //We are not supporting range in defparams
            int size = defparams.size();

            for(int i = 0; i < size; ++i){
                paramvals.insert(make_pair(defparams[i]->first,
                            defparams[i]->second->evaluate()));
            }

            /*size = instparams.size();
            for(int i = 0; i < size; ++i){
                paramvals.insert(make_pair(instparams[i]->first,
                            instparams[i]->second->evaluate()));
            }
            */

            ostringstream str;
            map<string, int>::iterator iter = paramvals.begin();
            map<string, int>::iterator eiter = paramvals.end();
            str << mod->getName() << "___";
            for(; iter != eiter; ++iter){
                str << iter->first << iter->second;
            }
            return str.str();
        }

    bool
        Elaborator::elaborationRequired(
                const Module* mod,
                const ModInst* inst,
                vector<NameConn*>& defparams,
                const Path& path
                )const
        {
            bool status = false;

            const vector<NameConn*>& instparams = inst->getParams();
            map<string, pair<Expr*, bool>, compare_str> paramvals;

            //We are not supporting range in defparams



            vector<Param*> sparams = mod->getScalarParams();
            int size = sparams.size();
            for(int i = 0; i < size; ++i){
                paramvals.insert(
                        make_pair(sparams[i]->getIxName().getName(),
                            make_pair(const_cast<Expr*>(sparams[i]->getVal()), 
                                false)));
            }
            vector<VecParam*> vparams = mod->getVectorParams();
            size = vparams.size();
            for(int i = 0; i < size; ++i){
                paramvals.insert(
                        make_pair(vparams[i]->getName(),
                            make_pair(const_cast<Expr*>(vparams[i]->getVal()), 
                                false)));
            }
            size = instparams.size();
            for(int i = 0; i < size; ++i){
                map<string, pair<Expr*, bool>, compare_str>::iterator iter = 
                    paramvals.find(instparams[i]->first);
                if(instparams[i]->second && (instparams[i]->second->evaluate() != 
                        iter->second.first->evaluate()))
                {
                    status = true;
                    iter->second = make_pair(instparams[i]->second, true);
                }
            }
            //Instance parameter override values have precedence over
            //defparams.
            DefparamInfo::const_iterator iter = defparamInfo.find(path);
            if(iter != defparamInfo.end())
            {
                map<string, Expr*, compare_str>::const_iterator titer = 
                    iter->second.begin();
                map<string, Expr*, compare_str>::const_iterator etiter = 
                    iter->second.end();
                for(; titer != etiter; ++titer)
                {
                    map<string, pair<Expr*, bool>, compare_str>::iterator 
                        ttiter = paramvals.find(titer->first);
                    if(ttiter->second.second == false){
                        if(titer->second->evaluate() != 
                                ttiter->second.first->evaluate()){
                            status = true;
                        }
                        ttiter->second.first = titer->second;
                    }
                }
            }

            map<string, pair<Expr*, bool>, compare_str>::iterator pvaliter = 
                paramvals.begin();
            map<string, pair<Expr*, bool>, compare_str>::iterator epvaliter = 
                paramvals.end();

            for(; pvaliter != epvaliter; ++pvaliter){
                NameConn* nc = new NameConn();
                *nc = make_pair(pvaliter->first, pvaliter->second.first);
                defparams.push_back(nc);
            }
            return status;
        }

    Module*
        Elaborator::getElabModule(
                const Module* mod,
                const ModInst* inst,
                const std::vector<NameConn*>& defparams 
                )const 
        {
            string modname = this->getElabModName(mod, inst, defparams);
            Module* newmod = new Module(modname);
            map<string, Expr*, compare_str> defparammap;
            int size = defparams.size();
            for(int i = 0; i < size; ++i){
                defparammap.insert(*(defparams[i]));
            }

            const vector<Param*>& params = mod->getParams();
            vector<string> paramnames = GetNames<Param>(params);
            size = paramnames.size();
            vector<Param*> newparams;
            for(int i = 0; i < size; ++i){
                Expr* val = 0;
                map<string, Expr*, compare_str>::iterator iter =
                    defparammap.find(paramnames[i]);

                if(iter != defparammap.end()){
                    val = new Num(iter->second->evaluate());
                    //val = iter->second->copy();
                }
                const VecParam* origvparam = dynamic_cast<const VecParam*>(
                        mod->getObject(paramnames[i]));
                const Param* origparam = dynamic_cast<const Param*>(
                        mod->getObject(paramnames[i]));
                if(!val){
                    if(origparam){
                        val = new Num(origparam->evaluate());
                        //val = origparam->getVal()->copy();
                    }
                    else if(origvparam){
                        val = new Num(origvparam->evaluate());
                        //val = origvparam->getVal()->copy();
                    }
                }
                assert(val);

                if(origparam){
                    //scalar param
                    Param* newparam = new Param(paramnames[i], val, 
                            origparam->getVarType());
                    newparam->setSign(origparam->isSigned());
                    newparams.push_back(newparam);

                }
                else {
                    //vector param
                    VecParam* vparam = new VecParam(paramnames[i],
                            origvparam->getDim()->copy(), 
                            origvparam->isSigned());
                    vparam->setVal(val);
                    const vector<Param*> tparams = vparam->getParams();
                    for(unsigned int j = 0; j < tparams.size(); ++j){
                        newparams.push_back(tparams[i]);
                    }
                    newmod->addObject(vparam->getName(), vparam,
                            true);
                }
            }
            newmod->addParams(newparams);
            return newmod;
        }

    const Expr*
        Elaborator::getReplacementParam(
                const Module* oldmod,
                const Module* newmod,
                const Expr* oldexpr) 
        {
            if(!oldexpr){
                return 0;
            }
            const Param* oldparam = dynamic_cast<const Param*>(oldexpr);
            const VecParam* oldvparam = dynamic_cast<const VecParam*>(
                    oldexpr);
            if(!oldparam && !oldvparam){
                //find and return a corresponding expr
                string exprstring = oldexpr->toString();
                map<string, Expr*, compare_str>::const_iterator iter =
                    newmodexprs.find(exprstring);
                if(iter != newmodexprs.end()){
                    return iter->second;
                }
                Expr* newexpr = oldexpr->copy();
                newmodexprs.insert(make_pair(newexpr->toString(), newexpr));
                return newexpr;
            }
            if(oldvparam){
                const VecParam* newvparam = dynamic_cast<const VecParam*>(
                        newmod->getObject(oldvparam->getName()));
                return newvparam;
            }

            if(oldparam){
                if(oldparam->isVecBit()){
                    const VecParam* vparam = dynamic_cast<const VecParam*>(
                            newmod->getObject(oldparam->getIxName().getName()));
                    return vparam->getBit(oldparam->getIxName().getIndex());
                }
                else {
                    const Param* param = dynamic_cast<const Param*>(
                            newmod->getObject(oldparam->getIxName().getName()));
                    return param;
                }
            }
            return 0; //should not reach
        }

    Dimension*
        Elaborator::createReplacementDim(
                const Module* oldmod,
                Module* newmod,
                const Dimension* dim
                ) 
        {
            Dimension* newdim = new Dimension();
            int size = dim->packDim.size();
            for(int i = 0; i < size; ++i)
            {
                Range* range = new Range(this->replaceParamInExpr(
                            oldmod, newmod, dim->packDim[i]->getLExpr(),
                            dim->packDim[i]->getLExpr()->copy()), 
                        this->replaceParamInExpr(oldmod, newmod,
                            dim->packDim[i]->getRExpr(),
                            dim->packDim[i]->getRExpr()->copy()));
                newdim->packDim.push_back(range);
            }

            size = dim->unpackDim.size();
            for(int i = 0; i < size; ++i)
            {
                Range* range = new Range(this->replaceParamInExpr(
                            oldmod, newmod, 
                            dim->unpackDim[i]->getLExpr(),
                            dim->unpackDim[i]->getLExpr()->copy()), 
                        this->replaceParamInExpr(oldmod, newmod,
                            dim->unpackDim[i]->getRExpr(),
                            dim->unpackDim[i]->getRExpr()->copy()));
                newdim->unpackDim.push_back(range);
            }
            return newdim;
        }





    void
        Elaborator::addPorts(
                const Module* oldmod,
                Module* newmod
                )
        {
            vector<Port*> newports;
            vector<Port*> scalarports = oldmod->getScalarPorts();
            int size = scalarports.size();
            for(int i = 0; i < size; ++i)
            {
                Port* oldport = scalarports[i];
                Port* newport = new Port(oldport->getIxName(),
                        oldport->getDir());
                newport->setVarType(oldport->getVarType());
                newport->setSign(oldport->isSigned());
                newports.push_back(newport);
            }

            vector<VecPort*> vectorports = oldmod->getVectorPorts();
            size = vectorports.size();
            for(int i = 0; i < size; ++i)
            {
                VecPort* oldvecport = vectorports[i];
                //const Expr* oldinit = oldvecport->getVal();
                VecPort* newvecport = new VecPort(
                        oldvecport->getName(),
                        this->createReplacementDim(oldmod,
                            newmod, oldvecport->getDim()),
                        oldvecport->getDir());
                newvecport->setVarType(oldvecport->getVarType());
                newvecport->setVal(this->getReplacementParam(oldmod, newmod,
                            oldvecport->getVal()));

                const vector<Port*>& ports = newvecport->getPorts();
                for(unsigned int j = 0; j < ports.size(); ++j){
                    newports.push_back(ports[j]);
                }
                newmod->addObject(newvecport->getName(), newvecport, true);
            }

            newmod->addPorts(newports);
        }

         void
        Elaborator::addPorts(
                const Module* oldmod,
                Module* newmod,
                const TaskOrFunc *oldtf,
                TaskOrFunc *newtf
                )
        {
            vector<Port*> newports;
            vector<Port*> scalarports = GetVSObjects<Port, Port>(
                oldtf, oldtf->getPorts());
            int size = scalarports.size();
            for(int i = 0; i < size; ++i)
            {
                Port* oldport = scalarports[i];
                Port* newport = new Port(oldport->getIxName(),
                        oldport->getDir());
                newport->setVarType(oldport->getVarType());
                newport->setSign(oldport->isSigned());
                newports.push_back(newport);
            }

            vector<VecPort*> vectorports = GetVSObjects<Port, VecPort>(
                oldtf, oldtf->getPorts());
            size = vectorports.size();
            for(int i = 0; i < size; ++i)
            {
                VecPort* oldvecport = vectorports[i];
                //const Expr* oldinit = oldvecport->getVal();
                VecPort* newvecport = new VecPort(
                        oldvecport->getName(),
                        this->createReplacementDim(oldmod,
                            newmod, oldvecport->getDim()),
                        oldvecport->getDir());
                newvecport->setVarType(oldvecport->getVarType());
                newvecport->setVal(this->getReplacementParam(oldmod, newmod,
                            oldvecport->getVal()));

                const vector<Port*>& ports = newvecport->getPorts();
                for(unsigned int j = 0; j < ports.size(); ++j){
                    newports.push_back(ports[j]);
                }
                newtf->addObject(newvecport->getName(), newvecport, true);
            }

            newtf->addPorts(newports);
        }


    void
        Elaborator::addNets(
                const Module* oldmod,
                Module* newmod
                )
        {
            vector<Net*> nets = oldmod->getNets();
            vector<Net*> snets = GetVSObjects<Net, Net>(oldmod, nets);
            vector<VecNet*> vnets = GetVSObjects<Net, VecNet> (oldmod, nets);
            vector<Net*> newnets;

            int size = snets.size();
            for(int i = 0; i < size; ++i)
            {
                Net* oldnet = snets[i];
                Net* newnet = new Net(oldnet->getIxName());
                newnet->setNetType(oldnet->getNetType());
                newnet->setInitExpr(this->getReplacementParam(oldmod, newmod,
                            oldnet->getInitExpr()));
                newnets.push_back(newnet);
            }

            size = vnets.size();
            for(int i = 0; i < size; ++i)
            {
                VecNet* oldnet = vnets[i];
                VecNet* newnet = new VecNet(oldnet->getName(),
                        this->createReplacementDim(oldmod, newmod,
                            oldnet->getDim()));
                newnet->setVal(this->getReplacementParam(oldmod, newmod,
                            oldnet->getVal()));
                const vector<Net*>& tnets = newnet->getNets();
                for(unsigned int j = 0; j < tnets.size(); ++j){
                    newnets.push_back(tnets[j]);
                }
                newmod->addObject(newnet->getName(), newnet, true);
            }
            newmod->addNets(newnets);
        }

    void
        Elaborator::addRegs(
                const Module* oldmod,
                Module* newmod
                )
        {
            vector<Reg*> regs = oldmod->getRegs();
            vector<Reg*> sregs = GetVSObjects<Reg, Reg>(oldmod, regs);
            vector<VecReg*> vregs = GetVSObjects<Reg, VecReg> (oldmod, regs);
            vector<Reg*> newregs;

            int size = sregs.size();
            for(int i = 0; i < size; ++i)
            {
                Reg* oldreg = sregs[i];
                Reg* newreg = new Reg(oldreg->getIxName());
                newreg->setVal(this->getReplacementParam(oldmod, newmod,
                            oldreg->getVal()));
                newregs.push_back(newreg);
                newreg->setScope(newmod);
            }

            size = vregs.size();
            for(int i = 0; i < size; ++i)
            {
                VecReg* oldreg = vregs[i];
                VecReg* newreg = new VecReg(oldreg->getName(),
                        this->createReplacementDim(oldmod, newmod,
                            oldreg->getDim()), oldreg->isSigned());
                newreg->setVal(this->getReplacementParam(oldmod, newmod,
                            oldreg->getVal()));
                newreg->setScope(newmod);            
                const vector<Reg*>& tregs = newreg->getRegs();
                for(unsigned int j = 0; j < tregs.size(); ++j){
                    tregs[j]->setScope(newmod);
                    //newregs.push_back(tregs[j]);
                }
                newmod->addObject(newreg->getName(), newreg, true);
            }
            for(int i = 0; i < newregs.size(); ++i)
            {
                newmod->addObject(newregs[i]->toString(), newregs[i], true);
            }
        }
    void
        Elaborator::addLocalParams(
                const Module* oldmod,
                Module* newmod
                )
        {
            const vector<LocalParam*>& lparams = oldmod->getLocalParams();
            const vector<LocalParam*> slparams = 
                GetVSObjects<LocalParam, LocalParam> (oldmod, lparams);
            const vector<VecLocalParam*> vlparams = 
                GetVSObjects<LocalParam, VecLocalParam> (oldmod, lparams);

            vector<LocalParam*> newlparams;

            int size = slparams.size();
            for(int i = 0; i < size; ++i)
            {
                const LocalParam* lparam = slparams[i];
                const Expr* oldval = lparam->getVal();
                LocalParam* newlparam = new LocalParam(lparam->getIxName(),
                        this->getReplacementParam(oldmod, newmod, oldval),
                        lparam->getVarType());
                newlparam->setSign(lparam->isSigned());
                newlparams.push_back(newlparam);
            }
            size = vlparams.size();

            for(int i = 0; i < size; ++i)
            {
                const VecLocalParam* oldparam = vlparams[i];
                const Expr* oldval = oldparam->getVal();
                const Dimension* dim = 
                    this->createReplacementDim(
                            oldmod, newmod, oldparam->getDim());
                VecLocalParam* newlparam = new VecLocalParam(
                        oldparam->getName(), dim, oldparam->isSigned());

                newlparam->setVal(oldval);
                const vector<LocalParam*>& tlparams = 
                    newlparam->getLocalParams();
                for(unsigned int j = 0; j < tlparams.size(); ++j) {
                    newlparams.push_back(tlparams[j]);
                }
                newmod->addObject(newlparam->getName(), newlparam, true);
            }
            newmod->addLocalParams(newlparams);
        }

    void
        Elaborator::replaceParamInDim(
                const Module* oldmod,
                Module* newmod,
                const Dimension* olddim,
                Dimension* newdim)
        {
            int size = newdim->packDim.size();
            for(int i = 0; i < size; ++i)
            {
                Range* nrange = newdim->packDim[i];
                Range* orange = olddim->packDim[i];
                nrange->lrExpr.first = const_cast<Expr*>(
                        this->replaceParamInExpr(
                        oldmod, newmod, orange->lrExpr.first, 
                        const_cast<Expr*>(nrange->lrExpr.first)));
                nrange->lrExpr.second = const_cast<Expr*>(
                        this->replaceParamInExpr(
                        oldmod, newmod, orange->lrExpr.second, 
                        const_cast<Expr*>(nrange->lrExpr.second)));
            }
            size = newdim->unpackDim.size();
            for(int i = 0; i < size; ++i)
            {
                Range* nrange = newdim->unpackDim[i];
                Range* orange = olddim->unpackDim[i];
                nrange->lrExpr.first = const_cast<Expr*>(
                        this->replaceParamInExpr(
                        oldmod, newmod, orange->lrExpr.first, 
                        const_cast<Expr*>(nrange->lrExpr.first)));
                nrange->lrExpr.second = const_cast<Expr*>(
                        this->replaceParamInExpr(
                        oldmod, newmod, orange->lrExpr.second, 
                        const_cast<Expr*>(nrange->lrExpr.second)));
            }
        }





    const Expr*
        Elaborator::replaceParamInExpr(
                const Module* oldmod,
                Module* newmod,
                const Expr* oldexpr,
                Expr* newexpr
                )
        {
            if(!oldexpr || !newexpr){
                return 0;
            }

            if(const UnaryExpr* ouexpr = 
                    dynamic_cast<const UnaryExpr*>(oldexpr))
            {
                const UnaryExpr* nuexpr = dynamic_cast<const UnaryExpr*>(
                        newexpr);
                assert(nuexpr);
                const_cast<UnaryExpr*>(nuexpr)->setExpr(
                        this->replaceParamInExpr(
                            oldmod, newmod,
                            ouexpr->getExpr(), const_cast<Expr*>(
                                nuexpr->getExpr())));
                return nuexpr;
            }
            else if(const BinExpr* obexpr = 
                    dynamic_cast<const BinExpr*>(oldexpr))
            {
                const BinExpr* nbexpr = dynamic_cast<const BinExpr*>(
                        newexpr);
                assert(nbexpr);
                const_cast<BinExpr*>(nbexpr)->setLExpr(this->replaceParamInExpr(
                            oldmod, newmod,
                            obexpr->getLExpr(), 
                            const_cast<Expr*>(nbexpr->getLExpr())));
                const_cast<BinExpr*>(nbexpr)->setRExpr(
                        this->replaceParamInExpr(
                            oldmod, newmod,
                            obexpr->getRExpr(), 
                            const_cast<Expr*>(nbexpr->getRExpr())));
                return nbexpr;
            }
            else if (const CondExpr* ocexpr = 
                    dynamic_cast<const CondExpr*>(oldexpr))
            {
                const CondExpr* ncexpr = dynamic_cast<const CondExpr*>(
                        newexpr);
                assert(newexpr);
                const_cast<CondExpr*>(ncexpr)->setCond(
                        this->replaceParamInExpr(
                        oldmod, newmod,
                        ocexpr->getCond(), 
                        const_cast<Expr*>(ncexpr->getCond())));
                const_cast<CondExpr*>(ncexpr)->setTExpr(
                        this->replaceParamInExpr(
                        oldmod, newmod,
                        ocexpr->getTExpr(), 
                        const_cast<Expr*>(ncexpr->getTExpr())));
                const_cast<CondExpr*>(ncexpr)->setFExpr(
                        this->replaceParamInExpr(
                        oldmod, newmod,
                        ocexpr->getFExpr(), 
                        const_cast<Expr*>(ncexpr->getFExpr())));
                return ncexpr;        
            }
            else if(const BitSel* obitsel = dynamic_cast<const BitSel*>(
                        oldexpr))
            {
                const BitSel* nbitsel = dynamic_cast<const BitSel*>(
                        newexpr);
                assert(nbitsel);
                const vector<Expr*>& odim = obitsel->getDim();
                vector<Expr*>& ndim = const_cast<vector<Expr*>&>(
                        nbitsel->getDim());
                for(unsigned int i = 0; i < ndim.size(); ++i) {
                    ndim[i] = const_cast<Expr*>(this->replaceParamInExpr(
                        oldmod, newmod, odim[i], ndim[i]));
                }
                const Expr* var = obitsel->getVar();
                const_cast<BitSel*>(nbitsel)->setVar(
                        this->replaceParamInExpr(oldmod,
                            newmod, var, const_cast<Expr*>(nbitsel->getVar())));
                if(const PartSel* opartsel = dynamic_cast<const PartSel*>(
                            oldexpr))
                {
                    const PartSel* npartsel = dynamic_cast<const PartSel*>(
                            newexpr);
                    assert(npartsel);
                    const Range* orange = opartsel->getRange();
                    Range* nrange = const_cast<Range*>(npartsel->getRange());
                    nrange->lrExpr.first = const_cast<Expr*>(
                            this->replaceParamInExpr(
                            oldmod, newmod, orange->getLExpr(), 
                            nrange->lrExpr.first));
                    nrange->lrExpr.second = const_cast<Expr*>(
                            this->replaceParamInExpr(
                            oldmod, newmod, orange->getRExpr(), 
                            nrange->lrExpr.second));
                    return npartsel;        
                }
                else {
                    return nbitsel;
                }    
            }    
            else if(const VecCntv* ovcntv = 
                    dynamic_cast<const VecCntv*>(oldexpr))
            {
                //VecCntv* nvcntv = const_cast<VecCntv*>(
                //        dynamic_cast<const VecCntv*>(newexpr));
                const VecCntv* nvcntv = GetObject<VecCntv>(ovcntv->getName(), ovcntv, newmod);
                assert(nvcntv);
                        
                //const_cast<VecCntv*>(nvcntv)->setVal(
                //        this->replaceParamInExpr(oldmod, newmod,
                //            ovcntv->getVal(), const_cast<Expr*>(
                //                nvcntv->getVal())));
                //this->replaceParamInDim(oldmod, newmod,
                //        ovcntv->getDim(), const_cast<Dimension*>(
                //            nvcntv->getDim()));
                return nvcntv;
            }
            else if(const ScalarCntv *oscntv =
                    dynamic_cast<const ScalarCntv*>(oldexpr))
            {
                ScalarCntv *nscntv = dynamic_cast<ScalarCntv*>(newexpr);
                if(nscntv->isVecBit())
                {
                    const VecCntv *ovcntv = oscntv->getVecCntv();
                    assert(ovcntv);
                    const VecCntv *nvcntv = GetObject<VecCntv>(
                        ovcntv->getName(), ovcntv, newmod);
                    return nvcntv->getBit(oscntv->getIxName().getIndex());    
                }

                return GetObject<ScalarCntv>(oscntv->getIxName().getName(),
                    oscntv, newmod);
            }        
            if(const MultiConcat *omconcat = dynamic_cast<const MultiConcat*>(
                         oldexpr))
            {
                MultiConcat* nmconcat = const_cast<MultiConcat*>(
                    dynamic_cast<const MultiConcat*>(newexpr));
                assert(nmconcat);
                nmconcat->count = const_cast<Expr*>(
                        this->replaceParamInExpr(oldmod, newmod,
                            omconcat->count, nmconcat->count));
                int size = nmconcat->exprs.size();
                for(int i = 0; i < size; ++i) {
                    nmconcat->exprs[i] = const_cast<Expr*>(
                            this->replaceParamInExpr(oldmod, newmod,
                                omconcat->exprs[i], nmconcat->exprs[i]));
                }
                return nmconcat;
            }
            if(const Concat* oconcat = dynamic_cast<const Concat*>(
                        oldexpr))
            {
                Concat* nconcat = const_cast<Concat*>(
                        dynamic_cast<const Concat*>(newexpr));
                assert(nconcat);
                int size = nconcat->exprs.size();
                for(int i = 0; i < size; ++i) {
                    nconcat->exprs[i] = const_cast<Expr*>(
                            this->replaceParamInExpr(oldmod, newmod,
                                oconcat->exprs[i], nconcat->exprs[i]));
                }
                return nconcat;
            }
            else if(const FuncCall* oldfunccall = 
                    dynamic_cast<const FuncCall*>(oldexpr))
            {
                FuncCall* newfunccall = dynamic_cast<FuncCall*>(
                        newexpr);
                assert(newfunccall);
                const vector<Expr*>& oldargs = oldfunccall->getArgs();
                vector<Expr*>& newargs = const_cast<vector<Expr*>&>(
                        newfunccall->getArgs());

                for(unsigned int i = 0; i < oldargs.size(); ++i){
                    newargs[i] = const_cast<Expr*>(
                            this->replaceParamInExpr(oldmod, newmod,
                                oldargs[i], newargs[i]));
                }
                return newfunccall;
            }
            else if(const Specparam* ospecparam = dynamic_cast<const Specparam*>(
                        oldexpr))
            {
                const Specparam* nspecparam = 
                    dynamic_cast<const Specparam*>(oldexpr);
                vector<Expr*>& nval = const_cast<vector<Expr*>&>(
                        nspecparam->getVal());
                const vector<Expr*>& oval = ospecparam->getVal();
                for(unsigned int i = 0; i < oval.size(); ++i) {
                    nval[i] = const_cast<Expr*>(
                            this->replaceParamInExpr(oldmod, newmod,
                            oval[i], nval[i]));
                }
                return nspecparam;
            }
            else if(const Const *const_ = dynamic_cast<const Const*>(oldexpr)) {
                return const_->copy();
            }    
            else {
                return this->getReplacementParam(oldmod, newmod, oldexpr);
            }
            return 0; //should not reach here
        }

    Task*
        Elaborator::addTask(
                const Module* oldmod,
                Module* newmod,
                const Task* oldtask)
        {
            Task* newtask = new Task(oldtask->getName(),
                    dynamic_cast<Stmt*>(this->addObject(
                            oldmod, newmod, 0)),
                    oldtask->isAuto());

            this->addPorts(oldmod, newmod, oldtask, newtask);        
            newmod->addTask(newtask);
            newtask->setScope(newmod);
            this->addObjectsInScope(oldmod, newmod, oldtask, newtask);
            vector<Stmt*>& stmts = const_cast<vector<Stmt*>&>(newtask->getStmts());
            //assert(stmts.size() < 2);
            if(!stmts.empty()) 
            {
                Stmt *stmt = stmts[0];
                stmts.clear();
                newtask->setStmt(stmt);
            }
            else {
                newtask->setStmt(0);
            }    
            return newtask;
        }

    Func*
        Elaborator::addFunc(
                const Module* oldmod,
                Module* newmod,
                const Func* oldfunc)
        {
            const Range* orange = oldfunc->getRange();
            Range* nrange = orange ? new Range(this->replaceParamInExpr(
                        oldmod, newmod, orange->lrExpr.first,
                        const_cast<Expr*>(orange->lrExpr.first->copy())), 
                    this->replaceParamInExpr(
                        oldmod, newmod, orange->lrExpr.second,
                        const_cast<Expr*>(orange->lrExpr.second->copy()))): 0;

            Func* newfunc = new Func(oldfunc->getName(), oldfunc->isAuto(),
                    oldfunc->getRetType(), nrange, oldfunc->isSigned());
            this->addPorts(oldmod, newmod, oldfunc, newfunc);        
            const Expr *retvar = dynamic_cast<const Expr*>(oldfunc->getObject(oldfunc->getName()));
            assert(retvar);
            const Expr *newretvar = dynamic_cast<const Expr*>(
                this->replaceParamInExpr(oldmod, newmod,
                retvar, const_cast<Expr*>(retvar->copy())));
            assert(newretvar);    
            string name;
            if(const Var *var = dynamic_cast<const Var*>(newretvar)) {
                name = var->getIxName().getName();
            }
            else if(const VecVar *vvar = dynamic_cast<const VecVar*>(newretvar)) {
                name = vvar->getName();
            }
            assert(!name.empty());
            newfunc->addObject(name, newretvar, true);    
            this->addObjectsInScope(oldmod, newmod, oldfunc, newfunc);
            
            newmod->addFunc(newfunc);
            newfunc->setScope(newmod);
            vector<Stmt*>& stmts = const_cast<vector<Stmt*>&>(newfunc->getStmts());
            //assert(stmts.size() < 2);
            if(!stmts.empty()) 
            {
                Stmt *stmt = stmts[0];
                stmts.clear();
                newfunc->setStmt(stmt);
            }
            else {
                newfunc->setStmt(0);
            }    
            return newfunc;
        }


    Always*
        Elaborator::addAlways(
                const Module* oldmod,
                Module* newmod,
                const Always* oldalways)
        {
            Always* newalways = new Always(dynamic_cast<Stmt*>(
                        this->addObject(oldmod, newmod,
                        oldalways->getStmt())));
            return newalways;
        }

    Block*
        Elaborator::createBlock(
                const Module* oldmod,
                Module* newmod,
                const Block* oldblock)
        {
            Block* newblock = new Block();
            const vector<Stmt*>& oldstmts = oldblock->getStmts();
            for(unsigned int i = 0; i < oldstmts.size(); ++i){
                newblock->addStmt(dynamic_cast<Stmt*>(
                            this->addObject(oldmod, newmod, oldstmts[i])));
            }
            return newblock;
        }

    Case*
        Elaborator::createCase(
                const Module* oldmod,
                Module* newmod,
                const Case* oldcase)
        {
            const vector<CaseItem*>& ocaseitems = oldcase->getCaseItems();
            vector<CaseItem*> ncaseitems;
            for(unsigned int i = 0; i < ocaseitems.size(); ++i)
            {
                const vector<Expr*>& olabel = ocaseitems[i]->getLabel();
                vector<Expr*> nlabel;
                Stmt* nstmt = dynamic_cast<Stmt*>(this->addObject(
                            oldmod, newmod, ocaseitems[i]->getStmt()));
                for(unsigned int j = 0; j < olabel.size(); ++j){
                    nlabel.push_back(const_cast<Expr*>(
                                this->replaceParamInExpr(oldmod,
                                newmod, olabel[i], const_cast<Expr*>(
                                    olabel[i]->copy()))));
                }
                ncaseitems.push_back(new CaseItem(nlabel, nstmt));
            }
            Expr* nselectexpr = const_cast<Expr*>(
                    this->replaceParamInExpr(oldmod, newmod, 
                    oldcase->getSelectExpr(), const_cast<Expr*>(
                        oldcase->getSelectExpr()->copy())));

            Case* ncase = new Case(ncaseitems, nselectexpr, oldcase->getType());
            return ncase;
        }

    CompAssign*
        Elaborator::createCompAssign(
                const Module* oldmod,
                Module* newmod,
                const CompAssign* ocomassign)
        {
            const Expr* nlexpr = this->replaceParamInExpr(oldmod, newmod,
                    ocomassign->getLhs(), const_cast<Expr*>(
                        ocomassign->getLhs()->copy()));
            const Expr* nrepexpr = this->replaceParamInExpr(oldmod, newmod,
                    ocomassign->getRepeatExpr(), 
                    const_cast<Expr*>(ocomassign->getRepeatExpr()->copy()));

            EventCntrl* neventctrl = dynamic_cast<EventCntrl*>(
                    this->addObject(oldmod, newmod, ocomassign->getEventCtrl()));
            const Expr* neventexpr = this->replaceParamInExpr(oldmod, newmod,
                    ocomassign->getEventCtrlExpr(), const_cast<Expr*>(
                        ocomassign-> getEventCtrlExpr()->copy()));

            CompAssign* ncompassign = new CompAssign(nlexpr, nrepexpr,
                    neventctrl, neventexpr, ocomassign->getAssignType());
            return ncompassign;
        }

    DelayStmt*
        Elaborator::createDelayStmt(
                const Module* oldmod,
                Module* newmod,
                const DelayStmt* olddstmt
                )
        {
            const Delay* odelay = olddstmt->getDelay();
            const Stmt* ostmt = olddstmt->getStmt();

            const vector<DelayVal*>& odvals = odelay->getDelVals();
            vector<DelayVal*> ndvals;

            for(unsigned int i = 0; i < odvals.size(); ++i)
            {
                const vector<Expr*>& odelexpr = odvals[i]->getDelExpr();
                vector<Expr*> ndelexpr;
                for(unsigned int j = 0; j < odelexpr.size(); ++j) {
                    ndelexpr.push_back(const_cast<Expr*>(
                                this->replaceParamInExpr(oldmod, newmod,
                                    odelexpr[j], const_cast<Expr*>(
                                        odelexpr[j]->copy()))));
                }
                if(ndelexpr.size() == 1){
                    ndvals.push_back(new DelayVal(ndelexpr[0]));
                }
                else if(ndelexpr.size() == 3){
                    ndvals.push_back(new DelayVal(ndelexpr[0], ndelexpr[1],
                            ndelexpr[2]));
                }
            }
            Delay* ndelay;
            if(ndvals.size() == 1){
                ndelay = new Delay(ndvals[0]);
            }
            else if(ndvals.size() == 2){
                ndelay = new Delay(ndvals[0], ndvals[1]);
            }
            else if(ndvals.size() == 3){
                ndelay = new Delay(ndvals[0], ndvals[1], ndvals[2]);
            }

            DelayStmt* ndelaystmt = new DelayStmt(ndelay, 
                    dynamic_cast<DelayStmt*>(this->addObject(oldmod, newmod,
                            ostmt)));
            return ndelaystmt;
        }

    EventCntrl*
        Elaborator::createEventCntrl(
                const Module* oldmod,
                Module* newmod,
                const EventCntrl* oldeventcntrl)
        {
            const vector<pair<EdgeType, Expr*> >& oeventexprs =
                oldeventcntrl->getEventExprs();
            vector<pair<EdgeType, Expr*> > neventexprs;
            for(unsigned int i = 0; i < oeventexprs.size(); ++i){
                neventexprs.push_back(make_pair(oeventexprs[i].first,
                            const_cast<Expr*>(this->replaceParamInExpr(
                                    oldmod, newmod, oeventexprs[i].second,
                                    const_cast<Expr*>(
                                        oeventexprs[i].second->copy())))));
            }
            return new EventCntrl(neventexprs, dynamic_cast<Stmt*>(
                        this->addObject(oldmod, newmod, 
                            oldeventcntrl->getStmt()
                            )), oldeventcntrl->isV2kStyle()); 
        }

    EventStmt*
        Elaborator::addEventStmt(
                const Module* oldmod,
                Module* newmod,
                const EventStmt* oeventsmt)
        {
            EventStmt* neventstmt = oeventsmt->copy();
                /*new EventStmt(this->replaceParamInExpr(
                            oldmod, newmod, oeventsmt->getExpr(),
                            const_cast<Expr*>(oeventsmt->getExpr()->copy())));
                */            
            newmod->addEvent(neventstmt);
            return neventstmt;
        }

    For*
        Elaborator::createFor(
                const Module* oldmod,
                Module* newmod,
                const For* oldfor)
        {
            const Expr* newloopvar = this->replaceParamInExpr(
                    oldmod, newmod, oldfor->getLoopVar(), 
                    const_cast<Expr*>(oldfor->getLoopVar()));

            const Expr* newinitexpr = this->replaceParamInExpr(
                    oldmod, newmod, oldfor->getInitExpr(), 
                    const_cast<Expr*>(oldfor->getInitExpr()));

            const Expr* newtermexpr = this->replaceParamInExpr(
                    oldmod, newmod, oldfor->getTermExpr(), 
                    const_cast<Expr*>(oldfor->getTermExpr()));

            const Expr* newassignexpr = this->replaceParamInExpr(
                    oldmod, newmod, oldfor->getAssignExpr(), 
                    const_cast<Expr*>(oldfor->getAssignExpr()));

            Stmt* newstmt = dynamic_cast<Stmt*>(this->addObject(
                        oldmod, newmod, oldfor->getStmt()));
            return new For(newloopvar, newinitexpr, newtermexpr,
                    newassignexpr, newstmt);
        }

    template <typename T1, typename T2> 
        void Elaborator::addVarsInScope(
                const Module* oldmod,
                Module* newmod,
                const Scope* oldscope,
                Scope* newscope,
                const vector<T1*>& oldvars)
        {
            vector<T1*> oldsvars = GetVSObjects<T1, T1>(oldscope, oldvars);
            vector<T2*> oldvvars = GetVSObjects<T1, T2>(oldscope, oldvars);

            for(unsigned int i = 0; i < oldsvars.size(); ++i)
            {
                T1* newvar = new T1(oldsvars[i]->getIxName());
                newvar->setVal(oldsvars[i]->getVal() ? 
                    this->replaceParamInExpr(oldmod, newmod,
                    oldsvars[i]->getVal(), const_cast<Expr*>(
                    oldsvars[i]->getVal()->copy())): 0);
                newvar->setSign(oldsvars[i]->isSigned());
                newscope->addDeclItem(newvar);
                newvar->setScope(newscope);
            }

            for(unsigned int i = 0; i < oldvvars.size(); ++i)
            {
                Dimension* newdim = this->createReplacementDim(oldmod,
                        newmod, oldvvars[i]->getDim());

                T2* newvecvar = new T2(oldvvars[i]->getName(),
                        newdim, oldvvars[i]->isSigned());
                newvecvar->setVal(oldvvars[i]->getVal() ? 
                    this->replaceParamInExpr(oldmod, newmod,
                    oldvvars[i]->getVal(), const_cast<Expr*>(
                    oldvvars[i]->getVal()->copy())): 0);
                newscope->addDeclItem(newvecvar);
                newvecvar->setScope(newscope);
            }
        }

    void
        Elaborator::addObjectsInScope(
                const Module* oldmod,
                Module* newmod,
                const Scope* oldscope,
                Scope* newscope)
        {
            this->addVarsInScope<Reg, VecReg>(oldmod, newmod, oldscope, 
                    newscope, oldscope->getRegs());
            this->addVarsInScope<Integer, VecInt>(oldmod, newmod, oldscope,
                    newscope, oldscope->getIntegers());
            this->addVarsInScope<Time, VecTime>(oldmod, newmod, oldscope,
                    newscope, oldscope->getTimes());
            this->addVarsInScope<Real, VecReal>(oldmod, newmod, oldscope,
                    newscope, oldscope->getReals());
            this->addVarsInScope<RealTime, VecRealTime>(oldmod, newmod, oldscope,
                    newscope, oldscope->getRealTimes());

            const vector<Stmt*>& oldstmts = oldscope->getStmts();
            for(unsigned int i = 0; i < oldstmts.size(); ++i){
                newscope->addStmt(dynamic_cast<Stmt*>(
                            this->addObject(oldmod, newmod, 
                            oldstmts[i])));
            }
        }

    DelayVal*
        Elaborator::createDelayVal(
                const Module* oldmod,
                Module* newmod,
                const DelayVal* olddelayval
                )
        {
            if(!olddelayval) {
                return 0;
            }
            const vector<Expr*>& odelexpr = olddelayval->getDelExpr();
            DelayVal* ndval = 0;
            vector<Expr*> ndelexpr;
            for(unsigned int j = 0; j < odelexpr.size(); ++j) {
                ndelexpr.push_back(const_cast<Expr*>(
                            this->replaceParamInExpr(oldmod, newmod,
                                odelexpr[j], const_cast<Expr*>(
                                    odelexpr[j]->copy()))));
            }
            if(ndelexpr.size() == 1){
                ndval = new DelayVal(ndelexpr[0]);
            }
            else if(ndelexpr.size() == 3){
                ndval = new DelayVal(ndelexpr[0], ndelexpr[1],
                        ndelexpr[2]);
            }
            return ndval;
        }

    Delay*
        Elaborator::createDelay(
                const Module* oldmod,
                Module* newmod,
                const Delay* olddelay
                )
        {
            if(!olddelay) {
                return 0;
            }    
            const vector<DelayVal*>& odvals = olddelay->getDelVals();
            vector<DelayVal*> ndvals;

            for(unsigned int i = 0; i < odvals.size(); ++i){
                ndvals.push_back(this->createDelayVal(oldmod, newmod,
                            odvals[i]));
            }
            Delay* ndelay;
            if(ndvals.size() == 1){
                ndelay = new Delay(ndvals[0]);
            }
            else if(ndvals.size() == 2){
                ndelay = new Delay(ndvals[0], ndvals[1]);
            }
            else if(ndvals.size() == 3){
                ndelay = new Delay(ndvals[0], ndvals[1], ndvals[2]);
            }
            return ndelay;
        }





    SpecifyPathBase*
        Elaborator::createSpecifyPath(
                const Module* oldmod,
                Module* newmod,
                const SpecifyPathBase* oldspath
                )
        {
            const Expr* nifexpr = this->replaceParamInExpr(oldmod, newmod,
                    oldspath->getIfExpr(), oldspath->getIfExpr()->copy());

            const vector<Expr*>& oldsrcs = oldspath->getSrcs();
            vector<Expr*> newsrcs;
            for(unsigned int i = 0; i < oldsrcs.size(); ++i){
                newsrcs.push_back(const_cast<Expr*>(
                            this->replaceParamInExpr(oldmod, newmod,
                            oldsrcs[i], oldsrcs[i]->copy())));
            }

            const vector<DelayVal*>& odvals = oldspath->getDelay();
            vector<DelayVal*> ndvals;
            for(unsigned int i = 0; i < odvals.size(); ++i) {
                ndvals[i] = this->createDelayVal(oldmod, newmod, odvals[i]);
            }

            if(const SpecifyPath* ospath = 
                    dynamic_cast<const SpecifyPath*>(oldspath))
            {
                const vector<Expr*>& olddestids = ospath->getDestIds();
                vector<Expr*> newdestids;
                for(unsigned int i = 0; i < olddestids.size(); ++i){
                    newdestids.push_back(const_cast<Expr*>(
                                this->replaceParamInExpr(oldmod,
                                newmod, olddestids[i], olddestids[i]->copy())));
                }
                SpecifyPath* nspath =  new SpecifyPath(ospath->getPolarity(), 
                        ospath->getConnType(), newsrcs, newdestids);
                nspath->setDelays(ndvals);
            }

            if(const SpecifyEdgePath* osedgepath = 
                    dynamic_cast<const SpecifyEdgePath*>(oldspath))
            {
                const Expr* newdestid = this->replaceParamInExpr(oldmod, newmod,
                        osedgepath->getDestId(), 
                        osedgepath->getDestId()->copy());

                const pair<Expr*, Expr*>& odestexprs = 
                    osedgepath->getDestExprPair();

                SpecifyEdgePath* nsepath =  new SpecifyEdgePath(osedgepath->getEdgeType(),
                        newdestid, osedgepath->getPolOp(), make_pair(
                            const_cast<Expr*>(this->replaceParamInExpr(oldmod, 
                                    newmod, odestexprs.first, 
                                    odestexprs.first->copy())),
                            const_cast<Expr*>(this->replaceParamInExpr(oldmod,
                                    newmod, odestexprs.second, 
                                    odestexprs.second->copy()))),
                        osedgepath->getPolarity(), osedgepath->getConnType(),
                        newsrcs);
                nsepath->setDelays(ndvals);
                nsepath->setIfExpr(nifexpr);
                return nsepath;
            }
            return NULL;
        }

    SpecRefEvent*
        Elaborator::createSpecRefEvent(
                const Module* oldmod,
                Module* newmod,
                const SpecRefEvent* oldsrevent
                )
        {
            return new SpecRefEvent(oldsrevent->getEdgeType(),
                    this->replaceParamInExpr(oldmod, newmod, 
                        oldsrevent->getPrimExpr(), 
                        oldsrevent->getPrimExpr()->copy()),
                    this->replaceParamInExpr(oldmod, newmod, 
                        oldsrevent->getExpr(), oldsrevent->getExpr()->copy()));
        }


    SpecTask*
        Elaborator::createSpecTask(
                const Module* oldmod,
                Module* newmod,
                const SpecTask* oldtask)
        {
            const vector<Expr*>& onotifiers = oldtask->getNotifiers();
            vector<Expr*> nnotifiers;

            for(unsigned int i = 0; i < onotifiers.size(); ++i){
                nnotifiers.push_back(const_cast<Expr*>(
                            this->replaceParamInExpr(oldmod,
                            newmod, onotifiers[i], onotifiers[i]->copy())));
            }
            const pair<SpecRefEvent*, SpecRefEvent*>& osrevents =
                oldtask->getSpecRefEvents();

            const pair<DelayVal*, DelayVal*>& odelayvals = 
                oldtask->getDelayVals();

            return new SpecTask(oldtask->getName(), make_pair(
                        this->createSpecRefEvent(oldmod, newmod, 
                            osrevents.first), this->createSpecRefEvent(
                                oldmod, newmod, osrevents.second)),
                    make_pair(this->createDelayVal(oldmod, newmod, 
                            odelayvals.first), this->createDelayVal(oldmod,
                                newmod, odelayvals.second)), nnotifiers);
        }

    pair<vector<Pin*>, vector<VecPin*> >
        Elaborator::createPins(
                const Module* oldmod,
                Module* newmod,
                const Inst *inst,
                const vector<Pin*>& oldpins
                )
        {
            set<string, compare_str> doneObjs;
            pair<vector<Pin*>, vector<VecPin*> > newpins;
            vector<Pin*> oldspins;
            vector<VecPin*> oldvpins;
            const ModInst *minst = dynamic_cast<const ModInst*>(inst);

            for(unsigned int i = 0; i < oldpins.size(); ++i)
            {
                if(doneObjs.find(oldpins[i]->getIxName().getName()) !=
                            doneObjs.end()){
                    continue;
                }
                doneObjs.insert(oldpins[i]->getIxName().getName());

                if(oldpins[i]->isVecBit())
                {
                    //const VecPin* vpin = dynamic_cast<const VecPin*>(
                    //        oldpins[i]->getVecCntv());
                    assert(minst);
                    const VecPin *vpin = minst->getVecPin(oldpins[i]->
                        getIxName().getName());
                    assert(vpin);    
                            
                    oldvpins.push_back(const_cast<VecPin*>(vpin));
                }
                else{
                    oldspins.push_back(oldpins[i]);
                }
            }

            for(unsigned int i = 0; i < oldvpins.size(); ++i)
            {
                Dimension* newdim = this->createReplacementDim(
                        oldmod, newmod, oldvpins[i]->getDim());
                VecPin* newvpin = new VecPin(oldvpins[i]->getName(),
                        newdim, oldvpins[i]->getDir(),
                        oldvpins[i]->getActual() ?
                            this->replaceParamInExpr(oldmod, newmod, 
                            oldvpins[i]->getActual(), 
                            oldvpins[i]->getActual()->copy()): 0);
                const vector<Pin*>& tnewpins = newvpin->getPins();
                for(unsigned int j = 0; j < tnewpins.size(); ++j){
                    newpins.first.push_back(tnewpins[j]);
                }
                newpins.second.push_back(newvpin);
            }

            for(unsigned int i = 0; i < oldspins.size(); ++i)
            {
                Pin* newpin = new Pin(oldspins[i]->getIxName(), 
                        oldspins[i]->getDir(), oldspins[i]->getActual() ?
                            this->replaceParamInExpr(
                            oldmod, newmod, oldspins[i]->getActual(),
                            oldspins[i]->getActual()->copy()): 0);
                newpins.first.push_back(newpin);
            }
            return newpins;
        }



    void
        Elaborator::addSpecifyBlock(
                const Module* oldmod,
                Module* newmod,
                const SpecifyBlock* ospecifyblock
                )
        {
            SpecifyBlock* nspecifyblock = new SpecifyBlock();
            const vector<Specparam*>& ospecparams = 
                ospecifyblock->getSpecparams();
            vector<Specparam*> nspecparams;
            for(unsigned int i = 0; i < ospecparams.size(); ++i){
                nspecparams.push_back(const_cast<Specparam*>(
                            dynamic_cast<const Specparam*>(
                                this->replaceParamInExpr(
                                    oldmod, newmod, ospecparams[i], 
                                    ospecparams[i]->copy()
                                    )
                                )
                            )
                        );
            }
            nspecifyblock->addSpecparams(nspecparams);

            const vector<SpecifyPathBase*>& ospecpaths = 
                ospecifyblock->getSpecPaths();

            for(unsigned int i = 0; i < ospecpaths.size(); ++i){
                nspecifyblock->addSpecPath(this->createSpecifyPath(
                            oldmod, newmod, ospecpaths[i]));
            }

            const vector<SpecTask*>& ospectasks = 
                ospecifyblock->getSpecTasks();

            for(unsigned int i = 0; i < ospectasks.size(); ++i){
                nspecifyblock->addSpecTask(this->createSpecTask(oldmod,
                            newmod, ospectasks[i]));
            }

            newmod->addSpecifyBlock(nspecifyblock);
        }

    Inst*
        Elaborator::addInst(
                const Module* oldmod,
                Module* newmod,
                const Inst* oldinst
                )
        {
            const vector<Pin*>& oldpins = oldinst->getPins();
            string name = oldinst->getName().getName();
            pair<vector<Pin*>, vector<VecPin*> > newpins = this->
                createPins(oldmod, newmod, oldinst, oldpins);
            Inst* newinst = 0;

            if(const GateInst* oginst = dynamic_cast<const GateInst*>(
                        oldinst))
            {
                GateInst* nginst = new GateInst(name, oginst->getType());
                nginst->setDelay(this->createDelay(oldmod, 
                            newmod, oginst->getDelay()));
                if(const DriveSt* oldst = oginst->getDrStrength()) {
                    nginst->setDrStr(new DriveSt(*oldst));
                }
                newmod->addGateInst(nginst);
                newinst = nginst;
            }
            else if(const ModInst* ominst = dynamic_cast<const ModInst*>(
                        oldinst))
            {
                ModInst* nmodinst = new ModInst(ominst->getName().getName(),
                        ominst->getModName(), ominst->getMaster());
                const vector<NameConn*>& oparams = ominst->getParams();
                vector<NameConn*> nparams;
                for(unsigned int i = 0; i < oparams.size(); ++i){
                    NameConn* nnameconn = new NameConn();
                    nnameconn->first = oparams[i]->first;
                    nnameconn->second = const_cast<Expr*>(
                            this->replaceParamInExpr(oldmod, newmod,
                                oparams[i]->second, const_cast<Expr*>(
                                    oparams[i]->second->copy())));
                }
                nmodinst->setParams(nparams);
                newinst = nmodinst;
                newmod->addModInst(nmodinst);
            }
            else if(const UDPInst* ouinst = dynamic_cast<const UDPInst*>(
                        oldinst))
            {
                UDPInst* nudpinst = new UDPInst(ouinst->getName(),
                        ouinst->getUDPName());
                newinst = nudpinst;
                newmod->addUDPInst(nudpinst);
            }
            newinst->setPins(newpins.first);
            if(ModInst *minst = dynamic_cast<ModInst*>(newinst)) {
                for(int i = 0; i < newpins.second.size(); ++i) {
                    minst->addVecPin(newpins.second[i]);
                }    
            }
            return newinst;
        }

    ContAssign*
        Elaborator::addContAssign(
                const Module* oldmod,
                Module* newmod,
                const ContAssign* ocontassign
                )
        {
            const DriveSt* olddst = ocontassign->getStrength();
            DriveSt* nddst = olddst ? new DriveSt(*olddst) : 0;
            const Delay* olddelay = ocontassign->getDelay();
            Delay* newdelay = this->createDelay(oldmod, newmod, olddelay);
            const Expr* newlhs = this->replaceParamInExpr(oldmod,
                        newmod, ocontassign->getLhs(), 
                        ocontassign->getLhs()->copy());
            const Expr* newrhs = this->replaceParamInExpr(oldmod, newmod,
                    ocontassign->getRhs(), ocontassign->getRhs()->copy());
            assert(newrhs);        
            assert(newlhs);        

            ContAssign* newcontassign = new ContAssign(newdelay, nddst, 
                    newlhs, newrhs);
            newmod->addContAssign(newcontassign);
            return newcontassign;
        }

    Defparam*
        Elaborator::addDefparam(
                const Module* oldmod,
                Module* newmod,
                const Defparam* odefparam
                )
        {
            const Expr* newlhs = this->replaceParamInExpr(oldmod,
                        newmod, odefparam->getLhs(), 
                        odefparam->getLhs()->copy());
            const Expr* newrhs = this->replaceParamInExpr(oldmod, newmod,
                    odefparam->getRhs(), odefparam->getRhs()->copy());

            Defparam* newdefparam = new Defparam(newlhs, newrhs);
            newmod->addDefparam(newdefparam);
            return newdefparam;
        }



    Base*
        Elaborator::addObject(
                const Module* oldmod,
                Module* newmod,
                const Base* oldobj)
        {
            if(const DAssign* odassign = dynamic_cast<const DAssign*>(
                        oldobj))
            {
                DAssign* ndassign = new DAssign(this->replaceParamInExpr(
                            oldmod, newmod,
                            odassign->getDAssign(), const_cast<Expr*>(
                                odassign->getDAssign()->copy())));
                return ndassign;
            }
            
            if(const DelayStmt* dstmt = dynamic_cast<const DelayStmt*>(
                        oldobj))
            {
                return this->createDelayStmt(oldmod, newmod, dstmt);
            }

            if(const Disable* olddisable = dynamic_cast<const Disable*>(
                        oldobj))
            {
                //return new Disable(this->replaceParamInExpr(oldmod, newmod,
                //            olddisable->getId(),
                //            const_cast<Expr*>(olddisable->getId()->copy())));
                return new Disable(olddisable->getId());
            }

            if(const EventCntrl* oecntrl = dynamic_cast<const EventCntrl*>(
                        oldobj))
            {
                return this->createEventCntrl(oldmod, newmod, oecntrl);
            }

            if(const Wait* oldwait = dynamic_cast<const Wait*>(oldobj))
            {
                return new Wait(this->replaceParamInExpr(oldmod, newmod,
                            oldwait->getCondExpr(), const_cast<Expr*>(
                                oldwait->getCondExpr()->copy())), 
                        dynamic_cast<Stmt*>(this->addObject(oldmod, newmod,
                                oldwait->getStmt())));
            }

            if(const Force* oldforce = dynamic_cast<const Force*>(oldobj))
            {
                return new Force(this->replaceParamInExpr(
                            oldmod, newmod, oldforce->getLhs(), 
                            const_cast<Expr*>(oldforce->getLhs()->copy())),
                            this->replaceParamInExpr(oldmod, newmod,
                                oldforce->getRhs(), const_cast<Expr*>(
                                    oldforce->getRhs()->copy())));
            }

            if(const Release* oldrelease = dynamic_cast<const Release*>(
                        oldobj))
            {
                return new Release(this->replaceParamInExpr(oldmod, newmod,
                            oldrelease->getVar(), const_cast<Expr*>(
                                oldrelease->getVar()->copy())));
            }

            if(const Initial* oldinit = dynamic_cast<const Initial*>(oldobj))
            {
                Initial* newinit = new Initial(dynamic_cast<Stmt*>(
                            this->addObject(
                                oldmod, newmod, oldinit->getStmt())));
                newmod->addInitial(newinit);
                return newinit;
            }

            if(const Forever* oldforever = dynamic_cast<const Forever*>(
                        oldobj))
            {
                return new Forever(dynamic_cast<Stmt*>(this->addObject(
                                oldmod, newmod, oldforever->getStmt())));
            }

            if(const Repeat* oldrepeat = dynamic_cast<const Repeat*>(oldobj))
            {
                return new Repeat(this->replaceParamInExpr(oldmod, newmod,
                            oldrepeat->getExpr(), const_cast<Expr*>(
                                oldrepeat->getExpr()->copy())),
                            dynamic_cast<Stmt*>(oldmod, newmod, 
                                const_cast<Stmt*>(oldrepeat->getStmt())));
            }

            if(const For* oldfor = dynamic_cast<const For*>(oldobj))
            {
                return this->createFor(oldmod, newmod, oldfor);
            }

            if(const While* oldwhile = dynamic_cast<const While*>(oldobj))
            {
                return new While(this->replaceParamInExpr(oldmod, newmod,
                            oldwhile->getExpr(), const_cast<Expr*>(
                                oldwhile->getExpr()->copy())),
                        dynamic_cast<Stmt*>(this->addObject(oldmod, newmod,
                                oldwhile->getStmt())));
            }

            if(const NamedBlock* oldnblock = dynamic_cast<const NamedBlock*>(
                        oldobj))
            {
                NamedBlock* newnblock = new NamedBlock(oldnblock->getName());
                this->addObjectsInScope(oldmod, newmod, oldnblock, newnblock);
                return newnblock;
            }
            if(const Block *oldblock = dynamic_cast<const Block*>(oldobj))
            {
                Block *newblock = new Block();
                const vector<Stmt*>& stmts = oldblock->getStmts();
                for(int i = 0; i < stmts.size(); ++i) {
                    newblock->addStmt(dynamic_cast<const Stmt*>(
                        this->addObject(oldmod, newmod, stmts[i])));
                }
                return newblock;
            }

            if(const Fork* oldfork = dynamic_cast<const Fork*>(oldobj))
            {
                Fork* newfork = new Fork(oldfork->getName());
                this->addObjectsInScope(oldmod, newmod, oldfork, newfork);
                return newfork;
            }
            
            if(const Trigger* otrigger = dynamic_cast<const Trigger*>(oldobj))
            {
                return new Trigger(this->replaceParamInExpr(oldmod, newmod,
                            otrigger->getId(), const_cast<Expr*>(
                                otrigger->getId()->copy())));
            }

            if(const ProcContAssign* oproccontassign = dynamic_cast<
                    const ProcContAssign*>(oldobj))
            {
                const Expr* newlhs = this->replaceParamInExpr(oldmod,
                        newmod, oproccontassign->getLhs(), 
                        oproccontassign->getLhs()->copy());
                const Expr* newrhs = this->replaceParamInExpr(oldmod, newmod,
                        oproccontassign->getRhs(), 
                        oproccontassign->getRhs()->copy());
                return new ProcContAssign(newlhs, newrhs);
            }

            if(const ProcAssign* oprocassign = dynamic_cast<const ProcAssign*>(
                        oldobj))
            {
                const Expr* newlhs = this->replaceParamInExpr(oldmod,
                        newmod, oprocassign->getLhs(), 
                        oprocassign->getLhs()->copy());
                const Expr* newrhs = this->replaceParamInExpr(oldmod, newmod,
                        oprocassign->getRhs(), 
                        oprocassign->getRhs()->copy());
                Delay* ldelay = this->createDelay(oldmod, newmod, 
                        oprocassign->getLDelay());
                Delay* rdelay = this->createDelay(oldmod, newmod, 
                        oprocassign->getRDelay());

                return new ProcAssign(ldelay, rdelay, newlhs, newrhs, 
                        oprocassign->getAssignType());
            }
            return 0;

        }

    void
        Elaborator::elaborate(const bool partial)
        {
            //PrintMsg("[INFO]: Starting elaboration .....\n");
            //MsgHandler::instance()->print(4);
            this->populateDefparamInfo();
            const vector<Module*>& topmodules = GRoot->getTopModules();
            for(unsigned int i = 0; i < topmodules.size(); ++i)
            {
                Path path(topmodules[i]->getName());
                this->elaborate(topmodules[i], path, partial);
            }
            //PrintMsg("[INFO]: Elaboration done .....\n");
            //MsgHandler::instance()->print(5);
        }
    void
        Elaborator::elaborate(
                const Module* mod,
                const Path& path,
                const bool partial
                )
        {
            const list<ModInst*>& minsts = mod->getModInsts();
            for(list<ModInst*>::const_iterator iter = minsts.begin();
                    iter != minsts.end(); ++iter)
            {
                const ModInst* minst = *iter;
                const Module* master = minst->getMaster();
                if(!master){
                    continue;
                }

                Path newpath(path);
                newpath.descend(minst->getName().toString());
                Module* newmod = this->elaborate(const_cast<ModInst*>(minst), 
                        newpath, partial);
                if(newmod)
                {
                    map<string, ElabInfo*, compare_str>::iterator iter =
                        elabInfos.find(newmod->getName());
                    if(iter == elabInfos.end())
                    {
                        ElabInfo* elabInfo = new ElabInfo();
                        elabInfo->oldmod = const_cast<Module*>(mod);
                        elabInfo->newmod = newmod;
                        elabInfo->insts.insert(make_pair(newpath, 
                                const_cast<ModInst*>(minst)));

                        elabInfos.insert(make_pair(newmod->getName(), elabInfo));
                        GRoot->addObject(newmod->getName(), newmod, true);
                    }
                    else {
                        iter->second->insts.insert(make_pair(newpath, 
                                    const_cast<ModInst*>(minst)));
                    }
                    const_cast<ModInst*>(minst)->setMaster(newmod);
                }
                this->elaborate(master, newpath, partial);
            }
        }

    Module*
        Elaborator::elaborate(
                ModInst* inst,
                const Path& path,
                const bool partial)
        {
            const Module* mod = inst->getMaster();
            if(!mod){
                return 0;
            }

            const Module* parent = dynamic_cast<const Module*>(
                    inst->getScope());
            assert(parent);
            vector<NameConn*> defparams;

            if(!this->elaborationRequired(mod, inst, defparams, path)){
                return 0;
            }
            //Check if we have a elaborated module with same parameter
            //overriding.
            string modname = this->getElabModName(mod, inst, defparams);
            map<string, ElabInfo*, compare_str>::iterator elabinfo =
                    elabInfos.find(modname);
            if(elabinfo != elabInfos.end()){
                return elabinfo->second->newmod;
            }
            //Now start the elaboration

            Module* newmod = this->getElabModule(mod, inst, defparams);
            this->addPorts(mod, newmod);
            this->addNets(mod, newmod);
            this->addRegs(mod, newmod);
            this->addLocalParams(mod, newmod);

            const list<GateInst*>& ginsts = mod->getGateInsts();
            for(list<GateInst*>::const_iterator ginst = ginsts.begin();
                    ginst != ginsts.end(); ++ginst){
                this->addInst(mod, newmod, *ginst);
            }
            const list<ModInst*>& minsts = mod->getModInsts();
            for(list<ModInst*>::const_iterator minst = minsts.begin();
                    minst != minsts.end(); ++minst){
                this->addInst(mod, newmod, *minst);
            }

            const list<UDPInst*>& uinsts = mod->getUDPInsts();
            for(list<UDPInst*>::const_iterator uinst = uinsts.begin();
                    uinst != uinsts.end(); ++uinst){
                this->addInst(mod, newmod, *uinst);
            }

            const vector<ContAssign*>& contassigns = mod->getContAssigns();
            for(unsigned int i = 0; i < contassigns.size(); ++i){
                this->addContAssign(mod, newmod, contassigns[i]);
            }

            const vector<Task*>& tasks = mod->getTasks();
            for(unsigned int i = 0; i < tasks.size(); ++i){
                this->addTask(mod, newmod, tasks[i]);
            }

            const vector<Func*>& funcs = mod->getFuncs();
            for(unsigned int i = 0; i < funcs.size(); ++i){
                this->addFunc(mod, newmod, funcs[i]);
            }

            const vector<Always*>& always = mod->getAlwaysBlocks();
            for(unsigned int i = 0; i < always.size(); ++i){
                this->addAlways(mod, newmod, always[i]);
            }

            const vector<Initial*>& initials = mod->getInitBlocks();
            for(unsigned int i = 0; i < initials.size(); ++i){
                this->addObject(mod, newmod, initials[i]);
            }

            const vector<SpecifyBlock*>& specifyblocks = 
                mod->getSpecifyBlocks();
            for(unsigned int i = 0; i < specifyblocks.size(); ++i){
                this->addSpecifyBlock(mod, newmod, specifyblocks[i]);
            }

            const vector<Defparam*>& olddefparams = mod->getDefparams();
            for(unsigned int i = 0; i < olddefparams.size(); ++i){
                this->addDefparam(mod, newmod, olddefparams[i]);
            }

            const vector<EventStmt*>& events = mod->getEvents();
            for(unsigned int i = 0; i < events.size(); ++i){
                this->addEventStmt(mod, newmod, events[i]);
            }

            const vector<GenerateStmt*>& genstmts = mod->getGenStmts();
            for(unsigned int i = 0; i < genstmts.size(); ++i){
                this->addGenerate(mod, newmod, genstmts[i]);
            }

            return newmod;
        }

    GenerateItem*
        Elaborator::createGenItem(
                const Module* oldmod,
                Module* newmod,
                const GenerateItem* genitem
                )
        {
            if(const GenerateCond* gcond = 
                    dynamic_cast<const GenerateCond*>(genitem)){
                return this->createGenCond(oldmod, newmod, gcond);
            }

            if(const GenerateCase* gcase = 
                    dynamic_cast<const GenerateCase*>(genitem)){
                return this->createGenCase(oldmod, newmod, gcase);
            }

            if(const GenerateBlock* gblock =
                    dynamic_cast<const GenerateBlock*>(genitem)){
                return this->createGenBlock(oldmod, newmod, gblock);
            }

            if(const GenerateLoop* gloop = 
                    dynamic_cast<const GenerateLoop*>(genitem)){
                return this->createGenLoop(oldmod, newmod, gloop);
            }

            if(const GenDecl* gdecl = dynamic_cast<const GenDecl*>(genitem)){
                return this->createGenDecl(oldmod, newmod, gdecl);
            }

            return 0; //should not reach here
        }

    GenerateCond*
        Elaborator::createGenCond(
                const Module* oldmod,
                Module* newmod,
                const GenerateCond* oldgencond
                )
        {
            GenerateCond* newgencond = new GenerateCond(
                    this->replaceParamInExpr(oldmod, newmod, 
                        oldgencond->condexpr, oldgencond->condexpr->copy()));
            newgencond->ifpart = this->createGenItem(oldmod, newmod,
                    oldgencond->ifpart);
            newgencond->elsepart = this->createGenItem(oldmod, newmod,
                    oldgencond->elsepart);
            return newgencond;
        }

    GenerateCase*
        Elaborator::createGenCase(
                const Module* oldmod,
                Module* newmod,
                const GenerateCase* oldgencase
                )
        {
            GenerateCase* newgencase = new GenerateCase(
                    this->replaceParamInExpr(oldmod, newmod, oldgencase->select,
                        oldgencase->select->copy()));
            newgencase->defstmt = this->createGenItem(oldmod, newmod,
                    oldgencase->defstmt);
            
            vector<pair<vector<Expr*>, GenerateItem*> > ncaseitems;

            for(unsigned int i = 0; i < oldgencase->caseitems.size(); ++i)
            {
                pair<vector<Expr*>, GenerateItem*> pvegi;
                for(unsigned int j = 0; j < oldgencase->caseitems[i].first.size(); ++j){
                    pvegi.first.push_back(const_cast<Expr*>(
                                this->replaceParamInExpr(oldmod, newmod,
                                    oldgencase->caseitems[i].first[j],
                                    oldgencase->caseitems[i].first[j]->
                                    copy())));
                }
                pvegi.second = this->createGenItem(oldmod, newmod,
                        oldgencase->caseitems[i].second);
                ncaseitems.push_back(pvegi);
            }
            newgencase->caseitems = ncaseitems;
            return newgencase;
        }

    GenerateBlock*
        Elaborator::createGenBlock(
                const Module* oldmod,
                Module* newmod,
                const GenerateBlock* oldgblock
                )
        {
            GenerateBlock* newgblock = new GenerateBlock(oldgblock->getName());
            this->addObjectsInScope(oldmod, newmod, oldgblock, newgblock);

            for(unsigned int i = 0; i < oldgblock->gitems.size(); ++i){
                newgblock->gitems.push_back(this->createGenItem(
                            oldmod, newmod, oldgblock->gitems[i]));
            }
            return newgblock;
        }

    GenerateLoop*
        Elaborator::createGenLoop(
                const Module* oldmod,
                Module* newmod,
                const GenerateLoop* oldgloop
                )
        {
            const Expr* loopvar = this->replaceParamInExpr(oldmod, newmod,
                    oldgloop->loopvar, oldgloop->loopvar->copy());
            const Expr* initexpr = this->replaceParamInExpr(oldmod, newmod,
                    oldgloop->initexpr, oldgloop->initexpr->copy());
            const Expr* termexpr = this->replaceParamInExpr(oldmod, newmod,
                    oldgloop->termexpr, oldgloop->termexpr->copy());
            const Expr* assignexpr = this->replaceParamInExpr(oldmod, newmod,
                    oldgloop->assignexpr, oldgloop->assignexpr->copy());

            GenerateLoop* newgloop = new GenerateLoop(loopvar, initexpr,
                    termexpr, assignexpr);

            newgloop->gitem = this->createGenItem(oldmod, newmod, 
                    oldgloop->gitem);
            return newgloop;
        }

    GenDecl*
        Elaborator::createGenDecl(
                const Module* oldmod,
                Module* newmod,
                const GenDecl* oldgdecl
                )
        {
            //TBD
            return 0;
        }

    GenerateStmt* 
        Elaborator::addGenerate(
            const Module* oldmod,
            Module* newmod,
            const GenerateStmt* ogstmt
            )
    {
        GenerateStmt* ngstmt = new GenerateStmt();
        const vector<Genvar*>& genvars = ogstmt->getGenvars();

        for(unsigned int i = 0; i < genvars.size(); ++i) {
            ngstmt->addGenvar(genvars[i]->copy());
        }

        const vector<GenerateCond*>& gconds = ogstmt->getCondGens();
        for(unsigned int i = 0; i < gconds.size(); ++i) {
            ngstmt->addGenerateItem(this->createGenItem(oldmod, newmod,
                        gconds[i]));
        }

        const vector<GenerateCase*>& gcase = ogstmt->getCaseGens();
        for(unsigned int i = 0; i < gcase.size(); ++i){
            ngstmt->addGenerateItem(this->createGenItem(oldmod, newmod,
                        gcase[i]));
        }
        const vector<GenerateLoop*>& gloop = ogstmt->getLoopGens();
        for(unsigned int i = 0; i < gloop.size(); ++i){
            ngstmt->addGenerateItem(this->createGenItem(oldmod, newmod, 
                        gloop[i]));
        }

        const vector<GenerateBlock*>& gblock = ogstmt->getBlockGens();
        for(unsigned int i = 0; i < gblock.size(); ++i){
            ngstmt->addGenerateItem(this->createGenItem(oldmod, newmod, 
                        gblock[i]));
        }

        newmod->addGenerate(ngstmt);
        return ngstmt;
    }





}
