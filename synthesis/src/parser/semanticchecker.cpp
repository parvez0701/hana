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
#include <assert.h>
#include "semanticchecker.h"

#include "module.h"
#include "port.h"
#include "pin.h"
#include "net.h"
#include "vecport.h"
#include "vecpin.h"
#include "vecnet.h"
#include "param.h"
#include "vecparam.h"
#include "localparam.h"
#include "veclocalparam.h"
#include "root.h"
#include "gateinst.h"
#include "modinst.h"
#include "binexpr.h"
#include "bitsel.h"
#include "partsel.h"
#include "concat.h"
#include "condexpr.h"
#include "hierid.h"
#include "intconst.h"
#include "multiconcat.h"
#include "num.h"
#include "basednum.h"
#include "unaryexpr.h"
#include "vstring.h"
#include "reg.h"
#include "vecreg.h"
#include "integer.h"
#include "vecint.h"
#include "vtime.h"
#include "vectime.h"
#include "real.h"
#include "vecreal.h"
#include "realtime.h"
#include "vecrealtime.h"
#include "force.h"
#include "namedblock.h"
#include "block.h"
#include "fork.h"
#include "disable.h"
#include "trigger.h"
#include "loopstmts.h"
#include "case.h"
#include "if.h"
#include "event.h"
#include "compassign.h"
#include "taskfunc.h"
#include "always.h"
#include "initial.h"
#include "procassign.h"
#include "proccontassign.h"
#include "contassign.h"
#include "defparam.h"
#include "specify.h"
#include "udp.h"
#include "udpinst.h"
#include "generate.h"
#include "elaborator.h"
#include "implicitnet.h"

#include "msghandler.h"

using namespace std;
namespace Rtl
{
    extern Root* GRoot;

    template<class T> void
    Accept(SemanticChecker *semCheck, const vector<T*>& vec) {
        for(unsigned int i = 0; i < vec.size(); ++i) {
            vec[i]->accept(semCheck);
        }
    }    

    SemanticChecker::SemanticChecker() : context(1), status(true), assign(0) {
    }

    SemanticChecker::~SemanticChecker() {
    }

    void
    SemanticChecker::setContext(ExprContext ctx) {
        context |= ctx;
    }

    void
    SemanticChecker::resetContext(ExprContext ctx) {
        unsigned int tmp = 0xffff;
        tmp ^= ctx;
        context &= tmp;
    }    

    bool
    SemanticChecker::isContext(ExprContext ctx) const {
        return context & ctx;
    }    

    void
    SemanticChecker::visit(const Module *module)
    {
        status = true;
        //Continuous assign statements.
        const vector<ContAssign*>& contAssigns = 
            module->getContAssigns();
        int size = contAssigns.size();
        for(int i = 0; i < size; ++i)
        {
            assign = contAssigns[i];
            contAssigns[i]->accept(this);
            assign = 0;
        }
        //End continuous assigns
        
        //Defparam assign statements
        const vector<Defparam*>& defparams = module->getDefparams();
        size = defparams.size();
        for(int i = 0; i < size; ++i){
            defparams[i]->accept(this);
        }
        //End defparam assign statements

        const vector<EventStmt*> events = module->getEvents();
        size = events.size();
        for(int i = 0; i < size; ++i){
            events[i]->accept(this);
        }
        

        //Initial blocks
        const std::vector<Initial*>& initials = module->getInitBlocks();
        size = initials.size();
        for(int i = 0; i < size; ++i) {
            initials[i]->accept(this);
        }
        //End initial blocks

        const std::list<GateInst*>& ginsts = module->getGateInsts();
        size = ginsts.size();
        std::list<GateInst*>::const_iterator giter = ginsts.begin();
        std::list<GateInst*>::const_iterator egiter = ginsts.end();
        for(; giter != egiter; ++giter){
            (*giter)->accept(this);
        }

        const std::list<ModInst*>& minsts = module->getModInsts();
        size = minsts.size();
        std::list<ModInst*>::const_iterator miter = minsts.begin();
        std::list<ModInst*>::const_iterator emiter = minsts.end();
        for(; miter != emiter; ++miter){
            (*miter)->accept(this);
        }

        const std::list<UDPInst*>& uinsts = module->getUDPInsts();
        size = uinsts.size();
        std::list<UDPInst*>::const_iterator uiter = uinsts.begin();
        std::list<UDPInst*>::const_iterator euiter = uinsts.end();
        for(; uiter != euiter; ++uiter){
            (*uiter)->accept(this);
        }


        //Always blocks
        const std::vector<Always*>& always = module->getAlwaysBlocks();
        size = always.size();
        for(int i = 0; i < size; ++i) {
            always[i]->accept(this);
        }
        //End always blocks
        
        //Specify blocks
        const vector<SpecifyBlock*>& specifyBlocks = 
        module->getSpecifyBlocks();
        size = specifyBlocks.size();
        for(int i = 0; i < size; ++i){
            specifyBlocks[i]->accept(this);
        }
        //End specify blocks
        
        //Tasks
        
        const vector<Task*> tasks = module->getTasks();
        size = tasks.size();
        for(int i = 0; i < size; ++i){
            tasks[i]->accept(this);
        }
        //End task
        //Functions
        const vector<Func*> funcs = module->getFuncs();
        size = funcs.size();
        for(int i = 0; i < size; ++i){
            funcs[i]->accept(this);
        }

        //End function

        //Generate statements
        const vector<GenerateStmt*>& generates = module->getGenStmts();
        size = generates.size();
        for(int i = 0; i < size; ++i){
            generates[i]->accept(this);
        }

                
    }

    void
    SemanticChecker::visit(const ContAssign *cassign)
    {
        this->setContext(CONTASSGNLEFT);
        assign = const_cast<ContAssign*>(cassign);
        assign->setLhs(this->checkImplicitNet(assign->getLhs()));
        if(cassign->getLhs()){
            assign->getLhs()->accept(this);
        }    
        this->resetContext(CONTASSGNLEFT);

        assign->setRhs(this->checkImplicitNet(assign->getRhs()));
        if(assign->getRhs()) {
            assign->getRhs()->accept(this);
        }    
        this->resetContext(CONTASSGNRIGHT);
        assign = 0;
    }

    void
    SemanticChecker::visit(const GateInst* ginst)
    {
        const vector<Pin*> pins = ginst->getPins();
        const int size = pins.size();
        this->setContext(GATEINSTPIN);
        for(int i = 0; i < size; ++i)
        {
            if(const Expr *actual = pins[i]->getActual()){
                pins[i]->setActual(this->checkImplicitNet(actual));
            }    
            if(const Expr *actual = pins[i]->getActual()){
                actual->accept(this);
            }    
        }
        this->resetContext(GATEINSTPIN);
    }

    void
    SemanticChecker::visit(const ModInst* minst)
    {
        const vector<Pin*> pins = minst->getPins();
        const int size = pins.size();
        this->setContext(MODINSTPIN);
        for(int i = 0; i < size; ++i)
        {
            if(const Expr *actual = pins[i]->getActual()){
                pins[i]->setActual(this->checkImplicitNet(actual));
            }    
            if(const Expr *actual = pins[i]->getActual()){
                actual->accept(this);
            }    
        }
        this->checkPortConnections(minst);
        this->resetContext(MODINSTPIN);
    }

    void
    SemanticChecker::visit(const BinExpr *binExpr)
    {
        const_cast<BinExpr*>(binExpr)->setLExpr(
            this->checkImplicitNet(binExpr->getLExpr()));
        const_cast<BinExpr*>(binExpr)->setRExpr(
            this->checkImplicitNet(binExpr->getRExpr()));
        if(binExpr->getLExpr()){
            binExpr->getLExpr()->accept(this);
        }
        if(binExpr->getRExpr()){
            binExpr->getRExpr()->accept(this);
        }    
    }

    void
    SemanticChecker::visit(const UnaryExpr *uexpr)
    {
        const_cast<UnaryExpr*>(uexpr)->setExpr(
            this->checkImplicitNet(uexpr->getExpr()));
        if(uexpr->getExpr()){
            uexpr->getExpr()->accept(this);
        }    
    }    

    void
    SemanticChecker::visit(const BitSel *bitSel) 
    {
        if(const VecPort *vport = 
            dynamic_cast<const VecPort*>(bitSel->getVar())) {
            if(vport->getVarType() == "reg") {
                this->printContAssignLhsError(bitSel);
            }    
        }    
        else if(!dynamic_cast<const VecNet*>(bitSel->getVar())) {
            this->printContAssignLhsError(bitSel);
        }    

        checkBitSelect(bitSel);

        const vector<Expr*>& dim = bitSel->getDim();
        for(int i = 0; i < dim.size(); ++i) {
            dim[i]->accept(this);
        }    
    }

    void
    SemanticChecker::visit(const PartSel *partSel)
    {
        if(const VecPort *vport = 
            dynamic_cast<const VecPort*>(partSel->getVar())) {
            if(vport->getVarType() == "reg") {
                this->printContAssignLhsError(partSel);
            }    
        }    
        else if(!dynamic_cast<const VecNet*>(partSel->getVar())) {
            this->printContAssignLhsError(partSel);
        }    

        checkPartSelect(partSel);

        const Range *range = partSel->getRange();
        range->getLExpr()->accept(this);
        range->getRExpr()->accept(this);

        const vector<Expr*>& dim = partSel->getDim();
        for(int i = 0; i < dim.size(); ++i) {
            dim[i]->accept(this);
        }    
    }

    void
    SemanticChecker::visit(const Concat *conc)
    {
        vector<Expr*>& exprs = const_cast<vector<Expr*>&>(conc->getExprs());
        for(unsigned int i = 0; i < exprs.size(); ++i) 
        {
            exprs[i] = const_cast<Expr*>(
                this->checkImplicitNet(exprs[i]));
            if(exprs[i]){
                exprs[i]->accept(this);
            }    
        }
    }    

    void
    SemanticChecker::visit(const MultiConcat *mconc)
    {
        const_cast<MultiConcat*>(mconc)->setCount(
            this->checkImplicitNet(mconc->getCount()));
        const Expr* count = mconc->getCount();
        this->setContext(MULTICONC);
        if(count) {
            count->accept(this);
        }
        this->resetContext(MULTICONC);

        //const Concat *conc = static_cast<const Concat*>(mconc);
        //conc->accept(this);
        vector<Expr*>& exprs = const_cast<vector<Expr*>&>(mconc->getExprs());
        for(unsigned int i = 0; i < exprs.size(); ++i) 
        {
            exprs[i] = const_cast<Expr*>(
                this->checkImplicitNet(exprs[i]));
            if(exprs[i]){
                exprs[i]->accept(this);
            }    
        }
    }    

    void
    SemanticChecker::visit(const CondExpr *condExpr)
    {
        const_cast<CondExpr*>(condExpr)->setCond(
            this->checkImplicitNet(condExpr->getCond()));
        const_cast<CondExpr*>(condExpr)->setTExpr(
            this->checkImplicitNet(condExpr->getTExpr()));
        const_cast<CondExpr*>(condExpr)->setFExpr(
            this->checkImplicitNet(condExpr->getFExpr()));

        if(condExpr->getCond()) {
            condExpr->getCond()->accept(this);
        }
        if(condExpr->getTExpr()) {
            condExpr->getTExpr()->accept(this);
        }
        if(condExpr->getFExpr()){
            condExpr->getFExpr()->accept(this);
        }    
    }    

    void
    SemanticChecker::visit(const HierId *hid) 
    {
        if(!hid->getActualId()) 
        {
            Path path = GetObjectPath(hid);
            const_cast<HierId*>(hid)->resolve(path);
        }    
    }

    void
    SemanticChecker::visit(const IntConst* iconst) 
    {
    }

    void
    SemanticChecker::visit(const VecInt* vint)
    {
        this->printContAssignLhsError(vint);
        this->printGenerateForError(vint);
    }
        
    void
    SemanticChecker::visit(const Num* num)
    {
    }
    void
    SemanticChecker::visit(const BasedNum* num)
    {
    }
    void
    SemanticChecker::visit(const Reg* reg)
    {
        this->printContAssignLhsError(reg);
        this->printGenerateForError(reg);
    }
    void
    SemanticChecker::visit(const VecReg* vreg)
    {
        this->printContAssignLhsError(vreg);
        this->printGenerateForError(vreg);
    }
    void
    SemanticChecker::visit(const Force* force)
    {
        force->getLhs()->accept(this);
        force->getRhs()->accept(this);
    }
    void
    SemanticChecker::visit(const Release* release)
    {
        release->getVar()->accept(this);
        
    }

    void
    SemanticChecker::visit(const NamedBlock* nblock)
    {
        //this->acceptStmts(nblock->getStmts());
        Accept<Stmt>(this, nblock->getStmts());
    }
    void
    SemanticChecker::visit(const Block* block)
    {
        //this->acceptStmts(block->getStmts());
        Accept<Stmt>(this, block->getStmts());
    }

    void
    SemanticChecker::visit(const SpecifyBlock* )
    {
    }
    void
    SemanticChecker::visit(const Specparam* )
    {
    }
    void
    SemanticChecker::visit(const SpecifyPath* )
    {
    }
    void
    SemanticChecker::visit(const SpecifyEdgePath* )
    {
    }
    void
    SemanticChecker::visit(const Fork* )
    {
    }
    void
    SemanticChecker::visit(const Disable* )
    {
    }
    void
    SemanticChecker::visit(const Trigger* )
    {
    }

    void
    SemanticChecker::visit(const Forever* forever)
    {
        forever->getStmt()->accept(this);
    }
    void
    SemanticChecker::visit(const Repeat* repeat)
    {
        const_cast<Repeat*>(repeat)->setExpr(
            this->checkImplicitNet(repeat->getExpr()));
        if(repeat->getExpr()){
            repeat->getExpr()->accept(this);
        }    
        repeat->getStmt()->accept(this);
    }
    void
    SemanticChecker::visit(const For* for_)
    {
        const_cast<For*>(for_)->setInitExpr(
            this->checkImplicitNet(for_->getInitExpr()));
        const_cast<For*>(for_)->setAssignExpr(
            this->checkImplicitNet(for_->getAssignExpr()));
        const_cast<For*>(for_)->setTermExpr(
            this->checkImplicitNet(for_->getTermExpr()));
        
        if(const Expr *expr = for_->getInitExpr()) {
            expr->accept(this);
        }

        if(const Expr *expr = for_->getAssignExpr()) {
            expr->accept(this);
        }

        if(const Expr *expr = for_->getTermExpr()) {
            expr->accept(this);
        }
        if(const Stmt *stmt = for_->getStmt()) {
            stmt->accept(this);
        }    
        
    }
    void
    SemanticChecker::visit(const While* while_)
    {
        const_cast<While*>(while_)->setExpr(
            this->checkImplicitNet(while_->getExpr()));
        if(while_->getExpr()){
            while_->getExpr()->accept(this);
        }    
        if(const Stmt *stmt = while_->getStmt()) {
            stmt->accept(this);
        }    
    }

    void
    SemanticChecker::visit(const CaseItem* caseItem)
    {
        if(!caseItem->isDefault())
        {
            const vector<Expr*>& label = caseItem->getLabel();
            for(unsigned int i = 0; i < label.size(); ++i) 
            {
                if(!label[i]->isConstExpr())
                {
                    MsgHandler::instance()->print(33, label[i]->getLine(), label[i]->toString().c_str()); 
                    status = false;
                }
            }

            caseItem->getStmt()->accept(this);
        }    
            
        
    }
    void
    SemanticChecker::visit(const Case* case_)
    {
        const vector<CaseItem*>& caseItems = case_->getCaseItems();
        for(unsigned int i = 0; i < caseItems.size(); ++i) {
            caseItems[i]->accept(this);
        }    
    }
    void
    SemanticChecker::visit(const If* if_)
    {
        const_cast<If*>(if_)->setCondExpr(
            this->checkImplicitNet(if_->getCondExpr()));
        if(if_->getCondExpr()) {
            if_->getCondExpr()->accept(this);
        }    
        if(const Stmt *stmt = if_->getStmt()) {
            stmt->accept(this);
        }

        if(const Stmt *stmt = if_->getElsePart()) {
            stmt->accept(this);
        }    
    }
    void
    SemanticChecker::visit(const EventCntrl* eventCtrl)
    {
        if(const Stmt *stmt = eventCtrl->getStmt()){
            stmt->accept(this);
        }    
    }
    void
    SemanticChecker::visit(const EventStmt* )
    {
    }
    void
    SemanticChecker::visit(const DelayStmt* )
    {
    }
    void
    SemanticChecker::visit(const Delay* )
    {
    }
    void
    SemanticChecker::visit(const CompAssign* )
    {
    }
    void
    SemanticChecker::visit(const ProcAssign* passign)
    {
        assign = const_cast<ProcAssign*>(passign);
        assign->setLhs(this->checkImplicitNet(assign->getLhs()));
        this->setContext(PROCASSIGNLEFT);
        if(assign->getLhs()){
            passign->getLhs()->accept(this);
        }    
        this->resetContext(PROCASSIGNLEFT);
        
        this->setContext(PROCASSIGNRIGHT);
        assign->setRhs(this->checkImplicitNet(assign->getRhs()));
        if(assign->getRhs()) {
            passign->getRhs()->accept(this);
        }    
        this->resetContext(PROCASSIGNRIGHT);
        assign = 0;
    }
    void
    SemanticChecker::visit(const ProcContAssign* )
    {
    }
    void
    SemanticChecker::visit(const Wait* )
    {
    }
    void
    SemanticChecker::visit(const TaskFuncCall* )
    {
    }
    void
    SemanticChecker::visit(const Task* )
    {
    }
    void
    SemanticChecker::visit(const Func* )
    {
    }
    void
    SemanticChecker::visit(const Always* always)
    {
        always->getStmt()->accept(this);
    }
    void
    SemanticChecker::visit(const Initial* initial)
    {
        if(const Stmt *s = initial->getStmt())
        {
            s->accept(this);
        }
    }
    void
    SemanticChecker::visit(const Defparam* defparam)
    {
        if(!defparam->getRhs()->isConstExpr()) 
        {
            MsgHandler::instance()->print(54, defparam->getLine());
            status = false;
        }    
        defparam->getLhs()->accept(this);
        defparam->getRhs()->accept(this);
    }
    void
    SemanticChecker::visit(const UDP* )
    {
        status = true;
    }
    void
    SemanticChecker::visit(const UDPInst* )
    {
    }
    void
    SemanticChecker::visit(const GenerateStmt* genStmt)
    {
        this->setContext(GENVAR);

        Accept<GenerateCond>(this, genStmt->getCondGens());
        Accept<GenerateCase>(this, genStmt->getCaseGens());
        Accept<GenerateLoop>(this, genStmt->getLoopGens());
        Accept<GenerateBlock>(this, genStmt->getBlockGens());

        this->resetContext(GENVAR);
    }
    void
    SemanticChecker::visit(const GenerateCond* genCond)
    {
        if(!genCond->condexpr->isConstExpr()) 
        {
            MsgHandler::instance()->print(34, genCond->getLine()); 
            status = false;
        }
        genCond->ifpart->accept(this);
        if(genCond->elsepart) {
            genCond->elsepart->accept(this);
        }    
    }
    void
    SemanticChecker::visit(const GenerateCase* genCase)
    {
        if(!genCase->select->isConstExpr())
        {
            MsgHandler::instance()->print(35, genCase->getLine());
            status = false;
        }
        for(unsigned int i = 0; i < genCase->caseitems.size(); ++i) 
        {
            genCase->caseitems[i].second->accept(this);
            for(unsigned int j = 0; j < genCase->caseitems[i].first.size();
                ++j)
            {
                if(!genCase->caseitems[i].first[j]->isConstExpr())
                {
                    MsgHandler::instance()->print(36, genCase->caseitems[i].first[j]->getLine());
                    status = false;
                }    
            }    
        }
    }
    void
    SemanticChecker::visit(const GenerateLoop* genLoop)
    {
        this->setContext(GENFORHEAD);
        genLoop->loopvar->accept(this);
        genLoop->initexpr->accept(this);
        genLoop->termexpr->accept(this);
        genLoop->assignexpr->accept(this);
        this->resetContext(GENFORHEAD);
        genLoop->gitem->accept(this);
    }
    void
    SemanticChecker::visit(const GenerateBlock* genBlock)
    {
        Accept<GenerateItem>(this, genBlock->gitems);
    }
    void
    SemanticChecker::visit(const GenDecl* )
    {
    }
    void
    SemanticChecker::visit(const Genvar *genvar )
    {
        if(!this->isContext(GENVAR)) {
            this->printGenvarError(genvar);
        }    
    }

    void
    SemanticChecker::visit(const VecPort *vport)
    {
        if(vport->getVarType() != "wire") {
            this->printContAssignLhsError(vport);
        }    
        else {
            this->printProcAssignLhsError(vport);
        }    
        this->printGenerateForError(vport);
    }
    
    void
    SemanticChecker::visit(const Port *port)
    {
        if(port->getVarType() != "wire") {
            this->printContAssignLhsError(port);
        }    
        else {
            this->printProcAssignLhsError(port);
        }    
        this->printGenerateForError(port);
    }

    void
    SemanticChecker::visit(const VecPin *vpin)
    {
    }
    
    void
    SemanticChecker::visit(const Pin *pin)
    {
    }

    void
    SemanticChecker::visit(const VecNet *vnet)
    {
        this->printProcAssignLhsError(vnet);
        this->printGenerateForError(vnet);
    }
    
    void
    SemanticChecker::visit(const Net *net)
    {
        this->printProcAssignLhsError(net);
        this->printGenerateForError(net);
    }

    void
    SemanticChecker::visit(const VecRealTime *vrt)
    {
        this->printContAssignLhsError(vrt);
        this->printGenerateForError(vrt);
    }
    void
    SemanticChecker::visit(const RealTime *rt)
    {
        this->printContAssignLhsError(rt);
        this->printGenerateForError(rt);
    }
    void
    SemanticChecker::visit(const VecReal *vreal)
    {
        this->printContAssignLhsError(vreal);
        this->printGenerateForError(vreal);
    }
    void
    SemanticChecker::visit(const Real *real)
    {
        this->printContAssignLhsError(real);
        this->printGenerateForError(real);
    }
    void
    SemanticChecker::visit(const Time * t)
    {
        this->printContAssignLhsError(t);
        this->printGenerateForError(t);
    }
    void
    SemanticChecker::visit(const VecTime *vt)
    {
        this->printContAssignLhsError(vt);
        this->printGenerateForError(vt);
    }
    void
    SemanticChecker::visit(const VString *str)
    {
        this->printContAssignLhsError(str);
    }

    void
    SemanticChecker::visit(const Integer *i)
    {
        this->printContAssignLhsError(i);
    }

    void
    SemanticChecker::visit(const ImplicitNet *)
    {
        //By now implicit nets should be resolved
        assert(0);
    }

/*    void
    SemanticChecker::acceptStmts(const vector<Stmt*>& stmts)
    {
        for(int i = 0; i < stmts.size(); ++i) {
            stmts[i]->accept(this);
        }    
    }
*/
    void
    SemanticChecker::printContAssignLhsError(const Expr* lhs) 
    {
        if(!this->isContext(CONTASSGNLEFT) ) {
            return;
        }    
        MsgHandler::instance()->print(37, assign->getLine(), lhs->toString().c_str());
        status = false;
        
    }
    void
    SemanticChecker::printProcAssignLhsError(const Expr* lhs) 
    {
        if(!this->isContext(PROCASSIGNLEFT) ) {
            return;
        }    
        MsgHandler::instance()->print(38, assign->getLine(), lhs->toString().c_str());
        status = false;
        
    }

    void
    SemanticChecker::printGenvarError(const Genvar *genvar)
    {
        if(!this->isContext(GENVAR))
        {
            MsgHandler::instance()->print(39, genvar->getLine(), genvar->toString().c_str());
            status = false;
        }    
    }

    void
    SemanticChecker::printGenvarUnInitError(const Genvar *genvar)
    {
        if(!this->isContext(GENVAR))
        {
            return;
        }    
        if(genvar->evaluate() == INT_MIN)
        {
            MsgHandler::instance()->print(40, genvar->getLine(), genvar->toString().c_str());
            status = false;
        }    
    }

    const Expr*
    SemanticChecker::checkImplicitNet(const Expr *expr)
    {
        ImplicitNet *inet = const_cast<ImplicitNet*>(
            dynamic_cast<const ImplicitNet*>(expr));
        if(!inet) {
            return expr;
        }    
        if(this->isContext(CONTASSGNLEFT) || this->isContext(MODINSTPIN) ||
            this->isContext(GATEINSTPIN))
        {
            //In these contexts implicit net is to be created
            if(GenerateStmt *genStmt = const_cast<GenerateStmt*>(
                dynamic_cast<const GenerateStmt*>(inet->getScope())))
            {
                ModGenDecl *modGenDecl = genStmt->getGenDecl();
                modGenDecl->nets.push_back(inet->getNet());
                return inet->getNet();
            }
            else if(GenerateBlock *genBlock = const_cast<GenerateBlock*>(
                dynamic_cast<const GenerateBlock*>(inet->getScope())))
            {    
                ModGenDecl *modGenDecl = genBlock->getGenDecl();
                modGenDecl->nets.push_back(inet->getNet());
                return inet->getNet();
            }

            Module *mod = const_cast<Module*>(GetModule(inet));
            Base *base = mod->getObject(inet->toString());
            if(!base)
            {
                vector<Net*> v;
                v.push_back(inet->getNet());
                mod->addNets(v);
                return inet->getNet();
            }
            else if(Expr *expr = dynamic_cast<Expr*>(base)) {
                return expr;
            }
            else
            {
                MsgHandler::instance()->print(50, inet->getLine());
                status = false;
                return 0;
            }    
        }    
        const Module *mod = GetModule(inet);
        Base *base = mod->getObject(inet->toString());
        if(!base)
        {
            MsgHandler::instance()->print(41, inet->getLine(), inet->toString().c_str());
            status = false;
            return 0;
        }    
        else if(Expr *expr = dynamic_cast<Expr*>(base)) {
            return expr;
        }
        else
        {
            MsgHandler::instance()->print(50, inet->getLine());
            status = false;
            return 0;
        }    
            
    }

    void
    SemanticChecker::printGenerateForError(const Expr *expr)
    {
        if(!this->isContext(GENFORHEAD)) {
            return;
        }    

        MsgHandler::instance()->print(42, expr->getLine(), expr->toString().c_str());
        status = false;
    }    

    void
    SemanticChecker::checkPortConnections(const ModInst *minst) const
    {
        const Module *master = minst->getMaster();
        if(!master) {
            return;
        }
        vector<VecPort*> vports = master->getVectorPorts();

        for(int i = 0; i < vports.size(); ++i)
        {
            const VecPort *vport = vports[i];
            VecPin *vpin = const_cast<VecPin*>(minst->getVecPin(vport->getName()));
            assert(vpin);
            const Expr *act = vpin->getActual();
            if(!act)
            {
                MsgHandler::instance()->print(71, vpin->getLine(),  vpin->getName().c_str());
                continue;
            }    
            long psize = vport->len();
            long asize = act->len();
            if(psize != asize) {
                MsgHandler::instance()->print(72, vpin->getLine(), vport->getName().c_str(), psize, asize);
            }    
        }
        
    }

    bool
    SemanticChecker::checkDimension(const BitSel *bitsel, bool ispartsel) const
    {
        const std::vector<Expr*>& dim = bitsel->getDim();
        const Expr *expr = bitsel->getVar();
        const char *type = ispartsel ? "part" : "bit";
        switch(ParseDimension(dim, expr, ispartsel))
        {
            case 0:
            {
                return true;
            }
            case 1:
                MsgHandler::instance()->print(30, bitsel->getLine(), type);
                return false;
            case 2:
                MsgHandler::instance()->print(32, bitsel->getLine());
                return false;
            case 3:
                MsgHandler::instance()->print(29, bitsel->getLine(), type);
                return false;
            case 4:
                assert(0);
            default:
                break;
        }
        return true;
    }
    void
    SemanticChecker::checkPartSelect(const PartSel *partsel)
    {
        //For generated stuff
        if(checkDimension(partsel, true))
        {
            const Range *range = partsel->getRange();
            PolOp polop = partsel->getPolarityOperator();
            int pstatus = (polop != POCOL) ? 0 : ParseRange(range, partsel->getVar());
            switch(pstatus)
            {
                case 0:    
                    break;
                case 1:
                    MsgHandler::instance()->print(29, partsel->getLine(), "part");
                    status = false;
                    break;
                case 2:
                    MsgHandler::instance()->print(30, partsel->getLine(), "part");
                    status = false;
                    break;
                case 3:
                    MsgHandler::instance()->print(29, partsel->getLine(), "part");
                    break;
                case 4:
                    MsgHandler::instance()->print(31, partsel->getLine());
                    status = false;
                    break;
                default:
                    break;
            }
        }    
        else {
            status = false;
        }    
    }

    void
    SemanticChecker::checkBitSelect(const BitSel *bitsel)
    {
        //For generated stuff
        if(!checkDimension(bitsel, false)) {
            status = false;
        }    
    }
}    
