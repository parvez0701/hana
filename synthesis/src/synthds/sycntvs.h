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

#ifndef _SYCNTVS_H_
#define _SYCNTVS_H_

#include <sstream>
#include <assert.h>
#include <ctype.h>

#include "sybase.h"
#include "syexpr.h"

#include "sylist.h"
#include "syixname.h"

#include "sydefines.h"

namespace Synth
{
    class SyInst;
    class SyConst;
    class SyConcat;
    class SyVisitorBase;
    class SyVNet;
    class SyModule;
    class SyPartSel;
    class TVisitor;

    template <class T1, class T2>
        void AddCntvBits(
                T1 *t1,
                const int msb, 
                const int lsb,
                const bool sign);

    template <class T1, class T2>
        const T2* GetBit(
                const T1 *t1, 
                const int bit);

   // template <class T1, class T2>
   //     const T2* GetVecCntv(
   //             const std::string& cntvname,
   //             const std::string& modname,
   //             const std::string& taskorfuncname=std::string(""));

    class SyCntv: public SyExpr
    {
        public:
            SyCntv(const bool s): SyExpr(), sign(s) {
            }
            virtual ~SyCntv() {
            }
            void operator delete(void *data) {
            }
            bool isSigned() const {
                return sign;
            }
            void setSign(const bool s=true){
                sign = s;
            }
            virtual bool contains(const std::string&) const;
        private:
            bool sign;
    };
    class SyConcat: public SyCntv
    {
        friend class SyOMVisitor;
        public:
            SyConcat() : SyCntv(true){
            }
            SyConcat(const bool sign_) : SyCntv(sign_){
            }
            virtual ~SyConcat() {
            }
            void addBit(const SyExpr *expr){
                exprs.push_back(const_cast<SyExpr*>(expr));
            }
            const SyList<SyExpr*>& getExprs() const {
                return exprs;
            }
            //virtual bool isSigned() const {
                //watchout : We are using SyConcat in general purpose
                //expression to extend other expressions. So the extended
                //expressions might be signed, which should be reflected 
                //in the newly formed SyConcat. So even though, a concat
                //expression is always unsigned (LRM), we have genuine
                //signedness for SyConcat.
                
            //    return false;
            //}
            virtual std::string toString() const 
            {
                std::ostringstream str;
                str << "{";
                SyNode<SyExpr*>* head = exprs.head;
                if(head->val) {
                    str << head->val->toString();
                }    
                head = head->next;
                for(; head; head = head->next){
                    str << ", ";
                    if(head->val){
                        str << head->val->toString();
                    }    
                }
                str << "}";
                return str.str();
            }
            virtual SyConcat *copy() const 
            {
                SyConcat *concat = new SyConcat(this->isSigned());
                for(SyNode<SyExpr*>* head = exprs.head; head; 
                        head = head->next){
                    concat->addBit(head->val->copy());
                }
                return concat;
            }
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);

            virtual void repVCntvByConcat();

            virtual SyConst* evaluate() const;
            virtual SyConst* size() const;
            virtual void addVal(const SyExpr *v, bool discard);
            virtual void removeVal();
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor *v, SyModule*);
            virtual bool contains(const std::string&) const;

            SyConcat* flatten() const;

            //Possible only if concatenation contains all consecutive 
            //bits from a bus only
            std::string busMerge() const;

        private:
            SyList<SyExpr*> exprs;
    };

    class SyScCntv: public SyCntv
    {
        public:
            SyScCntv(const SyIxName& n, const bool s) :
                SyCntv(s), name(const_cast<SyIxName&>(n)) {
                }
            virtual ~SyScCntv();
            const SyIxName& getIxName() const {
                return name;
            }
            virtual std::string toString() const {
                return this->getIxName().toString();
            }
            virtual SyScCntv *copy() const {
                return const_cast<SyScCntv*>(this);
            }
            const std::vector<SyExpr*>& getVal() const {
                return val;
            }
            bool isVecBit() const {
                return name.getIndex() != SyIxName::nix;
            }
            const SyVCntv *getVecCntv(const std::string&) const;
            virtual void addVal(const SyExpr *v, bool discard=true);
            virtual void removeVal();
            virtual SyConst* evaluate() const;
            virtual SyConst* size() const;
            virtual void repSubExpr(
                    const std::string&,
                    const SyExpr*);

        private:
            SyIxName name;
            //vector because it could be multi driven case
            std::vector<SyExpr*> val;
    };

    class SyVCntv: public SyCntv
    {
        public:
            SyVCntv(
                    const std::string& n, 
                    const bool sign) :
                SyCntv(sign), name(n) {
                }
            virtual ~SyVCntv();
            virtual const SyScCntv* getBit(const int) const = 0;
            const SyList<SyScCntv*>& getBits() const {
                return bits;
            }
            const SyScCntv* getVecBit(const int bit) const{
                std::map<int, const SyScCntv*>::const_iterator iter = bitMap.find(bit);
                if(iter == bitMap.end()) {
                    return NULL;
                }
                return iter->second;
            }    
            void addBit(const SyScCntv* bit) 
            {
                bits.push_back(const_cast<SyScCntv*>(bit));
                bitMap.insert(std::make_pair(bit->getIxName().getIndex(), bit));
            }
            const std::string& getName() const {
                return name;
            }
            virtual std::string toString() const {
                std::ostringstream str;
                str << name << "[";
                SyScCntv *msbcntv = bits.front();
                str << msbcntv->getIxName().getIndex();
                SyScCntv *lsbcntv = bits.last();
                str << ":" << lsbcntv->getIxName().getIndex() << "]";
                return str.str();
            }
            //virtual SyVCntv *copy() const {
            //    return const_cast<SyVCntv*>(this);
            //}
            SyVCntv* noConcatCopy() const {
                return const_cast<SyVCntv*>(this);
            }
            virtual SyExpr* copy() const;
            SyConcat* getConcat() const;

            int getMsb() const {
                return bits.front()->getIxName().getIndex();
            }
            int getLsb() const {
                return bits.last()->getIxName().getIndex();
            }
            const std::vector<SyExpr*>& getVal() const {
                return val;
            }
            virtual void addVal(const SyExpr *v, bool discard=true);
            virtual void removeVal();
            virtual SyConst* evaluate() const;
            virtual SyConst* size() const;
            virtual void repSubExpr(
                    const std::string&,
                    const SyExpr*);
            virtual bool contains(const std::string&) const;
        protected:
            //std::pair<int, int> msbLsb;
            //msb is always the first
            SyList<SyScCntv*> bits;
        private:
            std::string name;
            std::vector<SyExpr*> val;
            std::map<int, const SyScCntv*> bitMap;
    };

    class SyScNet: public SyScCntv
    {
        public:
            typedef enum {NONET, WIRE, WAND, WOR, TRI, TRIAND, TRIOR, TRI1,
                TRI0, SUPPLY0, SUPPLY1, REG, TRIREG } SyNetType;
            SyScNet(
                    const SyIxName& name,
                    const SyNetType t,
                    const bool sign):
                SyScCntv(name, sign), 
                netType(t) {
                }
            SyScNet(
                    const SyIxName& name,
                    const bool sign) :
                SyScCntv(name, sign) {
                }

            virtual ~SyScNet() {
            }
            virtual std::string toString() const {
                return SyScCntv::toString();
            }
            virtual SyScNet *copy() const{
                return const_cast<SyScNet*>(this);
            }
            SyNetType getNetType() const {
                return netType;
            }
            void setNetType(SyNetType type) {
                netType = type;
            }

            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor *v, SyModule*);
        private:
            SyNetType netType;
    };

    class SyVNet : public SyVCntv
    {
        public:
            SyVNet(
                    const std::string& n, 
                    const int msb, 
                    const int lsb,
                    const bool sign,
                    const SyScNet::SyNetType type) :
                SyVCntv(n, sign) {
                    AddCntvBits<SyVNet, SyScNet>(this, msb, lsb, sign);
                    for(SyNode<SyScCntv*>* node = bits.head; node; 
                            node = node->next){
                        (dynamic_cast<SyScNet*>(node->val))->setNetType(type);
                    }
                }
            virtual ~SyVNet() {
            }

            virtual const SyScNet* getBit(const int bit) const{
                return GetBit<SyVNet, SyScNet>(this, bit);
            }
            virtual std::string toString() const {
                return SyVCntv::toString();
            }
            //virtual SyVNet *copy() const {
            //    return const_cast<SyVNet*>(this);
            //}
            virtual SyConcat* copy() const {
                return dynamic_cast<SyConcat*>(SyVCntv::copy());
            }    
            SyScNet::SyNetType getNetType() const {
                const int lsb = this->getLsb();
                const SyScNet *net = this->getBit(lsb);
                return net->getNetType();
            }
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor *v, SyModule*);
    };

    class SyScPort: public SyScCntv
    {
        public:
            friend class SyVPort;
            typedef enum {NODIR, IN, OUT, INOUT } SyDirType;
            SyScPort(
                    const SyIxName& name,
                    const bool sign,
                    const SyDirType d,
                    const SyScNet::SyNetType t) :
                SyScCntv(name, sign),
                dir(d),
                netType(t){
                }
            SyScPort(
                    const SyIxName& n,
                    const bool sign) :
                SyScCntv(n, sign) {
                }
            virtual ~SyScPort();

            SyScNet::SyNetType getNetType() const {
                return netType;
            }

            SyDirType getDir() const {
                return dir;
            }

            virtual std::string toString() const {
                return SyScCntv::toString();
            }

            virtual SyScPort *copy() const {
                return const_cast<SyScPort*>(this);
            }
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor *v, SyModule*);

        private:
            SyDirType dir;
            SyScNet::SyNetType netType;
    };

    class SyVPort: public SyVCntv
    {
        public:
            SyVPort(
                    const std::string& n, 
                    const int msb, 
                    const int lsb,
                    const bool sign,
                    const SyScPort::SyDirType d,
                    const SyScNet::SyNetType type):
                SyVCntv(n, sign)
            {
                AddCntvBits<SyVPort, SyScPort>(this, msb, lsb, sign);
                for(struct SyNode<SyScCntv*>* node = bits.head; node;
                        node = node->next)
                {
                    SyScPort *port = dynamic_cast<SyScPort*>(node->val);
                    port->dir = d;
                    port->netType = type;
                }
            }

            virtual ~SyVPort();

            virtual const SyScPort* getBit(const int bit) const {
                return GetBit<SyVPort, SyScPort>(this, bit);
            }

            SyScNet::SyNetType getNetType() const {
                const SyScPort *scport = this->getBit(this->getLsb());
                return scport->getNetType();
            }
            SyScPort::SyDirType getDir() const {
                const SyScPort *scport = this->getBit(this->getLsb());
                return scport->getDir();
            }

            virtual std::string toString() const {
                return SyVCntv::toString();
            }

            //virtual SyVPort *copy() const {
            //    return const_cast<SyVPort*>(this);
            //}
            virtual SyConcat* copy() const {
                return dynamic_cast<SyConcat*>(SyVCntv::copy());
            }    
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor *v, SyModule*);
    };

    class SyScPin: public SyScCntv
    {
        friend class SyVPin;
        public:
            SyScPin(
                    const SyIxName& name,
                    const SyExpr *act,
                    const SyInst *ins):
                SyScCntv(name, false),
                actual(const_cast<SyExpr*>(act)),
                inst(const_cast<SyInst*>(ins)) {
                }
            SyScPin(
                    const SyIxName& n,
                    const bool sign
                   ):
                SyScCntv(n, sign) , actual(0), inst(0) {
                }
            void setActual(const SyExpr *act) {
                actual = const_cast<SyExpr*>(act);
            }
            const SyExpr* getActual() const {
                return actual;
            }
            virtual std::string toString() const {
                //check
                return SyScCntv::toString();
            }
            virtual SyScPin *copy() const {
                return new SyScPin(this->getIxName(), actual->copy(),
                        inst);
            }
            const SyInst *getInst() const {
                return inst;
            }
            SyScPort::SyDirType getDir() const;
            void disconnect() {
                this->setActual(0);
            }
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor *v, SyModule*);

        private:
            SyExpr *actual;
            SyInst *inst;
    };

    class SyVPin: public SyVCntv
    {
        public:
            SyVPin(
                    const std::string& n, 
                    const int msb, 
                    const int lsb,
                    const SyExpr *act,
                    const SyInst *inst);


            virtual const SyScPin* getBit(const int bit) const {
                return GetBit<SyVPin, SyScPin>(this, bit);
            }
            virtual std::string toString() const {
                return SyVCntv::toString();
            }
            const SyInst *getInst() const {
                return this->getBit(this->getMsb())->getInst();
            }
            //caller owns the pointer
            //SyConcat* getActual() const;
            const SyExpr* getActual() const;
            SyExpr* getActual();
            virtual SyVPin *copy() const {
                SyConcat *concat = new SyConcat();
                for(struct SyNode<SyScCntv*>* node = bits.head; node;
                    node = node->next) 
                {
                    concat->addBit(node->val->copy());
                }
                return new SyVPin(this->getName(), this->getMsb(),
                        this->getLsb(), concat,
                        this->getInst());
            }
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor *v, SyModule*);
        private:    
            SyList<SyExpr*>* getActualBits(const SyExpr*) const;
            const SyExpr *actual;
    };

    class SyScReg: public SyScCntv
    {
        public:
            SyScReg(
                    const SyIxName& name,
                    const bool sign):
                SyScCntv(name, sign), local(false) {
                }
            virtual ~SyScReg();
            virtual std::string toString() const {
                return SyScCntv::toString();
            }
            virtual SyScReg *copy() const {
                return const_cast<SyScReg*>(this);
            }
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor *v, SyModule*);
            void setLocal() {
                local = true;
            }
            bool isLocal() const {
                return local;
            }    
        private:
            //defined inside a named block, task or function
            bool local;
    };

    class SyVReg: public SyVCntv
    {
        public:
            SyVReg(
                    const std::string& n,
                    const int msb,
                    const int lsb,
                    const bool sign):
                SyVCntv(n, sign) { 
                    AddCntvBits<SyVReg, SyScReg>(this, msb, lsb, sign); 
                }
            virtual ~SyVReg();

            virtual const SyScReg* getBit(const int bit) const {
                return GetBit<SyVReg, SyScReg>(this, bit);
            }
            virtual std::string toString() const {
                return SyVCntv::toString();
            }

            //virtual SyVReg *copy() const {
            //    return const_cast<SyVReg*>(this);
            //}
            virtual SyConcat* copy() const {
                return dynamic_cast<SyConcat*>(SyVCntv::copy());
            }    
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor *v, SyModule*);
    };


    class SyBitSel: public SyCntv
    {
        friend class SyOMVisitor;
        public:
            SyBitSel(
                    const SyExpr* v,
                    const std::vector<SyExpr*>& d):
                SyCntv(v->isSigned()),
                var(const_cast<SyExpr*>(v)),
                dim(const_cast<std::vector<SyExpr*>&>(d)){
                }
            virtual ~SyBitSel() {
            }
            const SyExpr* getVar() const {
                return var;
            }
            const std::vector<SyExpr*>& getDim() const{
                return dim;
            }
            const SyExpr *getFirstDim() const {
                return dim[0];
            }    
            virtual bool isSigned() const {
                //bit select is always unsigned
                return false;
                //return var->isSigned();
            }
            virtual std::string toString() const;

            virtual SyExpr *copy() const;

            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);
            virtual SyConst* evaluate() const;
            virtual SyConst* size() const;
            virtual void addVal(const SyExpr *v, bool discard);
            virtual void removeVal();
            virtual void accept(SyVisitorBase*) const;
            virtual SyList<SyScCntv*>* toGate(SyModule* mod) {
                return NULL;
            }
            virtual void accept(TVisitor *v, SyModule*);
            bool isConstSelect() const;
            SyScCntv* isVecBit() const;

        private:
            SyExpr *var;
            std::vector<SyExpr*> dim;
    };

    class SyPartSel: public SyBitSel
    {
        friend class SyOMVisitor;
        public:
            SyPartSel(
                    const SyExpr* v, 
                    const std::vector<SyExpr*>& d,
                    const SyExpr *le,
                    const SyExpr *re):
                SyBitSel(v, d),
                lexpr(const_cast<SyExpr*>(le)), 
                rexpr(const_cast<SyExpr*>(re)) {
                }
            virtual ~SyPartSel() {
            }
            const SyExpr *getLExpr() const{
                return lexpr;
            }
            const SyExpr *getRExpr() const {
                return rexpr;
            }
            virtual std::string toString() const
            {
                std::ostringstream str;
                str << SyBitSel::toString();
                str << "[";
                str << lexpr->toString();
                str << ":";
                str << rexpr->toString();
                str << "]";
                return str.str();
            }

            virtual SyPartSel *copy() const 
            {
                std::vector<SyExpr*> tdim;
                const std::vector<SyExpr*>& dim = this->getDim();
                for(unsigned int i = 0; i < dim.size(); ++i){
                    tdim.push_back(dim[i]->copy());
                }
                return new SyPartSel(static_cast<const SyVCntv*>(this->getVar())->
                    noConcatCopy(), tdim,
                        lexpr->copy(), rexpr->copy());
            }
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);
            virtual SyConst* evaluate() const;
            virtual SyConst* size() const;
            virtual void addVal(const SyExpr *v, bool discard);
            virtual void removeVal();
            virtual void accept(SyVisitorBase*) const;
            virtual SyList<SyScCntv*>* toGate(SyModule* mod) {
                return NULL;
            }
            virtual void accept(TVisitor *v, SyModule*);
            
            //caller owns the pointer
            SyConcat* getConcat() const;


        private:
            SyExpr *lexpr;
            SyExpr *rexpr;
    };


    class SyXOrZ : public SyCntv
    {
        public:
            SyXOrZ(char c): SyCntv(false), xOrZ(toupper(c)) {
                assert(xOrZ == 'X' || xOrZ == 'Z');
            }
            SyXOrZ(const SyXOrZ& lhs) : SyCntv(false){
                xOrZ = lhs.xOrZ;
                this->setSign(false);
            }    
            
            virtual ~SyXOrZ() {
            }

            SyXOrZ& operator=(const SyXOrZ& lhs) 
            {
                xOrZ = lhs.xOrZ;
                this->setSign(false);
                return *this;
            }    

            bool operator==(const SyXOrZ& lhs) const {
                return (this->isX() == lhs.isX()) && 
                        (this->isZ() == lhs.isZ());
            }

            bool operator!=(const SyXOrZ& lhs) const {
                return !(*this == lhs);
            }    



            bool isX() const {
                return xOrZ == 'X';
            }

            bool isZ() const {
                return xOrZ == 'Z';
            }    

            virtual SyXOrZ* copy() const {
                return new SyXOrZ(*this);
            }    

            virtual std::string toString() const {
                return std::string("1'b") + std::string(xOrZ == 'X' ? "X" : "Z");
            }    

            virtual SyConst* size() const;
            virtual void accept(SyVisitorBase* v) const;
        private:
            char xOrZ;
    };




    class SyMemory: public SyVReg
    {
        public:
            SyMemory(
                    const std::string& n,
                    const int msb,
                    const int lsb,
                    const bool sign): SyVReg(n, msb, lsb, sign) { 
                }
            virtual ~SyMemory() {
            }
    };

    template <class T1, class T2>
        void AddCntvBits(
                T1 *t1,
                const int msb, 
                const int lsb,
                const bool sign)
        {
            const std::string& name = t1->getName();
            for(int i = msb; i != lsb; msb > lsb ? --i : ++i) {
                t1->addBit(new T2(SyIxName(name, i), sign));
            }
            //push the last remaining one
            t1->addBit(new T2(SyIxName(name, lsb), sign));
        }

    template <class T1, class T2>
        const T2* GetBit(
                const T1 *t1, 
                const int bit)
        {
            /*
            const SyList<SyScCntv*> & cntvs = t1->getBits();
            for(SyNode<SyScCntv*>* node = cntvs.head; node; node = node->next)
            {
                if(node->val->getIxName().getIndex() == bit) {
                    return dynamic_cast<const T2*>(node->val);
                }
            }
            return 0;
            */
            return dynamic_cast<const T2*>(t1->getVecBit(bit));
        }

    SyConst* Evaluate(const std::vector<SyExpr*>&);
    void RepSubExpr(
            const std::string&,
            const SyExpr *newexpr,
            std::vector<SyExpr*>&);
}
#endif
