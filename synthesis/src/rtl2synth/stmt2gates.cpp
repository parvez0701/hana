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
#include "stmt2gates.h"
#include "always2gates.h"
#include "ftcall2gates.h"

#include "systmt.h"
#include "namemgr.h"
#include "assignstmtstrans.h"

using std::make_pair;
namespace Synth
{

    OutCntvInfo::OutCntvInfo() : out(0), rep(0),
        cond(0), type(UNSET) {
    }    
    

    OutCntvInfo::OutCntvInfo(SyCntv* o, 
        SyCntv* r, SyCntv* c, OutType t) :
        out(o), rep(r), cond(c), type(t) {
    }

    OutCntvInfo::~OutCntvInfo() {
    }

    bool
    OutCntvInfo::operator<(
        const OutCntvInfo& outCntvInfo) const {
        return out < outCntvInfo.out;
    }    


    Stmt2Gates::Stmt2Gates(    const SyModule    *rmod, 
                            const SyModule    *smod, 
                            const SyScope    *sscope): 
                            ConVisitorBase(rmod, smod, sscope),
                            expr2Gates(new Expr2Gates(rmod, smod, sscope)),
                            argOutCntvInfoSet(0),
                            always(0)
    {
    }

    Stmt2Gates::~Stmt2Gates()
    {
        delete expr2Gates;
    }

    void
    Stmt2Gates::setCurrMod(SyModule *mod)
    {
        this->setCurrModInternal(mod);
        if(expr2Gates) {
            expr2Gates->setCurrMod(mod);
        }    
    }

    void
    Stmt2Gates::setCurrRtlMod(SyModule *mod)
    {
        this->setCurrRtlModInternal(mod);
        if(expr2Gates) {
            expr2Gates->setCurrRtlMod(mod);
        }    
    }

    void
    Stmt2Gates::setCurrSynthMod(SyModule *mod)
    {
        this->setCurrSynthModInternal(mod);
        if(expr2Gates) {
            expr2Gates->setCurrSynthMod(mod);
        }    
    }

    void
    Stmt2Gates::setCurrScope(SyScope *sc)
    {
        this->setCurrScopeInternal(sc);
        if(expr2Gates) {
            expr2Gates->setCurrScope(sc);
        }    
    }    

    Out2Rep
    Stmt2Gates::getOut2Rep() {
        return argOut2Rep;
    }

    OutCntvInfoSet*
    Stmt2Gates::getOutCntvInfoSet() {
        return argOutCntvInfoSet;
    }

    void
    Stmt2Gates::setOut2Rep(const Out2Rep& o2r) {
        argOut2Rep = o2r;
    }

    void

    Stmt2Gates::setOutCntvInfoSet(OutCntvInfoSet *ocis) {
        argOutCntvInfoSet = ocis;
    }    

    TaskFuncCallInfo
    Stmt2Gates::getTaskFuncCallInfo() const
    {
        return FuncTaskCall2Gates::instance()->getTaskFuncCallInfo();
    }

    void
    Stmt2Gates::visit(const SyUnary *expr)
    {
        expr->accept(expr2Gates);
    }    

    void
    Stmt2Gates::visit(const SyBinary *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyVPort *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyScPort *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyVNet *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyScNet *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyVReg *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyScReg *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyConcat *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyBitSel *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyPartSel *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyCondExpr *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyConst *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyXOrZ *expr)
    {
        expr->accept(expr2Gates);
    }    

    void
    Stmt2Gates::visit(const SyFuncCall *expr)
    {
        expr->accept(expr2Gates);
    }    
    void
    Stmt2Gates::visit(const SyAlways *a)
    {
        always = a;
        Always2Gates always2Gates(this->getCurrRtlMod(),
            this->getCurrSynthMod(), this->getCurrScope());

        SyAssignStmtsTrans trans(this->getCurrSynthMod());
        trans.visit(const_cast<SyList<SyStmt*>&>(a->getStmts()));
        
        always2Gates.setStmt2Gates(this);    
        always->accept(&always2Gates);    
    }

    void
    Stmt2Gates::visit(const SyIf *if_)
    {
        Out2Rep out2Rep = argOut2Rep;
        OutCntvInfoSet* outCntvInfoSet = new OutCntvInfoSet();
        vector<OutCntvInfoSet*> info;
        SyAssignStmtsTrans trans(this->getCurrSynthMod());
        trans.visit(const_cast<SyList<SyStmt*>&>(if_->getThenPart()));
        trans.visit(const_cast<SyList<SyStmt*>&>(if_->getElsePart()));

        info.push_back(this->createGates(if_->getThenPart(), out2Rep));
        info.push_back(this->createGates(if_->getElsePart(), out2Rep));
        vector<SyCntv*> outCntvs = this->getOutCntvs(info);


        const SyExpr *ifcond = if_->getCond();
        ifcond->accept(this);
        SyList<SyCntv*> ifcondCntvs = *(expr2Gates->getResult()->outputs);
        if(ifcondCntvs.size() > 1)
        {
            SyCntv *out = expr2Gates->create2InputGateTree(&ifcondCntvs, 
                OR)->outputs->head->val;
            ifcondCntvs.clear();
            ifcondCntvs.push_back(out);
        }
        SyCntv *thenCntv = 0;
        SyCntv *elseCntv = 0;

        for(unsigned int i = 0; i < outCntvs.size(); ++i)
        {
            OutCntvInfo outCntvInfo;
            outCntvInfo.out = outCntvs[i];
            
            OutCntvInfoSet::const_iterator iter0 = info[0]->find(outCntvInfo);
            OutCntvInfoSet::const_iterator iter1 = info[1]->find(outCntvInfo);

            assert((iter0 != info[0]->end()) || (iter1 != info[1]->end()));
            SyList<SyCntv*> in;

            if((iter0 != info[0]->end()) && (iter1 != info[1]->end()))
            {
                if(this->checkSetReset(outCntvs[i], if_)) 
                {
                    in.push_back(expr2Gates->createVccOrGround());
                    in.push_back(iter1->rep);
                    Expr2GatesRetType *res = expr2Gates->createCellInst(
                        0, &in, 1, 2, AND);
                    outCntvInfo.rep = res->outputs->head->val;    
                }
                else
                {
                    in.push_back(iter0->rep);
                    in.push_back(iter1->rep);
                    in.merge(ifcondCntvs);
                    Expr2GatesRetType *res = expr2Gates->createCellInst(
                        0, &in, 1, 2, MUX);
                    outCntvInfo.rep = res->outputs->head->val;    
                }    

                if(!thenCntv) {
                    thenCntv = ifcondCntvs.head->val;
                }
                SyList<SyCntv*> muxin;
                muxin.push_back(iter0->cond);
                muxin.push_back(iter1->cond);
                muxin.push_back(thenCntv);

                outCntvInfo.cond = expr2Gates->createCellInst(
                    0, &muxin, 1, 2, MUX)->outputs->head->val;

                outCntvInfo.type = ((iter0->type == OutCntvInfo::LATCH) ||
                    (iter1->type == OutCntvInfo::LATCH)) ? OutCntvInfo::LATCH:
                    OutCntvInfo::COMB;
            }
            else if(iter0 != info[0]->end())
            {
                in.push_back(iter0->rep);
                if(!thenCntv) {
                    thenCntv = ifcondCntvs.head->val;
                }
                in.push_back(thenCntv);
                outCntvInfo.rep = expr2Gates->create2InputGateTree(&in,
                    AND)->outputs->head->val;
                in.clear();    
                in.push_back(thenCntv);
                if(iter0->cond) {
                    in.push_back(iter0->cond);
                }    
                outCntvInfo.cond = (in.size()==1) ? in.head->val :
                expr2Gates->createCellInst(0, &in,
                    1, 2, AND)->outputs->head->val;
                outCntvInfo.type = OutCntvInfo::LATCH;
            }    
            else
            {
                in.push_back(iter1->rep);
                if(!elseCntv)
                {
                    if(!thenCntv) {
                        thenCntv = ifcondCntvs.head->val;
                    }
                    SyList<SyCntv*> tin;
                    tin.push_back(thenCntv);
                    Expr2GatesRetType* result = expr2Gates->createCellInst(0, &tin, 1, 1, NOT);
                    elseCntv = result->outputs->head->val;
                }
                in.push_back(elseCntv);
                outCntvInfo.rep = expr2Gates->create2InputGateTree(&in,
                    AND)->outputs->head->val;
                in.clear();    
                in.push_back(elseCntv);
                if(iter1->cond) {
                    in.push_back(iter1->cond);
                }    
                outCntvInfo.cond = (in.size()==1) ? in.head->val :
                    expr2Gates->createCellInst(0, &in,
                    1, 2, AND)->outputs->head->val;
                outCntvInfo.type = OutCntvInfo::LATCH;
            }    
            outCntvInfoSet->insert(outCntvInfo);
        }    
        argOutCntvInfoSet = outCntvInfoSet;

    }

    void
    Stmt2Gates::visit(const SyCase *case_)
    {
        Out2Rep out2Rep = argOut2Rep;
        vector<OutCntvInfoSet*> info;
        const SyExpr *select = case_->getSelect();
        const SyList<SyCaseItem*>& items = case_->getCaseItems();
        //Assumtion: items are sorted by label index

        SyAssignStmtsTrans trans(this->getCurrSynthMod());
        for(SyNode<SyCaseItem*>* node = items.head; node; 
            node = node->next) 
        {
            trans.visit(const_cast<SyList<SyStmt*>&>(node->val->getStmts()));
            info.push_back(this->createGates(node->val->getStmts(), out2Rep));
        }

        vector<SyCntv*> labels;

        vector<SyCntv*> outCntvs = this->getOutCntvs(info);
        vector<bool> muxFlag = this->checkIfMux(info);
        assert(outCntvs.size() == muxFlag.size());
        OutCntvInfoSet *outCntvInfoSet = new OutCntvInfoSet();

        for(unsigned int i = 0; i < outCntvs.size(); ++i)
        {
            if(muxFlag[i]) {
                outCntvInfoSet->insert(this->createMuxLogic(outCntvs[i],
                    select, info));
            }        
            else 
            {
                if(labels.empty()) {
                    labels = this->createLabelLogic(case_);
                }    
                outCntvInfoSet->insert(this->createCaseLogic(outCntvs[i],
                    info, labels));
            }        
        }    
        argOutCntvInfoSet = outCntvInfoSet;
    }

    void
    Stmt2Gates::visit(const SyBlockAssign *assign)
    {
        if(this->transformAndVisitBlockAssign(assign)) {
            return;
        }    
        Out2Rep out2Rep = argOut2Rep;
        argOutCntvInfoSet = this->createBuf(assign, out2Rep);
    }

    void
    Stmt2Gates::visit(const SyNonBlockAssign *assign)
    {
        Out2Rep out2Rep = argOut2Rep;
        argOutCntvInfoSet = this->createBuf(assign, out2Rep);
    }
    void
    Stmt2Gates::visit(const SyContAssign *assign)
    {
        SyList<SyCntv*> left;
        SyList<SyCntv*> right;
        assign->getRExpr()->accept(this);
        right = *(expr2Gates->getResult()->outputs);
        assign->getLExpr()->accept(this);
        left = *(expr2Gates->getResult()->outputs);

        int diff = left.size()-right.size();

        while(diff-- > 0) {
            right.push_front(expr2Gates->createVccOrGround(false));
        }
        SyNode<SyCntv*> *lnode = left.head;
        SyNode<SyCntv*> *rnode = right.head;

        diff = right.size()-left.size();
        while(diff-- > 0) {
            rnode = rnode->next;
        }
        Out2Rep out2Rep;
        for(; lnode && rnode; lnode = lnode->next, rnode = rnode->next) {
            out2Rep.insert(make_pair(lnode->val, rnode->val));
        }    


        OutCntvInfoSet *outCntvInfoSet = FuncCallMerge::instance()->merge(
            this->getCurrScope());
        if(outCntvInfoSet) {
            this->mergeRep(outCntvInfoSet, out2Rep);
        }    

        this->createGates(outCntvInfoSet, out2Rep);
        argOut2Rep.clear();
        argOutCntvInfoSet = 0;
    }

    void
    Stmt2Gates::visit(const SyTaskCall* tcall)
    {
        Out2Rep out2Rep = argOut2Rep;
        const SyTaskOrFunc *task = tcall->getTask();
        SyList<SyScCntv*> formals;
        SyList<SyCntv*> actuals;
        const vector<SyExpr*>& args = tcall->getArgs();
        const SyList<SyScPort*>& ports = task->getPorts();
        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next) {
            formals.push_back(node->val);
        }
        SyList<std::pair<SyVCntv*,SyList<SyScCntv*> > > info =
            GetVectorScalarPairs(formals, this->getCurrSynthMod()->getName(),
            task->getName());
        assert(info.size() == args.size());    
        SyNode<std::pair<SyVCntv*,SyList<SyScCntv*> > >* inode = info.head;

        for(int i = 0; (i < args.size()) && inode; ++i, inode = inode->next)
        {
            args[i]->accept(this);
            SyList<SyCntv*> outputs = *(expr2Gates->getResult()->outputs);
            SyNode<SyCntv*>* node = outputs.head;

            int diff = outputs.size() - inode->val.second.size();
            
            if(diff >= 0) {
                while(diff--) {
                    node = node->next;
                }
            }
            else {
                while(!diff) 
                {
                    ++diff;
                    outputs.push_front(expr2Gates->createNet());
                }    
            }
            actuals.merge(outputs);
        }

        StmtsConverter s(formals, actuals, task->getStmts());
        SyList<SyStmt*> stmts = s();

        argOutCntvInfoSet = this->createGates(stmts, argOut2Rep);
 
    }

    OutCntvInfo
    Stmt2Gates::createMuxLogic(const SyCntv *cntv, const SyExpr* sel,
        const vector<OutCntvInfoSet*>& info) 
    {
        SyList<SyCntv*> in;
        OutCntvInfo cntvInfo;
        cntvInfo.out = const_cast<SyCntv*>(cntv);
        OutCntvInfo::OutType type = OutCntvInfo::COMB;
        SyList<SyCntv*> conds;
        for(unsigned int i = 0; i < info.size(); ++i)
        {
            OutCntvInfoSet::const_iterator iter = info[i]->find(cntvInfo);
            assert(iter != info[i]->end());
            in.push_back(iter->rep);
            if(iter->type == OutCntvInfo::LATCH) {
                type = OutCntvInfo::LATCH;
            }    
            conds.push_back(iter->cond);
        }
        cntvInfo.out = const_cast<SyCntv*>(cntv);
        cntvInfo.type = type;    

        sel->accept(this);
        SyList<SyCntv*> result = *expr2Gates->getResult()->outputs;
        in.reverse();
        in.merge(result);
        conds.reverse();
        conds.merge(result);

        cntvInfo.rep= expr2Gates->createCellInst(0, &in, 1, info.size(), 
            MUX)->outputs->head->val;
        cntvInfo.cond= expr2Gates->createCellInst(0, &conds, 1, info.size(), 
            MUX)->outputs->head->val;
        
        return cntvInfo;
    }
    
    OutCntvInfo
    Stmt2Gates::createCaseLogic(const SyCntv *cntv, const SyExpr* sel,
        const vector<OutCntvInfoSet*>& info) 
    {
        SyList<SyCntv*> conds;
        SyList<SyCntv*> ifconds;
        OutCntvInfo cntvInfo;
        cntvInfo.out = const_cast<SyCntv*>(cntv);

        sel->accept(this);
        SyList<SyCntv*> selCntvs = *(expr2Gates->getResult()->outputs);
        selCntvs.reverse();

        SyList<SyCntv*> xnorOut;
        SyList<SyCntv*> orOut;
        OutCntvInfo::OutType type = OutCntvInfo::COMB;
        for(unsigned int i = 0; i < info.size(); ++i)
        {
            OutCntvInfoSet::const_iterator iter = info[i]->find(cntvInfo);
            if(iter == info[i]->end()) 
            {
                type = OutCntvInfo::LATCH;
                continue;
            }    
            if(iter->cond){
                 conds.push_back(iter->cond);
            }     
            SyConst const_(i, false, false);
            const_.accept(this);
            SyList<SyCntv*> result = *(expr2Gates->getResult()->outputs);
            result.reverse();
            assert(selCntvs.size() >= result.size());
            SyNode<SyCntv*>* n1 = result.head;
            SyNode<SyCntv*>* n2 = selCntvs.head;

            for(; n1 && n2; n1 = n1->next, n2= n2->next) 
            {
                SyList<SyCntv*> in;
                in.push_back(n1->val);
                in.push_back(n2->val);
                xnorOut.push_back(expr2Gates->
                    createCellInst(0, &in, 1, 2, XNOR)->outputs->head->val);
            }

            for(; n2; n2 = n2->next)
            {
                SyList<SyCntv*> in;
                in.push_back(n2->val);
                xnorOut.push_back(expr2Gates->
                    createCellInst(0, &in, 1, 1, NOT)->outputs->head->val);
            }        

            SyCntv* res = 0;
            if(xnorOut.size() == 1) {
                res = xnorOut.head->val;
            }
            else {
                res = expr2Gates->create2InputGateTree(&xnorOut, AND)->
                    outputs->head->val;
            }        
            ifconds.push_back(res);    
            SyList<SyCntv*> in;
            in.push_back(res);
            in.push_back(iter->rep);
            res = expr2Gates->create2InputGateTree(&in, AND)->outputs->head->val;    
            orOut.push_back(res);
            if(iter->type == OutCntvInfo::LATCH) {
                type = OutCntvInfo::LATCH;
            }
        }    
        cntvInfo.rep = (orOut.size() == 1) ? orOut.head->val :
            expr2Gates->create2InputGateTree(&orOut, OR)->
            outputs->head->val;
        
        cntvInfo.type = type;
        cntvInfo.cond = conds.empty() ? 0 : (conds.size() == 1) ? conds.
            head->val : expr2Gates->create2InputGateTree(&conds, OR)->
            outputs->head->val; 
        SyCntv    *tmp = (ifconds.size() == 1) ? ifconds.head->val :
            expr2Gates->create2InputGateTree(&ifconds, OR)->outputs->head->val;
        SyList<SyCntv*> andin;
        andin.push_back(cntvInfo.cond);
        andin.push_back(tmp);
        cntvInfo.cond = expr2Gates->createCellInst(0, &andin, 1, 2, AND)->
            outputs->head->val;
        return cntvInfo;
    }    

    void
    Stmt2Gates::removeOverriddenEntries(vector<OutCntvInfoSet*>& info)const
    {
        reverse(info.begin(), info.end());
        set<SyCntv*> comb;
        for(unsigned int i = 0; i < info.size(); ++i)
        {
            //remove entries if the later entry makes it a combinational
            //logic
            OutCntvInfoSet::const_iterator iter = info[i]->begin();
            OutCntvInfoSet::const_iterator iterEnd = info[i]->end();

            for(; iter != iterEnd; ++iter)
            {
                if(iter->type == OutCntvInfo::COMB) 
                {
                    comb.insert(iter->out);
                    for(unsigned int j = i+1; j < info.size(); ++j) {
                        info[j]->erase(*iter);
                    }    
                }
            }
        }

        //Irrespective of position, if a combinational connective
        //overrides any latch infered before or after.
        for(unsigned int i = 0; i < info.size(); ++i)
        {
            OutCntvInfoSet::iterator iter = info[i]->begin();
            OutCntvInfoSet::iterator iterEnd = info[i]->end();

            for(; iter != iterEnd; ++iter)
            {
                if(comb.find(iter->out) != comb.end()) {
                    OutCntvInfo* tmp = const_cast<OutCntvInfo*>(&(*iter));
                    tmp->type = OutCntvInfo::COMB;
                }
            }
        }    
    }

    OutCntvInfo
    Stmt2Gates::getMergedOutCntvInfo(const SyCntv *outCntv,
        const vector<OutCntvInfoSet*>& info) const
    {    
        vector<OutCntvInfo> outCntvInfo = 
            this->getOutCntvInfos(outCntv, info);
        assert(!outCntvInfo.empty());
        reverse(outCntvInfo.begin(), outCntvInfo.end());

        SyList<SyCntv*> in;
        SyList<SyCntv*> out;
        in.push_back(outCntvInfo[0].rep);
        if(outCntvInfo[0].cond) {
            in.push_back(outCntvInfo[0].cond);
        }    
        Expr2GatesRetType *ret = (in.size() == 1) ? expr2Gates->createCellInst(
            0, &in, 1, 1, BUF):
            expr2Gates->create2InputGateTree(&in, AND);
        if(outCntvInfo.size() == 1)
        {
            OutCntvInfo tmp(outCntvInfo[0].out,
                ret->outputs->head->val, outCntvInfo[0].cond,
                outCntvInfo[0].type);
            return tmp;
        }
        OutCntvInfo::OutType type = outCntvInfo[0].type;
        SyCntv* lastOut = ret->outputs->head->val;
        SyList<SyCntv*> conds;
        if(outCntvInfo[0].cond){
            conds.push_back(outCntvInfo[0].cond);
        }    
        for(unsigned int j = 1; j < outCntvInfo.size(); ++j)
        {
            SyList<SyCntv*> in;
            SyList<SyCntv*> out;
            in.push_back(outCntvInfo[j].rep);
            in.push_back(lastOut);
            in.push_back(outCntvInfo[j].cond);
            ret = expr2Gates->createCellInst(0, &in, 1, 2, MUX);
            lastOut = ret->outputs->head->val;
            conds.push_back(outCntvInfo[j].cond);
            if(outCntvInfo[j].type == OutCntvInfo::LATCH) {
                type = OutCntvInfo::LATCH;
            }    
        }    
        ret = (conds.size() == 1) ? expr2Gates->createCellInst(0, &conds,
            1, 1, BUF) : expr2Gates->create2InputGateTree(&conds, OR);
        OutCntvInfo tmp(const_cast<SyCntv*>(outCntv), 
            lastOut, ret->outputs->head->val,  type);
        return tmp;    
    }    


    void
    Stmt2Gates::addRep(SyCntv *out, SyCntv *rep, Out2Rep& out2Rep) const
    {
        assert(out && rep);
        out2Rep.erase(out);
        out2Rep.insert(make_pair(out, rep));
    }

    SyCntv*
    Stmt2Gates::getRep(SyCntv *out, Out2Rep& out2Rep) const
    {
        Out2Rep::iterator iter = out2Rep.find(out);
        if(iter == out2Rep.end()) {
            return NULL;
        }    
        return iter->second;
    }    

    vector<SyCntv*>
    Stmt2Gates::getOutCntvs(const vector<OutCntvInfoSet*>& info) const
    {
        set<SyCntv*> doneSet;
        vector<SyCntv*> outCntvs;
        for(unsigned int i = 0; i < info.size(); ++i) {
            OutCntvInfoSet::const_iterator iter = info[i]->begin();
            for(; iter != info[i]->end(); ++iter) {
                if(doneSet.find(iter->out) == doneSet.end())
                {
                    outCntvs.push_back(iter->out);
                    doneSet.insert(iter->out);
                }    
            }
        }
        return outCntvs;
    }

    void
    Stmt2Gates::mergeRep(const OutCntvInfoSet *outCntvInfoSet, 
        Out2Rep& out2Rep) const
    {
        OutCntvInfoSet::const_iterator iter = outCntvInfoSet->begin();
        OutCntvInfoSet::const_iterator iterEnd = outCntvInfoSet->end();

        for(; iter != iterEnd; ++iter) 
        {
            //this->addRep(iter->out, iter->rep, out2Rep);
            if(iter->type == OutCntvInfo::COMB) {
                this->addRep(iter->out, iter->rep, out2Rep);
            }
            else 
            {
                Out2Rep::const_iterator tmp = out2Rep.find(iter->out);
                if(tmp == out2Rep.end()) {
                    this->addRep(iter->out, iter->rep, out2Rep);
                }
                else
                {
                    SyList<SyCntv*> in;
                    in.push_back(iter->rep);
                    in.push_back(tmp->second);
                    in.push_back(iter->cond);
                    this->addRep(iter->out, expr2Gates->createCellInst(
                        0, &in, 1, 2, MUX)->outputs->head->val, out2Rep);
                }        
            }    
        }    
            
    }

       vector<OutCntvInfo>
    Stmt2Gates::getOutCntvInfos(const SyCntv* cntv,
        const vector<OutCntvInfoSet*>& info) const
    {
        vector<OutCntvInfo> outInfo;
        for(unsigned int i = 0; i < info.size(); ++i) 
        {
            OutCntvInfo tmp;
            tmp.out = const_cast<SyCntv*>(cntv);
            OutCntvInfoSet::const_iterator iter =
                info[i]->find(tmp);
            if(iter != info[i]->end()) {
                outInfo.push_back(*iter);
            }
        }    
        return outInfo;
    }    
    vector<bool>
    Stmt2Gates::checkIfMux(const vector<OutCntvInfoSet*>& info) const
    {
        vector<bool> ret;
        vector<SyCntv*> outCntvs = this->getOutCntvs(info);

        for(unsigned int i = 0; i < outCntvs.size(); ++i)
        {
            SyCntv *cntv = outCntvs[i];
            ret.push_back(true);
            for(unsigned int j = 0; j < info.size(); ++j) 
            {
                OutCntvInfo tmp;
                tmp.out = cntv;
                if(info[j]->find(tmp) == info[j]->end()) 
                {
                    ret[i] = false;
                    break;
                }
            }    
        }    
        return ret;
    }    

    OutCntvInfoSet*
    Stmt2Gates::createGates(const SyList<SyStmt*>& stmts, Out2Rep out2Rep)
    {
        vector<OutCntvInfoSet*> info;
        for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next)
        {
            argOut2Rep = out2Rep;
            node->val->accept(this);
            if(OutCntvInfoSet *cntvSet = 
                FuncCallMerge::instance()->merge(this->getCurrScope()))
            {    
                info.push_back(cntvSet);
                this->mergeRep(info[info.size()-1], out2Rep);
            }    
            info.push_back(argOutCntvInfoSet);
            this->mergeRep(info[info.size()-1], out2Rep);
        }    

        this->removeOverriddenEntries(info);

        vector<SyCntv*> outCntvs = this->getOutCntvs(info);
        OutCntvInfoSet* outCntvInfoSet = new OutCntvInfoSet();


        for(unsigned int i = 0; i < outCntvs.size(); ++i) {
            outCntvInfoSet->insert(this->getMergedOutCntvInfo(
                outCntvs[i], info));
        }    
        return outCntvInfoSet;
                    
                    
    }

    OutCntvInfoSet*
    Stmt2Gates::createBuf(const SyAssign *assign, Out2Rep out2Rep)
    {
        OutCntvInfoSet* outCntvInfoSet = new OutCntvInfoSet();
        SyList<SyCntv*> left;
        SyList<SyCntv*> right;
        SyList<SyCntv*> extend;

        assign->getRExpr()->accept(this);
        right = *(expr2Gates->getResult()->outputs);
        assign->getLExpr()->accept(this);
        left = *(expr2Gates->getResult()->outputs);

        int diff = left.size()-right.size();

        while(diff-- > 0) {
            right.push_front(expr2Gates->createVccOrGround(false));
        }
        SyNode<SyCntv*> *lnode = left.head;
        SyNode<SyCntv*> *rnode = right.head;

        diff = right.size()-left.size();
        while(diff-- > 0) {
            rnode = rnode->next;
        }

        for(; lnode && rnode; lnode = lnode->next, rnode = rnode->next)
        {

            OutCntvInfo outCntvInfo;
            outCntvInfo.out = lnode->val;
            SyCntv *net = this->createNet();
            SyCntv *rep = this->getRep(rnode->val, out2Rep);
            if(!rep) {
                rep = rnode->val;
            }
            SyList<SyCntv*> in;
            SyList<SyCntv*> out;
            in.push_back(rep);
            out.push_back(net);
            expr2Gates->createCellInst(&out, &in, 1, 1, BUF);
            outCntvInfo.rep = net;
            //tautology
            outCntvInfo.cond = expr2Gates->createVccOrGround();
            //outCntvInfo.cond = rep;
            outCntvInfo.type = OutCntvInfo::COMB;
            outCntvInfoSet->insert(outCntvInfo);
        }    
        return outCntvInfoSet;
    }    



    Expr2GatesRetType*
    Stmt2Gates::createCellInst(    SyList<SyCntv*> *outs, 
        SyList<SyCntv*> *ins, 
        short outsize, 
        short insize, 
        CellType type) {
        return expr2Gates->createCellInst(outs, ins, outsize, insize, type);
    }    

    bool
    Stmt2Gates::isLocal(const SyCntv *cntv) const
    {
        if(const SyScPort *port = dynamic_cast<const SyScPort*>(cntv)) {
            if(this->getCurrSynthMod()->findObject(port->getIxName()) ==
                port)
            {
                if(port->getNetType() == SyScNet::REG) {
                    return false;
                }

            }
            return true;
        }    
        const SyScReg *reg = dynamic_cast<const SyScReg*>(cntv);
        if(!reg) {
            return true;
        }
        return reg->isLocal();
    }

    void
    Stmt2Gates::createGates(SyList<SyCntv*>& left, SyList<SyCntv*>& right)
    {
        int diff = left.size()-right.size();

        while(diff-- > 0) {
            right.push_front(expr2Gates->createVccOrGround(false));
        }
        SyNode<SyCntv*> *lnode = left.head;
        SyNode<SyCntv*> *rnode = right.head;

        diff = right.size()-left.size();
        while(diff-- > 0) {
            rnode = rnode->next;
        }
        set<SyCntv*> doneSet;
        for(; lnode && rnode; lnode = lnode->next, rnode = rnode->next)
        {

            OutCntvInfo outCntvInfo;
            outCntvInfo.out = lnode->val;

            SyList<SyCntv*> in;
            SyList<SyCntv*> out;
            in.push_back(rnode->val);
            out.push_back(lnode->val);
            doneSet.insert(lnode->val);
            CellType type = BUF;
            
            expr2Gates->createCellInst(&out, &in, 1, 1, type);
        }    
        if(argOutCntvInfoSet)
        {
            OutCntvInfoSet::const_iterator iter = argOutCntvInfoSet->begin();
            for(; iter != argOutCntvInfoSet->end(); ++iter)
            {
                if(doneSet.find(iter->out) != doneSet.end()) {
                    continue;
                }    
                SyList<SyCntv*> in;
                SyList<SyCntv*> out;
                in.push_back(iter->rep);
                out.push_back(iter->out);
                CellType type = BUF;
                if(this->isLocal(iter->out)) {
                    //handling only globals
                    continue;
                }    
                if(iter->type == OutCntvInfo::LATCH) 
                {
                    type = LD;
                    in.push_back(iter->cond);
                }
                expr2Gates->createCellInst(&out, &in, 1, 1, type);
            }
        }
    }    

    void
    Stmt2Gates::createGates(OutCntvInfoSet *outCntvInfo, Out2Rep& out2Rep)
    {
        //This function is called in case of continuous assign only
        Out2Rep::const_iterator iter = out2Rep.begin();
        for(; iter != out2Rep.end(); ++iter)
        {
            SyList<SyCntv*> in;
            SyList<SyCntv*> out;
            in.push_back(iter->second);
            out.push_back(iter->first);
            CellType type = BUF;
            if(outCntvInfo)
            {
                OutCntvInfo tmp;
                tmp.out = iter->first;
                OutCntvInfoSet::const_iterator oinfo = outCntvInfo->find(tmp);
                if(oinfo != outCntvInfo->end())
                {
                    if(oinfo->type == OutCntvInfo::LATCH) {
                        if(!this->isLocal(oinfo->out)) 
                        {
                            type = LD;
                            in.push_back(oinfo->cond);
                        }
                    }
                }    
            }
            expr2Gates->createCellInst(&out, &in, 1, 1, type);
        }
    }
    bool 
    Stmt2Gates::checkSetReset(const SyCntv *out,  const SyIf *if_) const
    {
        if(!always) {
            return false;
        }    
        const AlwaysInfo_* info = SyProcMgr::instance()->getAlwaysInfo(always);
        if(info->isComb()) {
           return false;
        }   
        string condString = info->getCondString(if_);
        string outName = out->toString();
        

        if((info->getSetName(outName) != condString) && 
            (info->getResetName(outName) != condString)) {
            return false;
        }    
        return true;

    }

    vector<SyCntv*>
    Stmt2Gates::createLabelLogic(const SyCase *case_) const
    {
        vector<SyCntv*> labelCntvs;
        const SyList<SyCaseItem*>& items = case_->getCaseItems();
        const SyExpr *sel = case_->getSelect();
        sel->accept(expr2Gates);
        SyList<SyCntv*> selCntvs = *(expr2Gates->getResult()->outputs);
        selCntvs.reverse();

        for(SyNode<SyCaseItem*>* item = items.head; item; item = item->next)
        {
            if(item->val->getStmts().empty()) 
            {
                labelCntvs.push_back(0);
                continue;
            }    
            item->val->getLabel()->accept(expr2Gates);
            SyList<SyCntv*> lcntvs = *(expr2Gates->getResult()->outputs);
            lcntvs.reverse();

            //assert(selCntvs.size() >= lcntvs.size());

            SyNode<SyCntv*>* n1 = lcntvs.head;
            SyNode<SyCntv*>* n2 = selCntvs.head;
            SyList<SyCntv*> xnorOut;

            for(; n1 && n2; n1 = n1->next, n2= n2->next) 
            {
                SyList<SyCntv*> in;
                in.push_back(n1->val);
                in.push_back(n2->val);
                Expr2GatesRetType *tmp = expr2Gates->
                    createCellInst(0, &in, 1, 2, XNOR);
                xnorOut.push_back(tmp->outputs->head->val);
                delete tmp;
            }

            for(; n2; n2 = n2->next)
            {
                SyList<SyCntv*> in;
                in.push_back(n2->val);
                Expr2GatesRetType *tmp = expr2Gates->createCellInst(0, &in, 1, 1, NOT);
                xnorOut.push_back(tmp->outputs->head->val);
                delete tmp;
            }        
            if(xnorOut.size() == 1) {
                labelCntvs.push_back(xnorOut.head->val);
            }
            else {
                Expr2GatesRetType *tmp = expr2Gates->create2InputGateTree(&xnorOut, AND);
                labelCntvs.push_back(tmp->outputs->head->val);
                delete tmp;
            }        
            
        }
        return labelCntvs;
    }
    OutCntvInfo
    Stmt2Gates::createCaseLogic(const SyCntv *cntv,
        const vector<OutCntvInfoSet*>& info,
        const vector<SyCntv*>& labelCntvs) 
    {
        OutCntvInfo cntvInfo;
        cntvInfo.out = const_cast<SyCntv*>(cntv);
        OutCntvInfo::OutType type = OutCntvInfo::COMB;
        for(unsigned int i = 0; i < info.size(); ++i)
        {
            OutCntvInfoSet::iterator iter = info[i]->find(cntvInfo);
            if(iter == info[i]->end()) 
            {
                type = OutCntvInfo::LATCH;
                continue;
            }    
            SyList<SyCntv*> in;
            in.push_back(labelCntvs[i]);
            in.push_back(iter->cond);
            Expr2GatesRetType *tmp = expr2Gates->createCellInst(0, &in, 1, 2, AND);

            const_cast<OutCntvInfo&>(*iter).cond = tmp->outputs->head->val;
            delete tmp;
            //if(type == OutCntvInfo::LATCH) {
            //    const_cast<OutCntvInfo&>(*iter).type = OutCntvInfo::LATCH;
            //}    
        }    
        if(type == OutCntvInfo::LATCH)
        {
            for(unsigned int i = 0; i < info.size(); ++i)
            {
                OutCntvInfoSet::iterator iter = info[i]->find(cntvInfo);
                if(iter != info[i]->end()) {
                    const_cast<OutCntvInfo&>(*iter).type = OutCntvInfo::LATCH;
                }
            }
        }    
        return this->getMergedOutCntvInfo(cntv, info);
    }    

    bool
    Stmt2Gates::transformAndVisitBlockAssign(const SyBlockAssign *bassign)
    {
        return false;
        const SyExpr *lexpr = bassign->getLExpr();
        SyExpr *rexpr = const_cast<SyExpr*>(bassign->getRExpr());
        bool changed = false;
        SyList<SyCntv*> flist;
        FlattenLValue(lexpr, flist);
        SyList<SyStmt*> stmts;
        rexpr->repVCntvByConcat();

        SyModule *mod = this->getCurrSynthMod();
        for(SyNode<SyCntv*>* node = flist.head; node; node = node->next)
        {
            SyCntv *lhsc = node->val;
            if(!rexpr->contains(lhsc->toString())) {
                continue;
            }    
            changed = true;
            SyScReg *reg = new SyScReg(ModuleNameManager::instance()->
                createName(mod, ""), lexpr->isSigned());
            mod->addObject(    reg->getIxName().getName(), reg);
            mod->addReg(reg);

            stmts.push_back(new SyBlockAssign(reg, lhsc));
            rexpr->repSubExpr(lhsc->toString(), reg);
        }
        if(changed)
        {
            Out2Rep out2Rep = argOut2Rep;
            stmts.push_back(new SyBlockAssign(lexpr, rexpr));
            argOutCntvInfoSet = this->createGates(stmts, out2Rep);
        }
        return changed;
#if 0    
        const SyExpr *lexpr = bassign->getLExpr();
        const SyExpr *rexpr = bassign->getRExpr();
        if(!rexpr->contains(lexpr->toString())){
            return false;
        }
        SyBlockAssign *b1 = 0;
        SyBlockAssign *b2 = 0;
        SyExpr *tmplhs = 0;
        SyModule *mod = this->getCurrSynthMod();
        if(const SyVCntv *vcntv = dynamic_cast<const SyVCntv*>(lexpr))
        {
            SyVReg *vreg = new SyVReg(ModuleNameManager::instance()->
                createName(mod, vcntv->getName()),
                vcntv->getMsb(), vcntv->getLsb(), vcntv->isSigned());
            mod->addObject(    vreg->getName(), vreg);
            const SyList<SyScCntv*>& newbits = vreg->getBits();
            for(SyNode<SyScCntv*>* node = newbits.head; node; node = node->next)
            {
                SyScReg *reg = dynamic_cast<SyScReg*>(node->val);
                mod->addReg(reg);
            }    
            tmplhs = vreg;
        }
        else if(const SyScCntv *scntv = dynamic_cast<const SyScCntv*>(lexpr))
        {
            SyScReg *reg = new SyScReg(ModuleNameManager::instance()->
                createName(mod, scntv->getIxName().getName()), scntv->isSigned());
            mod->addObject(    reg->getIxName().getName(), reg);
            mod->addReg(reg);
            tmplhs = reg;
        }
        else {
            return false;
        }
        b1 = new SyBlockAssign(tmplhs, lexpr->copy());
        const_cast<SyExpr*>(rexpr)->repSubExpr(lexpr->toString(), tmplhs);
        b2 = new SyBlockAssign(lexpr, rexpr);
        
        Out2Rep out2Rep = argOut2Rep;
        SyList<SyStmt*> stmts;
        stmts.push_back(b1);
        stmts.push_back(b2);
        argOutCntvInfoSet = this->createGates(stmts, out2Rep);
        return true;
#endif        
    }
}
