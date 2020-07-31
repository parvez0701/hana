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

#include "ftcall2gates.h"
#include "always2gates.h"
#include "syexpr.h"
#include "systmt.h"

using std::auto_ptr;
using std::vector;
using std::string;
using std::map;
using std::make_pair;

namespace Synth
{
    FuncTaskCall2Gates* FuncTaskCall2Gates::funcTaskCall2Gates = 0;

    FuncTaskCall2Gates::FuncTaskCall2Gates() : always2Gates(0){
        tfCallInfo.second = NULL;
    }

    FuncTaskCall2Gates::~FuncTaskCall2Gates() {
    }

    FuncTaskCall2Gates*
    FuncTaskCall2Gates::instance()
    {
        if(!FuncTaskCall2Gates::funcTaskCall2Gates) {
            FuncTaskCall2Gates::funcTaskCall2Gates = new FuncTaskCall2Gates();
        }
        return funcTaskCall2Gates;
    }    

    TaskFuncCallInfo
    FuncTaskCall2Gates::getTaskFuncCallInfo(){
        return tfCallInfo;
    }    

    InOutCntvs
    FuncTaskCall2Gates::getInOutInfo(const string& mod, const string& ft) const
    {
        ModFuncTaskCallTable::const_iterator iter1 = modFuncTaskCallTable.find(mod);
        if(iter1 == modFuncTaskCallTable.end()) {
            return InOutCntvs();
        }    
        CallInOuts::const_iterator iter2 = iter1->second.find(ft);
        if(iter2 == iter1->second.end()){
            return InOutCntvs();
        }
        return iter2->second;
    }

    void
    FuncTaskCall2Gates::addInOutInfo(    const string& mod, const string& tf,
                                        InOutCntvs& ioc)
    {
        CallInOuts cio;
        cio.insert(make_pair(tf, ioc));
        modFuncTaskCallTable.insert(make_pair(mod, cio));
    }

    void
    FuncTaskCall2Gates::inlineExpand(    const SyModule *rtl, 
                                        const SyModule *synth,
                                        const SyTaskOrFuncCall* call,
                                        Out2Rep out2Rep)
    {
        const SyTaskOrFunc *tf = call->getTaskOrFunc();
        InOutCntvs inOutCntvs = this->getInOutInfo(synth->getName(), 
            tf->getName());
        //To my undesratnding, there should be just one copy of
        //hardware created for non-reentrant tasks. So following code
        //should stay, but iverilog has a different behviour, will check
        //it later. TBD
        
        //if(!tf->isAuto() && !inOutCntvs.empty()) 
        //{
        //    this->connectPortsWithArgs(call, inOutCntvs);
        //    tfCallInfo = make_pair(new InOutCntvs(inOutCntvs), 
        //        new OutCntvInfoSet());
        //    return;
        //}    

        always2Gates = new Always2Gates(rtl, synth, 0);
        inOutCntvs = this->createNets(tf->getPorts());
        this->createFuncOutNets(call, inOutCntvs);
        
        SyList<SyStmt*>* stmts = this->replacePortsByNets(tf->getStmts());
        OutCntvInfoSet *outCntvInfoSet = this->expand(*stmts, out2Rep);

        this->mergeInOutCntvs(outCntvInfoSet, inOutCntvs.inputs);
        this->mergeInOutCntvs(outCntvInfoSet, inOutCntvs.outputs);
        this->mergeInOutCntvs(outCntvInfoSet, inOutCntvs.inouts);

            
        this->connectPortsWithArgs(call, inOutCntvs);

        for(SyNode<PortNetPair>* node = 
            inOutCntvs.outputs.head; node; node = node->next) {
            Out2Rep::const_iterator iter = out2Rep.find(node->val.second);
            if(iter != out2Rep.end()) {
                node->val.second = iter->second;
            }    

            //if(const SyCntv *net = stmt2Gates->getNet(node->val.second)) {
            //    node->val.second = const_cast<SyCntv*>(net);
            //}    

        }
                        
        if(!tf->isAuto()) {
            this->addInOutInfo(synth->getName(), tf->getName(), inOutCntvs);
        }    
        tfCallInfo = make_pair(new InOutCntvs(inOutCntvs), outCntvInfoSet);
        
    }

    OutCntvInfoSet*
    FuncTaskCall2Gates::expand(const SyList<SyStmt*>& stmts, Out2Rep out2Rep)
    {
        return 0;
        //return always2Gates->createGates(stmts, out2Rep);
        
    }



    InOutCntvs    
    FuncTaskCall2Gates::createNets(const SyList<SyScPort*>& ports) 
    {
        ports2Nets.clear();
        InOutCntvs ret;
        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next)
        {
            SyScNet *net = always2Gates->createNet();
            ports2Nets.insert(make_pair(node->val, net));
            if(node->val->getDir() == SyScPort::IN) {
                ret.inputs.push_back(make_pair((SyCntv*)0, net));
            }
            else if(node->val->getDir() == SyScPort::OUT){
                ret.outputs.push_back(make_pair((SyCntv*)0, net));
            }
            else {
                ret.inouts.push_back(make_pair((SyCntv*)0, net));
            }
        }
        return ret;
    }

    SyList<SyStmt*>*
    FuncTaskCall2Gates::replacePortsByNets(const SyList<SyStmt*>& stmts) const
    {
        SyList<SyStmt*>* retList = new SyList<SyStmt*>();
        for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next) {
            retList->push_back(this->replacePortsByNets(node->val));
        }
        return retList;
    }

    SyStmt*
    FuncTaskCall2Gates::replacePortsByNets(const SyStmt* stmt) const
    {
        SyStmt *retStmt = 0;
        if(const SyNonBlockAssign *nba = 
            dynamic_cast<const SyNonBlockAssign*>(stmt)) {
            retStmt = new SyNonBlockAssign(
                this->replacePortsByNets(nba->getLExpr()),
                this->replacePortsByNets(nba->getRExpr()));
        }
        else if(const SyBlockAssign *ba =
            dynamic_cast<const SyBlockAssign*>(stmt)) {
            retStmt = new SyBlockAssign(
                this->replacePortsByNets(ba->getLExpr()),
                this->replacePortsByNets(ba->getRExpr()));
        }
        else if(const SyIf *if_ = dynamic_cast<const SyIf*>(stmt))
        {
            SyIf *_if = new SyIf(this->replacePortsByNets(if_->getCond()));
            const SyList<SyStmt*>& thenPart = if_->getThenPart();

            for(SyNode<SyStmt*>* node = thenPart.head; node; node = node->next) {
                _if->addThenStmt(this->replacePortsByNets(node->val));
            }
            const SyList<SyStmt*>& elsePart = if_->getElsePart();
            for(SyNode<SyStmt*>* node = elsePart.head; node; node = node->next) {
                _if->addElseStmt(this->replacePortsByNets(node->val));
            }
            retStmt = _if;
        }
        else if(const SyAlways *always = dynamic_cast<const SyAlways*>(stmt))
        {
            SyAlways *a = new SyAlways();
            const vector<SyExpr*>& posExprs = always->getPosEdgeExprs();

            for(unsigned int i = 0; i < posExprs.size(); ++i) {
                a->addPosEdgeExpr(this->replacePortsByNets(posExprs[i]));
            }    
            const vector<SyExpr*>& negExprs = always->getNegEdgeExprs();
            for(unsigned int i = 0; i < negExprs.size(); ++i) {
                a->addNegEdgeExpr(this->replacePortsByNets(negExprs[i]));
            }    
            const vector<SyExpr*>& exprs = always->getExprs();
            for(unsigned int i = 0; i < exprs.size(); ++i) {
                a->addExpr(this->replacePortsByNets(exprs[i]));
            }    

            const SyList<SyStmt*>& stmts = always->getStmts();
            for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next) {
                a->addStmt(this->replacePortsByNets(node->val));
            }
            retStmt = a;
        }    
        else if(const SyCase *case_ = dynamic_cast<const SyCase*>(
            stmt))
        {
            SyCase *_case = new SyCase(this->replacePortsByNets(case_->getSelect()));

            const SyList<SyCaseItem*>& items = case_->getCaseItems();

            for(SyNode<SyCaseItem*>* item = items.head; item; item = item->next)
            {
                SyCaseItem *newItem = new SyCaseItem(
                    this->replacePortsByNets(item->val->getLabel()));
                
                const SyList<SyStmt*>& stmts = item->val->getStmts();
                for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next) {
                    newItem->addStmt(this->replacePortsByNets(node->val));
                }    
                _case->addItem(newItem);
            }
            retStmt = _case;
            
        }
        else if(const SyTaskCall *taskCall = dynamic_cast<const SyTaskCall*>(
            stmt))
        {
            SyTaskCall *newCall = new SyTaskCall(taskCall->getTask());
            const vector<SyExpr*>& args = taskCall->getArgs();
            for(unsigned int i = 0; i < args.size(); ++i) {
                newCall->addArg(this->replacePortsByNets(args[i]));
            }
            retStmt = newCall;
        }    

        assert(retStmt);
        return retStmt;
    }

    SyExpr*
    FuncTaskCall2Gates::replacePortsByNets(const SyExpr* expr) const
    {
        SyExpr *retExpr = 0;
        if(const SyScCntv *cntv = dynamic_cast<const SyScCntv*>(expr))
        {
            Ports2Nets::const_iterator iter = 
                ports2Nets.find(const_cast<SyScCntv*>(cntv));
            if(iter != ports2Nets.end()) {
                retExpr = iter->second;
            }
            else {
                retExpr = expr->copy();
            }
        }    
        
        else if(const SyVCntv *cntv = dynamic_cast<const SyVCntv*>(expr))
        {
            const SyList<SyScCntv*>& bits = cntv->getBits();
            SyConcat *conc = new SyConcat(cntv->isSigned());

            for(SyNode<SyScCntv*>* node = bits.head; node; node = node->next) {
                conc->addBit(this->replacePortsByNets(node->val));
            }
            retExpr = conc;
        }    

        else if(const SyPartSel *partSel = dynamic_cast<const SyPartSel*>(expr))
        {
            const vector<SyExpr*>& dim = partSel->getDim();
            vector<SyExpr*> newDim;

            for(unsigned int i = 0; i < dim.size(); ++i) {
                newDim.push_back(this->replacePortsByNets(dim[i]));
            }    

            retExpr = new SyPartSel(this->replacePortsByNets(partSel->getVar()),
                                    newDim,
                                    this->replacePortsByNets(partSel->getLExpr()),
                                    this->replacePortsByNets(partSel->getRExpr()));
                                    
                    
        }
        else if (const SyBitSel *bitSel = dynamic_cast<const SyBitSel*>(expr))
        {
            const vector<SyExpr*>& dim = bitSel->getDim();
            vector<SyExpr*> newDim;

            for(unsigned int i = 0; i < dim.size(); ++i) {
                newDim.push_back(this->replacePortsByNets(dim[i]));
            }    
            retExpr = new SyBitSel(this->replacePortsByNets(bitSel->getVar()),
                                    newDim);
        }
        else if(const SyConst *const_ = dynamic_cast<const SyConst*>(expr)){
            retExpr = const_->copy();
        }
        else if(const SyXOrZ *xOrZ = dynamic_cast<const SyXOrZ*>(expr)) {
            retExpr = xOrZ->copy();
        }    

        else if(const SyConcat *conc = 
            dynamic_cast<const SyConcat*>(expr)) 
        {
            const SyList<SyExpr*>& exprs = conc->getExprs();
            SyConcat *newConc = new SyConcat(conc->isSigned());
            for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next) {
                newConc->addBit(this->replacePortsByNets(node->val));
            }
            retExpr = newConc;
        }
        else if(const SyUnary *unary = dynamic_cast<const SyUnary*>(expr)) {
            retExpr = new SyUnary(this->replacePortsByNets(unary->getExpr()),
               unary->getOperator());
        }
        else if(const SyBinary *binary = dynamic_cast<const SyBinary*>(expr)) {
            retExpr = new SyBinary(this->replacePortsByNets(binary->getLExpr()),
                this->replacePortsByNets(binary->getRExpr()), binary->getOperator());
        }
        else if(const SyCondExpr *condExpr = 
            dynamic_cast<const SyCondExpr*>(expr)) {
            retExpr = new SyCondExpr(this->replacePortsByNets(condExpr->getCond()),
                this->replacePortsByNets(condExpr->getTrueExpr()),
                this->replacePortsByNets(condExpr->getFalseExpr()));
        }
        else if(const SyFuncCall *funcCall = 
            dynamic_cast<const SyFuncCall*>(expr))
        {
            //const SyTaskOrFuncCall *tfCall = funcCall->getFuncCall();
            SyTaskOrFuncCall *newTfCall = 
                new SyTaskOrFuncCall(funcCall->getFunc());
            const vector<SyExpr*>& args = funcCall->getArgs();
            for(unsigned int i = 0; i < args.size(); ++i) {
                newTfCall->addArg(this->replacePortsByNets(args[i]));
            }
            retExpr = new SyFuncCall(newTfCall);
        }

        assert(retExpr);
        return retExpr;
            
    }


    void
    FuncTaskCall2Gates::connectPortsWithArgs(const SyTaskOrFuncCall *call,
        InOutCntvs& inOutCntvs)
    {
        const SyTaskOrFunc *tf = call->getTaskOrFunc();
        const vector<SyExpr*>& args = call->getArgs();

        SyNode<PortNetPair>* in = inOutCntvs.inputs.head;
        SyNode<PortNetPair>* out = inOutCntvs.outputs.head;
        SyNode<PortNetPair>* inout = inOutCntvs.inouts.head;
        
        const SyList<SyScPort*>& ports = tf->getPorts();
        unsigned int i = 0;

        string last;
        for(SyNode<SyScPort*>* port = ports.head; port && (i < args.size());
            port = port->next)
        {
            if(port->val->isVecBit()) {
                if(!last.empty()) {
                    if(last == port->val->getIxName().getName()) {
                        continue;
                    }
                    else {
                        last.clear();
                    }
                }
            }
            else {
                last.clear();
            }    

            SyList<SyCntv*> tmpCntvs;
            if(port->val->isVecBit()) 
            {
                const SyVPort *vport = dynamic_cast<const SyVPort*>(
                    port->val->getVecCntv(always2Gates->getCurrRtlMod()->
                    getName()));
                assert(vport);    
                last = vport->getName();
                const SyConst *const_ = vport->size();
                int portSize = const_->toLong();
                delete const_;
                SyScPort::SyDirType dir = vport->getDir();
                for(int j = 0; j < portSize; ++j) 
                {
                    if(dir == SyScPort::IN)
                    {
                        tmpCntvs.push_back(in->val.second);
                        in = in->next;
                    }
                    else if(dir == SyScPort::INOUT){
                        tmpCntvs.push_back(inout->val.second);
                        inout = inout->next;
                    }
                    else if(dir == SyScPort::OUT){
                        tmpCntvs.push_back(out->val.second);
                        out = out->next;
                    }
                    else {
                        assert (0);
                    }
                }
                if(dir == SyScPort::IN) {
                    this->createBufs(tmpCntvs.head, args[i], true, false, 0);
                }
                else {
                    this->createBufs(tmpCntvs.head, args[i], false, true, &inOutCntvs);
                }    
            }    
            else
            {
                last.clear();
                SyCntv *cntv = 0;
                if((port->val->getDir() == SyScPort::IN) && in) 
                {
                    cntv = in->val.second;
                    in = in->next;
                }    
                else if((port->val->getDir() == SyScPort::INOUT) && inout) 
                {
                    cntv = inout->val.second;
                    inout = inout->next;
                }
                else if((port->val->getDir() == SyScPort::OUT) && out) 
                {
                    cntv = out->val.second;
                    out = out->next;
                }
                assert(cntv);
                tmpCntvs.push_back(cntv);
                if(port->val->getDir() == SyScPort::IN) {
                    this->createBufs(tmpCntvs.head, args[i], true, false, 0);
                }
                else {
                    this->createBufs(tmpCntvs.head, args[i], false, true, &inOutCntvs);
                }
            }    
            ++i;
        }    
    }    

    void
    FuncTaskCall2Gates::createBufs(    const SyNode<SyCntv*>* cntvNode, 
                                    const SyExpr *expr,
                                    bool firstOut,
                                    bool outPort,
                                    InOutCntvs* inOutCntvs)
    {
        SyNode<SyCntv*>* exprNode = 0;
        SyList<SyCntv*> tlist;
        auto_ptr<SyList<SyCntv*> > result;
        if(expr)
        {
            Stmt2Gates stmt2Gates(always2Gates->getCurrRtlMod(),
                always2Gates->getCurrSynthMod(), 
                always2Gates->getCurrScope());
            expr->accept(&stmt2Gates);
            //result = stmt2Gates.getOutCntvs();
            exprNode = result->head;
        }    
        else 
        {
            const SyConst *const_ = cntvNode->val->size();
            int size = const_->toLong();
            delete const_;
            for(int i = 0; i < size; ++i) {
                tlist.push_back(0);
            }
            exprNode = tlist.head;
        }    
                
        for(; exprNode && cntvNode; exprNode = exprNode->next, 
            cntvNode = cntvNode->next) 
        {
            if(outPort) {
                this->associateOut2Net(*inOutCntvs, 
                    firstOut ? cntvNode->val : exprNode->val,
                    firstOut ? exprNode->val : cntvNode->val);
            }
            else
            {
                static_cast<ConVisitorBase*>(always2Gates)->createBuf(
                    firstOut ? cntvNode->val : exprNode->val,
                    firstOut ? exprNode->val : cntvNode->val);
            }        
        }
    }    

    void
    FuncTaskCall2Gates::associateOut2Net(    InOutCntvs& inOutCntvs, 
                                            SyCntv *out,
                                            SyCntv *net) const
    {
        for(SyNode<PortNetPair>* node = inOutCntvs.outputs.head; node;
            node = node->next)
        {
            if(node->val.second->toString() == net->toString()) 
            {
                node->val.first = out;
                break;
            }
        }
    }    

    void
    FuncTaskCall2Gates::createFuncOutNets(const SyTaskOrFuncCall* call,
        InOutCntvs& inOutCntvs)
    {
        const SyTaskOrFunc *tf= call->getTaskOrFunc();
        if(!tf) {
            return;
        }    
        const SyFunc *func = dynamic_cast<const SyFunc*>(tf); 
        if(!func) {
            return;
        }    
        auto_ptr<SyList<SyCntv*> > result;
        Stmt2Gates stmt2Gates(always2Gates->getCurrRtlMod(),
            always2Gates->getCurrSynthMod(), 
            always2Gates->getCurrScope());
        SyCntv *ret = const_cast<SyCntv*>(func->getRetCntv());
        ret->accept(&stmt2Gates);

        //result = stmt2Gates.getOutCntvs();
        for(SyNode<SyCntv*>* node = result->head; node; node = node->next)
        {
            SyCntv *net = always2Gates->createNet();
            inOutCntvs.outputs.push_back(make_pair(node->val, net));
            ports2Nets.insert(make_pair(node->val, net));
        }
        
    }

    void 
    FuncTaskCall2Gates::mergeInOutCntvs(
        const OutCntvInfoSet* outCntvInfoSet, 
        SyList<PortNetPair>& pnlist)
    {
        for(SyNode<PortNetPair>* node = pnlist.head; node; 
            node = node->next)
        {
            OutCntvInfo info;
            info.out = node->val.second;
            OutCntvInfoSet::const_iterator iter = outCntvInfoSet->find(info);
            if(iter != outCntvInfoSet->end()) {
                node->val.second = iter->rep;
            }
        }
    }
/*
    SyList<SyStmt*>*
    FuncTaskCall2Gates::getMappedStmts(const SyTaskOrFuncCall* call, 
       Always2Gates *a2g) 
    {
        always2Gates = a2g;
        const SyTaskOrFunc *tf = call->getTaskOrFunc();

        InOutCntvs inOutCntvs = this->createNets(tf->getPorts());
        this->createFuncOutNets(call, inOutCntvs);
        
        SyList<SyStmt*>* stmts = this->replacePortsByNets(tf->getStmts());
        this->connectPortsWithArgs(call, inOutCntvs);
        return stmts;
    }
*/            
}
