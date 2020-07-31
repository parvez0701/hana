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
#include <iostream>
#include <algorithm>

#include "syprocmgr.h"

#include "systmt.h"
#include "sycontainer.h"
#include "syconst.h"
#include "symsghandler.h"

using std::list;
using std::set;
using std::map;
using std::pair;
using std::make_pair;
using std::string;

namespace Synth
{
    SyProcMgr* SyProcMgr::procMgr = 0;
    SyProcMgr::SyProcMgr() {
    }

    SyProcMgr::~SyProcMgr() {
    }

    SyProcMgr*
    SyProcMgr::instance()
    {
        if(!procMgr) {
            procMgr = new SyProcMgr();
        }
        return procMgr;
    }

    const AlwaysInfo_*
    SyProcMgr::getAlwaysInfo(const SyAlways *always) const
    {
        AlwaysTable::const_iterator iter = alwaysTable.find(always);
        if(iter == alwaysTable.end()) {
            return 0;
        }
        return iter->second;
    }

    bool
    SyProcMgr::check(const SyAlways* always, const SyModule *module)
    {
        AlwaysInfo_ *info = new AlwaysInfo_(always, module);
        if(!info->getStatus()) 
        {
            delete info;
            return false;
        }
        alwaysTable.insert(make_pair(always, info));
        return true;
    }
    
    
    AlwaysInfo_::AlwaysInfo_(const SyAlways *a, const SyModule *m) :
        always(const_cast<SyAlways*>(a)), 
        module(const_cast<SyModule*>(m)), status(true),
        timestamp(0)
    {
        const vector<SyExpr*>& posExprs = always->getPosEdgeExprs();
        for(unsigned int i = 0; i < posExprs.size(); ++i) {
            sensvars.push_back(posExprs[i]->toString());
        }    
        
        const vector<SyExpr*>& negExprs = always->getNegEdgeExprs();
        for(unsigned int i = 0; i < negExprs.size(); ++i) {
            sensvars.push_back(negExprs[i]->toString());
        }    
        this->findSeqVars();
        if(status) {
            this->checkConsistencyAndSetClock();
        }
        if(status && !isComb()) {
            this->setSetReset();
        }    
        if(status && !isComb()) {
            this->populateUESTable();
        }    
        //this->print();
  
    }    

    AlwaysInfo_::~AlwaysInfo_() {
    }

    const string&
    AlwaysInfo_::getClock() const {
        return clock;
    }    

    bool
    AlwaysInfo_::getStatus() const {
        return status;
    }    

    // A variable which is to be synthesized as flip flop can be
    // inside set condition, reset condition, other condition or
    // final else statement. This can happen as any combination of
    // of 4 overmentioned statements

    bool
    AlwaysInfo_::isComb() const {
        return clock.empty();
    }    

    CellType
    AlwaysInfo_::getCellType(const string& var) const
    {
        if(isComb()) {
            //presently we are not handling combinational always
            //block
            return UNSET;
        }
        CellType type = FF;
        if(isSet(var) && isReset(var)) {
            type = (getSetTimeStamp(var) > getResetTimeStamp(var)) ? FFRS :
                FFSR;
        }        
        else if(isSet(var)) {
            type = FFS;
        }
        else if(isReset(var)) {
            type = FFR;
        }
        return type;
    }

    bool
    AlwaysInfo_::checkAndAddSet(const string& var, const string& cond)
    {
        if(!isSensVar(cond)) {
            return false;
        }    
        //If entry exists then it removes the entry and does nothing
        //further.
        SetResetInfo::const_iterator iter = setInfo.find(var);
        if(iter != setInfo.end())
        {
            setInfo.erase(var);
            SyMsgHandler::instance()->print(33, module->getName().c_str(),
                var.c_str());
            status = false;    
            return false;
        }
        setInfo.insert(make_pair(var, make_pair(cond, timestamp++)));
        return true;
    }
    bool
    AlwaysInfo_::checkAndAddReset(const string& var, const string& cond)
    {
        if(!isSensVar(cond)) {
            return false;
        }    
        //If entry exists then it removes the entry and does nothing
        //further.
        SetResetInfo::const_iterator iter = resetInfo.find(var);
        if(iter != resetInfo.end())
        {
            resetInfo.erase(var);
            SyMsgHandler::instance()->print(33, module->getName().c_str(),
                var.c_str());
            status = false;    
            return false;
        }
        resetInfo.insert(make_pair(var, make_pair(cond, timestamp++)));
        return true;
    }

    bool
    AlwaysInfo_::isSensVar(const string& var) const 
    {
        list<string>::const_iterator iter = std::find(sensvars.begin(),
            sensvars.end(), var);
        if(iter == sensvars.end()) {
            return false;
        }
        return true;
    }

    bool 
    AlwaysInfo_::isNegEdge(const string& sensVar) const
    {
        if(sensVar.empty()) {
            return false; 
        }    

        const vector<SyExpr*>& negExprs = always->getNegEdgeExprs();
        for(unsigned int i = 0; i < negExprs.size(); ++i) {
            if(negExprs[i]->toString() == sensVar) {
                return true;
            }
        }
        return false;
    }

    bool
    AlwaysInfo_::isNegEdgeClock() const {
        return this->isNegEdge(clock);
    }

    bool
    AlwaysInfo_::isNegEdgeReset(const string& var) const {
        return isSet(var) && isNegEdge(var);
    }    

    bool
    AlwaysInfo_::isNegEdgeSet(const string& var) const {
        return isReset(var) && isNegEdge(var);
    }    

    bool
    AlwaysInfo_::isSet(const string& var) const {
        return setInfo.find(var) != setInfo.end();
    }    

    bool
    AlwaysInfo_::isReset(const string& var) const {
        return resetInfo.find(var) != resetInfo.end();
    }    

    string
    AlwaysInfo_::getSetName(const string& var) const
    {
        SetResetInfo::const_iterator iter = setInfo.find(var);
        if(iter == setInfo.end()) {
            return "";
        }
        return iter->second.first;
    }
    
    string
    AlwaysInfo_::getResetName(const string& var) const
    {
        SetResetInfo::const_iterator iter = resetInfo.find(var);
        if(iter == resetInfo.end()) {
            return "";
        }
        return iter->second.first;
    }

    int
    AlwaysInfo_::getSetTimeStamp(const string& var) const
    {
        SetResetInfo::const_iterator iter = setInfo.find(var);
        if(iter == setInfo.end()) {
            return -1;
        }
        return iter->second.second;
    }    

    int
    AlwaysInfo_::getResetTimeStamp(const string& var) const
    {
        SetResetInfo::const_iterator iter = resetInfo.find(var);
        if(iter == resetInfo.end()) {
            return -1;
        }
        return iter->second.second;
    }    
    bool
    AlwaysInfo_::checkConsistencyAndSetClock()
    {
        if(sensvars.empty()) {
            //nothing to be done for combinational block
            return true;
        }
        if(sensvars.size() == 1) 
        {
            //only one signal in sensitivity list
            clock = sensvars.front();
            return true;
        }    
        const SyList<SyStmt*>& stmts = always->getStmts();
        for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next)
        {
            SyIf *if_ = dynamic_cast<SyIf*>(node->val);
            if(!if_) {
                continue;
            }    
            if(!checkIfConditionConsistency(if_)) {
                return status = false;
            }    
        }

        if(condvars.size() == sensvars.size())
        {
            //all sensitivity list signals are used in if conditions
            // I don't know which one is clock
            SyMsgHandler::instance()->print(29, module->getName().c_str());
            status = false;
            return false;
        }    
        if(condvars.size() != (sensvars.size()-1))
        {
            SyMsgHandler::instance()->print(32, module->getName().c_str());
            status = false;
            return false;
        }
        //Now set the clock
        for(list<string>::const_iterator iter = sensvars.begin();
            iter != sensvars.end(); ++iter) {
            if(condvars.find(*iter) == condvars.end()) 
            {
                clock = *iter;
                break;
            }
        }
        assert(!clock.empty());
        return true;
    }

    bool
    AlwaysInfo_::checkIfConditionConsistency(const SyIf *if_)
    {
        //check if conditions, the condition has to be simple
        //signal or unary expression with polarity matching with
        //the polarity specified in the sensitivity list
        //set<string, cmp_str> donevars;
        //bool done = false;
        while(if_)
        {
            const SyExpr *cond = if_->getCond();
            for(list<string>::const_iterator iter = sensvars.begin();
                iter != sensvars.end(); ++iter)
            {
                if(!cond->contains(*iter)) {
                    continue;
                }    
                if(!checkIfConditionConsistency(cond, *iter)) 
                {
                    status = false;
                    if_ = 0;
                    //done = true;
                    break;
                }    
                //donevars.insert(*iter);
                condvars.insert(*iter);
            }
            if(!if_) {
                break;
            }
            const SyList<SyStmt*>& stmts = if_->getElsePart();
            if(stmts.empty() || (stmts.size() > 1)) {
                if_ = 0;
            }
            else {
                //indeed a top level if
                if_ = dynamic_cast<SyIf*>(stmts.head->val);
            }    
        }
        return status;
    }

    bool
    AlwaysInfo_::checkIfConditionConsistency(const SyExpr *cond, 
        const string& var)
    {
        if(const SyUnary *unary = dynamic_cast<const SyUnary*>(cond)) {
            return this->checkUnaryConsistency(unary, var);
        }
        else if(const SyBinary *binary = dynamic_cast<const SyBinary*>(cond)) {
            return this->checkBinaryConsistency(binary, var);
        }
        else if(this->isNegEdge(var)) 
        {
            SyMsgHandler::instance()->print(27, module->getName().c_str(),
                var.c_str());
            return false;    
        }
        else if((cond->toString() != var)) {
            return false;
        }    
        return true;
    }    


    bool
    AlwaysInfo_::checkUnaryConsistency(const SyUnary *unary,
        const string& var)
    {    
        bool isNegEdge = this->isNegEdge(var);
        bool err = false;
        int negation = 0;
        while(unary)
        {
            SyExpr::SyOp op = unary->getOperator();
            if((op == SyExpr::LOGNEG) || (op == SyExpr::BITNEG)) {
                ++negation;
            }
            else if(op != SyExpr::UNPLUS) {
                err = true;
            }    
            unary = dynamic_cast<const SyUnary*>(unary->getExpr());
        }
        if(err || (isNegEdge && !(negation%2)) || 
            (!isNegEdge && (negation%2)))
        {    
            SyMsgHandler::instance()->print(27, module->getName().c_str(),
                var.c_str());
            return false;    
        }    
        return true;
    }    

    bool
    AlwaysInfo_::checkBinaryConsistency(const SyBinary *binary,
        const string& var)
    {
        SyExpr::SyOp op = binary->getOperator();
        if((op != SyExpr::LOGEQ) /*&& (op != SyExpr::LOGENEQ)*/) {
            SyMsgHandler::instance()->print(27, module->getName().c_str(),
                var.c_str());
            return false;
        }
        const SyExpr *lexpr = binary->getLExpr();
        const SyExpr *rexpr = binary->getRExpr();

        const SyExpr *cntv = 0;
        const SyConst *const_ = 0;
        if(!(const_ = dynamic_cast<const SyConst*>(rexpr))) 
        {
            const_ = dynamic_cast<const SyConst*>(lexpr);
            cntv = rexpr;
        }
        else {
            cntv = lexpr;
        }
        
        if(!const_ || (cntv->toString() != var))
        {
            SyMsgHandler::instance()->print(27, module->getName().c_str(),
                var.c_str());
            return false;    
        }    
        bool isNegEdge = this->isNegEdge(var);
        bool err = false;
        if(isNegEdge && !const_->iszero()) {
            err = true;
        }
        else if(!isNegEdge && const_->iszero()) {
            err = true;
        }    
        if(err)
        {
            SyMsgHandler::instance()->print(27, module->getName().c_str(),
                var.c_str());
            return false;    
        }

        return true;
    }


    void
    AlwaysInfo_::findSeqVars()
    {
        const SyList<SyStmt*>& stmts = always->getStmts();
        this->findSeqVars(stmts, module);
    }

    void
    AlwaysInfo_::findSeqVars(const SyList<SyStmt*>& stmts, const SyScope *sc) {
        for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next) {
            this->findSeqVars(node->val, sc);
        }
    }    

    void
    AlwaysInfo_::findSeqVars(const SyStmt *stmt, const SyScope *sc)
    {
        if(const SyBlockAssign *bassign = 
            dynamic_cast<const SyBlockAssign*>(stmt)) {
            this->findSeqVars(bassign, sc);
        }
        else if(const SyNonBlockAssign *nbassign = 
            dynamic_cast<const SyNonBlockAssign*>(stmt)) {
            this->findSeqVars(nbassign, sc);
        }
        else if(const SyIf *if_ = dynamic_cast<const SyIf*>(stmt)) {
            this->findSeqVars(if_, sc);
        }
        else if(const SyCase *case_ = dynamic_cast<const SyCase*>(stmt)) {
            this->findSeqVars(case_, sc);
        }
        else if(const SyTaskCall *tcall = 
            dynamic_cast<const SyTaskCall*>(stmt)) {
            this->findSeqVars(tcall, sc);
        }
        else if(const SyFuncCall *fcall = 
            dynamic_cast<const SyFuncCall*>(stmt)) {
            this->findSeqVars(fcall, sc);
        }
        //else {
        //    return 0;
        //}    
    }

    void
    AlwaysInfo_::findSeqVars(const SyIf *if_, const SyScope *sc)
    {
        this->findSeqVars(if_->getThenPart(), sc);
        this->findSeqVars(if_->getElsePart(), sc);
    }

    void
    AlwaysInfo_::findSeqVars(const SyCase *case_, const SyScope *sc)
    {
        const SyList<SyCaseItem*>& items = case_->getCaseItems();

        for(SyNode<SyCaseItem*>* node = items.head; node; 
            node = node->next){
            this->findSeqVars(node->val->getStmts(), sc);
        }
        
    }

    void
    AlwaysInfo_::findSeqVars(const SyFuncCall *fcall, const SyScope *sc)
    {
        const SyFunc *func = fcall->getFunc();
        this->findSeqVars(func->getStmts(), func);
    }

    void
    AlwaysInfo_::findSeqVars(const SyTaskCall *tcall, const SyScope *sc)
    {
        const SyTask *task = tcall->getTask();

        this->findSeqVars(task->getStmts(), sc);
        //All simple variables connected with output of the task
        if(sc != module) {
            return;
        }    

        const vector<SyExpr*>& args = tcall->getArgs();

        SyList<SyScCntv*> formals;
        const SyList<SyScPort*>& ports = task->getPorts();
        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next) {
            formals.push_back(node->val);
        }

        SyList<pair<SyVCntv*,SyList<SyScCntv*> > > info =
            GetVectorScalarPairs(formals, module->getName(), tcall->getName());
        
        if(info.size() != args.size())
        {
            SyMsgHandler::instance()->print(30, module->getName().c_str(),
                tcall->getName().c_str());
            status = false;
            return;
        }

        SyNode<pair<SyVCntv*,SyList<SyScCntv*> > >* node = info.head;



        for(int i = 0; i < args.size(); ++i, node = node->next)
        {
            SyScPort *scport = 
                dynamic_cast<SyScPort*>(node->val.second.head->val);
            if(scport->getDir() != SyScPort::OUT) {
                continue;
            }    
            SyList<const SyCntv*> flist;
            CreateFlatList<const SyCntv*>(flist, args[i]);
            for(SyNode<const SyCntv*>* n = flist.head; n; n = n->next) {
                vars.insert(n->val->toString());
            }
        }    
    }

    void
    AlwaysInfo_::findSeqVars(const SyNonBlockAssign *nbassign, 
        const SyScope *sc) {
        this->findSeqVars(static_cast<const SyAssign*>(nbassign), sc);
    }

    void
    AlwaysInfo_::findSeqVars(const SyBlockAssign *bassign,
        const SyScope *sc) {
        this->findSeqVars(static_cast<const SyAssign*>(bassign), sc);
    }

    void
    AlwaysInfo_::findSeqVars(const SyAssign *assign, const SyScope *sc)
    {
        const SyExpr *lhs = assign->getLExpr();
        SyList<const SyCntv*> flist;
        CreateFlatList<const SyCntv*>(flist, lhs);
        for(SyNode<const SyCntv*>* node = flist.head; node; 
            node = node->next) 
        {
            if(const SyScPort *p = dynamic_cast<const SyScPort*>(node->val))
            {
                //don't add internal function/task call nodes
                if(module->findObject(p->getIxName()) != p) {
                    continue;
                }
            }
            vars.insert(node->val->toString());
        }    
                  
    }

    void
    AlwaysInfo_::setSetReset()
    {

        set<string, cmp_str>::const_iterator iter = vars.begin();
        for(; iter != vars.end(); ++iter)
        {
            const SyList<SyStmt*>& stmts = always->getStmts();
            for(SyNode<SyStmt*>* s = stmts.head; s; s = s->next)
            {
                SyIf *if_ = dynamic_cast<SyIf*>(s->val);
                if(!if_) {
                    continue;
                }
                while(if_)
                {
                    int level = 1;
                    int flag = 0;
                    this->setSetReset(*iter, if_->getThenPart(), 
                    this->getCondString(if_), flag, level);
                    const SyList<SyStmt*>& elsePart = if_->getElsePart();
                    if_ = 0;
                    if(elsePart.size() == 1) {
                        if_ = dynamic_cast<SyIf*>(elsePart.head->val);
                    }    
                }
            }
        }    
        
    }

    void
    AlwaysInfo_::setSetReset(const string& var, 
        const SyList<SyStmt*>& stmts, const string& cond, 
        int setReset, int level)
    {
        //setReset -> 0; set/reset not done in current sequence of calls
        //setReset -> 1; set
        //setReset -> 2; reset
        //if(isSet(var) && isReset(var)){
            //already done
        //    return;
        //}    
        if(stmts.empty()) {
            //nothing to be done
            return;
        }

        for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next)
        {
            if(SyAssign *assign = dynamic_cast<SyAssign*>(node->val)) {
                this->setSetReset(var, assign, cond, setReset, level);
            }    
            else if(SyIf *if_ = dynamic_cast<SyIf*>(node->val))
            {
                this->setSetReset(var, if_->getThenPart(), "", setReset, level+1);
                this->setSetReset(var, if_->getElsePart(), "", setReset, level+1);
            }    
            else if(SyCase *case_ = dynamic_cast<SyCase*>(node->val))
            {
                const SyList<SyCaseItem*>& items = case_->getCaseItems();

                for(SyNode<SyCaseItem*>* item = items.head; item; 
                    item = item->next){
                    this->setSetReset(var, item->val->getStmts(), "",
                    setReset, level+1);
                }
            }
            else if(SyTaskCall *tcall = dynamic_cast<SyTaskCall*>(node->val)) {
                this->setSetReset(var, tcall, cond, setReset, level);
            }    
        }
    }
    
    void
    AlwaysInfo_::setSetReset(const string& var, 
        const SyAssign *assign, const string& cond, 
        int setReset, int level)
    {
        //first check function call on the right hand side
        setSetReset(var, assign->getRExpr(), cond, setReset, level);
        //if(assign->getLExpr()->toString() != var) {
        //    return;
        //}
        if(!this->contains(assign->getLExpr(), var)) {
            return;
        }    
        if(level != 1)
        {
            //this is inner level assignment, not a set/reset
            //assignment
            if(setReset == 1) {
                setInfo.erase(var);
            }
            else if(setReset == 2) {
                resetInfo.erase(var);
            }
            else if(setReset == 3) 
            {
                setInfo.erase(var);
                resetInfo.erase(var);
            }    
            return;
        }    
        //if(isSet(var) && isReset(var)) {
        //    return;
        //}
        const SyConst *const_ = dynamic_cast<const SyConst*>(
            assign->getRExpr());
        if(!const_) 
        {
            if(level == 1){
                SyMsgHandler::instance()->print(36, module->getName().c_str(),
                var.c_str());
            }    
            return;
        }
        const_ = const_->evaluate();
        if(const_->iszero()) {
          //  if(!isReset(var)) 
            {
                //resetvars.insert(make_pair(var, cond));
                if(checkAndAddReset(var, cond)){
                    if(setReset == 0){
                        setReset = 1;
                    }
                    else if(setReset == 2) {
                        setReset = 3;
                    }    
                }    
            }    
        }    
        else {
            //if(!isSet(var)) 
            {
                //setvars.insert(make_pair(var, cond));
                if(checkAndAddSet(var, cond)) {
                    if(setReset == 0){
                        setReset = 2;
                    }
                    else if(setReset == 1) {
                        setReset = 3;
                    }    
                }    
            }    
        }    
    }    
    void
    AlwaysInfo_::setSetReset(const string& var, 
        const SyTaskCall *tcall,  const string& cond, 
        int setReset, int level)
    {
        SyList<SyStmt*> stmts = this->getElabStmts(tcall);
        setSetReset(var, stmts, cond, setReset, level);
    }

    void
    AlwaysInfo_::setSetReset(const string& var, 
        const SyFuncCall *fcall,  const string& cond, 
        int setReset, int level)
    {
        SyList<SyStmt*> stmts = this->getElabStmts(*fcall);
        setSetReset(var, stmts, cond, setReset, level);
    }
    void
    AlwaysInfo_::setSetReset(const string& var, 
        const SyExpr *expr,  const string& cond, 
        int setReset, int level)
    {
        //mainly for function call
        if(const SyFuncCall *fcall = 
            dynamic_cast<const SyFuncCall*>(expr)) {
            setSetReset(var, fcall, cond, setReset, level);
        }    
        else if(const SyBinary *bin = dynamic_cast<const SyBinary*>(expr))
        {
            setSetReset(var, bin->getLExpr(), cond, setReset, level);
            setSetReset(var, bin->getRExpr(), cond, setReset, level);
        }
        else if(const SyUnary *unary = dynamic_cast<const SyUnary*>(expr)) {
            setSetReset(var, unary->getExpr(), cond, setReset, level);
        }
        else if(const SyCondExpr *cexpr = dynamic_cast<const SyCondExpr*>(expr)){
            //what ??
        }
    }

    string
    AlwaysInfo_::getCondString(const SyIf *if_) const
    {
        const SyExpr *cond = if_->getCond();
        const SyExpr *expr = cond;
        if(const SyUnary *unary = dynamic_cast<const SyUnary*>(cond))
        {
        
            while(unary)
            {
                expr = unary->getExpr();
                unary = dynamic_cast<const SyUnary*>(unary->getExpr());
            }
        }
        else if(const SyBinary *binary = dynamic_cast<const SyBinary*>(cond))
        {
            //already tests done
            if(dynamic_cast<const SyConst*>(binary->getLExpr())) {
                expr = binary->getRExpr();
            }
            else {
                expr = binary->getLExpr();
            }
        }    
        return expr->toString();
    }

    SyList<SyStmt*>
    AlwaysInfo_::getElabStmts(const SyTaskOrFuncCall *tfcall) const
    {
        const SyTaskOrFunc *tf = tfcall->getTaskOrFunc();
        const vector<SyExpr*>& args = tfcall->getArgs();
        const SyList<SyScPort*>& ports = tf->getPorts();

        SyList<SyScCntv*> formals;
        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next) {
            formals.push_back(node->val);
        }

        SyList<pair<SyVCntv*,SyList<SyScCntv*> > > info =
            GetVectorScalarPairs(formals, module->getName(), tf->getName());

        SyList<SyStmt*> stmts;
        const SyList<SyStmt*>& tstmts = tf->getStmts();
        for(SyNode<SyStmt*>* s = tstmts.head; s; s = s->next) {
            stmts.push_back(s->val->copy());
        }    
        SyNode<pair<SyVCntv*,SyList<SyScCntv*> > >*node = info.head;


        for(int i = 0; i < args.size(); ++i, node = node->next)
        {
            SyList<const SyCntv*> flist;
            CreateFlatList<const SyCntv*>(flist, args[i]);
            SyExpr *expr = 0;
            string name;
            flist.reverse();
            node->val.second.reverse();
            SyNode<const SyCntv*>* n1 = flist.head;
            SyNode<SyScCntv*>* n2 = node->val.second.head;
                
            for(; n1 && n2; n1 = n1->next, n2= n2->next) {
                expr = const_cast<SyCntv*>(n1->val);
                name = n2->val->toString();

                for(SyNode<SyStmt*>* s = stmts.head; s; s = s->next) {
                    s->val->repSubExpr(name, expr);
                }
            }    
        }    
        return stmts;
    }

    void
    AlwaysInfo_::populateUESTable()
    {
        if(isComb()) {
            return;
        }    

        set<string, cmp_str>::const_iterator var = vars.begin();
        set<string, cmp_str>::const_iterator varEnd = vars.end();

        for(; var != varEnd; ++var)
        {
            uesTable.insert(make_pair(*var,vector<string>()));
            list<string>::const_iterator sv = sensvars.begin();
            list<string>::const_iterator svEnd = sensvars.end();
            for(; sv != svEnd; ++sv)
            {
                if(*sv == clock) {
                    continue;
                }
                if((this->getSetName(*var) == *sv) || 
                    (this->getResetName(*var) == *sv)) {
                    continue;
                }
                uesTable[*var].push_back(*sv);
            }    
        }
    }

    vector<string>
    AlwaysInfo_::getUnusedSignals(const string& var) const
    {
        UESTable::const_iterator iter = uesTable.find(var);
        if(iter == uesTable.end()) {
            return vector<string>();
        }
        return iter->second;
    }

    bool
    AlwaysInfo_::contains(const SyExpr *expr, const string& var) const
    {
        SyList<const SyCntv*> flist;
        CreateFlatList<const SyCntv*>(flist, expr);
        for(SyNode<const SyCntv*>* n = flist.head; n; n = n->next) {
            if(n->val->toString() == var) {
                return true;
            }
        }
        return false;
    }

    void
    AlwaysInfo_::print() const
    {
        std::cout << "SIGNAL      SET NAME\n";
        std::cout << "-----------------------\n";

        for(SetResetInfo::const_iterator iter = setInfo.begin();
            iter != setInfo.end(); ++iter) {
            std::cout << iter->first << "      " << iter->second.first << "\n";
        }    
        
        std::cout << "SIGNAL      RESSET NAME\n";
        std::cout << "-----------------------\n";
        for(SetResetInfo::const_iterator iter = resetInfo.begin();
            iter != resetInfo.end(); ++iter) {
            std::cout << iter->first << "      " << iter->second.first << "\n";
        }    
    }

}
