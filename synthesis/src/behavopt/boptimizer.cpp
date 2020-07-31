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

#include <map>
#include "boptimizer.h"
#include "syomvisitor.h"
#include "constprop.h"
#include "sycontainer.h"
#include "syroot.h"
#include "systmt.h"
#include "syexprmgr.h"
#include "syconst.h"
#include "syconstprop.h"

using std::map;
using std::string;

namespace Synth
{
    BOptimizer *BOptimizer::boptimizer = 0;

    BOptimizer::BOptimizer()
    {
    }

    BOptimizer::~BOptimizer()
    {
        boptimizer = 0;
    }

    BOptimizer*
        BOptimizer::instance()
        {
            if(!boptimizer){
                boptimizer = new BOptimizer();
            }
            return boptimizer;
        }

    void
        BOptimizer::optimize() const
        {
        /*/
            map<string, SyModule*, cmp_str> modules = 
            SyRoot::instance()->getSynthMods();
            map<string, SyModule*, cmp_str>::iterator mod = 
                modules.begin();
            map<string, SyModule*, cmp_str>::iterator modEnd = 
                modules.end();

            for(; mod != modEnd; ++mod) {
                this->propagateConsts(dynamic_cast<SyBModule*>(mod->second));
            }
        */    
            SyOMVisitor v1(SyOMVisitor::BEHAV_OPT);
            v1.visit();

            SyOMVisitor v2(SyOMVisitor::TF_COPY);
            v2.visit();


            SyConstProp cprop;
            cprop.visitObjectModel();
            SyExprMgr::instance()->process();

            SyOMVisitor v3(SyOMVisitor::CONST_COMPACT);
            v3.visit();

        }

    void
        BOptimizer::propagateConsts(SyBModule *mod) const
        {
            if(!mod){
                return;
            }
            this->optimizeContAssigns(mod);
            const SyList<SyAlways*>& alwaysBlocks = mod->getAlwaysBlocks();
            for(SyNode<SyAlways*>* node = alwaysBlocks.head; node;
                    node = node->next) {
                SyAlways *always = node->val;
                this->propagateConsts(mod, const_cast<SyList<SyStmt*>&>(
                            always->getStmts()));
            }
            const SyList<SyTask*>& tasks = mod->getTasks();
            for(SyNode<SyTask*>* node = tasks.head; node;
                    node = node->next) {
                this->propagateConsts(mod, node->val);
            }
            const SyList<SyFunc*>& funcs = mod->getFuncs();
            for(SyNode<SyFunc*>* node = funcs.head; node;
                    node = node->next) {
                this->propagateConsts(mod, node->val);
            }
        }

    void
        BOptimizer::propagateConsts(
                SyBModule *mod,
                SyList<SyStmt*>& oldstmts) const
        {
            ConstProp constprop;
            SyList<SyStmt*>* newstmts = constprop.optimize(mod, 
                        &oldstmts);
            oldstmts.clear();
            for(SyNode<SyStmt*>* node = newstmts->head; node; 
                    node = node->next) {
                oldstmts.push_back(node->val);
            }
            delete newstmts;
        }
    void
        BOptimizer::propagateConsts(
                SyBModule *mod,
                SyTaskOrFunc *taskorfunc) const
        {
            //This function is no-op for now.
            //The port of a function/task is not moved inside module scope.
            //This is causing some issues in fecthing the vector connective.
            //So effectively constant is not propagated inside task or function.
            return;
            ConstProp constprop;
            SyList<SyStmt*>& oldstmts = const_cast<SyList<SyStmt*>&>(
                    taskorfunc->getStmts());
            SyList<SyStmt*>* newstmts = constprop.optimize(mod, 
                        &oldstmts);
            oldstmts.clear();
            for(SyNode<SyStmt*>* node = newstmts->head; node; 
                    node = node->next) {
                taskorfunc->addStmt(node->val);
            }
            delete newstmts;
        }

    void
        BOptimizer::optimizeContAssigns(SyBModule *mod) const
        {
            const SyList<SyContAssign*>& contAssigns = mod->getContAssigns();

            for(SyNode<SyContAssign*>* node = contAssigns.head; node; node = node->next)
            {
                SyContAssign *assign = node->val;
                SyConst *const_ = assign->getRExpr()->evaluate();
                if(!const_){
                    continue;
                }
                SyExprMgr::instance()->processAssign(assign);
                delete const_;
                const_ = assign->getRExpr()->evaluate();
                assign->setRExpr(const_);
            }
        }
}
