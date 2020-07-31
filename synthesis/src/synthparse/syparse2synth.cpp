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

#include "syparse2synth.h"
#include "syparse2synthtrans.h"
#include "symsghandler.h"
#include "syroot.h"
#include "taskfuncrecursionchecker.h"

using std::list;
using std::pair;
using std::map;
using std::make_pair;
namespace Synth
{
    SyParse2Synth *SyParse2Synth::synth2parse = 0;

    SyParse2Synth::SyParse2Synth() {
    }

    SyParse2Synth::~SyParse2Synth() {
        synth2parse = 0;
    }

    SyParse2Synth*
    SyParse2Synth::instance()
    {
        if(!synth2parse){
            synth2parse = new SyParse2Synth();
        }
        return synth2parse;
    }
    void 
        SyParse2Synth::parse2Synth(const Root *rRoot)
        {
            this->synthesizabilityCheck(rRoot);
            SyParse2SynthTrans translator;
            translator.translate(rRoot, synthMods);
        }


    void
        SyParse2Synth::synthesizabilityCheck(const Root *rRoot)
        {
            SyMsgHandler::instance()->print(1);
            const vector<Module*>& rTopMods = rRoot->getTopModules();
            for(unsigned int i = 0; i < rTopMods.size(); ++i) {
                this->synthesizabilityCheck(rTopMods[i]);
            }
        }

    void
        SyParse2Synth::synthesizabilityCheck(const Module* mod)
        {
            const list<ModInst*>& minsts = mod->getModInsts();
            list<ModInst*>::const_iterator iter = minsts.begin();
            list<ModInst*>::const_iterator eiter = minsts.end();

            for(; iter != eiter; ++iter)
            {
                const ModInst* minst = *iter;
                const Module* master = minst->getMaster();

                if(!master) 
                {
                    SyMsgHandler::instance()->print(21, minst->getLine(), 
                    mod->getName().c_str(), minst->getName().toString().c_str());
                    unsynthMods.insert(mod->getName());
                    continue;
                }
                if(unsynthMods.find(master->getName()) != unsynthMods.end()){
                    continue;
                }
                this->synthesizabilityCheck(master);
            }

            if(unsynthMods.find(mod->getName()) == unsynthMods.end())
            {

                if(!this->isSynthesizable(mod)){
                    unsynthMods.insert(mod->getName());
                }
                else {
                    synthMods.insert(mod->getName());
                }
            }    
        }

    bool
        SyParse2Synth::isSynthesizable(const Module* mod) const
        {
            if(SyRoot::instance()->isDontTouchModule(mod->getName())) {
                return true;
            }

            bool status = true;
            if(!this->checkVarDecls(mod)) {
                return false;
            }
            const vector<Net*>& net = mod->getNets();
            if(!this->checkMultiDimArray(mod, net, 1)) {
                return false;
            }    

            TaskFunctionRecursionChecker tfrc;
            mod->accept(&tfrc);
            if(!tfrc.getStatus()) {
                return false;
            }    

            this->checkGates(mod);
            const vector<Always*>& always = mod->getAlwaysBlocks();

            for(unsigned int i = 0; i < always.size(); ++i)
            {
                if(!this->checkAlways(mod, always[i])){
                    status = false;
                }
              /*  const Stmt* stmt = always[i]->getStmt();
                if(!this->checkStmt(stmt)){
                    status = false;
                }
                */
            }

            const vector<Task*>& tasks = mod->getTasks();
            for(unsigned int i = 0; i < tasks.size(); ++i){
                if(!this->checkTaskOrFunc(tasks[i])){
                    status = false;
                }
            }

            const vector<Func*>& funcs = mod->getFuncs();
            for(unsigned int i = 0; i < funcs.size(); ++i){
                if(!this->checkTaskOrFunc(funcs[i])){
                    status = false;
                }
            }
            return status;

        }

    bool
        SyParse2Synth::checkVarDecls(const Scope* scope) const
        {
            const Module *mod = GetModule(scope);
            //check for the presence of time, real and realtime
            //declarations
            const vector<Time*>& times = scope->getTimes();
            if(!times.empty())
            {
                const Time* time = times[0];
                SyMsgHandler::instance()->print(2, time->getLine(), 
                        mod->getName().c_str(), "time data type");
                return false;
            }
            const vector<Real*>& reals = scope->getReals();
            if(!reals.empty())
            {
                const Real *real = reals[0];
                SyMsgHandler::instance()->print(2, real->getLine(), 
                        mod->getName().c_str(), "real data type");
                return false;
            }

            const vector<RealTime*>& realtimes = scope->getRealTimes();

            if(!realtimes.empty())
            {
                const RealTime* realtime = realtimes[0];
                SyMsgHandler::instance()->print(2, realtime->getLine(), 
                        mod->getName().c_str(), "realtime data type");
                return false;
            }
            if(!this->checkMultiDimArray<Reg>(scope, scope->getRegs(), 2)) {
                return false;
            }    
            if(!this->checkMultiDimArray<Integer>(scope, scope->getIntegers(), 1)) {
                return false;
            }    
            return true;
        }

    template<class T> bool
        SyParse2Synth::checkMultiDimArray(    const Scope *scope,
                                            const vector<T*>& vars, int count) const
        {
            const Module *mod = GetModule(scope);
            string name;

            for(unsigned int i = 0; i < vars.size(); ++i)
            {
                if(!name.empty()) {
                    if(name == vars[i]->getIxName().getName()) {
                        continue;
                    }
                    name.clear();
                }    
                if(!vars[i]->isVecBit()) {
                    continue;
                }    
                const VecCntv *vcntv = vars[i]->getVecCntv();
                if(!vcntv) {
                    if(const Func *func = dynamic_cast<const Func*>(scope)) {
                        if(func->getName() == vars[i]->getIxName().getName()) {
                            continue;
                        }
                    }
                }    
                assert(vcntv);
                const Dimension *dim = vcntv->getDim();

                if((dim->packDim.size() + dim->unpackDim.size()) > 
                    count) 
                {
                    SyMsgHandler::instance()->print(2, vcntv->getLine(),
                        mod->getName().c_str(), "multi-dimensional array");
                    return false;
                }
                if((dim->packDim.size() + dim->unpackDim.size()) == 2) 
                {
                    SyMsgHandler::instance()->print(17,vcntv->getLine(),
                        vcntv->getName().c_str());
                    return false;
                }    
                name = vcntv->getName();
            }    
            return true;
        }

    void
        SyParse2Synth::checkGates(const Module* mod) const
        {
            //Certain gate types are not synthesizable.
            //There presence though does not make a module
            //unsynthesizable. Those gates will simply be ignored.

            const list<GateInst*>& gates = mod->getGateInsts();
            list<GateInst*>::const_iterator gate = gates.begin();
            list<GateInst*>::const_iterator egate = gates.end();

            for(; gate != egate; ++gate)
            {
                GateType type = (*gate)->getType();
                switch(type)
                {
                    case NMOS:
                    case RNMOS:
                    case PMOS:
                    case RPMOS:
                    case CMOS:
                    case RCMOS:
                    case TRAN:
                    case RTRAN:
                    case TRANIF0:
                    case TRANIF1:
                    case RTRANIF0:
                    case RTRANIF1:
                    case PULLUP:
                    case PULLDOWN:
                        SyMsgHandler::instance()->print(3, 
                                mod->getName().c_str(), 
                                GetGateName(type).c_str());
                        break;
                    default:
                        break;
                }
            }
        }

    bool
        SyParse2Synth::checkCase(const Case* case_) const 
        {
            //CaseType type = case_->getType();
            const Module* mod = GetModule(case_);
            /*
            if(type == CASEX) 
            {
                SyMsgHandler::instance()->print(2, case_->getLine(),
                        mod->getName().c_str(), "casex");
                return false;
            }
            else if(type == CASEZ)
            {
                SyMsgHandler::instance()->print(2, case_->getLine(),
                        mod->getName().c_str(), "casez");
                return false;
            }
            */
            bool synthesizable = true;
            const vector<CaseItem*>& caseItems = case_->getCaseItems();
            for(unsigned int i = 0; i < caseItems.size(); ++i) 
            {
                if(!synthesizable) {
                    break;
                }    
                if(!caseItems[i]->isDefault())
                {
                    const vector<Expr*>& label = caseItems[i]->getLabel();
                    for(int j = 0; j < label.size(); ++j) {
                        if(!label[j]->isConstExpr()) 
                        {
                            SyMsgHandler::instance()->print(14, mod->getName().c_str());
                            synthesizable = false;
                            break;
                        }
                    }    
                }    

                if(!this->checkStmt(caseItems[i]->getStmt())){
                    synthesizable = false;
                }
            }

            return synthesizable;
        }

    bool
        SyParse2Synth::checkRepeat(const Repeat* repeat) const
        {
            if(!repeat->getExpr()->isConstExpr())
            {
                const Module *mod = GetModule(repeat);
                SyMsgHandler::instance()->print(5, repeat->getLine(),
                        mod->getName().c_str());
                return false;
            }
            return true;
        }

    bool
        SyParse2Synth::checkFor(const For *for_) const
        {
            const Module *mod = GetModule(for_);
            //check in the for header
            const Expr *init = for_->getInitExpr();
            if(init && !init->isConstExpr())
            {
                SyMsgHandler::instance()->print(5, init->getLine(),
                        mod->getName().c_str());
                return false;
            }

            const Expr* assexpr = for_->getAssignExpr();
            if(assexpr && !this->checkForLoopConst(for_->getLoopVar(), assexpr)) 
            {
                SyMsgHandler::instance()->print(5, assexpr->getLine(),
                        mod->getName().c_str());
                return false;
            }


            const Expr *termexpr = for_->getTermExpr();
            if(termexpr && !this->checkForLoopConst(
                        for_->getLoopVar(), termexpr)) 
            {
                SyMsgHandler::instance()->print(5, termexpr->getLine(),
                        mod->getName().c_str());
                return false;
            }
            //TBD: check inside the block for assignment of loop index
            return true;
        }

    bool
        SyParse2Synth::checkStmt(const Stmt* stmt) const 
        {
            if(!stmt) {
                return true;
            }
            bool synthesizable = true;
            const Module* mod = GetModule(stmt);

            if(const Always *always = dynamic_cast<const Always*>(stmt))
            {
                const Stmt *astmt = always->getStmt();
                return this->checkStmt(astmt);
            }
            else if(const While *while_ = dynamic_cast<const While*>(stmt))
            {
                SyMsgHandler::instance()->print(2, while_->getLine(),
                        mod->getName().c_str(), "while");
                return false;
            }
            else if(const Forever *forever = dynamic_cast<const Forever*>(stmt))
            {
                SyMsgHandler::instance()->print(2, forever->getLine(),
                        mod->getName().c_str(), "forever");
                return false;
            }
            else if(const Repeat *repeat = dynamic_cast<const Repeat*>(stmt))
            {
                if(!this->checkRepeat(repeat)){
                    return false;
                }
                const Stmt* repstmt = repeat->getStmt();
                return this->checkStmt(repstmt);
            }
            else if(const For *for_ = dynamic_cast<const For*>(stmt))
            {
                if(!this->checkFor(for_)){
                    return false;
                }
                const Stmt *forstmt = for_->getStmt();
                return this->checkStmt(forstmt);
            }
            else if(const Block *block = dynamic_cast<const Block*>(stmt))
            {
                const vector<Stmt*>& stmts = block->getStmts();
                for(unsigned int i = 0; i < stmts.size(); ++i){
                    if(!this->checkStmt(stmts[i])){
                        synthesizable = false;
                    }
                }
            }
            else if(const NamedBlock *nblock = dynamic_cast<const NamedBlock*>(
                        stmt))
            {
                if(!this->checkVarDecls(nblock)){
                    return false;
                }
                const vector<Stmt*>& stmts = nblock->getStmts();
                for(unsigned int i = 0; i < stmts.size(); ++i){
                    if(!this->checkStmt(stmts[i])){
                        synthesizable = false;
                    }
                }
            }
            else if(const Case *case_ = dynamic_cast<const Case*>(stmt)){
                return this->checkCase(case_);
            }
            else if(const Fork *fork = dynamic_cast<const Fork*>(stmt))
            {
                SyMsgHandler::instance()->print(2, fork->getLine(),
                        mod->getName().c_str(), "fork");
                return false;
            }
            else if(const Disable *disable = dynamic_cast<const Disable*>(
                        stmt))
            {
                SyMsgHandler::instance()->print(2, disable->getLine(),
                        mod->getName().c_str(), "disable");
                return false;
            }
            else if(const ProcContAssign* proccont = 
                    dynamic_cast<const ProcContAssign*>(stmt))
            {
                SyMsgHandler::instance()->print(2, proccont->getLine(),
                        mod->getName().c_str(), "procedural continuous assign");
                return false;
            }
            else if(const EventStmt *event = dynamic_cast<const EventStmt*>(
                        stmt))
            {
                SyMsgHandler::instance()->print(2, event->getLine(),
                        mod->getName().c_str(), "event");
                return false;
            }

            else if(const Wait *wait = dynamic_cast<const Wait*>(stmt))
            {
                SyMsgHandler::instance()->print(2, wait->getLine(),
                                mod->getName().c_str(), "wait statement");
                return false;
                /*
                const Expr *cond = wait->getCondExpr();
                if(cond && !cond->isConstExpr()){
                    return false;
                }
                return true;
                */
            }
            else if(const EventCntrl *evctrl = dynamic_cast<const EventCntrl*>(
                        stmt))
            {
                //Event statements are synthesizable only as always
                //statement.
                SyMsgHandler::instance()->print(45, evctrl->getLine(),
                        mod->getName().c_str());
                return false;
            }
            else if(const CompAssign *cassign = dynamic_cast<const CompAssign*>(
                        stmt))
            {
                const Expr *repExpr = cassign->getRepeatExpr();
                if(repExpr && !repExpr->isConstExpr()){
                    SyMsgHandler::instance()->print(2, cassign->getLine(),
                        mod->getName().c_str(), 
                        "repeat statement with non-const repeat expr");
                    return false;
                }
            }
            else if(const If *if_ = dynamic_cast<const If*>(stmt))
            {
                if(!this->checkStmt(if_->getStmt())) {
                    return false;
                }    
                return this->checkStmt(if_->getElsePart());
            }

            return synthesizable;
        }

    bool
        SyParse2Synth::checkTaskOrFunc(const TaskOrFunc* tf) const
        {
            if(!this->checkVarDecls(tf)){
                return false;
            }
            const Module *mod = GetModule(tf);

            if(tf->isAuto())
            {
                SyMsgHandler::instance()->print(2, tf->getLine(),
                        mod->getName().c_str(), "automatic task/function");
                return false;
            }

            bool synthesizable = true;
            const Stmt *stmt;
            if(const Task *task = dynamic_cast<const Task*>(tf)){
                stmt = task->getTaskStmt();
            }
            else {
                const Func *func = dynamic_cast<const Func*>(tf);
                stmt = func->getFuncStmt();
            }

            if(const Block* block = dynamic_cast<const Block*>(stmt))
            {
                const vector<Stmt*>& stmts = block->getStmts();
                for(unsigned int i = 0; i < stmts.size(); ++i)
                {
                    if(const EventCntrl *evctrl = 
                            dynamic_cast<const EventCntrl*>(stmts[i]))
                    {
                        SyMsgHandler::instance()->print(2, evctrl->getLine(),
                                mod->getName().c_str(), "event control");
                        return false;
                    }
                    else if(!this->checkStmt(stmts[i])){
                        synthesizable = false;
                    }
                }
            }
            else if(const NamedBlock *nblock = dynamic_cast<const NamedBlock*>(
                        stmt))
            {
                if(!this->checkVarDecls(nblock)){
                    return false;
                }
                const vector<Stmt*>& stmts = nblock->getStmts();
                for(unsigned int i = 0; i < stmts.size(); ++i){
                    if(const EventCntrl *evctrl = 
                            dynamic_cast<const EventCntrl*>(stmts[i]))
                    {
                        SyMsgHandler::instance()->print(2, evctrl->getLine(),
                                mod->getName().c_str(), "event control");
                        return false;
                    }
                    else if(!this->checkStmt(stmts[i])){
                        synthesizable = false;
                    }
                }
            }
            return synthesizable;
        }

    bool
        SyParse2Synth::checkForLoopConst(
                const Expr* index,
                const Expr* expr) const
        {
            bool isConst = true;

            if(index->toString() == expr->toString()){
                return isConst;
            }

            if(/*const Const *const_ = */dynamic_cast<const Const*>(expr)){
                return true;
            }

            if(dynamic_cast<const Cntv*>(expr) || 
                    dynamic_cast<const VecCntv*>(expr)){
                return expr->isConstExpr();
            }

            if(const PartSel *partsel = dynamic_cast<const PartSel*>(expr))
            {
                if(!this->checkForLoopConst(index, partsel->getVar())){
                    return false;
                }

                const vector<Expr*>& dim = partsel->getDim();
                for(unsigned int i = 0; i < dim.size(); ++i){
                    if(!this->checkForLoopConst(index, dim[i])){
                        return false;
                    }
                }
                const Range *range = partsel->getRange();
                if(!this->checkForLoopConst(index, range->getLExpr())){
                    return false;
                }
                if(!this->checkForLoopConst(index, range->getRExpr())){
                    return false;
                }

            }
            if(const BitSel *bitsel = dynamic_cast<const BitSel*>(expr))
            {
                if(!this->checkForLoopConst(index, bitsel->getVar())){
                    return false;
                }

                const vector<Expr*>& dim = bitsel->getDim();
                for(unsigned int i = 0; i < dim.size(); ++i){
                    if(!this->checkForLoopConst(index, dim[i])){
                        return false;
                    }
                }

            }

            if(const Concat *concat = dynamic_cast<const Concat*>(expr))
            {
                const vector<Expr*>& exprs = concat->getExprs();
                for(unsigned int i = 0; i < exprs.size(); ++i){
                    if(!this->checkForLoopConst(index, exprs[i])){
                        return false;
                    }
                }
            }

            if(/*const FuncCall *funccall = */dynamic_cast<const FuncCall*>(expr)){
                return false;
            }

            if(/*const VString *vstring = */dynamic_cast<const VString*>(expr)){
                return false;
            }

            if(const UnaryExpr *unary = dynamic_cast<const UnaryExpr*>(expr)){
                if(!this->checkForLoopConst(index, unary->getExpr())){
                    return false;
                }
            }

            if(const BinExpr *binary = dynamic_cast<const BinExpr*>(expr)){
                if(!this->checkForLoopConst(index, binary->getLExpr()) ||
                        !this->checkForLoopConst(index, binary->getRExpr())){
                    return false;
                }
            }

            if(const CondExpr *cond = dynamic_cast<const CondExpr*>(expr)){
                if(!this->checkForLoopConst(index, cond->getCond()) ||
                        !this->checkForLoopConst(index, cond->getTExpr()) ||
                        !this->checkForLoopConst(index, cond->getFExpr())){
                    return false;
                }
            }
            return true;
        }

    bool
        SyParse2Synth::checkAlways(
                const Module *mod, 
                const Always *always) const
        {
            const EventCntrl *eveCtrl = dynamic_cast<const EventCntrl*>(
                    always->getStmt());
            if(!eveCtrl)
            {
                SyMsgHandler::instance()->print(7,
                        always->getLine(),
                        mod->getName().c_str());
                return false;
            }
            if(!this->checkSensList(mod, always)){
                return false;
            }
            if(!this->checkAlwaysStmts(mod, always)){
                return false;
            }
            return true;
        }


    bool
        SyParse2Synth::isSequential(const Always *always) const
        {
            const EventCntrl *eveCtrl = dynamic_cast<const EventCntrl*>(
                    always->getStmt());
            if(!eveCtrl){
                return false;
            }
            const vector<pair<EdgeType, Expr*> >& eventExprs = 
                eveCtrl->getEventExprs();
            for(unsigned int i = 0; i < eventExprs.size(); ++i){
                if(eventExprs[i].first != Rtl::NOEDGE){
                    return true;
                }
            }
            return false;
        }

    bool
        SyParse2Synth::checkSensList(
                const Module *mod,
                const Always *always) const
        {
            short noedge = 0;
            short posedge = 0;
            short negedge = 0;
            bool complex_ = false;
            bool multibit = false;
            bool status = true;
            const EventCntrl *eveCtrl = dynamic_cast<const EventCntrl*>(
                    always->getStmt());
            if(!eveCtrl){
                return false;
            }
            const vector<pair<EdgeType, Expr*> >& eventExprs = 
                eveCtrl->getEventExprs();
            for(unsigned int i = 0; i < eventExprs.size(); ++i){
                if(eventExprs[i].first == Rtl::NOEDGE){
                    ++noedge;
                }
                else if(eventExprs[i].first == Rtl::POSEDGE){
                    ++posedge;
                }
                else if(eventExprs[i].first == Rtl::NEGEDGE){
                    ++negedge;
                }
                if(dynamic_cast<VecCntv*>(eventExprs[i].second)) {
                    multibit = true;
                }    
                if(!dynamic_cast<ScalarCntv*>(eventExprs[i].second) &&
                        !dynamic_cast<BitSel*>(eventExprs[i].second) 
                        ){
                    complex_ = true;
                }
            }

            if(noedge && posedge)
            {
                SyMsgHandler::instance()->print(8, eveCtrl->getLine(),
                        mod->getName().c_str());
                status = false;
            }
            if(noedge && negedge)
            {
                SyMsgHandler::instance()->print(9, eveCtrl->getLine(),
                        mod->getName().c_str());
                status = false;
            }
            /*
            if((posedge+negedge) > 2)
            {
                SyMsgHandler::instance()->print(10, eveCtrl->getLine(),
                        mod->getName().c_str());
                status = false;
            }
            */
            if(complex_   && (((posedge || negedge)) || !multibit))  
            {
                SyMsgHandler::instance()->print(11, eveCtrl->getLine(),
                        mod->getName().c_str());
                status = false;
            } 
            /*if(((posedge + negedge) == 2) && !this->checkForSetReset(mod, always)){
                status = false;
            }*/

            return status;
        }

    bool
        SyParse2Synth::checkForSetReset(
                const Module *mod,
                const Always *always) const
        {
            //This function should be called only in the context of an always
            //statement with exactly 2 edge sensitive expressions.
            const EventCntrl *eventCtrl = dynamic_cast<const EventCntrl*>(
                    always->getStmt());
            if(!eventCtrl){
                return false;
            }
            const If *if_ = 0;
            if(const NamedBlock *nblock = dynamic_cast<const NamedBlock*>(
                        eventCtrl->getStmt()))
            {
                const vector<Stmt*>& stmts = nblock->getStmts();
                if(stmts.size() == 1) {
                    //note not handling weird nested block
                    if_ = dynamic_cast<const If*>(stmts[0]);
                }
            }
            else if(const Block *block = dynamic_cast<const Block*>(
                        eventCtrl->getStmt()))
            {
                const vector<Stmt*>& stmts = block->getStmts();
                if(stmts.size() == 1) {
                    //note not handling weird nested block
                    if_ = dynamic_cast<const If*>(stmts[0]);
                }
            }
            else {
                if_ = dynamic_cast<const If*>(eventCtrl->getStmt());
            }
            if(!if_)
            {
                SyMsgHandler::instance()->print(13, eventCtrl->getLine(), 
                        mod->getName().c_str());
                return false;
            }
            return true;
        }

    bool
        SyParse2Synth::checkAlwaysStmts(
                const Module* mod,
                const Always* always) const
        {
            const EventCntrl *eveCtrl = dynamic_cast<const EventCntrl*>(
                    always->getStmt());
            if(!eveCtrl){
                return true;
            }
            const Stmt *stmt = eveCtrl->getStmt();
            vector<Stmt*> stmts;
            if(const NamedBlock* nblock = dynamic_cast<const NamedBlock*>(
                        stmt))
            {
                stmts = nblock->getStmts();
                if(!checkVarDecls(nblock)) {
                    return false;
                }    
            }
            else if(const Block *block = dynamic_cast<const Block*>(
                        stmt)){
                stmts = block->getStmts();
            }
            else {
                stmts.push_back(const_cast<Stmt*>(stmt));
            }    
            bool status = true;
            //In the pair, first is true if the variable is used on
            //the left hand side of a blocking assign. Second is true
            //when then variable is used on the lhs in a non-blocking
            //assign
            map<string, pair<bool, bool>, cmp_str > nameAssignTypes;

            for(unsigned int i = 0; i < stmts.size(); ++i)
            {
                if(const CompAssign *cassign = 
                    dynamic_cast<const CompAssign*>(stmts[i]))
                {
                    SyMsgHandler::instance()->print(2, cassign->getLine(), 
                        mod->getName().c_str(), "intra-assignment event statement");
                    status = false;
                }    
                else if(!this->checkStmt(stmts[i])){
                    status = false;
                }
                this->checkAlwaysStmt(stmts[i], nameAssignTypes);
            }
            map<string, pair<bool, bool>, cmp_str >::const_iterator iter = 
                nameAssignTypes.begin();
            map<string, pair<bool, bool>, cmp_str >::const_iterator eiter = 
                nameAssignTypes.end();

            for(; iter != eiter; ++iter)
            {
                if(iter->second.first && iter->second.second)
                {
                    string name(iter->first.substr(
                                iter->first.find_last_of('/')+1));
                    SyMsgHandler::instance()->print(12, stmt->getLine(),
                            mod->getName().c_str(), name.c_str());
                    status = false;
                }
            }
            return status;
        }

    void
        SyParse2Synth::checkAlwaysStmt(
                const Stmt *stmt,
                map<string, pair<bool, bool>, cmp_str >& nameAssignTypes) const
        {
            string name;
            if(const ProcAssign *procassign = dynamic_cast<const ProcAssign*>(
                        stmt))
            {
                if(const ScalarCntv *sccntv = dynamic_cast<const ScalarCntv*>(
                            procassign->getLhs())){
                    //TBD: name algo should be changed, to use paths.
                    // This way does not create unique name
                    name = GetObjectPath(sccntv).toString();
                    name = name + string("/");
                    name = name + sccntv->getIxName().getName();
                }
                else if(const VecCntv *vcntv = dynamic_cast<const VecCntv*>(
                            procassign->getLhs())){
                    name = GetObjectPath(vcntv).toString();
                    name = name + string("/");
                    name = name + vcntv->getName();
                }
                else if(const BitSel *bitsel = dynamic_cast<const BitSel*>(
                            procassign->getLhs())){
                    name = GetObjectPath(bitsel).toString();
                    name = name + string("/");
                    name = name + bitsel->getVar()->toString();
                }
                else {
                    return;
                }
                map<string, pair<bool, bool>, cmp_str >::iterator iter =
                        nameAssignTypes.find(name);
                if(iter != nameAssignTypes.end()){
                    if(procassign->getAssignType() == BLOCK){
                        iter->second.first = true;
                    }
                    else {
                        iter->second.second = true;
                    }
                }
                else {
                    if(procassign->getAssignType() == BLOCK){
                        nameAssignTypes.insert(make_pair(name,
                                    make_pair( true, false)));
                    }
                    else {
                        nameAssignTypes.insert(make_pair( name,
                                    make_pair( false, true)));
                    }
                }
            }
            else if(const Block *block = dynamic_cast<const Block*>(stmt))
            {
                const vector<Stmt*>& stmts = block->getStmts();
                for(unsigned int i = 0; i < stmts.size(); ++i){
                    this->checkAlwaysStmt(stmts[i], nameAssignTypes);
                }
            }
            else if(const NamedBlock *nblock = dynamic_cast<const NamedBlock*>(
                        stmt))
            {
                checkVarDecls(nblock);
                const vector<Stmt*>& stmts = nblock->getStmts();
                for(unsigned int i = 0; i < stmts.size(); ++i){
                    this->checkAlwaysStmt(stmts[i], nameAssignTypes);
                }
            }
            else if(const Case *case_ = dynamic_cast<const Case*>(stmt)){
                const vector<CaseItem*>& caseItems = case_->getCaseItems();
                for(unsigned int i = 0; i < caseItems.size(); ++i) {
                    this->checkAlwaysStmt(caseItems[i]->getStmt(), nameAssignTypes);
                }
            }
            else if(const If *if_ = dynamic_cast<const If*>(stmt))
            {
                this->checkAlwaysStmt(if_->getStmt(), nameAssignTypes);
                this->checkAlwaysStmt(if_->getElsePart(), nameAssignTypes);
            }
            else if(const StmtCont *stcont = dynamic_cast<const StmtCont*>(
                        stmt)){
                this->checkAlwaysStmt(stcont->getStmt(), nameAssignTypes);
            }
            else {
                return;
            }
        }

    bool
        SyParse2Synth::checkPorts(const Scope*) const {
            //check needs to be added for constant function
            return true;
        }
}
