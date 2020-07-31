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

#include "sycntvs.h"
#include "syconst.h"
#include "sycontainer.h"

#include "syvisitor.h"
#include "syroot.h"
#include "tvisitor.h"

using std::string;
using std::vector;
namespace Synth
{
    bool
    SyCntv::contains(const string& str) const
    {
        if(this->toString() == str) {
            return true;
        }
        return false;
    }    
    SyScCntv::~SyScCntv()
    {
        for(unsigned int i = 0; i < val.size(); ++i){
            if(dynamic_cast<SyConst*>(val[i])){
                delete val[i];
            }
        }
        val.clear();
    }

    SyVCntv::~SyVCntv() {
        for(unsigned int i = 0; i < val.size(); ++i){
            if(dynamic_cast<SyConst*>(val[i])){
                delete val[i];
            }
        }
        val.clear();
    }
    SyExpr*
    SyVCntv::copy() const
    {
        SyConcat *concat = new SyConcat(this->isSigned());
        for(SyNode<SyScCntv*>* node = bits.head; node; node = node->next) {
            concat->addBit(node->val);
        }
        return concat;
    }

    SyConcat*
    SyVCntv::getConcat() const {
        return static_cast<SyConcat*>(this->copy());
    }

    bool
    SyVCntv::contains(const string& str) const
    {
        if(this->toString() == str) {
            return true;
        }

        if(this->getName() == str) {
            return true;
        }

        SyIxName ixname(str);
        if(ixname.getName() == this->getName()) 
        {
            long index = ixname.getIndex();
            return this->getVecBit(index) != 0;
        }
        return false;
            
    }

    SyVPort::~SyVPort(){
    }

    SyScPort::~SyScPort(){
    }

    SyScReg::~SyScReg(){
    }

    SyVReg::~SyVReg(){
    }

    SyVPin::SyVPin( const std::string& n, 
                    const int msb, 
                    const int lsb,
                    const SyExpr *act,
                    const SyInst *inst) :
                    SyVCntv(n, false),
                    actual(act)
    {
        AddCntvBits<SyVPin, SyScPin>(this, msb, lsb, false);
        SyList<SyExpr*>* actualBits = this->getActualBits(act);
        SyNode<SyExpr*>* actExpr = actualBits->head;
        for(struct SyNode<SyScCntv*>* node = bits.head; node;
            node = node->next) 
        {
            SyScPin *pin = dynamic_cast<SyScPin*>(node->val);
            pin->inst = const_cast<SyInst*>(inst);
            if(actExpr)
            {
                pin->actual = actExpr->val;
                actExpr = actExpr->next;
            }    
        }
    }
    SyExpr*
    SyVPin::getActual() 
    {
        return const_cast<SyExpr*>(actual);
    }    

    const SyExpr*
    SyVPin::getActual() const
    {
        return actual;
    /*
        SyConcat *conc = new SyConcat();
        for(struct SyNode<SyScCntv*>* node = bits.head; node;
            node = node->next) 
        {
            SyScPin *pin = dynamic_cast<SyScPin*>(node->val);
            const SyExpr *actual = pin->getActual();
            conc->addBit(actual);
            if(actual) {
                conc->setSign(actual->isSigned());
            }    
        }
        //May contain null in between
        return conc;
    */    
    }    

    SyList<SyExpr*>*
    SyVPin::getActualBits(const SyExpr *act) const
    {
        SyList<SyExpr*>* retBits = new SyList<SyExpr*>();
        if(const SyConcat *conc = dynamic_cast<const SyConcat*>(act)) {
            *retBits = conc->getExprs();
        }
        else if(const SyVCntv* vcntv = dynamic_cast<const SyVCntv*>(act))
        {
            const SyList<SyScCntv*>& bits = vcntv->getBits();
            for(SyNode<SyScCntv*>* node = bits.head; node; node = node->next) {
                retBits->push_back(node->val);
            }
        }    

        return retBits;
    }    
                    

    void
        SyScCntv::addVal(const SyExpr *v, bool discard){
            if(discard) {
                this->removeVal();
            }
            val.push_back(const_cast<SyExpr*>(v->copy()));
        }
    void
        SyScCntv::removeVal()
        {
            for(unsigned int i = 0; i < val.size(); ++i){
                if(dynamic_cast<SyConst*>(val[i])){
                    delete val[i];
                }
            }
            val.clear();
        }    

    SyConst*
        SyScCntv::evaluate() const {
            return Evaluate(val);
        }
    SyConst*
        SyScCntv::size() const{
            return new SyConst(1, true, false);
        }

    void
        SyVCntv::addVal(const SyExpr *v, bool discard){
            if(discard) {
                this->removeVal();
            }
            SyNode<SyScCntv*>* node = bits.head;
            SyConcat *conc = new SyConcat(this->isSigned());
            SyConst *const_ = v->evaluate();
            if(const_)
            {
                vector<SyConst*> constbits = const_->getBits();
                SyNode<SyScCntv*>* limit = 0;
                unsigned int j = 0;
                if((unsigned int)bits.size() > constbits.size()){
                    int diff = bits.size() - constbits.size();
                    limit = bits.head;
                    for(int i = 0; i < diff; ++i){
                        limit = limit->next;
                    }
                    for(node = bits.head; node != limit; node = node->next){
                        if(this->isSigned()){
                            //sign extension
                            node->val->addVal(constbits[0]->copy());
                            conc->addBit(constbits[0]->copy());
                        }
                        else
                        {
                            node->val->addVal(SyConst::ZERO());
                            conc->addBit(SyConst::ZERO());
                        }
                    }
                }
                else {
                    j = constbits.size() - bits.size();
                }
                for(; (j < constbits.size()) && node; ++j, node = node->next) {
                    node->val->addVal(constbits[j]->copy(), discard);
                        conc->addBit(constbits[j]->copy());
                }
                val.push_back(conc);

            }
            else
            {
           //     for(int i = 0; (i < bits.size()) && node; ++i, node = node->next) {
            //        node->val->addVal(val[i]->copy(), discard);
            //    }
            //    val.push_back(const_cast<SyExpr*>(v->copy()));
            }

        }

    void
        SyVCntv::removeVal()
        {
            for(unsigned int i = 0; i < val.size(); ++i){
                if(dynamic_cast<SyConst*>(val[i])){
                    delete val[i];
                }
            }
            val.clear();
        }
    SyConst*
        SyVCntv::evaluate() const 
        {
            string tmp;
            for(SyNode<SyScCntv*>* node = bits.head; node; 
                    node = node->next)
            {
                //Note: multi driven case not handled
                const vector<SyExpr*> & v = node->val->getVal();
                if(v.empty()) {
                    return NULL;
                }    
                SyConst *const_ = dynamic_cast<SyConst*>(
                        node->val->getVal()[0]);
                if(!const_){
                    return 0;
                }
                //tmp += const_->getVal();
                tmp += string(const_->iszero() ? "0" : "1");
            }
            return new SyConst(this->isSigned(), tmp);
            //return Evaluate(val);
        }

    SyConst*
        SyVCntv::size() const {
            return new SyConst(SyConst(bits.size(), true, false)* 
                    SyConst(1, true, false));
        }
    SyConst*
        SyBitSel::size() const {
            return new SyConst(1, true, false);
        }


    void
        SyScCntv::repSubExpr(
                const string& substr,
                const SyExpr *newexpr)
        {
            RepSubExpr(substr, newexpr, val);
        }

    void
        SyVCntv::repSubExpr(
                const string& substr,
                const SyExpr *newexpr)
        {
            RepSubExpr(substr, newexpr, val);
        }

    SyConst*
        Evaluate(const vector<SyExpr*>& val)
        {
            signed short flag = -1; 
            //flag 0-> all 0
            //flag 1-> all 1
            //flag 2-> conflict, multi driven, error

            for(unsigned int i = 0; i < val.size(); ++i)
            {
                SyConst *const_ = val[i] ? val[i]->evaluate() : 0;
                if(!const_)
                {
                    flag = 2;
                    break;
                }

                if(const_->iszero()){
                    if(flag == -1){
                        flag = 0;
                    }
                    else if(flag == 1)
                    {
                        flag = 2;
                        break;
                    }
                }
                else if(const_->isone()){
                    if(flag == -1){
                        flag = 1;
                    }
                    else if(flag == 0)
                    {
                        flag = 2;
                        break;
                    }
                }
            }
            if(flag == 0){
                return SyConst::ZERO();
            }
            else if(flag == 1){
                return SyConst::ONE();
            }
            else {
                return 0;
            }
        }
    void
        RepSubExpr(
                const string& substr,
                const SyExpr *newexpr,
                vector<SyExpr*>& val)
        {
            for(unsigned int i = 0; i < val.size(); ++i)
            {
                if(val[i]&& (val[i]->toString() == substr)){
                    if(const SyConst *newconst = dynamic_cast<const SyConst*>(
                            newexpr))
                    {
                        delete val[i];
                        val[i] = newconst->copy();
                    }
                }
            }
        }

    void
        SyScNet::accept(SyVisitorBase* v) const{
            v->visit(this);
        }

    void
        SyScNet::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyVNet::accept(SyVisitorBase* v) const{
            v->visit(this);
        }
    void
        SyVNet::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyScPort::accept(SyVisitorBase* v) const{
            v->visit(this);
        }
    void
        SyScPort::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyVPort::accept(SyVisitorBase* v) const{
            v->visit(this);
        }
    void
        SyVPort::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyScPin::accept(SyVisitorBase* v) const{
            v->visit(this);
        }
    void
        SyScPin::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyVPin::accept(SyVisitorBase* v) const{
            v->visit(this);
        }

    void
        SyVPin::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyScReg::accept(SyVisitorBase* v) const{
            v->visit(this);
        }

    void
        SyScReg::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyVReg::accept(SyVisitorBase* v) const{
            v->visit(this);
        }

    void
        SyVReg::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }

    SyScPort::SyDirType
        SyScPin::getDir() const 
        {
            if(SyModInst *modInst = dynamic_cast<SyModInst*>(inst))
            {
                const SyModule *mod = modInst->getMaster();
                const SyScPort *port = dynamic_cast<const SyScPort*>(
                        mod->findObject(this->getIxName()));
                return port->getDir();
            }
            else 
            {
                string tname = this->getIxName().toString();
                if(tname[0] == 'o'){
                    return SyScPort::OUT;
                }
                return SyScPort::IN;
            }
            return SyScPort::NODIR; //should not reach here
        }

    const SyVCntv*
        SyScCntv::getVecCntv(const string& modname) const
        {
            const SyModule *mod = dynamic_cast<const SyModule*>(
                    SyRoot::instance()->getModule(modname));
            if(!mod){
                return 0;
            }

          //  return dynamic_cast<const SyVCntv*>(mod->findObject(
          //              this->getIxName().getName()));
            const SyBase *base = mod->findObject(this->getIxName().getName());
            if(!base){
                return 0;
            }
            const SyVCntv *vcntv = dynamic_cast<const SyVCntv*>(base);
            if(!vcntv){
                //Try this could be the variable corresponding to the return
                //of a function.
                const SyFunc *func = dynamic_cast<const SyFunc*>(base);
                if(func){
                    vcntv = dynamic_cast<const SyVCntv*>(func->getRetCntv());
                }
            }
            return vcntv;
        }
#if 0        
    template <class T1, class T2>
        const T2* GetVecCntv(
                const std::string& cntvname,
                const std::string& modname,
                const std::string& taskorfuncname)
        {
            const SyModule *mod = dynamic_cast<const SyModule*>(
                    SyRoot::instance()->getModule(modname));
            if(!mod){
                return 0;
            }
            const T2 *retcntv = 0;
            if(!taskorfuncname.empty())
            {
                //If task/function name supplied, then try first to look
                // into task function scope. Note that all such variables
                //can only be port of task/function. All other declarations
                //are moved out of the function/task and exist at module scope,
                //with consistently changed name.
                const SyTaskOrFunc *taskorfunc = 
                    dynamic_cast<const SyTaskOrFunc*>(mod->findObject(
                                taskorfuncname));
                if(taskorfunc){
                    retcntv = dynamic_cast<const T2*>(taskorfunc->findObject(
                                cntvname));
                }
            }
            if(!retcntv){
                retcntv = dynamic_cast<const T2*>(mod->findObject(cntvname));
            }
        }
#endif

    string
        SyBitSel::toString() const
        {
            SyVCntv *vecvar = dynamic_cast<SyVCntv*>(var);
            assert(vecvar);
            std::ostringstream str;
            str << vecvar->getName();

            for(unsigned int i = 0; i < dim.size(); ++i){
                str << "[" << dim[i]->toString() << "]";
            }
            return str.str();
        }

    void
        SyBitSel::repSubExpr(
                const string& subexpr,
                const SyExpr *newexpr)
        {
            if(var->toString() == subexpr) {
                //delete var;
                var = newexpr->copy();
            }
            else {
                var->repSubExpr(subexpr, newexpr);
            }

            for(unsigned int i = 0; i < dim.size(); ++i)
            {
                if(dim[i]->toString() == subexpr)
                {
                    if(!dynamic_cast<SyCntv*>(dim[i]) ||
                        dynamic_cast<SyConst*>(dim[i])){
                        delete dim[i];
                    }
                    dim[i] = newexpr->copy();
                }
                else {
                    dim[i]->repSubExpr(subexpr, newexpr);
                }
            }
        }

    void
        SyPartSel::repSubExpr(
                const string& subexpr,
                const SyExpr *newexpr)
        {
            SyBitSel::repSubExpr(subexpr, newexpr);

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
            else{
                rexpr->repSubExpr(subexpr, newexpr);
            }
        }

    SyConst*
        SyPartSel::size() const
        {
            //const SyConst *lconst = dynamic_cast<const SyConst*>(lexpr);
            //const SyConst *rconst = dynamic_cast<const SyConst*>(rexpr);
            const SyConst *lconst = lexpr->evaluate();
            const SyConst *rconst = rexpr->evaluate();

            if(!lconst || !rconst) {
                return 0;
            }
            long tsize = abs(lconst->toLong() - rconst->toLong()) + 1;
            return new SyConst(tsize, true, false);
            /*SyConst retsize(true, string("00"));
            SyConst *lsize = lexpr->size();
            SyConst *rsize = rexpr->size();
            retsize = *lsize > *rsize ? (*lsize - *rsize) : (*rsize- *lsize);
            delete lsize;
            delete rsize;
            return (retsize+SyConst(true,"01")).copy();
            */
        }

    string    
        SyConcat::busMerge() const
        {
            int index = 0;
            int msb = 0;
            string name;

            
            if(exprs.empty()) {
                return string();
            }    
            //flag values:
            //0 => undecided 
            //1 => decreasing left to right, 
            //2 => increasing left to right
            int flag;
            
            for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next)
            {
                SyScCntv * sccntv = dynamic_cast<SyScCntv*>(node->val);
                if(!sccntv) {
                    return string();
                }
                if(!sccntv->isVecBit()) {
                    return string();
                }

                SyIxName ixname = sccntv->getIxName();
                int newindex = ixname.getIndex();
                string newname = ixname.getName();
                if(name.empty()) 
                {
                    name = newname;
                    index = newindex;
                    msb = index;
                    flag = 0;
                    continue;
                }
                if(name != newname) {
                    return string();
                }
                if(abs(index - newindex) != 1) {
                    return string();
                }
                if(flag == 0) 
                {
                    if(index > newindex) {
                        flag = 1;
                    }
                    else {
                        flag = 2;
                    }    
                }
                else if(flag == 1) {
                    if(index < newindex) {
                        return string();
                    }
                }
                else if(flag == 2) {
                    if(index > newindex) {
                        return string();
                    }
                }    
                index = newindex;
            }    

            std::ostringstream ss;
            ss << name << "[" << msb << ":" << index << "]";
            return ss.str();
        }

    void
        SyConcat::repSubExpr(
                const string& subexpr,
                const SyExpr *newexpr)
        {
            string slice = this->busMerge();

            if(!slice.empty() && ( slice == subexpr))
            {
                exprs.clear();
                exprs.push_back(newexpr->copy());
                return;
            }
            
            SyList<SyExpr*> newlist;
            for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next){
                if(node->val->toString() == subexpr)
                {
                    newlist.push_back(newexpr->copy());
                }
                else 
                {
                    SyExpr *tmp = node->val->copy();
                    newlist.push_back(tmp);
                    tmp->repSubExpr(subexpr, newexpr);
                }
            }
            for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next){
                if(!dynamic_cast<SyCntv*>(node->val) ||
                    dynamic_cast<SyConst*>(node->val)){
                    delete node->val;
                }
            }
            exprs = newlist;
        }

    void
        SyConcat::repVCntvByConcat()
        {
            for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next)
            {
                if(SyVCntv *vcntv = dynamic_cast<SyVCntv*>(node->val)) {
                    node-> val = vcntv->getConcat();
                }
                else {
                    node->val->repVCntvByConcat();
                }
            }    
            
        }

    SyConst*
        SyConcat::evaluate() const
        {
            string str;

            for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next)
            {
                SyConst *const_ = node->val->evaluate();
                if(!const_){
                    return 0;
                }
                if(str.empty()){
                    str = const_->getVal();
                }
                else {
                    str += const_->getVal();
                }    
            }
            return new SyConst(this->isSigned(), str);
        }

    SyConst*
        SyConcat::size() const
        {
            SyConst *rsize = SyConst::ZERO();
            for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next) {
                if(node->val) {
                    *rsize = *rsize + *(node->val->size());
                }
                else 
                {
                    SyConst *one = SyConst::ONE();
                    *rsize = *rsize + *one;
                    delete one;
                }    
            }
            return rsize;
        }
    
    bool
        SyConcat::contains(const string& str) const
        {
            if(this->toString() == str) {
                return true;
            }
            for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next){ 
                if(node->val->contains(str)) {
                    return true;
                }
            }
            return false;
        }
        
    SyConcat*    
        SyConcat::flatten() const
        {
            SyConcat *conc = new SyConcat(this->isSigned());

            for(SyNode<SyExpr*> *node = exprs.head; node; node = node->next)
            {
                SyConcat *tconc = dynamic_cast<SyConcat*>(node->val);
                if(!tconc) {
                    conc->addBit(node->val);
                }
                else
                {
                    SyConcat *newconc = tconc->flatten();
                    const SyList<SyExpr*>& newexprs = newconc->getExprs();
                    for(SyNode<SyExpr*> *n = newexprs.head; n; n = n->next) {
                        conc->addBit(n->val);
                    }    
                }
            }
            return conc;
        }

    SyConst*
        SyBitSel::evaluate() const
        {
            SyConst *sel = dim[dim.size()-1]->evaluate();
            if(!sel) {
                return 0;
            }

            long selbit = sel->toLong();
            SyVCntv *vecvar = dynamic_cast<SyVCntv*>(var);
            const SyScCntv *svar = vecvar->getBit(selbit);

            if(!svar){
                return 0;
            }
            return svar->evaluate();
        }

    void
        SyBitSel::addVal(
                const SyExpr *v, 
                bool discard)
        {
            if(discard) {
                this->removeVal();
            }    
            const SyConst *const_ = dynamic_cast<const SyConst*>(v);
            SyExpr *selbit = dim[dim.size() -1];
            if(SyConst *bitconst = selbit->evaluate())
            {
                SyVCntv *vcntv = dynamic_cast<SyVCntv*>(var);
                const SyScCntv *actual = vcntv->getBit(bitconst->toLong());
                const_cast<SyScCntv*>(actual)->addVal(const_);
            }
        }

    void
        SyBitSel::removeVal()
        {
            SyExpr *selbit = dim[dim.size() -1];
            if(SyConst *bitconst = selbit->evaluate())
            {
                SyVCntv *vcntv = dynamic_cast<SyVCntv*>(var);
                const SyScCntv *actual = vcntv->getBit(bitconst->toLong());
                const_cast<SyScCntv*>(actual)->removeVal();
                delete bitconst;
            }
        }


    SyConst*
        SyPartSel::evaluate() const
        {
            SyConst *lconst = lexpr->evaluate();
            SyConst *rconst = rexpr->evaluate();

            if(!lconst || !rconst){
                return 0;
            }

            const long msb = lconst->toLong();
            const long lsb = rconst->toLong();

            SyConcat *concat = new SyConcat(false);
            const SyVCntv *var = dynamic_cast<const SyVCntv*>(
                    this->getVar());

            for(int i = msb; (msb > lsb) ? i >= lsb : i <= lsb;
                    (msb > lsb) ? --i : ++i)
            {
                const SyScCntv *cntv = var->getBit(i);
                SyConst *const_ = cntv->evaluate();
                if(!const_){
                    return 0;
                }
                concat->addBit(const_);
            }
            SyConst *const_ = concat->evaluate();
            delete concat;
            return const_;
        }
    
    SyConcat*
        SyPartSel::getConcat() const
        {
            SyConst *lconst = lexpr->evaluate();
            SyConst *rconst = rexpr->evaluate();

            if(!lconst || !rconst){
                return 0;
            }

            const long msb = lconst->toLong();
            const long lsb = rconst->toLong();

            SyConcat *concat = new SyConcat(false);
            const SyVCntv *var = dynamic_cast<const SyVCntv*>(
                    this->getVar());

            for(int i = msb; (msb > lsb) ? i >= lsb : i <= lsb;
                    (msb > lsb) ? --i : ++i)
            {
                const SyScCntv *cntv = var->getBit(i);
                concat->addBit(cntv);
            }
            //part selects are always unsigned
            concat->setSign(false);
            
            return concat;
        }    

    void
        SyPartSel::addVal(
                const SyExpr *v, 
                bool discard)
        {
            const SyConst *const_ = dynamic_cast<const SyConst*>(v);
            if(!const_){
                return;
            }
            const SyConst *cmsb = dynamic_cast<SyConst*>(lexpr);
            const SyConst *clsb = dynamic_cast<SyConst*>(rexpr);

            if(!cmsb || !clsb){
                return;
            }
            const long msb = cmsb->toLong();
            const long lsb = clsb->toLong();
            delete cmsb;
            delete clsb;
            const long csize = (const_->size())->toLong();

            const SyVCntv *var = dynamic_cast<const SyVCntv*>(
                    this->getVar());
            for(int i = 0, j = msb; 
                 (i < csize) && (msb > lsb ? j >=lsb : j <= lsb);
                 ++i, (msb > lsb ? --j : ++j))
            {
                const SyScCntv *sccntv = var->getBit(j);
                const_cast<SyScCntv*>(sccntv)->addVal((*const_)[i].copy(), discard);
            }
        }

    void
        SyPartSel::removeVal()
        {
            const SyConst *cmsb = dynamic_cast<SyConst*>(lexpr);
            const SyConst *clsb = dynamic_cast<SyConst*>(rexpr);

            if(!cmsb || !clsb){
                return;
            }
            const long msb = cmsb->toLong();
            const long lsb = clsb->toLong();
            delete cmsb;
            delete clsb;

            const SyVCntv *var = dynamic_cast<const SyVCntv*>(
                    this->getVar());
            for(int j = msb; (msb > lsb ? j >=lsb : j <= lsb);
                  (msb > lsb ? --j : ++j))
            {
                const SyScCntv *sccntv = var->getBit(j);
                const_cast<SyScCntv*>(sccntv)->removeVal();
            }
        }

    void
        SyConcat::addVal(
                const SyExpr *v, 
                bool discard)
        {
            const SyConst *const_ = dynamic_cast<const SyConst*>(v);
            if(!const_){
                return;
            }
            string bits = const_->getVal();
            SyList<SyExpr*> texpr;
            for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next) {
                texpr.push_front(node->val);
            }

            std::reverse(bits.begin(), bits.end());
            int limit = bits.size();
            int total = 0;
            SyNode<SyExpr*>* node = texpr.head;
            for(; (total < bits.size()) && node;  node = node->next)
            {
                long exprsize = node->val->size()->toLong();
                string num = bits.substr(total, exprsize);
                total += exprsize;
                if(total > limit) {
                    num = string(total-limit, '0') + num;
                }    

                SyConst val(false, num);
                node->val->addVal(val.copy());
            }

            for(; node; node = node->next){
                node->val->addVal(SyConst::ZERO());
            }
    #if 0        
            const long constsize = const_->size()->toLong();
            SyList<SyExpr*> texpr;
            for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next) {
                texpr.push_front(node->val);
            }
            signed long limit = constsize;
            SyNode<SyExpr*>* node = texpr.head;

            for(; node; node = node->next)
            {
                if(limit <= 0){
                    break;
                }
                const long exprsize = node->val->size()->toLong();
                SyConst slice = limit > exprsize ? 
                    const_->slice(limit-exprsize, limit) : const_->slice(
                            0, limit);
                limit -= exprsize;
                node->val->addVal(slice.copy(), discard);
            }
            for(; node; node = node->next){
                node->val->addVal(SyConst::ZERO());
            }
    #endif        

        }

    void
    SyConcat::removeVal()
    {
        for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next) {
            node->val->removeVal();
        }    
    }



    SyConst*
    SyXOrZ::size() const {
        return SyConst::ONE();
    }    

    void
        SyXOrZ::accept(SyVisitorBase *v) const{
            v->visit(this);
        }
    void
        SyBitSel::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    void
        SyBitSel::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyPartSel::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    void
        SyPartSel::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    void
        SyConcat::accept(SyVisitorBase *v) const{
            v->visit(this);
        }

    void
        SyConcat::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }
    SyExpr*
        SyBitSel::copy() const 
        {
            std::vector<SyExpr*> tdim;
            for(unsigned int i = 0; i < dim.size(); ++i){
                tdim.push_back(dim[i]->copy());
            }
            return new SyBitSel(static_cast<SyVCntv*>(var)->noConcatCopy(), tdim);
        }

    bool
        SyBitSel::isConstSelect() const
        {
#if 0        
            //For now always return false.
            //This has issues with  un rolling of loops.
            //Since primitive connectives are not replaced
            //by an expr
            //return false;
#endif            
            for(unsigned int i = 0; i < dim.size(); ++i) 
            {
                SyConst *const_ = dim[i]->evaluate();
                if(!const_)
                {
                    return false;
                }    
                delete const_;
            }
            return true;
        }    

    SyScCntv*
        SyBitSel::isVecBit() const
        {
            if(!isConstSelect()) {
                return NULL;
            }    
            SyVCntv *vcntv = dynamic_cast<SyVCntv*>(var);
            assert(var);
            SyConst *const_ = dim[dim.size()-1]->evaluate();
            long tmp = 0;
            const vector<SyExpr*>& tval = vcntv->getVal();
            for(unsigned int i = 0; !tval.empty() && i < tval.size() - 1; ++i)
            {
                SyConst *c1 = tval[i]->evaluate();
                SyConst *c2 = dim[i]->evaluate();
                tmp +=  c1->toLong() * c2->toLong();
                delete c1;
                delete c2;
            }
            tmp += const_->toLong();
            delete const_;
            return const_cast<SyScCntv*>(vcntv->getBit(tmp));
        }
}
