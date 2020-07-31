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

#include "syconstprop.h"

#include "sycontainer.h"
#include "systmt.h"

#include "symsghandler.h"
#include "syconst.h"
#include "sycntvs.h"
#include "syexprmgr.h"

const int MAX_CALL_SIZE = 4096;
namespace Synth
{

    SyConstProp::SyConstProp() :
        module(0), currexpr(0), 
        currstmts(0),
        frameinfo(new SyFrameInfo(MAX_CALL_SIZE))
    {
    }
    SyConstProp::~SyConstProp()
    {
    }

    const string&
    SyConstProp::getCurrModName() const {
        return module->getName();
    }    


    void
    SyConstProp::popStackFrame()
    {
        if(frameinfo->empty()) {
            currstmts = 0;
        }
        else
        {
            frameinfo->pop();
            if(!frameinfo->empty()) {
                currstmts = frameinfo->getStatements();
            }
        }
    }

    void
    SyConstProp::pushStackFrame(bool reachable)
    {
        bool oldreachable = frameinfo->isReachable();
        frameinfo->createAndPushFrame(reachable && oldreachable);
        currstmts = frameinfo->getStatements();
    }

    bool
    SyConstProp::isReachable() const {
        return frameinfo->isReachable();
    }

    void
    SyConstProp::evaluateAndSetCurrExpr(const SyExpr *expr)
    {
        SyConst *const_ = isReachable() ? expr->evaluate() : 0;
        if(const_) {
            currexpr = const_;
        }
        else {
            currexpr = expr;
        }    
    }


    void
    SyConstProp::visit(const SyModule *mod)
    {
    }

    void
    SyConstProp::visit(const SyBModule *bmod)
    {
        const_cast<SyBModule*>(bmod)->resetCntvs();
        AcceptListOfObjs<SyModInst, SyConstProp>(this, bmod->getModInsts());
        AcceptListOfObjs<SyPrimInst, SyConstProp>(this, bmod->getPrimInsts());

        AcceptListOfObjs<SyTask, SyConstProp>(this, bmod->getTasks());
        AcceptListOfObjs<SyFunc, SyConstProp>(this, bmod->getFuncs());

        SyList<SyContAssign*>& cassigns = 
            const_cast<SyList<SyContAssign*>&>(bmod->getContAssigns());

        AcceptListOfStatements<SyContAssign>(cassigns);

        SyList<SyAlways*>& always = 
            const_cast<SyList<SyAlways*>&>(bmod->getAlwaysBlocks());
        AcceptListOfStatements<SyAlways>(always);
        const_cast<SyBModule*>(bmod)->resetCntvs();
    }

    void
    SyConstProp::visit(const SyScPort *port) {
        evaluateAndSetCurrExpr(port);
    }

    void
    SyConstProp::visit(const SyVPort *vport) {
        evaluateAndSetCurrExpr(vport);
    }

    void
    SyConstProp::visit(const SyScNet *net) {
        evaluateAndSetCurrExpr(net);
    }

    void
    SyConstProp::visit(const SyVNet *vnet) {
        evaluateAndSetCurrExpr(vnet);
    }

    void
    SyConstProp::visit(const SyScReg *reg) {
        evaluateAndSetCurrExpr(reg);
    }

    void
    SyConstProp::visit(const SyVReg *vreg) {
        evaluateAndSetCurrExpr(vreg);
    }
    void
    SyConstProp::visit(const SyMemory *mem) {
        currexpr = mem;
    }    

    void
    SyConstProp::visit(const SyVString *vstring) {
        currexpr = vstring;
    }

    void
    SyConstProp::visit(const SyXOrZ *xorz) {
        currexpr = xorz;
    }

    void
    SyConstProp::visit(const SyConst *const_) {
        if(isReachable()) {
            currexpr = const_;
        }
        else {
            currexpr = 0;
        }    
    }    

    void
    SyConstProp::visit(const SyCondExpr *condExpr)
    {
        const SyExpr *cond = condExpr->getCond();
        const SyExpr *texpr = condExpr->getTrueExpr();
        const SyExpr *fexpr = condExpr->getFalseExpr();
        cond->accept(this);

        if(const SyConst *const_ = dynamic_cast<const SyConst*>(currexpr))
        {
            if(const_->isone()) {
                texpr->accept(this);
            }
            else {
                fexpr->accept(this);
            }    
        }
        else
        {
            texpr->accept(this);
            const_cast<SyCondExpr*>(condExpr)->setTrueExpr(currexpr);
            fexpr->accept(this);
            const_cast<SyCondExpr*>(condExpr)->setFalseExpr(currexpr);
            currexpr = condExpr;
        }    
    }
    void
    SyConstProp::visit(const SyBinary *binary)
    {
        binary->getLExpr()->accept(this);
        if(currexpr) {
            const_cast<SyBinary*>(binary)->setLExpr(currexpr);
        }    
        binary->getRExpr()->accept(this);
        if(currexpr) {
            const_cast<SyBinary*>(binary)->setRExpr(currexpr);
        }    
        evaluateAndSetCurrExpr(binary);
    }

    void
    SyConstProp::visit(const SyUnary *unary)
    {
        unary->getExpr()->accept(this);
        if(currexpr) {
            const_cast<SyUnary*>(unary)->setExpr(currexpr);
        }    

        evaluateAndSetCurrExpr(unary);
    }

    void
    SyConstProp::visit(const SyBitSel *bitsel)
    {
        const vector<SyExpr*>& dim = bitsel->getDim();
        dim[dim.size()-1]->accept(this);

        const SyConst *sel = dynamic_cast<const SyConst*>(currexpr);
        if(!sel)
        {
            currexpr = bitsel;
            return;
        }

        long selbit = sel->toLong();
        const SyVCntv *vecvar = dynamic_cast<const SyVCntv*>(bitsel->getVar());
        const SyScCntv *svar = vecvar->getBit(selbit);

        if(!svar)
        {
            currexpr = bitsel;
            return;
        }
        svar->accept(this);
        const SyConst *const_ = dynamic_cast<const SyConst*>(currexpr);
        if(const_) {
            currexpr = const_;
        }
        else {
            currexpr = svar;
        }    
    }

    void
    SyConstProp::visit(const SyPartSel *partsel)
    {
        const SyConst *lconst = 0;
        const SyConst *rconst = 0;
        partsel->getLExpr()->accept(this);
        lconst = dynamic_cast<const SyConst*>(currexpr);
        partsel->getRExpr()->accept(this);
        rconst = dynamic_cast<const SyConst*>(currexpr);

        if(!lconst || !rconst){
            currexpr = partsel;
            return;
        }

        const long msb = lconst->toLong();
        const long lsb = rconst->toLong();

        SyConcat *concat = new SyConcat(false);
        const SyVCntv *var = dynamic_cast<const SyVCntv*>(
            partsel->getVar());

        for(int i = msb; (msb > lsb) ? i >= lsb : i <= lsb;
                (msb > lsb) ? --i : ++i)
        {
            const SyScCntv *cntv = var->getBit(i);
            cntv->accept(this);
            const SyConst *const_ = dynamic_cast<const SyConst*>(currexpr);
            if(!const_)
            {
                currexpr = partsel;
                delete concat;
                return;
            }
            concat->addBit(const_);
        }
        if(isReachable()) {
            currexpr = concat->evaluate();
        }
        else {
            currexpr = partsel;
        }

        delete concat;
    }

    void
    SyConstProp::visit(const SyConcat *conc)
    {
        string str;
        const SyList<SyExpr*>& exprs = conc->getExprs();

        for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next)
        {
            SyConst *const_ = node->val->evaluate();
            if(!const_)
            {
                currexpr = conc;
                return;
            }
            if(str.empty()){
                str = const_->getVal();
            }
            else {
                str += const_->getVal();
            }    
        }
        currexpr = new SyConst(conc->isSigned(), str);
    }


    void
    SyConstProp::visit(const SyAlways *always)
    {
        SyCheckAllBlocking checker;
        always->accept(&checker);

        if(!checker.allBlockAssign()) {
            return;
        }    
        SyList<SyStmt*>& stmts = 
            const_cast<SyList<SyStmt*>&>(always->getStmts());

        pushStackFrame(true);    
        AcceptListOfStatements<SyStmt>(stmts);
        copyToList<SyStmt>(stmts);
        popStackFrame();
    }

    

    void
    SyConstProp::visit(const SyIf *if_)
    {
        if_->getCond()->accept(this);
        const SyConst *cond = dynamic_cast<const SyConst*>(currexpr);
        if(cond && isReachable())
        {
            if(cond->iszero()) {
                AcceptListOfStatements<SyStmt>(if_->getElsePart());
            }
            else {
                AcceptListOfStatements<SyStmt>(if_->getThenPart());
            }
        }
        else 
        {
            SyList<SyStmt*>& thenpart = const_cast<SyList<SyStmt*>&>(if_->getThenPart());
            this->pushStackFrame(false);
            AcceptListOfStatements<SyStmt>(thenpart);
            copyToList<SyStmt>(thenpart);
            this->popStackFrame();

            SyList<SyStmt*>& elsepart = const_cast<SyList<SyStmt*>&>(if_->getElsePart());
            this->pushStackFrame(false);
            AcceptListOfStatements<SyStmt>(elsepart);
            copyToList<SyStmt>(elsepart);
            //elsepart = *currstmts;
            this->popStackFrame();
            currstmts->push_back(const_cast<SyIf*>(if_));
        }

    }


    void
    SyConstProp::visit(const SyCase *case_)
    {
        SyList<SyCaseItem*>& items = 
            const_cast<SyList<SyCaseItem*>&>(case_->getCaseItems());
        SyNode<SyCaseItem*>* item = 0;

        const SyExpr *select = case_->getSelect();
        select->accept(this);
        const SyConst *const_ = dynamic_cast<const SyConst*>(currexpr);
        if(const_ && isReachable())
        {
            long sellabel = const_->toLong();
            long label = LONG_MAX;

            for(item = items.head; item; item = item->next)
            {
                item->val->getLabel()->accept(this);
                const SyConst *const_ = dynamic_cast<const SyConst*>(currexpr);
                if(!const_)
                {
                    currstmts->push_back(const_cast<SyCase*>(case_));
                    return;
                }
                label = const_->toLong();
                if(label == sellabel) {
                    break;
                }
            }
            if(!item)
            {
                currstmts->push_back(const_cast<SyCase*>(case_));
                return;
            }    
                
            SyList<SyStmt*>& stmts = 
                const_cast<SyList<SyStmt*>&>(item->val->getStmts());
            AcceptListOfStatements<SyStmt>(stmts);    
        }
        else
        {
            for(item = items.head; item; item = item->next)
            {
                pushStackFrame(false);
                SyList<SyStmt*>& stmts = 
                    const_cast<SyList<SyStmt*>&>(item->val->getStmts());
                AcceptListOfStatements<SyStmt>(stmts);    
                //stmts = *currstmts;
                copyToList<SyStmt>(stmts);
                popStackFrame();
            }    
            currstmts->push_back(const_cast<SyCase*>(case_));
        }

    }
    void
    SyConstProp::visit(const SyContAssign *cassign) 
    {
        this->visitAssign(cassign);;
    }

    void
    SyConstProp::visit(const SyBlockAssign *bassign)
    {
        this->visitAssign(bassign);;
        currstmts->push_back(const_cast<SyBlockAssign*>(bassign));
    }
    
    void
    SyConstProp::visit(const SyNonBlockAssign *nbassign)
    {
        this->visitAssign(nbassign);;
        currstmts->push_back(const_cast<SyNonBlockAssign*>(nbassign));
    }
    

    void
    SyConstProp::visitAssign(const SyAssign *assign)
    {
        SyExpr *rexpr = assign->getRExpr()->copy();
        SyExpr *lexpr = const_cast<SyExpr*>(assign->getLExpr());
        //assign->getRExpr()->accept(this);
        rexpr->accept(this);
        const SyConst *const_ = dynamic_cast<const SyConst*>(currexpr);
        if(!const_) 
        {
            //reached a non-const, don't forget to reset the value
            lexpr->removeVal();
            return;
        }
        //delete rexpr;
        SyExprMgr::instance()->processAssign(const_cast<SyAssign*>(assign));
        delete const_;
        assign->getRExpr()->accept(this);
        const_ = dynamic_cast<const SyConst*>(currexpr);
        if(!const_) 
        {
            lexpr->removeVal();
            return;
        }    
        //assert(const_);
        lexpr->addVal(const_);
        const_cast<SyConst*>(const_)->setSign(assign->getRExpr()->isSigned());

        const_cast<SyAssign*>(assign)->setRExpr(const_);
    }

    void
    SyConstProp::visit(const SyTask *task) 
    {
        this->visitTf(task);
    }

    void
    SyConstProp::visit(const SyFunc *func) 
    {
        this->visitTf(func);
    }    
    

    void
    SyConstProp::visitTf(const SyTaskOrFunc *taskOrFunc)
    {
        pushStackFrame(true);
        SyList<SyStmt*>& oldStmts = 
            const_cast<SyList<SyStmt*>&>(taskOrFunc->getStmts());

        AcceptListOfStatements<SyStmt>(oldStmts);
        copyToList<SyStmt>(oldStmts);
        popStackFrame();
    }

    void
    SyConstProp::visit(const SyTaskCall *tcall)
    {
        this->visitTfCall(tcall);
        currstmts->push_back(const_cast<SyTaskCall*>(tcall));
    }

    void
    SyConstProp::visit(const SyFuncCall *fcall)
    {
        this->visitTfCall(*fcall);
        SyConst *ret = isReachable() ? 
            fcall->getFunc()->getRetCntv()->evaluate() : 0;
        if(ret) {
            currexpr = ret;
        }
        else {
            currexpr = fcall;
        }    
    }

    void
    SyConstProp::visitTfCall(const SyTaskOrFuncCall *tfcall)
    {

        SyTaskOrFunc *tf = const_cast<SyTaskOrFunc*>(tfcall->getTaskOrFunc());
        //frameinfo->createAndPushFrame(tfcall);

        const vector<SyExpr*>& args = tfcall->getArgs();

        SyList<SyCntv*>* ports = tf->getMixPorts();

        SyNode<SyCntv*>* p = ports->head;
        bool reachable = true;

        for(int i = 0; p && (i < args.size()); p = p->next)
        {
            SyVPort *vport = dynamic_cast<SyVPort*>(p->val);
            SyScPort *port = dynamic_cast<SyScPort*>(p->val);

            SyScPort::SyDirType dir = vport ? vport->getDir() : port->getDir();
            if(dir == SyScPort::OUT){
                continue;
            }
            if(vport) {
                vport->addVal(args[i++]);
            }
            else {
                port->addVal(args[i++]);
            }    
        }
        pushStackFrame(true); //Check
        tf->accept(this);
        popStackFrame();
    }
    
    template< typename T > void
    SyConstProp::AcceptListOfStatements(const SyList<T*>& oldstmts)
    {
        if(oldstmts.empty()) {
            return;
        }
        for(SyNode<T*>* s = oldstmts.head; s; s = s->next) {
            s->val->accept(this);
        }
    }

    template< typename T > void
    SyConstProp::copyToList(SyList<T*>& tolist) const 
    {
        if(!currstmts) {
            return;
        }
        tolist.clear();

        for(SyNode<SyStmt*>* s = currstmts->head; s; s = s->next) {
            tolist.push_back(dynamic_cast<T*>(s->val));
        }    
    }
    

    /////////////////////////////////////////////////////////////////
    //    Class: SyFrameInfo
    /////////////////////////////////////////////////////////////////

    SyFrameInfo::SyFrameInfo(int size) : maxsize(size) {
    }

    SyFrameInfo::~SyFrameInfo() {
        while(!actstack.empty()) 
        {
            SyFrameData data = actstack.top();
            actstack.pop();
            delete data.second;
        }
    }

    SyFrameInfo::SyFrameData
    SyFrameInfo::getFrameData() {
        return actstack.top();
    }

    bool    
    SyFrameInfo::isReachable() const{
        if(empty()) {
            return true;
        }    
        return actstack.top().first;
    }

    SyList<SyStmt*>*
    SyFrameInfo::getStatements() {
        return actstack.top().second;
    }

    void
    SyFrameInfo::overflow() const
    {
        if(actstack.size() >= maxsize)
        {
            SyMsgHandler::instance()->print(501, maxsize);
            exit(1);
        }    
    }

    void
    SyFrameInfo::push(const SyFrameData& data)
    {
        overflow();
        actstack.push(data);
    }

    void
    SyFrameInfo::push(SyStmt *stmt) {
        actstack.top().second->push_back(stmt);
    }    

    void
    SyFrameInfo::pop() 
    {
        if(actstack.empty()){
            //No function/task calls, never required to push 
            //in any activation stack
            return;
        }    
        SyFrameData data = actstack.top();
        delete data.second;
        actstack.pop();
    }    

    void
    SyFrameInfo::createAndPushFrame(bool reachable)
    {
        SyList<SyStmt*>* stmts = new SyList<SyStmt*>();
        actstack.push(std::make_pair(reachable, stmts));
    }

    bool
    SyFrameInfo::empty() const {
        return actstack.empty();
    }    

    //--------------------------------------------------------------------

    SyCheckAllBlocking::SyCheckAllBlocking() : allblocking(true) {
    }

    SyCheckAllBlocking::~SyCheckAllBlocking() {
    }

    bool
    SyCheckAllBlocking::allBlockAssign() const {
        return allblocking;
    }    

    void
    SyCheckAllBlocking::visit(const SyNonBlockAssign *nbassign) {
        allblocking = false;
    }    

    void
    SyCheckAllBlocking::visit(const SyIf *if_) 
    {
        if(!allblocking) {
            return;
        }

        AcceptListOfStatements(if_->getThenPart());
        AcceptListOfStatements(if_->getElsePart());
    }

    void
    SyCheckAllBlocking::visit(const SyAlways *always)
    {
        if(!allblocking) {
            return;
        }

        AcceptListOfStatements(always->getStmts());
    }

    void
    SyCheckAllBlocking::visit(const SyCaseItem *item)
    {
        if(!allblocking) {
            return;
        }

        AcceptListOfStatements(item->getStmts());
    }    

    void
    SyCheckAllBlocking::visit(const SyCase *case_)
    {
        if(!allblocking) {
            return;
        }

        const SyList<SyCaseItem*>& items = case_->getCaseItems();

        for(SyNode<SyCaseItem*>* node = items.head; node; node = node->next)
        {
            node->val->accept(this);
            if(!allblocking) {
                return;
            }
        }    
    }

    void
    SyCheckAllBlocking::visitTf(const SyTaskOrFunc *tf)
    {
        if(!allblocking) {
            return;
        }
        AcceptListOfStatements(tf->getStmts());
    }


    void
    SyCheckAllBlocking::visit(const SyTask *task) {
        this->visitTf(task);
    }

    void
    SyCheckAllBlocking::visit(const SyFunc *func) {
        this->visitTf(func);
    }

    void
    SyCheckAllBlocking::visitTfCall(const SyTaskOrFuncCall *tfcall) {
        tfcall->getTaskOrFunc()->accept(this);
    }    

    void
    SyCheckAllBlocking::visit(const SyTaskCall *tcall) {
        visitTfCall(tcall);
    }

    void
    SyCheckAllBlocking::visit(const SyFuncCall *fcall)
    {
        const SyTaskOrFuncCall * tfcall = *fcall;
        visitTfCall(tfcall);
    }

    const string&
    SyCheckAllBlocking::getCurrModName() const {
        return modname;
    }    

    void
    SyCheckAllBlocking::AcceptListOfStatements(const SyList<SyStmt*>& stmts)
    {
        for(SyNode<SyStmt*>* s = stmts.head; s; s = s->next) {
            s->val->accept(this);
        }    
    }
}
