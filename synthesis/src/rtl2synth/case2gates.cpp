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
#include <sstream>

#include "systmt.h"
#include "syconst.h"
#include "case2gates.h"
#include "stmt2gates.h"

using std::make_pair;
using std::cout;
using std::ostringstream;
using std::string;
using std::map;
using std::pair;
using std::for_each;

namespace Synth
{
    CntvInfo::CntvInfo(const SyCase *c): case_(const_cast<SyCase*>(c)) {
    }

    CntvInfo::~CntvInfo() 
    {
        InnerCaseCntvInfo::iterator inCntvInfoBegin = innerCaseCntvInfo.begin();
        InnerCaseCntvInfo::iterator inCntvInfoEnd = innerCaseCntvInfo.end();


        for(; inCntvInfoBegin != inCntvInfoEnd; ++inCntvInfoBegin) {
            for_each(inCntvInfoBegin->second.begin(), 
                inCntvInfoBegin->second.end(), 
                Delete<CntvInfo>());
        }        

        CntvAssignInfoMap::iterator iter = cntvAssignInfoMap.begin();
        CntvAssignInfoMap::iterator iterEnd = cntvAssignInfoMap.end();

        for(; iter != iterEnd; ++iter)
        {
            
            for_each(iter->second.begin(), iter->second.end(),
                Delete<CntvAssignsInCaseItem>());
        }
    }    

    int
    CntvInfo::getSelectSize() const 
    {
        const SyExpr *select = case_->getSelect();
        SyConst *const_ = select->size();
        long size = const_->toLong();
        delete const_;
        return size;
    }    

    CntvInfo*
    CntvInfo::analyze(const SyCase* c)
    {
        CntvInfo *cntvInfo = new CntvInfo(c);
        cntvInfo->analyze();
        return cntvInfo;
    }

    void
    CntvInfo::analyze()
    {
        this->populateCntvInfo();
        this->merge();
        this->populateOutCntvInfo();
        this->print();
    }

    void
    CntvInfo::populateCntvInfo()
    {
        const SyList<SyCaseItem*>& caseItems = case_->getCaseItems();

        for(SyNode<SyCaseItem*>* node = caseItems.head; node; node = node->next){
            this->populateCntvInfo(node->val);
        }    
    }

    void
    CntvInfo::populateCntvInfo(const SyCaseItem *item)
    {
        const SyExpr *lexpr = item->getLabel();
        const SyConst *const_ = lexpr->evaluate();
        assert(const_);

        int label = const_->toLong();
        delete const_;
        const SyList<SyStmt*>& stmts = item->getStmts();
        for(SyNode<SyStmt*>* st = stmts.head; st; st = st->next)
        {
            this->checkAndAdd(st->val, label);
        }
    }    

    void
    CntvInfo::checkAndAdd(const SyStmt *stmt, int label)
    {
        if(const SyAssign *assign = dynamic_cast<const SyAssign*>(stmt)){
            this->checkAndAdd(assign, label);
        }
        else if(const SyCase *c = dynamic_cast<const SyCase*>(stmt)) 
        {
            this->checkAndAdd(c, label);
        }    
    }

    void
    CntvInfo::checkAndAdd(const SyAssign* assign, int label)
    {
        const SyExpr *lhs = assign->getLExpr();
        const SyExpr *rhs = assign->getRExpr();
        SyList<const SyCntv*> lhsCntvs;
        CreateFlatList<const SyCntv*>(lhsCntvs, lhs);

        if(!dynamic_cast<const SyCntv*>(rhs)) 
        {
            //complex expression, can not flatten
            const SyCntv *lhsc = dynamic_cast<const SyCntv*>(lhs);
            assert(lhsc);
            bool isBlockAssign = dynamic_cast<const SyBlockAssign*>(assign) != 0;
            this->checkAndAdd(lhsc, rhs, label, isBlockAssign);
            for(SyNode<const SyCntv*>* node = lhsCntvs.head; node; node = node->next) {
                this->checkAndAdd(node->val, rhs, label, isBlockAssign);
            }    
            return;
        }    

        SyList<const SyExpr*> rhsCntvs;


        if(const SyConst *const_ = dynamic_cast<const SyConst*>(rhs))
        {
            vector<SyConst*> bits = const_->getBits();
            for(unsigned int i = 0; i < bits.size(); ++i) {
                rhsCntvs.push_back(bits[i]);
            }
            int rem = lhsCntvs.size()-bits.size();
            for(int i = 0; i < rem; ++i) {
                //sign extension ?
                rhsCntvs.push_front(SyConst::ZERO());
            }    
        }
        else {
            CreateFlatList<const SyExpr*>(rhsCntvs, rhs);
        }    
        lhsCntvs.reverse();
        rhsCntvs.reverse();

        SyNode<const SyCntv*> *lnode = lhsCntvs.head;
        SyNode<const SyExpr*> *rnode = rhsCntvs.head;
        for(; lnode && rnode; lnode = lnode->next, rnode = rnode->next){
            this->checkAndAdd(lnode->val, rnode->val, label, 
            dynamic_cast<const SyBlockAssign*>(assign) != 0);
        }    

    }

    void
    CntvInfo::init(const SyCntv *lhs)
    {
        CntvAssignsInCase caic;
        int selectSize = this->getSelectSize();
        int itemSize = pow(2, selectSize);
        for(int i = 0; i < itemSize; ++i){
            caic.push_back( new CntvAssignsInCaseItem());
        }
        cntvAssignInfoMap.insert(
            make_pair(const_cast<SyCntv*>(lhs), caic));
    }    

    void
    CntvInfo::checkAndAdd(const SyCntv *lhs, const SyExpr* rhs, int label, bool isBlock)
    {
        CntvAssignInfoMap::iterator iter = 
            cntvAssignInfoMap.find(const_cast<SyCntv*>(lhs));
        if(iter == cntvAssignInfoMap.end()) 
        {
            this->init(lhs);
            iter = cntvAssignInfoMap.find(const_cast<SyCntv*>(lhs));
        }    
        assert(iter != cntvAssignInfoMap.end());
        ((iter->second))[label]->push_back(
            make_pair(const_cast<SyExpr*>(rhs), isBlock));
    }    

    void
    CntvInfo::checkAndAdd(const SyCase *c, int label)
    {
        CntvInfo *cntvInfo = this->analyze(c);
        InnerCaseCntvInfo::iterator iter = innerCaseCntvInfo.find(label);

        if(iter == innerCaseCntvInfo.end()) 
        {
            vector<CntvInfo*> vci;
            for(int i = 0; i < label; ++i) {
                vci.push_back(NULL);
            }
            vci.push_back(cntvInfo);
            innerCaseCntvInfo.insert(make_pair(label, vci));
        }
        else 
        {
            for(int i = iter->second.size(); i <= label; ++i) {
                iter->second.push_back(NULL);
            }    
            iter->second[label] = cntvInfo;
        }
    }    

    bool
    CntvInfo::assignsEquivalent(const SyCntv *cntv, int label1, int label2) const
    {
        CntvAssignInfoMap::const_iterator iter = 
            cntvAssignInfoMap.find(const_cast<SyCntv*>(cntv));
        assert(iter != cntvAssignInfoMap.end());

        CntvAssignsInCaseItem* vec1 = ((iter->second))[label1];
        CntvAssignsInCaseItem* vec2 = ((iter->second))[label2];
        if(!vec1 && !vec2) {
            return true;
        }
        if(!vec1 || !vec2) {
            return false;
        }
        unsigned int size = vec1->size();
        if(size != vec2->size()){
            return false;
        }    

        for(unsigned int i = 0; i < size; ++i)
        {
            string cntv1 = (*vec1)[i].first->toString();
            bool type1 = (*vec1)[i].second;
            string cntv2 = (*vec2)[i].first->toString();
            bool type2 = (*vec2)[i].second;

            if((cntv1 != cntv2) || (type1 != type2)) {
                return false;
            }
        }
        return true;
    }

    void
    CntvInfo::removeCntvEntry(const SyCntv *cntv, int label)
    {
        CntvAssignInfoMap::iterator iter = 
            cntvAssignInfoMap.find(const_cast<SyCntv*>(cntv));
        if(iter == cntvAssignInfoMap.end()){
            return;
        }
        if(label < 0 || label > ((int)(iter->second.size()))){
            return;
        }    
        (iter->second)[label] = NULL;
    }    

    void
    CntvInfo::merge()
    {
        //We require a smaller mux from case statement is debatable.
        // Turning it off for now
        return;
        SyList<SyCaseItem*> items = case_->getCaseItems();
        const SyExpr *select = case_->getSelect();
        long itemCount = pow(2, select->size()->toLong());

        this->sortCaseItems(items, itemCount);
        this->merge(items, true);
        items.reverse();
        this->merge(items, false);
    }    

    void
    CntvInfo::merge(SyList<SyCaseItem*>& items, bool fromTop)
    {
        //once known to be non mergeable, store the connective
        set<CntvWrap> nonMergeable;

        for(SyNode<SyCaseItem*>* node = items.head; node->next; node = node->next)
        {
            const SyList<SyStmt*>& stmts = node->val->getStmts();
            const SyExpr *lexpr = node->val->getLabel();

            const SyConst *const_ = lexpr->evaluate();
            assert(const_);
            int label = const_->toLong();
            int nextLabel = fromTop ? label + 1 : label -1;
            delete const_;


            for(SyNode<SyStmt*>* st = stmts.head; st; st = st->next)
            {
                SyAssign *assign = dynamic_cast<SyAssign*>(st->val);
                if(!assign) {
                    continue;
                }    
                SyList<const SyCntv*> lhs;
                if(dynamic_cast<const SyCntv*>(assign->getRExpr())) {
                    CreateFlatList<const SyCntv*>(lhs, assign->getLExpr());
                }
                else {
                    const SyCntv *lcntv = 
                        dynamic_cast<const SyCntv*>(assign->getLExpr());
                    assert(lcntv);    
                    lhs.push_back(lcntv);
                }    
                for(SyNode<const SyCntv*>* cntv = lhs.head; cntv; cntv = cntv->next)
                {
                
                    if(!this->assignsEquivalent(cntv->val, label, nextLabel)) 
                    {
                        nonMergeable.insert(CntvWrap(cntv->val));
                        continue;
                    }
                    if(nonMergeable.find(CntvWrap(cntv->val)) == nonMergeable.end()){
                        this->removeCntvEntry(cntv->val, label);
                    }    
                }
            }    

        }    
    }
            

        
    void
    CntvInfo::sortCaseItems(SyList<SyCaseItem*>& caseItems, int size) const
    {
        map<long, SyCaseItem*> label2Items;
        for(SyNode<SyCaseItem*>* node = caseItems.head; node; node = node->next)
        {
            const SyConst *label = node->val->getLabel()->evaluate();
            label2Items.insert(make_pair(label->toLong(), node->val));
            delete label;
        }
        caseItems.clear();

        for(int i = 0; i < size; ++i)
        {
            map<long, SyCaseItem*>::const_iterator iter = label2Items.find(i);
            if(iter == label2Items.end()){
                continue;
            }    
            caseItems.push_back(iter->second);
        }    
    }

    void
    CntvInfo::populateOutCntvInfo()
    {
        CntvAssignInfoMap::const_iterator iter = cntvAssignInfoMap.begin();
        CntvAssignInfoMap::const_iterator iterEnd = cntvAssignInfoMap.end();
        for(; iter != iterEnd; ++iter)
        {
            OutCntvInfo info;
            const SyCntv *tmp = iter->first;
            info.cntv = const_cast<SyCntv*>(tmp);
            info.type = this->isMux(iter->first) ? MUX: COMB;
            pair<int, int> pse = this->getStartEnd(iter->first);
            info.start = pse.first;
            info.end = pse.second;
            outCntvInfoSet.insert(info);
        }    
            
    }

    CntvInfo::OutCntvInfo
    CntvInfo::getOutCntvInfo(const SyCntv* cntv) const 
    {
        OutCntvInfo info;
        info.cntv = const_cast<SyCntv*>(cntv);
        OutCntvInfoSet::const_iterator iter = outCntvInfoSet.find(info);
        assert(iter != outCntvInfoSet.end());
        return *iter;
    }    

    bool
    CntvInfo::entryExists(const SyCntv *cntv, int label) const
    {
        bool exists = true;
        CntvAssignInfoMap::const_iterator iter = 
            cntvAssignInfoMap.find(const_cast<SyCntv*>(cntv));
        if(iter == cntvAssignInfoMap.end()) {
            return false;
        }    
        if(label == -1)
        {

            for(unsigned int i = 0; exists && (i < iter->second.size()); ++i) 
            {
                exists = ((iter->second)[i] != NULL) && 
                    !(iter->second[i])->empty();
                if(!exists)
                {
                    //try searching in inner case statement
                    InnerCaseCntvInfo::const_iterator iiter = innerCaseCntvInfo.find(i);
                    if(iiter != innerCaseCntvInfo.end())
                    { 
                        vector<CntvInfo*> icntvInfo = iiter->second;
                        for(unsigned int j = 0; j < icntvInfo.size(); ++j) 
                        {
                            //Note search in all breanches
                            if(exists = icntvInfo[j]->entryExists(cntv)) {
                                break;
                            }
                        }
                    }
                }    
            }
        }    
        else
        {
            exists = ((iter->second)[label] != NULL) &&
                     !(iter->second)[label]->empty();
            if(!exists)
            {
                //try searching in inner case statement
                InnerCaseCntvInfo::const_iterator iiter = innerCaseCntvInfo.find(label);
                if(iiter != innerCaseCntvInfo.end())
                { 
                    vector<CntvInfo*> icntvInfo = iiter->second;
                    for(unsigned int j = 0; j < icntvInfo.size(); ++j) 
                    {
                        //Note search in all breanches
                        if(exists = icntvInfo[j]->entryExists(cntv)) {
                            break;
                        }
                    }
                }
            }    
        }
        return exists;
    }    

    bool
    CntvInfo::isMux(const SyCntv* cntv) const
    {
        pair<int, int> startEnd = this->getStartEnd(cntv);
        if(startEnd.first == -1 || startEnd.second == -1) {
            return false;
        }    

        if(startEnd.first == startEnd.second) {
            //We don't want one input mux
            return false;
        }    

        for(int i = startEnd.first; i < startEnd.second; ++i)
        {
            if(!this->entryExists(cntv, i)) {
                return false;
            }
        }
        return true;
    }

    pair<int, int>
    CntvInfo::getStartEnd(const SyCntv *cntv) const
    {
        int start = -1;
        int end = -1;
        CntvAssignInfoMap::const_iterator iter = 
            cntvAssignInfoMap.find(const_cast<SyCntv*>(cntv));
        if(iter == cntvAssignInfoMap.end()) {
            return make_pair(start, end);
        }

        int itemCount = pow(2, this->getSelectSize());
        for(int i = 0; i < itemCount; ++i)
        {
            if(!this->entryExists(cntv, i)) {
                continue;
            }
            if(start == -1) {
                start = i;
            }
            end = i;
        }
        return make_pair(start, end);
    }    

    void
    CntvInfo::print() const
    {
        if(outCntvInfoSet.empty()) {
            return;
        }

        OutCntvInfoSet::const_iterator iter = outCntvInfoSet.begin();
        OutCntvInfoSet::const_iterator iterEnd = outCntvInfoSet.end();
        
        cout << "CASE VAR TYPE\n";
        cout << "--------------------------------------------\n";
        cout << "Cntv            Type        Start        End\n";
        cout << "--------------------------------------------\n";
        for(; iter != iterEnd; ++iter)
        {
            ostringstream str;
            string cntv = iter->cntv->toString();
            str << cntv;
            int size = 16 - cntv.size();
            for(int i = 0; i < size; ++i) {
                str << " ";
            }
            string type("MUX");
            if(iter->type == COMB){
                type = "COMB";
            }
            str << type;
            size = 12 - type.size();
            for(int i = 0; i < size; ++i) {
                str << " ";
            }
            str << iter->start;
            size = (iter->start == -1) ? 10 : (iter->start< 10) ?
                11 : 10;
            for(int i = 0; i < size; ++i) {
                str << " ";
            }
            str << iter->end;
            size = (iter->end == -1) ? 10 : (iter->end < 10) ?
                11 : 10;
            for(int i = 0; i < size; ++i) {
                str << " ";
            }
            cout << str.str();
            cout <<"\n";
        }

    }
            

                
        
}
