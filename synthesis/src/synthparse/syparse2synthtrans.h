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

#ifndef _SYPARSE2SYNTHTRANS_H_
#define _SYPARSE2SYNTHTRANS_H_
#include "syparse2synth.h"
#include "sycontainer.h"
namespace Synth
{
    class SyBase;
    class SyRoot;
    class SyIf;
    class SyCase;
    class SyEventCtrl;
    class SyEventStmt;
    class SyScope;
    class SyInst;
    class SyPrimInst;
    class SyCompAssign;
    class SyTaskCall;
    class SyCaseItem;
    class SyParse2SynthTrans
    {
        friend class SyTfCallCopier;
        private:
            friend class SyParse2Synth;
            SyParse2SynthTrans():
                rtlRoot(0), synthError(false), 
                maxLoopCount( 1 << 14){
                }

            void translate(
                    const Root*, 
                    const std::set<std::string, cmp_str>&
                    ) const;
            SyModule* translate(const Module*) const;
            void translate(
                    SyScope*,
                    const std::vector<Port*>&,
                    SyBModule* = 0,
                    bool=false) const;

            void translate(
                    SyBModule*,
                    const std::vector<Net*>&) const;

            void translate(
                    SyBModule*,
                    const std::vector<Reg*>&,
                    const bool useNewName=false) const;

            void translate(
                    SyBModule*,
                    const std::vector<Integer*>&,
                    const bool useNewName=false) const;
            void translate(
                    SyBModule*,
                    const Integer*,
                    const bool useNewName=false) const;

            const SyExpr *translate(
                    const SyBModule*,
                    const Expr*) const;
            const SyExpr *translateInteger(
                    const SyBModule*,
                    const Expr*) const;

            void translate(
                    SyBModule*,
                    SyInst*,
                    const ModInst*,
                    const std::vector<Pin*>&)const;

            const SyExpr *translateParam(const SyBModule*, const Expr*) const;

            const SyExpr *translateBitSel(
                    const SyBModule*,
                    const BitSel*) const;
            const SyExpr *translatePartSel(
                    const SyBModule*,
                    const PartSel*) const;
            const SyExpr *translateConcat(
                    const SyBModule*,
                    const Concat*) const;

            const SyStmt *translate(
                    SyBModule*,
                    const Stmt*) const;
            const SyContAssign *translateContAssign(
                    SyBModule*,
                    const ContAssign*) const;
            const SyStmt *translateProcAssign(
                    SyBModule*,
                    const ProcAssign*) const;

            const SyAlways *translateAlways(
                    SyBModule*,
                    const Always*) const;
            const SyEventCtrl *translateEventCtrl(
                    SyBModule*,
                    const EventCntrl*) const;
            const SyEventStmt *translateAlwaysOrEventCtrl(
                    SyBModule*,
                    const Stmt*) const;

            void 
                translateInnerScopeVars(
                    SyBModule*,
                    const std::vector<Reg*>&) const;
            void translateInnerScopeVars(
                    SyBModule*,
                    const std::vector<Integer*>&) const;
            std::string createUniqueName(
                    const SyBModule*,
                    const std::string&) const;

            SyList<SyStmt*>* translateBlock(
                    SyBModule*,
                    const Block*) const;
            SyList<SyStmt*>* translateNamedBlock(
                    SyBModule*,
                    const NamedBlock*) const;
            SyList<SyStmt*>* translateBlockOrStmt(
                    SyBModule*,
                    const Stmt*) const;
            SyList<SyStmt*>* translateFor(
                    SyBModule*,
                    const For*) const;
            SyList<SyStmt*>* translateRepeat(
                    SyBModule*,
                    const Repeat*) const;

            const SyIf* translateIf(
                    SyBModule*,
                    const If*) const;

            SyList<SyStmt*>* translateCase(
                    SyBModule*,
                    const Case*) const;
            SyList<SyStmt*>* transformCaseItems(SyCase*, int) const;        
            void addMissingCaseLabels(SyCase*) const;
            SyList<SyCaseItem*>* getNewCaseItems(
                    const SyCaseItem*,
                    int,
                    int,
                    std::set<long>&) const;
            void sortCaseItems(SyList<SyCaseItem*>&, int) const;        

            const SyFunc *translateFunction(
                    SyBModule*,
                    const Func*) const;
            const SyTask *translateTask(
                    SyBModule*,
                    const Task*) const;

            const SyExpr *translateFuncCall(
                    const SyBModule*,
                    const FuncCall*) const;

            const SyTaskCall *translateTaskCall(
                    SyBModule*,
                    const TaskFuncCall*) const;

            const SyPrimInst* translatePrimInst(
                    SyBModule*,
                    const GateInst*) const;
            const SyModInst* translateModInst(
                    SyBModule*,
                    const ModInst*) const;

            const SyCompAssign* translateCompAssign(
                    SyBModule*,
                    const CompAssign*)const;
            
            const SyExpr* translateXOrZ(const BasedNum*)  const;
            const SyExpr* translateHierId(const SyBModule*, const HierId*) const;

            bool isMemory(const VecVar*) const;

            void createMemory(
                    SyBModule*,
                    const VecVar*,
                    const bool useNewName=false) const;
            
            void resolveModInsts() const;
            void checkSensList(const SyAlways*, const Always*) const;

            template <class T1, class T2, class T3, class T4>
            const SyExpr* translate(
                    const SyBModule*,
                    const Expr*) const;

            void addNewName(const std::string& old, const std::string& new_
                    ) const
            {
                if(table.empty()){
                    return;
                }
                table.top().insert(std::make_pair(old, new_));
            }
/*
            std::string getNewName(const std::string& old) const
            {
                if(table.empty()){
                    return old;
                }
                OldNewNameTable::const_iterator iter = table.top().find(old);
                if(iter == table.top().end()){
                    return old;
                }
                return iter->second;
            }
*/            
            std::string getNewName(const std::string& old) const
            {
                //expensive function
                std::string str = old;
                std::stack<OldNewNameTable> ttable;
                
                while(!table.empty())
                {
                    OldNewNameTable::const_iterator iter = table.top().find(old);
                    if(iter != table.top().end())
                    {
                        str = iter->second; 
                        break;
                    }
                    ttable.push(table.top());
                    table.pop();
                }    
                while(!ttable.empty()) 
                {
                    //restore the stack
                    table.push(ttable.top());
                    ttable.pop();
                }
                return str;
            }
            std::pair<Expr*, Expr*> getRangeForPartSelect(const PartSel*) const;

            const SyBase *findObject(
                    const SyModule *mod,
                    const std::string& name) const {
                const SyBase *base = 0;
                if(!tfname.empty()){
                    //for function task ports, since they are not moved
                    //to module level
                    const SyTaskOrFunc *tf = dynamic_cast<const SyTaskOrFunc*>(
                            mod->findObject(tfname));
                    base = tf->findObject(name);
                }
                if(!base){
                    base = mod->findObject(this->getNewName(name));
                }
                return base;
            }

            SyConst* createConst(const BasedNum*) const;
            SyConst* createConst(const Num*) const; 


            //general purpose map used across functions
            typedef std::map<std::string, std::string, cmp_str> 
                OldNewNameTable;
            //Every new scope created by named block and function
            //wil have a map enitity pushed inside the stack.
            //We are creating a unique name for each variable declared
            //inside innerscope. There is no concept of inner scope
            // in synthesis data structures, so all the names have to
            //be uniquified.
            mutable std::stack<OldNewNameTable> table;
            Root *rtlRoot;
            mutable bool synthError;
            mutable std::string tfname;
            const int maxLoopCount;
    };

    template<class T1, class T2, class T3, class T4>
    const SyExpr *SyParse2SynthTrans::translate(
            const SyBModule *mod,
            const Expr * expr) const
    {
        if(const T1 * t1 = dynamic_cast<const T1*>(expr))
        {
            if(t1->isVecBit())
            {
                const T4 *t4 = dynamic_cast<const T4*>(
                        this->findObject(mod, t1->getIxName().getName()));
                return t4->getBit(t1->getIxName().getIndex());
            }
            else {
                return dynamic_cast<const T3*>(
                        this->findObject(mod, t1->getIxName().getName()));
            }
        }
        else if(const T2 *t2 = dynamic_cast<const T2*>(expr)) {
            return dynamic_cast<const T4*>(
                    this->findObject(mod, t2->getName()));
        }
        return 0; 
    }

    SyPrimInst::SyGateType RtlToSynthGateType(GateType);
}
#endif
