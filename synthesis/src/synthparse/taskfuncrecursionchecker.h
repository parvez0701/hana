/* 
Copyright (C) 2009-2011 Parvez Ahmad
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

#ifndef _TASKFUNCRECURSIONCHECKER_H_
#define _TASKFUNCRECURSIONCHECKER_H_

#include <stack>
#include <set>
#include <string>

#include "omvisitorbase.h"
#include "utils.h"

namespace Rtl
{
    class FuncCall;
    class Assign;
    class TaskOrFunc;
    class TaskFunctionRecursionChecker : public OMVisitorBase
    {
        typedef std::stack<std::set<std::string, compare_str> > NameStack;
        public:
            TaskFunctionRecursionChecker();
            virtual ~TaskFunctionRecursionChecker();
            bool getStatus() const;

            virtual void visit(const Module*);
            virtual void visit(const BinExpr*);
            virtual void visit(const UnaryExpr*);
            virtual void visit(const Concat*);
            virtual void visit(const CondExpr*);
            virtual void visit(const MultiConcat*);
            virtual void visit(const NamedBlock*);
            virtual void visit(const Block*);
            virtual void visit(const Repeat*);
            virtual void visit(const For*);
            virtual void visit(const CaseItem*);
            virtual void visit(const Case*);
            virtual void visit(const If*);
            virtual void visit(const ProcAssign*);
            virtual void visit(const ProcContAssign*);
            virtual void visit(const ContAssign*);
            virtual void visit(const TaskFuncCall*);
            virtual void visit(const FuncCall*);
            virtual void visit(const Task*);
            virtual void visit(const Func*);
            virtual void visit(const Always*);
        private:
            TaskFunctionRecursionChecker(const TaskFunctionRecursionChecker&);
            TaskFunctionRecursionChecker& operator=(const TaskFunctionRecursionChecker&);

            void    push(const std::string&);
            void    push();
            void    pushTop();
            void    pop();
            bool    isVisited(const std::string&) const;
            void    visitAssign(const Assign *);
            void    visitTf(const TaskOrFunc *);
            void    visitTfCall(const TaskFuncCall *);

            bool status;
            NameStack nameStack;
            const Module *module;
    };
}    
        

#endif
