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

#ifndef _SEMANTICCHECKER_H_
#define _SEMANTICCHECKER_H_

#include "omvisitorbase.h"
namespace Rtl
{
    class Expr;
    class Assign;
    class SemanticChecker : public OMVisitorBase
    {
        public:
            typedef enum {
                MOD = 1,
                PORTSPEC = MOD << 1,
                CONTASSGNLEFT = PORTSPEC << 1,
                CONTASSGNRIGHT = CONTASSGNLEFT << 1,
                GATEINSTPIN =  CONTASSGNRIGHT<< 1,
                PROCASSIGNLEFT = GATEINSTPIN << 1,
                PROCASSIGNRIGHT = PROCASSIGNLEFT << 1,
                GENVAR = PROCASSIGNRIGHT << 1,
                MODINSTPIN = GENVAR << 1,
                MULTICONC = MODINSTPIN << 1,
                GENFORHEAD = MULTICONC << 1
            } ExprContext;    

            SemanticChecker();
            ~SemanticChecker();

            virtual void visit(const Module*);
            virtual void visit(const VecPort*);
            virtual void visit(const Port*);
            virtual void visit(const VecPin*);
            virtual void visit(const Pin*);
            virtual void visit(const VecNet*);
            virtual void visit(const Net*);
            virtual void visit(const Reg*);
            virtual void visit(const VecReg*);
            virtual void visit(const GateInst*);
            virtual void visit(const ModInst*);
            virtual void visit(const BinExpr*);
            virtual void visit(const UnaryExpr*);
            virtual void visit(const BitSel*);
            virtual void visit(const PartSel*);
            virtual void visit(const Concat*);
            virtual void visit(const MultiConcat*);
            virtual void visit(const CondExpr*);
            virtual void visit(const HierId*);
            virtual void visit(const IntConst*);
            virtual void visit(const Integer*);
            virtual void visit(const VecInt*);
            virtual void visit(const Num*);
            virtual void visit(const VString*);
            virtual void visit(const BasedNum*);
            virtual void visit(const Time*);
            virtual void visit(const VecTime*);
            virtual void visit(const Real*);
            virtual void visit(const VecReal*);
            virtual void visit(const RealTime*);
            virtual void visit(const VecRealTime*);
            virtual void visit(const Force*);
            virtual void visit(const Release*);
            virtual void visit(const NamedBlock*);
            virtual void visit(const Block*);
            virtual void visit(const SpecifyBlock*);
            virtual void visit(const Specparam*);
            virtual void visit(const SpecifyPath*);
            virtual void visit(const SpecifyEdgePath*);
            virtual void visit(const Fork*);
            virtual void visit(const Disable*);
            virtual void visit(const Trigger*);
            virtual void visit(const Forever*);
            virtual void visit(const Repeat*);
            virtual void visit(const For*);
            virtual void visit(const While*);
            virtual void visit(const CaseItem*);
            virtual void visit(const Case*);
            virtual void visit(const If*);
            virtual void visit(const EventCntrl*);
            virtual void visit(const EventStmt*);
            virtual void visit(const DelayStmt*);
            virtual void visit(const Delay*);
            virtual void visit(const CompAssign*);
            virtual void visit(const ProcAssign*);
            virtual void visit(const ProcContAssign*);
            virtual void visit(const ContAssign*);
            virtual void visit(const Wait*);
            virtual void visit(const TaskFuncCall*);
            virtual void visit(const Task*);
            virtual void visit(const Func*);
            virtual void visit(const Always*);
            virtual void visit(const Initial*);
            virtual void visit(const Defparam*);
            virtual void visit(const UDP*);
            virtual void visit(const UDPInst*);
            virtual void visit(const GenerateStmt*);
            virtual void visit(const GenerateCond*);
            virtual void visit(const GenerateCase*);
            virtual void visit(const GenerateLoop*);
            virtual void visit(const GenerateBlock*);
            virtual void visit(const GenDecl*);
            virtual void visit(const Genvar*);
            virtual void visit(const ImplicitNet*);

            void setContext(ExprContext);
            void resetContext(ExprContext);
            bool isContext(ExprContext) const;
            bool getStatus() const {
                return status;
            }    


        private:
            void printContAssignLhsError(const Expr*);
            void printProcAssignLhsError(const Expr*);
            void printGenvarError(const Genvar*);
            void printGenvarUnInitError(const Genvar*);
            void printGenerateForError(const Expr*);

            const Expr* checkImplicitNet(const Expr*);
            void checkPortConnections(const ModInst*) const;
            void checkPartSelect(const PartSel*);
            void checkBitSelect(const BitSel*);
            bool checkDimension(const BitSel*, bool ispartsel) const;

        
            unsigned int context;
            bool status;
            Assign    *assign;
         
    };
}  

#endif
