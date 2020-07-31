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
#include <sstream>
#include <assert.h>
#include "syparse2synthtrans.h"
#include "syroot.h"
#include "sycontainer.h"
#include "systmt.h"
#include "syconst.h"
#include "symemory.h"
#include "symsghandler.h"
#include "syexprmgr.h"

using std::pair;
using std::ostringstream;
using std::map;
using std::list;
using std::set;
using std::make_pair;

namespace Synth
{

    void 
        SyParse2SynthTrans::translate(
                const Root* rtlRoot,
                const set<string, cmp_str>& synthModNames
                ) const
        {
            if(!rtlRoot){
                return;
            }

            set<string, cmp_str>::const_iterator mod = 
                synthModNames.begin();
            set<string, cmp_str>::const_iterator emod = 
                synthModNames.end();

            for(; mod != emod; ++mod)
            {
                const Module *rtlMod = dynamic_cast<const Module*>(
                        rtlRoot->getObject(*mod));
                assert(rtlMod);
                synthError = false;
                SyModule *synthMod = this->translate(rtlMod);
                if(synthMod && !synthError) {
                    SyRoot::instance()->addSynthMod(synthMod);
                }
            }

            this->resolveModInsts();
            //SyExprMgr::instance()->process();
        }

    SyModule*
        SyParse2SynthTrans::translate(const Module* rtlMod) const
        {
            if(SyModule *mod = SyRoot::instance()->getModule(rtlMod->getName())) {
                return mod;
            }    
            SyBModule* mod = new SyBModule(rtlMod->getName());
            this->translate(mod, rtlMod->getPorts());
            this->translate(mod, rtlMod->getNets());
            this->translate(mod, rtlMod->getRegs());
            this->translate(mod, rtlMod->getIntegers());

            const vector<Task*>& tasks = rtlMod->getTasks();
            for(unsigned int i = 0; i < tasks.size(); ++i){
                this->translateTask(mod, tasks[i]);
            }

            const vector<Func*>& funcs = rtlMod->getFuncs();
            for(unsigned int i = 0; i < funcs.size(); ++i){
                this->translateFunction(mod, funcs[i]);
            }
            const vector<ContAssign*>& contassigns = rtlMod->getContAssigns();
            for(unsigned int i = 0; i < contassigns.size(); ++i){
                this->translate(mod, contassigns[i]);
            }

            const vector<Always*>& alwaysBlocks = rtlMod->getAlwaysBlocks();
            for(unsigned int i = 0; i < alwaysBlocks.size(); ++i){
                this->translate(mod, alwaysBlocks[i]);
            }


            const list<GateInst*>& ginsts = rtlMod->getGateInsts();
            for(list<GateInst*>::const_iterator iter = ginsts.begin();
                    iter != ginsts.end(); ++iter)
            {
                const SyPrimInst *inst = this->translatePrimInst(mod, *iter);
                mod->addPrimInst(inst);
            }
            const list<ModInst*>& minsts = rtlMod->getModInsts();
            for(list<ModInst*>::const_iterator iter = minsts.begin();
                    iter != minsts.end(); ++iter)
            {
                const SyModInst *inst = this->translateModInst(mod, *iter);
                mod->addModInst(inst);
            }
            return mod;

        }

    void 
        SyParse2SynthTrans::translate(
                SyScope* sc,
                const vector<Port*>& ports,
                SyBModule *mod,
                bool useNewName) const
        {
            set<string, cmp_str> names;

            for(unsigned int i = 0; i < ports.size(); ++i)
            {
                const Port *port = ports[i];
                if(names.find(port->getIxName().getName()) != names.end()){
                    continue;
                }
                names.insert(port->getIxName().getName());
                if(port->isVecBit())
                {
                    //create vector port
                    const VecPort *vport = dynamic_cast<const VecPort*>(
                            port->getVecCntv());
                    const vector<Port*>& bits = vport->getPorts();
                    //memories to be handled
                    SyVPort *svport = new SyVPort(useNewName ? 
                        this->createUniqueName(mod, vport->getName()) : vport->getName(),
                            bits[0]->getIxName().getIndex(),
                            bits[bits.size()-1]->getIxName().getIndex(),
                            bits[0]->isSigned(),
                            Rtl2SynthPortDir(bits[0]->getDir()),
                            String2SynthNetType(bits[0]->getVarType()));
                            
                    this->addNewName(vport->getName(), svport->getName());

                    const SyList<SyScCntv*>& newbits = svport->getBits();
                    for(SyNode<SyScCntv*>* node = newbits.head; node; node = node->next)
                    {
                        const SyScPort *tport = dynamic_cast<const SyScPort*>(node->val);
                        sc->addPort(tport);
                    }
                    sc->addObject(svport->getName(), svport);
                    if(useNewName) 
                    {
                        assert(mod);
                        mod->addObject(svport->getName(), svport);
                    }    
                }
                else 
                {
                    SyScPort *scport = new SyScPort(
                            useNewName ? this->createUniqueName(mod,
                            port->getIxName().getName()) :
                            port->getIxName().getName(),
                            port->isSigned(), 
                            Rtl2SynthPortDir(port->getDir()),
                            String2SynthNetType(port->getVarType()));
                    sc->addPort(scport);
                    sc->addObject(scport->getIxName().getName(), scport);
                    this->addNewName(port->getIxName().getName(),
                        scport->getIxName().getName());
                    if(useNewName)
                    {
                        assert(mod);
                        mod->addObject(scport->getIxName().getName(), scport);
                    }    
                }
            }


        }
    void 
        SyParse2SynthTrans::translate(
                SyBModule* mod,
                const vector<Net*>& nets) const
        {
            set<string, cmp_str> names;

            for(unsigned int i = 0; i < nets.size(); ++i)
            {
                const Net *net = nets[i];
                if(names.find(net->getIxName().getName()) != names.end()){
                    continue;
                }
                names.insert(net->getIxName().getName());
                if(net->isVecBit())
                {
                    //create vector port
                    const VecNet *vnet = dynamic_cast<const VecNet*>(
                            net->getVecCntv());
                    const vector<Net*>& bits = vnet->getNets();
                    //memories to be handled
                    SyVNet *svnet = new SyVNet(vnet->getName(),
                            bits[0]->getIxName().getIndex(),
                            bits[bits.size()-1]->getIxName().getIndex(),
                            bits[0]->isSigned(),
                            Rtl2SynthNetType(bits[0]->getNetType()));

                    const SyList<SyScCntv*>& newbits = svnet->getBits();
                    for(SyNode<SyScCntv*>* node = newbits.head; node; node = node->next)
                    {
                        const SyScNet *tnet = dynamic_cast<const SyScNet*>(node->val);
                        mod->addNet(tnet);
                    }
                    mod->addObject(svnet->getName(), svnet);
                }
                else 
                {
                    SyScNet *scnet = new SyScNet(SyIxName(net->getIxName().getName()),
                            Rtl2SynthNetType(net->getNetType()),
                            net->isSigned());
                    mod->addNet(scnet);
                    mod->addObject(scnet->getIxName().getName(), scnet);
                }
            }

        }
    void 
        SyParse2SynthTrans::translate(
                SyBModule* mod,
                const vector<Reg*>& regs,
                const bool useNewName) const
        {
            set<string, cmp_str> names;
            SyTaskOrFunc *tf = const_cast<SyTaskOrFunc*>(dynamic_cast<const SyTaskOrFunc*>(
                mod->findObject(tfname)));

            for(unsigned int i = 0; i < regs.size(); ++i)
            {
                const Reg *reg = regs[i];
                if(names.find(reg->getIxName().getName()) != names.end()){
                    continue;
                }
                names.insert(reg->getIxName().getName());
                if(reg->isVecBit())
                {
                    //create vector port
                    const VecReg *vreg = dynamic_cast<const VecReg*>(
                            reg->getVecCntv());
                    if(this->isMemory(vreg))
                    {
                        this->createMemory(mod, vreg, useNewName);
                        continue;
                    }
                    const vector<Reg*> bits = vreg->getRegs();
                    SyVReg *svreg = new SyVReg(
                            useNewName ? this->createUniqueName(mod,
                                 vreg->getName()) : vreg->getName(),
                            bits[0]->getIxName().getIndex(),
                            bits[bits.size()-1]->getIxName().getIndex(),
                            bits[0]->isSigned());
                    this->addNewName(vreg->getName(), svreg->getName());

                    const SyList<SyScCntv*>& newbits = svreg->getBits();
                    for(SyNode<SyScCntv*>* node = newbits.head; node; node = node->next)
                    {
                        SyScReg *treg = dynamic_cast<SyScReg*>(node->val);
                        mod->addReg(treg);
                        if(useNewName) {
                            treg->setLocal();
                        }    
                        if(tf) {
                            tf->addReg(treg);
                        }
                    }
                    mod->addObject(svreg->getName(), svreg);
                    if(tf){
                        tf->addObject(svreg->getName(), svreg);
                    }    
                }
                else 
                {
                    SyScReg *screg = new SyScReg(SyIxName(
                                useNewName ? this->createUniqueName(
                                    mod, reg->getIxName().getName()) :
                                reg->getIxName().getName()),
                            reg->isSigned());
                    if(useNewName) {        
                        screg->setLocal();        
                    }    
                    mod->addReg(screg);
                    mod->addObject(screg->getIxName().getName(), screg);
                    if(tf)
                    {
                        tf->addReg(screg);
                        tf->addObject(screg->getIxName().getName(), screg);
                    }    
                    this->addNewName(reg->getIxName().getName(), 
                            screg->getIxName().getName());
                }
            }

        }

    void 
        SyParse2SynthTrans::translate(
                SyBModule* mod,
                const vector<Integer*>& ints,
                const bool useNewName) const
        {
            set<string, cmp_str> names;
            SyTaskOrFunc *tf = const_cast<SyTaskOrFunc*>(
                dynamic_cast<const SyTaskOrFunc*>( mod->findObject(tfname)));

            for(unsigned int i = 0; i < ints.size(); ++i)
            {
                const Integer *int_ = ints[i];
                if(names.find(int_->getIxName().getName()) != names.end()){
                    continue;
                }
                names.insert(int_->getIxName().getName());
                if(int_->isVecBit())
                {
                    //create vector port
                    const VecInt *vint = dynamic_cast<const VecInt*>(
                            int_->getVecCntv());
                    if(this->isMemory(vint))
                    {
                        this->createMemory(mod, vint, useNewName);
                        continue;
                    }
                    const vector<Integer*> bits = vint->getIntegers();
                    int msb = bits[0]->getIxName().getIndex();
                    int lsb = bits[bits.size()-1]->getIxName().getIndex();
                    if(msb > lsb) {
                        msb = 32*(msb-lsb+1) + lsb-1;
                    }
                    else {
                        lsb = 32*(lsb-msb+1) + msb -1;
                    }
                    SyVReg *svreg = new SyVReg(
                            useNewName ? this->createUniqueName(mod,
                                vint->getName()) : vint->getName(),
                            msb, lsb, true);
                    this->addNewName(vint->getName(), svreg->getName());

                    const SyList<SyScCntv*>& newbits = svreg->getBits();
                    for(SyNode<SyScCntv*>* node = newbits.head; node; node = node->next)
                    {
                        SyScReg *treg = dynamic_cast<SyScReg*>(node->val);
                        if(useNewName) {
                            treg->setLocal();
                        }    
                        mod->addReg(treg);
                        if(tf) {
                            tf->addReg(treg);
                        }    
                    }
                    mod->addObject(svreg->getName(), svreg);
                    if(tf){
                        tf->addObject(svreg->getName(), svreg);
                    }    
                }
                else {
                    this->translate(mod, int_, useNewName);
                }
            }

        }
    void
        SyParse2SynthTrans::translate(
                SyBModule *mod,
                const Integer *int_,
                const bool useNewName) const
        {
            SyTaskOrFunc *tf = const_cast<SyTaskOrFunc*>(
                dynamic_cast<const SyTaskOrFunc*>(mod->findObject(tfname)));
            string name = int_->getIxName().getName();
            SyVReg *svreg = new SyVReg(
                    useNewName ? this->createUniqueName(mod, name): 
                    name, 31, 0, true);
            this->addNewName(name, svreg->getName());
            const SyList<SyScCntv*>& newbits = svreg->getBits();
            for(SyNode<SyScCntv*>* node = newbits.head; node; node = node->next)
            {
                SyScReg *treg = dynamic_cast<SyScReg*>(node->val);
                if(useNewName) {
                    treg->setLocal();
                }    
                mod->addReg(treg);
                if(tf) {
                    tf->addReg(treg);
                }    
            }
            mod->addObject(svreg->getName(), svreg);
            if(tf) {
                tf->addObject(svreg->getName(), svreg);
            }
        }

    const SyExpr*
        SyParse2SynthTrans::translate(
                const SyBModule* mod,
                const Expr* expr) const
        {
            const SyExpr *retexpr = 0;
            if(retexpr = this->translate<Net, VecNet, SyScNet, SyVNet>(
                    mod, expr)){
                return retexpr;
            }
            if(retexpr = this->translate
                    <Port, VecPort, SyScPort, SyVPort>( mod, expr)){
                return retexpr;
            }

            if(retexpr = this->translate
                    <Reg, VecReg, SyScReg, SyVReg>(mod, expr)) {
                return retexpr;
            }

            if(retexpr = this->translateInteger(mod, expr)) {
                return retexpr;
            }

            if(retexpr = this->translateParam(mod, expr)) {
                return retexpr;
            }
            if(const BasedNum *bnum = dynamic_cast<const BasedNum*>(expr)){
                if(bnum->isUnknownOrHighImpedance()){
                    return this->translateXOrZ(bnum);
                }
                return this->createConst(bnum);
            }    
            if(const Num *num = dynamic_cast<const Num*>(expr)){
                //long val = const_->evaluate();
                //return new SyConst(val, true, val < 0);
                return this->createConst(num);
            }

            if(const RealConst *rconst_ = dynamic_cast<const RealConst*>(expr)) 
            {
                SyMsgHandler::instance()->print(44, mod->getName().c_str(), "real constant", expr->toString().c_str());
                return new SyXOrZ('x');
            }
            if(const HierId *id = dynamic_cast<const HierId*>(expr)) 
            {
                return translateHierId(mod, id);
            }

            if(const PartSel *partsel = dynamic_cast<const PartSel*>(expr)){
                return this->translatePartSel(mod, partsel);
            }
            if(const BitSel *bitsel = dynamic_cast<const BitSel*>(expr)){
                return this->translateBitSel(mod, bitsel);
            }

            if(const UnaryExpr *unary = dynamic_cast<const UnaryExpr*>(
                        expr))
            {
                return new SyUnary(this->translate(mod, unary->getExpr()),
                        Rtl2SynthOp(unary->getOp()));
            }
            if(const BinExpr *binary = dynamic_cast<const BinExpr*>(expr)){
                return new SyBinary(this->translate(mod, binary->getLExpr()),
                        this->translate(mod, binary->getRExpr()),
                        Rtl2SynthOp(binary->getOp()));
            }
            if(const CondExpr *condexpr = dynamic_cast<const CondExpr*>(expr)){
                return new SyCondExpr(
                        this->translate(mod, condexpr->getCond()),
                        this->translate(mod, condexpr->getTExpr()),
                        this->translate(mod, condexpr->getFExpr()));
            }
            if(const Concat *concat = dynamic_cast<const Concat*>(expr)){
                return this->translateConcat(mod, concat);
            }

            if(const VString *vstring = dynamic_cast<const VString*>(expr)){
                //return new SyVString(vstring->toString());
                //return SyVString(vstring->toString()).toConcat();
                return SyVString(vstring->toString()).toConst();
            }

            if(const FuncCall *funcCall = dynamic_cast<const FuncCall*>(expr)){
                return this->translateFuncCall(mod, funcCall);
            }

            return retexpr;

        }

    const SyExpr*
        SyParse2SynthTrans::translateInteger(
                const SyBModule *mod,
                const Expr *expr) const
        {
            const Integer *int_ = dynamic_cast<const Integer*>(expr);
            const VecInt *vint = dynamic_cast<const VecInt*>(expr);

            if(!int_ && !vint) {
                return 0;
            }
            if(vint && this->isMemory(vint)){
                //check
                return 0;
            }
            const SyVReg *vreg = dynamic_cast<const SyVReg*>(
                    int_ ? this->findObject(mod, int_->getIxName().getName()):
                    this->findObject(mod, vint->getName()));
            //This should have been converted to SyVReg
            assert(vreg);
            SyConcat *concat = new SyConcat(vreg->isSigned());
            const SyList<SyScCntv*>& bits = vreg->getBits();
            for(SyNode<SyScCntv*>* head = bits.head; head; head = head->next) {
                concat->addBit(head->val);
            }
            return concat;
        }

    const SyExpr*
        SyParse2SynthTrans::translateParam(const SyBModule *mod, const Expr *expr) const 
        {
            if(const VecParam* vparam = dynamic_cast<const VecParam*>(expr))
            {
                const SyExpr *rhs = this->translate(mod, vparam->getVal());
                //RHS evaluation
                SyConst *const_ = rhs->evaluate();
                assert(const_);
                //Now set the actual sign
                const_->setSign(vparam->isSigned());
                long psize = std::abs(vparam->getMsb() - vparam->getLsb()) + 1;
                long rsize = const_->size()->toLong();
                if(psize > rsize) {
                    return SyExprMgr::extend(const_, psize-rsize);
                }
                else if(psize < rsize) {
                    return SyExprMgr::truncate(const_, rsize, psize);
                }    
                return const_; //not reachable
                
            }
            else if(const Param *param = dynamic_cast<const Param*>(expr)) 
            {
                const SyExpr *rhs = this->translate(mod, param->getVal());
                //RHS evaluation
                SyConst *const_ = rhs->evaluate();
                //assert(const_);
                if(const_)
                {
                    string varType = param->getVarType();
                    if(varType.empty()) {
                        const_->setSign(param->isSigned());
                    }
                    else if(varType == "integer") {
                        const_->setSign(true);
                    }    
                    return const_;
                }
                return rhs; //SyXOrZ 
            }
        #if 0
            //check, the value is assumed to be positive
            if(const Param *param = dynamic_cast<const Param*>(expr)) {
                return new SyConst(param->getVal()->evaluate(), true, false);
            }
            else if(const VecParam* vparam = dynamic_cast<const VecParam*>(
                        expr)){
                return new SyConst(vparam->getVal()->evaluate(), true, false);
            }
        #endif
            return 0;
        }

    const SyExpr*
        SyParse2SynthTrans::translateBitSel(
                const SyBModule* mod,
                const BitSel *bitsel) const
        {
            const ScalarCntv *actual = dynamic_cast<const ScalarCntv*>(
                    bitsel->getActual());
            const VecCntv *rtlvcntv = dynamic_cast<const VecCntv*>(
                    bitsel->getVar());
            if(const VecParam *vparam = dynamic_cast<const VecParam*>(rtlvcntv))
            {
                int val = vparam->getBit(actual->getIxName().getIndex())->evaluate();
                return new SyConst(val, vparam->isSigned(), false);
                //int val = vparam->getVal()->evaluate();
                //return SyConst(val, vparam->isSigned(), val < 0).
                //    getBits()[actual->getIxName().getIndex()]; 
            }
            string varname;        
            if(rtlvcntv) {
                varname = rtlvcntv->getName();
            }
            else if(const Var *var = dynamic_cast<const Var*>(
                bitsel->getVar())) {
                varname = var->getIxName().getName();
            }
            assert(!varname.empty());
            const SyVCntv *vcntv = dynamic_cast<const SyVCntv*>(
                    this->findObject(mod, varname));
            if(actual)
            {
                assert(vcntv);
                return vcntv->getBit(actual->getIxName().getIndex());
            }

            //else create bit select
            const vector<Expr*> rdim = bitsel->getDim();
            vector<SyExpr*>sdim;
            for(unsigned int i = 0; i < rdim.size(); ++i){
                sdim.push_back(const_cast<SyExpr*>(
                            this->translate(mod, rdim[i])));
            }
            return new SyBitSel(vcntv, sdim);
        }

    const SyExpr*
        SyParse2SynthTrans::translatePartSel(
                const SyBModule *mod,
                const PartSel *partsel) const
        {
            const Concat *actual = partsel->getActual();
            const VecCntv *rtlvcntv = dynamic_cast<const VecCntv*>(
                    partsel->getVar());
            string varname;        
            if(rtlvcntv) {
                varname = rtlvcntv->getName();
            }
            else if(const Var *var = dynamic_cast<const Var*>(
                partsel->getVar())) {
                varname = var->getIxName().getName();
            }
            assert(!varname.empty());

            const SyVCntv *vcntv = dynamic_cast<const SyVCntv*>(
                    this->findObject(mod, varname));

            if(actual)
            {
                //constant part select translated into concat expression
                SyConcat *sconcat = new SyConcat(false);
                const vector<Expr*> exprs = actual->getExprs();
                for(unsigned int i = 0; i < exprs.size(); ++i){
                    sconcat->addBit(this->translate(mod, exprs[i]));
                }
                return sconcat;
            }
            vector<SyExpr*> sdim;
            const vector<Expr*>& rdim = partsel->getDim();
            //const Range *range = partsel->getRange();
            for(unsigned int i = 0; i < rdim.size(); ++i){
                sdim.push_back(const_cast<SyExpr*>(
                            this->translate(mod, rdim[i])));
            }

            pair<Expr*, Expr*> range = this->getRangeForPartSelect(partsel);
            return new SyPartSel(vcntv, sdim, this->translate(
                        mod, range.first), this->translate(
                        mod, range.second));
        }

    const SyExpr*
        SyParse2SynthTrans::translateConcat(
                const SyBModule *mod,
                const Concat *concat) const 
        {
            const MultiConcat *mconcat = dynamic_cast<const MultiConcat*>(
                    concat);
            const int count = mconcat ? mconcat->getCount()->evaluate() : 1;

            const vector<Expr*>& exprs = concat->getExprs();
            SyConcat *sconcat = new SyConcat(false);

            for(int i = 0; i < count; ++i){
                for(unsigned int j = 0; j < exprs.size(); ++j){
                    sconcat->addBit(this->translate(mod, exprs[j]));
                }
            }
            return sconcat;
        }

    const SyStmt*
        SyParse2SynthTrans::translate(
                SyBModule *mod,
                const Stmt *rstmt) const
        {
            if(const ContAssign *contassign = dynamic_cast<const ContAssign*>(
                        rstmt)){
                return this->translateContAssign(mod, contassign);
            }

            if(const ProcAssign *passign = dynamic_cast<const ProcAssign*>(
                        rstmt)){
                return this->translateProcAssign(mod, passign);
            }
            if(const CompAssign *cassign = dynamic_cast<const CompAssign*>(
                        rstmt)){
                return this->translateCompAssign(mod, cassign);
            }


            if(const Always *always = dynamic_cast<const Always*>(rstmt)){
                return this->translateAlways(mod, always);
            }

            if(const EventCntrl *evectrl = dynamic_cast<const EventCntrl*>(
                        rstmt)){
                return this->translateEventCtrl(mod, evectrl);
            }

            if(const If *if_ = dynamic_cast<const If*>(rstmt)){
                return this->translateIf(mod, if_);
            }

            if(const Case *case_ = dynamic_cast<const Case*>(rstmt)){
                assert(0);
                //return this->translateCase(mod, case_);
            }

            if(const TaskFuncCall *tfcall = dynamic_cast<const TaskFuncCall*>(
                        rstmt)){
                return this->translateTaskCall(mod, tfcall);
            }

            if(const DelayStmt *dstmt = dynamic_cast<const DelayStmt*>(rstmt)){
                //Delay is ignored
                return this->translate(mod, dstmt->getStmt());
            }
            return 0;
        }

    const SyContAssign*
        SyParse2SynthTrans::translateContAssign(
                SyBModule *mod,
                const ContAssign *rcontassign) const
        {
            SyContAssign *scontassign = new SyContAssign(
                    this->translate(mod, rcontassign->getLhs()),
                    this->translate(mod, rcontassign->getRhs()));
            mod->addContAssign(scontassign);
            return scontassign;
        }

    const SyStmt*
        SyParse2SynthTrans::translateProcAssign(
                SyBModule *mod,
                const ProcAssign *passign) const 
        {
            if(passign->getAssignType() == BLOCK){
                return new SyBlockAssign(this->translate(
                            mod, passign->getLhs()), this->translate(
                            mod, passign->getRhs()));
            }
            else {
                return new SyNonBlockAssign(this->translate(
                            mod, passign->getLhs()), this->translate(
                            mod, passign->getRhs()));
            }
        }

    const SyAlways*
        SyParse2SynthTrans::translateAlways(
                SyBModule *mod,
                const Always *always) const
        {
            const SyAlways *salways = static_cast<const SyAlways*>(
                    this->translateAlwaysOrEventCtrl(mod, always));
            mod->addAlways(salways);
            this->checkSensList(salways, always);
            return salways;
        }

    const SyEventCtrl*
        SyParse2SynthTrans::translateEventCtrl(
                SyBModule *mod,
                const EventCntrl *ctrl) const
        {
            return static_cast<const SyEventCtrl*>(
                    this->translateAlwaysOrEventCtrl(mod, ctrl));
        }


    const SyEventStmt*
        SyParse2SynthTrans::translateAlwaysOrEventCtrl(
                SyBModule* mod,
                const Stmt *stmt) const
        {
            const Always *always = dynamic_cast<const Always*>(stmt);
            const EventCntrl *rstmt = dynamic_cast<const EventCntrl*>(stmt);
            if(!rstmt){
                rstmt = dynamic_cast<const EventCntrl*>(
                    dynamic_cast<const EventCntrl*>(always->getStmt()));
            }

            if(!rstmt) {
                //not synthesizable
                return 0;
            }
            const vector<pair<EdgeType, Expr*> >& eventExprs = 
                rstmt->getEventExprs();

            SyEventStmt *sevestmt = always ? dynamic_cast<SyEventStmt*>(
                    new SyAlways()): dynamic_cast<SyEventStmt*>(
                    new SyEventCtrl());

            for(unsigned int i = 0; i < eventExprs.size(); ++i) {
                switch(eventExprs[i].first)
                {
                    case POSEDGE:
                        sevestmt->addPosEdgeExpr(this->translate(
                                    mod, eventExprs[i].second));
                        break;
                    case NEGEDGE:
                        sevestmt->addNegEdgeExpr(this->translate(
                                    mod, eventExprs[i].second));
                        break;
                    case NOEDGE:
                        sevestmt->addExpr(this->translate(
                                    mod, eventExprs[i].second));
                        break;
                    default:
                        assert(0);
                }
            }
            SyList<SyStmt*> *stmts = this->translateBlockOrStmt(
                    mod, rstmt->getStmt());

            if(stmts)
            {

                for(SyNode<SyStmt*> *head = stmts->head; head; head = head->next){
                    sevestmt->addStmt(head->val);
                }
            }    
            return sevestmt;
        }

    void 
        SyParse2SynthTrans::translateInnerScopeVars(
                SyBModule *mod,
                const vector<Reg*>& vars) const
        {
            //the variables declared at inner scopes eg named block
            // function etc will have name changed. They will appear
            //as if they are declared at module level.
            this->translate(mod, vars, true);
        }

    void 
        SyParse2SynthTrans::translateInnerScopeVars(
                SyBModule *mod,
                const vector<Integer*>& vars) const
        {
            //the variables declared at inner scopes eg named block
            // function etc will have name changed. They will appear
            //as if they are declared at module level.
            this->translate(mod, vars, true);
        }

    SyList<SyStmt*>*
        SyParse2SynthTrans::translateBlock(
                SyBModule *mod,
                const Block *block) const 
        {
            const vector<Stmt*>& stmts = block->getStmts();
            SyList<SyStmt*> *retlist = new SyList<SyStmt*>();

            for(unsigned int i = 0; i < stmts.size(); ++i)
            {
                if(const Block *nblock = dynamic_cast<const Block*>( stmts[i])) 
                {

                    SyList<SyStmt*> *tlist = 
                        this->translateBlock(mod, nblock);
                    retlist->splice(tlist);
                }
                else if(const NamedBlock *nblock = 
                        dynamic_cast<const NamedBlock*>(stmts[i])){
                    SyList<SyStmt*> *tlist = 
                        this->translateNamedBlock(mod, nblock);
                    retlist->splice(tlist);
                }
                else 
                {
                    //const SyStmt *sstmt = this->translate(mod, stmts[i]);
                    //retlist->push_back(const_cast<SyStmt*>(sstmt));
                    SyList<SyStmt*>* tlist = this->translateBlockOrStmt(mod, stmts[i]);
                    retlist->splice(tlist);
                }
            }
            return retlist;
        }

    SyList<SyStmt*>*
        SyParse2SynthTrans::translateNamedBlock(
                SyBModule *mod,
                const NamedBlock *nblock) const
        {
            table.push(map<string, string, cmp_str>());
            this->translateInnerScopeVars(mod, nblock->getRegs());
            this->translateInnerScopeVars(mod, nblock->getIntegers());
            const vector<Stmt*>& stmts = nblock->getStmts();

            SyList<SyStmt*> *retlist = new SyList<SyStmt*>();
            for(unsigned int i = 0; i < stmts.size(); ++i)
            {
                if(const Block *block = dynamic_cast<const Block*>( stmts[i])) {
                    SyList<SyStmt*> *tlist = 
                        this->translateBlock(mod, block);
                    retlist->splice(tlist);
                }
                else if(const NamedBlock *nnblock = 
                        dynamic_cast<const NamedBlock*>(stmts[i])){
                    SyList<SyStmt*> *tlist = 
                        this->translateNamedBlock(mod, nnblock);
                    retlist->splice(tlist);
                }
                else 
                {
                    //const SyStmt *sstmt = this->translate(mod, stmts[i]);
                    //retlist->push_back(const_cast<SyStmt*>(sstmt));
                    SyList<SyStmt*>* tlist = this->translateBlockOrStmt(mod, stmts[i]);
                    retlist->splice(tlist);
                }
            }
            table.pop();
            return retlist;
        }

    SyList<SyStmt*>*
        SyParse2SynthTrans::translateBlockOrStmt(
                SyBModule *mod,
                const Stmt *stmt) const
        {
            if(!stmt) {
                return 0;
            }
            if(const NamedBlock *nblock = dynamic_cast<const NamedBlock*>(
                        stmt))
            {
                return this->translateNamedBlock(mod, nblock);
            }
            else if(const Block *block = dynamic_cast<const Block*>(stmt)){
                return this->translateBlock(mod, block);
            }
            else if(const For *for_ = dynamic_cast<const For*>(stmt)){
                return this->translateFor(mod, for_);
            }
            else if(const Repeat *rep = dynamic_cast<const Repeat*>(stmt)){
                return this->translateRepeat(mod, rep);
            }
            else if(const Case *case_ = dynamic_cast<const Case*>(stmt)) {
                return translateCase(mod, case_);
            }    
            else
            {
                SyList<SyStmt*>* retlist = new SyList<SyStmt*>();
                SyStmt *newstmt = const_cast<SyStmt*>(this->translate(mod, stmt));
                if(newstmt) {
                    retlist->push_back(newstmt);
                }    
                //retlist->push_back(const_cast<SyStmt*>(
                //            this->translate(mod, stmt)));
                return retlist;
            }
            assert(0);
        }

    const SyIf*
        SyParse2SynthTrans::translateIf(
                SyBModule *mod,
                const If *if_) const
    {
        SyIf *sif = new SyIf(this->translate(mod, if_->getCondExpr()));

        SyList<SyStmt*>* stmts = this->translateBlockOrStmt(
                mod, if_->getStmt());
        if(!stmts) {
            return sif;
        }

        for(SyNode<SyStmt*>* head = stmts->head; head; head = head->next){
            sif->addThenStmt(head->val);
        }
        delete stmts;
        stmts = this->translateBlockOrStmt(mod, if_->getElsePart());

        if(!stmts) {
            return sif;
        }

        for(SyNode<SyStmt*>* head = stmts->head; head; head = head->next){
            sif->addElseStmt(head->val);
        }
        return sif;
    }

    SyList<SyStmt*>* 
        SyParse2SynthTrans::translateCase(
                SyBModule *mod,
                const Case *rcase) const
        {
            SyCase *scase = new SyCase(this->translate(
                        mod, rcase->getSelectExpr()));

            const vector<CaseItem*>& rcaseItems = rcase->getCaseItems();

            for(unsigned int i = 0; i < rcaseItems.size(); ++i)
            {
                const Stmt *rstmt = rcaseItems[i]->getStmt();
                const vector<Expr*>& rlabel = rcaseItems[i]->getLabel();
                SyCaseItem *scaseItem = 0;
                for(unsigned int j = 0; j < rlabel.size(); ++j)
                {
                    scaseItem = new SyCaseItem(
                            this->translate(mod, rlabel[j]), rcaseItems[i]->isDefault());
                    SyList<SyStmt*> *sstmts = this->translateBlockOrStmt(
                            mod, rstmt);
                    if(sstmts) {        
                        for(SyNode<SyStmt*>* head = sstmts->head; 
                            head; head = head->next){
                            scaseItem->addStmt(head->val);
                        }
                    }    
                    scase->addItem(scaseItem);
                }
            }
            SyList<SyStmt*>* stmts = this->transformCaseItems(scase, 
                (rcase->getType() == Rtl::CASE) ? 0 : 
                (rcase->getType() == Rtl::CASEX) ? 1 : 2);

            if(!stmts) {
                stmts = new SyList<SyStmt*>();
            }
            stmts->push_back(scase);

            return stmts;
        }

    SyList<SyStmt*>*
        SyParse2SynthTrans::translateFor(
                SyBModule *mod,
                const For *for_) const
        {
            //unrolling of the for loop
            SyList<SyStmt*>* stmts = new SyList<SyStmt*>();
            SyExpr *termExpr = const_cast<SyExpr*>(
                    this->translate(mod, for_->getTermExpr()));
            SyExpr *assignExpr = const_cast<SyExpr*>(
                    this->translate(mod, for_->getAssignExpr()));
            SyExpr *initExpr = const_cast<SyExpr*>(
                    this->translate(mod, for_->getInitExpr()));
            string loopvar = this->translate(mod, for_->getLoopVar())->toString();

            SyConst *cindex = initExpr->evaluate();
            SyVCntv *lvarcntv = const_cast<SyVCntv*>(
                    dynamic_cast<const SyVCntv*>(this->findObject(
                            mod, for_->getLoopVar()->toString())));
            if(lvarcntv){
                lvarcntv->addVal(cindex);
            }
            else {
                SyScCntv *lvarcntv = const_cast<SyScCntv*>(
                        dynamic_cast<const SyScCntv*>(this->findObject(
                                mod, for_->getLoopVar()->toString())));
            }
            SyList<SyStmt*>* origstmts = this->translateBlockOrStmt(mod,
                        for_->getStmt());
            if(!origstmts){
                return stmts;
            }
            int count = 0;
            while(1)
            {
                //mod->resetCntvs();
                if(!cindex) {
                    break;
                }
                if(count > maxLoopCount)
                {
                    SyMsgHandler::instance()->print(47, mod->getName().c_str());
                    return stmts;
                }    
                SyExpr *ttermExpr = termExpr->copy();
                ttermExpr->repSubExpr(loopvar, cindex);
                SyConst *ttermConst = ttermExpr->evaluate();
                if(!ttermConst || ttermConst->iszero())
                {
                    delete ttermExpr;
                    break;
                }
                for(SyNode<SyStmt*>* node = origstmts->head; node;
                            node = node->next) {
                    //mod->resetCntvs();
                    SyStmt *stmt = node->val->copy();
                    stmt->repSubExpr(loopvar, cindex);
                    stmts->push_back(stmt);
                }
                SyExpr *texpr = assignExpr->copy();
                texpr->repSubExpr(loopvar, cindex);
                cindex = texpr->evaluate();
                delete texpr;
                ++count;
            }
            return stmts;

        }

    SyList<SyStmt*>*
        SyParse2SynthTrans::translateRepeat(
                SyBModule *mod,
                const Repeat *rep) const
        {
            SyList<SyStmt*>* retstmts = new SyList<SyStmt*>();
            const SyConst *count = dynamic_cast<const SyConst*>(
                    this->translate(mod, rep->getExpr()));
            if(!count){
                return retstmts;
            }
            SyConst *one = SyConst::ONE();
            SyConst *lvar = SyConst::ZERO();
            SyList<SyStmt*> *stmts = this->translateBlockOrStmt(
                    mod,rep->getStmt());
            for(; *lvar < *count; *lvar = *lvar + *one)
            {
                for(SyNode<SyStmt*>* node = stmts->head; node; 
                        node = node->next)
                {
                    retstmts->push_back(node->val->copy());
                }
            }
            return retstmts;
        }



    const SyFunc*
        SyParse2SynthTrans::translateFunction(
                SyBModule *mod,
                const Func *rfunc) const
        {
            table.push(map<string, string, cmp_str>());
            const SyCntv *retcntv = 0;
          /*  const Range *range = rfunc->getRange();
            int msb = range ? range->getLExpr()->evaluate() : 0;
            int lsb = range ? range->getRExpr()->evaluate() :0;
            if(rfunc->getRetType() == "integer")
            {
                if(!range){
                    msb = 31;
                    lsb = 0;
                }
                else 
                {
                    msb *= 32;
                    lsb *= 32;
                }
            }
            if(msb > 0 || lsb > 0) {

                retcntv = new SyVReg(rfunc->getName(), msb, lsb,
                        rfunc->isSigned());
            }
            else {
                retcntv = new SyScReg(rfunc->getName(), rfunc->isSigned());
            }
            */

            SyFunc *sfunc = new SyFunc(rfunc->getName(), retcntv, 
                    rfunc->isSigned(), rfunc->isAuto());
            tfname = sfunc->getName();
            mod->addFunc(sfunc);
            mod->addObject(sfunc->getName(), sfunc);

            const vector<Port*>& rports = rfunc->getPorts();
            this->translate(sfunc, rports, mod, true);
            this->translate(mod, rfunc->getRegs(), true);
            this->translate(mod, rfunc->getIntegers(), true);
            retcntv = dynamic_cast<const SyCntv*>(
                sfunc->findObject(this->getNewName(rfunc->getName())));
            assert(retcntv);
            sfunc->setRetCntv(retcntv);
            if(const SyVCntv *rvcntv = dynamic_cast<const SyVCntv*>(retcntv)) {
                sfunc->addObject(rvcntv->getName(), rvcntv);
            }
            else if(const SyScCntv *rscntv = dynamic_cast<const SyScCntv*>(retcntv))  {
                sfunc->addObject(rscntv->getIxName().getName(), rscntv);
            }

            SyList<SyStmt*>* stmts = this->translateBlockOrStmt(mod,
                    rfunc->getFuncStmt());

            for(SyNode<SyStmt*>* node = stmts->head; node; node = node->next){
                sfunc->addStmt(node->val);
            }
            delete stmts;

            tfname.clear();
            table.pop();
            return sfunc;
        }


    const SyTask*
        SyParse2SynthTrans::translateTask(
                SyBModule *mod,
                const Task *rtask) const
        {
            table.push(map<string, string, cmp_str>());
            SyTask *stask = new SyTask(rtask->getName(), rtask->isAuto());
            mod->addTask(stask);
            mod->addObject(stask->getName(), stask);
            tfname = stask->getName();

            const vector<Port*>& rports = rtask->getPorts();
            this->translate(stask, rports, mod, true);
            this->translate(mod, rtask->getRegs(), true);
            this->translate(mod, rtask->getIntegers(), true);

            SyList<SyStmt*>* stmts = this->translateBlockOrStmt(mod,
                    rtask->getTaskStmt());

            for(SyNode<SyStmt*>* node = stmts->head; node; node = node->next){
                stask->addStmt(node->val);
            }
            delete stmts;

            tfname.clear();
            table.pop();
            return stask;
        }

    const SyExpr*
        SyParse2SynthTrans::translateFuncCall(
                const SyBModule *mod,
                const FuncCall *rfunccall) const
        {
            //SyTfCallCopier copier(this, mod, rfunccall);
            //return copier.copyFuncCall();
            if(rfunccall->isBuiltIn())
            {
                SyMsgHandler::instance()->print(18, rfunccall->getLine(),
                    rfunccall->getName().c_str());
                return new SyXOrZ('x');    
                //return 0;
            }    
            string fname = rfunccall->getName();
            if(fname.find_first_of('.') != string::npos)
            {
                SyMsgHandler::instance()->print(44, mod->getName().c_str(), "hierarchical function call", fname.c_str());
                return 0;
            }
            const SyFunc *sfunc = dynamic_cast<const SyFunc*>(
                    mod->findObject(fname));

            SyTaskOrFuncCall *stfcall = sfunc ? new SyTaskOrFuncCall(sfunc) :
                new SyTaskOrFuncCall(rfunccall->getName());

            const vector<Expr*>& rargs = rfunccall->getArgs();

            for(unsigned int i = 0; i < rargs.size(); ++i){
                stfcall->addArg(this->translate(mod, rargs[i]));
            }

            return new SyFuncCall(stfcall);
        }

    const SyTaskCall*
        SyParse2SynthTrans::translateTaskCall(
                SyBModule *mod,
                const TaskFuncCall *rtaskcall) const
        {
            //SyTfCallCopier copier(this, mod, rtaskcall);
            //return copier.copyTaskCall();
            if(rtaskcall->isBuiltIn()) 
            {
                SyMsgHandler::instance()->print(18, rtaskcall->getLine(),
                    rtaskcall->getName().c_str());
                return 0;    
            }
            string tname = rtaskcall->getName();
            if(tname.find_first_of('.') != string::npos)
            {
                SyMsgHandler::instance()->print(44, mod->getName().c_str(), "hierarchical task call", tname.c_str());
                return 0;
            }
            const SyTask *stask = dynamic_cast<const SyTask*>(
                    mod->findObject(tname));

            SyTaskCall *stcall = stask ? new SyTaskCall(stask) :
                new SyTaskCall(rtaskcall->getName());
            const vector<Expr*>& rargs = rtaskcall->getArgs();

            for(unsigned int i = 0; i < rargs.size(); ++i){
                stcall->addArg(this->translate(mod, rargs[i]));
            }
            return stcall;
        }

    void
        SyParse2SynthTrans::translate(
                SyBModule* mod,
                SyInst* inst,
                const ModInst* rtlinst,
                const vector<Pin*>& rpins) const
        {
            set<string, cmp_str> doneObjs;

            for(unsigned int i = 0; i < rpins.size(); ++i)
            {
                string name = rpins[i]->getIxName().getName();
                if(doneObjs.find(name) != doneObjs.end()){
                    continue;
                }
                doneObjs.insert(name);
                if(rpins[i]->isVecBit())
                {
                    //const VecPin *rvpins = dynamic_cast<const VecPin*>(
                    //        rpins[i]->getVecCntv());
                    const VecPin *rvpins = rtlinst->getVecPin(rpins[i]->getIxName().getName());
                    assert(rvpins);
                    const vector<Pin*>& rbits = rvpins->getPins();
                    SyVPin *svpin = new SyVPin(name, 
                            rbits[0]->getIxName().getIndex(),
                            rbits[rbits.size()-1]->getIxName().getIndex(),
                            this->translate(mod, rvpins->getActual()),
                            inst);
                    inst->addPin(svpin);
                    SyModInst *minst = dynamic_cast<SyModInst*>(inst);
                    assert(minst);
                    minst->addVecPin(svpin);
                }
                else {
                    SyScPin *spin = new SyScPin(name, this->translate(
                                mod, rpins[i]->getActual()), inst);
                    inst->addPin(spin);
                }
            }
        }





    const SyPrimInst*
        SyParse2SynthTrans::translatePrimInst(
                SyBModule* mod,
                const GateInst *ginst) const
        {
            SyPrimInst::SyGateType type = RtlToSynthGateType(
                    ginst->getType());
            if(type == SyPrimInst::NOGATE){
                return 0;
            }
            SyPrimInst *inst = new SyPrimInst(ginst->getName().toString(),
                    type);
            this->translate(mod, inst, NULL, ginst->getPins());
            return inst;
        }

    const SyModInst*
        SyParse2SynthTrans::translateModInst(
                SyBModule *mod,
                const ModInst *inst) const
        {
            SyModInst *sinst = new SyModInst(inst->getName().toString(), 0,
                inst->getModName());
                   // inst->getName().toString(),
                    //    SyRoot::instance()->getModule(mod->getName()), mod->getName());

            this->translate(mod, sinst, inst, inst->getPins());
            mod->addObject(sinst->getIxName().toString(), sinst);
            return sinst;
        }

    const SyCompAssign*
        SyParse2SynthTrans::translateCompAssign(
                SyBModule *mod,
                const CompAssign *rcassign) const
        {
            SyCompAssign *cassign = 0;
            if(rcassign->getAssignType() == Rtl::BLOCK){
                cassign = new SyBlockCompAssign(this->translate(mod,
                            rcassign->getLhs()));
            }
            else {
                cassign = new SyNonBlockCompAssign(this->translate(mod,
                            rcassign->getLhs()));
            }

            const Expr *repexpr = rcassign->getRepeatExpr();
            const int count = repexpr ? repexpr->evaluate() : 1;
            const EventCntrl *eventCtrl = rcassign->getEventCtrl();

            const vector<pair<EdgeType, Expr*> >& eventExprs = 
                eventCtrl->getEventExprs();

            for(int c = 0; c < count; ++c)
            {
                SyEventCtrlList ecl;
                for(unsigned int i = 0; i < eventExprs.size(); ++i) {
                    switch(eventExprs[i].first)
                    {
                        case POSEDGE:
                            ecl.addPosEdgeExpr(this->translate(
                                    mod, eventExprs[i].second));
                            break;
                        case NEGEDGE:
                            ecl.addNegEdgeExpr(this->translate(
                                    mod, eventExprs[i].second));
                            break;
                        case NOEDGE:
                            ecl.addExpr(this->translate(
                                    mod, eventExprs[i].second));
                            break;
                        default:
                            assert(0);
                    }
                    cassign->addEventCtrlExpr(ecl, this->translate(
                                mod, rcassign->getEventCtrlExpr()));
                }
            }
            return cassign;
        }

    string
        SyParse2SynthTrans::createUniqueName(
                const SyBModule *mod,
                const string& hint) const
        {
            int i = 0;
            string name = hint;

            while(1)
            {
                if(!mod->findObject(name)) {
                    return name;
                }
                ostringstream str;
                str << hint << "_" << i++;
                name = str.str();
            }
            assert(0);
        }

    bool
        SyParse2SynthTrans::isMemory(const VecVar* var) const
        {
            return false;
            const Dimension *dim = var->getDim();

            if(dynamic_cast<const VecReg*>(var) && 
                    (dim->packDim.size() == 1) &&
                    (dim->unpackDim.size() == 1)){
                //memory defined by reg variable
                return true;
            }

            if(dynamic_cast<const VecInt*>(var) && 
                    (dim->unpackDim.size() == 1)){
                //memory defined by integer variable with 32 bit word
                //size.
                return true;
            }
            return false;
        }

    void
        SyParse2SynthTrans::createMemory(
                SyBModule *mod,
                const VecVar *var,
                const bool useNewName) const
        {
            const Dimension *dim = var->getDim();
            const Range *packRange = 0;
            const VecReg *vreg = dynamic_cast<const VecReg*>(var);
            //const VecInt *vint = dynamic_cast<const VecInt*>(var);
            if(vreg){
                packRange = dim->packDim[0];
            }
            else {
                //packRange = new Range(new IntConst(31), new IntConst(0));
                vector<char> v1(5, '1');
                vector<char> v2(1, '0');
                packRange = new Range(    new BasedNum(5, 'b', false, v1), 
                                        new BasedNum(1, 'b', false, v2));
            }

            const int pmsb = packRange->getLExpr()->evaluate();
            const int plsb = packRange->getRExpr()->evaluate();
            
            const Range *unpackRange = dim->unpackDim[0];
            const int umsb = unpackRange->getLExpr()->evaluate();
            const int ulsb = unpackRange->getRExpr()->evaluate();
            SyMemoryHandler::instance()->createMemory(
                mod, var->getName(), abs(pmsb-plsb)+1, abs(umsb-ulsb)+1);

            /*const Range *unpackRange = dim->unpackDim[0];

            const int umsb = unpackRange->getLExpr()->evaluate();
            const int ulsb = unpackRange->getRExpr()->evaluate();

            SyList<SyMemory::Word> memory;
            string name = useNewName ? this->createUniqueName(
                    mod, var->getName()): var->getName();
            this->addNewName(name, var->getName());
            //Not sure if individual bits are required to be created.
            //To be looked into.

            for(int i = umsb; (umsb > ulsb) ? i >= ulsb : i <= ulsb;
                    (umsb > ulsb ) ? --i : ++i)
            {
                SyMemory::Word word;
                for(int j = pmsb; (pmsb > plsb) ? j >= plsb : j <= plsb;
                        (pmsb > plsb ) ? --j : ++j)
                {
                    word.push_back(new SyScReg(SyIxName(name, j), false));
                }
                memory.push_back(word);
            }

            SyMemory *mem = new SyMemory(memory);
            mod->addMemory(mem);
            mod->addObject(name, mem);
            */
        }

    SyPrimInst::SyGateType
        RtlToSynthGateType(GateType t)
        {
            switch(t)
            {
                case Rtl::OR: return SyPrimInst::OR;
                case Rtl::AND: return SyPrimInst::AND;
                case Rtl::NAND: return SyPrimInst::NAND;
                case Rtl::NOR: return SyPrimInst::NOR;
                case Rtl::XOR: return SyPrimInst::XOR;
                case Rtl::XNOR: return SyPrimInst::XNOR;
                case Rtl::BUF: return SyPrimInst::BUF;
                case Rtl::BUFIF0: return SyPrimInst::BUFIF0;
                case Rtl::BUFIF1: return SyPrimInst::BUFIF1;
                case Rtl::NOT: return SyPrimInst::NOT;
                case Rtl::NOTIF0: return SyPrimInst::NOTIF0;
                case Rtl::NOTIF1: return SyPrimInst::NOTIF1;
                default: return SyPrimInst::NOGATE;
            }
        }

    void
    SyParse2SynthTrans::resolveModInsts()  const
    {
        map<string, SyModule*, cmp_str> modules = 
            SyRoot::instance()->getSynthMods();
        map<string, SyModule*, cmp_str>::const_iterator mod = modules.begin();
        map<string, SyModule*, cmp_str>::const_iterator modEnd = modules.end();

        for(; mod != modEnd; ++mod) {
            const SyList<SyModInst*>& modInsts = mod->second->getModInsts();
            for(SyNode<SyModInst*>* node = modInsts.head; node; node = node->next)
            {
                node->val->setMaster(
                SyRoot::instance()->getModule(node->val->getMasterName()));
            }    
        }
    }    

    const SyExpr*
    SyParse2SynthTrans::translateXOrZ(const BasedNum* bnum) const
    {
        const vector<char>& bits = bnum->getExtendedBits();
        SyConcat *conc = new SyConcat(bnum->isSigned());
        int bitsize = bits.size();
        int size = bnum->len();
        for(int i = 0; i < bitsize && i < size; ++i) {
            switch(bits[i])
            {
                case 'x': 
                case 'X': 
                    conc->addBit(new SyXOrZ('X'));
                    break;
                case 'z': 
                case 'Z': 
                    conc->addBit(new SyXOrZ('Z'));
                    break;
                case '0': 
                    conc->addBit(SyConst::ZERO());
                    break;
                case '1': 
                    conc->addBit(SyConst::ONE());
                    break;
                default:
                    assert(0);
            }        
            
        }    
        return conc;
                
    /*
        //Assumption binary ?
        string numStr = bnum->toString();
        numStr = numStr.substr(numStr.find_first_of('\''));
        if(numStr.size() == 2) {
            return new SyXOrZ(numStr[1] == 'X' ? 'X' : 'Z');
        }    
        SyConcat *conc = new SyConcat(bnum->isSigned());

        for(unsigned int i = 2; i < numStr.size(); ++i) 
        {
            char c = toupper(numStr[i]);
            if(c == 'X' || c == 'Z' || c == '?'){
                conc->addBit( new SyXOrZ(c == 'X' ? 'X' : 'Z'));
            }
            else {
                conc->addBit(new SyConst((long)(numStr[i] - '0'), false, false));
            }    
        }
        return conc;
        */
    }

    SyList<SyStmt*>*
    SyParse2SynthTrans::transformCaseItems(SyCase *case_, int type) const
    {
        //type: 0->case, 1, casex, 2 casez
        SyList<SyStmt*>* defstmts = 0;
        SyList<SyCaseItem*> origItems = case_->getCaseItems();
        SyList<SyCaseItem*> newItems;
        std::set<long> labels;
        const int selSize = case_->getSelect()->size()->toLong();
        SyCaseItem *defaultItem = 0;

        for(SyNode<SyCaseItem*>* node = origItems.head; node; node = node->next)
        {
            if(node->val->isDefaultItem())
            {
                defaultItem = node->val;
                continue;
            }    
            SyList<SyCaseItem*> *tmp = 
                this->getNewCaseItems(node->val, type, selSize, labels);
            
            for(SyNode<SyCaseItem*>* item = tmp->head; item; item = item->next)
            {
                const SyList<SyStmt*>& stmts = node->val->getStmts();
                for(SyNode<SyStmt*>* stmt = stmts.head; stmt; stmt = stmt->next){
                    item->val->addStmt(stmt->val);
                }
            }    
            newItems.merge(*tmp);
            delete tmp;
        }    

        //Processing the default
        if(defaultItem)
        {
            defstmts = new SyList<SyStmt*>();
            *defstmts = defaultItem->getStmts();
        }    
        
        case_->clearItems();
        for(SyNode<SyCaseItem*>* node = newItems.head; node; node = node->next){
            case_->addItem(node->val);
        }    

        this->addMissingCaseLabels(case_);
        return defstmts;
    }

    SyList<SyCaseItem*>*
    SyParse2SynthTrans::getNewCaseItems(const SyCaseItem *oldItem,
                                        int type,
                                        int selSize,
                                        set<long>& labels) const
    {
        SyList<SyCaseItem*>* newItems = new SyList<SyCaseItem*>();
        //assumption: only positive labels
        if(const SyConst *label = dynamic_cast<const SyConst *>(
            oldItem->getLabel()->evaluate()))
        {
            set<long>::const_iterator iter = labels.find(label->toLong());
            if(iter != labels.end()) {
                return newItems;
            }
            newItems->push_back(new SyCaseItem(label->copy()));
            labels.insert(label->toLong());
            return newItems;
        }
        SyConcat *conc = 0;
        //if(/*const SyVString *vstring = */dynamic_cast<const SyVString*>(
        //    oldItem->getLabel()))
        if(oldItem->isDefaultItem())
        {
            //default case: make the label as all don't cares 
            conc = new SyConcat(false);
            for(int i = 0; i < selSize; ++i){
                conc->addBit(new SyXOrZ((type == 2) ? 'Z' : 'X'));
            }
        }    
        if(!conc) {
            conc = const_cast<SyConcat*>(
                dynamic_cast<const SyConcat*>(oldItem->getLabel()));
        }    
        if(!conc) {
            return newItems;
        }    

        conc = conc->flatten();
        
        int loopCount = (int)pow(2, selSize);
        for(int i = 0; i < loopCount; ++i)
        {
            //generate labels
            set<long>::const_iterator iter = labels.find(i);
            if(iter != labels.end()){
                continue;
            }    
             
            string str = Long2String(i, selSize);
            SyList<SyExpr*> exprs = conc->getExprs();
            int exprSize = exprs.size();
            if(exprSize < selSize) {
                //zero fill
                for(int j = 0; j < (selSize - exprSize); ++j) {
                    exprs.push_front(SyConst::ZERO());
                }    
            }
            SyNode<SyExpr*>* node = exprs.head;
            for(int j = 0; j < (exprSize-selSize); ++j){
                if(node) {
                    node = node->next;
                }
            }
            bool match = true;

            for(int j = 0; match && node && (j < selSize); node = node->next, ++j)
            {
                char c1 = str[j];
                char c2;
                if(SyConst *const_ = node->val->evaluate()) {
                    c2 = const_->isone() ? '1' : '0';
                }    
                else if(const SyXOrZ *xOrZ = dynamic_cast<const SyXOrZ*>(node->val)) {
                    if(xOrZ->isX()){
                        c2 = 'X';
                    }
                    else {
                        c2 = 'Z';
                    }    
                }
                else {
                    assert(0);
                }    
                    

                switch(type) 
                {
                    case 0:
                    {
                        //case
                        if((c1 != c2) && (c2 != 'X')) {
                            match = false;
                        }
                        break;
                    }
                    case 1:
                    {
                        //casex
                        if((c1 != c2) && (c2 != 'X') && (c2 != 'Z')) {
                            match = false;
                        }
                        break;
                    }
                    case 2:
                    {
                        //casez
                        if((c1 != c2) && (c2 != 'Z')) {
                            match = false;
                        }
                        break;
                    }
                }    
            }    
            if(match) 
            {
                SyConst *newConst = new SyConst(i, false, false);
                newItems->push_back(new SyCaseItem(newConst));
                labels.insert(i);
            }
        }    
        return newItems;
    }

    void
    SyParse2SynthTrans::addMissingCaseLabels(SyCase *case_) const
    {
        set<long> labels;

        SyList<SyCaseItem*>& items = const_cast<SyList<SyCaseItem*>& >(
            case_->getCaseItems());

        for(SyNode<SyCaseItem*> *node = items.head; node; node = node->next)
        {
            const SyExpr *expr = node->val->getLabel();
            SyConst *const_ = expr->evaluate();
            assert(const_);
            labels.insert(const_->toLong());
            delete const_;
        }

        SyConst *const_ = case_->getSelect()->size();

        int size = const_->toLong();
        if(size > 16) {
        //we support case with select size <= 16
            size = 16;
        }    
        delete const_;
        size = pow(2, size);
        for(int i = 0; i < size; ++i)
        {
            if(labels.find(i) != labels.end()) {
                continue;
            }
            SyCaseItem *newItem = new SyCaseItem(new SyConst( i, false, false));
            case_->addItem(newItem);
        }    
        this->sortCaseItems(const_cast<SyList<SyCaseItem*>&>(
            case_->getCaseItems()), size);

    }

    void
    SyParse2SynthTrans::sortCaseItems(SyList<SyCaseItem*>& caseItems, int size) const
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
    SyParse2SynthTrans::checkSensList(const SyAlways* always, const Always* ralways) const
    {
        if(const EventCntrl *ec = 
            dynamic_cast<const EventCntrl*>(ralways->getStmt())){
            
            if(ec->isV2kStyle()) {
                return;
            }
        }    
        set<SyExpr*> sensVars;
        if(!always->getPosEdgeExprs().empty()) {
            return;
        }    
        if(!always->getNegEdgeExprs().empty()) {
            return;
        }    
        const vector<SyExpr*>& exprs = always->getExprs();
        for(unsigned int i = 0; i < exprs.size(); ++i) {
            sensVars.insert(exprs[i]);
        }    

        SyList<SyCntv*>* readVars = always->getReadVars();
        for(SyNode<SyCntv*>* node = readVars->head; node; node = node->next) {
            if(sensVars.find(node->val) == sensVars.end())
            {
                SyMsgHandler::instance()->print(19, ralways->getLine());
                delete readVars;
                return;
            }
        }    
        delete readVars;
    }

    pair<Expr*, Expr*>    
    SyParse2SynthTrans::getRangeForPartSelect(const PartSel *partSel) const
    {
        const Range *rRange = partSel->getRange();
        if(dynamic_cast<const Rtl::FuncCall*>(rRange->getLExpr()) ||
            dynamic_cast<const  Rtl::FuncCall*>(rRange->getRExpr())) 
        {
            SyMsgHandler::instance()->print(20, partSel->getLine());
            synthError = true;
            return make_pair((Expr*)NULL, (Expr*)NULL);
        }    
        if(!partSel->isV2kStyle()) {
            return make_pair(const_cast<Expr*>(
                rRange->getLExpr()), 
                const_cast<Expr*>(rRange->getRExpr()));
        }    
        const VecCntv *vcntv = dynamic_cast<const VecCntv*>(partSel->getVar());
        assert(vcntv);
        const Dimension *dim = vcntv->getDim();

        Range *msbLsb = !dim->packDim.empty() ? dim->packDim[0] : dim->unpackDim[0];
        int msb = msbLsb->getLExpr()->evaluate();
        int lsb = msbLsb->getRExpr()->evaluate();

        bool littleEndian = msb > lsb;
        //int partSelMsb = rRange->getLExpr()->evaluate();
        //int partSelLsb = rRange->getRExpr()->evaluate();
        const Expr *partSelMsb = rRange->getLExpr();
        const Expr *partSelLsb = rRange->getRExpr();
        Expr *newMsb = 0;
        Expr *newLsb = 0;
        vector<char> v(1, '1');

        switch (partSel->getPolarityOperator())
        {
            case Rtl::POPOS:
            {
                if(littleEndian) {
                    newMsb = new BinExpr(partSelMsb->copy(), partSelLsb->copy(),
                        Rtl::PLUS);
                    //newMsb = new BinExpr(newMsb, new BasedNum(1, 'b', true, v), Rtl::MINUS);
                    newLsb = new BinExpr(newMsb->copy(), partSelLsb->copy(),
                        Rtl::MINUS);
                    //newLsb = new BinExpr(newLsb, new BasedNum(1, 'b', true, v),
                      //  Rtl::PLUS);
                }
                else {
                    newMsb = partSelMsb->copy();
                    newLsb = new BinExpr(partSelLsb->copy(), 
                        partSelMsb->copy(), PLUS);
            //        newLsb = new BinExpr(newLsb, new BasedNum(1, 'b', true, v), Rtl::MINUS);
                }    
                break;
            }
            case Rtl::PONEG:
            {
                if(littleEndian) {
                    newLsb = new BinExpr(partSelMsb->copy(), partSelLsb->copy(),
                        Rtl::MINUS);
                    //newLsb = new BinExpr(newLsb, new BasedNum(1, 'b', true, v),
                    //    Rtl::PLUS);
                    newMsb = partSelMsb->copy();    
                }
                else 
                {
                    newLsb = partSelMsb->copy();
                    newMsb = new BinExpr(partSelMsb->copy(), partSelLsb->copy(),
                        Rtl::MINUS);
                    //newMsb = new BinExpr(newMsb, new BasedNum(1, 'b', true, v),
                    //    Rtl::PLUS);
                }    
                break;
            }
            default: assert(0);
                
            
        }

        return make_pair(newMsb, newLsb);

        
    }

    SyConst*
    SyParse2SynthTrans::createConst(const BasedNum *num) const
    {
        num = num->convert2Base2();
        const vector<char>& bits = num->getBits();
        string val;
        for(int i = 0; i < bits.size(); ++i) {
            val.push_back(bits[i]);
        }    
        SyConst *const_ = new SyConst(num->isSigned(), val);
        //const_->setSize(num->len());
        return const_;
    }

    SyConst*
    SyParse2SynthTrans::createConst(const Num *num) const
    {
        int tmp = num->evaluate();
        string val;
        while(tmp) {
            if(tmp%2) {
                val.push_back('1');
            }
            else {
                val.push_back('0');
            }
            tmp /= 2;
        }
        std::reverse(val.begin(), val.end());
        int size = num->len();
        int valsize = val.size();
        //if(size < valsize) {
         //   val = val.substr(valsize-size);
        //}    
        //else {
            //to accomodate sign bit
        //    val = string("0") + val;
        //}    
        if(size > valsize) 
        {
            for(; valsize < size; ++valsize) {
                val.insert(val.begin(), '0');
            }
        }
        else {
            val = val.substr(valsize-size);
        }    
    //decimal numbers are always signed            
        return new SyConst(true, val);
    }

    const SyExpr*
    SyParse2SynthTrans::translateHierId(const SyBModule *mod, const HierId *id) const
    {
        const Expr *actual = id->getActualId();
        if(actual && actual->isConstExpr()) 
        {
            synthError = false;
            const Module * rtlMod = Rtl::GetModule(actual);
            SyModule *synthMod = this->translate(rtlMod);
            if(synthMod && !synthError) {
                SyRoot::instance()->addSynthMod(synthMod);
            }

            return translate(dynamic_cast<SyBModule*>(synthMod), actual);
        }
        else
        {
            synthError = false;
            SyMsgHandler::instance()->print(44, mod->getName().c_str(), "hierarchical identifier", id->toString().c_str());
            return new SyXOrZ('x');
        }    
    }    

    

}
