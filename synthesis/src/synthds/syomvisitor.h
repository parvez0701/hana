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

#ifndef _SYOMVISITOR_H_
#define _SYOMVISITOR_H_

#include "syvisitorbase.h"

namespace Synth
{
    //A general purpose object model visitor.
    class SyOMVisitor  : public SyVisitorBase
    {
        public:
            typedef enum {
                UNSET = 1,
                BEHAV_OPT = UNSET << 1,
                TF_COPY = BEHAV_OPT << 1,
                CONST_COMPACT = TF_COPY << 1
            } SynthContext;    
        public:
            SyOMVisitor();
            SyOMVisitor(SynthContext);
            virtual ~SyOMVisitor();

            void    visit();

            virtual void visit(const SyModule*);
            virtual void visit(const SyBModule*);
            virtual void visit(const SyScPort*);
            virtual void visit(const SyVPort*);
            virtual void visit(const SyScNet*);
            virtual void visit(const SyVNet*);
            virtual void visit(const SyScReg*);
            virtual void visit(const SyVReg*);
            virtual void visit(const SyMemory*);
            virtual void visit(const SyPrimInst*);
            virtual void visit(const SyModInst*);
            virtual void visit(const SyTask*);
            virtual void visit(const SyFunc*);
            virtual void visit(const SyUnary*); 
            virtual void visit(const SyBinary*);
            virtual void visit(const SyCondExpr*);
            virtual void visit(const SyFuncCall*);
            virtual void visit(const SyBitSel*);
            virtual void visit(const SyPartSel*);
            virtual void visit(const SyConcat*);
            virtual void visit(const SyScPin*); 
            virtual void visit(const SyVPin*); 
            virtual void visit(const SyContAssign*);
            virtual void visit(const SyNonBlockAssign*);
            virtual void visit(const SyBlockAssign*);
            virtual void visit(const SyIf*);
            virtual void visit(const SyAlways*);
            virtual void visit(const SyEventCtrl*);
            virtual void visit(const SyCaseItem*){}
            virtual void visit(const SyCase*);
            virtual void visit(const SyTaskOrFuncCall*){}
            virtual void visit(const SyTaskCall*);
            virtual void visit(const SyConst*);
            virtual void visit(const SyVString*);
            virtual void visit(const SyBlockCompAssign*);
            virtual void visit(const SyNonBlockCompAssign*);
            virtual void visit(const SyXOrZ*);

            virtual std::string& getCurrModName() const {
                assert(0);
            }

        private:
            //copy constructor and assignment operator not
            //implemented.
            SyOMVisitor(const SyOMVisitor&);
            SyOMVisitor& operator=(const SyOMVisitor&);
            //functions
            void setContext(SynthContext);
            //void resetContext(SynthContext);
            bool isContext(unsigned int) const;

            void resetExpr() {
                currExpr = 0;
            }    

            void checkTfCall(const SyTaskOrFuncCall*);
            void checkOutputPort(const SyScPort*, const SyExpr*, const std::string&);
            SyExpr* adjustActualInTfCall(SyExpr*, long);
            void checkPowerOperator(const SyBinary*);
            void convertPowerOperator(SyBinary*);

            void checkAndSetConstExpr(const SyExpr*);

            //bool isConstExpr(const SyExpr*) const;

            //data
            unsigned int context;
            SyExpr *currExpr;
            SyModule *currMod;
            bool status;
    };
}
#endif
