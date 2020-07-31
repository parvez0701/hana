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

#include "assignstmtstrans.h"
#include "systmt.h"
#include "namemgr.h"
#include "syvisitor.h"

namespace Synth
{
    SyAssignStmtsTrans::SyAssignStmtsTrans(SyModule *mod) : 
        module(mod), currNode(0){
    }

    SyAssignStmtsTrans::~SyAssignStmtsTrans() {
    }

    void
    SyAssignStmtsTrans::visit(SyList<SyStmt*>& stmts)
    {
        newStmts.clear();
        if(stmts.size() < 2) {
            return;
        }

        SyList<SyStmt*> tstmts;
        for(SyNode<SyStmt*>* n = stmts.head; n; n = n->next) {
            //Work on consecutive block assigns
            if(dynamic_cast<SyBlockAssign*>(n->val)) {
                tstmts.push_back(n->val);
            }
            else {
                for(currNode = tstmts.head; currNode; currNode = currNode->next) {
                    currNode->val->accept(this);
                }    
                currNode = 0;
                tstmts.clear();
                newStmts.push_back(n->val);
            }    
        }
        for(currNode = tstmts.head; currNode; currNode = currNode->next) {
            currNode->val->accept(this);
        }    
        stmts = newStmts;
        //SyVisitor v;
        //for(SyNode<SyStmt*> *n = stmts.head; n; n = n->next) {
        //    n->val->accept(&v);
        //}    
    }

    void
    SyAssignStmtsTrans::visit(const SyBlockAssign *assign) {
        transformAssign(const_cast<SyBlockAssign*>(assign));
    }

    void
    SyAssignStmtsTrans::visit(const SyNonBlockAssign *assign) {
        transformAssign(const_cast<SyNonBlockAssign*>(assign));
    }

    void
    SyAssignStmtsTrans::transformAssign(SyAssign *assign)
    {
        bool isblkassign = dynamic_cast<SyBlockAssign*>(assign) ? true : false;
        const SyExpr *lexpr = assign->getLExpr();
        SyList<SyCntv*> flist;
        FlattenLValue(lexpr, flist);

        bool changed = false;
        for(SyNode<SyCntv*>* node = flist.head; node; node = node->next)
        {
            SyCntv *lhsc = node->val;
            if(checkVariableRead(lhsc)) 
            {
                changed = true;
                break;
            }    

        }    
        newStmts.push_back(assign);
        if(!changed) {
            return;
        }    

        std::map<string, SyCntv*, cmp_str> oldName2RegMap;

        for(SyNode<SyCntv*>* node = flist.head; node; node = node->next)
        {
            SyCntv *lhsc = node->val;
            if(!checkVariableRead(lhsc)) {
                continue;
            }    
            SyScReg *reg = new SyScReg(ModuleNameManager::instance()->
                createName(module, "reg"), lexpr->isSigned());
            module->addObject(    reg->getIxName().getName(), reg);
            module->addReg(reg);
            reg->setLocal();

            if(isblkassign) {
                newStmts.push_back(new SyBlockAssign(reg, lhsc));
            }    
            else { 
                newStmts.push_back(new SyNonBlockAssign(reg, lhsc));
            }   
            oldName2RegMap.insert(make_pair(lhsc->toString(), reg));    
        }

        //Now replace by new reg on rhs of assigns.
        for(std::map<string, SyCntv*, cmp_str>::const_iterator iter =
            oldName2RegMap.begin(); iter != oldName2RegMap.end(); ++iter)
        {    
            for(SyNode<SyStmt*>* s = currNode->next; s; s = s->next)
            {
                SyAssign *assign = dynamic_cast<SyAssign*>(s->val);
                if(!assign) {
                    continue;
                }
                const SyExpr *lhs = assign->getLExpr();
                SyExpr *rhs = const_cast<SyExpr*>(assign->getRExpr());
                if(rhs->contains(iter->first)){
                    rhs->repSubExpr(iter->first, iter->second);
                }    
                if(lhs->contains(iter->first)) {
                    break;
                }
            }    
        }

    }


    bool
    SyAssignStmtsTrans::checkVariableRead(const SyCntv *cntv) const
    {
        for(SyNode<SyStmt*>* node = currNode; node; node = node->next)
        {
            SyAssign *assign = dynamic_cast<SyAssign*>(node->val);
            if(!assign) {
                continue;
            }
            SyExpr *rexpr = const_cast<SyExpr*>(assign->getRExpr());
            rexpr->repVCntvByConcat();

            if(rexpr->contains(cntv->toString())) {
                return true;
            }    
        }
        return false;
    }

    bool
    SyAssignStmtsTrans::checkVariableRead(const SyCntv *cntv, const SyAssign* assign) const
    {
        return assign->getRExpr()->contains(cntv->toString());
    }

    bool
    SyAssignStmtsTrans::allBlockAssign(const SyList<SyStmt*>& stmts) const
    {
        for(SyNode<SyStmt*>* s = stmts.head; s; s = s->next)
        {
            if(dynamic_cast<SyNonBlockAssign*>(s->val)) {
                return false;
            }    
        }
        return true;
    }

    
}
