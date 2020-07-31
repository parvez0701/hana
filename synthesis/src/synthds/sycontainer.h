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

#ifndef _SYCONTAINER_H_
#define _SYCONTAINER_H_

#include "sybase.h"
#include "syixname.h"
#include "sylist.h"
#include "sycntvs.h"

namespace Synth
{
    class SyScPort;
    class SyScNet;
    class SyModInst;
    class SyPrimInst;
    class SyContAssign;
    class SyAlways;
    class SyTask;
    class SyFunc;
    class SyScPin;
    class SyVPin;
    class SyScReg;
    class SyMemory;
    class SyStmt;
    class SyScCntv;
    class SyVisitorBase;
    class TVisitor;

    template<class T1, class T2> void ResetCntvs(
            const std::string&,
            const SyList<T1*>&,
            const std::string& = "");
    class SyScope: public SyBase
    {
        public:
            SyScope(const std::string& n) :
                SyBase(),
                name(n) {
                }
            virtual ~SyScope() {
            }
            const std::string& getName() const {
                return name;
            }

            const SyList<SyScPort*>& getPorts() const {
                return ports;
            }

            //scalar and vector ports mixed
            SyList<SyCntv*>* getMixPorts() const;

            const SyList<SyScReg*>& getRegs() const {
                return regs;
            }

            inline void addObject(
                    const std::string&, 
                    const SyBase*, 
                    const bool=true);
            virtual const SyBase* findObject(const std::string&) const;
            virtual const SyBase* findObject(const SyIxName&) const;
            inline void removeObject(const std::string&);
            void addPort(const SyScPort *port) {
                if(port)
                {
                    ports.push_back(const_cast<SyScPort*>(port));
                    this->addObject(port->getIxName().toString(), port);
                }
            }
            void addReg(const SyScReg *reg){
                if(reg){
                    regs.push_back(const_cast<SyScReg*>(reg));
                }
            }
            virtual void resetCntvs();
        private:
            std::string name;
            SyList<SyScPort*> ports;
            SyList<SyScReg*> regs;
            std::map<std::string, SyBase*, cmp_str> symbolTable;
    };
    class SyModule: public SyScope
    {
        public:
            typedef std::map<std::string, const SyCntv*, cmp_str> TfPortTable;
            SyModule(const std::string& n):
                SyScope(n) {
                }
            virtual ~SyModule() {
            }

            void addNet(const SyScNet *net) {
                if(net){
                    nets.push_back(const_cast<SyScNet*>(net));
                }
            }

            void addMemory(const SyMemory *mem){
                if(mem){
                    memories.push_back(const_cast<SyMemory*>(mem));
                }
            }
            void addModInst(const SyModInst *minst){
                if(minst){
                    minsts.push_back(const_cast<SyModInst*>(minst));
                }
            }
            void addPrimInst(const SyPrimInst *pinst){
                if(pinst){
                    pinsts.push_back(const_cast<SyPrimInst*>(pinst));
                }
            }

            const SyList<SyScNet*>& getNets() const {
                return nets;
            }

            const SyList<SyModInst*>& getModInsts() const {
                return minsts;
            }

            const SyList<SyPrimInst*>& getPrimInsts() const {
                return pinsts;
            }
            const SyList<SyMemory*>& getMemories() const {
                return memories;
            }
            virtual void resetCntvs();
            bool connect(
                    SyScCntv*, 
                    SyCntv*, 
                    std::string hint,
                    const bool force=false);

            virtual void accept(SyVisitorBase *v) const;
            virtual void accept(TVisitor *v, SyModule*);

            void addTfPort(const std::string&, const SyCntv*);
            const SyCntv* getTfPort(const std::string&) const;
            const SyVPort* getTfVPort(const std::string&) const;
            const SyScPort* getTfSPort(const std::string&) const;

            const TfPortTable& getTfPorts() const;
            virtual const SyBase* findObject(const std::string&) const;
            virtual const SyBase* findObject(const SyIxName&) const;
            

        private:
            SyList<SyScNet*> nets;
            SyList<SyModInst*> minsts;
            SyList<SyPrimInst*> pinsts;
            SyList<SyMemory*> memories;
            //port names become unique after processing
            //storing them in a separate map
            TfPortTable tfPortTable;
    };

    void
        SyScope::addObject(
                const std::string& n,
                const SyBase *b,
                const bool rep)
        {
            if(rep){
                symbolTable.erase(n);
            }
            symbolTable.insert(std::make_pair(const_cast<std::string&>(n),
                        const_cast<SyBase*>(b)));
        }
    void
        SyScope::removeObject(const std::string& n)
        {
            symbolTable.erase(n);
        }

    //module with behavioural code
    class SyBModule: public SyModule
    {
        public:
            SyBModule(const std::string& n) :SyModule(n) {
            }
            virtual ~SyBModule() {
            }
            void addContAssign(const SyContAssign *assign) {
                if(assign){
                    cassigns.push_back(const_cast<SyContAssign*>(assign));
                }
            }

            void addAlways(const SyAlways *always) {
                if(always){
                    alwaysBlocks.push_back(const_cast<SyAlways*>(always));
                }
            }

            void addTask(const SyTask *task) {
                if(task){
                    tasks.push_back(const_cast<SyTask*>(task));
                }
            }

            void addFunc(const SyFunc *func) {
                if(func){
                    funcs.push_back(const_cast<SyFunc*>(func));
                }
            }

            const SyList<SyContAssign*>& getContAssigns() const {
                return cassigns;
            }

            const SyList<SyAlways*>& getAlwaysBlocks() const {
                return alwaysBlocks;
            }

            const SyList<SyTask*>& getTasks() const {
                return tasks;
            }

            const SyList<SyFunc*>& getFuncs() const {
                return funcs;
            }
            virtual void accept(SyVisitorBase *v) const;
            virtual void accept(TVisitor *v, SyModule*);
            virtual void resetCntvs();
        private:
            SyList<SyContAssign*> cassigns;
            SyList<SyAlways*> alwaysBlocks;
            SyList<SyTask*> tasks;
            SyList<SyFunc*> funcs;
    };

    class SyInst: public SyBase
    {
        public:
            SyInst(const SyIxName& n): SyBase(), name(n) {
            }
            virtual ~SyInst() {
            }

            const SyIxName& getIxName() const {
                return name;
            }
            void addPin(const SyScPin *pin){
                if(pin){
                    pins.push_back(const_cast<SyScPin*>(pin));
                }
            }
            void addPin(const SyVPin *pin){
                const SyList<SyScCntv*>& bits = pin->getBits();
                for(SyNode<SyScCntv*>* node = bits.head; node; 
                        node = node->next)
                {
                    //check, where the actual vector pin goes ?
                    const SyScPin *pin = dynamic_cast<const SyScPin*>(
                            node->val);
                    pins.push_back(const_cast<SyScPin*>(pin));
                }
            }
            const SyList<SyScPin*>& getPins() const {
                return pins;
            }
            SyScPin* getPin(const SyIxName& n) const
            {
                for(SyNode<SyScPin*>* node = pins.head; node; node = node->next){
                    if(node->val->getIxName() == n){
                        return node->val;
                    }
                }
                return 0;
            }

        private:
            SyIxName name;
            //to be changed to map
            SyList<SyScPin*> pins;
    };

    class SyPrimInst: public SyInst
    {
        public:
            typedef enum {
                NOGATE,
                OR,
                AND,
                NAND,
                NOR,
                XOR,
                XNOR,
                BUF,
                BUFIF0,
                BUFIF1,
                NOT,
                NOTIF0,
                NOTIF1
            } SyGateType;

            SyGateType getGateType() const {
                return gateType;
            }

            std::string getGateName() const 
            {
                switch(gateType)
                {
                    case OR: return "or";
                    case AND: return "and";
                    case NAND: return "nand";
                    case NOR: return "nor";
                    case XOR: return "xor";
                    case XNOR: return "xnor";
                    case BUF: return "buf";
                    case BUFIF0: return "bufif0";
                    case BUFIF1: return "bufif1";
                    case NOT: return "not";
                    case NOTIF0: return "notif0";
                    case NOTIF1: return "notif1";
                    default: return "";
                }
            }


            SyPrimInst(const SyIxName& n, SyGateType t):
                SyInst(n), gateType(t) {
                }

            virtual ~SyPrimInst() {
            }
            virtual void accept(SyVisitorBase *v) const;
            virtual void accept(TVisitor *v, SyModule*);
        private:
            SyGateType gateType;
    };

    class SyModInst: public SyInst
    {
        public:
            SyModInst(const SyIxName& n, const SyModule* m, 
                const std::string& name):
                SyInst(n), master(const_cast<SyModule*>(m)), masterName(name) {
                }

            virtual ~SyModInst() {
            }

            virtual void accept(SyVisitorBase *v) const;
            virtual void accept(TVisitor *v, SyModule*);
            const std::string& getMasterName() const {
                return masterName;
            }


            const SyModule* getMaster() const {
                return master;
            }
            void setMaster(const SyModule *m) {
                master = const_cast<SyModule*>(m);
            }

            void addVecPin(SyVPin *pin) {
                if(pin) {
                    vecPins.insert(std::make_pair(pin->getName(), pin));
                }
            }

            const SyVPin* getVecPin(const std::string& name) const {
                std::map<std::string, SyVPin*, cmp_str>::const_iterator
                    iter = vecPins.find(name);
                if(iter == vecPins.end()){
                    return NULL;
                }
                return iter->second;
            }    
        private:
            SyModule* master;
            std::string masterName;
            std::map<std::string, SyVPin*, cmp_str> vecPins;
    };

    class SyTaskOrFunc: public SyScope
    {
        public:
            SyTaskOrFunc(const std::string& n, bool a):
                SyScope(n), auto_(a) {
                }
            virtual ~SyTaskOrFunc() {
            }
            void addStmt(const SyStmt *st){
                stmts.push_back(const_cast<SyStmt*>(st));
            }
            const SyList<SyStmt*>& getStmts() const {
                return stmts;
            }
            bool isAuto() const {
                return auto_;
            }    
        private:
            SyList<SyStmt*>stmts;
            bool auto_;
    };

    class SyTask: public SyTaskOrFunc
    {
        public:
            SyTask(const std::string& n, bool auto_):
                SyTaskOrFunc(n, auto_) {
                }
            virtual ~SyTask() {
            }
            virtual void accept(SyVisitorBase *v) const;
            virtual void accept(TVisitor *v, SyModule*);
    };

    class SyFunc: public SyTaskOrFunc
    {
        public:
            SyFunc(
                    const std::string& n, 
                    const SyCntv* ret,
                    const bool sig,
                    const bool auto_) :
                SyTaskOrFunc(n, auto_), 
                retcntv(const_cast<SyCntv*>(ret)), 
                sign(sig) {
                }
            virtual ~SyFunc() {
            }
            bool isSigned() const {
                //A function with retur type as integer
                //might not be declared as signed. But it
                //is still signed function since return type
                //is signed.
                return sign || retcntv->isSigned();
            }
            virtual void accept(SyVisitorBase *v) const;
            virtual void accept(TVisitor *v, SyModule*);
            const SyCntv* getRetCntv() const {
                return retcntv;
            }
            SyCntv* getRetCntv() {
                return retcntv;
            }
            void setRetCntv(const SyCntv *cntv) {
                retcntv = const_cast<SyCntv*>(cntv);
            }    
        private:
            //Just to be used to get the return type
            SyCntv *retcntv;
            bool sign;
    };


}
#endif
