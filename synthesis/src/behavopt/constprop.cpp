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

#include "constprop.h"

#include "systmt.h"
#include "syconst.h"
#include "sycontainer.h"
#include "symsghandler.h"
#include "syexprmgr.h"

namespace Synth
{
    /**********************************************************************
     * Check if there is any non blocking assignment. If any found then the
     * list is not optimizable.
     * The list is divided into smaller lists, stopping at other than,
     * blocking assignments. Optimize this smaller list
     * If an if statement is encountered, then check for its condition.
     * If the condition part is constant then throw away the un used 
     * branch and start optimization from the relevant branch. In this
     * case the if statement is removed and the statements contained in
     * the true branch is merged with the previous list.
     * If a case statement is encountered, check for it branch statement.
     * If the branch statement is non const then the module is not
     * synthesizable. If it is constant then change the branch statement
     * to this constant and start the process for statements in all case
     * items.
     *********************************************************************/
    SyList<SyStmt*>*
        ConstProp::optimize(
                SyBModule *mod,
                const SyList<SyStmt*>* stmts) const
        {
            if(!stmts){
                return 0;
            }
            if(!this->checkAllBlocking(stmts))
            {
                SyList<SyStmt*>* retstmts = new SyList<SyStmt*>();
                for(SyNode<SyStmt*>* node = stmts->head; node; 
                        node = node->next){
                    retstmts->push_back(node->val->copy());
                }
                return retstmts;
            }
            return this->optimizeStatements1(mod, stmts);
        }

    SyList<SyStmt*>*
        ConstProp::optimizeStatements1(
                SyBModule *mod,
                const SyList<SyStmt*>* stmts
                ) const
        {
            mod->resetCntvs();
            return this->optimizeStatements2(mod, stmts);
        }


    SyList<SyStmt*>*
        ConstProp::optimizeStatements2(
                SyBModule *mod,
                const SyList<SyStmt*>* stmts
                ) const
        {
            SyList<SyStmt*>* newstmts = new SyList<SyStmt*>();
            for(SyNode<SyStmt*>* node = stmts->head; node; node = node->next)
            {
                if(SyAssign *assign = dynamic_cast<SyAssign*>(node->val))
                {
                    newstmts->push_back(assign);
                    SyConst *const_ = assign->getRExpr()->evaluate();
                    if(!const_){
                        continue;
                    }
                    SyExprMgr::instance()->processAssign(assign);
                    delete const_;
                    const_ = assign->getRExpr()->evaluate();
                    assert(const_);
                    SyExpr *lexpr = const_cast<SyExpr*>(assign->getLExpr());
                    lexpr->addVal(const_);
                    SyConst *size1 = const_->size();
                    SyConst *size2 = lexpr->size();
                    if(*size2 < *size1)
                    {
                        int msb = size1->toLong()-size2->toLong();
                        SyConst tmp = const_->trunc(msb);
                        delete const_;
                        const_ = tmp.copy();
                    }
                    delete size1;
                    delete size2;
                    const_->setSign(assign->getRExpr()->isSigned());

                    assign->setRExpr(const_);
                    delete const_;
                }
                else if(SyIf *if_ = dynamic_cast<SyIf*>(node->val))
                {
                    SyConst *cond = if_->getCond()->evaluate();
                    if(cond)
                    {
                        if(cond->iszero()/* == *SyConst::ZERO()*/) {
                            newstmts->splice(this->optimizeStatements2(
                                        mod, &(if_->getElsePart())));
                        }
                        else {
                            newstmts->splice(this->optimizeStatements2(
                                        mod, &(if_->getThenPart())));
                        }
                    }
                    else 
                    {
                        SyList<SyStmt*> tstmts = if_->getThenPart();
                        if_->setThenPart(*(this->optimizeStatements1(
                                    mod, &tstmts)));
                        tstmts.clear();
                        tstmts = if_->getElsePart();
                        if_->setElsePart(*(this->optimizeStatements1(
                                    mod, &tstmts)));
                        tstmts.clear();
                        newstmts->push_back(if_);
                    }
                }
                else if(SyCase *case_ = dynamic_cast<SyCase*>(node->val))
                {
                    mod->resetCntvs();

                    SyList<SyCaseItem*>& items = 
                        const_cast<SyList<SyCaseItem*>&>(case_->getCaseItems());
                    for(SyNode<SyCaseItem*>* node1 = items.head; node1;
                            node1 = node1->next)
                    {
                        SyList<SyStmt*>& itemStmts = 
                            const_cast<SyList<SyStmt*>&>(node1->val->getStmts());
                        SyList<SyStmt*> titemStmts = itemStmts;
                        itemStmts.clear();
                        SyList<SyStmt*>* newItemStmts = 
                            this->optimizeStatements2(mod, &titemStmts);
                        for(SyNode<SyStmt*>* node2 = newItemStmts->head;
                                node2; node2 = node2->next) { 
                            itemStmts.push_back(node2->val);
                        }
                        delete newItemStmts;
                    }
                    newstmts->push_back(case_);
                }
                else 
                {
                    mod->resetCntvs();
                    newstmts->push_back(node->val);
                }
            }
            return newstmts;
        }




    bool
        ConstProp::checkAllBlocking(const SyList<SyStmt*>* stmts) const
        {
            //Presently constant propagation is done only if all
            //none of the statements is non-blocking in nature.

            for(SyNode<SyStmt*>* node = stmts->head; node; node = node->next){
                if(!this->checkAllBlocking(node->val)){
                    return false;
                }
            }
            return true;
        }

    bool
        ConstProp::checkAllBlocking(const SyStmt* stmt) const
        {
            if(/*const SyNonBlockAssign *nbassign = */ 
                    dynamic_cast<const SyNonBlockAssign*>(stmt)){
                return false;
            }

            if(const SyIf *if_ = dynamic_cast<const SyIf*>(stmt))
            {
                const SyList<SyStmt*>& thenstmts = if_->getThenPart();
                if(!this->checkAllBlocking(&thenstmts)){
                    return false;
                }
                const SyList<SyStmt*>& elsestmts = if_->getElsePart();
                if(!this->checkAllBlocking(&elsestmts)){
                    return false;
                }
            }
            else if(const SyCase *case_ = dynamic_cast<const SyCase*>(stmt))
            {
                const SyList<SyCaseItem*>& items = case_->getCaseItems();

                for(SyNode<SyCaseItem*>* node = items.head; node; 
                        node = node->next)
                {
                    const SyList<SyStmt*>& stmts = node->val->getStmts();
                    if(!this->checkAllBlocking(&stmts)){
                        return false;
                    }
                }
            }
            return true;
        }
                

}
