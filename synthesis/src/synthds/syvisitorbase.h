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

#ifndef _SYVISITORBASE_H_
#define _SYVISITORBASE_H_

#include <set>

#include "syutils.h"
#include "syutils2.h"
#include "sylist.h"

namespace Synth
{
    class SyModule;
    class SyBModule;
    class SyScPort;
    class SyVPort;
    class SyScNet;
    class SyVNet;
    class SyScReg;
    class SyVReg;
    class SyPrimInst;
    class SyModInst;
    class SyTask;
    class SyFunc;
    class SyUnary;
    class SyBinary;
    class SyCondExpr;
    class SyFuncCall;
    class SyBitSel;
    class SyPartSel;
    class SyConcat;
    class SyScPin;
    class SyVPin;
    class SyContAssign;
    class SyNonBlockAssign;
    class SyBlockAssign;
    class SyIf;
    class SyAlways;
    class SyEventCtrl;
    class SyCaseItem;
    class SyCase;
    class SyTaskOrFuncCall;
    class SyTaskCall;
    class SyConst;
    class SyEventStmt;
    class SyVString;
    class SyScope;
    class SyBlockCompAssign;
    class SyNonBlockCompAssign;
    class SyEventCtrlList;
    class SyXOrZ;
    class SyMemory;
    class SyAssign;

    class SyVisitorBase
    {
        public:
            virtual ~SyVisitorBase() {}
            inline void visitObjectModel();
            virtual void visit(const SyModule*) {}; 
            virtual void visit(const SyBModule*) {}; 
            virtual void visit(const SyScPort*) {}
            virtual void visit(const SyVPort*) {}
            virtual void visit(const SyScNet*) {}
            virtual void visit(const SyVNet*) {}
            virtual void visit(const SyScReg*) {}
            virtual void visit(const SyVReg*) {}
            virtual void visit(const SyMemory*) {}
            virtual void visit(const SyPrimInst*) {}
            virtual void visit(const SyModInst*) {}
            virtual void visit(const SyTask*) {}
            virtual void visit(const SyFunc*) {}
            virtual void visit(const SyUnary*) {}
            virtual void visit(const SyBinary*) {}
            virtual void visit(const SyCondExpr*){}
            virtual void visit(const SyFuncCall*){}
            virtual void visit(const SyBitSel*){}
            virtual void visit(const SyPartSel*){}
            virtual void visit(const SyConcat*){}
            virtual void visit(const SyScPin*) {}
            virtual void visit(const SyVPin*) {}
            virtual void visit(const SyContAssign*) {}
            virtual void visit(const SyNonBlockAssign*) {}
            virtual void visit(const SyBlockAssign*) {}
            virtual void visit(const SyIf*) {}
            virtual void visit(const SyAlways*) {}
            virtual void visit(const SyEventCtrl*) {}
            virtual void visit(const SyCaseItem*){}
            virtual void visit(const SyCase*) {}
            virtual void visit(const SyTaskOrFuncCall*){}
            virtual void visit(const SyTaskCall*) {}
            virtual void visit(const SyConst*) {}
            virtual void visit(const SyVString*) {}
            virtual void visit(const SyBlockCompAssign*) {}
            virtual void visit(const SyNonBlockCompAssign*) {}
            virtual void visit(const SyXOrZ*) {}

            virtual const std::string& getCurrModName() const = 0;
    };


    void
    SyVisitorBase::visitObjectModel() 
    {
        std::map<std::string, SyModule*, Synth::cmp_str> modules = 
            SyRoot::instance()->getSynthMods();
        std::map<std::string, SyModule*, Synth::cmp_str>::const_iterator mod = modules.begin();
        std::map<std::string, SyModule*, Synth::cmp_str>::const_iterator modEnd = modules.end();

        for(; mod != modEnd; ++mod) {
            mod->second->accept(this);
        }
    }

    template<class T1, class T2, class Visitor>
    void AcceptVectorVisitor( Visitor *v, const SyList<T1*>& scalars,
        const std::string& tfname = "")
    {
        std::set<std::string, cmp_str> doneObjs;
        for(SyNode<T1*>* node = scalars.head; node; node = node->next)
        {
            if(doneObjs.find(node->val->getIxName().getName()) !=
                        doneObjs.end()){
                continue;
            }
            doneObjs.insert(node->val->getIxName().getName());
            if(node->val->isVecBit())
            {
             //   const T2 *vec = dynamic_cast<const T2*>(
              //          node->val->getVecCntv(v->getCurrModName()));
                const T2 *vec = GetVecCntv<T1, T2>(
                    node->val->getIxName().getName(), v->getCurrModName(),
                    tfname);
                vec->accept(v);
            }
            else {
                node->val->accept(v);
            }
        }
    }

    template <class T, class Visitor>
    void AcceptListOfObjs(
            Visitor* v, 
            const SyList<T*>& objs)
    {
        for(SyNode<T*>* node = objs.head; node; node = node->next){
            node->val->accept(v);
        }
    }

    
}    

#endif
