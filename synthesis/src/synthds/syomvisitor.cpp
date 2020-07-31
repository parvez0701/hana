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
#include <list>

#include "syomvisitor.h"

#include "sycontainer.h"
#include "sycntvs.h"
#include "syexpr.h"
#include "syexprmgr.h"
#include "systmt.h"
#include "syconst.h"
#include "symsghandler.h"
#include "syroot.h"
#include "syprocmgr.h"

#include "namemgr.h"

using std::map;
using std::string;
using std::vector;
using std::swap;
using std::pair;
using std::list;


namespace Synth
{
    class SyTfCallCopier
    {
        public:
            SyTfCallCopier(SyFuncCall*, SyBModule*);
            SyTfCallCopier(SyTaskCall*, SyBModule*);

            void operator()();


        private:
            SyTfCallCopier(const SyTfCallCopier&);
            SyTfCallCopier& operator=(const SyTfCallCopier&);

            void copyFuncCall();
            void copyTaskCall();

            SyFunc *copyFunc();
            SyTask *copyTask();
            void copyData(const SyTaskOrFunc*, SyTaskOrFunc*);
            SyFuncCall *fcall;
            SyTaskCall *tcall;
            SyBModule  *mod;
    };

    SyTfCallCopier::SyTfCallCopier(SyFuncCall *f, SyBModule *m) :
        fcall(f),
        tcall(0),
        mod(m){
    }    

    SyTfCallCopier::SyTfCallCopier(SyTaskCall *t, SyBModule *m) :
        fcall(0),
        tcall(t),
        mod(m){
    }

    void
    SyTfCallCopier::operator()()
    {
        assert(tcall || fcall);
        if(tcall) {
            copyTaskCall();
        }
        else {
            copyFuncCall();
        }    
    }

    void    
    SyTfCallCopier::copyTaskCall()
    {
        const SyTask *stask = this->copyTask();
        tcall->setTaskOrFunc(stask);
        vector<SyExpr*>& args = const_cast<vector<SyExpr*>&>(tcall->getArgs());
        for(int i = 0; i < args.size(); ++i) {
            args[i] = args[i] ? args[i]->copy() : args[i]; 
        }    
    }

    SyTask*
    SyTfCallCopier::copyTask()
    {
        const SyTask *stask = tcall->getTask();
        assert(stask);        
        string name = NameMgr<SyBModule>::instance()->createName(mod, tcall->getName());
        SyTask *newtask = new SyTask(name, stask->isAuto());
        const_cast<SyBModule*>(mod)->addTask(newtask);
        const_cast<SyBModule*>(mod)->addObject(newtask->getName(), newtask);

        this->copyData(stask, newtask);
        return newtask;
    }


    void    
    SyTfCallCopier::copyFuncCall()
    {
        SyFunc * sfunc = copyFunc();
        fcall->setFunc(sfunc);

        vector<SyExpr*>& args = const_cast<vector<SyExpr*>&>(fcall->getArgs());
        for(int i = 0; i < args.size(); ++i) {
            args[i] = args[i] ? args[i]->copy() : args[i]; 
        }    
    }

    void
    SyTfCallCopier::copyData(const SyTaskOrFunc *from, SyTaskOrFunc *to)
    {
        SyList<SyCntv*>* ports = from->getMixPorts();

        for(SyNode<SyCntv*>* port = ports->head; port; port = port->next)
        {
            if(SyVPort *vport = dynamic_cast<SyVPort*>(port->val))
            {
                SyVPort *newport = new SyVPort(vport->getName(), vport->getMsb(),
                    vport->getLsb(), vport->isSigned(), vport->getDir(),
                    vport->getNetType());
                to->addObject(newport->getName(), newport);    

                const SyList<SyScCntv*>& bits = newport->getBits();
                for(SyNode<SyScCntv*>* bit = bits.head; bit; bit = bit->next)
                {
                    to->addPort(static_cast<SyScPort*>(bit->val));
                }
            }
            else if(SyScPort *scport = dynamic_cast<SyScPort*>(port->val))
            {
                SyScPort *newport = new SyScPort(scport->getIxName(), scport->isSigned(),
                    scport->getDir(), scport->getNetType());
                to->addPort(newport);
            }    
        }
        delete ports;

        const SyList<SyStmt*>& stmts = from->getStmts();
        for(SyNode<SyStmt*>* stmt = stmts.head; stmt; stmt = stmt->next)
        {
            SyStmt *newstmt = stmt->val->copy();
            const SyList<SyScPort*>& ports = to->getPorts();

            for(SyNode<SyScPort*>* p = ports.head; p; p = p->next) {
                newstmt->repSubExpr(p->val->toString(), p->val);
            }
            to->addStmt(newstmt);
        }
    }

    SyFunc*
    SyTfCallCopier::copyFunc()
    {
        const SyFunc *sfunc = fcall->getFunc(); 
        assert(sfunc);        
        const SyCntv *retcntv = 0;

        string name = NameMgr<SyBModule>::instance()->createName(mod, fcall->getName());
        SyFunc *newfunc = new SyFunc(name, retcntv, sfunc->isSigned(), sfunc->isAuto());
        const_cast<SyBModule*>(mod)->addFunc(newfunc);
        const_cast<SyBModule*>(mod)->addObject(newfunc->getName(), newfunc);
        this->copyData(sfunc, newfunc);
        const SyCntv *oldretcntv = sfunc->getRetCntv();
        
        const SyList<SyStmt*>& stmts = newfunc->getStmts();
        string oldfuncname = sfunc->getName();
        if(const SyVReg *vreg = dynamic_cast<const SyVReg*>(oldretcntv)) 
        {
            retcntv = new SyVReg(newfunc->getName(),
                vreg->getMsb(), vreg->getLsb(), vreg->isSigned());
            string oldcntvname = vreg->getName();    
                
            const SyList<SyScCntv*>& bits = static_cast<const SyVReg*>(retcntv)->getBits();
            for(SyNode<SyScCntv*>* bit = bits.head; bit; bit = bit->next)
            {

                for(SyNode<SyStmt*>* s = stmts.head; s; s = s->next)
                {
                    s->val->repSubExpr(SyIxName(oldcntvname, 
                    bit->val->getIxName().getIndex()).toString(), bit->val);
                }    
            }
        }    
        else if(const SyScReg *screg = dynamic_cast<const SyScReg*>(oldretcntv)) 
        {
            string oldcntvname = screg->getIxName().getName();    
            retcntv = new SyScReg(newfunc->getName(), screg->isSigned());
            for(SyNode<SyStmt*>* s = stmts.head; s; s = s->next) {
                s->val->repSubExpr(oldcntvname, retcntv);
            }    
        }
        newfunc->setRetCntv(retcntv);

        return newfunc;
    }




//////////////////////////////////////////////////////////////////////////
    template<class T> void
    AcceptListOfObjects(SyOMVisitor *v, const SyList<T*>& tlist) {
        for(SyNode<T*>* node = tlist.head; node; node = node->next) {
            node->val->accept(v);
        }    
    }    
    SyOMVisitor::SyOMVisitor() : context(UNSET), currExpr(0), 
        currMod(0), status(true)
    {
    }

    SyOMVisitor::SyOMVisitor(SynthContext ctx) : context(ctx), currExpr(0),
        currMod(0), status(true)
    {
    }

    SyOMVisitor::~SyOMVisitor()
    {
    }

    void
    SyOMVisitor::setContext(SynthContext ctx) {
        context |= ctx;
    }

    //void
    //SyOMVisitor::resetContext(SynthContext ctx) {
    //    unsigned int tmp = 0xffff;
    //    tmp ^= ctx;
    //    context &= tmp;
    //}    

    bool
    SyOMVisitor::isContext(unsigned int ctx) const {
        return context & ctx;
    }

    void
    SyOMVisitor::visit()
    {
        const map<string, SyModule*, cmp_str> & synthMods =
        SyRoot::instance()->getSynthMods();
        map<string, SyModule*, cmp_str>::const_iterator beginIter =
        synthMods.begin();
        map<string, SyModule*, cmp_str>::const_iterator endIter =
        synthMods.end();

        list<SyModule*> rmmods;

        for(; beginIter != endIter; ++beginIter) {
            status = true;
            currMod = beginIter->second; 
            beginIter->second->accept(this);
            if(!status){
                rmmods.push_back(currMod);
                //SyRoot::instance()->removeSynthMod(currMod);
            }    
        }    
        for(list<SyModule*>::const_iterator iter = rmmods.begin();
            iter != rmmods.end(); ++iter) {
            SyRoot::instance()->removeSynthMod(*iter);
            SyRoot::instance()->addDontTouchModule((*iter)->getName());
        }        


    }

    void
        SyOMVisitor::visit(const SyModule* mod)
        {
            AcceptListOfObjects<SyScNet>(this, mod->getNets());
            AcceptListOfObjects<SyModInst>(this, mod->getModInsts());
            AcceptListOfObjects<SyPrimInst>(this, mod->getPrimInsts());
        }
    

    void
        SyOMVisitor::visit(const SyBModule* mod)
        {
            AcceptListOfObjects<SyScNet>(this, mod->getNets());
            AcceptListOfObjects<SyModInst>(this, mod->getModInsts());
            AcceptListOfObjects<SyPrimInst>(this, mod->getPrimInsts());
            AcceptListOfObjects<SyContAssign>(this, mod->getContAssigns());
            AcceptListOfObjects<SyAlways>(this, mod->getAlwaysBlocks());
            AcceptListOfObjects<SyTask>(this, mod->getTasks());
            AcceptListOfObjects<SyFunc>(this, mod->getFuncs());
        }
        
    void
        SyOMVisitor::visit(const SyScPort *port)
        {
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                currExpr = const_cast<SyScPort*>(port);
            }    
        }
    void
        SyOMVisitor::visit(const SyVPort *port)
        {
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                currExpr = const_cast<SyVPort*>(port);
            }    
        }

    void
        SyOMVisitor::visit(const SyScNet *net)
        {
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                currExpr = const_cast<SyScNet*>(net);
            }    
        }

    void
        SyOMVisitor::visit(const SyVNet *net)
        {
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                currExpr = const_cast<SyVNet*>(net);
            }    
        }

    void
        SyOMVisitor::visit(const SyScReg *reg)
        {
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                currExpr = const_cast<SyScReg*>(reg);
            }    
        }

    void
        SyOMVisitor::visit(const SyVReg *reg)
        {
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                currExpr = const_cast<SyVReg*>(reg);
            }    
        }

    void
        SyOMVisitor::visit(const SyMemory *mem)
        {
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                currExpr = const_cast<SyMemory*>(mem);
            }    
        }

    void
        SyOMVisitor::visit(const SyXOrZ *xorz) {
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                currExpr = const_cast<SyXOrZ*>(xorz);
            }    
        }    
    void
        SyOMVisitor::visit(const SyVString *str) {
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                currExpr = const_cast<SyVString*>(str);
            }    
        }    

    void
        SyOMVisitor::visit(const SyModInst *minst)
        {
            AcceptListOfObjects<SyScPin>(this, minst->getPins());
        }

    void
        SyOMVisitor::visit(const SyPrimInst *pinst)
        {
            AcceptListOfObjects<SyScPin>(this, pinst->getPins());

        }

    void
        SyOMVisitor::visit(const SyScPin *pin)
        {
            const SyExpr *actual = pin->getActual();
            if(actual) {
                actual->accept(this);
            }    
            else {
                currExpr = 0;
            }    
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                const_cast<SyScPin*>(pin)->setActual(currExpr);
            }    
        }    

    void
        SyOMVisitor::visit(const SyVPin *pin)
        {
            const SyExpr *actual = pin->getActual();
            if(actual) {
                actual->accept(this);
            }    
            //if(isContext(BEHAV_OPT)) {
            //    pin->setActual(currExpr);
            //}    
        }    
    void
        SyOMVisitor::visit(const SyContAssign *assign)
        {
            assign->rhs->accept(this);
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                const_cast<SyContAssign*>(assign)->rhs = currExpr;
            }    
            assign->lhs->accept(this);
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                const_cast<SyContAssign*>(assign)->lhs = currExpr;
            }    
        }

    void
        SyOMVisitor::visit(const SyNonBlockAssign *nbassign)
        {
            nbassign->rhs->accept(this);
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                const_cast<SyNonBlockAssign*>(nbassign)->rhs = currExpr;
            }    
            nbassign->lhs->accept(this);

            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                const_cast<SyNonBlockAssign*>(nbassign)->lhs = currExpr;
            }    
        }


    void
        SyOMVisitor::visit(const SyBlockAssign *bassign)
        {
            bassign->rhs->accept(this);
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                const_cast<SyBlockAssign*>(bassign)->rhs = currExpr;
            }    
            bassign->lhs->accept(this);
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                const_cast<SyBlockAssign*>(bassign)->lhs = currExpr;
            }    
        }

    void
        SyOMVisitor::visit(const SyIf* if_)
        {
            if_->cond->accept(this);
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                const_cast<SyIf*>(if_)->cond = currExpr;
            }    
            AcceptListOfObjects<SyStmt>(this, if_->getThenPart());
            AcceptListOfObjects<SyStmt>(this, if_->getElsePart());
        }


    void
        SyOMVisitor::visit(const SyAlways *always)
        {
            vector<SyExpr*>& exprs = const_cast<vector<SyExpr*>&>(
            always->getExprs());
            vector<SyExpr*>& posExprs = const_cast<vector<SyExpr*>&>(
            always->getPosEdgeExprs());
            vector<SyExpr*>& negExprs = const_cast<vector<SyExpr*>&>(
            always->getNegEdgeExprs());

            for(unsigned int i = 0; i < exprs.size(); ++i) 
            {
                exprs[i]->accept(this);
                if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                    exprs[i] = currExpr;
                }    
            }    
            for(unsigned int i = 0; i < posExprs.size(); ++i) 
            {
                posExprs[i]->accept(this);
                if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                    posExprs[i] = currExpr;
                }    
            }    
            for(unsigned int i = 0; i < negExprs.size(); ++i) 
            {
                negExprs[i]->accept(this);
                if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                    negExprs[i] = currExpr;
                }    
            }    

            AcceptListOfObjects<SyStmt>(this, always->getStmts());
            if(isContext(TF_COPY | CONST_COMPACT)) {
            }
            else if(status){
                if(!SyProcMgr::instance()->check(always, currMod)) {
                    status = false;
                }    
            }    

        }

    void
        SyOMVisitor::visit(const SyEventCtrl *eventCtrl)
        {
        }

    void
        SyOMVisitor::visit(const SyCase *case_)
        {
            case_->select->accept(this); 
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                const_cast<SyCase*>(case_)->select = currExpr;
            }    
            const SyList<SyCaseItem*>& caseItems = case_->getCaseItems();

            for(SyNode<SyCaseItem*>* node = caseItems.head; node; node = node->next)
            {
                node->val->label->accept(this);
                if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                    node->val->label = currExpr;
                }    
                AcceptListOfObjects<SyStmt>(this, node->val->getStmts());
            }    

        }

    void
        SyOMVisitor::visit(const SyTask *task)
        {
            AcceptListOfObjects<SyStmt>(this, task->getStmts());
        }

    void
        SyOMVisitor::visit(const SyFunc *func)
        {
            AcceptListOfObjects<SyStmt>(this, func->getStmts());
        }

    void
        SyOMVisitor::visit(const SyTaskCall *tcall)
        {
            const SyTask *task = dynamic_cast<const SyTask*>(
                currMod->findObject(tcall->getName()));
            if(!task)
            {
                SyMsgHandler::instance()->print(43, tcall->getName().c_str(), currMod->getName().c_str());
                status = false;
                return;
            }    
            //assert(task);
            const_cast<SyTaskCall*>(tcall)->setTask(task);
            vector<SyExpr*>& args = 
            const_cast<vector<SyExpr*>&>(tcall->getArgs());
            for(unsigned int i = 0; i < args.size(); ++i) 
            {
                args[i]->accept(this);
                if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                    args[i] = currExpr;
                }    
            }

            if(isContext(TF_COPY)) {
                SyTfCallCopier(const_cast<SyTaskCall*>(tcall), static_cast<SyBModule*>(currMod))();
            }
            this->checkTfCall(tcall);
            
        }

    void
        SyOMVisitor::visit(const SyBlockCompAssign* bcassign)
        {
        }

    void
        SyOMVisitor::visit(const SyNonBlockCompAssign* nbcassign)
        {
        }
    void
        SyOMVisitor::visit(const SyConst *const_)
        {
            if(isContext(BEHAV_OPT | CONST_COMPACT)) {
                currExpr = const_->copy();
            }    
        }
    void
    SyOMVisitor::visit(const SyBinary *binary)
    {
        SyExpr::SyOp op = binary->getOperator();
        if(!isContext(TF_COPY))
        {
            if(op == SyExpr::CASEEQ || op == CASEENEQ) 
            {
                SyMsgHandler::instance()->print(37, currMod->getName().c_str(),
                "case equality operator" );
                status = false;
            }

            this->checkPowerOperator(binary);
        }    

        if(isContext(BEHAV_OPT | CONST_COMPACT) ) 
        {
            binary->lexpr->accept(this);
            const_cast<SyBinary*>(binary)->lexpr = currExpr;
            binary->rexpr->accept(this);
            const_cast<SyBinary*>(binary)->rexpr = currExpr;
            currExpr = const_cast<SyBinary*>(binary);
            if(op == SyExpr::POW) {
                this->convertPowerOperator(const_cast<SyBinary*>(binary));
            }    
        }    

        checkAndSetConstExpr(binary);
    }

    void
    SyOMVisitor::visit(const SyUnary *unary)
    {
        if(isContext(BEHAV_OPT | CONST_COMPACT))
        {
            unary->expr->accept(this);
            const_cast<SyUnary*>(unary)->expr = currExpr;
            currExpr = const_cast<SyUnary*>(unary);
        }    
        checkAndSetConstExpr(unary);
    }    
    void
    SyOMVisitor::visit(const SyCondExpr *condExpr)
    {
        if(isContext(BEHAV_OPT | CONST_COMPACT))
        {
            condExpr->cond->accept(this);
            const_cast<SyCondExpr*>(condExpr)->cond = currExpr;

            condExpr->texpr->accept(this);
            const_cast<SyCondExpr*>(condExpr)->texpr = currExpr;

            condExpr->fexpr->accept(this);
            const_cast<SyCondExpr*>(condExpr)->fexpr = currExpr;

            currExpr = const_cast<SyCondExpr*>(condExpr);
        }
        checkAndSetConstExpr(condExpr);
    }    

    void
    SyOMVisitor::visit(const SyConcat *conc)
    {
        if(isContext(BEHAV_OPT | CONST_COMPACT))
        {
            SyList<SyExpr*> bits;
            for(SyNode<SyExpr*>* node = conc->exprs.head; node; node = node->next)
            {
                if(node->val) 
                {
                    //removing NULLs  ?
                    node->val->accept(this);
                    bits.push_back(currExpr);
                }    
            }
            const_cast<SyConcat*>(conc)->exprs.clear();
            const_cast<SyConcat*>(conc)->exprs = bits;
            currExpr = const_cast<SyConcat*>(conc);
        }    

        checkAndSetConstExpr(conc);
                
    }
    void
    SyOMVisitor::visit(const SyFuncCall *funcCall)
    {
        if(isContext(TF_COPY))
        {
            SyTfCallCopier(const_cast<SyFuncCall*>(funcCall), static_cast<SyBModule*>(currMod))();
        }
        else
        {
            const SyFunc *func = dynamic_cast<const SyFunc*>
            (currMod->findObject(funcCall->getName()));
            assert(func);
            const_cast<SyFuncCall*>(funcCall)->setFunc(func);
            if(isContext(BEHAV_OPT | CONST_COMPACT))
            {
                const SyTaskOrFuncCall *tfCall = *funcCall;

                for(unsigned int i = 0; i < tfCall->args.size(); ++i) 
                {
                    tfCall->args[i]->accept(this);
                    const_cast<SyTaskOrFuncCall*>(tfCall)->args[i] = currExpr;
                }
                currExpr = const_cast<SyFuncCall*>(funcCall);
            }
            if(isContext(BEHAV_OPT)) {
                this->checkTfCall(*funcCall);
            }    
        }
    }    
    void
    SyOMVisitor::visit(const SyBitSel *bitSel)
    {
        if(isContext(BEHAV_OPT | CONST_COMPACT))
        {
            for(unsigned int i = 0; i < bitSel->dim.size(); ++i)
            {
                bitSel->dim[i]->accept(this);
                const_cast<SyBitSel*>(bitSel)->dim[i] = currExpr;
            }
            //Since multi-D array is not supported, we are able to do this
            const SyExpr *select = bitSel->getFirstDim();
            currExpr = 0;
            if(const SyConst *const_ = dynamic_cast<const SyConst*>(select))
            {
                int bit = const_->toLong();
                const SyVCntv *var = dynamic_cast<const SyVCntv*>(bitSel->getVar());
                assert(var);
                int msb = var->getMsb();
                int lsb = var->getLsb();
                if (msb < lsb) {
                    swap(msb, lsb);
                }    
                if(bit < lsb || bit > msb) 
                {
                    SyMsgHandler::instance()->print(22, 
                        currMod->getName().c_str(), bitSel->toString().c_str());
                    status = false;    
                    //SyRoot::instance()->removeSynthMod(currMod);    
                }
                else {
                    currExpr = const_cast<SyScCntv*>(var->getBit(bit));
                }    
            }
            if(!currExpr) {
                currExpr = const_cast<SyBitSel*>(bitSel);
            }    
        }    
        checkAndSetConstExpr(bitSel);
    }
    
    void
    SyOMVisitor::visit(const SyPartSel *partSel)
    {
        if(isContext(BEHAV_OPT | CONST_COMPACT))
        {
            for(unsigned int i = 0; i < partSel->dim.size(); ++i)
            {
                partSel->dim[i]->accept(this);
                const_cast<SyPartSel*>(partSel)->dim[i] = currExpr;
            }
            partSel->lexpr->accept(this);
            const_cast<SyPartSel*>(partSel)->lexpr = currExpr;
            partSel->rexpr->accept(this);
            const_cast<SyPartSel*>(partSel)->rexpr = currExpr;
            SyConcat *conc = partSel->getConcat();
            //let us get rid of part select
            if(conc) {
                currExpr = conc;
            }
            else {
                currExpr = const_cast<SyPartSel*>(partSel);
            }    
        }
        checkAndSetConstExpr(partSel);
    }    

    void
    SyOMVisitor::checkTfCall(const SyTaskOrFuncCall *tfcall) 
    {
        if(isContext(TF_COPY)) {
            return;
        }    
        vector<SyExpr*>& args = const_cast<vector<SyExpr*>&>(tfcall->getArgs());
        const SyTaskOrFunc *tf = tfcall->getTaskOrFunc();

        SyList<SyScCntv*> formals;
        const SyList<SyScPort*>& ports = tf->getPorts();
        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next) {
            formals.push_back(node->val);
        }

        SyList<pair<SyVCntv*,SyList<SyScCntv*> > > info =
            GetVectorScalarPairs(formals, currMod->getName(), tf->getName());
        
        if(info.size() != args.size())
        {
            SyMsgHandler::instance()->print(30, currMod->getName().c_str(),
                tfcall->getName().c_str());
            status = false;    
            //SyRoot::instance()->removeSynthMod(currMod);    
            return;
        }

        SyNode<pair<SyVCntv*,SyList<SyScCntv*> > >* node = info.head;



        for(int i = 0; i < args.size(); ++i, node = node->next)
        {
            checkOutputPort(dynamic_cast<SyScPort*>(
                node->val.second.head->val), args[i], tfcall->getName());
            

            SyConst *size = args[i]->size();
            assert(size);
            
            if(!IsConstExpr(args[i]) && 
                node->val.second.size() != size->toLong())
            {
                SyMsgHandler::instance()->print(34, currMod->getName().c_str(),
                    tfcall->getName().c_str());
            }        
            args[i] = this->adjustActualInTfCall(args[i], node->val.second.size());
        //    if(IsConstExpr(args[i])) {
        //        args[i] = this->adjustConstActualInTfCall(args[i], node->val.second.size());
        //    }    
          //  else if(node->val.second.size() != size->toLong())
          //  {
            //    SyMsgHandler::instance()->print(34, currMod->getName().c_str(),
            //        tfcall->getName().c_str());
            //    status = false;    
                //SyRoot::instance()->removeSynthMod(currMod);
            //    delete size;
            //    return;
        //    }
            delete size;
        }
    }

    void
    SyOMVisitor::checkOutputPort(const SyScPort *port, const SyExpr *expr,
        const string& task)
    {    
        if(!port || !expr || (port->getDir() != SyScPort::OUT)) {
            return;
        }    
        const SyConst *const_ = dynamic_cast<const SyConst*>(expr);
        const SyCntv *cntv = dynamic_cast<const SyCntv*>(expr);

        if(const_ || !cntv) 
        {
            SyMsgHandler::instance()->print(35, 
                currMod->getName().c_str(), task.c_str());
            status = false;
        }
    }    
#if 0
    bool
    SyOMVisitor::isConstExpr(const SyExpr *expr) const
    {
        if(dynamic_cast<const SyConst*>(expr)) {
            return true;
        }

        const SyConcat *conc = dynamic_cast<const SyConcat*>(expr);
        if(!conc) {
            return false;
        }
        const SyList<SyExpr*>& exprs = conc->getExprs();
        for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next) {
            if(!isConstExpr(node->val)) {
                return false;
            }
        }
        return true;
    }
#endif    

    void
    SyOMVisitor::checkPowerOperator(const SyBinary *binary)
    {
        SyExpr::SyOp op = binary->getOperator();
        if(op != SyExpr::POW) {
            return;
        }

        int val = IsConstExpr(binary->rexpr) ? binary->rexpr->evaluate()->toLong() : INT_MAX;

        if(!IsConstExpr(binary->rexpr) || (val < 0)) 
        {
            SyMsgHandler::instance()->print(42, currMod->getName().c_str(),
                binary->toString().c_str());
            status = false;
        }    
    }

    void
    SyOMVisitor::convertPowerOperator(SyBinary *binary)
    {
        SyExpr::SyOp op = binary->getOperator();
        if(op != SyExpr::POW) {
            return;
        }
        assert(IsConstExpr(binary->rexpr)); 
        SyConst *power = binary->rexpr->evaluate();

        long val = power->toLong();
        if(val == 0) 
        {
            currExpr = SyConst::ONE();
            return;
        }
        if(val == 1)
        {
            currExpr = binary->lexpr;
            return;
        }    
        SyBinary *newbin = new SyBinary(binary->lexpr, binary->lexpr, SyExpr::MULT);
        for(int i = 2; i < val; ++i) {
            newbin = new SyBinary(binary->lexpr, newbin, SyExpr::MULT);
        }    
        currExpr = newbin;
    }

    SyExpr*
    SyOMVisitor::adjustActualInTfCall(SyExpr *act, long size)
    {
        if(!act) {
            return 0;
        }
    //    if(!IsConstExpr(act)) {
    //        return act;
    //    }    

        long actsize = act->size()->toLong();
        if(actsize == size) {
            return act;
        }
        if(actsize < size) {
            return SyExprMgr::extend(act, size-actsize);
        }
        else {
            return SyExprMgr::truncate(act, actsize, size);
        }
        return act; //not reachable

    }

    void
    SyOMVisitor::checkAndSetConstExpr(const SyExpr *expr)
    {
        if(isContext(CONST_COMPACT))
        {
            SyConst *const_ = expr->evaluate();
            if(const_) {
                currExpr = const_;
            }
        }
    }

    
}
