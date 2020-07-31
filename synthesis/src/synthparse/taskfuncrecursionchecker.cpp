/* 
Copyright (C) 2009-2011 Parvez Ahmad
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

#include <cassert>
#include "taskfuncrecursionchecker.h"

#include "symsghandler.h"

#include "module.h"
#include "taskfunc.h"
#include "namedblock.h"
#include "block.h"
#include "loopstmts.h"
#include "case.h"
#include "if.h"
#include "procassign.h"
#include "proccontassign.h"
#include "contassign.h"
#include "always.h"

#include "binexpr.h"
#include "unaryexpr.h"
#include "condexpr.h"
#include "multiconcat.h"

using std::string;
using std::set;
using std::vector;

using Synth::SyMsgHandler;

namespace Rtl
{
    TaskFunctionRecursionChecker::TaskFunctionRecursionChecker() : 
        status( true), module(0) {
    }

    TaskFunctionRecursionChecker::~TaskFunctionRecursionChecker() {
    }

    bool
    TaskFunctionRecursionChecker::getStatus() const {
        return status;
    }    

    void
    TaskFunctionRecursionChecker::push() {
        nameStack.push(set<string, compare_str>());
    }

    void
    TaskFunctionRecursionChecker::push(const string& tfname)
    {
        if(nameStack.empty()) {
            nameStack.push(set<string, compare_str>());
        }
        nameStack.top().insert(tfname);
    }

    void
    TaskFunctionRecursionChecker::pushTop()
    {
        if(nameStack.empty()) {
            push();
        }
        else {
            nameStack.push(nameStack.top());
        }    
    }

    void
    TaskFunctionRecursionChecker::pop() 
    {
        assert(!nameStack.empty());
        nameStack.pop();
    }

    bool
    TaskFunctionRecursionChecker::isVisited(const string& tfname) const 
    {
        if(nameStack.empty()) {
            return false;
        }
        return nameStack.top().find(tfname) != nameStack.top().end();
    }

    void
    TaskFunctionRecursionChecker::visit(const Module *mod)
    {
        module = mod;

        const vector<Task*>& tasks = mod->getTasks();
        const vector<Func*>& funcs = mod->getFuncs();

        for(int i = 0; i < tasks.size(); ++i) 
        {
            Task *task = tasks[i];
            push();
            task->accept(this);
            pop();
            if(!status) {
                return;
            }    
        }
        for(int i = 0; i < funcs.size(); ++i) 
        {
            Func *func = funcs[i];
            push();
            func->accept(this);
            pop();
            if(!status) {
                return;
            }    
        }

    }

    void
    TaskFunctionRecursionChecker::visit(const NamedBlock *nblock)
    {
        if(!status) {
            return;
        }
        const vector<Stmt*>& stmts = nblock->getStmts();
        for(int i = 0; i < stmts.size(); ++i) 
        {
            stmts[i]->accept(this);
            if(!status) {
                return;
            }
        }    
    }

    void
    TaskFunctionRecursionChecker::visit(const Block *block)
    {
        if(!status) {
            return;
        }
        const vector<Stmt*>& stmts = block->getStmts();
        for(int i = 0; i < stmts.size(); ++i) 
        {
            stmts[i]->accept(this);
            if(!status) {
                return;
            }
        }    
    }
    
    void
    TaskFunctionRecursionChecker::visit(const Repeat *repeat)
    {
        if(!status) {
            return;
        }
        repeat->getExpr()->accept(this);
        repeat->getStmt()->accept(this);
    }    

    void
    TaskFunctionRecursionChecker::visit(const For *for_)
    {
        if(!status) {
            return;
        }

        for_->getInitExpr()->accept(this);
        for_->getTermExpr()->accept(this);
        for_->getAssignExpr()->accept(this);
        for_->getStmt()->accept(this);
    }

    void
    TaskFunctionRecursionChecker::visit(const CaseItem *item)
    {
        if(!status) {
            return;
        }
        const vector<Expr*>& label = item->getLabel();
        for(int i = 0; i < label.size(); ++i) 
        {
            label[i]->accept(this);
            if(!status) {
                return;
            }
        }

        item->getStmt()->accept(this);
    }

    void
    TaskFunctionRecursionChecker::visit(const Case *case_)
    {
        if(!status) {
            return;
        }

        case_->getSelectExpr()->accept(this);
        const vector<CaseItem*>& items = case_->getCaseItems();
        for(int i = 0; i < items.size(); ++i)
        {
            items[i]->accept(this);
            if(!status) {
                return;
            }
        }
    }

    void
    TaskFunctionRecursionChecker::visit(const If *if_)
    {
        if(!status) {
            return;
        }

        if_->getCondExpr()->accept(this);
        if(const Stmt *s = if_->getStmt()) {
            s->accept(this);
        }    
        if(const Stmt *s = if_->getElsePart()) {
            s->accept(this);
        }    
    }

    void
    TaskFunctionRecursionChecker::visit(const ProcAssign *passign)
    {
        visitAssign(passign);
    }

    void
    TaskFunctionRecursionChecker::visit(const ProcContAssign* pcassign)
    {
        visitAssign(pcassign);
    }

    void
    TaskFunctionRecursionChecker::visit(const ContAssign *cassign)
    {
        visitAssign(cassign);
    }

    void
    TaskFunctionRecursionChecker::visitAssign(const Assign *assign) 
    {
        assign->getRhs()->accept(this);
    }

    void
    TaskFunctionRecursionChecker::visit(const TaskFuncCall *tcall)
    {
        visitTfCall(tcall);
        if(!tcall->isBuiltIn()) 
        {
            if(tcall->getTask()) {
                tcall->getTask()->accept(this);
            }    
        }    
    }

    void
    TaskFunctionRecursionChecker::visit(const FuncCall *fcall)
    {
        visitTfCall(*fcall);
        if(!fcall->isBuiltIn() && fcall->getFunc()) {
            fcall->getFunc()->accept(this);
        }    
    }

    void
    TaskFunctionRecursionChecker::visitTfCall(const TaskFuncCall *tfcall)
    {
        if(!status) {
            return;
        }

        const vector<Expr*>& args = tfcall->getArgs();
        for(int i = 0; i < args.size(); ++i) {
            if(args[i]) {
                args[i]->accept(this);
            }    
        }    
    }

    void
    TaskFunctionRecursionChecker::visit(const Task *task)
    {
        if(!status) {
            return;
        }
        visitTf(task);
        if(!status) {
            SyMsgHandler::instance()->print(46, task->getLine(), "task", task->getName().c_str());
        }    
        
    }

    void
    TaskFunctionRecursionChecker::visit(const Func *func)
    {
        if(!status) {
            return;
        }
        visitTf(func);
        if(!status) {
            SyMsgHandler::instance()->print(46, func->getLine(), "function", func->getName().c_str());
        }    
    }

    void
    TaskFunctionRecursionChecker::visitTf(const TaskOrFunc *tf)
    {
        string name = tf->getName();
        if(isVisited(name))
        {
            status = false;
            return;
        }    
        pushTop();
        push(name);
        tf->getTaskOrFuncStmt()->accept(this);
        pop();
    }

    void
    TaskFunctionRecursionChecker::visit(const Always *always)
    {
        if(!status) {
            return;
        }
        always->getStmt()->accept(this);
    }

    void
    TaskFunctionRecursionChecker::visit(const BinExpr *bexpr)
    {
        if(!status) {
            return;
        }
        bexpr->getLExpr()->accept(this);
        bexpr->getRExpr()->accept(this);
    }

    void
    TaskFunctionRecursionChecker::visit(const UnaryExpr *uexpr) 
    {
        if(!status) {
            return;
        }
        uexpr->getExpr()->accept(this);
    }

    void
    TaskFunctionRecursionChecker::visit(const Concat *conc)
    {
        if(!status) {
            return;
        }
        const vector<Expr*>& exprs = conc->getExprs();
        for(int i = 0; i < exprs.size(); ++i) {
            exprs[i]->accept(this);
        }    
    }

    void
    TaskFunctionRecursionChecker::visit(const MultiConcat *mconc)
    {
        if(!status) {
            return;
        }
        mconc->getCount()->accept(this);
        const vector<Expr*>& exprs = mconc->getExprs();
        for(int i = 0; i < exprs.size(); ++i) {
            exprs[i]->accept(this);
        }    
    }

    void
    TaskFunctionRecursionChecker::visit(const CondExpr *cexpr)
    {
        if(!status) {
            return;
        }
        cexpr->getCond()->accept(this);
        cexpr->getTExpr()->accept(this);
        cexpr->getFExpr()->accept(this);
    }
}
