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

#ifndef _EXPR2GATES_H_
#define _EXPR2GATES_H_
#include <memory>

#include "sycontainer.h"
#include "convisitorbase.h"
#include "cellutil.h"

namespace Synth
{
    class Expr2GatesRetType    
    {
        public:
            Expr2GatesRetType() 
            {
                inputs = new SyList<SyCntv*>();
                outputs = new SyList<SyCntv*>();
                gateInsts = 0;
            }    
            ~Expr2GatesRetType() 
            {
                delete inputs;
                delete outputs;
                delete gateInsts;
            }

            DISABLE_COPYCONST_ASSGNOP(Expr2GatesRetType);
            SyList<SyCntv*>*    inputs;
            SyList<SyCntv*>*    outputs;
            SyList<SyModInst*>*    gateInsts;
    } ;    
         

    class Expr2Gates : public ConVisitorBase
    {
        public:
            Expr2Gates(const SyModule*, const SyModule*, const SyScope*);
            virtual ~Expr2Gates();

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

            std::auto_ptr<Expr2GatesRetType> getResult() {
                return result;
            }    
            Expr2GatesRetType*    createCellInst(
                SyList<SyCntv*>*, SyList<SyCntv*>*,  short, short, CellType);
            Expr2GatesRetType*    create2InputGateTree(
                                            SyList<SyCntv*>*,
                                            CellType);
            Expr2GatesRetType*    createMuxForDimension(    const SyBitSel*,
                                                        int    dim) const;
            void                createDecoderLogicForAssign(
                                            const SyBitSel*,
                                            const SyExpr*);

            virtual void setCurrMod(SyModule *mod); 
            virtual void setCurrSynthMod(SyModule *mod); 
            virtual void setCurrRtlMod(SyModule *mod);  
            virtual void setCurrScope(SyScope *sc);                                    
            SyCntv*            createVccOrGround(bool=true);
        private:
            Expr2GatesRetType* convert(const SyUnary*);
            Expr2GatesRetType* convert(const SyBinary*);
            Expr2GatesRetType* convert(const SyVPort*);
            Expr2GatesRetType* convert(const SyScPort*);
            Expr2GatesRetType* convert(const SyVNet*);
            Expr2GatesRetType* convert(const SyScNet*);
            Expr2GatesRetType* convert(const SyScReg*);
            Expr2GatesRetType* convert(const SyVReg*);
            Expr2GatesRetType* convert(const SyConcat*);
//            Expr2GatesRetType* convert(const SyConst*);
            //Expr2GatesRetType* convert(const SyExpr*) {}
            DISABLE_COPYCONST_ASSGNOP(Expr2Gates);
            Expr2GatesRetType*    getReturnElement() const;
            //Unary exprs
            Expr2GatesRetType*     convertUnaryPlus(const SyUnary*);
            Expr2GatesRetType*     convertUnaryMinus(const SyUnary*);
            Expr2GatesRetType*     convertLogicNegation(const SyUnary*);
            Expr2GatesRetType*     convertBitwiseNegation(const SyUnary*);
            Expr2GatesRetType*    convertLogicalNegation(const SyUnary*);
            //Expr2GatesRetType* convertReductionAnd(const SyUnary*);
            Expr2GatesRetType* convertReductionOperator(const SyUnary*, CellType);
            //End unary exprs

            //Binary exprs

            Expr2GatesRetType*    convertLogicalAndOr(const SyBinary*);
            Expr2GatesRetType*    convertArithOps(const SyBinary*);

            Expr2GatesRetType*    createLogicForLogEq(Expr2GatesRetType*);
            Expr2GatesRetType*    createLogicForLogIneq(Expr2GatesRetType*);
            Expr2GatesRetType*    createLogicForLogGThan(Expr2GatesRetType*);
            Expr2GatesRetType*    createLogicForLogLThan(Expr2GatesRetType*);
            Expr2GatesRetType*    createLogicForLogGTE(Expr2GatesRetType*);
            Expr2GatesRetType*    createLogicForLogLTE(Expr2GatesRetType*);
            
            Expr2GatesRetType*    createLogicForShiftOperator(const SyBinary*);
            Expr2GatesRetType*    createLogicForConstShift(
                                    Expr2GatesRetType*,
                                    bool,
                                    short,
                                    short);
            Expr2GatesRetType*    createLogicForVariableShift(
                                    Expr2GatesRetType*,
                                    bool,
                                    const SyExpr*,
                                    short);
            Expr2GatesRetType*    createLogicForAddSubMulDivModOperators(const SyBinary*);
            Expr2GatesRetType*    createLogicForBitAndOrXorXnorNandNorOperators(const SyBinary*);
            bool                createTribufs(const SyCondExpr*);
            bool                createMuxes(const SyCondExpr*);
            Expr2GatesRetType*    createMux(const SyBitSel*);
            Expr2GatesRetType*    createDecoder(const SyBitSel*);
                                            
            template <class T>
            Expr2GatesRetType*    createRetElemForVecCntvs(const T*) const;
            template<class T>
            Expr2GatesRetType*    createRetElemForScCntvs(const T*) const;


            const SyCntv*                getNextPort(
                                            const SyModule*, 
                                            SyNode<SyScPort*>*&, 
                                            SyScPort::SyDirType
                                        ) const;
            std::auto_ptr<Expr2GatesRetType> result;
            
    };

    template <class T>
    Expr2GatesRetType* Expr2Gates::createRetElemForVecCntvs(const T* rtlCntv) const 
    {
        const T* synthCntv = dynamic_cast<const T*>(this->getCurrSynthMod()->
                findObject(rtlCntv->getName()));
        assert(synthCntv);    
            
        Expr2GatesRetType *retElement = this->getReturnElement();
        SyList<SyScCntv*> tmp1 = synthCntv->getBits();
        SyList<SyCntv*> tmp2 = ConvertList<SyScCntv*, SyCntv*>(tmp1);
        
        *(retElement->inputs) = tmp2; 
        *(retElement->outputs) = tmp2; 
        return retElement;
    }    

    template <class T>
    Expr2GatesRetType* Expr2Gates::createRetElemForScCntvs(const T *rtlCntv) const
    {
        const T *synthCntv = 0;
        if(rtlCntv->isVecBit())
        {
            const SyVCntv *vcntv = dynamic_cast<const SyVCntv*>(this->getCurrSynthMod()->findObject(
                rtlCntv->getIxName().getName()));
            synthCntv = dynamic_cast<const T*>(vcntv->getBit(
                rtlCntv->getIxName().getIndex()));    
            
        }
        else 
        {
            synthCntv = dynamic_cast<const T*>(this->getCurrSynthMod()->findObject(SyIxName(rtlCntv->toString())));
        }    
        assert(synthCntv);    
            
        Expr2GatesRetType *retElement = this->getReturnElement();
        retElement->inputs->push_back(const_cast<T*>(synthCntv));
        retElement->outputs->push_back(const_cast<T*>(synthCntv));
        return retElement;
    }    


//----------------------------------------------------------------------
    class OutCntvInfo;
    typedef std::set<OutCntvInfo> OutCntvInfoSet;
    typedef std::map<SyCntv*, SyCntv*> Out2Rep;

    class StmtsConverter
    {
        public:
            typedef std::map<SyCntv*, SyExpr*> FormalActualMap;
        public:
            StmtsConverter(const SyList<SyScCntv*>&,
                const SyList<SyCntv*>&,
                const SyList<SyStmt*>&);
            ~StmtsConverter();

            SyList<SyStmt*> convert();
            SyList<SyStmt*> operator()();
        private:
            DISABLE_COPYCONST_ASSGNOP(StmtsConverter);

            //functions
            SyExpr*    getActual(SyCntv*) const;
            void    addActual(SyCntv*, SyExpr*);

            void    createFormalActualMap();
            

            //data
            SyList<SyScCntv*>    formals;
            SyList<SyCntv*> actuals;
            SyList<SyStmt*> stmts;

            FormalActualMap formActMap;
    };

    class FuncCall2Gates
    {
        public:
            //static FuncCall2Gates* instance();
            FuncCall2Gates();
            ~FuncCall2Gates();
            Expr2GatesRetType*    funcCall2Gates(
                SyModule*, SyModule*, SyScope*, const SyFuncCall*);
                
            void clear();    
            //bool isCallActive() const;

            Out2Rep getOut2Rep();
            OutCntvInfoSet* getOutCntvInfoSet();

        private:
            DISABLE_COPYCONST_ASSGNOP(FuncCall2Gates);

            SyList<SyStmt*> createStmts(SyModule*, SyModule*, const SyFuncCall*);

            //static FuncCall2Gates* fCall2Gates;
            //SyFuncCall    *funcCall;
            //SyModule *rtlMod;
            //SyModule *synthMod;

            //keeps return val info
            OutCntvInfoSet    *retOutCntvSet;
            Out2Rep            retOut2Rep;
    };

    class FuncCallMerge
    {
        public:
            struct FuncCallInfo {
                Out2Rep    out2Rep;
                OutCntvInfoSet    *outCntvInfoSet;
                SyModule    *rmod;
                SyModule    *smod;
//                SyFunc        *func;
            };    
            typedef SyList<FuncCallInfo> FuncCallList;
            typedef std::map<SyScope*, FuncCallList> FuncCallInfoTable;
            static FuncCallMerge* instance();
            ~FuncCallMerge();

            void    addInfo(SyScope*, FuncCallInfo&);
            void    clear();
            OutCntvInfoSet* merge(SyScope*);
        private:
            FuncCallMerge();
            DISABLE_COPYCONST_ASSGNOP(FuncCallMerge);

            static FuncCallMerge    *funcCallMerge;
            FuncCallInfoTable funcCallInfoTable;
    };

}
#endif

