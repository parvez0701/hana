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

#include "always2gates.h"

#include "ftcall2gates.h"

#include "systmt.h"
#include "symsghandler.h"
#include "syconst.h"

using std::runtime_error;
using std::vector;
using std::make_pair;
using std::map;

namespace Synth
{


    Always2Gates::Always2Gates(    const SyModule    *rmod, 
                                const SyModule    *smod, 
                                const SyScope    *sscope): 
                                ConVisitorBase(rmod, smod, sscope),
                                alwaysInfo(0) 
    {
    }

    Always2Gates::~Always2Gates() 
    {
        delete alwaysInfo;
    }    

    void
    Always2Gates::setCurrMod(SyModule *mod)
    {
        this->setCurrModInternal(mod);
    }

    void
    Always2Gates::setCurrRtlMod(SyModule *mod)
    {
        this->setCurrRtlModInternal(mod);
    }

    void
    Always2Gates::setCurrSynthMod(SyModule *mod)
    {
        this->setCurrSynthModInternal(mod);
    }

    void
    Always2Gates::setCurrScope(SyScope *sc)
    {
        this->setCurrScopeInternal(sc);
    }

    void
    Always2Gates::setStmt2Gates(Stmt2Gates *s2g) {
        stmt2Gates = s2g;
    }    


    void
    Always2Gates::visit(const SyAlways *always) 
    {
        //try {
        //    alwaysInfo = new AlwaysInfo(
        //        const_cast<SyAlways*>(always), this->getCurrSynthMod());
        //}
        //catch (runtime_error& e) {
        //    return;
        //}
        alwaysInfo = SyProcMgr::instance()->getAlwaysInfo(always);
        assert(alwaysInfo);    

        const SyList<SyStmt*>& stmts = always->getStmts();
        vector<OutCntvInfoSet*> info;
        Out2Rep out2Rep;
        stmt2Gates->setOut2Rep(out2Rep);
        for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next)
        {
            node->val->accept(stmt2Gates);
            if(OutCntvInfoSet *cntvSet = 
                FuncCallMerge::instance()->merge(stmt2Gates->getCurrScope()))
            {    
                info.push_back(cntvSet);
                stmt2Gates->mergeRep(info[info.size()-1], out2Rep);
            }
            info.push_back(stmt2Gates->getOutCntvInfoSet());
            stmt2Gates->mergeRep(info[info.size()-1], out2Rep);
            stmt2Gates->setOut2Rep(out2Rep);
        }    
        this->createGates(info);
    }    

    void
    Always2Gates::createGates(vector<OutCntvInfoSet*>& info)
    {
        //Now is the time to create final gates
        stmt2Gates->removeOverriddenEntries(info);
        vector<SyCntv*> outCntvs = stmt2Gates->getOutCntvs(info);

        SyCntv *clk = const_cast<SyCntv*>(dynamic_cast<const SyCntv*>(
            this->getCurrSynthMod()->findObject(alwaysInfo->clock)));

        if(!alwaysInfo->isComb())
        {


            if(alwaysInfo->isNegEdgeClock())
            {
                SyScNet *net = this->createNet();
                this->createInv(net, clk);
                clk = net;
            }
        }    

        map<SyCntv*, SyCntv*> setMap;
        map<SyCntv*, SyCntv*> resetMap;

        for(unsigned int i = 0; i < outCntvs.size(); ++i)
        {
            OutCntvInfo outCntvInfo = stmt2Gates->getMergedOutCntvInfo(
                outCntvs[i], info);
                

            SyList<SyCntv*> in;    
            SyList<SyCntv*> out;    
            out.push_back(outCntvInfo.out);
            bool latch = false;
            if(outCntvInfo.type == OutCntvInfo::LATCH) {
                //don't create latch for local variables
                latch = !stmt2Gates->isLocal(outCntvInfo.out);
            }    
                
            if(!alwaysInfo->isComb() && 
                !stmt2Gates->isLocal(outCntvInfo.out))
            {
                if(outCntvInfo.type == OutCntvInfo::LATCH)
                {
                    //This may be under some switch
                    in.push_back(outCntvInfo.rep);
                    in.push_back(outCntvInfo.out);
                    in.push_back(outCntvInfo.cond);
                    Expr2GatesRetType *res = stmt2Gates->createCellInst(
                        0, &in, 1, 2, MUX);
                    in.clear();
                    in.push_back(res->outputs->head->val);
                    delete res;
                }
                else {
                    in.push_back(outCntvInfo.rep);
                }    
                in.push_back(clk);
                CellType type = alwaysInfo->getCellType(outCntvInfo.out->toString());
                switch (type) 
                {
                    case FFR:
                    {
                        createInputList(in, outCntvInfo.out, false, resetMap);
                        break;
                    }    
                    case FFS:
                    {
                        createInputList(in, outCntvInfo.out, true, setMap);
                        break;
                    }
                    case FFRS:
                    {
                        createInputList(in, outCntvInfo.out, false, resetMap);
                        createInputList(in, outCntvInfo.out, true, setMap);
                        break;
                    }
                    case FFSR:
                    {
                        createInputList(in, outCntvInfo.out, false, resetMap);
                        createInputList(in, outCntvInfo.out, true, setMap);
                        break;
                    }
                    case FF: {
                        break;
                    }
                    default: {
                        assert(0);
                    }    
                }
                stmt2Gates->createCellInst(&out, &in, 1, in.size(), type);
            }
            else if(latch/*outCntvInfo.type == OutCntvInfo::LATCH*/)
            {
                in.push_back(outCntvInfo.rep);
                in.push_back(outCntvInfo.cond);
                stmt2Gates->createCellInst(&out, &in, 1, 1, LD);
            }
            else 
            {
                in.push_back(outCntvInfo.rep);
                stmt2Gates->createCellInst(&out, &in, 1, 1, BUF);
            }
        }    

    }

    void
    Always2Gates::createInputList(
        SyList<SyCntv*>& inputs,
        SyCntv *out,
        bool isSet,
        map<SyCntv*, SyCntv*>& setResetMap)
    {
        SyCntv *setReset = const_cast<SyCntv*>( dynamic_cast<const SyCntv*>(
            this->getCurrSynthMod()->findObject(
            isSet ? alwaysInfo->getSetName(out->toString()) :
            alwaysInfo->getResetName(out->toString()))));
        assert(setReset);
        if(alwaysInfo->isNegEdge(isSet ? 
            alwaysInfo->getSetName(out->toString()) : alwaysInfo->getResetName(out->toString())))
        {
            map<SyCntv*, SyCntv*>::const_iterator iter = setResetMap.find(setReset);
            if(iter != setResetMap.end()) {
                inputs.push_back(iter->second);
            }    
            else 
            {
                SyScNet *net = this->createNet();
                this->createInv(net, setReset);
                inputs.push_back(net);
                setResetMap.insert(make_pair(setReset, net));
            }
        }
        else {
            inputs.push_back(setReset);
        }    
    }

}
