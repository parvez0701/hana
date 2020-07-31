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

#ifndef _OMVISITOR_H_
#define _OMVISITOR_H_
#include <fstream>
#include "omvisitorbase.h"

#include "dimension.h"
namespace Rtl
{
    typedef std::vector<TableEntry*> UDPTable;
    class OMVisitor : public OMVisitorBase
    {
        public:
            OMVisitor(const std::string&);
            virtual ~OMVisitor();
            virtual void visit(const Module*);
            virtual void visit(const VecPort*);
            virtual void visit(const VecNet*);
            virtual void visit(const VecPin*);
            virtual void visit(const VecParam*);
            virtual void visit(const VecLocalParam*);
            virtual void visit(const Port*);
            virtual void visit(const Net*);
            virtual void visit(const Pin*);
            virtual void visit(const Param*);
            virtual void visit(const LocalParam*);
            virtual void visit(const GateInst*);
            virtual void visit(const ModInst*);
            virtual void visit(const BinExpr*);
            virtual void visit(const UnaryExpr*);
            virtual void visit(const BitSel*);
            virtual void visit(const PartSel*);
            virtual void visit(const Concat*);
            virtual void visit(const CondExpr*);
            virtual void visit(const HierId*);
            virtual void visit(const IntConst*);
            virtual void visit(const Integer*);
            virtual void visit(const VecInt*);
            virtual void visit(const MultiConcat*);
            virtual void visit(const Num*);
            virtual void visit(const VString*);
            virtual void visit(const BasedNum*);
            virtual void visit(const Reg*);
            virtual void visit(const VecReg*);
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
            virtual void visit(const EventVar*);
            virtual void visit(const EventStmt*);
            virtual void visit(const DelayStmt*);
            virtual void visit(const Delay*);
            virtual void visit(const CompAssign*);
            virtual void visit(const ProcAssign*);
            virtual void visit(const ProcContAssign*);
            virtual void visit(const ContAssign*);
            virtual void visit(const Wait*);
            virtual void visit(const TaskFuncCall*);
            virtual void visit(const FuncCall*);
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
            void writeDesign();
        private:
            void printPackDim(const Dimension*);
            void printUnpackDim(const Dimension*);
            void printModHead(const Module*);
            void printScopeDecls(const Scope*);
            void printScopeVars(const Var*);
            void printUDPTable(const UDPTable*, const UDP*);
            void printGenerateDecls(const ModGenDecl*);

            void incInd() {indent += 4;}
            void decInd() {if(indent > 0) indent -= 4;}
            void printInd() { outfile << std::string(indent, ' '); }
            std::ofstream outfile;
            int indent;    
            short linewidth;
            bool useNameConn;
    };
}
#endif
