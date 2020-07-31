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

#include <cmath>

#include "sycntvs.h"
#include "sycontainer.h"
#include "syconst.h"
#include "syvisitor.h"
#include "systmt.h"
#include "cellmgr.h"
#include "synthcell.h"
#include "tvisitor.h"

using std::vector;
using std::string;
namespace Synth
{
    SyConst*
        SyExpr::size() const {
            return new SyConst(1, true, false);
        }

    bool
        SyExpr::isConst() const
        {
            SyConst *const_ = this->evaluate();
            if(const_)
            {
                delete const_;
                return true;
            }
            return false;
        }

    SyFuncCall::SyFuncCall(    const SyTaskOrFuncCall *fcall):
                            SyExpr(),
                            funcCall(const_cast<SyTaskOrFuncCall*>(fcall))
    {
    }    
    SyFuncCall::~SyFuncCall() {
        delete funcCall;
    }    

    const SyFunc*
    SyFuncCall::getFunc() const {
        return static_cast<const SyFunc*>(funcCall->getTaskOrFunc());
    }    

    SyFunc*
    SyFuncCall::getFunc() {
        return const_cast<SyFunc*>(
            static_cast<const SyFunc*>(funcCall->getTaskOrFunc()));
    }    
    void
    SyFuncCall::setFunc(const SyFunc *func) {
        funcCall->setTaskOrFunc(func);
    }    

    void
    SyFuncCall::addArg(const SyExpr* arg) {
        funcCall->addArg(arg);
    }

    const std::vector<SyExpr*>&
    SyFuncCall::getArgs() const {
        return funcCall->getArgs();
    }

    bool
        SyFuncCall::isSigned() const {
            return this->getFunc()->isSigned();
        }

    SyFuncCall*
    SyFuncCall::copy() const {
        SyFuncCall *newCall = new SyFuncCall(
            new SyTaskOrFuncCall(this->getFunc()));
        const vector<SyExpr*>& args = this->getArgs();
        for(unsigned int i = 0; i < args.size(); ++i) {
            newCall->addArg(args[i]->copy());
        }
        return newCall;
        //return new SyFuncCall(funcCall);
    }    
    void
        SyFuncCall::repSubExpr(
                const string& subexpr, 
                const SyExpr *newexpr) {
            funcCall->repSubExpr(subexpr, newexpr);
        }    

    const string&
    SyFuncCall::getName() const {
        return funcCall->getName();
    }    

    SyConst*
    SyFuncCall::size() const {
        return this->getFunc()->getRetCntv()->size();
    }    

    void
        SyUnary::repSubExpr(
                const string& subexpr, 
                const SyExpr *newexpr)
        {
            if(expr->toString() == subexpr) 
            {
                if(!dynamic_cast<SyCntv*>(expr) ||
                        dynamic_cast<SyConst*>(expr)){
                    //connectives are shared, so should not be deleted
                    delete expr;
                }
                expr = newexpr->copy();
                return;
            }
            expr->repSubExpr(subexpr, newexpr);
        }
    
    void
        SyUnary::repVCntvByConcat()
        {
            if(SyVCntv *vcntv = dynamic_cast<SyVCntv*>(expr)) {
                expr = vcntv->getConcat();
            }    
            else {
                expr->repVCntvByConcat();
            }    
        }
    
    bool    
        SyUnary::contains(const string& str) const
        {
            if(this->toString() == str) {
                return true;
            }
            return expr->contains(str);
        }

    SyConst* 
        SyUnary::evaluate() const
        {
            SyConst *const_ = expr->evaluate();
            if(!const_){
                return 0;
            }
            SyConst *newconst = 0;
            switch(op)
            {
                case UNPLUS: 
                    newconst = new SyConst(+(*const_));
                    break;
                case UNMINUS:
                    newconst = new SyConst(-(*const_));
                    break;
                case LOGNEG: 
                  if(!(*const_)) {
                      newconst = SyConst::ONE();
                  }
                  else {
                      newconst = SyConst::ZERO();
                  }
                  break;
                case BITNEG:
                    newconst = new SyConst(~(*const_));
                    break;
                case REDAND: 
                    newconst = new SyConst(&(*const_));
                    break;
                case REDNAND:
                    newconst = new SyConst(const_->rednand());
                    break;
                case REDNOR:
                    newconst = new SyConst(const_->rednor());
                    break;
                case REDOR:
                    newconst = new SyConst(const_->redor());
                    break;
                case REDXOR:
                    newconst = new SyConst(const_->redxor());
                    break;
                case REDXNOR:
                    newconst = new SyConst(const_->redxnor());
                    break;
                default: break;
            }
            delete const_;
            return newconst;
        }

    SyConst*
        SyUnary::size() const
        {
            //SyConst *newconst = 0;
            switch(op)
            {
                case REDAND: 
                case REDNAND:
                case REDNOR:
                case REDOR:
                case REDXOR:
                case REDXNOR:
                case LOGNEG: 
                    return new SyConst(1, true, false);
                case BITNEG:
                case UNPLUS:
                case UNMINUS:
                    return expr->size();
                default: return 0;
            }
        }

    void
        SyUnary::removeVal() {
            expr->removeVal();
        }

    void
        SyBinary::repSubExpr(
                const string& subexpr,
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

            if(rexpr->toString() == subexpr)
            {
                if(!dynamic_cast<SyCntv*>(rexpr) ||
                        dynamic_cast<SyConst*>(rexpr)){
                    delete rexpr;
                }
                rexpr = newexpr->copy();
            }
            else {
                rexpr->repSubExpr(subexpr, newexpr);
            }
        }

    void
        SyBinary::repVCntvByConcat()
        {
            if(SyVCntv *vcntv = dynamic_cast<SyVCntv*>(lexpr)) {
                lexpr = vcntv->getConcat();
            }
            else {
                lexpr->repVCntvByConcat();
            }    

            if(SyVCntv *vcntv = dynamic_cast<SyVCntv*>(rexpr)) {
                rexpr = vcntv->getConcat();
            }
            else {
                rexpr->repVCntvByConcat();
            }    
        }

    SyConst*
        SyBinary::evaluate() const
        {
            SyConst *lval = lexpr->evaluate();
            SyConst *rval = rexpr->evaluate();
            if(!lval || !rval){
                return 0;
            }
            SyConst *const_ = 0;

            switch(op)
            {
                case LOGAND:
                    const_ = (*lval && *rval) ? SyConst::ONE() : SyConst::ZERO();
                    break;
                case LOGOR:
                    (*lval || *rval) ? SyConst::ONE() : SyConst::ZERO();
                    break;
                case GTHAN:
                    const_ = (*lval > *rval) ? SyConst::ONE() : SyConst::ZERO();
                    break;
                case GTHANEQ:
                    const_ = (*lval >= *rval) ? SyConst::ONE() : SyConst::ZERO();
                    break;
                case LTHAN:
                    const_ = (*lval < *rval) ? SyConst::ONE() : SyConst::ZERO();
                    break;
                case LTHANEQ:
                    const_ = (*lval <= *rval) ? SyConst::ONE() : SyConst::ZERO();
                    break;
                case LOGEQ:
                    const_ = (*lval == *rval) ? SyConst::ONE() : SyConst::ZERO();
                    break;
                case LOGNEG:
                    const_ = (*lval != *rval) ? SyConst::ONE() : SyConst::ZERO();
                    break;
                case LSHIFT:
                    const_ = new SyConst(*lval << rval->toLong());
                    break;
                case RSHIFT:
                    const_ = new SyConst(*lval >> rval->toLong());
                    break;
                case RRSHIFT:
                    //check
                    const_ = new SyConst(lval->sshiftright(rval->toLong(),
                                true));
                    break;
                case PLUS:
                    const_ = new SyConst(*lval+*rval);
                    break;
                case MINUS:
                    const_ = new SyConst(*lval-*rval);
                    break;
                case MULT:
                    const_ = new SyConst(*lval**rval);
                    break;
                case DIV:
                    const_ = new SyConst(*lval/ *rval);
                    break;
                case MOD:
                    const_ = new SyConst(*lval%*rval);
                    break;
                case BITXOR:
                    const_ = new SyConst(*lval^*rval);
                    break;
                case BITAND:
                    const_ = new SyConst(*lval&*rval);
                    break;
                case BITNAND:
                    const_ = new SyConst(~*lval&*rval);
                    break;
                case BITNOR:
                    const_ = new SyConst(~*lval|*rval);
                    break;
                case BITOR:
                    const_ = new SyConst(*lval|*rval);
                    break;
                case POW:    
                {
                    long base = lval->toLong();
                    long exponent = rval->toLong();
                    long res = pow(base, exponent);
                    const_ = new SyConst(res, this->isSigned(), res < 0);
                }    
                default: break;
            }
            delete lval;
            delete rval;
            return const_;
        }

    bool
        SyBinary::isShiftExpr() const
        {
            switch(op)
            {
                case LSHIFT:
                case RSHIFT:
                case RRSHIFT:
                    return true;
                default:
                    return false;
            }        
        }

    bool
        SyBinary::isSigned() const
        {
            if(isShiftExpr()) {
                return lexpr->isSigned();
            }
            return lexpr->isSigned() && rexpr->isSigned();
        }

    SyConst*
        SyBinary::size() const
        {
            switch(op)
            {
                case LOGAND:
                case LOGOR:
                case GTHAN:
                case GTHANEQ:
                case LTHAN:
                case LTHANEQ:
                case LOGEQ:
                case LOGENEQ:
                case LOGNEG:
                    return new SyConst(1, true, false);
                case LSHIFT:
                case RSHIFT:
                case RRSHIFT:
                case POW:
                    return lexpr->size();
                case PLUS:
                case MINUS:
                case MULT:
                case DIV:
                case MOD:
                case BITXOR:
                case BITNXOR:
                case BITAND:
                case BITNAND:
                case BITNOR:
                case BITOR:
                    {
                        SyConst *retsize = 0;
                        SyConst *lsize = lexpr->size();
                        SyConst *rsize = rexpr->size();
                        retsize = *lsize > *rsize ? lsize->copy() : rsize->copy();
                        delete lsize;
                        delete rsize;
                        return retsize;
                    }
                default: 
                    return 0;
            }
        }

    bool    
        SyBinary::contains(const string& str) const
        {
            if(this->toString() == str) {
                return true;
            }

            if(lexpr->contains(str)) {
                return true;
            }
            return rexpr->contains(str);
        }

    void
        SyBinary::removeVal() 
        {
            lexpr->removeVal();
            rexpr->removeVal();
        }

    void
        SyCondExpr::repSubExpr(
                const string& subexpr,
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

            if(texpr->toString() == subexpr)
            {
                if(!dynamic_cast<SyCntv*>(texpr) ||
                        dynamic_cast<SyConst*>(texpr)){
                    delete texpr;
                }
                texpr = newexpr->copy();
            }
            else {
                texpr->repSubExpr(subexpr, newexpr);
            }

            if(fexpr->toString() == subexpr)
            {
                if(!dynamic_cast<SyCntv*>(fexpr) ||
                        dynamic_cast<SyConst*>(fexpr)){
                    delete fexpr;
                }
                fexpr = newexpr->copy();
            }
            else {
                fexpr->repSubExpr(subexpr, newexpr);
            }
        }

    void
        SyCondExpr::repVCntvByConcat()
        {
            if(SyVCntv *vcntv = dynamic_cast<SyVCntv*>(cond)) {
                cond = vcntv->getConcat();
            }
            else {
                cond->repVCntvByConcat();
            }    

            if(SyVCntv *vcntv = dynamic_cast<SyVCntv*>(texpr)) {
                texpr = vcntv->getConcat();
            }
            else {
                texpr->repVCntvByConcat();
            }    

            if(SyVCntv *vcntv = dynamic_cast<SyVCntv*>(fexpr)) {
                fexpr = vcntv->getConcat();
            }
            else {
                fexpr->repVCntvByConcat();
            }    
        }

    SyConst*
        SyCondExpr::evaluate() const
        {
            SyConst *tcond = cond->evaluate();
            if(!tcond) {
                return 0;
            }    
            if(tcond && !!(*tcond)){
                return texpr->evaluate();
            }
            else {
                return fexpr->evaluate();
            }
        }
    SyConst*
        SyCondExpr::size() const
        {
            SyConst *retsize = 0;
            SyConst *tsize = texpr->size();
            SyConst *fsize = fexpr->size();
            retsize = *tsize > *fsize ? tsize->copy() : fsize->copy();
            delete tsize;
            delete fsize;
            return retsize;
        }

    bool
        SyCondExpr::contains(const string& str) const
        {
            if(cond->contains(str)) {
                return true;
            }

            if(texpr->contains(str)) {
                return true;
            }

            return fexpr->contains(str);
        }

    void
        SyCondExpr::removeVal() 
        {
            cond->removeVal();
            texpr->removeVal();
            fexpr->removeVal();
        }    
     


    string
        SyFuncCall::toString() const 
        {
            string retstr(this->getFunc()->getName());
            retstr += string("(");
            const vector<SyExpr*>& args = funcCall->getArgs();
            bool first = true;
            for(unsigned int i = 0; i < args.size(); ++i){
                if(first)
                {
                    first = false;
                    retstr += args[i]->toString();
                }
                else 
                {
                    retstr += string(",");
                    retstr += args[i]->toString();
                }
            }
            retstr += string(")");
            return retstr;
        }


    bool
        SyFuncCall::contains(const string& str) const
        {
            if(this->toString() == str) {
                return true;
            }
            return false;
        }

    void
        SyFuncCall::removeVal()
        {
            getFunc()->getRetCntv()->removeVal();
        }

    void
        SyUnary::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    void
        SyUnary::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }

    void
        SyBinary::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyBinary::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyCondExpr::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyCondExpr::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyFuncCall::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyFuncCall::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }

    void
        SyVString::accept(SyVisitorBase *v) const {
            v->visit(this);
        }

    bool
        SyVString::contains(const string& str) const 
        {
            if(vstr == str) {
                return true;
            }
            return false;
        }    

    SyConcat*
        SyVString::toConcat() const
        {
            SyConcat *conc = new SyConcat();
            for(int i = 0; i < vstr.size(); ++i)
            {
                char tmp[2];
                tmp[0] = vstr[i];
                tmp[1] = '\0';
                conc->addBit(new SyConst(atol(tmp), true, false));
            }
            return conc;
        }

    SyConst*
        SyVString::toConst() const
        {
            string str = vstr.substr(1, vstr.size()-2);
            string tmp;
            for(int i = 0; i < str.size(); ++i)
            {
                long c = (long)str[i];
                tmp += Long2String(c, 8);
            }
            return new SyConst(false, tmp);
            
        }

    SyList<SyScCntv*>*
        SyUnary::toGate(SyModule *mod)
        {
        //to be obsoleted
            SyList<SyScCntv*> *outCntvs = expr->toGate(mod);
            switch(op)
            {
                case SyExpr::UNPLUS:
                    return outCntvs;
                case SyExpr::UNMINUS:
                    this->toUminus(mod, outCntvs);
                    break;
                default:
                    return 0;
            }
            return 0;
        }

    SyList<SyScPin*>*
        SyUnary::toUminus(
                SyModule *mod,
                SyList<SyScCntv*> *outCntvs) const
        {
            //to be obsoleted
            //XOR_2 gate tree followed by an incrementor
            SyList<SyScPin*> pins;
            SyList<SyScPin*> xoroutpins;
            for(SyNode<SyScCntv*>* node = outCntvs->head; node; 
                    node = node->next)
            {
                vector<int> inputs;
                inputs.push_back(1); //area
                inputs.push_back(1); //delay
                //const SynthGND *gnd = dynamic_cast<const SynthGND*>(
                //        CellMgr::instance()->create(CellMgr::GND, inputs));

                //SyModInst *ginst = gnd->instantiate(mod, mod->getName());
                inputs.clear();
                inputs.push_back(1); //area
                inputs.push_back(1); //delay
                const SynthXOR *xor_ = dynamic_cast<const SynthXOR*>(
                        CellMgr::instance()->create(CellMgr::XOR, inputs));
                SyModInst *xinst = xor_->instantiate(mod, mod->getName());
                xoroutpins.push_back(xinst->getPin(SyIxName("o")));
            }
            //Create incrementor
            vector<int> inputs;
            inputs.push_back(1); //area
            inputs.push_back(1); //delay
            inputs.push_back(xoroutpins.size());
            const SynthINC *inc = dynamic_cast<const SynthINC*>(
                    CellMgr::instance()->create(CellMgr::INC, inputs));
            SyModInst *incinst = inc->instantiate(mod, mod->getName());
            SyList<SyScPin*>* incinpins = inc->getInPins(incinst);
            SyNode<SyScPin*>* node2 = incinpins->head;
            for(SyNode<SyScPin*>* node1 = xoroutpins.head; node1 && node2; 
                    node1 = node1->next, node2 = node2->next){
                mod->connect(node1->val, node2->val, "synth_wire");
            }
            delete incinpins;
            return inc->getOutPins(incinst);
        }
}
