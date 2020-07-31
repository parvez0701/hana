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

#ifndef _OMCREATOR_H_
#define _OMCREATOR_H_
#include <vector>
#include <string>
#include <stack>
#include "parseinfo.h"
#include "defines.h"
#include "modinst.h"
#include "blockdecl.h"
#include "module.h"
#include "veclocalparam.h"
#include "vecparam.h"
#include "num.h"

#include "msghandler.h"
namespace Rtl
{
    class Module;
    class UDP;
    class Param;
    class Base;
    class Scope;
    class Range;
    class File;
    class BasedNum;
    class Inst;
    class Delay;
    class NamedBlockBase;
    class Stmt;
    class Defparam;
    class SpecifyEdgePath;
    class SpecifyPathBase;
    class SpecRefEvent;
    class DelayVal;
    class TableEntry;
    class PartSel;
    class GenerateStmt;
    class VecVar;
    class ImplicitNet;
    class OMCreator 
    {
        public:
            static OMCreator* instance();
            void setCurrMod(Module* mod){
                currMod = mod;
            }
            void addParams(std::vector<Param*>*);
            void addLocalParams(std::vector<LocalParam*>*);
            Module* createModule(AttribTable*, const std::string&);
            UDP* createUDP(const std::string&);
            void incLine(const int dist=1){
                lineno += dist;
            }
            void setLine(const int val){
                lineno = val;
            }
            long getLine() const{
                return lineno;
            }

            const Base* findObj(const std::string&, const Scope* = 0) const;
            bool idExists(const std::string&, const Scope* = 0) const;

            const Range* getCurrRange() const{
                return currRange;
            }
            const GateType getCurrGateType() const {
                return currGateType;
            }
            template<class T, class VecT>
            std::vector<T*>* createParams(std::vector<ParseInfo*>*,
                int vartype=-1);
            std::vector<Param*>* createVecParams(std::vector<ParseInfo*>*);
            std::vector<LocalParam*>* createVecLocalParams(std::vector<ParseInfo*>*);

            void addNets(AttribTable*, const NetType, const bool, const Range*,
            std::vector<std::pair<char*, Expr*> >*);

            void addNets(AttribTable*, const NetType, const bool, const Range*,
            std::vector<std::pair<char*, std::vector<Range*>* > >*);

            void addPorts(const Dir, const NetType, const bool, const Range*,
            std::vector<std::pair<char*, std::vector<Range*>*> >*);

            void addTaskFuncPort(const std::string&, const Dir, 
                const Range*, std::vector<Range*>*,
                const bool=false, const int var=-1); 
            void addPort(const Dir,  const bool, const Range*,
                std::pair<char*, std::vector<Range*>* >&, std::string);
            void addV2kPorts();
            void checkPortDecl() const;

            void setParseInfo(std::vector<ParseInfo*>* vinfo){
                parseInfos = vinfo;
                //parseInfos.push_back(vinfo);
            }

            BasedNum* createBasedNum(std::string) const;

        //    void popInfo(){
        //        parseInfos.pop();
        //    }

            void setCurrFile(const File* file){
                currFile = const_cast<File*>(file);
            }
            const File* getCurrFile() const {
                return currFile;
            }

            Expr* createIdUse(std::string, const bool=false) const;

            Inst* createInst(const std::string&, 
                const std::vector<NameConn*>*) const;

            ModInst* createModInst(const std::string&, 
                const std::vector<NameConn*>*) const;

            UDPInst* createUDPInst(const std::string&, 
                const std::vector<NameConn*>*) const;
            void setCurrGateType(const GateType t){
                currGateType = t;
            }

            void setCurrMasterName(const std::string& n){
                currInstMaster = n;
            }
            int setMasterAndPins(ModInst*,
                Module*, const std::vector<NameConn*>* conns) const;

            void setInstParams(ModInst* inst,
                const std::vector<NameConn*>& nv);

            void setDelayAndDrStr(std::vector<Inst*>*, Delay*, DriveSt*) const;

            void createContAssigns(const DriveSt*, const Delay*,
                std::vector<std::pair<Expr*, Expr*>* >*);

            BlockDecl* getCurrBlkDecl() const {
                return currBlkDecl;
            }
            const Scope* getCurrScope() const{
                return currScope;
            }
            const Module* getCurrModule() const {
                return currMod;
            }    
            const UDP* getCurrUdp() const {
                return currUDP;
            }    

            void createBlockDecls(AttribTable*, std::vector<RegInfo*>*, Range*, bool, std::string);
            void createBlockDecls(AttribTable*, std::vector<std::pair<char*, 
                std::vector<Range*>*> >*, std::string);
            void setCurrBlockDecl(BlockDecl* bd){
                currBlkDecl = bd;
            }

            void addDefparams(std::vector<Defparam*>*);

            void addNamedBlock(NamedBlockBase*);
            void createTask(const std::string&, const bool);
            void createFunc(const std::string&, const bool, const bool,
            std::pair<int, Range*>*);
            void createAlwaysBlock(AttribTable*, const Stmt*);
            void createInitialBlock(AttribTable*, const Stmt*);
            void createEvents(std::vector<std::pair<char*, 
                std::vector<Range*>*> >*);
            void incErrCount(){
                ++errcount;
            }
            int getErrCount() const{
                return errcount;
            }

            SpecifyEdgePath* createSpecifyEdgePath(EdgeType, std::vector<Expr*>*,
                char, SpecConnType, const std::string& = "", Expr* = NULL,
                PolOp=NOPOOP, Expr* = NULL);

            void addSpecparamDecl(const std::vector<Specparam*>*);

            void createSpecifyBlock();
            void addSpecPath(const SpecifyPathBase*);
            void createSpecTask(const std::string&, const SpecRefEvent*,
                const SpecRefEvent*, const DelayVal*, const DelayVal*,
                const std::vector<Expr*>&);

            void scopeEndAction();

            void createUDPPorts(const std::vector<ParseInfo*>*,
                const std::vector<char*>*);
            void createUDPPorts(const std::string&, const Expr*,
                const std::vector<char*>*, const bool);
            void addTableEntry(const TableEntry*);
            int setUDPInstPins(UDPInst*, const std::vector<NameConn*>*) const;
            void setUDPInitVal(const Expr*);
            Expr* createBitSelect(const std::string&, 
                const std::vector<Expr*>*)const;

            PartSel* createPartSelect(const std::string&, 
                const Range*, const std::vector<Expr*>*, 
                PolOp) const;
            int parseSetup(const std::string& inclfile="");
            void addStmt(const Stmt* st);
            void setTaskOrFuncStmt(const Stmt* st);
            void setCurrAttribTable(AttribTable* t) {
                currTable = t;
            }    
            AttribTable* getCurrAttribTable() const{
                return currTable;
            }
            void createGenStmt();
            GenerateStmt* getCurrGenStmt() const {
                return currGenStmt;
            }

            void setCurrGenStmt(GenerateStmt* gs){
                currGenStmt = gs;
            }

            GenerateCase* createGenCase(const Expr*);
            GenerateCond* createGenCond(const Expr*);
            GenerateLoop* createGenLoop(const Expr*, const Expr*, const Expr*,
                const Expr*);
            GenerateBlock* createGenBlock(const std::string& ="");
            Genvar* createGenvar(const std::string&);
            void setGenerateItems(const std::vector<GenerateItem*>*);
            void removeGenItem(const bool onlydecl=false);
            GenerateItem* getCurrGenItem() {
                if(genItems.empty()){
                    return NULL;
                }
                return genItems.top();
            }
            ModGenDecl* getCurrGenDecl() const;
            bool handlePortRegNet(
                    const AttribTable*, 
                    const NetType, 
                    const bool,
                    const Range*,
                    const std::vector<Range*>*,
                    std::pair<char*, Expr*>&
                    );

            Range* checkIfPartSelect(
                    const Expr*,
                    const std::vector<Expr*>*) const;
            void setDefNetType(NetType t) {
                defNetType = t;
            }
            NetType getDefNetType() const {
                return defNetType;
            }

            Range* createRange(const Expr*, const Expr*);
        private:
            OMCreator();
            ~OMCreator();
            OMCreator(const OMCreator&);
            const OMCreator& operator=(const OMCreator&);
            static OMCreator* omcreator;
            //---------------------------------------------------------
            // Different syntax/semantic checks.
            // --------------------------------------------------------
            bool checkPortExists(const std::string&) const;

            bool checkDupPort(std::vector<std::pair<char*, Expr*> >*) const;
            //bool checkDupPort(const std::string&, 
            //    const std::vector<ParseInfo*>*) const;
            std::string intToVarString(const int) const;
            void checkAndAddImplicitNet(const std::vector<NameConn*>*) const;
            Net* checkAndAddImplicitNet(const VString*) const;
            void addVecVarToModGenDecl(const VecVar*);
            File* createIncludeFile(const std::string&);
            const Expr* replaceVStringInParamExpr(const Expr*) const;

            Module* currMod;
            Scope* currScope;
            Range* currRange;
            File* currFile;
            long lineno;
            int errcount;
            mutable GateType currGateType;
            BlockDecl* currBlkDecl;
            SpecifyBlock* currSpecifyBlock;
            UDP* currUDP;
            std::vector<ParseInfo*>* parseInfos;
            AttribTable* currTable;
            GenerateStmt* currGenStmt;
            NetType defNetType;
            NamedBlockBase* currNamedBlock;
            std::string currInstMaster;
            GenerateItem* currGenItem;
            //ModGenDecl* currGenDecl;
            mutable std::stack<GenerateItem*> genItems;
            std::stack<ParseState> parseStates;
            std::stack<File*> filesToBeParsed;
    };

    template <class T, class VecT> std::vector<T*>*
    OMCreator::createParams(
        std::vector<ParseInfo*>* parseInfos,
        int vartype)
    {
        if(MsgHandler::instance()->getMsgCount(MsgInfo::ERROR) > 0 ) {
            return 0;
        }    
        std::vector<T*>* params = new std::vector<T*>();
        const int size = parseInfos->size();

        for(int i = 0; i < size; ++i)
        {
            if(this->findObj((*parseInfos)[i]->name, currScope)){
                MsgHandler::instance()->print(21, lineno, (*parseInfos)[i]->name.c_str());
                continue;
            }
            T* param;
            if(!(*parseInfos)[i]->exprs.empty())
            {
                const Expr *pval = replaceVStringInParamExpr((*parseInfos)[i]->exprs[0]);
                if(!pval) {
                    continue;
                }    
                int ival = pval->len();
                if(ival > 1) 
                {
                    Range *range = new Range(new Num(ival-1), new Num(0));
                    Dimension* dim = new Dimension();
                    dim->packDim.push_back(range);
                    VecT* vT = new VecT((*parseInfos)[i]->name, dim, pval->isSigned());
                    std::vector<T*> vecT = vT->getParams();
                    std::copy(vecT.begin(), vecT.end(), std::back_inserter(*params));
                    currScope->addObject((*parseInfos)[i]->name, vT, true);
                    vT->setVal(pval);

                    
                }
                else
                {
                    param = new T((*parseInfos)[i]->name, pval, intToVarString(vartype));
                    currScope->addObject((*parseInfos)[i]->name, param, true);
                    params->push_back(param);
                }
            }
            else {
                param = new T((*parseInfos)[i]->name,
                NULL, intToVarString(vartype));
                currScope->addObject((*parseInfos)[i]->name, param, true);
                params->push_back(param);
            }
        }
        delete parseInfos;
        return params;
    }

}
#endif
