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

#include <set>
#include <sstream>
#include "syvisitor.h"

#include "sycontainer.h"
#include "sycntvs.h"
#include "syexpr.h"
#include "systmt.h"
#include "syconst.h"
#include "symsghandler.h"

static const short INDWIDTH = 4;
static const short LINEWIDTH = 80;

using std::string;
using std::set;
using std::ofstream;
using std::ostringstream;
using std::pair;

namespace Synth
{
    SyVisitor::SyVisitor() :
        col(0), indwidth(INDWIDTH),
        linewidth(LINEWIDTH), indent(0), 
        outfile(0) 
    {
    }

    SyVisitor::SyVisitor(const string& filename) :
        col(0), indwidth(INDWIDTH),
        linewidth(LINEWIDTH), indent(0)
    {
        outfile = new ofstream();
        outfile->open(filename.c_str(), std::ios_base::trunc);

        if(outfile->bad())
        {
            delete outfile;
            outfile = 0;
            SyMsgHandler::instance()->print(6, filename.c_str());
        }
    }

    SyVisitor::~SyVisitor()
    {
        if(outfile){
            outfile->close();
            delete outfile;
        }
    }

    void
        SyVisitor::printInd()
        {
            if((indent*indwidth ) <= col){
                return;
            }
            if(outfile){
                (*outfile) << string(indent*indwidth, ' ');
            }
            else {
                cout << string(indent*indwidth, ' ');
            }
            col += indent*indwidth;
        }

    void
        SyVisitor::printLine()
        {
            if(outfile){
                (*outfile) << "\n";
            }
            else {
                cout << "\n";
            }
            col = 0;
        }

    void
        SyVisitor::print(const string& str)
        {
            this->printInd();
            if((col + str.size()) > (unsigned int)linewidth)
            {
                printLine();
                printInd();
            }
            if(outfile){
                (*outfile) << str;
            }
            else {
                cout << str;
            }
            col += str.size();
        }

    void
        SyVisitor::incIndent(short count){
            indent += count;
        }

    void
        SyVisitor::decIndent(short count)
        {
            indent -= count;
            if(indent < 0){
                indent = 0;
            }
        }


    void
        SyVisitor::visit(const SyModule* mod)
        {
            this->printModCommon(mod);
            this->printLine();
            this->print("endmodule");
            this->printLine();

        }
    

    void
        SyVisitor::visit(const SyBModule* mod)
        {
            this->printModCommon(mod);
            AcceptListOfObjs<SyContAssign, SyVisitor>( this, mod->getContAssigns());
            AcceptListOfObjs<SyAlways, SyVisitor>(this, mod->getAlwaysBlocks());
            AcceptListOfObjs<SyTask, SyVisitor>(this, mod->getTasks());
            AcceptListOfObjs<SyFunc, SyVisitor>(this, mod->getFuncs());
            this->printLine();
            this->print("endmodule");
            this->printLine();

        }
    void
        SyVisitor::visit(const SyScPort *port)
        {
            this->printLine();
            this->print(DirTypeToString(port->getDir()));
            this->print(" ");
            this->print(NetTypeToString(port->getNetType()));
            this->print(" ");
            this->print(port->getIxName().getName());
            this->print(";");
        }
    void
        SyVisitor::visit(const SyVPort *port)
        {
            ostringstream str;
            this->printLine();
            this->print(DirTypeToString(port->getDir()));
            this->print(" ");
            this->print(NetTypeToString(port->getNetType()));
            this->print(" ");
            str << "[" << port->getMsb() << ":";
            str << port->getLsb() << "]";
            this->print(str.str());
            this->print(port->getName());
            this->print(";");
        }

    void
        SyVisitor::visit(const SyScNet *net)
        {
            this->printLine();
            this->print(NetTypeToString(net->getNetType()));
            this->print(" ");
            this->print(net->getIxName().getName());
            this->print(";");
        }

    void
        SyVisitor::visit(const SyVNet *net)
        {
            ostringstream str;
            this->printLine();
            this->print(NetTypeToString(net->getNetType()));
            this->print(" ");
            str << "[" << net->getMsb() << ":";
            str << net->getLsb() << "]";
            this->print(str.str());
            this->print(net->getName());
            this->print(";");
        }

    void
        SyVisitor::visit(const SyScReg *reg)
        {
            this->printLine();
            this->print("reg ");
            if(reg->isSigned()){
                this->print("signed ");
            }
            this->print(reg->getIxName().getName());
            this->print(";");
        }

    void
        SyVisitor::visit(const SyVReg *reg)
        {
            ostringstream str;
            this->printLine();
            this->print("reg ");
            if(reg->isSigned()){
                this->print("signed ");
            }
            str << "[" << reg->getMsb() << ":";
            str << reg->getLsb() << "] ";
            this->print(str.str());
            this->print(reg->getName());
            this->print(";");
        }

    void
        SyVisitor::visit(const SyMemory *mem)
        {
            visit(static_cast<const SyVReg*>(mem));
        }

    void
        SyVisitor::visit(const SyXOrZ *xorz) {
            this->print(xorz->toString());
        }    

    void
        SyVisitor::visit(const SyModInst *minst)
        {
            this->printLine();
            this->printInd();

            this->print(minst->getMasterName());
            this->print(" ");
            this->print(minst->getIxName().toString());
            this->print("(");
            const SyList<SyScPin*>& pins = minst->getPins();
            SyNode<SyScPin*>* node = pins.head;
            this->incIndent();
            if(node)
            {
                //this->print("\n");
                if(node->val->isVecBit())
                {
                    const SyVPin *vpin = minst->getVecPin(node->val->getIxName().getName());
                    
                    while(node->next)
                    {
                        if(node->next->val->getIxName().getName() != vpin->getName()){
                            break;
                        }
                        node = node->next;
                    }
                    vpin->accept(this);
                }
                else
                {
                    node->val->accept(this);
                }
            }    
            node =  node ? node->next : 0;
            
            for(; node; node = node->next)
            {
                this->print(", ");
                if(node->val->isVecBit())
                {
                    const SyVPin *vpin = minst->getVecPin(node->val->getIxName().getName());
                    
                    while(node->next)
                    {
                        if(node->next->val->getIxName().getName() 
                            != vpin->getName()){
                            break;
                        }
                        node = node->next;
                    }
                    vpin->accept(this);
                }
                else
                {
                    node->val->accept(this);
                }
            }    

            //this->print("\n");
            this->decIndent();
            this->printInd();
            this->print(");");
        }

    void
        SyVisitor::visit(const SyPrimInst *pinst)
        {
            this->printLine();
            this->printInd();

            this->print(pinst->getGateName());
            this->print("(");

            const SyList<SyScPin*>& pins = pinst->getPins();
            SyNode<SyScPin*>* node = pins.head;
            const SyExpr *actual = node->val->getActual();
            if(actual){
                this->print(actual->toString());
            }

            for(node = node->next; node; node = node->next)
            {
                this->print(", ");
                const SyExpr *actual = node->val->getActual();
                if(actual){
                    this->print(actual->toString());
                }
            }
            this->print(");");

        }

    void
        SyVisitor::visit(const SyScPin *pin)
        {
            this->print(".");
            this->print(pin->getIxName().toString());
            const SyExpr *actual = pin->getActual();
            this->print("(");
            if(actual){
                this->print(actual->toString());
            }
            this->print(")");
        }    

    void
        SyVisitor::visit(const SyVPin *pin)
        {
            this->print(".");
            this->print(pin->getName());
            const SyExpr *actual = pin->getActual();
            if(actual){
                this->print("(");
                this->print(actual->toString());
            }
            this->print(")");
        }    
    void
        SyVisitor::visit(const SyContAssign *assign)
        {
            this->printLine();
            this->printInd();

            this->print("assign ");
            this->print(assign->getLExpr()->toString());
            this->print(" = ");
            this->print(assign->getRExpr()->toString());
            this->print(";");
        }

    void
        SyVisitor::visit(const SyNonBlockAssign *nbassign)
        {
            this->printLine();
            this->printInd();
            this->print(nbassign->getLExpr()->toString());
            this->print(" <= ");
            this->print(nbassign->getRExpr()->toString());
            this->print(";");
        }


    void
        SyVisitor::visit(const SyBlockAssign *bassign)
        {
            this->printLine();
            this->printInd();
            this->print(bassign->getLExpr()->toString());
            this->print(" = ");
            this->print(bassign->getRExpr()->toString());
            this->print(";");
        }

    void
        SyVisitor::visit(const SyIf* if_)
        {
            this->printLine();
            this->print("if(");
            this->print(if_->getCond()->toString());
            this->print(") begin");
            this->printLine();
            this->incIndent();
            AcceptListOfObjs<SyStmt, SyVisitor>(this, if_->getThenPart());
            this->decIndent();
            this->printLine();
            this->print("end");
            if(!if_->getElsePart().empty())
            {
                this->printLine();
                this->print("else begin");
                this->printLine();
                this->incIndent();
                AcceptListOfObjs<SyStmt, SyVisitor>(this, if_->getElsePart());
                this->decIndent();
                this->printLine();
                this->print("end");
            }
        }


    void
        SyVisitor::visit(const SyAlways *always)
        {
            this->printLine();
            this->printInd();
            this->print("always ");
            this->printEventCtrl(always);
        }

    void
        SyVisitor::visit(const SyEventCtrl *eventCtrl)
        {
            this->printEventCtrl(eventCtrl);
        }

    void
        SyVisitor::visit(const SyCase *case_)
        {
            this->printLine();
            this->print("case(");
            this->print(case_->getSelect()->toString());
            this->print(")");

            const SyList<SyCaseItem*>& caseItems = case_->getCaseItems();

            for(SyNode<SyCaseItem*>* node = caseItems.head; node; 
                    node = node->next)
            {
                this->printLine();
                if(dynamic_cast<const SyVString*>(node->val->getLabel())){
                    this->print("default");
                }
                else{
                    this->print(node->val->getLabel()->toString());
                }
                this->print(": begin");
                this->printLine();
                this->incIndent();
                AcceptListOfObjs<SyStmt, SyVisitor>(this, node->val->getStmts());
                this->decIndent();
                this->printLine();
                this->print("end");
            }
            this->printLine();
            this->print("endcase");
        }

    void
        SyVisitor::visit(const SyTask *task)
        {
            this->printLine();
            this->printInd();
            this->print("task ");
            this->print(task->getName());
            this->printHeaderPorts(task);
            AcceptVectorVisitor<SyScPort, SyVPort>(this, task->getPorts(),
                task->getName());
            this->incIndent();
            this->printLine();
            this->print("begin");
            AcceptListOfObjs<SyStmt, SyVisitor>(this, task->getStmts());
            this->printLine();
            this->decIndent();
            this->print("end");
            this->printLine();
            this->print("endtask");
        }

    void
        SyVisitor::visit(const SyFunc *func)
        {
            this->printLine();
            this->print("function ");
            if(func->isSigned()){
                this->print("signed ");
            }
            if(const SyVReg *vreg = dynamic_cast<const SyVReg*>(
                        func->getRetCntv()))
            {
                ostringstream str;
                str << "[" << vreg->getMsb() << ":";
                str << vreg->getLsb() << "] ";
                this->print(str.str());
            }
            this->print(func->getName());
            this->printHeaderPorts(func);
            //AcceptVectorVisitor<SyScPort, SyVPort>(this, func->getPorts());
            this->incIndent();
            this->printLine();
            this->print("begin");
            AcceptListOfObjs<SyStmt, SyVisitor>(this, func->getStmts());
            this->printLine();
            this->print("end");
            this->decIndent();
            this->printLine();
            this->print("endfunction");
        }




    void
        SyVisitor::visit(const SyTaskCall *tcall)
        {
            this->printLine();
            this->print(tcall->getTask()->getName());
            const vector<SyExpr*>& args = tcall->getArgs();
            this->print("(");
            bool first = true;
            for(unsigned int i = 0; i < args.size(); ++i){
                if(!first){
                    this->print(", ");
                }
                else {
                    first = false;
                }
                this->print(args[i]->toString());
            }
            this->print(")");
            this->print(";");
        }

    void
        SyVisitor::visit(const SyBlockCompAssign* bcassign)
        {
            const SyExpr *lexpr = bcassign->getLExpr();
            const SyList<pair<SyEventCtrlList, SyExpr*> > & eclExprs =
                bcassign->getEventExprs();

            for(SyNode<pair<SyEventCtrlList, SyExpr*> >* node = eclExprs.head;
                    node; node = node->next)
            {
                this->printLine();
                this->print(lexpr->toString());
                this->print(" = ");
                this->printEventCtrlList(&node->val.first);
                this->print(node->val.second->toString());
                this->print(";");
            }
        }

    void
        SyVisitor::visit(const SyNonBlockCompAssign* nbcassign)
        {
            const SyExpr *lexpr = nbcassign->getLExpr();
            const SyList<pair<SyEventCtrlList, SyExpr*> > & eclExprs =
                nbcassign->getEventExprs();

            for(SyNode<pair<SyEventCtrlList, SyExpr*> >* node = eclExprs.head;
                    node; node = node->next)
            {
                this->printLine();
                this->print(lexpr->toString());
                this->print(" <= ");
                this->printEventCtrlList(&node->val.first);
                this->print(node->val.second->toString());
                this->print(";");
            }
        }






    void
        SyVisitor::printEventCtrl(const SyEventStmt *evestmt)
        {
            const SyEventCtrlList& ecl = evestmt->getEventCtrlList();
            this->printEventCtrlList(&ecl);

            this->print("begin");
            this->incIndent();
            AcceptListOfObjs<SyStmt, SyVisitor>(this, evestmt->getStmts());
            this->decIndent();
            this->printLine();
            this->print("end");
        }

    void
        SyVisitor::printEventCtrlList(const SyEventCtrlList* ecl)
        {
            this->print("@(");
            const vector<SyExpr*>& posexprs = ecl->getPosEdgeExprs();
            bool first = true;

            for(unsigned int i = 0; i < posexprs.size(); ++i){
                if(!first) {
                    this->print(", ");
                }
                else {
                    first = false;
                }
                this->print("posedge ");
                this->print(posexprs[i]->toString());
            }
            const vector<SyExpr*>& negexprs = ecl->getNegEdgeExprs();
            for(unsigned int i = 0; i < negexprs.size(); ++i){
                if(!first) {
                    this->print(", ");
                }
                else {
                    first = false;
                }
                this->print("negedge ");
                this->print(negexprs[i]->toString());
            }
            const vector<SyExpr*>& exprs = ecl->getExprs();
            for(unsigned int i = 0; i < exprs.size(); ++i){
                if(!first) {
                    this->print(", ");
                }
                else {
                    first = false;
                }
                this->print(exprs[i]->toString());
            }

            this->print(") ");
        }


    void
        SyVisitor::printModCommon(const SyModule *mod)
        {
            currMod = mod->getName();
            this->print("module ");
            this->print(currMod);
            this->printHeaderPorts(mod);

            AcceptVectorVisitor<SyScPort, SyVPort, SyVisitor>(this, mod->getPorts());
            AcceptVectorVisitor<SyScNet, SyVNet, SyVisitor>(this, mod->getNets());
            AcceptVectorVisitor<SyScReg, SyVReg, SyVisitor>(this, mod->getRegs());
            const SyList<SyMemory*>& memories = mod->getMemories();
            for(SyNode<SyMemory*>* node = memories.head; node; node = node->next){
                node->val->accept(this);
            }    

            AcceptListOfObjs<SyModInst, SyVisitor>(this, mod->getModInsts());
            AcceptListOfObjs<SyPrimInst, SyVisitor>(this, mod->getPrimInsts());
        }

    void
        SyVisitor::printHeaderPorts(const SyScope *sc)
        {
            this->print("(");

            set<string, cmp_str> doneObjs;
            const SyList<SyScPort*>& ports = sc->getPorts();
            if(!ports.empty())
            {
                SyNode<SyScPort*>* node = ports.head;
                this->print(node->val->getIxName().getName());
                doneObjs.insert(node->val->getIxName().getName());

                for(node = node->next; node; node = node->next){
                    if(doneObjs.find(node->val->getIxName().getName()) !=
                                doneObjs.end()){
                        continue;
                    }
                    doneObjs.insert(node->val->getIxName().getName());
                    this->print(", ");
                    this->print(node->val->getIxName().getName());
                }
            }
            this->print(");");
            this->printLine();
        }


}
