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
#include <algorithm>
#include "expr2gates.h"
#include "sycntvs.h"
#include "gencellmgr.h"
#include "syroot.h"
#include "systmt.h"
#include "namemgr.h"
#include "syconst.h"
#include "ftcall2gates.h"
#include "stmt2gates.h"
#include "assignstmtstrans.h"

using std::auto_ptr;
using std::make_pair;
using std::pair;
namespace Synth
{
    Expr2Gates::Expr2Gates(    const SyModule    *rmod,
                            const SyModule    *smod,
                            const SyScope    *sscope): 
                            ConVisitorBase(rmod, smod, sscope)
                             {
    }

    Expr2Gates::~Expr2Gates() {
    }

    void
    Expr2Gates::setCurrMod(SyModule *mod) {
        this->setCurrModInternal(mod);
    }

    void
    Expr2Gates::setCurrRtlMod(SyModule *mod) {
        this->setCurrRtlModInternal(mod);
    }

    void
    Expr2Gates::setCurrSynthMod(SyModule *mod) {
        this->setCurrSynthModInternal(mod);
    }

    void
    Expr2Gates::setCurrScope(SyScope *sc) {
        this->setCurrScopeInternal(sc);
    }    

    Expr2GatesRetType*
    Expr2Gates::getReturnElement() const
    {
        Expr2GatesRetType *elem = new Expr2GatesRetType();
        return elem;
    }    

    void 
    Expr2Gates::visit(const SyUnary* expr){
        result = auto_ptr<Expr2GatesRetType>(this->convert(expr));
    }    
    void 
    Expr2Gates::visit(const SyBinary* expr){
        result = auto_ptr<Expr2GatesRetType>(this->convert(expr));
    }    
    void 
    Expr2Gates::visit(const SyVPort* expr)
    {
        result = auto_ptr<Expr2GatesRetType>(this->convert(expr));
    }    
    void 
    Expr2Gates::visit(const SyScPort* expr){
        result = auto_ptr<Expr2GatesRetType>(this->convert(expr));
    }    
    void 
    Expr2Gates::visit(const SyVNet* expr)
    {
        result = auto_ptr<Expr2GatesRetType>(this->convert(expr));
    }    
    void 
    Expr2Gates::visit(const SyScNet* expr)
    {
        result = auto_ptr<Expr2GatesRetType>(this->convert(expr));
    }    
    void 
    Expr2Gates::visit(const SyScReg* expr){
        result = auto_ptr<Expr2GatesRetType>(this->convert(expr));
    }    
    void 
    Expr2Gates::visit(const SyVReg* expr)
    {
        result = auto_ptr<Expr2GatesRetType>(this->convert(expr));
    }    

    void 
    Expr2Gates::visit(const SyConcat* expr)
    {
        result = auto_ptr<Expr2GatesRetType>(this->convert(expr));
    }    


    void
    Expr2Gates::visit(const SyConst *const_)
    {
        Expr2GatesRetType* result_ = new Expr2GatesRetType();
        const string& val = const_->getVal();
        for(unsigned int i = 0; i < val.size(); ++i)
        {
            if(val[i] == '0') {
                result_->outputs->push_back(this->createVccOrGround(false));
            }
            else {
                result_->outputs->push_back(this->createVccOrGround(true));
            }
        }
        result = auto_ptr<Expr2GatesRetType>(result_);
    }
    void 
    Expr2Gates::visit(const SyBitSel* expr)
    {
        //This should not be reached if expr is on the lhs of an assign
        //statement.
        result = auto_ptr<Expr2GatesRetType>(this->createMux(expr));
        
    }    
    void 
    Expr2Gates::visit(const SyPartSel* expr)
    {
        const SyConcat *conc = expr->getConcat();
        conc->accept(this);
    }    

    void
    Expr2Gates::visit(const SyCondExpr *expr)
    {
        if(this->createTribufs(expr)) {
            return;
        }    
        else {
            this->createMuxes(expr);
        }
    }

    void
    Expr2Gates::visit(const SyXOrZ *xOrZ)
    {
        Expr2GatesRetType *encntvs = new Expr2GatesRetType();
        if(xOrZ->isZ())
        {
            encntvs->outputs->push_back(this->createVccOrGround(false));
            encntvs->outputs->push_front(this->createNet());
            result = auto_ptr<Expr2GatesRetType>(
            this->createCellInst(0, encntvs->outputs, 1, 2, TRIBUF));
        }    
        else 
        {
            result = auto_ptr<Expr2GatesRetType>(
            this->createCellInst(0, 0, 1, 1, BUF));
        }    
        delete encntvs;    
    }

    void
    Expr2Gates::visit(const SyFuncCall *funcCall)
    {
        SyScope *scope = this->getCurrScope();
        this->setCurrScope(const_cast<SyFunc*>(funcCall->getFunc()));
        FuncCall2Gates f2g;

        //result = auto_ptr<Expr2GatesRetType>(
        //    FuncCall2Gates::instance()->funcCall2Gates(
        //    this->getCurrRtlMod(), this->getCurrSynthMod(), funcCall));    
        result = auto_ptr<Expr2GatesRetType>(f2g.funcCall2Gates(
            this->getCurrRtlMod(), this->getCurrSynthMod(), 
            scope, funcCall));    
            
        this->setCurrScope(scope);    
    }
            

    bool
    Expr2Gates::createTribufs(const SyCondExpr *expr)
    {
        //Please refactor this function
        const SyExpr *fexpr = expr->getFalseExpr();
        if(const SyConcat *conc = dynamic_cast<const SyConcat*>(fexpr))
        {
            const SyList<SyExpr*>& bits = conc->getExprs();
            if(!dynamic_cast<const SyXOrZ*>(bits.head->val)) {
                return false;
            }
        }
        else if(!dynamic_cast<const SyXOrZ*>(fexpr)){
            return false;
        }    
        const SyExpr *texpr = expr->getTrueExpr();
        const SyExpr *cexpr = expr->getCond();
            
        texpr->accept(this);
        auto_ptr<Expr2GatesRetType> tcntvs = result;
        cexpr->accept(this);
        auto_ptr<Expr2GatesRetType> ccntvs = result;

        const int tsize = tcntvs->outputs->size();
        const int csize = ccntvs->outputs->size();
        int size = csize > tsize ? tsize : csize;

        Expr2GatesRetType *tmpCntvs = new Expr2GatesRetType();
        if(csize == 1) 
        {
            for(SyNode<SyCntv*>* node = tcntvs->outputs->head; node;
                node = node->next)
            {
                SyList<SyCntv*> input;
                input.push_back(node->val);
                input.push_back(ccntvs->outputs->head->val);
                Expr2GatesRetType *tmp = this->createCellInst(0, &input, 1, 2, TRIBUF);
                tmpCntvs->outputs->push_back(tmp->outputs->head->val);
                delete tmp;
            }
            result = auto_ptr<Expr2GatesRetType>(tmpCntvs);
            return true;
        }    
            

        SyNode<SyCntv*> *tnode = tcntvs->outputs->head;
        SyNode<SyCntv*> *cnode = ccntvs->outputs->head;

        for(int i = 0; i < (int)(tsize - size); ++i){
            tnode = tnode->next;
        }    
        for(int i = 0; i < (int)(csize - size); ++i){
            cnode = cnode->next;
        }    

        for(; tnode && cnode; tnode = tnode->next, cnode = cnode->next)
        {
            SyList<SyCntv*> input;
            input.push_back(tnode->val);
            input.push_back(cnode->val);
            Expr2GatesRetType *tmp = this->createCellInst(0, &input, 1, 2, TRIBUF);
            tmpCntvs->outputs->push_back(tmp->outputs->head->val);
            delete tmp;
        }    
        result = auto_ptr<Expr2GatesRetType>(tmpCntvs);
        return true;
    }    

    bool
    Expr2Gates::createMuxes(const SyCondExpr* expr)
    {
        //Please refactor this functions
        const SyExpr *texpr = expr->getTrueExpr();
        const SyExpr *fexpr = expr->getFalseExpr();
        const SyExpr *cexpr = expr->getCond();
            
        texpr->accept(this);
        auto_ptr<Expr2GatesRetType> tcntvs = result;
        cexpr->accept(this);
        auto_ptr<Expr2GatesRetType> ccntvs = result;
        fexpr->accept(this);
        auto_ptr<Expr2GatesRetType> fcntvs = result;

        const int tsize = tcntvs->outputs->size();
        const int fsize = fcntvs->outputs->size();

        Expr2GatesRetType *tmpCntvs = new Expr2GatesRetType();


        int diff = abs(tsize-fsize);
        if(tsize > fsize) {
            for(int i = 0; i < diff; ++i) {
                fcntvs->outputs->push_front(this->createVccOrGround(false));
            }
        }
        else  {
            for(int i = 0; i < diff; ++i) {
                tcntvs->outputs->push_front(this->createVccOrGround(false));
            }
        }
        SyCntv *sel = 0;
        if(ccntvs->outputs->size() == 1) {
            sel = ccntvs->outputs->head->val;
        }    
        else
        {
            SyConst *tconst = cexpr->size();
            int csize = tconst->toLong();
            delete tconst;
            SyList<SyCntv*> tlist;
            SyNode<SyCntv*> *node = ccntvs->outputs->head;
            int diff = ccntvs->outputs->size() - csize;
            for(int i = 0; i < diff; ++i) {
                node = node->next;
            }    
            for(; node; node = node->next) {
                tlist.push_back(node->val);
            }    
            auto_ptr<Expr2GatesRetType> ret(
                this->create2InputGateTree(&tlist, OR));
            sel = ret->outputs->head->val;
        }    
        assert(tcntvs->outputs->size() == fcntvs->outputs->size());
        SyNode<SyCntv*> *tnode = tcntvs->outputs->head;
        SyNode<SyCntv*> *fnode = fcntvs->outputs->head;

        for(; tnode && fnode; tnode = tnode->next, fnode = fnode->next) 
        {
            SyList<SyCntv*> inputs;
            inputs.push_back(tnode->val);
            inputs.push_back(fnode->val);
            inputs.push_back(sel);
            Expr2GatesRetType *tmp = this->createCellInst(0, &inputs, 1, 2, MUX);
            tmpCntvs->outputs->push_back(tmp->outputs->head->val);
            delete tmp;
        }    

        result = auto_ptr<Expr2GatesRetType>(tmpCntvs);
        return true;
        /*
        if(csize == 1)
        {
            int size = tsize > fsize ? fsize : tsize;

            for(int i = 0; i < (tsize - size); ++i){
                tnode = tnode->next;
            }    
            for(int i = 0; i < (fsize - size); ++i){
                fnode = fnode->next;
            }    

            for(; tnode && fnode; tnode = tnode->next, fnode = fnode->next)
            {
                SyList<SyCntv*> inputs;
                inputs.push_back(tnode->val);
                inputs.push_back(fnode->val);
                inputs.push_back(cnode->val);
                Expr2GatesRetType *tmp = this->createCellInst(0, &inputs, 1, 2, MUX);
                tmpCntvs->outputs->push_back(tmp->outputs->head->val);
                delete tmp;
            }    

            result = auto_ptr<Expr2GatesRetType>(tmpCntvs);
            return true;
        }    

        int size = tsize > fsize ? fsize : tsize;
        size = size > csize ? csize : size;
        for(int i = 0; i < (tsize - size); ++i){
            tnode = tnode->next;
        }    
        for(int i = 0; i < (fsize - size); ++i){
            fnode = fnode->next;
        }    
        
        for(int i = 0; i < (csize - size); ++i){
            cnode = cnode->next;
        }    
        for(; tnode && fnode && cnode; tnode = tnode->next, 
            fnode = fnode->next, cnode = cnode->next)
        {
            SyList<SyCntv*> inputs;
            inputs.push_back(tnode->val);
            inputs.push_back(fnode->val);
            inputs.push_back(cnode->val);
            Expr2GatesRetType *tmp = this->createCellInst(0, &inputs, 1, 2, MUX);
            tmpCntvs->outputs->push_back(tmp->outputs->head->val);
            delete tmp;
        }    

        result = auto_ptr<Expr2GatesRetType>(tmpCntvs);
        return true;
        */
    }    

    Expr2GatesRetType*    
    Expr2Gates::convert(const SyUnary *uexpr)
    {
        switch(uexpr->getOperator())
        {
            case SyExpr::UNPLUS:
                return this->convertUnaryPlus(uexpr);
            case SyExpr::UNMINUS:
                return this->convertUnaryMinus(uexpr);
            case SyExpr::LOGNEG:
                return this->convertLogicalNegation(uexpr);
            case SyExpr::BITNEG:
                return this->convertBitwiseNegation(uexpr);
            case SyExpr::REDAND:
                return this->convertReductionOperator(uexpr, AND);
            case SyExpr::REDNAND:
                return this->convertReductionOperator(uexpr, NAND);
            case SyExpr::REDOR:
                return this->convertReductionOperator(uexpr, OR);
            case SyExpr::REDNOR:
                return this->convertReductionOperator(uexpr, NOR);
            case SyExpr::REDXOR:
                return this->convertReductionOperator(uexpr, XOR);
            case SyExpr::REDXNOR:
                return this->convertReductionOperator(uexpr, XNOR);
            default: 
                assert(0);
                break;
        }
    }    

    Expr2GatesRetType*    
    Expr2Gates::convert(const SyBinary *bexpr)
    {
        if(SyConst *const_ = bexpr->evaluate()) 
        {
            const_->accept(this);
            delete const_;
            return result.get();
        }    

        switch(bexpr->getOperator())
        {
            case SyExpr::LOGAND:
            case SyExpr::LOGOR:
                return this->convertLogicalAndOr(bexpr);
            case SyExpr::GTHAN:
            case SyExpr::GTHANEQ:
            case SyExpr::LTHAN:
            case SyExpr::LTHANEQ:
            case SyExpr::LOGEQ:
            case SyExpr::LOGENEQ:
                return this->convertArithOps(bexpr);
            case SyExpr::LSHIFT:
            case SyExpr::RSHIFT:
            case SyExpr::RRSHIFT:
                return this->createLogicForShiftOperator(bexpr);
            case SyExpr::PLUS:
            case SyExpr::MINUS:
            case SyExpr::MULT:
            case SyExpr::DIV:
            case SyExpr::MOD:
                return this->createLogicForAddSubMulDivModOperators(bexpr);
            case SyExpr::BITXOR:
            case SyExpr::BITNXOR:
            case SyExpr::BITAND:
            case SyExpr::BITOR:
            case SyExpr::BITNAND:
            case SyExpr::BITNOR:
                return this->createLogicForBitAndOrXorXnorNandNorOperators(bexpr);
            default: break;
        }
        return 0;
    }    

    Expr2GatesRetType*
    Expr2Gates::convert(const SyVPort* rtlPort) 
    {
        return this->createRetElemForVecCntvs<SyVPort>(rtlPort);
    }    

    
    Expr2GatesRetType*
    Expr2Gates::convert(const SyScPort* rtlPort) 
    {
        return this->createRetElemForScCntvs<SyScPort>(rtlPort);
    }    



    Expr2GatesRetType*
    Expr2Gates::convert(const SyVNet* rtlNet) 
    {
        return this->createRetElemForVecCntvs<SyVNet>(rtlNet);
    }    

    
    Expr2GatesRetType*
    Expr2Gates::convert(const SyScNet* rtlNet) 
    {
        return this->createRetElemForScCntvs<SyScCntv>(rtlNet);
    }    


    Expr2GatesRetType*
    Expr2Gates::convert(const SyVReg* rtlReg) 
    {
        return this->createRetElemForVecCntvs<SyVReg>(rtlReg);
    }    

    
    Expr2GatesRetType*
    Expr2Gates::convert(const SyScReg* rtlReg) 
    {
        return this->createRetElemForScCntvs<SyScCntv>(rtlReg);
    }    

    Expr2GatesRetType*
    Expr2Gates::convert(const SyConcat* rtlConc) 
    {
        Expr2GatesRetType *retElem = this->getReturnElement();
        const SyList<SyExpr*>& bits = rtlConc->getExprs();

        for(SyNode<SyExpr*>* bit = bits.head; bit; bit = bit->next)
        {
            if(bit->val)
            {
                bit->val->accept(this);
                //retElem->outputs->push_back(result->outputs->head->val);
                retElem->outputs->merge(*result->outputs);
            }    
            else {
                retElem->outputs->push_back(this->createNet());
            }    
        }
        return retElem;
    }    

    Expr2GatesRetType*    
    Expr2Gates::convertUnaryPlus(const SyUnary *uplus) 
    {
        uplus->getExpr()->accept(this);
        return result.get();
    }

    Expr2GatesRetType*    
    Expr2Gates::convertUnaryMinus(const SyUnary *uminus) 
    {
        if(SyConst *const_ = uminus->evaluate()) 
        {
            const_->accept(this);
            delete const_;
            return result.get();
        }    
        uminus->getExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> result_ = result;
        Expr2GatesRetType *invret = new Expr2GatesRetType();
        for(SyNode<SyCntv*> *node = result_->outputs->head; node; node = node->next)
        {
            SyList<SyCntv*> prevout;
            prevout.push_back(node->val);
            Expr2GatesRetType *invcntvs = 
            this->createCellInst(NULL, &prevout, 1, 1, NOT);
            invret->outputs->push_back(invcntvs->outputs->head->val);    
            invret->inputs->push_back(invcntvs->inputs->head->val);    
            delete invcntvs;
        }
        Expr2GatesRetType *incret = this->createCellInst(
            NULL, invret->outputs, invret->outputs->size(), 
            invret->outputs->size(), INC);
        delete invret;
        return incret;
    }

    Expr2GatesRetType*
    Expr2Gates::convertLogicNegation(const SyUnary *logneg)
    {
        //TBD
        logneg->getExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> result_ = result;
        return 0;
    }

    Expr2GatesRetType*
    Expr2Gates::convertBitwiseNegation(const SyUnary *bitneg)
    {
        bitneg->getExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> result_ = result;
        Expr2GatesRetType *ret = new Expr2GatesRetType();

        for(SyNode<SyCntv*>* node = result_->outputs->head; node; node = node->next)
        {
            SyList<SyCntv*> prevout;
            prevout.push_back(node->val);
            Expr2GatesRetType *tmp = this->createCellInst(0, &prevout, 1, 1, NOT);
            ret->inputs->push_back(node->val);
            ret->outputs->push_back(tmp->outputs->head->val);
            delete tmp;
        }
        return ret;
    }

    Expr2GatesRetType*
    Expr2Gates::convertLogicalNegation(const SyUnary *logNeg)
    {
        logNeg->getExpr()->accept(this);

        if(result->outputs->size() == 1) {
            return this->createCellInst(0, result->outputs, 1, 1, NOT);
        }    
        auto_ptr<Expr2GatesRetType> ret(
            this->create2InputGateTree(result->outputs, OR));
        return this->createCellInst(0, ret->outputs, 1, 1, NOT);
    }    
        

    Expr2GatesRetType*
    Expr2Gates::convertReductionOperator(const SyUnary *redop, CellType type)
    {
        bool inv = false;
        if(type == NAND) 
        {
            type = AND;
            inv = true;
        }
        else if(type == NOR) 
        {
            type = OR;
            inv = true;
        }
        else if(type == XNOR) 
        {
            type = XOR;
            inv = true;
        }

        redop->getExpr()->accept(this);
        //assert(result->outputs->size() > 1); //only on vectors
        if(result->outputs->size() == 1) {
            if(inv)
            {
                Expr2GatesRetType *ret = new Expr2GatesRetType();
                SyCntv *net = this->createNet();
                this->createInv(net, result->outputs->head->val);
                ret->outputs->push_back(net);
                return ret;
            }
            else {
                return result.get();
            }    
        }    
        auto_ptr<Expr2GatesRetType> result_ = result;
        Expr2GatesRetType *ret = new Expr2GatesRetType();
        SyList<SyCntv*> lastoutput;
        SyList<SyCntv*> firstinputs;
        SyNode<SyCntv*>* node = result_->outputs->head;
        firstinputs.push_back(node->val);
        firstinputs.push_back(node->next->val);
        ret->inputs->push_back(node->val);
        ret->inputs->push_back(node->next->val);
        Expr2GatesRetType *firstCntvs = this->createCellInst(0, &firstinputs, 1, 2, type);
        lastoutput = *(firstCntvs->outputs);
        node = node->next->next;
        for(; node; node = node->next)
        {
            SyList<SyCntv*> input;
            input.push_back(node->val);
            input.push_back(lastoutput.head->val);
            Expr2GatesRetType *cntvs = this->createCellInst(0, &input, 1, 2, type);
            ret->inputs->push_back(node->val);
            lastoutput.clear();
            lastoutput.push_back(cntvs->outputs->head->val);
            delete cntvs;
        }    
        if(inv)
        {
            SyCntv *net = this->createNet();
            this->createInv(net, lastoutput.tail->val);
            ret->outputs->push_back(net);
        }
        else {
            *(ret->outputs) = lastoutput;
        }    
        delete firstCntvs;
        return ret;
    }


    Expr2GatesRetType*
    Expr2Gates::convertLogicalAndOr(const SyBinary *logandor)
    {
        logandor->getLExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> lhscntvs = result;

        logandor->getRExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> rhscntvs = result;

        Expr2GatesRetType* inouts = new Expr2GatesRetType();
        

        if(lhscntvs->outputs->size() > 1)
        {
            auto_ptr<Expr2GatesRetType> result_(
                this->create2InputGateTree(lhscntvs->outputs, OR));
            inouts->inputs->push_back(result_->outputs->head->val);
        }    
        else {
            inouts->inputs->push_back(lhscntvs->outputs->head->val);
        }    
        
        if(rhscntvs->outputs->size() > 1)
        {
            auto_ptr<Expr2GatesRetType> result_(
                this->create2InputGateTree(rhscntvs->outputs, OR));
            inouts->inputs->push_back(result_->outputs->head->val);
        }    
        else {
            inouts->inputs->push_back(rhscntvs->outputs->head->val);
        }    

        Expr2GatesRetType *ret = this->createCellInst(
            0, inouts->inputs, 1, 2, 
            logandor->getOperator() == SyExpr::LOGAND ? AND : OR);
        delete inouts;
        return ret;
    }

    Expr2GatesRetType*
    Expr2Gates::convertArithOps(const SyBinary *arith)
    {
        arith->getLExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> lhscntvs = result;

        arith->getRExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> rhscntvs = result;

        //Expr2GatesRetType* inouts = new Expr2GatesRetType();
        SyConst *lsize = arith->getLExpr()->size();
        SyConst *rsize = arith->getRExpr()->size();

        if(lsize->toLong() < lhscntvs->outputs->size() ) 
        {
            int diff = lhscntvs->outputs->size() - (int) lsize->toLong();
            for(int i = 0; i < diff; ++i) {
                lhscntvs->outputs->remove_front();
               } 
        }    
        if(rsize->toLong() < rhscntvs->outputs->size() ) 
        {
            int diff = rhscntvs->outputs->size() - (int) rsize->toLong();
            for(int i = 0; i < diff; ++i) {
                rhscntvs->outputs->remove_front();
               } 
        }    
        delete lsize;
        delete rsize;
        int size1 = lhscntvs->outputs->size();
        int size2 = rhscntvs->outputs->size();

        const int size = std::max(size1, size2);

        SyList<SyCntv*>* cmpinputs = new SyList<SyCntv*>();
        *cmpinputs = *lhscntvs->outputs;
        for(int i = 0; i < (int)(size-size1); ++i) {
            cmpinputs->push_front(this->createVccOrGround(false));
        }    
        SyList<SyCntv*>* tlist = new SyList<SyCntv*>();
        *tlist = *rhscntvs->outputs;
        for(int i = 0; i < (int)(size-size2); ++i) {
            tlist->push_front(this->createVccOrGround(false));
        }    
        cmpinputs->splice(tlist);

        Expr2GatesRetType *cmpouts = this->createCellInst(
            0, cmpinputs, size, size, CMP);
        
        
        //SyModInst *cmpInst = cmpouts->gateInsts->head->val;
        cmpouts->inputs->clear();
        cmpouts->outputs->clear();
        cmpouts->inputs->splice(cmpinputs);

        switch(arith->getOperator())
        {
            case SyExpr::LOGEQ:
                return this->createLogicForLogEq(cmpouts);
            case SyExpr::LOGENEQ:
                return this->createLogicForLogIneq(cmpouts);
            case SyExpr::GTHAN:    
                return this->createLogicForLogGThan(cmpouts);
            case SyExpr::GTHANEQ:    
                return this->createLogicForLogGTE(cmpouts);
            case SyExpr::LTHAN:    
                return this->createLogicForLogLThan(cmpouts);
            case SyExpr::LTHANEQ:    
                return this->createLogicForLogLTE(cmpouts);
            default: break;
        }
        return 0; 
    }    




    
    Expr2GatesRetType*    
    Expr2Gates::createLogicForLogGThan(Expr2GatesRetType *cmpouts)
    {
        SyModInst *cmpInst = cmpouts->gateInsts->head->val;
        cmpouts->outputs->clear();
        cmpouts->outputs->push_back(const_cast<SyCntv*>(
            dynamic_cast<const SyCntv*>(cmpInst->getPin(
            SyIxName("GT"))->getActual())));
        //remove unwanted connections
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("EQ")))->setActual(0);
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("INEQ")))->setActual(0);
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("LT")))->setActual(0);
        return cmpouts;    
          
    }

    Expr2GatesRetType*    
    Expr2Gates::createLogicForLogLThan(Expr2GatesRetType *cmpouts)
    {
        SyModInst *cmpInst = cmpouts->gateInsts->head->val;
        cmpouts->outputs->clear();
        cmpouts->outputs->push_back(const_cast<SyCntv*>(
            dynamic_cast<const SyCntv*>(cmpInst->getPin(
            SyIxName("LT"))->getActual())));
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("EQ")))->setActual(0);
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("INEQ")))->setActual(0);
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("GT")))->setActual(0);
        return cmpouts;    
    }


    Expr2GatesRetType*    
    Expr2Gates::createLogicForLogGTE(Expr2GatesRetType *cmpouts)
    {
        //INEQ pin is high or EQ pin is high
        SyModInst *cmpInst = cmpouts->gateInsts->head->val;
        cmpouts->outputs->clear();
        Expr2GatesRetType orins;
        orins.inputs->push_back(const_cast<SyCntv*>(
            dynamic_cast<const SyCntv*>(
            cmpInst->getPin(SyIxName("GT"))->getActual())));

        orins.inputs->push_back(const_cast<SyCntv*>(
            dynamic_cast<const SyCntv*>(
            cmpInst->getPin(SyIxName("EQ"))->getActual())));

        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("INEQ")))->setActual(0);
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("LT")))->setActual(0);
        return this->createCellInst(0, orins.inputs, 1, 2, OR);
    }

    Expr2GatesRetType*    
    Expr2Gates::createLogicForLogLTE(Expr2GatesRetType *cmpouts)
    {
        //INEQ pin is Low or EQ pin is high
        SyModInst *cmpInst = cmpouts->gateInsts->head->val;
        cmpouts->outputs->clear();
        Expr2GatesRetType orins;

        orins.inputs->push_back(const_cast<SyCntv*>(
            dynamic_cast<const SyCntv*>( 
            cmpInst->getPin(SyIxName("EQ"))->getActual())));
        orins.inputs->push_back(const_cast<SyCntv*>(
            dynamic_cast<const SyCntv*>( 
            cmpInst->getPin(SyIxName("LT"))->getActual())));

        
        cmpouts->gateInsts->clear();
        
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("INEQ")))->setActual(0);
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("GT")))->setActual(0);

        return this->createCellInst(0, orins.inputs, 1, 2, OR);
    }
    Expr2GatesRetType*    
    Expr2Gates::createLogicForLogEq(Expr2GatesRetType *cmpouts)
    {
        SyModInst *cmpInst = cmpouts->gateInsts->head->val;
        cmpouts->outputs->clear();
        cmpouts->outputs->push_back(const_cast<SyCntv*>(
            dynamic_cast<const SyCntv*>(
            cmpInst->getPin(SyIxName("EQ"))->getActual())));

        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("INEQ")))->setActual(0);
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("GT")))->setActual(0);
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("LT")))->setActual(0);

        return cmpouts;
    }    

    Expr2GatesRetType*    
    Expr2Gates::createLogicForLogIneq(Expr2GatesRetType *cmpouts)
    {
        SyModInst *cmpInst = cmpouts->gateInsts->head->val;
        cmpouts->outputs->clear();
        cmpouts->outputs->push_back(const_cast<SyCntv*>(
            dynamic_cast<const SyCntv*>(cmpInst->getPin(
            SyIxName("INEQ"))->getActual())));

        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("EQ")))->setActual(0);
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("GT")))->setActual(0);
        const_cast<SyScPin*>(cmpInst->getPin(SyIxName("LT")))->setActual(0);
        
        return cmpouts;

    }    

    Expr2GatesRetType*
    Expr2Gates::createLogicForShiftOperator(const SyBinary *shiftExpr)
    {
        shiftExpr->getLExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> lhscntvs = result;
        //int shiftVal = shiftExpr/*->getLExpr()*/->isSigned() ? 1 : 0;
        int shiftVal = 0;
        if(shiftExpr->isSigned() && 
            (shiftExpr->getOperator() == SyExpr::RRSHIFT)) {
            shiftVal = 1;
        }    
        bool leftShift = (shiftExpr->getOperator() == SyExpr::LSHIFT);

        if(const SyConst *const_ = dynamic_cast<const SyConst*>(
            shiftExpr->getRExpr()))
        {
            const_cast<SyConst*>(const_)->setSign(false); //always unsigned
            long shift = const_->toLong();
            return this->createLogicForConstShift(
                lhscntvs.get(), leftShift, shift, shiftVal);

        }
        else {
            return this->createLogicForVariableShift(
                lhscntvs.get(), leftShift, shiftExpr->getRExpr(), shiftVal);
        }        
        return 0;
    }    
            

    Expr2GatesRetType*
    Expr2Gates::createLogicForConstShift(    Expr2GatesRetType     *outs,
                                            bool                isLeftShift,
                                            short                 shift,
                                            short                shiftVal)
    {
        Expr2GatesRetType *ret = new Expr2GatesRetType();
        vector<SyCntv*> cntvs;
        for(SyNode<SyCntv*>* node = outs->outputs->head; node;
            node = node->next) {
            cntvs.push_back(node->val);
        }    
        for(int i = 0; i < shift; ++i)
        {

            if(shiftVal == 0) 
            {
                auto_ptr<Expr2GatesRetType> tmp(
                    this->createCellInst(0, 0, 1, 0, GND));
                ret->outputs->push_back(tmp->outputs->head->val);    
            }
            else {
                auto_ptr<Expr2GatesRetType> tmp(
                    this->createCellInst(0, 0, 1, 0, VCC));
                ret->outputs->push_back(tmp->outputs->head->val);    
            }
        }    
        if(isLeftShift) {
            int count = (cntvs.size()-shift);
            int size = cntvs.size()-1;
            for(int i = 0; i < count; ++i) {
                ret->outputs->push_front(cntvs[size-i]);
            }
        }
        else {
            for(int i = 0; i < (int)(cntvs.size()-shift); ++i) {
                ret->outputs->push_back(cntvs[i]);
            }
        }    
        return ret;

    }    


    Expr2GatesRetType*
    Expr2Gates::createLogicForVariableShift(    
                                Expr2GatesRetType *outs,
                                bool    isLeftShift,
                                const    SyExpr *shift,
                                short    shiftVal)
    {
        shift->accept(this);
        auto_ptr<Expr2GatesRetType> shiftCntvs = result;
        int inputSize = outs->outputs->size();
        int shiftSize = BitCount(inputSize);

        SyList<SyCntv*> shifterInputs = *outs->outputs;
        shiftCntvs->outputs->reverse();
        int i = 0;
        SyList<SyCntv*>* tmp = new SyList<SyCntv*>();
        for(SyNode<SyCntv*>* node = shiftCntvs->outputs->head; 
            node && (i < shiftSize); node = node->next, ++i) {
            tmp->push_front(node->val);
            //shifterInputs.push_back(node->val);
        }    

        for(; i < shiftSize; ++i){
            //For variable smaller in size that max shift size,
            //fill remaining msb bits with 0
            tmp->push_front(this->createVccOrGround(false));
        }    
        shifterInputs.splice(tmp);

        shifterInputs.push_back(this->createVccOrGround(!isLeftShift));
        shifterInputs.push_back(this->createVccOrGround(shiftVal));

        return this->createCellInst(0, &shifterInputs, inputSize, inputSize, SHIFTER);
    }    


    SyCntv*
    Expr2Gates::createVccOrGround(bool isVcc) 
    {
        auto_ptr<Expr2GatesRetType> tmp(
                isVcc ? this->createCellInst(0, 0, 1, 0, VCC):
                this->createCellInst(0, 0, 1, 0, GND));
        return tmp->outputs->head->val;
    }    
        


    Expr2GatesRetType*
    Expr2Gates::createLogicForAddSubMulDivModOperators(const SyBinary *expr)
    {
        //bool isSigned = expr->isSigend();
        
        expr->getLExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> lhscntvs = result;
        expr->getRExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> rhscntvs = result;

        int size = lhscntvs->outputs->size();
        int lhssize = size;
        int rhssize = rhscntvs->outputs->size();

        if(size < rhssize) {
            size = rhssize;
        }

        for(; lhssize < size; ++lhssize){
            lhscntvs->outputs->push_front(this->createVccOrGround(false));
        }

        for(; rhssize < size; ++rhssize) {
            rhscntvs->outputs->push_front(this->createVccOrGround(false));
        }
        SyList<SyCntv*> inputs = *lhscntvs->outputs;
        inputs.merge(*rhscntvs->outputs);


        switch (expr->getOperator())
        {
            case SyExpr::PLUS:
            {
                //For CIN
                inputs.push_back(this->createVccOrGround(false));
                return this->createCellInst(0, &inputs, size, size, ADD);
            }    
            case SyExpr::MINUS:    
            {
                //For BIN
                inputs.push_back(this->createVccOrGround(false));
                return this->createCellInst(0, &inputs, size, size, SUB);
            }    
            case SyExpr::MULT:    
                return this->createCellInst(0, &inputs, size, size, MUL);
            case SyExpr::DIV:    
                return this->createCellInst(0, &inputs, size, size, DIV);
            case SyExpr::MOD:    
            {
                //Bring the nets connected with REM to the beginnig of the list
                Expr2GatesRetType *ret = this->createCellInst(0, &inputs, size, size, DIV);
                SyNode<SyCntv*>* node = ret->outputs->head;
                for(int i = 0; i < size; ++i) {
                    node = node->next;
                }
                vector<SyCntv*> tmp;
                for(int i = 0; i < size; ++i) {
                    tmp.push_back(node->val);
                    node = node->next;
                }    
                for(int i = 0; i < tmp.size(); ++i) {
                    ret->outputs->remove(tmp[i]);
                }    
                return ret;
            }    
            default:
                assert(0);
        }
        return 0;
    }    

    Expr2GatesRetType*
    Expr2Gates::createLogicForBitAndOrXorXnorNandNorOperators(const SyBinary *expr)
    {
        expr->getLExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> lhscntvs = result;
        expr->getRExpr()->accept(this);
        auto_ptr<Expr2GatesRetType> rhscntvs = result;

        int size = lhscntvs->outputs->size();
        int lhssize = size;
        int rhssize = rhscntvs->outputs->size();

        if(size < rhssize) {
            size = rhssize;
        }

        for(int i = 0; i < (int)(size-rhssize); ++i){
            rhscntvs->outputs->push_front(this->createVccOrGround(false));
        }    
        for(int i = 0; i < size-lhssize; ++i){
            lhscntvs->outputs->push_front(this->createVccOrGround(false));
        }    

        Expr2GatesRetType *ret = new Expr2GatesRetType();

        SyNode<SyCntv*>* node1 = lhscntvs->outputs->head;
        SyNode<SyCntv*>* node2 = rhscntvs->outputs->head;
        for(; node1 && node2; node1 = node1->next, node2 = node2->next)
        {
            SyList<SyCntv*> cntvs;
            cntvs.push_back(node1->val);
            cntvs.push_back(node2->val);
            ret->inputs->push_back(node1->val);
            ret->inputs->push_back(node2->val);

            Expr2GatesRetType *tmpret = 0;
            switch(expr->getOperator())
            {
                case SyExpr::BITAND:
                {
                    tmpret = this->createCellInst(0, &cntvs, 1, 2, AND);
                    break;
                }
                case SyExpr::BITOR:
                {
                    tmpret = this->createCellInst(0, &cntvs, 1, 2, OR);
                    break;
                }
                case SyExpr::BITXOR:
                {
                    tmpret = this->createCellInst(0, &cntvs, 1, 2, XOR);
                    break;
                }
                case SyExpr::BITNXOR:
                {
                    tmpret = this->createCellInst(0, &cntvs, 1, 2, XNOR);
                    break;
                }
                case SyExpr::BITNOR:
                {
                    tmpret = this->createCellInst(0, &cntvs, 1, 2, NOR);
                    break;
                }
                case SyExpr::BITNAND:
                {
                    tmpret = this->createCellInst(0, &cntvs, 1, 2, NAND);
                    break;
                }
                default: assert(0);
            }    
            ret->outputs->push_back(tmpret->outputs->head->val);
        }    
        return ret;
    }    
                    

        

    Expr2GatesRetType*
    Expr2Gates::createCellInst(    SyList<SyCntv*> *outs, 
                                SyList<SyCntv*> *ins,
                                short outsize,
                                short insize,
                                CellType type)
    {
        GenCellBase *cellMod = GenCellMgr::instance()->create(type, outsize, insize);
        SyRoot::switchStage();
        SyRoot::instance()->addCellMod(cellMod);
        SyRoot::switchStage();
        SyModInst *cellInst = new SyModInst(ModuleNameManager::instance()->createName(
            this->getCurrSynthMod(), CellType2String(type)), cellMod, cellMod->getName());
        this->getCurrSynthMod()->addModInst(cellInst);
        this->getCurrSynthMod()->addObject(cellInst->getIxName().toString(), cellInst);
        Expr2GatesRetType *retelem = this->getReturnElement();
        const SyList<SyScPort*>& ports = cellMod->getPorts();
        SyNode<SyScPort*>* pnode = ports.head;

        if(outs)
        {
            SyNode<SyCntv*>* node = outs->head;    
            while(const SyCntv *cntv = this->getNextPort(cellMod, pnode, SyScPort::OUT))
            {
                if(const SyVPort *port = dynamic_cast<const SyVPort*>(cntv))
                {
                    int msb = port->getMsb();
                    SyConcat *conc = new SyConcat(port->isSigned());
                    for(int i = 0; (i <= msb) && node ; ++i, node = node->next){
                        conc->addBit(node->val);
                        retelem->outputs->push_back(node->val);
                    }
                    SyVPin *pin = new SyVPin(port->getName(), msb, 0, conc, cellInst);
                    cellInst->addVecPin(pin);
                    const SyList<SyScCntv*>& bits = pin->getBits();
                    for(SyNode<SyScCntv*>* bit = bits.head; bit; bit = bit->next){
                        cellInst->addPin(dynamic_cast<SyScPin*>(bit->val));
                    }    
                }
                else  if(const SyScPort *port = dynamic_cast<const SyScPort*>(cntv)){
                    SyScPin *outpin = new SyScPin(
                        port->getIxName(), node->val, cellInst);
                    retelem->outputs->push_back(node->val);
                    cellInst->addPin(outpin);
                    node = node->next;
                }
            }    
        }    
                    
        while(const SyCntv *cntv = this->getNextPort(cellMod, pnode, SyScPort::OUT))
        {
            if(const SyVPort *port = dynamic_cast<const SyVPort*>(cntv))
            {
                int msb = port->getMsb();
                SyConcat *conc = new SyConcat(port->isSigned());
                for(int i = 0; (i <= msb) ; ++i){
                    SyScNet *net = this->createNet();
                    net->setSign(port->isSigned());
                    conc->addBit(net);
                    retelem->outputs->push_back(net);
                }
                SyVPin *pin = new SyVPin(port->getName(), msb, 0, conc, cellInst);
                cellInst->addVecPin(pin);
                const SyList<SyScCntv*>& bits = pin->getBits();
                for(SyNode<SyScCntv*>* bit = bits.head; bit; bit = bit->next){
                    cellInst->addPin(dynamic_cast<SyScPin*>(bit->val));
                }    
            }
            else  if(const SyScPort *port = dynamic_cast<const SyScPort*>(cntv)){
                SyScNet *net = this->createNet();
                SyScPin *outpin = new SyScPin(
                    port->getIxName(), net, cellInst);
                retelem->outputs->push_back(net);
                cellInst->addPin(outpin);
            }
        }    


        pnode = ports.head;
        if(ins)
        {
            SyNode<SyCntv*>* node = ins->head;    
            while(const SyCntv *cntv = this->getNextPort(cellMod, pnode, SyScPort::IN))
            {
                if(const SyVPort *port = dynamic_cast<const SyVPort*>(cntv))
                {
                    int msb = port->getMsb();
                    SyConcat *conc = new SyConcat(port->isSigned());
                    for(int i = 0; (i <= msb) && node ; ++i, node = node->next){
                        conc->addBit(node->val);
                        retelem->inputs->push_back(node->val);
                    }
                    SyVPin *pin = new SyVPin(port->getName(), msb, 0, conc, cellInst);
                    cellInst->addVecPin(pin);
                    const SyList<SyScCntv*>& bits = pin->getBits();
                    for(SyNode<SyScCntv*>* bit = bits.head; bit; bit = bit->next){
                        cellInst->addPin(dynamic_cast<SyScPin*>(bit->val));
                    }    
                }
                else  if(const SyScPort *port = dynamic_cast<const SyScPort*>(cntv)){
                    SyScPin *inpin = new SyScPin(
                        port->getIxName(), node->val, cellInst);
                    retelem->inputs->push_back(node->val);
                    cellInst->addPin(inpin);
                    node = node->next;
                }
            }    
        }
        while(const SyCntv *cntv = this->getNextPort(cellMod, pnode, SyScPort::IN))
        {
            if(const SyVPort *port = dynamic_cast<const SyVPort*>(cntv))
            {
                int msb = port->getMsb();
                SyConcat *conc = new SyConcat(port->isSigned());
                for(int i = 0; (i <= msb) ; ++i){
                    SyScNet *net = this->createNet();
                    net->setSign(port->isSigned());
                    conc->addBit(net);
                    retelem->inputs->push_back(net);
                }
                SyVPin *pin = new SyVPin(port->getName(), msb, 0, conc, cellInst);
                cellInst->addVecPin(pin);
                const SyList<SyScCntv*>& bits = pin->getBits();
                for(SyNode<SyScCntv*>* bit = bits.head; bit; bit = bit->next){
                    cellInst->addPin(dynamic_cast<SyScPin*>(bit->val));
                }    
            }
            else  if(const SyScPort *port = dynamic_cast<const SyScPort*>(cntv)){
                SyScNet *net = this->createNet();
                SyScPin *inpin = new SyScPin(
                    port->getIxName(), net, cellInst);
                retelem->inputs->push_back(net);
                cellInst->addPin(inpin);
            }
        }
        retelem->gateInsts = new SyList<SyModInst*>();
        retelem->gateInsts->push_back(cellInst);

        return retelem;    

    }


    const SyCntv*
    Expr2Gates::getNextPort(const SyModule *mod, 
                            SyNode<SyScPort*>*& node, 
                            SyScPort::SyDirType dir) const
    {
        if(!node) {
            return 0;
        }
        while(node && (node->val->getDir() != dir)){
            node = node->next;
        }    
        if(!node) {
            return 0;
        }
        if(!node->val->isVecBit()) 
        {
            const SyScPort *port = node->val;
            node = node->next;
            return port;
        }    

        string pname = node->val->getIxName().getName();

        for(; node && (node->val->getIxName().getName() == pname); 
            node = node->next)
        {
        }

        return dynamic_cast<const SyCntv*>(mod->findObject(pname));
    }    


    Expr2GatesRetType*
    Expr2Gates::create2InputGateTree(
                                SyList<SyCntv*>* ins,
                                CellType type)
    {                            
        Expr2GatesRetType *ret = new Expr2GatesRetType();
        SyList<SyCntv*> firstinputs;
        SyList<SyCntv*> lastoutput;
        SyNode<SyCntv*>* node = ins->head;
        firstinputs.push_back(node->val);
        firstinputs.push_back(node->next->val);
        ret->inputs->push_back(node->val);
        ret->inputs->push_back(node->next->val);
        Expr2GatesRetType *firstCntvs = this->createCellInst(0, &firstinputs, 1, 2, type);
        lastoutput = *(firstCntvs->outputs);
        node = node->next->next;
        for(; node; node = node->next)
        {
            SyList<SyCntv*> input;
            input.push_back(node->val);
            input.push_back(lastoutput.head->val);
            Expr2GatesRetType *cntvs = this->createCellInst(0, &input, 1, 2, type);
            ret->inputs->push_back(node->val);
            lastoutput.clear();
            lastoutput.push_back(cntvs->outputs->head->val);
            delete cntvs;
        }    
        *(ret->outputs) = lastoutput;
        delete firstCntvs;
        return ret;
    }    

/*
    Expr2GatesRetType*
    Expr2Gates::createMuxForDimension(const SyBitSel *bitSel, int num) const
    {
        const vector<SyExpr*>& dim = bitSel->getDim();
        assert(num >= 0 && num < dim.size());
        const SyExpr *var = bitSel->getVar();
    }
*/    

    Expr2GatesRetType*
    Expr2Gates::createMux(const SyBitSel *bitSel)
    {
        if(SyScCntv *cntv = dynamic_cast<SyScCntv*>(bitSel->copy()))
        {
            cntv->accept(this);
            return result.get();
        }

        const SyExpr *var = bitSel->getVar();
        const SyExpr *selExpr = bitSel->getFirstDim();

        var->accept(this);
        auto_ptr<Expr2GatesRetType> varCntvs = result;

        selExpr->accept(this);
        auto_ptr<Expr2GatesRetType> selCntvs = result;

        int varSize = varCntvs->outputs->size();
        int selSize = selCntvs->outputs->size();
        int maxSelSize = Log2Ceiling(varSize);

        SyNode<SyCntv*>* selNode = selCntvs->outputs->head;
        if(selSize > maxSelSize) 
        {
            for(int i = 0; i < (int)(selSize - maxSelSize); ++i) {
                selNode = selNode->next;
            }
        }

        SyList<SyCntv*> select;
        for(; selNode; selNode = selNode->next) {
            select.push_back(selNode->val);
        }
        SyList<SyCntv*> inputs;
        inputs = *(varCntvs->outputs);
        inputs.merge(select);

        return this->createCellInst(0, &inputs, 1, varSize, MUX);
    }    

    Expr2GatesRetType*
    Expr2Gates::createDecoder(const SyBitSel *bitSel)
    {
        if(SyScCntv *cntv = dynamic_cast<SyScCntv*>(bitSel->copy()))
        {
            cntv->accept(this);
            return result.get();
        }

        const SyExpr *var = bitSel->getVar();
        const SyExpr *inExpr = bitSel->getFirstDim();

        var->accept(this);
        auto_ptr<Expr2GatesRetType> varCntvs = result;

        inExpr->accept(this);
        auto_ptr<Expr2GatesRetType> inCntvs = result;
        int varSize = varCntvs->outputs->size();
        int inSize = inCntvs->outputs->size();
        int maxInSize = Log2Ceiling(varSize);

        SyNode<SyCntv*>* inNode = inCntvs->outputs->head;
        if(inSize > maxInSize) 
        {
            for(int i = 0; i < (int)(inSize - maxInSize); ++i) {
                inNode = inNode->next;
            }
        }

        SyList<SyCntv*> decInput;
        for(; inNode; inNode = inNode->next) {
            decInput.push_back(inNode->val);
        }
        decInput.push_back(createVccOrGround(true));

        int maxOutSize = pow (2, decInput.size());
        SyNode<SyCntv*>* outNode = varCntvs->outputs->head;

        for(int i = 0; i < (int)(varSize-maxOutSize); ++i) {
            outNode = outNode->next;
        }    
        SyList<SyCntv*> decOutput;

        for(; outNode; outNode = outNode->next) {
            decOutput.push_back(outNode->val);
        }

        return this->createCellInst(0/*&decOutput*/, &decInput, decOutput.size(),
            decInput.size(), DEC);
    }        

    void
    Expr2Gates::createDecoderLogicForAssign(const SyBitSel *bitSel, 
                                            const SyExpr *expr)
    {
        const SyExpr *var = bitSel->getVar();
        Expr2GatesRetType *decResult = this->createDecoder(bitSel);
        expr->accept(this);
        auto_ptr<Expr2GatesRetType> exprResult = result;
        var->accept(this);
        auto_ptr<Expr2GatesRetType> varResult = result;

        int actSize = decResult->outputs->size();
        SyNode<SyCntv*>* varNode = varResult->outputs->head;
        int varSize = varResult->outputs->size();

        for(int i = 0; i < (int)(varSize - actSize); ++i) {
            varNode = varNode->next;
        }

        SyList<SyCntv*> varList;

        for(; varNode; varNode = varNode->next) {
            varList.push_back(varNode->val);
        }
        SyNode<SyCntv*>* decNode = decResult->outputs->head;
        Expr2GatesRetType *tmp = new Expr2GatesRetType();

        for(varNode = varList.head; varNode && decNode; 
            varNode = varNode->next, decNode = decNode->next)
        {
            for(SyNode<SyCntv*>* exprNode = exprResult->outputs->head; exprNode;
                exprNode = exprNode->next)
            {
                SyList<SyCntv*> inputs;
                SyList<SyCntv*> outputs;

                inputs.push_back(decNode->val);
                inputs.push_back(exprNode->val);
                outputs.push_back(varNode->val);

                tmp->outputs->merge(*(this->createCellInst(0, &inputs, 1, 2, AND)->outputs));
            }
        }
        result = auto_ptr<Expr2GatesRetType>(tmp);

    }


    //-----------------Stmt Converter---------------------------------

    StmtsConverter::StmtsConverter(const SyList<SyScCntv*>& f,
        const SyList<SyCntv*>& a, const SyList<SyStmt*>& s) :
        formals(f), actuals(a), stmts(s) {
        this->createFormalActualMap();
    }

    StmtsConverter::~StmtsConverter() {
    }

    SyList<SyStmt*>
    StmtsConverter::operator()() {
        return this->convert();
    }    


    SyExpr*
    StmtsConverter::getActual(SyCntv* formal) const
    {
        if(!formal) {
            return 0;
        }    
        FormalActualMap::const_iterator iter = formActMap.find(formal);

        if(iter == formActMap.end()) {
            return 0;
        }
        return iter->second;
    }

    void
    StmtsConverter::addActual(SyCntv *formal, SyExpr *actual) 
    {
        if(!formal || !actual) {
            return;
        }
        formActMap.erase(formal);
        formActMap.insert(make_pair(formal, actual));
    }

    void
    StmtsConverter::createFormalActualMap()
    {
        assert(formals.size() == actuals.size());
        
        SyNode<SyScCntv*>* fnode = formals.head;
        SyNode<SyCntv*>* anode = actuals.head;

        for(; anode && fnode; anode = anode->next, fnode = fnode->next)
        {
            formActMap.insert(make_pair(fnode->val, anode->val));
        }
    }

    SyList<SyStmt*>
    StmtsConverter::convert()
    {
        SyList<SyStmt*> retstmts;
        for(SyNode<SyStmt*>* node = stmts.head; node; node = node->next)
        {
            SyStmt *stmt = node->val->copy();
            FormalActualMap::const_iterator iter = formActMap.begin();
            for(; iter != formActMap.end(); ++iter) {
                stmt->repSubExpr(iter->first->toString(), iter->second);
            }    
            retstmts.push_back(stmt);
        }
        return retstmts;
    }

    
    FuncCall2Gates::FuncCall2Gates(): retOutCntvSet(0) {
    }

    FuncCall2Gates::~FuncCall2Gates() {
    }
    void
    FuncCall2Gates::clear() 
    {
        retOutCntvSet = 0;
        retOut2Rep.clear();
    }    


    Out2Rep
    FuncCall2Gates::getOut2Rep() {
        return retOut2Rep;
    }

    OutCntvInfoSet*
    FuncCall2Gates::getOutCntvInfoSet() {
     return retOutCntvSet;
    }    


    SyList<SyStmt*>
    FuncCall2Gates::createStmts(SyModule *rtlMod,
        SyModule *synthMod, const SyFuncCall *funcCall)
    {
        assert(funcCall);


        const SyFunc *func = funcCall->getFunc();
        Expr2Gates expr2Gates(rtlMod, synthMod, func);
        SyList<SyScCntv*> formals;
        SyList<SyCntv*> actuals;
        const vector<SyExpr*>& args = funcCall->getArgs();
        const SyList<SyScPort*>& ports = func->getPorts();
        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next) {
            formals.push_back(node->val);
        }
        SyList<std::pair<SyVCntv*,SyList<SyScCntv*> > > info =
            GetVectorScalarPairs(formals, synthMod->getName());
        assert(info.size() == (int)args.size());    
        SyNode<std::pair<SyVCntv*,SyList<SyScCntv*> > >* inode = info.head;

        for(int i = 0; (i < (int)args.size()) && inode; ++i, inode = inode->next)
        {
            args[i]->accept(&expr2Gates);
            SyList<SyCntv*> outputs = *(expr2Gates.getResult()->outputs);
            SyNode<SyCntv*>* node = outputs.head;

            int diff = outputs.size() - inode->val.second.size();
            
            if(diff >= 0) {
                while(diff--) {
                    node = node->next;
                }
            }
            else {
                while(!diff) 
                {
                    ++diff;
                    outputs.push_front(expr2Gates.createNet());
                }    
            }
            for(; node; node = node->next) {
                actuals.push_back(node->val);
            }    
            //actuals.merge(outputs);
        }
        SyList<const SyCntv*> flist;
        CreateFlatList<const SyCntv*>(flist, func->getRetCntv());
        for(SyNode<const SyCntv*>* node = flist.head; node; node = node->next) 
        {
            formals.push_back(dynamic_cast<SyScCntv*>(
                const_cast<SyCntv*>(node->val)));
            SyCntv *net = expr2Gates.createNet();    
            actuals.push_back(net);
            
            retOut2Rep.insert(make_pair(const_cast<SyCntv*>(node->val), net));
        }

        SyAssignStmtsTrans trans(synthMod);
        trans.visit(const_cast<SyList<SyStmt*>&>(func->getStmts()));
        

        StmtsConverter s(formals, actuals, func->getStmts());

        return s();
    }

    Expr2GatesRetType*
    FuncCall2Gates::funcCall2Gates(SyModule* rmod, 
        SyModule* smod, SyScope* scope, const SyFuncCall* fcall)
    {
        SyList<SyStmt*> stmts = this->createStmts(rmod, smod, fcall);
        const SyFunc *func = fcall->getFunc();
        Stmt2Gates stmt2Gates(rmod, smod, func);

        OutCntvInfoSet *outCntvInfoSet =  
            stmt2Gates.createGates(stmts, retOut2Rep);
        FuncCallMerge::FuncCallInfo info = {stmt2Gates.getOut2Rep(),
            outCntvInfoSet, rmod, smod};    
        FuncCallMerge::instance()->addInfo(scope, info);    
        Expr2GatesRetType *result = new Expr2GatesRetType();


        for(Out2Rep::const_iterator iter = retOut2Rep.begin();
            iter != retOut2Rep.end(); ++iter) {

            result->outputs->push_back(iter->second);
        }    

            

        retOutCntvSet = outCntvInfoSet;
        retOut2Rep = stmt2Gates.getOut2Rep();
        return result;
    }

    FuncCallMerge* FuncCallMerge::funcCallMerge = 0;
    FuncCallMerge::FuncCallMerge() {
    }

    FuncCallMerge::~FuncCallMerge() {
    }

    FuncCallMerge*
    FuncCallMerge::instance() 
    {
        if(!funcCallMerge) {
            funcCallMerge = new FuncCallMerge();
        }
        return funcCallMerge;
    }

    void
    FuncCallMerge::addInfo(SyScope *scope, FuncCallInfo& info) 
    {
        FuncCallInfoTable::iterator iter = funcCallInfoTable.find(scope);
        if(iter == funcCallInfoTable.end()) {
            funcCallInfoTable.insert(make_pair(scope, info));
        }
        else {
            iter->second.push_back(info);
        }    
    }

    void 
    FuncCallMerge::clear() {
        funcCallInfoTable.clear();
    }

    OutCntvInfoSet*
    FuncCallMerge::merge(SyScope *scope)
    {
        FuncCallInfoTable::const_iterator iter = funcCallInfoTable.find(scope);
        if(iter == funcCallInfoTable.end()) {
            return NULL;
        }    

        FuncCallInfo funcInfo = iter->second.head->val;

        Stmt2Gates stmt2Gates(funcInfo.rmod, funcInfo.smod, scope);

        vector<OutCntvInfoSet*> info;
        for(SyNode<FuncCallInfo>* node = iter->second.head; node;
            node = node->next)
        {
            funcInfo = node->val;
            info.push_back(funcInfo.outCntvInfoSet);
            stmt2Gates.mergeRep(info[info.size()-1], funcInfo.out2Rep);
        }    

        stmt2Gates.removeOverriddenEntries(info);

        vector<SyCntv*> outCntvs = stmt2Gates.getOutCntvs(info);
        OutCntvInfoSet* outCntvInfoSet = new OutCntvInfoSet();


        for(unsigned int i = 0; i < outCntvs.size(); ++i) {
            outCntvInfoSet->insert(stmt2Gates.getMergedOutCntvInfo(
                outCntvs[i], info));
        }    
        funcCallInfoTable.erase(scope);
        return outCntvInfoSet;

    }
}
