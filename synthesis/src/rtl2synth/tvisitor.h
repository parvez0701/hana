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

#ifndef _TVISITOR_H_
#define _TVISITOR_H_

namespace Synth
{
    //T for translation
    class SyModule;
    class SyBModule;
    class SyScPort;
    class SyVPort;
    class SyScNet;
    class SyVNet;
    class SyScReg;
    class SyVReg;
    class SyBitSel;
    class SyPartSel;
    class SyConcat;
    class SyUnary;
    class SyBinary;
    class SyAlways;
    class SyEventStmt;
    class SyIf;
    class SyCase;
    class SyScPin;
    class SyVPin;
    class SyCondExpr;
    class SyTask;
    class SyFunc;
    class SyTaskCall;
    class SyFuncCall;
    class SyPrimInst;
    class SyModInst;
    class SyConst;
    class SyContAssign;
    class SyBlockAssign;
    class SyNonBlockAssign;

    class TVisitor
    {
        public:
            virtual void visit(SyModule*, SyModule*);
            virtual void visit(SyBModule*, SyModule*);
            virtual void visit(SyScPort*, SyModule*);
            virtual void visit(SyVPort*, SyModule*);
            virtual void visit(SyScNet*, SyModule*);
            virtual void visit(SyVNet*, SyModule*);
            virtual void visit(SyScReg*, SyModule*);
            virtual void visit(SyVReg*, SyModule*);
            virtual void visit(SyBitSel*, SyModule*);
            virtual void visit(SyPartSel*, SyModule*);
            virtual void visit(SyConcat*, SyModule*);
            virtual void visit(SyUnary*, SyModule*);
            virtual void visit(SyBinary*, SyModule*);
            virtual void visit(SyAlways*, SyModule*);
            virtual void visit(SyEventStmt*, SyModule*);
            virtual void visit(SyIf*, SyModule*);
            virtual void visit(SyCase*, SyModule*);
            virtual void visit(SyScPin*, SyModule*);
            virtual void visit(SyVPin*, SyModule*);
            virtual void visit(SyCondExpr*, SyModule*);
            virtual void visit(SyFuncCall*, SyModule*);
            virtual void visit(SyTaskCall*, SyModule*);
            virtual void visit(SyFunc*, SyModule*);
            virtual void visit(SyTask*, SyModule*);
            virtual void visit(SyPrimInst*, SyModule*);
            virtual void visit(SyModInst*, SyModule*);
            virtual void visit(SyConst*, SyModule*);
            virtual void visit(SyContAssign*, SyModule*);
            virtual void visit(SyBlockAssign*, SyModule*);
            virtual void visit(SyNonBlockAssign*, SyModule*);
    };
}
#endif
