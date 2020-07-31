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

#include "syroot.h"
#include "syexprmgr.h"
#include "sycontainer.h"
#include "systmt.h"
#include "sycntvs.h"
#include "syconst.h"

using std::map;
using std::pair;
using std::make_pair;
using std::max;
namespace Synth
{
    class SyExprExtVisitor
    {
        public:
            SyExprExtVisitor(SyExpr*, long to);
            ~SyExprExtVisitor();

            SyExpr* getResult();
            const SyExpr* getResult() const;
        private:
            SyExprExtVisitor(const SyExprExtVisitor&);
            SyExprExtVisitor& operator=(const SyExprExtVisitor&);
            SyExpr* extend(SyExpr*, long to) const;
            SyExpr* extendCommon(SyExpr*, long to) const;
            SyExpr* extend(SyCondExpr*, long to) const;
            SyExpr* extend(SyUnary*, long to) const;
            SyExpr* extend(SyBinary*, long to) const;

            SyExpr* extend(SyCntv*, long to) const;
            SyExpr* extend(SyFuncCall*, long to) const;


            SyExpr *result;
    };

    SyExprExtVisitor::SyExprExtVisitor(SyExpr *expr, long to) :
        result(extend(expr, to))
    {
    }

    SyExprExtVisitor::~SyExprExtVisitor() {
    }

    const SyExpr*
    SyExprExtVisitor::getResult() const {
        return result;
    }    

    SyExpr*
    SyExprExtVisitor::getResult() {
        return result;
    }    

    SyExpr*
    SyExprExtVisitor::extend(SyExpr *expr, long to) const
    {
        if(SyCntv *cntv = dynamic_cast<SyCntv*>(expr)) {
            return extend(cntv, to);
        }

        if(SyCondExpr *condExpr = dynamic_cast<SyCondExpr*>(expr)) {
            return extend(condExpr, to);
        }

        if(SyUnary *unary = dynamic_cast<SyUnary*>(expr)) {
            return extend(unary, to);
        }

        if(SyBinary *binary = dynamic_cast<SyBinary*>(expr)) {
            return extend(binary, to);
        }

        if(SyFuncCall *funcCall = dynamic_cast<SyFuncCall*>(expr)) {
            return extend(funcCall, to);
        }

        return expr;
    }

    SyExpr*
    SyExprExtVisitor::extend(SyCondExpr *condExpr, long to) const
    {
        //condExpr->cond = extend(condExpr->cond, to);
        condExpr->texpr = extend(condExpr->texpr, to);
        condExpr->fexpr = extend(condExpr->fexpr, to);

        return condExpr;
    }

    SyExpr*
    SyExprExtVisitor::extend(SyUnary *unary, long to) const
    {
        //if(unary->getOperator() == SyExpr::UNMINUS) 
        if(unary->isConst() || unary->getOperator() == SyExpr::UNMINUS) {
            unary->expr = extend(unary->expr, to);
            return unary;
        }    
        return extendCommon(unary, to);
    }

    SyExpr*
    SyExprExtVisitor::extend(SyBinary *binary, long to) const
    {
        binary->lexpr = extend(binary->lexpr, to);
        binary->rexpr = extend(binary->rexpr, to);

        return binary;
    }
    
    SyExpr*
    SyExprExtVisitor::extend(SyFuncCall *funcCall, long to) const
    {
        SyConst *size = funcCall->size();
        int diff = to - size->toLong();
        delete size;
        
        if(diff == 0) {
            return funcCall;
        }

        SyConst *const_ = funcCall->getFunc()->getRetCntv()->evaluate();
        
        SyConcat *conc = new SyConcat(funcCall->isSigned());
        char exchar = '0';
        if(const_ && funcCall->isSigned()) {
            exchar = (const_->iszero() || const_->isone()) ? '0' : const_->getVal()[0];
        }    
        for(int i = 0; i < diff; ++i) {
            if(exchar == '0') {
                conc->addBit(SyConst::ZERO());
            }
            else if(exchar == '1') {
                conc->addBit(SyConst::ONE());
            }
        }    
        if(const_) {
            conc->addBit(const_);
        }
        else {
            conc->addBit(funcCall);
        }    
        return conc;
    }

    SyExpr*
    SyExprExtVisitor::extend(SyCntv *cntv, long to) const
    {
        return extendCommon(cntv, to);
    }

    SyExpr*
    SyExprExtVisitor::extendCommon(SyExpr *expr, long to) const
    {
        SyConst *size = expr->size();
        int diff = to - size->toLong();
        delete size;
        
        if(diff == 0) {
            return expr;
        }
        
        SyConcat *conc = new SyConcat(expr->isSigned());
        char exchar = '0';
        SyConst *cval = expr->evaluate();
        if(cval && expr->isSigned()) 
        {
            exchar = (cval->iszero() || cval->isone()) ? '0' : cval->getVal()[0];
        }    
        else if(const SyXOrZ *xorZ = dynamic_cast<const SyXOrZ*>(expr)) {
            exchar = xorZ->isX() ? 'X' : 'Z';
        }    
             
            
        for(int i = 0; i < diff; ++i) {
            if(exchar == '0') {
                conc->addBit(SyConst::ZERO());
            }
            else if(exchar == '1') {
                conc->addBit(SyConst::ONE());
            }
            else 
            {
                SyXOrZ *xorZ = new SyXOrZ(exchar);
                xorZ->setSign(expr->isSigned());
                conc->addBit(xorZ);
            }    
        }    
        conc->addBit(expr);
        return conc;
    }


    ///////////////////////////////////////////////////////////////////
    //    Class : SyExprMgr
    ///////////////////////////////////////////////////////////////////

    SyExprMgr* SyExprMgr::exprMgr = 0;

    SyExprMgr::SyExprMgr() {
    }

    SyExprMgr::~SyExprMgr() {
    }

    SyExprMgr*
    SyExprMgr::instance()
    {
        if(!exprMgr) {
            exprMgr = new SyExprMgr();
        }
        return exprMgr;
    }

    template<class T> void
    SyExprMgr::processListOfObjects(const SyList<T*>& tlist) const{
        for(SyNode<T*>* node = tlist.head; node; node = node->next) {
            this->process(node->val);
        }    
    }
    void
    SyExprMgr::process() const
    {
        const map<string, SyModule*, cmp_str> & synthMods =
        SyRoot::instance()->getSynthMods();
        map<string, SyModule*, cmp_str>::const_iterator beginIter =
        synthMods.begin();
        map<string, SyModule*, cmp_str>::const_iterator endIter =
        synthMods.end();

        for(; beginIter != endIter; ++beginIter) 
        {
            SyBModule *module = dynamic_cast<SyBModule*>(beginIter->second);
            this->process(module);
        }    
    }

    void
    SyExprMgr::process(SyBModule *mod) const
    {
        this->processListOfObjects<SyContAssign>(mod->getContAssigns());
        this->processListOfObjects<SyAlways>(mod->getAlwaysBlocks());
        this->processListOfObjects<SyTask>(mod->getTasks());
        this->processListOfObjects<SyFunc>( mod->getFuncs());
    }
    
    void
    SyExprMgr::process(SyTask *task) const {
        this->process(task->getStmts());
    }
    void
    SyExprMgr::process(SyFunc *func) const {
        this->process(func->getStmts());
    }
    void
    SyExprMgr::process(SyAlways *always) const {
        this->process(always->getStmts());
    }

    void
    SyExprMgr::process(SyCase *case_) const
    {
        const SyList<SyCaseItem*>& items = case_->getCaseItems();

        for(SyNode<SyCaseItem*>* item = items.head; item; item = item->next) {
            this->process(item->val->getStmts());
        }    
        
    }

    void
    SyExprMgr::process(SyIf *if_) const
    {
        this->process(if_->getThenPart());
        this->process(if_->getElsePart());
    }
    void
    SyExprMgr::process(SyBlockAssign *assign ) const
    {
        this->processAssign(assign);
    }
    void
    SyExprMgr::process(SyNonBlockAssign *assign ) const
    {
        this->processAssign(assign);
    }
    void
    SyExprMgr::process(SyContAssign *assign ) const {
        this->processAssign(assign);
    }

    void
    SyExprMgr::process(const SyList<SyStmt*>& stmts ) const {
        for(SyNode<SyStmt*>* s = stmts.head; s; s = s->next) {
            this->process(s->val);
        }    
    }

    void
    SyExprMgr::process(SyTaskCall *tcall) const {
        //nothing to be done
    }

    void
    SyExprMgr::process(SyStmt *stmt ) const
    {
        if(SyTaskCall *tcall = dynamic_cast<SyTaskCall*>(stmt)) {
            this->process(tcall);
        }
        else if(SyAlways *always = dynamic_cast<SyAlways*>(stmt)) {
            this->process(always);
        }
        else if(SyCase *case_ = dynamic_cast<SyCase*>(stmt)) {
            this->process(case_);
        }    
        else if(SyIf *if_ = dynamic_cast<SyIf*>(stmt)) {
            this->process(if_);
        }    
        else if(SyBlockAssign *bassign = dynamic_cast<SyBlockAssign*>(stmt)) {
            this->process(bassign);
        }    
        else if(SyNonBlockAssign *nbassign = dynamic_cast<SyNonBlockAssign*>(
        stmt)) {
            this->process(nbassign);
        }
        else if(SyContAssign *cassign = dynamic_cast<SyContAssign*>(stmt)) {
            this->process(cassign);
        }    
    }

    void
    SyExprMgr::processAssign(SyAssign *assign ) const
    {
        SyExpr *lexpr = const_cast<SyExpr*>(assign->getLExpr());
        SyExpr *rexpr = const_cast<SyExpr*>(assign->getRExpr());

        std::pair<int, bool> ssl = this->findSS(lexpr, make_pair(0, true), INT_MAX);
        std::pair<int, bool> ssr = this->findSS(rexpr, make_pair(0, true), INT_MAX);

        //lexpr = this->process(lexpr, ssl);
        rexpr = this->process(rexpr, ssr);
    //    if(ssl.first < ssr.first) {
    //        rexpr = this->process(rexpr, ssl);
    //    }    

        //extension pass 32 bits
        if(ssl.first > ssr.first) {
            rexpr = this->extend(rexpr, ssl.first-ssr.first);
        }
        else if(ssl.first < ssr.first) {
            rexpr = this->truncate(rexpr, ssr.first, ssl.first);
        }
        assign->setRExpr(rexpr);

    }

    SyExpr*
    SyExprMgr::process(SyCondExpr *expr, pair<int, bool> ss) const
    {
        expr->texpr = this->process(expr->texpr, ss);
        expr->fexpr = this->process(expr->fexpr, ss);
        return expr;
    }

    SyExpr*
    SyExprMgr::process(SyUnary *expr, pair<int, bool> ss) const
    {
        //if(expr->getOperator() == SyExpr::UNMINUS) 
        if(expr->isConst() || expr->getOperator() == SyExpr::UNMINUS) 
        {
            expr->expr = this->process(expr->expr, ss);
            return expr;
        }    
        return processCommon(expr, ss);
            
    //    expr->expr = this->process(expr->expr, ss);
    //    return expr;
    }
    SyExpr*
    SyExprMgr::process(SyBinary *expr, pair<int, bool> ss) const
    {
        expr->lexpr = this->process(expr->lexpr, ss);
        expr->rexpr = this->process(expr->rexpr, ss);
        return expr;
    }
    SyExpr*
    SyExprMgr::process(SyFuncCall *fcall, pair<int, bool> ss) const
    {
        SyConst *const_ = fcall->size();
        long size = const_->toLong();
        delete const_;

        if(ss.first > size) {
            return this->extend(fcall, ss.first - size);
        }
    //    else if(ss.first < size) {
    //        return this->truncate(cntv, size - ss.first);
    //    }    

        return fcall;
    }
    SyExpr*
    SyExprMgr::process(SyCntv *cntv, pair<int, bool> ss) const
    {
        return processCommon(cntv, ss);
    }
    SyExpr*
    SyExprMgr::processCommon(SyExpr *expr, pair<int, bool> ss) const
    {
        SyConst *const_ = expr->size();
        long size = const_->toLong();
        delete const_;

        if(ss.first > size) {
            return this->extend(expr, ss.first - size);
        }
        else if(ss.first < size) {
            return this->truncate(expr, size, ss.first);
        }    

        return expr;
    }
    SyExpr*
    SyExprMgr::process(SyExpr *expr, pair<int, bool> ss) const
    {
        SyExpr *texpr = 0;
        if(SyCondExpr *cexpr = dynamic_cast<SyCondExpr*>(expr)) {
            texpr = this->process(cexpr, ss);
        }
        else if(SyUnary *unary = dynamic_cast<SyUnary*>(expr)) {
            texpr = this->process(unary, ss);
        }
        else if(SyBinary *binary = dynamic_cast<SyBinary*>(expr)) {
            texpr = this->process(binary, ss);
        }
        else if(SyFuncCall *fcall = dynamic_cast<SyFuncCall*>(expr)) {
            texpr = this->process(fcall, ss);
        }    
        else if(SyCntv *cntv = dynamic_cast<SyCntv*>(expr)) {
            texpr = this->process(cntv, ss);
        }

        SyConst *const_ = texpr->size();
        int diff = ss.first-const_->toLong();
        delete const_;

        return this->extend(texpr, diff);
    }

    pair<int, bool>
    SyExprMgr::findSS(const SyCondExpr *expr, pair<int, bool> ss, int limit) const
    {
        if((ss.first == limit) && !ss.second) {
            return ss;
        }    
        pair<int, bool> tp = this->findSS(expr->getTrueExpr(), ss, limit);
        pair<int, bool> fp = this->findSS(expr->getFalseExpr(), ss, limit);
        pair<int, bool> tmp = make_pair(max(tp.first, fp.first),
            tp.second && fp.second);
        return make_pair(max(tmp.first, ss.first) > limit ? limit :
            max(tmp.first, ss.first), tmp.second && ss.second);

    }
    pair<int, bool>
    SyExprMgr::findSS(const SyUnary *expr, pair<int, bool> ss, int limit) const 
    {
        if((ss.first == limit) && !ss.second) {
            return ss;
        }    
        return this->findSS(expr->getExpr(), ss, limit);
    }
    pair<int, bool>
    SyExprMgr::findSS(const SyBinary *expr, pair<int, bool> ss, int limit) const
    {
        if((ss.first == limit) && !ss.second) {
            return ss;
        }    
        pair<int, bool> left = this->findSS(expr->getLExpr(), ss, limit);
        pair<int, bool> right = this->findSS(expr->getRExpr(), ss, limit);
        return make_pair(max(left.first, right.first) > limit ? limit :
            max(left.first, right.first), left.second && right.second);
    }

    pair<int, bool>
    SyExprMgr::findSS(const SyCntv *expr, pair<int, bool> ss, int limit) const
    {
        if((ss.first == limit) && !ss.second) {
            return ss;
        }    
        SyConst *const_ = expr->size();
        int size = const_->toLong();
        delete const_;
        return make_pair(max(size, ss.first) > limit ? limit : 
            max(size, ss.first), expr->isSigned() && ss.second);
    }
    pair <int, bool>
    SyExprMgr::findSS(const SyFuncCall *expr, pair<int, bool> ss, int limit) const
    {
        if((ss.first == limit) && !ss.second) {
            return ss;
        }    
        SyConst *const_ = expr->size();
        int size = const_->toLong();
        delete const_;
        return make_pair(max(size, ss.first) > limit ? limit : 
            max(size, ss.first), expr->isSigned() && ss.second);
    }

    pair<int, bool>
    SyExprMgr::findSS(const SyConst *expr, pair<int, bool> ss, int limit) const
    {
        if((ss.first == limit) && !ss.second) {
            return ss;
        }    
        long size = 0;
        if(!expr->isNeg())
        {
            //SyConst *tmp = expr->evaluate();
            //long val = tmp->toLong();
            SyConst *const_ = expr->size();
            size = const_->toLong();
            delete const_;
            //delete tmp;
            //long count = 0;
            //while(val) 
            //{
             //   ++count;
            //    val = val >> 1;
            //}    
            //if((size == 0) && (count == 0)) {
            //    size = 1;
            //}
            //else if(size == 0) {
            //    size = count;
            //}
            //else {
            //    size = std::min(size, count);
            //}    
        }
        else {
            SyConst *const_ = expr->size();
            size = const_->toLong();
            delete const_;
        }    
        return make_pair(max(size, (long)ss.first) > limit ? limit : 
            max(size, (long)ss.first), expr->isSigned() && ss.second);
    }

    pair<int, bool>
    SyExprMgr::findSS(const SyExpr *expr, pair<int, bool> ss, int limit) const
    {
        if(const SyCondExpr *cexpr = dynamic_cast<const SyCondExpr*>(expr)) {
            return this->findSS(cexpr, ss, limit);
        }
        else if(const SyUnary *unary = dynamic_cast<const SyUnary*>(expr)){
            return findSS(unary, ss, limit);
        }    
        else if(const SyBinary *binary = dynamic_cast<const SyBinary*>(expr)){
            return findSS(binary, ss, limit);
        }    
        else if(const SyConst *const_ = dynamic_cast<const SyConst*>(expr)) {
            return findSS(const_, ss, limit);
        }    
        else if(const SyFuncCall *fcall = dynamic_cast<const SyFuncCall*>(expr)) {
            return findSS(fcall, ss, limit);
        }    
        
        else if(const SyCntv *cntv = dynamic_cast<const SyCntv*>(expr)){
            return findSS(cntv, ss, limit);
        }    
        assert(0);
    }

    SyExpr *
    SyExprMgr::extend(SyExpr *expr, int count) 
    {
        SyConst *size = expr->size();
        count += size->toLong();
        delete size;
        SyExprExtVisitor exprEEV(expr, count);
        return exprEEV.getResult();
        #if 0
        if(count == 0) {
            return expr;
        }    
        SyConcat *conc = new SyConcat();
        bool extzero = true;
        if(SyConst *const_ = dynamic_cast<SyConst*>(expr)) 
        {
            char msb = const_->getVal()[0];
            extzero = (msb == '0');
        }    
        for(int i = 0; i < count; ++i) {
            conc->addBit(extzero ? SyConst::ZERO() : SyConst::ONE());
        }    
        conc->addBit(expr);
        return conc;
        #endif
    }

    SyExpr*
    SyExprMgr::truncate(SyExpr *expr, int count)
    {
        if(count == 0) {
            return expr;
        }    
        SyConst *const_ = expr->evaluate();
        if(const_) {
            expr = const_;
        }    

        if(SyPartSel *psel = dynamic_cast<SyPartSel*>(expr)) {
            expr = psel->getConcat();
        }

        if(SyConcat *conc = dynamic_cast<SyConcat*>(expr))
        {
            SyConst *csize = conc->size();
            long concsize = csize->toLong();
            delete csize;
            int size = concsize - count;
            assert(size >= 0);
            SyConcat *newconc = new SyConcat(conc->isSigned());
            SyConcat tconc;
            SyList<SyExpr*> texprs;
            SyList<SyExpr*> exprs = conc->getExprs();
            exprs.reverse();
            SyNode<SyExpr*>* node = exprs.head;
            concsize = 0;

            while((concsize != size) && node) //Why node in condition? TBD
            {
                SyConst *tconst = node->val->size();
                if((tconst->toLong() + concsize) > size)
                {
                    int diff = tconst->toLong()+concsize-size;
                    SyExpr *tmp = truncate(node->val, diff);

                    //newConc->addBit(tmp);
                    tconc.addBit(tmp);
                    texprs.push_front(tmp);
                    node = node->next;
                }
                else /*if ((tconst->toLong() + concsize) < size)*/{
                    //newConc->addBit(node->val);
                    tconc.addBit(node->val);
                    texprs.push_front(node->val);
                    node = node->next;
                }
                delete tconst;
                tconst = tconc.size();
                concsize = tconst->toLong();
                delete tconst;
            }
            node = texprs.head;
            for(; node; node = node->next) {
                newconc->addBit(node->val);
            }    
            return newconc;
        }
        else if(SyVCntv* vcntv = dynamic_cast<SyVCntv*>(expr))
        {
            const SyList<SyScCntv*>& cntvs = vcntv->getBits();
            SyNode<SyScCntv*>* node = cntvs.head;
            for(int i = 0; i < count; ++i) {
                node = node->next;
            }
            SyConcat *conc = new SyConcat(vcntv->isSigned());
            for(; node; node = node->next) {
                conc->addBit(node->val);
            }
            return conc;
        }
        else if(SyConst *const_ = dynamic_cast<SyConst*>(expr))
        {
            std::vector<SyConst*> bits = const_->getBits();
            assert(bits.size() > count);
            int index = 0;
            for(; index < count; ++index) {
                delete bits[index];
            }
            SyConcat *conc = new SyConcat(const_->isSigned());
            for(; index < bits.size(); ++index) {
                conc->addBit(bits[index]);
            }    
            return conc;
        }
        return expr;
    }
    SyExpr*
    SyExprMgr::truncate(SyExpr *expr, int from, int to)
    {
        if(from <= to) {
            return expr;
        }    
        SyConst *esize = expr->size();
        if(esize->toLong() <= to)
        {
            delete esize;
            return expr;
        }
        delete esize;
        SyConst *const_ = expr->evaluate();
        if(const_) {
            expr = const_;
        }    

        if(SyPartSel *psel = dynamic_cast<SyPartSel*>(expr)) {
            expr = psel->getConcat();
        }

        if(SyConcat *conc = dynamic_cast<SyConcat*>(expr))
        {
            SyConcat *newconc = new SyConcat(conc->isSigned());
            SyList<SyExpr*> texprs;
            SyList<SyExpr*> exprs = conc->getExprs();
            exprs.reverse();
            SyNode<SyExpr*>* node = exprs.head;
            int total = 0;

            for(; node && (total < to); node = node->next)
            {
                SyConst *bitsize = node->val->size();
                if((total+bitsize->toLong()) <= to) 
                {
                    texprs.push_front(node->val);
                    total += bitsize->toLong();
                }
                else 
                {
                    long newfrom = bitsize->toLong();
                    long newto = to - total;
                    texprs.push_front(truncate(node->val, newfrom, newto));
                }        
                delete bitsize;
            }
            for(node = texprs.head; node; node = node->next) {
                newconc->addBit(node->val);
            }    

            return newconc;
        }
        else if(SyVCntv* vcntv = dynamic_cast<SyVCntv*>(expr))
        {
            int count = from - to;
            const SyList<SyScCntv*>& cntvs = vcntv->getBits();
            SyNode<SyScCntv*>* node = cntvs.head;
            for(int i = 0; i < count && node; ++i) {
                node = node->next;
            }
            SyConcat *conc = new SyConcat(vcntv->isSigned());
            for(; node; node = node->next) {
                conc->addBit(node->val);
            }
            return conc;
        }
        else if(SyConst *const_ = dynamic_cast<SyConst*>(expr))
        {
            std::vector<SyConst*> bits = const_->getBits();
            from = bits.size();
            if(from <= to) {
                return const_->copy();
            }    
            int count = from - to;
            assert(bits.size() >= count);
            int index = 0;
            for(; index < count; ++index) {
                delete bits[index];
            }
            SyConcat *conc = new SyConcat(const_->isSigned());
            for(; index < bits.size(); ++index) {
                conc->addBit(bits[index]);
            }    
            return conc;
        }
        else if(SyCondExpr *condExpr = dynamic_cast<SyCondExpr*>(expr))
        {
            const SyExpr *cond = condExpr->getCond();
            const SyExpr *texpr = condExpr->getTrueExpr();
            const SyExpr *fexpr = condExpr->getFalseExpr();

            SyConst *csize = condExpr->size();
            SyConst *tsize = texpr->size();
            SyConst *fsize = fexpr->size();

            /*
            SyExpr *newcond = (csize->toLong() > to ) ?
                truncate(const_cast<SyExpr*>(cond), 
                csize->toLong(), to) : cond->copy();
            */    
            SyExpr *newtexpr =     (tsize->toLong() > to ) ?
                truncate(const_cast<SyExpr*>(texpr), 
                tsize->toLong(), to) : texpr->copy();
            SyExpr *newfexpr =     (fsize->toLong() > to ) ?
                truncate(const_cast<SyExpr*>(fexpr), 
                fsize->toLong(), to) : fexpr->copy();
            delete csize;
            delete tsize;
            delete fsize;
            return new SyCondExpr(    /*new*/cond, newtexpr, newfexpr);
        }
        else if (SyUnary *unary = dynamic_cast<SyUnary*>(expr))
        {
            SyConst *usize = unary->size();
            if(usize->toLong() == 1) {
                return unary;
            }    

            SyUnary *newunary = new SyUnary(truncate(
                const_cast<SyExpr*>(unary->getExpr()), from, to), 
                unary->getOperator());
            return newunary;
        }
        else if (SyBinary *binary = dynamic_cast<SyBinary*>(expr))
        {
            SyExpr::SyOp op = binary->getOperator();
            switch(op)
            {
                case SyExpr::GTHAN: 
                case SyExpr::GTHANEQ:
                case SyExpr::LTHAN:
                case SyExpr::LTHANEQ:
                case SyExpr::LOGEQ:
                case SyExpr::LOGENEQ:
                case SyExpr::CASEEQ:
                case SyExpr::CASEENEQ:
                case SyExpr::LOGAND:
                case SyExpr::LOGOR:
                //{
                   //    SyConst *lsize = binary->lexpr->size();
                //    SyConst *rsize = binary->rexpr->size();
                //    to = std::max(lsize->toLong(), rsize->toLong());
                //    delete lsize;
                //    delete rsize;
                //}    
                //    break;
                return binary;
                default:
                    break;
            }        


            SyBinary *newbinary = new SyBinary(
                truncate(const_cast<SyExpr*>(binary->getLExpr()), from, to),
                truncate(const_cast<SyExpr*>(binary->getRExpr()), from, to),
                binary->getOperator());
            return newbinary;
        }
        return expr;
    }
}
