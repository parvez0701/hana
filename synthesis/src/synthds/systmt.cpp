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

#include "systmt.h"
#include "sycntvs.h"
#include "syconst.h"
#include "syvisitor.h"
#include "tvisitor.h"
#include "sycontainer.h"

using std::vector;
using std::pair;

namespace Synth
{
    void
        SyAssign::setRExpr(const SyExpr *expr)
        {
            //rhs = expr ? expr->copy() : 0;
            rhs = const_cast<SyExpr*>(expr);
        }


    void
        SyAssign::repSubExpr(
                const std::string& subexpr,
                const SyExpr *newexpr)
        {
            if(lhs->toString() == subexpr) 
            {
                if(!dynamic_cast<SyCntv*>(lhs) ||
                        dynamic_cast<SyConst*>(lhs)){
                    delete lhs;
                }
                lhs = newexpr->copy();
            }
            else {
                lhs->repSubExpr(subexpr, newexpr);
            }

            if(rhs->toString() == subexpr) 
            {
                if(!dynamic_cast<SyCntv*>(rhs) ||
                        dynamic_cast<SyConst*>(rhs)){
                    delete rhs;
                }
                rhs = newexpr->copy();
            }
            else {
                rhs->repSubExpr(subexpr, newexpr);
            }
        }

    void
        SyIf::repSubExpr(
                const std::string& subexpr,
                const SyExpr *newexpr)
        {
            if(cond->toString() == subexpr) 
            {
                if(!dynamic_cast<SyCntv*>(cond) ||
                        dynamic_cast<SyConst*>(cond)){
                    delete cond;
                }
                cond = newexpr->copy();
            }
            else {
                cond->repSubExpr(subexpr, newexpr);
            }

            for(SyNode<SyStmt*>* node = thenPart.head; node; 
                    node = node->next){
                node->val->repSubExpr(subexpr, newexpr);
            }

            for(SyNode<SyStmt*>* node = elsePart.head; node; 
                    node = node->next){
                node->val->repSubExpr(subexpr, newexpr);
            }
        }

    void
        SyEventStmt::repSubExpr(
                const std::string& subexpr,
                const SyExpr *newexpr)
        {
            eventList.repSubExpr(subexpr, newexpr);

            for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next){
                node->val->repSubExpr(subexpr, newexpr);
            }
        }

    void
        SyCase::repSubExpr(
                const std::string& subexpr,
                const SyExpr *newexpr)
        {
            if(select->toString() == subexpr) 
            {
                if(!dynamic_cast<SyCntv*>(select) ||
                    dynamic_cast<SyConst*>(select)){
                    delete select;
                }
                select = newexpr->copy();
            }
            else {
                select->repSubExpr(subexpr, newexpr);
            }

            for(SyNode<SyCaseItem*>* node = caseItems.head; node; 
                    node = node->next)
            {
                SyList<SyStmt*>& stmts = const_cast<SyList<SyStmt*>&>(
                        node->val->getStmts());

                for(SyNode<SyStmt*>* stmt = stmts.head; stmt; 
                        stmt = stmt->next){
                    stmt->val->repSubExpr(subexpr, newexpr);
                }
            }
        }

    SyTaskOrFuncCall::SyTaskOrFuncCall(const SyTaskOrFunc *tf) : 
        SyStmt(), name(tf->getName()),
        taskOrFunc(const_cast<SyTaskOrFunc*>(tf)){
    }

    SyTaskOrFuncCall::SyTaskOrFuncCall(const std::string& n) : 
        SyStmt() ,
        name(n), taskOrFunc(0) {
        //used when function definition is not yet available
    }    

    void
        SyTaskOrFuncCall::repSubExpr(
                const std::string& subexpr,
                const SyExpr *newexpr)
        {
            for(unsigned int i = 0; i < args.size(); ++i){
                if(args[i]->toString() == subexpr) 
                {
                    if(!dynamic_cast<SyCntv*>(args[i]) ||
                        dynamic_cast<SyConst*>(args[i])){
                        delete args[i];
                    }
                    args[i] = newexpr->copy();
                }
                else {
                    args[i]->repSubExpr(subexpr, newexpr);
                }
            }
        }

    SyEventCtrlList::SyEventCtrlList(const SyEventCtrlList& rhs)
    {
        for(unsigned int i = 0; i < rhs.posExprs.size(); ++i){
            posExprs.push_back(rhs.posExprs[i]->copy());
        }
        for(unsigned int i = 0; i < rhs.negExprs.size(); ++i){
            negExprs.push_back(rhs.negExprs[i]->copy());
        }
        for(unsigned int i = 0; i < rhs.exprs.size(); ++i){
            exprs.push_back(rhs.exprs[i]->copy());
        }
    }

    SyEventCtrlList&
        SyEventCtrlList::operator=(const SyEventCtrlList& rhs)
    {
        for(unsigned int i = 0; i < rhs.posExprs.size(); ++i){
            posExprs.push_back(rhs.posExprs[i]->copy());
        }
        for(unsigned int i = 0; i < rhs.negExprs.size(); ++i){
            negExprs.push_back(rhs.negExprs[i]->copy());
        }
        for(unsigned int i = 0; i < rhs.exprs.size(); ++i){
            exprs.push_back(rhs.exprs[i]->copy());
        }

        return *this;
    }


    void
        SyEventCtrlList::repSubExpr(
                const std::string& subexpr,
                const SyExpr *newexpr)
        {
            for(unsigned int i = 0; i < posExprs.size(); ++i){
                if(posExprs[i]->toString() == subexpr) 
                {
                    if(!dynamic_cast<SyCntv*>(posExprs[i]) ||
                        dynamic_cast<SyConst*>(posExprs[i])){
                        delete posExprs[i];
                    }
                    posExprs[i] = newexpr->copy();
                }
                else {
                    posExprs[i]->repSubExpr(subexpr, newexpr);
                }
            }
                
            for(unsigned int i = 0; i < negExprs.size(); ++i){
                if(negExprs[i]->toString() == subexpr) 
                {
                    if(!dynamic_cast<SyCntv*>(negExprs[i]) ||
                        dynamic_cast<SyConst*>(negExprs[i])){
                        delete negExprs[i];
                    }
                    negExprs[i] = newexpr->copy();
                }
                else {
                    negExprs[i]->repSubExpr(subexpr, newexpr);
                }
            }
            
            for(unsigned int i = 0; i < exprs.size(); ++i){
                if(exprs[i]->toString() == subexpr) 
                {
                    if(!dynamic_cast<SyCntv*>(exprs[i]) ||
                        dynamic_cast<SyConst*>(exprs[i])){
                        delete exprs[i];
                    }
                    exprs[i] = newexpr->copy();
                }
                else {
                    exprs[i]->repSubExpr(subexpr, newexpr);
                }
            }
        }

    void
        SyCompAssign::repSubExpr(
                const std::string& subexpr,
                const SyExpr *newexpr)
        {
            if(lexpr->toString() == subexpr)
            {
                if(!dynamic_cast<SyCntv*>(lexpr) ||
                    dynamic_cast<SyConst*>(lexpr)){
                    delete lexpr;
                }
                lexpr = newexpr->copy();
            }
            else {
                lexpr->repSubExpr(subexpr, newexpr);
            }

            for(SyNode<pair<SyEventCtrlList, SyExpr*> >* node = eventExprs.head;
                    node; node = node->next)
            {
                node->val.first.repSubExpr(subexpr, newexpr);
                if(node->val.second->toString() == subexpr){
                    if(!dynamic_cast<SyCntv*>(node->val.second) ||
                        dynamic_cast<SyConst*>(node->val.second))
                    {
                        delete node->val.second;
                        node->val.second = newexpr->copy();
                    }
                }
                else {
                    node->val.second->repSubExpr(subexpr, newexpr);
                }
            }
        }

    SyBlockCompAssign*
        SyBlockCompAssign::copy() const 
        {
            SyBlockCompAssign *bcassign = new SyBlockCompAssign(
                    this->getLExpr()->copy());
            const SyList<pair<SyEventCtrlList, SyExpr*> > & eventExprs = 
                this->getEventExprs();
            for(SyNode<pair<SyEventCtrlList, SyExpr*> >* node = eventExprs.head;
                    node; node = node->next) {
                bcassign->addEventCtrlExpr(SyEventCtrlList(node->val.first),
                        node->val.second->copy());
            }
            return bcassign;
        }

    SyNonBlockCompAssign*
        SyNonBlockCompAssign::copy() const 
        {
            SyNonBlockCompAssign *nbcassign = new SyNonBlockCompAssign(
                    this->getLExpr()->copy());
            const SyList<pair<SyEventCtrlList, SyExpr*> > & eventExprs = 
                this->getEventExprs();
            for(SyNode<pair<SyEventCtrlList, SyExpr*> >* node = eventExprs.head;
                    node; node = node->next) {
                nbcassign->addEventCtrlExpr(SyEventCtrlList(node->val.first),
                        node->val.second->copy());
            }
            return nbcassign;
        }


    SyContAssign*
        SyContAssign::copy() const{
            return new SyContAssign(
                    this->getLExpr()->copy(),
                    this->getRExpr()->copy());
        }

    SyNonBlockAssign*
        SyNonBlockAssign::copy() const{
            return new SyNonBlockAssign(
                    this->getLExpr()->copy(),
                    this->getRExpr()->copy());
        }

    SyBlockAssign*
        SyBlockAssign::copy() const{
            return new SyBlockAssign(
                    this->getLExpr()->copy(),
                    this->getRExpr()->copy());
        }

    SyIf*
        SyIf::copy() const
        {
            SyIf *if_ = new SyIf(this->getCond()->copy());

            for(SyNode<SyStmt*>* node = thenPart.head; node; 
                    node = node->next){
                if_->addThenStmt(node->val->copy());
            }
            for(SyNode<SyStmt*>* node = elsePart.head; node; 
                    node = node->next){
                if_->addElseStmt(node->val->copy());
            }
            return if_;
        }

    SyAlways*
        SyAlways::copy() const
        {
            return dynamic_cast<SyAlways*>(Copy(this));
        }

    SyEventCtrl*
        SyEventCtrl::copy() const
        {
            return dynamic_cast<SyEventCtrl*>(Copy(this));
        }

    SyCaseItem*
        SyCaseItem::copy() const
        {
            SyCaseItem* caseitem = new SyCaseItem(label->copy());
            for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next){
                caseitem->addStmt(node->val->copy());
            }

            return caseitem;
        }

    SyCase*
        SyCase::copy() const
        {
            SyCase *case_ = new SyCase(select->copy());
            for(SyNode<SyCaseItem*>* node = caseItems.head; 
                    node; node = node->next){
                case_->addItem(node->val->copy());
            }
            return case_;
        }
    
    SyTaskCall::SyTaskCall(const SyTask* t):
                SyTaskOrFuncCall(t)
    {
    }

    SyTaskCall::SyTaskCall(const string& name):
                SyTaskOrFuncCall(name)
    {
    }
    SyTaskCall::~SyTaskCall() {
    }

    const SyTask*
    SyTaskCall::getTask() const {
        return dynamic_cast<const SyTask*>(
            this->getTaskOrFunc());
    }

    void 
    SyTaskCall::setTask(const SyTask* task) {
        this->setTaskOrFunc(task);
    }


    SyTaskOrFuncCall*
    SyTaskOrFuncCall::copy() const
    {
        SyTaskOrFuncCall *tfCall = new SyTaskOrFuncCall(taskOrFunc);
        for(unsigned int i = 0; i < args.size(); ++i) {
            tfCall->addArg(args[i]->copy());
        }    
        return tfCall;
    }
    SyTaskCall*
        SyTaskCall::copy() const
        {
            SyTaskCall *newCall = this->getTask() ? 
                new SyTaskCall(this->getTask()) :
                new SyTaskCall(this->getName());
            const vector<SyExpr*>& args = this->getArgs();
            for(unsigned int i = 0; i < args.size(); ++i) {
                newCall->addArg(args[i]->copy());
            }
            return newCall;
        }




    SyEventStmt* 
        Copy(const SyEventStmt* stmt)
        {
            SyEventStmt *retstmt = 0;
            if(dynamic_cast<const SyAlways*>(stmt)){
                retstmt = new SyAlways();
            }
            else {
                retstmt = new SyEventCtrl();
            }

            const vector<SyExpr*>& posExprs = stmt->getPosEdgeExprs();
            const vector<SyExpr*>& negExprs = stmt->getNegEdgeExprs();
            const vector<SyExpr*>& exprs = stmt->getExprs();
            const SyList<SyStmt*>& stmts = stmt->getStmts();

            for(unsigned int i = 0; i < posExprs.size(); ++i){
                retstmt->addPosEdgeExpr(posExprs[i]->copy());
            }
            for(unsigned int i = 0; i < negExprs.size(); ++i){
                retstmt->addNegEdgeExpr(negExprs[i]->copy());
            }
            for(unsigned int i = 0; i < exprs.size(); ++i){
                retstmt->addExpr(exprs[i]->copy());
            }

            for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next){
                retstmt->addStmt(node->val->copy());
            }

            return retstmt;
        }

    void
        SyContAssign::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    void
        SyContAssign::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyNonBlockAssign::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    void
        SyNonBlockAssign::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyBlockAssign::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyBlockAssign::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyIf::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    void
        SyIf::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyAlways::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyAlways::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyEventCtrl::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyEventStmt::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyCase::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyCase::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyTaskCall::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyTaskCall::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyBlockCompAssign::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyNonBlockCompAssign::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    SyList<SyCntv*>*
    SyEventStmt::getReadVars() const
    {
        //This should be non-member function perhaps
        SyList<SyCntv*> *retList = new SyList<SyCntv*>();

        for(SyNode<SyStmt*>* stmt = stmts.head; stmt; stmt = stmt->next)
        {
            SyList<SyCntv*> *tlist = this->getReadVars(stmt->val);
            retList->merge(*tlist);
            delete tlist;
        }    
        return retList;
    }

    SyList<SyCntv*>*
    SyEventStmt::getReadVars(const SyStmt* stmt) const
    {
        if(const SyAssign *assign = dynamic_cast<const SyAssign*>(stmt)){
            return this->getReadVars(assign->getRExpr());
        }
        SyList<SyCntv*> *retList = new SyList<SyCntv*>();
        SyList<SyStmt*> tstmts;
        if(const SyIf *if_ = dynamic_cast<const SyIf*>(stmt))
        {
            SyList<SyCntv*> *cond = this->getReadVars(if_->getCond());
            retList->merge(*cond);
            delete cond;
            tstmts.merge(if_->getThenPart());
            tstmts.merge(if_->getElsePart());
        }
        else if (const SyCase *case_ = dynamic_cast<const SyCase*>(stmt))
        {
            SyList<SyCntv*> *select = this->getReadVars(case_->getSelect());
            retList->merge(*select);
            delete select;

            const SyList<SyCaseItem*>& items = case_->getCaseItems();
            for(SyNode<SyCaseItem*>* item = items.head; item; item = item->next) {
                tstmts.merge(item->val->getStmts());
            }
        }    
        else if(const SyTaskOrFuncCall *call = dynamic_cast<const SyTaskOrFuncCall*>(
            stmt))
        {
            SyList<SyCntv*> *tlist = this->getReadVars(call);
            if(tlist)
            {
                retList->merge(*tlist);
                delete tlist;
            }    
                
        }

        for(SyNode<SyStmt*>* stmt = tstmts.head; stmt; stmt = stmt->next)
        {
            SyList<SyCntv*>* tlist = this->getReadVars(stmt->val);
            retList->merge(*tlist);
            delete tlist;
        }
        return retList;
    }    

    SyList<SyCntv*>*
    SyEventStmt::getReadVars(const SyExpr* expr) const
    {
        SyList<SyCntv*>* retList = new SyList<SyCntv*>();
        SyList<const SyExpr*> exprs;

        if(/*const SyConst *const_ = */dynamic_cast<const SyConst*>(expr)) {
            return retList;
        }

        if(/*const SyXOrZ *xOrZ = */dynamic_cast<const SyXOrZ*>(expr)) {
            return retList;
        }
        if(const SyPartSel *partSel = dynamic_cast<const SyPartSel*>(expr))
        {
            SyConcat *conc = partSel->getConcat();
            const SyList<SyExpr*>& bits = conc->getExprs();
            for(SyNode<SyExpr*>* bit = bits.head; bit; bit = bit->next) {
                exprs.push_back(bit->val);
            }    
            delete conc;
        }
        else if(const SyBitSel *bitSel = dynamic_cast<const SyBitSel*>(expr)) {
            retList->push_back(const_cast<SyBitSel*>(bitSel));
        }
        else if(const SyConcat *conc = dynamic_cast<const SyConcat*>(expr)) 
        {
            const SyList<SyExpr*>& bits = conc->getExprs();
            for(SyNode<SyExpr*>* bit = bits.head; bit; bit = bit->next) {
                exprs.push_back(bit->val);
            }    
        }
        else if(const SyVCntv *vcntv = dynamic_cast<const SyVCntv*>(expr)) 
        {
            const SyList<SyScCntv*>& bits = vcntv->getBits();
            for(SyNode<SyScCntv*>* bit = bits.head; bit; bit = bit->next) {
                retList->push_back(bit->val);
            }
        }
        else if(const SyCntv* cntv = dynamic_cast<const SyCntv*>(expr)) {
            retList->push_back(const_cast<SyCntv*>(cntv));
        }    
            

        else if(const SyUnary *unary = dynamic_cast<const SyUnary*>(expr)) {
            exprs.push_back(unary->getExpr());
        }    
        else if(const SyBinary *binary = dynamic_cast<const SyBinary*>(expr))
        {
            exprs.push_back(binary->getLExpr());
            exprs.push_back(binary->getRExpr());
        }    
        else if(const SyCondExpr *condExpr = dynamic_cast<const SyCondExpr*>(expr))
        {
            exprs.push_back(condExpr->getCond());
            exprs.push_back(condExpr->getTrueExpr());
            exprs.push_back(condExpr->getFalseExpr());
        }    
        else if(const SyTaskOrFuncCall *call = dynamic_cast<const SyTaskOrFuncCall*>(
            expr))
        {
            SyList<SyCntv*> *tlist = this->getReadVars(call);
            retList->merge(*tlist);
            delete tlist;
        }

        for(SyNode<const SyExpr*>* node = exprs.head; node; node = node->next)
        {
            SyList<SyCntv*> *tlist = this->getReadVars(node->val);
            retList->merge(*tlist);
            delete tlist;
        }    
        return retList;

    }
    SyList<SyCntv*>*
    SyEventStmt::getReadVars(const SyTaskOrFuncCall *call) const
    {
        const vector<SyExpr*>& args = call->getArgs();
        const SyTaskOrFunc *tf = call->getTaskOrFunc();
        if(!tf) {
            return 0;
        }    
        SyList<SyCntv*>* ports = tf->getMixPorts();
        unsigned int i = 0;
        SyList<SyExpr*> exprs;
        for(SyNode<SyCntv*>* node = ports->head; 
            node && (i < args.size()); node = node->next, ++i)
        {
            if(const SyScPort *port = dynamic_cast<const SyScPort*>(
                node->val)) {
                if(port->getDir() == SyScPort::OUT) {
                    continue;
                }
            }    
            else if(const SyVPort *vport = dynamic_cast<const SyVPort*>(
                node->val)) {
                if(vport->getDir() == SyScPort::OUT) {
                    continue;
                }
            }    
            exprs.push_back(args[i]);
        }    
        SyList<SyCntv*>* retList = new SyList<SyCntv*>();

        for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next)
        {
            SyList<SyCntv*>* tlist = this->getReadVars(node->val);
            retList->merge(*tlist);
            delete tlist;
        }    
        return retList;
    }
}
