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

#ifndef _SYCONSTPROP_H_
#define _SYCONSTPROP_H_

#include <stack>

#include "syvisitorbase.h"
namespace Synth
{
    class SyFrameInfo;
    class SyConstProp : public SyVisitorBase
    {
        public:
            SyConstProp();
            virtual ~SyConstProp();

            virtual void visit(const SyScPort*); 
            virtual void visit(const SyVPort*);
            virtual void visit(const SyScNet*);
            virtual void visit(const SyVNet*);
            virtual void visit(const SyScReg*);
            virtual void visit(const SyVReg*);


            virtual void visit(const SyBModule*); 
            virtual void visit(const SyModule*); 
            virtual void visit(const SyFuncCall*);
            virtual void visit(const SyContAssign*);
            virtual void visit(const SyNonBlockAssign*);
            virtual void visit(const SyBlockAssign*);
            virtual void visit(const SyIf*);
            virtual void visit(const SyAlways*);
            //virtual void visit(const SyCaseItem*);
            virtual void visit(const SyCase*);
            virtual void visitTf(const SyTaskOrFunc*);
            virtual void visit(const SyTaskCall*);
            virtual void visit(const SyTask*);
            virtual void visit(const SyFunc*);
            virtual void visitAssign(const SyAssign*);
            virtual void visitTfCall(const SyTaskOrFuncCall*);
            virtual void visit(const SyUnary*);
            virtual void visit(const SyBinary*);
            virtual void visit(const SyCondExpr*);
            virtual void visit(const SyBitSel*);
            virtual void visit(const SyPartSel*);
            virtual void visit(const SyConcat*);
            virtual void visit(const SyConst*);
            virtual void visit(const SyMemory*);
            virtual void visit(const SyVString*);
            virtual void visit(const SyXOrZ*);

            virtual const std::string& getCurrModName() const;
        private:
            SyConstProp(const SyConstProp&);
            SyConstProp& operator=(const SyConstProp&);

            void popStackFrame();
            void pushStackFrame(bool);
            bool isReachable() const;
            void evaluateAndSetCurrExpr(const SyExpr*);

            template<typename T>void 
            copyToList(SyList<T*>&) const;

            template<typename T> void
            AcceptListOfStatements(const SyList<T*>&);

            SyBModule *module;
            const SyExpr *currexpr;
            SyList<SyStmt*>* currstmts;
            //const SyStmt *currstmt;
            SyFrameInfo* frameinfo;
    };

    class SyFrameInfo
    {
        public:
            //First boolean field indicates whether the statements
            //are reachable or not. This means they are under some
            //non-const condition
            typedef std::pair<bool, SyList<SyStmt*>* > SyFrameData;
            typedef std::stack<SyFrameData> SyActStack;
        public:
            SyFrameInfo(int);
            ~SyFrameInfo();

            void    push(const SyFrameData&);
            void    push(SyStmt*);
            void    pop();
            void    createAndPushFrame(bool);
            bool    empty() const;

            SyFrameData getFrameData();
            bool        isReachable() const;
            SyList<SyStmt*>* getStatements();

        private:
            SyFrameInfo(const SyFrameInfo&);
            SyFrameInfo& operator=(const SyFrameInfo&);

            void overflow() const;

            SyActStack    actstack;  //activation stack
            const int    maxsize;
    };
    class SyCheckAllBlocking : public SyVisitorBase
    {
        public:
            SyCheckAllBlocking();
            virtual ~SyCheckAllBlocking();

            bool allBlockAssign() const;

            virtual void visit(const SyNonBlockAssign*);
            virtual void visit(const SyIf*);
            virtual void visit(const SyAlways*);
            virtual void visit(const SyCaseItem*);
            virtual void visit(const SyCase*);
            virtual void visitTf(const SyTaskOrFunc*);
            virtual void visit(const SyFuncCall*);
            virtual void visit(const SyTaskCall*);
            virtual void visit(const SyTask*);
            virtual void visit(const SyFunc*);
            virtual void visitTfCall(const SyTaskOrFuncCall*);
            virtual const std::string& getCurrModName() const;
        private:
            SyCheckAllBlocking(const SyCheckAllBlocking&);
            SyCheckAllBlocking& operator=(const SyCheckAllBlocking&);
            void
            AcceptListOfStatements(const SyList<SyStmt*>&);

            bool allblocking;
            std::string modname; //always empty
    };        

}
#endif
