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

#ifndef _STMT2GATES_H_
#define _STMT2GATES_H_
#include <stdexcept>

#include "expr2gates.h"
#include "syconst.h"
#include "case2gates.h"

namespace Synth
{
    class Always2Gates;

    struct InOutCntvs;

    struct OutCntvInfo 
    {
        typedef enum {UNSET, COMB, LATCH } OutType;

        OutCntvInfo(SyCntv*, SyCntv*, SyCntv*, OutType=UNSET);
        OutCntvInfo();
        ~OutCntvInfo();
        bool operator<(const OutCntvInfo&) const;
        SyCntv    *out;
        SyCntv    *rep; //representative for out
        SyCntv    *cond; //Condition on output
        OutType    type;
    };
    typedef std::set<OutCntvInfo> OutCntvInfoSet;
    typedef std::map<SyCntv*, SyCntv*> Out2Rep;

    typedef std::pair<InOutCntvs*, OutCntvInfoSet*> TaskFuncCallInfo;

    
    class Stmt2Gates: public ConVisitorBase
    {
        public:
            friend class Always2Gates;
            friend class FuncCall2Gates;
            Stmt2Gates(const SyModule*, const SyModule*, const SyScope*);
            virtual ~Stmt2Gates();

            //Expressions to be handled by expression
            //visitor
            virtual void visit(const SyUnary*);
            virtual void visit(const SyBinary*);
            virtual void visit(const SyVPort*);
            virtual void visit(const SyScPort*);
            virtual void visit(const SyVNet*);
            virtual void visit(const SyScNet*);
            virtual void visit(const SyScReg*);
            virtual void visit(const SyVReg*);
            virtual void visit(const SyConcat*);
            virtual void visit(const SyBitSel*);
            virtual void visit(const SyPartSel*);
            virtual void visit(const SyCondExpr*);
            virtual void visit(const SyConst*);
            virtual void visit(const SyXOrZ*);
            virtual void visit(const SyFuncCall*);
            virtual void visit(const SyIf*);
            virtual void visit(const SyCase*);
            virtual void visit(const SyContAssign*);
            virtual void visit(const SyBlockAssign*);
            virtual void visit(const SyNonBlockAssign*);
            //End expression visits

            //Statement visits
            virtual void visit(const SyAlways*);
            virtual void visit(const SyTaskCall*);

            virtual void setCurrMod(SyModule*);
            virtual void setCurrRtlMod(SyModule*);
            virtual void setCurrSynthMod(SyModule*);
            virtual void setCurrScope(SyScope*);

            TaskFuncCallInfo getTaskFuncCallInfo() const;
            //std::auto_ptr<SyList<SyCntv*> > getOutCntvs() {
            //    return outCntvs;
            //}    
            Out2Rep    getOut2Rep();
            OutCntvInfoSet*    getOutCntvInfoSet();

            void    setOut2Rep(const Out2Rep&);
            void    setOutCntvInfoSet(OutCntvInfoSet*);
            Expr2GatesRetType*    createCellInst(
                SyList<SyCntv*>*, SyList<SyCntv*>*,  short, short, CellType);
            void    mergeRep(const OutCntvInfoSet*, Out2Rep&) const;
            std::vector<SyCntv*>    getOutCntvs(const std::vector<OutCntvInfoSet*>&) const;
            void    removeOverriddenEntries(std::vector<OutCntvInfoSet*>&)const;
            OutCntvInfo    getMergedOutCntvInfo(const SyCntv*,
                const std::vector<OutCntvInfoSet*>&) const;
        private:
            DISABLE_COPYCONST_ASSGNOP(Stmt2Gates);

            OutCntvInfo        createMuxLogic(    const SyCntv*, const SyExpr*,
                const std::vector<OutCntvInfoSet*>&);
            OutCntvInfo        createCaseLogic(const SyCntv*, const SyExpr*,
                const std::vector<OutCntvInfoSet*>&);
            OutCntvInfo        createCaseLogic(const SyCntv*, /*const SyExpr*,*/
                const std::vector<OutCntvInfoSet*>&,
                const std::vector<SyCntv*>&);

            std::vector<SyCntv*> createLabelLogic(const SyCase*) const;    
            bool isLocal(const SyCntv*) const;


            void    addRep(SyCntv*, SyCntv*, Out2Rep&) const;    
            SyCntv*    getRep(SyCntv*, Out2Rep&) const;

            std::vector<bool>    checkIfMux(const std::vector<OutCntvInfoSet*>&) const;
            OutCntvInfoSet*    createGates(const SyList<SyStmt*>&, Out2Rep);
            OutCntvInfoSet* createBuf(const SyAssign*, Out2Rep);
            void createGates(SyList<SyCntv*>&, SyList<SyCntv*>&);
            void createGates(OutCntvInfoSet*, Out2Rep&);

            vector<OutCntvInfo> getOutCntvInfos(
                const SyCntv* cntv, 
                const vector<OutCntvInfoSet*>& info) const;
            
            bool checkSetReset(const SyCntv*, const SyIf*) const;
            bool transformAndVisitBlockAssign(const SyBlockAssign*);


            Expr2Gates        *expr2Gates;
            //std::auto_ptr<SyList<SyCntv*> > outCntvs;
            //Used to pass arguments
            Out2Rep    argOut2Rep;
            OutCntvInfoSet    *argOutCntvInfoSet;
            const SyAlways *always;

    };

}
#endif
