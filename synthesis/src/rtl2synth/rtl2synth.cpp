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

#include <math.h>
#include <iostream>

#include "rtl2synth.h"
#include "systmt.h"
#include "syconst.h"
#include "syroot.h"
#include "sycontainer.h"
#include "syexprmgr.h"

using std::map;
using std::make_pair;
namespace Synth
{
    Rtl2Synth* Rtl2Synth::rtl2synth = 0;

    Rtl2Synth::Rtl2Synth(){
    }

    Rtl2Synth::~Rtl2Synth() {
    }

    Rtl2Synth*
        Rtl2Synth::instance() 
        {
            if(!rtl2synth){
                rtl2synth = new Rtl2Synth();
            }
            return rtl2synth;
        }


    const SyList<const SyScCntv*>*
    Rtl2Synth::getLatchVars(const string& modName) const 
    {
        map<string, SyList<const SyScCntv*>*, cmp_str>::const_iterator
            iter = moduleLatches.find(modName);
        
        if(iter == moduleLatches.end()){
            return NULL;
        }
        return iter->second;
    }    

    void
    Rtl2Synth::addLatchVars(const string& modName, 
                            SyList<const SyScCntv*>* latchVars)
    {
        SyList<const SyScCntv*>* tmp = const_cast<SyList<const SyScCntv*>*>(
            this->getLatchVars(modName));
        
        if(tmp && latchVars) {
            tmp->merge(*latchVars);
        }    
        else if(latchVars){
            moduleLatches.insert(make_pair(modName, latchVars));
        }
    }    

            
        

    void
        Rtl2Synth::translate()
        {
            SyRoot::setStage(SyRoot::STAGE_1);
            map<string, SyModule*, cmp_str> modules = 
            SyRoot::instance()->getSynthMods();
            map<string, SyModule*, cmp_str>::const_iterator mod = modules.begin();
            map<string, SyModule*, cmp_str>::const_iterator modEnd = modules.end();

            for(; mod != modEnd; ++mod) {
                this->translate(mod->second);
            }
            SyExprMgr::instance()->process();
        }

    SyModule*
        Rtl2Synth::translate(const SyModule *mod) const
        {
            SyModule *synthMod = new SyModule(mod->getName());
            SyList<SyAlways*> alwaysBlocks;
            if(const SyBModule* bmod = dynamic_cast<const SyBModule*>(mod)){
                alwaysBlocks = bmod->getAlwaysBlocks();
            }

            for(SyNode<SyAlways*>* node = alwaysBlocks.head; node; 
                    node = node->next)
            {
                SyList<const SyScCntv*>* cntvs = this->detectLatch(node->val);
                //cout << "---------Latch Variables----------\n";
                //for(SyNode<const SyScCntv*>* cntv = cntvs->head; cntv; 
                //        cntv = cntv->next){
                //    std::cout << cntv->val->toString() << std::endl;
                    
                //}
                //cout << "---------End Latch Variables------\n";
                moduleLatches.insert(make_pair(synthMod->getName(), cntvs));
                
            }
            return synthMod;
        }

    /*------------------Latch detection routines-----------------------------------*/
    SyList<const SyScCntv*>*
        Rtl2Synth::detectLatch(const SyAlways *always) const
        {
            map<const SyScCntv*, bool, cmp_cntvs> latchVars;
            this->detectLatch(always, latchVars);
            SyList<const SyScCntv*> *latchVarList = new SyList<const SyScCntv*>();

            for(map<const SyScCntv*, bool, cmp_cntvs>::const_iterator iter = latchVars.begin();
                    iter != latchVars.end(); ++iter){
                if(iter->second){
                    latchVarList->push_back(iter->first);
                }
            }
            return latchVarList;
        }

    void
        Rtl2Synth::detectLatch(
                const SyStmt *stmt,
                map<const SyScCntv*, bool, cmp_cntvs>& wcntvs) const
        {
            if(const SyAssign *assign = dynamic_cast<const SyAssign*>(stmt)){
                this->detectLatchAssign(assign, wcntvs);
            }
            else if(const SyIf *if_ = dynamic_cast<const SyIf*>(stmt)) {
                this->detectLatchIf(if_, wcntvs);
            }
            else if(const SyCase *case_ = dynamic_cast<const SyCase*>(stmt)) {
                this->detectLatchCase(case_, wcntvs);
            }
            else if(const SyEventStmt *eventStmt = 
                    dynamic_cast<const SyEventStmt*>(stmt)){
                this->detectLatchEvent(eventStmt, wcntvs);
            }
        }

    void
        Rtl2Synth::detectLatchAssign(
                const SyAssign *assign,
                map<const SyScCntv*, bool, cmp_cntvs>& wcntvs) const
        {
            if(const SyVCntv *vcntv = dynamic_cast<const SyVCntv*>(
                        assign->getLExpr()))
            {
                const SyList<SyScCntv*>& bits = vcntv->getBits();
                for(SyNode<SyScCntv*>* node = bits.head; node;
                        node = node->next)
                {
                    map<const SyScCntv*, bool, cmp_cntvs>::iterator iter =
                        wcntvs.find(node->val);
                    if(iter == wcntvs.end()){ 
                        wcntvs.insert(make_pair(node->val, false));
                    }
                }
            }
            else if(const SyScCntv *sccntv = dynamic_cast<const SyScCntv*>(
                        assign->getLExpr()))
            {
                map<const SyScCntv*, bool, cmp_cntvs>::iterator iter =
                    wcntvs.find(sccntv);
                if(iter == wcntvs.end()){ 
                    wcntvs.insert(make_pair(sccntv, false));
                }
            }
        }
    void
        Rtl2Synth::detectLatchIf(
                const SyIf *if_,
                map<const SyScCntv*, bool, cmp_cntvs>& wcntvs) const
        {
            //TBD: to consider local variables not as latch
            const SyList<SyStmt*>& thenPart = if_->getThenPart();
            map<const SyScCntv*, bool, cmp_cntvs> thencntvs =
                wcntvs;
            for(SyNode<SyStmt*>* node = thenPart.head; node; node = node->next){
                this->detectLatch(node->val, thencntvs);
            }
            //merge
            const SyList<SyStmt*>& elsePart = if_->getElsePart();
            map<const SyScCntv*, bool, cmp_cntvs> elsecntvs = wcntvs;
            for(SyNode<SyStmt*>* node = elsePart.head; node; node = node->next){
                this->detectLatch(node->val, elsecntvs);
            }

            for(map<const SyScCntv*, bool, cmp_cntvs>::const_iterator iter =
                    thencntvs.begin(); iter != thencntvs.end(); ++iter)
            {
                if(!iter->second)
                {
                    wcntvs.erase(iter->first);
                    wcntvs.insert(*iter);
                    continue;
                }
                map<const SyScCntv*, bool, cmp_cntvs>::iterator titer =
                    elsecntvs.find(iter->first);
                if((titer == elsecntvs.end()) || !titer->second) {
                    wcntvs.erase(iter->first);
                    wcntvs.insert(make_pair(iter->first, false));
                }
            }
            for(map<const SyScCntv*, bool, cmp_cntvs>::const_iterator iter =
                    elsecntvs.begin(); iter != elsecntvs.end(); ++iter)
            {
                if(!iter->second)
                {
                    wcntvs.erase(iter->first);
                    wcntvs.insert(*iter);
                    continue;
                }
                map<const SyScCntv*, bool, cmp_cntvs>::iterator titer =
                    thencntvs.find(iter->first);
                if((titer == thencntvs.end()) || !titer->second) {
                    wcntvs.erase(iter->first);
                    wcntvs.insert(make_pair(iter->first, false));
                }
            }
            for(map<const SyScCntv*, bool, cmp_cntvs>::const_iterator iter =
                    thencntvs.begin(); iter != thencntvs.end(); ++iter)
            {
                map<const SyScCntv*, bool, cmp_cntvs>::const_iterator titer =
                    wcntvs.find(iter->first);
                //TBD: stop propagating local variables up
                if(titer == wcntvs.end()){
                    wcntvs.insert(*iter);
                }
            }
            for(map<const SyScCntv*, bool, cmp_cntvs>::const_iterator iter =
                    elsecntvs.begin(); iter != elsecntvs.end(); ++iter)
            {
                map<const SyScCntv*, bool, cmp_cntvs>::const_iterator titer =
                    wcntvs.find(iter->first);
                //TBD: stop propagating local variables up
                if(titer == wcntvs.end()){
                    wcntvs.insert(*iter);
                }
            }


        }

    void
        Rtl2Synth::detectLatchCase(
                const SyCase *case_,
                map<const SyScCntv*, bool, cmp_cntvs>& wcntvs) const
        {
            const SyList<SyCaseItem*>& caseItems = case_->getCaseItems();
            //check if all case labels have been used.
            //to implement synopsys full_case kind of stuff here.
            int size = case_->getSelect()->size()->toLong();
            int expectedLabels = pow(2, size);
            if(caseItems.size() != expectedLabels)
            {
                //mark all previous variables as not assigned in missing
                //case labels.
                for(map<const SyScCntv*, bool, cmp_cntvs>::iterator iter = 
                        wcntvs.begin(); iter != wcntvs.end(); ++iter){
                    iter->second = false;
                }
            }

            SyList<map<const SyScCntv*, bool, cmp_cntvs> > ccntvs;
            for(SyNode<SyCaseItem*>* node = caseItems.head; node; 
                    node = node->next)
            {
                const SyList<SyStmt*>& stmts = node->val->getStmts();
                this->detectLatchStmts(stmts, wcntvs);
            }
        }

    void
        Rtl2Synth::detectLatchEvent(
                const SyEventStmt *eventStmt,
                map<const SyScCntv*, bool, cmp_cntvs>& wcntvs) const
        {
            const SyList<SyStmt*>& stmts = eventStmt->getStmts();
            this->detectLatchStmts(stmts, wcntvs);
        }

    void
        Rtl2Synth::detectLatchStmts(
                const SyList<SyStmt*>& stmts,
                map<const SyScCntv*, bool, cmp_cntvs>& wcntvs) const
        {
            SyList<map<const SyScCntv*, bool, cmp_cntvs> > cntvs;
            for(SyNode<SyStmt*>* node = stmts.head; node; 
                node = node->next)
            {
                map<const SyScCntv*, bool, cmp_cntvs> cntv = wcntvs;
                this->detectLatch(node->val, cntv);
                cntvs.push_back(cntv);
            }
            for(SyNode<map<const SyScCntv*, bool, cmp_cntvs> >* node1 =
                    cntvs.head; node1 && node1->next; node1 = node1->next)
            {
                for(SyNode<map<const SyScCntv*, bool, cmp_cntvs> >* node2 =
                    node1->next; node2; node2 = node2->next)
                {

                    for(map<const SyScCntv*, bool, cmp_cntvs>::const_iterator 
                            iter = node1->val.begin(); 
                            iter != node1->val.end(); ++iter)
                    {
                        if(!iter->second)
                        {
                            wcntvs.erase(iter->first);
                            wcntvs.insert(*iter);
                            continue;
                        }
                        map<const SyScCntv*, bool, cmp_cntvs>::iterator titer =
                            node2->val.find(iter->first);
                        if((titer == node2->val.end()) || !titer->second) 
                        {
                            wcntvs.erase(iter->first);
                            wcntvs.insert(make_pair(iter->first, false));
                        }
                    }
                }
            }
            for(SyNode<map<const SyScCntv*, bool, cmp_cntvs> >* node =
                    cntvs.head; node; node = node->next)
            {
                for(map<const SyScCntv*, bool, cmp_cntvs>::const_iterator iter =
                    node->val.begin(); iter != node->val.end(); ++iter)
                {
                    map<const SyScCntv*, bool, cmp_cntvs>::const_iterator titer =
                        wcntvs.find(iter->first);
                    //TBD: stop propagating local variables up
                    if(titer == wcntvs.end()){
                        wcntvs.insert(*iter);
                    }
                }
            }
        }
    /*------------------End Latch detection routines--------------------------------*/


}
