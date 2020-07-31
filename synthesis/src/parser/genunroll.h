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

#ifndef _GENUNROLL_H_
#define _GENUNROLL_H_
#include "generate.h"
#include "utils.h"
namespace Rtl
{
    class Genvar;
    class GenerateStmt;
    class GenerateCond;
    class GenerateItem;
    class GenerateCase;
    class GenerateBlock;
    class GenerateLoop;
    class GenDecl;
    class ModGenDecl;
    
    class BasedNum;
    class Always;
    class ContAssign;
    class Stmt;
    class BlockNameMgr
    {
        public:
            typedef std::map<std::string, std::string, compare_str> NewOldNameMap;
            typedef std::map<std::string, Genvar*, compare_str> GenvarMap;
            typedef std::map<std::string, Expr*, compare_str> LoopvarMap;
        //    typedef std::pair<std::string, NewOldNameMap> GenBlockInfo;
            struct GenBlockInfo
            {
                std::string    blockName;
                GenvarMap genvars;
                NewOldNameMap newOldNameMap;
                LoopvarMap loopvars;
                Expr *loopvar;
            };
            typedef std::stack<GenBlockInfo> NameTableStack;
        public:
            BlockNameMgr(const Module*);
            ~BlockNameMgr();

            std::string getOldName(const std::string&) const;
            std::string createNewName(const std::string&) const;
            void addName(const std::string&, const std::string&);
            const Base *findObject(const std::string&) const;
            void diveInsideNewScope(const GenerateItem*);
            void comeOutOfScope();
            
            void addGenvar(const Genvar*);
            Genvar* findGenvar(const std::string&) const;
            const GenvarMap& getGenvars() const;
            //current loops loop var
            Expr* getLoopVar() const;
            void setLoopVar(Expr *);
            Expr* getLoopVarVal() const;

            Expr* getLoopVar(const std::string&) const;
            const LoopvarMap& getLoopVars() const;
            const NewOldNameMap& getNameMap() const;
            Expr* getLoopVarVal(const Expr*) const;
            void  addLoopVar(const Expr*);
            void removeFromNewOldNameMap(const std::string&);
        private:
            BlockNameMgr();
            BlockNameMgr(const BlockNameMgr&);
            BlockNameMgr& operator=(const BlockNameMgr&);

            std::string createGenBlockName(const GenerateItem*) const;
            const std::string& getGenBlockName() const;
            NameTableStack nameTableStack;
            Module *module;
    };

    class GenUnroll 
    {
        public:
            typedef std::map<std::string, Genvar*, compare_str>    Genvars;
        public:
            GenUnroll(const Module*);
            ~GenUnroll();

            
            bool getStatus() const {
                return status;
            }
            bool unroll();
            bool operator()();
        private:
            GenUnroll();
            GenUnroll(const GenUnroll&);
            GenUnroll& operator=(const GenUnroll&);
            
            BasedNum *getGenvarValue(const Genvar*) const;
            bool init();
            bool unroll(const GenerateItem*);
            bool unroll(const GenerateStmt*);
            bool unroll(const GenerateCond*);
            bool unroll(const GenerateCase*);
            bool unroll(const GenerateBlock*);
            bool unroll(const GenerateLoop*);
            bool unroll(const GenDecl*);
            bool unroll(const ModGenDecl*);

            void createNets(const ModGenDecl*);
            void createVars(const ModGenDecl*);
            void createGateInsts(const ModGenDecl*);
            void createModInsts(const ModGenDecl*);
            void createGenvars(const ModGenDecl*);
            void createAlwaysBlocks(const ModGenDecl*);
            void createAlwaysBlock(const Always*);
            void createContAssigns(const ModGenDecl*);
            void createContAssign(const ContAssign*);
            void createStmts(const std::vector<Stmt*>&);

            void setInitExprVal(Expr*, const Expr*) const;

            template<class T> void replaceGenvarsAndLoopVars(T*) const;
            
            std::vector<std::string> currUnrollNames;
            BlockNameMgr *blockNameMgr;
            Module *module;
            bool status;
    };

    template <class T1, class T2> 
    std::pair<std::vector<T1*>, std::vector<T2*> >* 
    GetVecScalarCntvs(    const std::vector<T2*>& cntvs, const Scope *sc);
}
#endif
