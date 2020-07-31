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

#include "rtl2netlist.h"

#include "sycontainer.h"
#include "sycntvs.h"
#include "syexpr.h"
#include "systmt.h"
#include "syconst.h"
#include "symsghandler.h"
#include "syroot.h"
#include "expr2gates.h"
#include "gencell.h"
#include "gencellmgr.h"
#include "stmt2gates.h"

const char *BUFINST = "bufinst";
using std::string;
using std::auto_ptr;
namespace Synth
{
//    const string&
//    Rtl2Netlist::getCurrModName() const
//    {
//        return currMod->getName();
//    }    

    void
    Rtl2Netlist::setCurrMod(SyModule *mod) 
    {
        this->setCurrModInternal(mod);
        if(expr2Gates) {
            expr2Gates->setCurrMod(mod);
        }    
    }    

    void
    Rtl2Netlist::setCurrSynthMod(SyModule *mod)
    {
        this->setCurrSynthModInternal(mod);
        if(expr2Gates) {
            expr2Gates->setCurrSynthMod(mod);
        }    
    }    

    void
    Rtl2Netlist::setCurrRtlMod(SyModule *mod)
    {
        this->setCurrRtlModInternal(mod);
        if(expr2Gates) {
            expr2Gates->setCurrRtlMod(mod);
        }    
    }    

    void
    Rtl2Netlist::setCurrScope(SyScope *sc)
    {
        this->setCurrScopeInternal(sc);
        if(expr2Gates) {
            expr2Gates->setCurrScope(sc);
        }    
    }    

    void
    Rtl2Netlist::setExpr2GateHandler()
    {
        if(expr2Gates) {
            return;
        }
        expr2Gates = new Expr2Gates(this->getCurrRtlMod(), this->getCurrSynthMod(), this->getCurrSynthMod());
    }    
    void
    Rtl2Netlist::visit(const SyBModule *rtlMod)
    {
        SyMsgHandler::instance()->print(15, rtlMod->getName().c_str());
        this->setCurrSynthMod(new SyModule(rtlMod->getName()));
        //untidy but I will live with this. Fetching vector cntv
        //for scalar bit requires the currMod to be set to rtlMod
        //because the synthMod is not ready yet.
        this->setCurrMod(const_cast<SyBModule*>(rtlMod));
        this->setCurrRtlMod(this->getCurrMod());
        this->setCurrScope(this->getCurrMod());
        this->setExpr2GateHandler();

        AcceptVectorVisitor<SyScPort, SyVPort, Rtl2Netlist>(this, rtlMod->getPorts());
        if(SyRoot::instance()->isDontTouchModule(this->getCurrSynthMod())){
            return;
        }    
        AcceptVectorVisitor<SyScNet, SyVNet, Rtl2Netlist>(this, rtlMod->getNets());
        AcceptVectorVisitor<SyScReg, SyVReg, Rtl2Netlist>(this, rtlMod->getRegs());

        //Now set synth module as current module
        //currMod = this->getCurrSynthMod();
        //currScope = this->getCurrSynthMod();
        this->setCurrMod(this->getCurrSynthMod());
        this->setCurrScope(this->getCurrSynthMod());
        AcceptListOfObjs<SyTask, Rtl2Netlist>(this, rtlMod->getTasks());
        AcceptListOfObjs<SyFunc, Rtl2Netlist>(this, rtlMod->getFuncs());

        AcceptListOfObjs<SyModInst, Rtl2Netlist>(this, rtlMod->getModInsts());
        AcceptListOfObjs<SyPrimInst, Rtl2Netlist>(this, rtlMod->getPrimInsts());

        AcceptListOfObjs<SyContAssign, Rtl2Netlist>( this, rtlMod->getContAssigns());
        AcceptListOfObjs<SyAlways, Rtl2Netlist>(this, rtlMod->getAlwaysBlocks());
        SyRoot::switchStage();
        SyRoot::instance()->addSynthMod(this->getCurrSynthMod());
        SyRoot::switchStage();
        SyMsgHandler::instance()->print(16);

    }

    //-----------------------------------------------------------------
    //    Create vector ports in module to be synthesized
    //-----------------------------------------------------------------

    void
    Rtl2Netlist::visit(const SyVPort *rtlPort)
    {
        SyVPort *synthPort = new SyVPort(    rtlPort->getName(),
                                            rtlPort->getMsb(),
                                            rtlPort->getLsb(),
                                            rtlPort->isSigned(),
                                            rtlPort->getDir(),
                                            rtlPort->getNetType()
                                        );
        this->getCurrSynthMod()->addObject(synthPort->getName(), synthPort);
        const SyList<SyScCntv*>& bits = synthPort->getBits();
        for(SyNode<SyScCntv*>* node = bits.head; node; node = node->next)
        {
            const SyScPort *tport = dynamic_cast<const SyScPort*>(node->val);
            this->getCurrSynthMod()->addPort(tport);
        }
    }    

    //-----------------------------------------------------------------
    //    Create scalar ports in module to be synthesized.
    //    It should not be a bit of a vector.
    //-----------------------------------------------------------------
    void
    Rtl2Netlist::visit(const SyScPort *rtlPort)
    {
        assert(!rtlPort->isVecBit());
        SyScPort *synthPort = new SyScPort(    rtlPort->getIxName().getName(),
                                            rtlPort->isSigned(),
                                            rtlPort->getDir(),
                                            rtlPort->getNetType()
                                        );
        this->getCurrSynthMod()->addObject(synthPort->getIxName().getName(), synthPort);
        this->getCurrSynthMod()->addPort(synthPort);
    }    

    //-----------------------------------------------------------------
    //    Create vector nets in module to be synthesized
    //-----------------------------------------------------------------
    void
    Rtl2Netlist::visit(const SyVNet *rtlNet)
    {
        SyVNet *synthNet = new SyVNet(    rtlNet->getName(),
                                        rtlNet->getMsb(),
                                        rtlNet->getLsb(),
                                        rtlNet->isSigned(),
                                        rtlNet->getNetType()
                                    );
        this->getCurrSynthMod()->addObject(synthNet->getName(), synthNet);
        const SyList<SyScCntv*>& bits = synthNet->getBits();
        for(SyNode<SyScCntv*>* node = bits.head; node; node = node->next)
        {
            const SyScNet *tnet = dynamic_cast<const SyScNet*>(node->val);
            this->getCurrSynthMod()->addNet(tnet);
        }
    }    

    //-----------------------------------------------------------------
    //    Create scalar nets in module to be synthesized
    //    It should not be a bit of a vector.
    //-----------------------------------------------------------------
    void
    Rtl2Netlist::visit(const SyScNet *rtlNet)
    {
        assert(!rtlNet->isVecBit());
        SyScNet *synthNet = new SyScNet(rtlNet->getIxName().getName(),
                                        rtlNet->getNetType(),
                                        rtlNet->isSigned()
                                        );
        this->getCurrSynthMod()->addNet(synthNet);
        this->getCurrSynthMod()->addObject(synthNet->getIxName().getName(), synthNet);
    }    

    //-----------------------------------------------------------------
    //    Create vector regs in module to be synthesized
    //-----------------------------------------------------------------
    void    
    Rtl2Netlist::visit(const SyVReg *rtlReg)
    {
        SyVReg *synthReg = new SyVReg(    rtlReg->getName(),
                                        rtlReg->getMsb(),
                                        rtlReg->getLsb(),
                                        rtlReg->isSigned()
                                    );
        this->getCurrSynthMod()->addObject(synthReg->getName(), synthReg);
        const SyList<SyScCntv*>& bits = synthReg->getBits();
        for(SyNode<SyScCntv*>* node = bits.head; node; node = node->next)
        {
            const SyScReg *treg = dynamic_cast<const SyScReg*>(node->val);
            this->getCurrSynthMod()->addReg(treg);
        }
    }    

    //-----------------------------------------------------------------
    //    Create scalar regs in module to be synthesized
    //    It should not be a bit of a vector.
    //-----------------------------------------------------------------
    void
    Rtl2Netlist::visit(const SyScReg *rtlReg)
    {
        assert(!rtlReg->isVecBit());
        SyScReg *synthReg = new SyScReg(rtlReg->getIxName().getName(),
                                        rtlReg->isSigned()
                                        );
        this->getCurrSynthMod()->addReg(synthReg);
        this->getCurrSynthMod()->addObject(synthReg->getIxName().getName(), synthReg);
    }    

    void
    Rtl2Netlist::visit(const SyContAssign *cassign)
    {
        Stmt2Gates stmt2Gates(this->getCurrRtlMod(), 
            this->getCurrSynthMod(), this->getCurrSynthMod());
        cassign->accept(&stmt2Gates);    
        
    }    

    void
    Rtl2Netlist::visit(const SyModInst *rinst)
    {
        //Assumption: The master module, if exists is already synthesized
        SyModule *master = SyRoot::instance()->getModule(rinst->getMasterName());
        SyModInst *sinst = new SyModInst(rinst->getIxName(), master, rinst->getMasterName());
        this->getCurrSynthMod()->addModInst(sinst);
        if(!master) {
            return;
        }

        SyList<SyCntv*> *ports = master->getMixPorts();
        for(SyNode<SyCntv*>* node = ports->head; node; node = node->next)
        {
            if(SyVPort *vport = dynamic_cast<SyVPort*>(node->val))
            {
                const SyVPin *rpin = rinst->getVecPin(vport->getName());
                assert(rpin);
                const SyExpr *actual = rpin->getActual();
                actual->accept(expr2Gates);
                auto_ptr<Expr2GatesRetType> result = expr2Gates->getResult();
                int psize = vport->size()->toLong();
                int asize = result->outputs->size();
                SyNode<SyCntv*> *outBit = result->outputs->head;

                for(int i = 0; i < (psize-asize); ++i) {
                    if(outBit->val->isSigned()) {
                        //sign extension
                        result->outputs->push_front(outBit->val);
                    }
                    else {
                        result->outputs->push_front(expr2Gates->createVccOrGround(false));
                    }
                }    
                outBit = result->outputs->head;
                for(int i = 0; (i < (asize-psize)) && outBit; ++i) {
                    outBit = outBit->next;
                }    
                SyConcat *conc = new SyConcat(actual->isSigned());
                for(; outBit; outBit = outBit->next) {
                    conc->addBit(outBit->val);
                }
                SyVPin *spin = new SyVPin(vport->getName(),
                                            vport->getMsb(),
                                            vport->getLsb(),
                                            conc,
                                            sinst);
                sinst->addVecPin(spin);                            
                sinst->addPin(spin);
            }    
            else 
            {
                SyScPort *rport = dynamic_cast<SyScPort*>(node->val);
                assert(rport);
                const SyScPin *rpin = rinst->getPin(rport->getIxName());
                assert(rpin);
                const SyExpr *actual = rpin->getActual();
                auto_ptr<Expr2GatesRetType> result;
                if(actual)
                {
                    actual->accept(expr2Gates);
                    result = expr2Gates->getResult();
                }    

                SyScPin *spin = new SyScPin(rport->getIxName(), 
                    actual ? result->outputs->tail->val : 0, sinst);
                sinst->addPin(spin);
            }    
                                            
        }

    }

    void
    Rtl2Netlist::visit(const SyPrimInst *pinst)
    {
        switch (pinst->getGateType())
        {
            case SyPrimInst::BUFIF0:
            case SyPrimInst::BUFIF1:
            case SyPrimInst::NOTIF0:
            case SyPrimInst::NOTIF1:
            case SyPrimInst::OR:
            case SyPrimInst::AND:
            case SyPrimInst::NAND:
            case SyPrimInst::NOR:
            case SyPrimInst::XOR:
            case SyPrimInst::XNOR:
            {
                this->createPrimitiveGate(pinst);
                break;
            }    

            case SyPrimInst::NOT:
            case SyPrimInst::BUF:
            {
                this->createNotBuf(pinst);
                break;
            }    
            default:
                break;
        }    
    }

    void
    Rtl2Netlist::visit(const SyAlways *always)
    {
        Stmt2Gates stmt2Gates(this->getCurrRtlMod(), 
            this->getCurrSynthMod(), this->getCurrSynthMod());
        always->accept(&stmt2Gates);
    }    

    void
    Rtl2Netlist::visit(const SyFunc *func)
    {
        this->createPortsForTfPorts(func);
    }
    void
    Rtl2Netlist::visit(const SyTask *task)
    {
        this->createPortsForTfPorts(task);
    }
    void
    Rtl2Netlist::createPrimitiveGate(const SyPrimInst *pinst)
    {
        const SyList<SyScPin*>& pins = pinst->getPins();
        Expr2GatesRetType *inouts = new Expr2GatesRetType();
        for(SyNode<SyScPin*> *node = pins.head; node; node = node->next)
        {
            const SyExpr *actual = node->val->getActual();
            SyCntv *newcntv = 0;
            if(actual)
            {
                actual->accept(expr2Gates);
                newcntv = expr2Gates->getResult()->outputs->head->val;
            }
            else {
                newcntv = createNet();
            }
            if(node->val->getDir() == SyScPort::IN) {
                inouts->inputs->push_back(newcntv);
            }
            else {
                inouts->outputs->push_back(newcntv);
            }    
        }
    /*
        auto_ptr<Expr2GatesRetType> in0(0);
        auto_ptr<Expr2GatesRetType> in1(0);
        auto_ptr<Expr2GatesRetType> o0(0);
        const SyExpr *actual = pinst->getPin(SyIxName("i0"))->getActual();
        
        if(actual) 
        {
            actual->accept(expr2Gates);
            in0 = expr2Gates->getResult();
        }

        actual = pinst->getPin(SyIxName("i1"))->getActual();
        if(actual)
        {
            actual->accept(expr2Gates);
            in1 = expr2Gates->getResult();
        }

        actual = pinst->getPin(SyIxName("o"))->getActual();
        if(actual)
        {
            actual->accept(expr2Gates);
            o0 = expr2Gates->getResult();
        }

        Expr2GatesRetType *inouts = new Expr2GatesRetType();
        inouts->inputs->push_back(in0->outputs->head->val);
        inouts->inputs->push_back(in1->outputs->head->val);
        inouts->outputs->push_back(o0->outputs->head->val);
    */    

        switch(pinst->getGateType())
        {
            case SyPrimInst::OR:
            {
                auto_ptr<Expr2GatesRetType> ret(
                    expr2Gates->create2InputGateTree(inouts->inputs, OR));
                expr2Gates->createCellInst(inouts->outputs, ret->outputs, 1, 1, BUF);
                break;
            }    
            case SyPrimInst::AND:
            {
                auto_ptr<Expr2GatesRetType> ret(
                    expr2Gates->create2InputGateTree(inouts->inputs, AND));
                expr2Gates->createCellInst(inouts->outputs, ret->outputs, 1, 1, BUF);
                break;
            }    
            case SyPrimInst::NAND:
            {
                auto_ptr<Expr2GatesRetType> ret(
                    expr2Gates->create2InputGateTree(inouts->inputs, AND));
                expr2Gates->createCellInst(inouts->outputs, ret->outputs, 1, 1, NOT);
                break;
            }    
            case SyPrimInst::NOR:
            {
                auto_ptr<Expr2GatesRetType> ret(
                    expr2Gates->create2InputGateTree(inouts->inputs, OR));
                expr2Gates->createCellInst(inouts->outputs, ret->outputs, 1, 1, NOT);
                break;
            }    
            case SyPrimInst::XOR:
            {
                auto_ptr<Expr2GatesRetType> ret(
                    expr2Gates->create2InputGateTree(inouts->inputs, XOR));
                expr2Gates->createCellInst(inouts->outputs, ret->outputs, 1, 1, BUF);
                break;
            }    
            case SyPrimInst::XNOR:
            {
                auto_ptr<Expr2GatesRetType> ret(
                    expr2Gates->create2InputGateTree(inouts->inputs, XOR));
                expr2Gates->createCellInst(inouts->outputs, ret->outputs, 1, 1, NOT);
                break;
            }    
            case SyPrimInst::BUFIF0:
            case SyPrimInst::BUFIF1:
            case SyPrimInst::NOTIF0:
            case SyPrimInst::NOTIF1:
            {
                this->createTristate(pinst, inouts);
                break;
            }    
            default:
                break;
        }        
        delete inouts;

        expr2Gates->getResult();
    }    

    void
    Rtl2Netlist::createNotBuf(const SyPrimInst *pinst)
    {
        auto_ptr<Expr2GatesRetType> in0(0);
        auto_ptr<Expr2GatesRetType> o0(0);
        const SyExpr *actual = pinst->getPin(SyIxName("i0"))->getActual();
        if(actual) 
        {
            actual->accept(expr2Gates);
            in0 = expr2Gates->getResult();
        }

        actual = pinst->getPin(SyIxName("o"))->getActual();
        if(actual)
        {
            actual->accept(expr2Gates);
            o0 = expr2Gates->getResult();
        }
        Expr2GatesRetType *inouts = new Expr2GatesRetType();
        inouts->inputs->push_back(in0->outputs->head->val);
        inouts->outputs->push_back(o0->outputs->head->val);

        if(pinst->getGateType() == SyPrimInst::NOT) {
            expr2Gates->createCellInst(inouts->outputs, inouts->inputs, 1, 1, NOT);
        }
        else {
            expr2Gates->createCellInst(inouts->outputs, inouts->inputs, 1, 1, BUF);
        }

        delete inouts;
        //to flushout the result
        expr2Gates->getResult();
    }    


    void
    Rtl2Netlist::createTristate(const SyPrimInst* pinst, Expr2GatesRetType* inouts)
    {
        auto_ptr<Expr2GatesRetType> invret;
        auto_ptr<Expr2GatesRetType> invsel;
        Expr2GatesRetType *tribufret = new Expr2GatesRetType();

        SyNode<SyCntv* > *in = inouts->inputs->head;
        SyNode<SyCntv* > *sel = inouts->inputs->head->next;
        SyNode<SyCntv* > *out = inouts->outputs->head;

        SyPrimInst::SyGateType type = pinst->getGateType();

        if(    type == SyPrimInst::BUFIF0 || type == SyPrimInst::NOTIF0)
        {
            Expr2GatesRetType tmp;
            tmp.inputs->push_back(sel->val);
            invsel = auto_ptr<Expr2GatesRetType>(
                expr2Gates->createCellInst(0, tmp.inputs, 1, 1, NOT));
        }

        if(    type == SyPrimInst::NOTIF0 || type == SyPrimInst::NOTIF1)
        {
            Expr2GatesRetType tmp;
            tmp.inputs->push_back(in->val);
            invret = auto_ptr<Expr2GatesRetType>(
                expr2Gates->createCellInst(0, tmp.inputs, 1, 1, NOT));
        }

        switch (type)
        {
            case BUFIF0:
            {
                tribufret->inputs->push_back(in->val);
                tribufret->inputs->push_back(invsel->outputs->head->val);
                tribufret->outputs->push_back(out->val);
                expr2Gates->createCellInst(    tribufret->outputs, 
                                            tribufret->inputs,
                                            1, 2, TRIBUF);
                break;
            }
            case BUFIF1:
            {
                expr2Gates->createCellInst(    inouts->outputs, 
                                            inouts->inputs,
                                            1, 2, TRIBUF);
                break;
            }

            case NOTIF0:
            {
                tribufret->inputs->push_back(invret->outputs->head->val);
                tribufret->inputs->push_back(invsel->outputs->head->val);
                tribufret->outputs->push_back(out->val);
                
                expr2Gates->createCellInst(    tribufret->outputs, 
                                            tribufret->inputs,
                                            1, 2, TRIBUF);
                break;
            }

            case NOTIF1:
            {
                tribufret->inputs->push_back(invret->outputs->head->val);
                tribufret->inputs->push_back(sel->val);
                tribufret->outputs->push_back(out->val);
                
                expr2Gates->createCellInst(    tribufret->outputs, 
                                            tribufret->inputs,
                                            1, 2, TRIBUF);
                break;
            }

            default:
                assert(0);
        }        
        expr2Gates->getResult();
        delete tribufret;
                
    }

    void
    Rtl2Netlist::createDecoderLogicForAssign(   const SyBitSel *bitSel,
                                                const SyExpr *expr)
    {                                            
        expr2Gates->createDecoderLogicForAssign(bitSel, expr);
        //SyList<const SyCntv*> tmp;
        //CreateFlatList<const SyCntv*>(tmp, bitSel->getVar());
        auto_ptr<Expr2GatesRetType> result = expr2Gates->getResult();
        result->outputs->reverse();
        bitSel->getVar()->accept(expr2Gates);
        auto_ptr<Expr2GatesRetType> tmp = expr2Gates->getResult();
        tmp->outputs->reverse();
        SyNode <SyCntv*>* node1 = tmp->outputs->head;
        SyNode <SyCntv*>* node2 = result->outputs->head;
        for(; node1 && node2; node1 = node1->next, node2 = node2->next){
            this->createBuf(const_cast<SyCntv*>(node1->val), node2->val);
        }
    }    

    void
    Rtl2Netlist::createPortsForTfPorts(const SyTaskOrFunc *tf)
    {
        const SyList<SyScPort*>& ports = tf->getPorts();
        set<string, cmp_str> doneObjs;
        string tfname = tf->getName();
        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next)
        {
            if(doneObjs.find(node->val->getIxName().getName()) !=
                        doneObjs.end()){
                continue;
            }
            doneObjs.insert(node->val->getIxName().getName());
            if(node->val->isVecBit())
            {
                const SyVPort *vport = GetVecCntv<SyScPort, SyVPort>(
                    node->val->getIxName().getName(), this->getCurrModName(),
                    tfname);
                SyVPort *synthPort = new SyVPort(vport->getName(),
                                        vport->getMsb(),
                                        vport->getLsb(),
                                        vport->isSigned(),
                                        vport->getDir(),
                                        SyScNet::WIRE    
                                    );
                this->getCurrSynthMod()->addTfPort(synthPort->getName(), synthPort);
                const SyList<SyScCntv*>& bits = synthPort->getBits();
                for(SyNode<SyScCntv*>* node = bits.head; node; node = node->next) {
                    this->getCurrSynthMod()->addTfPort(node->val->toString(), node->val);
                }
            }
            else 
            {
                SyScPort *synthPort = new SyScPort(
                    node->val->getIxName(),
                    node->val->isSigned(),
                    node->val->getDir(),
                    SyScNet::WIRE
                    );
                this->getCurrSynthMod()->addTfPort(synthPort->toString(), synthPort);
            }
        }

    }

}
