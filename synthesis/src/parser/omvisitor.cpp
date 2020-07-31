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
#include <set>
#include <stdlib.h>
#include <unistd.h>
#include "omvisitor.h"

#include "msghandler.h"

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

using namespace std;
namespace Rtl
{
    extern Root* GRoot;
    OMVisitor::OMVisitor(const string& fname):
        indent(0), linewidth(80), useNameConn(true)
    {
        outfile.open(fname.c_str(), ios_base::trunc);
        if(outfile.fail())
        {
            char str[1024];
            //sprintf(str, "[ERROR]: Can not open file '%s' to write object model.\n", fname.c_str());
            //GRoot->printLogMsg(str);
            MsgHandler::instance()->print(14, fname.c_str());
            exit(0);
        }
    }

    OMVisitor::~OMVisitor()
    {
        outfile.close();
    }

    void
    OMVisitor::visit(const Module* module)
    {
        set<string, compare_str> doneObjs;
        //  ---------------------------------------------------------
        //    Write module header.
        //    ---------------------------------------------------------
        this->printModHead(module);
        //    End module header writing
        incInd();
        printInd();
        //    ---------------------------------------------------------
        //    Write parameter declaration.
        //    ---------------------------------------------------------
        const vector<Param*>& params = module->getParams();
        int size = params.size();
        if(size > 0){
            outfile << "// Parameter declarations.\n";
        }
        for(int i = 0; i < size; ++i)
        {
            Param* param = params[i];
            IxName ixName = param->getIxName();
            if(param->isVecBit())
            {
                if(doneObjs.find(ixName.getName()) != doneObjs.end()){
                    continue;
                }
                doneObjs.insert(ixName.getName());
                const VecParam* vecParam = static_cast<const VecParam*>(
                param->getVecCntv());
                vecParam->accept(this);
            }
            else{
                param->accept(this);
            }
            //printInd();
        }
        //End parameter declaration writing

        //    ---------------------------------------------------------
        //    Write parameter declaration.
        //    ---------------------------------------------------------
        const vector<LocalParam*>& lparams = module->getLocalParams();
        size = lparams.size();
        if(size > 0){
            outfile << "// Localparam declarations.\n";
        }
        for(int i = 0; i < size; ++i)
        {
            LocalParam* lparam = lparams[i];
            IxName ixName = lparam->getIxName();
            if(lparam->isVecBit())
            {
                if(doneObjs.find(ixName.getName()) != doneObjs.end()){
                    continue;
                }
                doneObjs.insert(ixName.getName());
                const VecLocalParam* veclParam = static_cast<const VecLocalParam*>(
                lparam->getVecCntv());
                veclParam->accept(this);
            }
            else{
                lparam->accept(this);
            }
            //printInd();
        }
        //End local parameter declaration writing
        //    ---------------------------------------------------------
        //    ---------------------------------------------------------
        //    Write port declaration
        //    ---------------------------------------------------------
        const vector<Port*>& ports = module->getPorts();
        size = ports.size();
        for(int i = 0; i < size; ++i)
        {
            Port* port = ports[i];
            IxName ixName = port->getIxName();
            if(port->isVecBit())
            {
                if(doneObjs.find(ixName.getName()) != doneObjs.end()){
                    continue;
                }
                doneObjs.insert(ixName.getName());
                const VecPort* vecPort = static_cast<const VecPort*>(
                port->getVecCntv());
                vecPort->accept(this);
            }
            else {
                port->accept(this);
            }
        }
        //    End wrting port declaration
        

        //    ---------------------------------------------------------
        //    Write net declaration
        //    ---------------------------------------------------------
        const vector<Net*>& nets = module->getNets();
        size = nets.size();
        for(int i = 0; i < size; ++i)
        {
            Net* net = nets[i];
            IxName ixName = net->getIxName();
            if(net->isVecBit())
            {
                if(doneObjs.find(ixName.getName()) != doneObjs.end()){
                    continue;
                }
                doneObjs.insert(ixName.getName());
                const VecNet* vecNet = static_cast<const VecNet*>(
                    net->getVecCntv());
                vecNet->accept(this);
            }
            else {
                net->accept(this);
            }
            printInd();
        }

        //    End writing nets

        this->printScopeDecls(module);
        const vector<EventVar*>& evars = module->getEventVars();
        size = evars.size();
        for(int i = 0; i < size; ++i){
            evars[i]->accept(this);
        }

        //Genvars
        const vector<Genvar*>& genvars = module->getGenvars();
        size = genvars.size();
        for(int i = 0; i < size; ++i){
            genvars[i]->accept(this);
        }

        //End genvar


        //Continuous assign statements.
        const vector<ContAssign*>& contAssigns = 
            module->getContAssigns();
        size = contAssigns.size();
        for(int i = 0; i < size; ++i){
            contAssigns[i]->accept(this);
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

        //    ---------------------------------------------------------
        //    Write gate instances
        //    ---------------------------------------------------------
        const std::list<GateInst*>& ginsts = module->getGateInsts();
        size = ginsts.size();
        outfile <<"\n";
        if(size > 0){
            outfile << "//Primitive gate instantiations.\n";
        }
        std::list<GateInst*>::const_iterator giter = ginsts.begin();
        std::list<GateInst*>::const_iterator egiter = ginsts.end();
        for(; giter != egiter; ++giter){
            printInd();
            (*giter)->accept(this);
        }

        //    -----------------------------------------------------------
        //    Write module/udp instances
        //    -----------------------------------------------------------
        //printInd();
        const std::list<ModInst*>& minsts = module->getModInsts();
        size = minsts.size();
        if(size > 0){
            outfile << "//Module instantiations.\n";
        }
        std::list<ModInst*>::const_iterator miter = minsts.begin();
        std::list<ModInst*>::const_iterator emiter = minsts.end();
        for(; miter != emiter; ++miter){
            printInd();
            (*miter)->accept(this);
        }

        //-------------------------------------------------------------
        // Write UDP instances.
        // ------------------------------------------------------------
        const std::list<UDPInst*>& uinsts = module->getUDPInsts();
        size = uinsts.size();
        if(size > 0){
            outfile << "//UDP instantiations.\n";
        }
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
        //Now since we are elaborating generate statement,
        //we would not print the original one.
        //const vector<GenerateStmt*>& generates = module->getGenStmts();
        //size = generates.size();
        //for(int i = 0; i < size; ++i){
        //    generates[i]->accept(this);
        //}

                
        decInd();
        outfile <<"\nendmodule\n";
    }

    void
    OMVisitor::visit(const Port* port)
    {
        printInd();
        outfile << GetDirString(port->getDir());
        outfile <<" " <<port->getVarType() << " ";
        if(port->isSigned()){
            outfile << "signed ";
        }
        outfile << port->getIxName().getName() << ";\n";
    }

    void
    OMVisitor::visit(const Net* net)
    {
        outfile << GetNetTypeString(net->getNetType()) << " "; 
        outfile << net->getIxName().getName();
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const VecPort* vport)
    {
        printInd();
        const vector<Port*>& ports = vport->getPorts();
        const Dimension* dim = vport->getDim();
        outfile << GetDirString(ports[0]->getDir()) << " ";
        outfile << ports[0]->getVarType() << " ";
        if(ports[0]->isSigned()){
            outfile << "signed ";
        }
        this->printPackDim(dim);
        outfile << " " << vport->getName() <<" ";
        this->printUnpackDim(dim);
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const VecNet* vnet)
    {
        const vector<Net*>& nets = vnet->getNets();
        const Dimension* dim = vnet->getDim();
        outfile << GetNetTypeString(nets[0]->getNetType()) << " ";
        this->printPackDim(dim);
        outfile << " " << vnet->getName() <<" ";
        this->printUnpackDim(dim);
        outfile << ";\n";
    }
    void
    OMVisitor::visit(const Pin* pin){
        bool isNamedConn = !pin->getIxName().getName().empty();
        isNamedConn = isNamedConn && useNameConn;
        if(isNamedConn){
            outfile << "." << pin->getIxName().getName() <<"(";
        }
        const Expr* actual = pin->getActual();
        if(actual){
            outfile << actual->toString();
        }
        if(isNamedConn){
            outfile << ")";
        }
        
    }
        
    void
    OMVisitor::visit(const VecPin* vpin)
    {
        const Expr* actual = vpin->getActual();
        outfile <<"." << vpin->getName() <<"(";
        if(actual){
            outfile << actual->toString();
        }
        outfile << ")";
        /*
        const vector<Pin*>& pins = vpin->getPins();
        const int size = pins.size();
        outfile << ".(" << vpin->getName();
        if(size == 0)
        {
            outfile << ")";
            return;
        }
        outfile << "{";
        const Expr* expr = pins[0]->getActual();
        if(expr){
            outfile << expr->toString();
        }
        for(int i = 1; i < size; ++i)
        {
            outfile << ", ";
            expr = pins[i];
            if(expr){
                outfile << expr->toString();
            }
        }
        outfile << "}";
    */
    }
            

    void
    OMVisitor::visit(const Param* param){
        printInd();
        outfile << "parameter ";
        if(param->isSignSpecified()){
            outfile << "signed ";
        }
        string vt = param->getVarType();
        if(!vt.empty()){
            outfile << vt << " ";
        }
        outfile << param->getIxName().getName() <<" ";
        outfile << "= ";
        outfile << param->getVal()->toString();    
        outfile << ";\n";
    }
    void
    OMVisitor::visit(const VecParam* vparam){
        //const vector<Param*>& params = vparam->getParams();
        const Dimension* dim = vparam->getDim();
        printInd();
        outfile << "parameter ";
        if(vparam->isSignSpecified()){
            outfile << "signed ";
        }
        this->printPackDim(dim);
        outfile << vparam->getName() <<" ";
        outfile << "= ";
        outfile << vparam->getVal()->toString();    
        outfile << ";\n";
    }
    void
    OMVisitor::visit(const LocalParam* lparam){
        printInd();
        outfile << "localparam ";
        if(lparam->isSigned()){
            outfile << "signed ";
        }
        string vt = lparam->getVarType();
        if(!vt.empty()){
            outfile << vt << " ";
        }
        outfile << lparam->getIxName().getName() <<" ";
        outfile << "= ";
        outfile << lparam->getVal()->toString();    
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const VecLocalParam* vlparam){
        const Dimension* dim = vlparam->getDim();
        printInd();
        outfile << "localparam ";
        if(vlparam->isSigned()){
            outfile << "signed ";
        }
        this->printPackDim(dim);
        outfile << vlparam->getName() <<" ";
        outfile << "= ";
        outfile << vlparam->getVal()->toString();    
        outfile << ";\n";
    }
    void
    OMVisitor::visit(const GateInst* ginst){
        const DriveSt* dst = ginst->getDrStrength();
        if(dst){
            outfile << "(" << GetDriveStrName(dst->first);
            outfile << " , " << GetDriveStrName(dst->second) << ");\n";
        }

        const Delay* delay = ginst->getDelay();
        if(delay){
            delay->accept(this);
        }

        outfile << GetGateName(ginst->getType());
        outfile << "(";
        const vector<Pin*> pins = ginst->getPins();
        const int size = pins.size();
        if(!pins.empty())
        {
            const Expr* actual = pins[0]->getActual();    
            if(actual){
                outfile << actual->toString();
            }
        }
        for(int i = 1; i < size; ++i)
        {
            outfile <<", ";
            const Expr* actual = pins[i]->getActual();    
            if(actual){
                outfile << actual->toString();
            }
        }
        outfile <<");\n";
    }
    void
    OMVisitor::visit(const ModInst* minst)
    {
        set<string, compare_str> doneObjs;
        outfile << minst->getModName() << " ";
        vector<NameConn*>& params = const_cast<vector<NameConn*>&>(
            minst->getParams());

        if(!params.empty())
        {
            const int size = params.size();
            outfile << "#( ";
            NameConn* nc = params[0];
            if(nc->first.empty()){
                if(nc->second){
                    outfile << nc->second->toString();
                }
            }
            else 
            {
                outfile << "." << nc->first << "(";
                if(nc->second){
                    outfile << nc->second->toString();
                }
                outfile << ")";
            }

            for(int i = 1; i < size; ++i)
            {
                outfile << ", ";
                nc = params[i];
                if(nc->first.empty()){
                    if(nc->second){
                        outfile << nc->second->toString();
                    }
                }
                else 
                {
                    outfile << "." << nc->first << "(";
                    if(nc->second){
                        outfile << nc->second->toString();
                    }
                    outfile << ")";
                }
            }
            outfile << ") ";
        }

        outfile << minst->getName().toString();
        outfile << "(";
        const vector<Pin*> pins = minst->getPins();
        const int size = pins.size();
        if(!pins.empty())
        {
            doneObjs.insert(pins[0]->getIxName().getName());
            if(pins[0]->isVecBit()){
                const VecPin* vpin = minst->getVecPin(pins[0]->
                getIxName().getName());
                vpin->accept(this);
            }
            else{
                pins[0]->accept(this);
            }
        }
        for(int i = 1; i < size; ++i){
            string name(pins[i]->getIxName().getName());
            if(!name.empty() && doneObjs.find(name) != 
                doneObjs.end()){
                continue;
            }
            doneObjs.insert(name);
            outfile <<", ";
            if(pins[i]->isVecBit()){
                const VecPin* vpin = minst->getVecPin(name);
                vpin->accept(this);
            }
            else{
                pins[i]->accept(this);
            }
        }
        outfile <<");\n";
            
    }

    void
    OMVisitor::visit(const UnaryExpr* expr){
        outfile << expr->toString();
    }

    void
    OMVisitor::visit(const BinExpr* expr){
        outfile << expr->toString();
    }

    void
    OMVisitor::visit(const BitSel* bitsel){
        outfile << bitsel->toString();
    }

    void
    OMVisitor::visit(const PartSel* partsel){
        outfile << partsel->toString();
    }
    void
    OMVisitor::visit(const Concat* conc){
        outfile << conc->toString();
    }

    void
    OMVisitor::visit(const CondExpr* expr){
        outfile << expr->toString();
    }


    void
    OMVisitor::visit(const HierId* hid){
        outfile << hid->toString();
    }


    void
    OMVisitor::visit(const IntConst* i){
        outfile << i->toString();
    }

    void
    OMVisitor::visit(const MultiConcat* mc){
        outfile << mc->toString();
    }

    void
    OMVisitor::visit(const Num* n){
        outfile << n->toString();
    }
    

    void
    OMVisitor::visit(const VString* str){
        outfile << str->toString();
    }
    void
    OMVisitor::visit(const BasedNum* bn){
        outfile << bn->toString();
    }

    void
    OMVisitor::visit(const Reg* reg)
    {
        outfile << "reg " << reg->getIxName().getName();
        if(const Expr* expr = reg->getVal()) {
            outfile << " = " << expr->evaluate();
        }
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const VecReg* vreg)
    {
        outfile << "reg " << vreg->getName();
        //const vector<Var*>& regs = vreg->getVars();
        const Dimension* dim = vreg->getDim();
        this->printPackDim(dim);
        outfile << " " << vreg->getName() <<" ";
        this->printUnpackDim(dim);
        outfile << ";\n";
    }


    void
    OMVisitor::visit(const Integer* int_)
    {
        outfile << "integer " << int_->getIxName().getName();
        if(const Expr* expr = int_->getVal()) {
            outfile << " = " << expr->evaluate();
        }
        outfile << ";\n";
    }


    void
    OMVisitor::visit(const VecInt* vint)
    {
        outfile << "integer " << vint->getName();
        //const vector<Var*>& ints = vint->getVars();
        const Dimension* dim = vint->getDim();
        this->printPackDim(dim);
        outfile << " " << vint->getName() <<" ";
        this->printUnpackDim(dim);
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const Time* time)
    {
        outfile << "time " << time->getIxName().getName();
        if(const Expr* expr = time->getVal()) {
            outfile << " = " << expr->evaluate();
        }
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const VecTime* vtime)
    {
        outfile << "time " << vtime->getName();
        //const vector<Var*>& regs = vtime->getVars();
        const Dimension* dim = vtime->getDim();
        this->printPackDim(dim);
        outfile << " " << vtime->getName() <<" ";
        this->printUnpackDim(dim);
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const Real* real)
    {
        outfile << "real " << real->getIxName().getName();
        if(const Expr* expr = real->getVal()) {
            outfile << " = " << expr->evaluate();
        }
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const VecReal* vreal)
    {
        outfile << "real " << vreal->getName();
        //const vector<Var*>& regs = vreal->getVars();
        const Dimension* dim = vreal->getDim();
        this->printPackDim(dim);
        outfile << " " << vreal->getName() <<" ";
        this->printUnpackDim(dim);
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const RealTime* realt)
    {
        outfile << "realtime " << realt->getIxName().getName();
        if(const Expr* expr = realt->getVal()) {
            outfile << " = " << expr->evaluate();
        }
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const VecRealTime* vrealt)
    {
        outfile << "realtime " << vrealt->getName();
        //const vector<Var*>& regs = vrealt->getVars();
        const Dimension* dim = vrealt->getDim();
        this->printPackDim(dim);
        outfile << " " << vrealt->getName() <<" ";
        this->printUnpackDim(dim);
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const Force* f){
        outfile << "force " << f->getLhs()->toString();
        outfile << " = " << f->getRhs()->toString() <<";\n";
    }
    
    void
    OMVisitor::visit(const Release* r){
        outfile << "release " << r->getVar()->toString() <<";\n";
    }

    void
    OMVisitor::visit(const NamedBlock* nb)
    {
        printInd();
        outfile << "begin : " << nb->getName() << endl;
        this->printScopeDecls(nb);
        printInd();
        outfile <<"end\n";
    }

    void
    OMVisitor::visit(const Block* b)
    {
        printInd();
        outfile << "begin\n";
        const std::vector<Stmt*>& stmts = b->getStmts();
        incInd();
        const int size = stmts.size();
        for(int i = 0; i < size; ++i){
            stmts[i]->accept(this);
        }
        decInd();
        printInd();
        outfile <<"end\n";
    }
        
    void
    OMVisitor::visit(const Fork* f)
    {
        outfile << "fork : " << f->getName() << endl;
        this->printScopeDecls(f);
        outfile <<"join\n";
    }

    void
    OMVisitor::visit(const Disable* d){
        outfile << "disable " << d->getId() << ";\n";
    }

    void
    OMVisitor::visit(const Trigger* t){
        outfile << "-> " << t->getId()->toString() << ";\n";
    }

    void
    OMVisitor::visit(const Forever* f){
        outfile << "forever ";
        const Stmt* st = f->getStmt();
        if(st){
            st->accept(this);
        }
    }
    void
    OMVisitor::visit(const Repeat* r){
        outfile << "repeat (";
        const Expr* expr = r->getExpr();
        if(expr){
            outfile << expr->toString();
        }
        outfile << ") ";
        const Stmt* st = r->getStmt();
        if(st){
            st->accept(this);
        }
    }

    void
    OMVisitor::visit(const While* w){
        outfile << "while (";
        const Expr* expr = w->getExpr();
        if(expr){
            outfile << expr->toString();
        }
        outfile << ") ";
        const Stmt* st = w->getStmt();
        if(st){
            st->accept(this);
        }
    }
    void
    OMVisitor::visit(const For* f)
    {
        outfile << "for (";
        const Expr* lv = f->getLoopVar();
        const Expr* expr = f->getInitExpr();
        if(expr){
            outfile << lv->toString() << " = " << expr->toString() << ";";
        }
        expr = f->getTermExpr();
        if(expr){
            outfile << expr->toString() << ";";
        }
        expr = f->getAssignExpr();
        if(expr){
            outfile << lv->toString() << " = " << expr->toString() << ")\n";
        }
        const Stmt* st = f->getStmt();
        if(st){
            st->accept(this);
        }
        
    }

    void
    OMVisitor::visit(const CaseItem* item)
    {
        const std::vector<Expr*>& label = item->getLabel();
        const Stmt* stmt = item->getStmt();

        const int size = label.size();
        if(item->isDefault()){
            outfile << "default";
        }
        else{
            outfile << label[0]->toString();
        }
        for(int i = 1; i < size; ++i) {
            outfile <<", " << label[i]->toString();
        }
        outfile << ": ";
        if(stmt){
            stmt->accept(this);
        }
    }

    void
    OMVisitor::visit(const Case* c)
    {
        const std::vector<CaseItem*>& caseItems = c->getCaseItems();
        const Expr* selExpr = c->getSelectExpr();
        CaseType type = c->getType();
        if(type == CASE){
            outfile << "case (";
        }
        else if(type == CASEX){
            outfile << "casex (";
        }
        else if(type == CASEZ){
            outfile << "casez (";
        }
        outfile << selExpr->toString();
        outfile << ")\n";
        const int size = caseItems.size();
        for(int i = 0; i < size; ++i){
            caseItems[i]->accept(this);
        }

        outfile <<"endcase\n";
    }

    void
    OMVisitor::visit(const If* if_)
    {
        const Expr* cond = if_->getCondExpr();
        const Stmt* stmt = if_->getStmt();
        const Stmt* elseStmt = if_->getElsePart();

        outfile << "if (" << cond->toString() << ")\n";
        if(stmt){
            stmt->accept(this);
        }
        if(elseStmt)
        {
            outfile << "else \n";
            elseStmt->accept(this);
        }
    }

    void
    OMVisitor::visit(const EventCntrl* e )
    {
        const std::vector<std::pair<EdgeType, Expr*> >& eventExprs = 
        e->getEventExprs();
        const Stmt* stmt = e->getStmt();

        outfile << "@ (";
        if(e->isV2kStyle()){
            outfile << "*\n";
        }
        else
        {
            const int size = eventExprs.size();
            if(eventExprs[0].first == POSEDGE){
                outfile <<"posedge ";
            }
            else if(eventExprs[0].first == NEGEDGE){
                outfile << "negedge ";
            }

            outfile << eventExprs[0].second->toString();
            for(int i = 1; i < size; ++i)
            {
                outfile << " or ";
                if(eventExprs[i].first == POSEDGE){
                    outfile <<"posedge ";
                }
                else if(eventExprs[i].first == NEGEDGE){
                    outfile << "negedge ";
                }

                outfile << eventExprs[i].second->toString();
            }
        }
        outfile <<")";
        if(stmt){
            stmt->accept(this);
        }
        else {
            outfile << ";\n";
        }
    }

    void
    OMVisitor::visit(const EventVar* ev) {
        printInd();
        outfile << "event ";
        outfile << ev->getName();
        outfile << ";\n";
    }    
    void
    OMVisitor::visit(const EventStmt* es)
    {
        printInd();
        const EventVar* var = es->getVar();
        outfile << "event ";
        var->accept(this);
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const DelayStmt* d)
    {
        const Delay* delay = d->getDelay();
        if(delay){
            delay->accept(this);
        }
        const Stmt* st = d->getStmt();
        if(st){
            st->accept(this);
        }
    }

    void
    OMVisitor::visit(const Delay* d)
    {
        printInd();
        const std::vector<DelayVal*>& delays = d->getDelVals();
        const int size1 = delays.size();
        outfile << "#(";
        const std::vector<Expr*>& delExpr = delays[0]->getDelExpr();
        const int size2 = delExpr.size();
        outfile << delExpr[0]->toString();
        for(int j = 1; j < size2; ++j) {
            outfile << ":" << delExpr[j]->toString();
        }

        for(int i = 1; i < size1; ++i) 
        {
            outfile << ", ";
            const std::vector<Expr*>& delExpr = delays[i]->getDelExpr();
            const int size2 = delExpr.size();
            outfile << delExpr[0]->toString();
            for(int j = 1; j < size2; ++j) {
                outfile << ":" << delExpr[j]->toString();
            }
        }

        outfile << ") ";
    }
            

    void
    OMVisitor::visit(const CompAssign* ca)
    {
        const Expr* lexpr = ca->getLhs();
        const Expr* repexpr = ca->getRepeatExpr();
        const EventCntrl* eventCtrl = ca->getEventCtrl();
        const Expr* eventCtrlExpr = ca->getEventCtrlExpr();
        const AssignType type = ca->getAssignType();

        printInd();
        outfile << lexpr->toString();
        if(type == BLOCK){
            outfile << " = ";
        }
        else{
            outfile << " <= ";
        }
        if(repexpr)
        {
            outfile << "repeat (";
            outfile << repexpr->toString() << ") ";
        }
        if(eventCtrl && eventCtrlExpr)
        {
            eventCtrl->accept(this);
            printInd();
            outfile << eventCtrlExpr->toString();
        }
        outfile << ";\n";
    }

    void
    OMVisitor::visit(const Wait* w)
    {
        const Expr* conexpr = w->getCondExpr();
        const Stmt* st = w->getStmt();

        outfile << "wait (";
        if(conexpr){
            outfile << conexpr->toString();
        }
        outfile << ")";
        if(st){
            st->accept(this);
        }
    }
    void 
    OMVisitor::visit(const FuncCall* fcall)
    {
        TaskFuncCall *tfcall = *fcall;
        tfcall->accept(this);
        
    }
        
    void 
    OMVisitor::visit(const TaskFuncCall* tf)
    {
        printInd();
        outfile << " " <<  tf->getName();
        const std::vector<Expr*>& args  = tf->getArgs();
        if(args.empty())
        {
            outfile << ";\n";
            return;
        }

        outfile << "(";
        if(args[0]) {
            outfile <<  args[0]->toString();
        }
        const int size = args.size();
        for(int i = 1; i < size; ++i) {
            outfile << ", ";
            if(args[i]) {
                outfile <<  args[i]->toString();
            }
        }

        outfile <<");\n";
    }

    void
    OMVisitor::visit(const Task* t)
    {
        set<string, compare_str> doneObjs;
        outfile <<"\n";
        printInd();
        outfile << "task ";
        if(t->isAuto()){
            outfile << "automatic ";
        }
        outfile << t->getName() << ";\n";
        const vector<Port*>& ports = t->getPorts();
        const int size = ports.size();
        for(int i = 0; i < size; ++i)
        {
            Port* port = ports[i];
            IxName ixName = port->getIxName();
            if(port->isVecBit())
            {
                if(doneObjs.find(ixName.getName()) != doneObjs.end()){
                    continue;
                }
                doneObjs.insert(ixName.getName());
                const VecPort* vecPort = static_cast<const VecPort*>(
                port->getVecCntv());
                vecPort->accept(this);
            }
            else {
                port->accept(this);
            }
            //printInd();
        }

        this->printScopeDecls(t);
        //const Stmt* st = t->getTaskStmt();
        //if(st){
        //    st->accept(this);
        //}
        printInd();
        outfile <<"endtask\n";
    }

    void
    OMVisitor::visit(const Func* f)
    {
        set<string, compare_str> doneObjs;
        printInd();
        outfile << "function ";
        if(f->isAuto()){
            outfile << "automatic ";
        }
        if(f->isSigned()){
            outfile << "signed ";
        }
        const Range* range = f->getRange();
        const std::string& rettype = f->getRetType();
        if(range)
        {
            outfile << "[" << range->getLExpr()->toString();
            outfile << ":" << range->getRExpr()->toString();
            outfile << "]";
        }
        else {
            outfile << rettype << " ";
        }
        outfile << f->getName() <<";\n";
        const vector<Port*>& ports = f->getPorts();
        const int size = ports.size();
        for(int i = 0; i < size; ++i)
        {
            Port* port = ports[i];
            IxName ixName = port->getIxName();
            if(port->isVecBit())
            {
                if(doneObjs.find(ixName.getName()) != doneObjs.end()){
                    continue;
                }
                doneObjs.insert(ixName.getName());
                const VecPort* vecPort = static_cast<const VecPort*>(
                port->getVecCntv());
                vecPort->accept(this);
            }
            else {
                port->accept(this);
            }
        }
        this->printScopeDecls(f);
    //    const Stmt* st = f->getFuncStmt();
    //    if(st){
    //        st->accept(this);
    //    }
        printInd();
        outfile << "endfunction\n";
        
    }

    void
    OMVisitor::visit(const Always* a)
    {
        printInd();
        outfile << "always ";
        const Stmt* st = a->getStmt();
        if(st){
            st->accept(this);
        }
    }

    void
    OMVisitor::visit(const Initial* a)
    {
        printInd();
        outfile << "initial ";
        incInd();
        const Stmt* st = a->getStmt();
        if(st){
            st->accept(this);
        }
        decInd();
    }

    void
    OMVisitor::visit(const ProcAssign* pa)
    {
        const Delay* ldelay = pa->getLDelay();
        const Delay* rdelay = pa->getRDelay();
        const AssignType type = pa->getAssignType();

        const Expr* lexpr = pa->getLhs();
        const Expr* rexpr = pa->getRhs();
        printInd();
        if(ldelay){
            ldelay->accept(this);
        }
        outfile << lexpr->toString();
        if(type == BLOCK){
            outfile << " = ";
        }
        else{
            outfile << " <= ";
        }
        if(rdelay){
            rdelay->accept(this);
        }
        outfile << rexpr->toString() << ";\n";
    }

    void
    OMVisitor::visit(const ProcContAssign* pca)
    {
        const Expr* lexpr = pca->getLhs();
        const Expr* rexpr = pca->getRhs();
        outfile << "assign " << lexpr->toString();
        outfile << " = ";
        outfile << rexpr->toString() << ";\n";
    }

    void
    OMVisitor::visit(const ContAssign* ca)
    {
        printInd();
        const Expr* lexpr = ca->getLhs();
        const Expr* rexpr = ca->getRhs();
        const DriveSt* dst = ca->getStrength();
        const Delay* del = ca->getDelay();
        
        outfile << "assign ";
        if(dst)
        {
            outfile << "(" << GetDriveStrName(dst->first);
            outfile << " , " << GetDriveStrName(dst->second) << ") ";
        }
        if(del){
            del->accept(this);
        }
        outfile <<" " << lexpr->toString();
        outfile <<" = " << rexpr->toString() <<";\n";
    }

    void
    OMVisitor::visit(const Defparam* d)
    {
        const Expr* lexpr = d->getLhs();
        const Expr* rexpr = d->getRhs();
        const Range* range = d->getRange();
        printInd();
        outfile <<"defparam ";
        if(range)
        {
            outfile << "[" << range->getLExpr()->toString();
            outfile << ":" << range->getRExpr()->toString();
            outfile << "]";
        }
        outfile << lexpr->toString() << " ";
        outfile << " = " << rexpr->toString() << ";\n";
    }
        

    void
    OMVisitor::visit(const SpecifyBlock* s)
    {
        printInd();
        outfile <<"specify\n";
        incInd();
        const vector<Specparam*>& sps = s->getSpecparams();
        int size = sps.size();
        for(int i = 0; i < size; ++i){
            sps[i]->accept(this);
        }

        const vector<SpecifyPathBase*>&  spbs = s->getSpecPaths();
        size = spbs.size();    
        for(int i = 0; i < size; ++i){
            spbs[i]->accept(this);
        }

        const vector<SpecTask*>& st = s->getSpecTasks();
        size = st.size();

        for(int i = 0; i < size; ++i){
            st[i]->accept(this);
        }
        decInd();
        printInd();
        outfile << "endspecify\n";
    }

    void
    OMVisitor::visit(const Specparam* s)
    {
        printInd();
        outfile << "specparam "  << s->getName() << " = ";
        vector<Expr*> val = s->getVal();
        outfile << val[0]->toString();

        const int size = val.size();
        for(int i = 1; i < size; ++i) {
            outfile <<":" << val[i]->toString();
        }
        outfile <<";\n";
    }

    void
    OMVisitor::visit(const SpecifyPath* s)
    {
        const vector<Expr*>& srcs = s->getSrcs();
        int size = srcs.size();
        printInd();
        if(s->isConditional()){
            outfile << "if(" << s->getIfExpr()->toString() <<") ";
        }
        outfile << "(";
        outfile << srcs[0]->toString();
        for(int i = 1; i < size; ++i){
            outfile << ", " << srcs[i]->toString();
        }

        outfile << s->getPolarity();
        if(s->getConnType() == PCONN){
            outfile << "=>";
        }
        else {
            outfile <<"*>";
        }
        const vector<Expr*>& dests = s->getDestIds();
        size = dests.size();
        outfile <<" " << dests[0]->toString();
        for(int i = 1; i < size; ++i){
            outfile << ", " << dests[i]->toString();
        }
        outfile << ")";

        outfile << " = ";
        const vector<DelayVal*>& delay = s->getDelay();
        const int size1 = delay.size();
        
        if(size1 > 1){
            outfile <<" (";
        }
        const vector<Expr*>& delExpr = delay[0]->getDelExpr();
        int size2 = delExpr.size();
        outfile << delExpr[0]->toString();
        for(int j = 1; j < size2; ++j) {
            outfile << ":" << delExpr[j]->toString();
        }
        for(int i = 1; i < size1; ++i)
        {
            outfile << ", ";
            const vector<Expr*>& delExpr = delay[i]->getDelExpr();
            int size2 = delExpr.size();
            outfile << delExpr[0]->toString();
            for(int j = 1; j < size2; ++j) {
                outfile << ":" << delExpr[j]->toString();
            }
        }
        if(size1 > 1){
            outfile <<")";
        }

        outfile <<";\n";
        
    }

    void
    OMVisitor::visit(const SpecifyEdgePath* s)
    {
        printInd();
        if(s->isConditional()){
            outfile << "if(" << s->getIfExpr()->toString() <<") ";
        }
    
        outfile << "(";
        if(s->getEdgeType() == POSEDGE){
            outfile <<"posedge ";
        }
        else{
            outfile <<"negedge ";
        }
        const vector<Expr*>& srcs = s->getSrcs();
        int size = srcs.size();
        outfile << srcs[0]->toString();
        for(int i = 1; i < size; ++i){
            outfile << ", " << srcs[i]->toString();
        }

        outfile << s->getPolarity() << " ";
        if(s->getConnType() == PCONN){
            outfile << "=>";
        }
        else {
            outfile <<"*>";
        }

        if(const Expr* destId = s->getDestId())
        {
            outfile << destId->toString();
            outfile << ")" ;
            return;
        }
        else
        {
            const pair<Expr*, Expr*>& dep = s->getDestExprPair();
            outfile << " (" << dep.first->toString();
            PolOp p = s->getPolOp();
            if(p == POPOS){
                outfile << " +: ";
            }
            else if(p == PONEG){
                outfile << " -: ";
            }
            else {
                outfile << " : ";
            }
            outfile << dep.second->toString() << ")";
        }
        outfile << ")";
        outfile << " = ";
        const vector<DelayVal*>& delay = s->getDelay();
        
        const vector<Expr*>& delExpr = delay[0]->getDelExpr();
        size = delExpr.size();
        if(size > 1){
            outfile <<" (";
        }
        outfile << delExpr[0]->toString();
        for(int j = 1; j < size; ++j) {
            outfile << ":" << delExpr[j]->toString();
        }
        if(size > 1){
            outfile <<" )";
        }

        outfile <<";\n";
        
    }

    void
    OMVisitor::visit(const UDP* udp)
    {
        printInd();
        outfile <<"primitive " << udp->getName() << "(";
        const Port* out = udp->getOutPort();
        outfile << out->getIxName().getName();
        outfile <<", ";

        const vector<Port*>& inports = udp->getInPorts();
        const int size = inports.size();

        outfile << inports[0]->getIxName().getName();

        for(int i = 1; i < size; ++i) {
            outfile <<", " << inports[i]->getIxName().getName();
        }

        outfile << ");\n";
        outfile << "output " << out->getIxName().getName() << ";\n";
        //out->accept(this);
        for(int i = 0; i < size; ++i) 
        {
            outfile << "input ";
            outfile  << inports[i]->getIxName().getName() << ";\n";
            //inports[i]->accept(this);
        }
        const Expr* initval = udp->getInitVal();
        if(initval)
        {
            printInd();
            outfile << "initial " << out->toString() 
                << " = " << initval->toString() << ";\n";
        }

        const UDPTable& table = udp->getUDPTable();
        this->printUDPTable(&table, udp);
        outfile <<"endprimitive\n";
    }

    void
    OMVisitor::visit(const UDPInst* uinst)
    {
        useNameConn = false;
        printInd();
        outfile << uinst->getUDPName() << " ";
        outfile << uinst->getName().toString() << "(";
        const std::vector<Pin*>& pins = uinst->getPins();
        
        const int size = pins.size();
        pins[0]->accept(this);
        for(int i = 1; i < size; ++i)
        {
            outfile << ", ";
            pins[i]->accept(this);
        }
        outfile << ");\n";
        useNameConn = true;
    }

    void
    OMVisitor::visit(const Genvar* g)
    {
        printInd();
        outfile << "genvar " << g->toString() << ";\n";
    }

    void
    OMVisitor::visit(const GenerateCond* gc)
    {
        printInd();
        outfile << "if(" << gc->condexpr->toString() << ")\n";
        gc->ifpart->accept(this);
        if(gc->elsepart)
        {
            printInd();
            outfile << "else\n";
            gc->elsepart->accept(this);
        }
    }

    void
    OMVisitor::visit(const GenerateCase* gc)
    {
        printInd();
        outfile << "case(" << gc->select->toString() << ")\n";
        const int size = gc->caseitems.size();

        for(int i = 0; i < size; ++i)
        {
            const int labels = gc->caseitems[i].first.size();
            for(int j = 0; j < labels; ++j)
            {
                printInd();
                outfile << gc->caseitems[i].first[j]->toString() << ":\n";
            }
            incInd();
            gc->caseitems[i].second->accept(this);
            decInd();
        }
        if(gc->defstmt)
        {
            printInd();
            outfile << "default: \n";
            incInd();
            gc->defstmt->accept(this);
            decInd();
        }
        printInd();
        outfile << "endcase\n";
    }

    void
    OMVisitor::visit(const GenerateBlock* gb)
    {
        printInd();
        outfile << "begin";
        if(!gb->getName().empty()) {
            outfile << ": " << gb->getName() << endl;
        }
        incInd();
        const int size = gb->gitems.size();
        for(int i = 0; i < size; ++i){
            gb->gitems[i]->accept(this);
        }
        decInd();
        printInd();
        outfile << "end\n";
    }

    void
    OMVisitor::visit(const GenerateLoop* gl)
    {
        printInd();
        outfile << "for(" << gl->loopvar->toString() << " = ";
        outfile << gl->initexpr->toString() << ";";
        outfile << gl->termexpr->toString() << ";";
        outfile << gl->loopvar->toString() << " = ";
        outfile << gl->assignexpr->toString() << ")\n";
        //outfile << " begin : " << gl->name << endl; 
        //incInd();
        gl->gitem->accept(this);
        //decInd();
        //printInd();
        //outfile << "end\n";
    }

    void
    OMVisitor::visit(const GenDecl* gd) {
        this->printGenerateDecls(gd->getGenDecl());
    }

    void
    OMVisitor::visit(const GenerateStmt* gs)
    {
        printInd();
        outfile <<"generate\n";
        const vector<Genvar*>& genvars = gs->getGenvars();
        incInd();
        int size = genvars.size();
        for(int i = 0; i < size; ++i) {
            genvars[i]->accept(this);
        }
        const GenDecl* gdecl = gs->getDecl();
        this->printGenerateDecls(gdecl->getGenDecl());
        const vector<GenerateCond*>& condgs = gs->getCondGens();
        size = condgs.size();
        for(int i = 0; i < size; ++i) {
            condgs[i]->accept(this);
        }
        const vector<GenerateCase*>& casesg = gs->getCaseGens();
        size = casesg.size();
        for(int i = 0; i < size; ++i) {
            casesg[i]->accept(this);
        }
        const vector<GenerateLoop*>& loopsg = gs->getLoopGens();
        size = loopsg.size();
        for(int i = 0; i < size; ++i) {
            loopsg[i]->accept(this);
        }
        const vector<GenerateBlock*>& blocksg = gs->getBlockGens();
        size = blocksg.size();
        for(int i = 0; i < size; ++i) {
            blocksg[i]->accept(this);
        }
        decInd();
        printInd();
        outfile << "endgenerate\n";
    }
            
    
    void
    OMVisitor::printGenerateDecls(const ModGenDecl* gis)
    {
        if(!gis){
            return;
        }

        int size = gis->genvars.size();
        for(int i = 0; i < size; ++i){
            gis->genvars[i]->accept(this);
        }
        size = gis->variables.size();
        for(int i = 0; i < size; ++i){
            printInd();
            gis->variables[i]->accept(this);
        }

        size = gis->nets.size();
        for(int i = 0; i < size; ++i){
            printInd();
            gis->nets[i]->accept(this);
        }
        
        size = gis->ginsts.size();
        for(int i = 0; i < size; ++i){
            printInd();
            gis->ginsts[i]->accept(this);
        }

        size = gis->minsts.size();
        for(int i = 0; i < size; ++i){
            printInd();
            gis->minsts[i]->accept(this);
        }

        size = gis->uinsts.size();
        for(int i = 0; i < size; ++i){
            printInd();
            gis->uinsts[i]->accept(this);
        }

        size = gis->alwaysBlocks.size();
        for(int i = 0; i < size; ++i){
            printInd();
            gis->alwaysBlocks[i]->accept(this);
        }

        size = gis->initials.size();
        for(int i = 0; i < size; ++i){
            printInd();
            gis->initials[i]->accept(this);
        }


        size = gis->defparams.size();
        for(int i = 0; i < size; ++i){
            gis->defparams[i]->accept(this);
        }

        size = gis->contassigns.size();
        for(int i = 0; i < size; ++i){
            gis->contassigns[i]->accept(this);
        }

        size = gis->events.size();
        for(int i = 0; i < size; ++i){
            gis->events[i]->accept(this);
        }

        size = gis->tasks.size();
        for(int i = 0; i < size; ++i){
            gis->tasks[i]->accept(this);
        }

        size = gis->funcs.size();
        for(int i = 0; i < size; ++i){
            gis->funcs[i]->accept(this);
        }

    }
    void
    OMVisitor::printUDPTable(const UDPTable* table, const UDP* udp)
    {
        printInd();
        outfile << "table\n";
        incInd();
        const int size = table->size();
        for(int i = 0; i < size; ++i)
        {
            printInd();
            const TableEntry* tentry = (*table)[i];
            const int insize = tentry->inval.size();
            for(int j = 0; j < insize; ++j) {
                outfile << tentry->inval[j] << "  ";
            }
            if(udp->isSequential()){
                outfile << ":  " << tentry->currState << "  ";
            }
            outfile << ":  " << tentry->outval << ";\n";
        }
        outfile << "endtable\n";
    }

    void
    OMVisitor::printPackDim(const Dimension* dim) 
    {
        const int size = dim->packDim.size();
        for(int i = 0; i < size; ++i){
            outfile <<"[";
            outfile << dim->packDim[i]->getLExpr()->toString();
            outfile << ":";
            outfile << dim->packDim[i]->getRExpr()->toString();
            outfile << "]";
        }

    }
        
    void
    OMVisitor::printUnpackDim(const Dimension* dim)
    {
        const int size = dim->unpackDim.size();
        for(int i = 0; i < size; ++i){
            outfile <<"[";
            outfile << dim->unpackDim[i]->getLExpr()->toString();
            outfile << ":";
            outfile << dim->unpackDim[i]->getRExpr()->toString();
            outfile << "]";
        }
    }

    void
    OMVisitor::writeDesign()
    {
        const vector<File*>& files = GRoot->getFiles();
        int filec = files.size();

        for(int i = 0; i < filec; ++i)
        {
            File* file = files[i];
            const vector<Module*>& modules = file->getModules();
            const int modc = modules.size();
            for(int j = 0; j < modc; ++j){
                modules[j]->accept(this);
            }

            const vector<UDP*>& udps = file->getUDPs();
            const int udpc = udps.size();
            for(int j = 0; j < udpc; ++j){
                udps[j]->accept(this);
            }
        }
        const vector<File*>& libfiles = GRoot->getLibFiles();
        filec = libfiles.size();

        for(int i = 0; i < filec; ++i)
        {
            File* file = libfiles[i];
            const vector<Module*>& modules = file->getModules();
            const int modc = modules.size();
            for(int j = 0; j < modc; ++j){
                modules[j]->accept(this);
            }
            const vector<UDP*>& udps = file->getUDPs();
            const int udpc = udps.size();
            for(int j = 0; j < udpc; ++j){
                udps[j]->accept(this);
            }
        }

        map<string, Elaborator::ElabInfo*, compare_str>::const_iterator 
            elabInfo = Elaborator::instance()->elabInfos.begin();
        map<string, Elaborator::ElabInfo*, compare_str>::const_iterator 
            eelabInfo = Elaborator::instance()->elabInfos.end();

        for(; elabInfo != eelabInfo; ++elabInfo){
            elabInfo->second->newmod->accept(this);
        }
    }

    void
    OMVisitor::printModHead(const Module* module)
    {
        set<string, compare_str> doneObjs;
        outfile <<"module " << module->getName() <<"(";
        //incInd();
        const vector<Port*>& ports = module->getPorts();
        int size = ports.size();
        if(size == 0) {
            outfile <<");\n";
            return;
        }
        //printInd();
        Port* port = ports[0];
        IxName ixName = port->getIxName();
        outfile << ixName.getName();
        doneObjs.insert(ixName.getName());
        for(int i = 1; i < size; ++i)
        {
            port = ports[i];
            string name = port->getIxName().getName();
            if(doneObjs.find(name) != doneObjs.end()){
                continue;
            }
            outfile <<", ";
            doneObjs.insert(name);
            if((signed)(indent + name.size()) > linewidth)
            {
                outfile <<"\n";
                printInd();
            }
            outfile << name;
        }
        outfile <<");\n";
    }

    void
    OMVisitor::printScopeDecls(const Scope* sc)
    {
        outfile << "\n";
        outfile << "// Variable declaration.\n";
        set<string, compare_str> doneObjs;
        
        int size = 0;
        const std::vector<Reg*>& regs = sc->getRegs();
        size = regs.size();
        //if(size > 0){
        //    printInd();
        //}
        for(int i = 0; i < size; ++i)
        {
            if(regs[i]->isVecBit()){
                if(doneObjs.find(regs[i]->getIxName().getName()) !=
                    doneObjs.end()){
                    continue;
                }
                doneObjs.insert(regs[i]->getIxName().getName());
            }
            printInd();
            outfile << "reg ";
            if(regs[i]->isSigned()){
                outfile << "signed ";
            }
            this->printScopeVars(regs[i]);
        }

        const std::vector<Integer*>& ints = sc->getIntegers();
        size = ints.size();

        for(int i = 0; i < size; ++i)
        {
            if(ints[i]->isVecBit()){
                if(doneObjs.find(ints[i]->getIxName().getName()) !=
                    doneObjs.end()){
                    continue;
                }
                doneObjs.insert(ints[i]->getIxName().getName());
            }
            printInd();
            outfile << "integer ";
            this->printScopeVars(ints[i]);
        }
        const std::vector<Time*>& times = sc->getTimes();
        size = times.size();

        if(size > 0){
            printInd();
        }
        for(int i = 0; i < size; ++i)
        {
            if(times[i]->isVecBit()){
                if(doneObjs.find(times[i]->getIxName().getName()) !=
                    doneObjs.end()){
                    continue;
                }
                doneObjs.insert(times[i]->getIxName().getName());
            }
            outfile << "time ";
            this->printScopeVars(times[i]);
        }
        const std::vector<Real*>& reals = sc->getReals();
        size = reals.size();

        if(size > 0){
            printInd();
        }
        for(int i = 0; i < size; ++i)
        {
            if(reals[i]->isVecBit()){
                if(doneObjs.find(reals[i]->getIxName().getName()) !=
                    doneObjs.end()){
                    continue;
                }
                doneObjs.insert(reals[i]->getIxName().getName());
            }
            outfile << "real ";
            this->printScopeVars(reals[i]);
        }

        const std::vector<RealTime*>& realts = sc->getRealTimes();
        size = realts.size();

        if(size > 0){
            printInd();
        }
        for(int i = 0; i < size; ++i)
        {
            if(realts[i]->isVecBit()){
                if(doneObjs.find(realts[i]->getIxName().getName()) !=
                    doneObjs.end()){
                    continue;
                }
                doneObjs.insert(realts[i]->getIxName().getName());
            }
            outfile << "realtime ";
            this->printScopeVars(realts[i]);
        }

        const std::vector<EventVar*>& evars = sc->getEventVars();
        for(int i = 0; i < evars.size(); ++i) {
            evars[i]->accept(this);
        }    

        const std::vector<Stmt*>& stmts = sc->getStmts();
        size = stmts.size();
        for(int i = 0; i < size; ++i){
            stmts[i]->accept(this);
        }
    }

    void
    OMVisitor::printScopeVars(const Var* var)
    {
        const bool isBit = var->isVecBit();
        if(isBit)
        {
            const VecCntv* veccntv = var->getVecCntv();
            const Dimension* dim = veccntv->getDim();
            this->printPackDim(dim);
            outfile << " " << veccntv->getName() <<" ";
            this->printUnpackDim(dim);
            outfile << ";\n";
        }
        else {
            outfile << " " << var->getIxName().getName() << ";\n";
        }
    }
            
        
        
        
}

