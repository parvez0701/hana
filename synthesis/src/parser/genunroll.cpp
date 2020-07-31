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

#include <sstream>
#include <assert.h>
#include <algorithm>

#include "genunroll.h"
#include "module.h"
#include "generate.h"
#include "basednum.h"
#include "always.h"
#include "contassign.h"
#include "intconst.h"
#include "vecnet.h"
#include "vecvar.h"
#include "integer.h"

using std::string;
using std::map;
using std::pair;
using std::ostringstream;
using std::make_pair;
using std::vector;
using std::for_each;
using std::back_inserter;


namespace Rtl
{
    template <class T1, class T2> 
    pair<vector<T1*>, vector<T2*> >* 
    GetVecScalarCntvs(    const vector<T2*>& cntvs,
                        const Scope *sc)
    {
        pair<vector<T1*>, vector<T2*> >* vecScCntvs = 
            new pair<vector<T1*>, vector<T2*> >();
        std::string name;
        for(unsigned int i = 0; i < cntvs.size(); ++i)
        {
            if(cntvs[i]->isVecBit())
            {
                if(name != cntvs[i]->getIxName().getName())
                {
                    name = cntvs[i]->getIxName().getName();
                    const T1* t1 = dynamic_cast<const T1*>(
                    GetObject(name, sc));
                    if(t1) {
                        vecScCntvs->first.push_back(const_cast<T1*>(t1));
                    }
                }
            }
            else {
                vecScCntvs->second.push_back(cntvs[i]);
            }
        }
        return vecScCntvs;
    }    

    BlockNameMgr::BlockNameMgr(const Module *mod):
        module(const_cast<Module*>(mod))
    {    
    }

    BlockNameMgr::~BlockNameMgr() {
    }

    string
    BlockNameMgr::getOldName(const string& newName) const
    {
        if(nameTableStack.empty()) {
            return newName;
        }
        NewOldNameMap::const_iterator iter = nameTableStack.top().
            newOldNameMap.find(newName);
        if(iter == nameTableStack.top().newOldNameMap.end()) {
            return newName;
        }
        return iter->second;
    }

    string
    BlockNameMgr::createNewName(const string& hint) const
    {
        int i = 0;
        string newHint(string("gen_") + hint);
        //std::replace(name.find_first_of('['), 1, "_");
        //std::replace(name.find_first_of(']'), 1, "_");
        string name(newHint);

        while(1)
        {
            if(!module->getObject(name)) {
                return name;
            }
            ostringstream str;
            name = newHint;
            str << name << "_" << i++;
            name = str.str();
        }
        assert(0);
    }

    string
    BlockNameMgr::createGenBlockName(const GenerateItem *genItem) const
    {
        string name/*("GEN_")*/;
        if(const Scope *sc = dynamic_cast<const Scope*>(genItem)) {
            name += sc->getName();
        }
        return this->createNewName(name);
    }

    const string&
    BlockNameMgr::getGenBlockName() const
    {
        assert(!nameTableStack.empty());
        return nameTableStack.top().blockName;
    }

    void
    BlockNameMgr::addName(const string& oldName, const string& newName) 
    {
        if(nameTableStack.empty()) {
            return;
        }
        nameTableStack.top().newOldNameMap.insert(make_pair(newName, oldName));
    }    

    const Base*
    BlockNameMgr::findObject(const string& name) const
    {
        const Base *obj = module->getObject(name);
        //if(obj) {
        //    return obj;
        //}
        //obj = module->getObject(this->getOldName(name));
        return obj;
    }

    void
    BlockNameMgr::diveInsideNewScope(const GenerateItem *genItem) {
        GenBlockInfo genInfo = {this->createGenBlockName(genItem),
                                GenvarMap(), NewOldNameMap(),
                                LoopvarMap(), 0};
        //while we dive in, we use the maps of the higher scope
        if(!nameTableStack.empty())
        {
            genInfo.genvars = nameTableStack.top().genvars;
            genInfo.newOldNameMap = nameTableStack.top().newOldNameMap;
            genInfo.loopvars = nameTableStack.top().loopvars;
        }    
        nameTableStack.push(genInfo);
    }

    void
    BlockNameMgr::comeOutOfScope() {
        if(nameTableStack.empty()) {
            return;
        }
        nameTableStack.pop();
    }    
    void    
    BlockNameMgr::addGenvar(const Genvar *genvar)
    {
        assert(!nameTableStack.empty());

        GenvarMap& genvars = nameTableStack.top().genvars;

        string newName = this->createNewName(genvar->getName());
        Genvar *newGenvar = 0;
        if(newName != genvar->getName()) 
        {
            newGenvar = new Genvar(newName, genvar->getVal());
            genvars.erase(genvar->getName());
            genvars.insert(make_pair(newGenvar->getName(), newGenvar));
        }    
        else 
        {
            genvars.erase(genvar->getName());
            genvars.insert(make_pair(genvar->getName(), 
                const_cast<Genvar*>(genvar)));
        }    
        this->addName(genvar->getName(), newName);
    }    

    Genvar*
    BlockNameMgr::findGenvar(const string& name) const
    {
        if(nameTableStack.empty()) {
            return NULL;
        }

        GenvarMap::const_iterator iter = nameTableStack.top().genvars.find(name);
        if(iter == nameTableStack.top().genvars.end()) {
            return NULL;
        }
        return iter->second;
    }    

    const BlockNameMgr::GenvarMap&
    BlockNameMgr::getGenvars() const
    {
        assert(!nameTableStack.empty());
        return nameTableStack.top().genvars;
    }    

    Expr*
    BlockNameMgr::getLoopVar() const 
    {
        assert(!nameTableStack.empty());
        return nameTableStack.top().loopvar;
    }    

    void
    BlockNameMgr::setLoopVar(Expr *lv)
    {
        assert(!nameTableStack.empty());
        nameTableStack.top().loopvar = lv;
        nameTableStack.top().loopvars.erase(lv->toString());
        nameTableStack.top().loopvars.insert(
            make_pair(lv->toString(), lv));
    }    

    Expr*
    BlockNameMgr::getLoopVarVal() const
    {
        Expr *loopvar = this->getLoopVar();
        return this->getLoopVarVal(loopvar);
    }

    Expr*
    BlockNameMgr::getLoopVar(const string& name) const
    {
        if(nameTableStack.empty()) {
            return NULL;
        }    

        LoopvarMap::const_iterator iter = 
            nameTableStack.top().loopvars.find(name);
        if(iter == nameTableStack.top().loopvars.end()) {
            return NULL;
        }
        return iter->second;
    }

    const  BlockNameMgr::LoopvarMap& 
    BlockNameMgr::getLoopVars() const 
    {
        assert(!nameTableStack.empty());
        return nameTableStack.top().loopvars;
    }

    Expr*
    BlockNameMgr::getLoopVarVal(const Expr* loopvar) const
    {
        if(!loopvar) {
            return NULL;
        }    
        int val = loopvar->evaluate();
        assert(val != INT_MIN);
        IntConst *newloopvar = new IntConst(val);
        newloopvar->setLine(loopvar->getLine());
        return newloopvar;
    }    

    void
    BlockNameMgr::addLoopVar(const Expr* lv)
    {
        //does not set the current loopvar
        assert(!nameTableStack.empty());
        nameTableStack.top().loopvars.erase(lv->toString());
        nameTableStack.top().loopvars.insert(
            make_pair(lv->toString(), const_cast<Expr*>(lv)));
    }        

    const BlockNameMgr::NewOldNameMap&
    BlockNameMgr::getNameMap() const 
    {
        assert(!nameTableStack.empty());
        return nameTableStack.top().newOldNameMap;
    }

    void
    BlockNameMgr::removeFromNewOldNameMap(const string& name) 
    {
        if(nameTableStack.empty()) {
            return;
        }
        nameTableStack.top().newOldNameMap.erase(name);
    }    
        


//-----------------------------------------------------------------------
//    Class which elaborates generate block
//-----------------------------------------------------------------------

    GenUnroll::GenUnroll(   const Module *mod) :
                            blockNameMgr(new BlockNameMgr(mod)),
                            module(const_cast<Module*>(mod)),
                            status(true){
    }

    GenUnroll::~GenUnroll() {
    }

    bool
    GenUnroll::operator()() {
        return this->unroll();
    }


    BasedNum*
    GenUnroll::getGenvarValue(const Genvar* genvar) const 
    {
        if(!genvar || (genvar->getVal() == INT_MIN)) {
            return NULL;
        }    
        vector<char> valString = genvar->getValString();
        BasedNum* bnum = new BasedNum(valString.size(), 'd', false, valString);
        bnum->setLine(genvar->getLine());
        return bnum;
    }    

    bool
    GenUnroll::init()
    {
        const vector<Genvar*>& gvars = module->getGenvars();
        for(unsigned int i = 0; i < gvars.size(); ++i) {
            blockNameMgr->addGenvar(gvars[i]);
        }    
        return true;

        //return !module->getGenStmts().empty();
    }

    bool
    GenUnroll::unroll()
    {
        blockNameMgr->diveInsideNewScope(0);
        if(!init()) {
            return false;
        }    
        const vector<GenerateStmt*>& genStmts = module->getGenStmts();
        bool status = true;
        for(unsigned int i = 0; i < genStmts.size(); ++i) {
            if(!unroll(genStmts[i])) {
                status = false;
            }
        }
        blockNameMgr->comeOutOfScope();
        return status;
    }

    bool
    GenUnroll::unroll(const GenerateStmt *genStmt)
    {
        const vector<Genvar*>& gvs = genStmt->getGenvars();
        for(unsigned int i = 0; i < gvs.size(); ++i) {
            blockNameMgr->addGenvar(gvs[i]);
        }    
        bool status = this->unroll(genStmt->getGenDecl());

        const vector<GenerateCond*>& genConds = genStmt->getCondGens();
        for(unsigned int i = 0; i < genConds.size(); ++i) {
            if(!this->unroll(genConds[i])) {
                status = false;
            }
        }    

        const vector<GenerateCase*>& genCases = genStmt->getCaseGens();
        for(unsigned int i = 0; i < genCases.size(); ++i) {
            if(!this->unroll(genCases[i])) {
                status = false;
            }
        }    

        const vector<GenerateLoop*>& genLoops = genStmt->getLoopGens();
        for(unsigned int i = 0; i < genLoops.size(); ++i) {
            if(!this->unroll(genLoops[i])) {
                status = false;
            }
        }    

        const vector<GenerateBlock*> genBlocks = genStmt->getBlockGens();
        for(unsigned int i = 0; i < genBlocks.size(); ++i) {
            if(!this->unroll(genBlocks[i])) {
                status = false;
            }
        }    
        return status;
    }

    bool
    GenUnroll::unroll(const GenerateItem *genItem)
    {
        if(const GenerateCond *genCond = 
            dynamic_cast<const GenerateCond*>(genItem)) {
            return this->unroll(genCond);
        }

        if(const GenerateCase *genCase =
            dynamic_cast<const GenerateCase*>(genItem)) {
            return this->unroll(genCase);
        }

        if(const GenerateBlock* genBlock =
            dynamic_cast<const GenerateBlock*>(genItem)) {
            return this->unroll(genBlock);
        }

        if(const GenerateLoop *genLoop = 
            dynamic_cast<const GenerateLoop*>(genItem)) {
            return this->unroll(genLoop);
        }    
            
        if(const GenDecl *genDecl = 
            dynamic_cast<const GenDecl*>(genItem)) {
            return this->unroll(genDecl);
        }
        return false;
    }

    bool
    GenUnroll::unroll(const GenerateCond *gcond)
    {
        blockNameMgr->diveInsideNewScope(gcond);
        int cond = gcond->condexpr->evaluate();
        bool status = true;
        if(cond == INT_MIN) {
            status = false;
        }
        if(cond != 0) {
            status = unroll(gcond->ifpart);
        }
        else if(gcond->elsepart) {
            status = unroll(gcond->elsepart);
        }
        blockNameMgr->comeOutOfScope();
        return status; 
    }    

    bool
    GenUnroll::unroll(const GenerateCase *genCase)
    {
        bool status = true;
        if(!genCase->select->isConstExpr()) {
            //message to be added
            return false;
        }    
        blockNameMgr->diveInsideNewScope(genCase);
        //this->unroll(genCase->getGenDecl());
        int select = genCase->select->evaluate();
        
        bool done = false;
        for(unsigned int i = 0; !done && i < genCase->caseitems.size(); ++i)
        {
            for(unsigned int j = 0; !done && j < genCase->caseitems[i].first.size(); ++j)
            {
                if(!genCase->caseitems[i].first[j]->isConstExpr()) 
                {
                    //message to be added
                    done = true;
                    status = false;
                }
                if(select == genCase->caseitems[i].first[j]->evaluate())
                {
                    done = true;
                    status = this->unroll(genCase->caseitems[i].second);
                }    
            }    
        }

        if(!done && genCase->defstmt) {
            status = this->unroll(genCase->defstmt);
        }    
        blockNameMgr->comeOutOfScope();
        return status;
    }

    bool
    GenUnroll::unroll(const GenerateBlock *genBlock)
    {
        bool status = true;
        blockNameMgr->diveInsideNewScope(genBlock);
        //this->unroll(genBlock->getGenDecl());

        for(unsigned int i = 0; i < genBlock->gitems.size(); ++i) {
            if(!this->unroll(genBlock->gitems[i])) {
                status = false;
            }
        }
        return status;
    }

    bool
    GenUnroll::unroll(const GenerateLoop *genLoop)
    {
        bool status = true;
        blockNameMgr->diveInsideNewScope(genLoop);

        Expr *lv = blockNameMgr->findGenvar(genLoop->loopvar->toString());

        if(!lv) {
            lv = dynamic_cast<Expr*>(const_cast<Base*>(
                GetObject(genLoop->loopvar->toString(), genLoop)));
        }        

        if(!lv)
        {
            //loop var can only be a genvar.
            //message to be issued
            blockNameMgr->comeOutOfScope();
            return false;
        }    

        if(!genLoop->initexpr->isConstExpr() /*|| 
            !genLoop->termexpr->isConstExpr()*/)
        {
            blockNameMgr->comeOutOfScope();
            return false;
        }    
        blockNameMgr->setLoopVar(lv);

        this->setInitExprVal(lv, genLoop->initexpr);


        int loopCount = 0;
        while(1)
        {
            int termVal = genLoop->termexpr->evaluate();
            ++loopCount;
            if(!termVal || termVal == INT_MIN) {
                break;
            }    
            if(loopCount > INT_MAX/2) {
                //I am not going to go into infinite loop because
                //you have an infinite loop in your verilog code
                break;
            }
            //if(!this->unroll(genLoop->getGenDecl())) {
            //    status = false;
            //}    
            if(!this->unroll(genLoop->gitem)) {
                status = false;
            }
            //gv->setVal(genLoop->assignexpr->evaluate());
            this->setInitExprVal(lv, genLoop->assignexpr);
        }

        blockNameMgr->comeOutOfScope();
        return status;
            
    }

    bool
    GenUnroll::unroll(const GenDecl *genDecl)
    {
        ModGenDecl *modGenDecl = genDecl->getGenDecl();
        if(!modGenDecl) {
            return true;
        }
        return this->unroll(modGenDecl);
    }    
    bool
    GenUnroll::unroll(const ModGenDecl *modGenDecl)
    {
        if(!modGenDecl) {
            return true;
        }

        currUnrollNames.clear();

        this->createGenvars(modGenDecl);
        this->createNets(modGenDecl);
        this->createVars(modGenDecl);
        this->createGateInsts(modGenDecl);
        this->createModInsts(modGenDecl);
        this->createAlwaysBlocks(modGenDecl);
        this->createContAssigns(modGenDecl);

        //These are irrelevant now, so remove them
        for(unsigned int i = 0; i < currUnrollNames.size(); ++i) {
            blockNameMgr->removeFromNewOldNameMap(currUnrollNames[i]);
        }
        currUnrollNames.clear();

        return true;
    }

    void
    GenUnroll::createNets(const ModGenDecl* genDecl)
    {
        if(genDecl->nets.empty()) {
            return;
        }    
        pair<vector<VecNet*>, vector<Net*> >* vecScCntvs =
            GetVecScalarCntvs<VecNet, Net>(genDecl->nets,
            genDecl->nets[0]->getScope());
        
        vector<Net*> nets;
        for(unsigned int i = 0; i < vecScCntvs->first.size(); ++i)
        {
            string newName = blockNameMgr->createNewName(
                vecScCntvs->first[i]->getName());
            VecNet *newNet = vecScCntvs->first[i]->copy();
            newNet->setName(newName);
            newNet->setScope(module);
            const vector<Net*>& newNets = newNet->getNets();
            for(unsigned int j = 0; j < newNets.size(); ++j) 
            {
                newNets[j]->setIxName(IxName(newName, newNets[j]->getIxName().getIndex()));
                blockNameMgr->addName(IxName(vecScCntvs->first[i]->
                getName(), newNets[j]->getIxName().getIndex()).
                toString(), newNets[j]->toString());
                nets.push_back(newNets[j]);

                newNets[j]->setScope(module);
                currUnrollNames.push_back(newNets[j]->toString());
            }    
            blockNameMgr->addName(vecScCntvs->first[i]->getName(), newName);
            currUnrollNames.push_back(newName);
            module->addObject(newName, newNet, false);
        }
        for(unsigned int i = 0; i < vecScCntvs->second.size(); ++i)
        {
            string newName = blockNameMgr->createNewName(
                vecScCntvs->second[i]->toString());
            Net *newNet = vecScCntvs->second[i]->copy();
            newNet->setIxName(IxName(newName));
            newNet->setScope(module);
            module->addObject(newName, newNet, false);
            blockNameMgr->addName(vecScCntvs->second[i]->
            getIxName().getName(), newName);
            nets.push_back(newNet);
            currUnrollNames.push_back(newName);
        }    
        module->addNets(nets);

        /*
        for(unsigned int i = 0; i < genDecl->nets.size(); ++i) 
        {
            Net *net = new Net(blockNameMgr->createNewName(
                genDecl->nets[i]->toString()));
            net->setNetType(genDecl->nets[i]->getNetType());    
            net->setScope(module);
            module->addObject(net->getIxName().toString(), net, false);
            nets.push_back(net);
            blockNameMgr->addName(genDecl->nets[i]->getIxName().toString(),
                net->getIxName().toString());
        }
        */
    }

    void
    GenUnroll::createVars(const ModGenDecl* genDecl)
    {
        if(genDecl->variables.empty()) {
            return;
        }    
        pair<vector<VecVar*>, vector<Var*> >* vecScCntvs =
            GetVecScalarCntvs<VecVar, Var>(genDecl->variables,
            genDecl->variables[0]->getScope());
        
        for(unsigned int i = 0; i < vecScCntvs->first.size(); ++i)
        {
            string newName = blockNameMgr->createNewName(
                vecScCntvs->first[i]->toString());
            VecVar *newVar = dynamic_cast<VecVar*>(
                vecScCntvs->first[i]->copy());
            assert(newVar);    
            newVar->setName(newName);
            newVar->setScope(module);
            module->addObject(newName, newVar, false);
            const vector<Var*>& vars = newVar->getVars();
            for(unsigned int j = 0; j < vars.size(); ++j) 
            {
                vars[j]->setIxName(IxName(newName, vars[j]->getIxName().getIndex()));
                vars[j]->setScope(module);
                blockNameMgr->addName(IxName(vecScCntvs->first[i]->
                getName(), vars[j]->getIxName().getIndex()).toString(), 
                vars[j]->toString());
                currUnrollNames.push_back(vars[j]->toString());
            }    
            blockNameMgr->addName(vecScCntvs->first[i]->getName(), newName);
            currUnrollNames.push_back(newName);
        }
        for(unsigned int i = 0; i < vecScCntvs->second.size(); ++i)
        {
            string newName = blockNameMgr->createNewName(
                vecScCntvs->second[i]->toString());
            Var *newVar = vecScCntvs->second[i]->copy();
            newVar->setIxName(IxName(newName));
            newVar->setScope(module);
            module->addObject(newName, newVar, false);
            blockNameMgr->addName(vecScCntvs->second[i]->
            getIxName().toString(), newName);
            currUnrollNames.push_back(newName);
        }    
                
/*    
        for(unsigned int i = 0; i < genDecl->variables.size(); ++i) 
        {
            string newName = blockNameMgr->createNewName(
                genDecl->variables[i]->toString());
            Var *newVar = genDecl->variables[i]->copy();
            newVar->setIxName(IxName(newName));
            module->addDeclItem(newVar);
        }
*/        
    }

    void
    GenUnroll::createGateInsts(const ModGenDecl *genDecl)
    {
        vector<Inst*> ginsts;
        for(unsigned int i = 0; i < genDecl->ginsts.size(); ++i)
        {
            GateInst *ginst = genDecl->ginsts[i];
            string newname = blockNameMgr->createNewName(ginst->getName().getName());
            GateInst *newInst = ginst->copy();
            newInst->setName(newname);
            this->replaceGenvarsAndLoopVars<GateInst>(newInst);
            newInst->setScope(module);
            module->addGateInst(newInst);
        }
    }

    void
    GenUnroll::createModInsts(const ModGenDecl *genDecl)
    {
        for(unsigned int i = 0; i < genDecl->minsts.size(); ++i)
        {
            ModInst *minst = genDecl->minsts[i];

            string newname = blockNameMgr->createNewName(minst->getName().getName());

            ModInst *newInst = minst->copy();
            newInst->setName(newname);
            this->replaceGenvarsAndLoopVars<ModInst>(newInst);
            module->addModInst(newInst);    
            newInst->setScope(module);

        }
    }

    void
    GenUnroll::createAlwaysBlocks(const ModGenDecl *genDecl)
    {
        for(unsigned int i = 0; i < genDecl->alwaysBlocks.size(); ++i) {
            createAlwaysBlock(genDecl->alwaysBlocks[i]);
        }    
    }

    void 
    GenUnroll::createAlwaysBlock(const Always *always)
    {
        Always *newAlways = always->copy();
        this->replaceGenvarsAndLoopVars<Always>(newAlways);
        module->addAlways(newAlways);
    }

    void
    GenUnroll::createGenvars(const ModGenDecl *genDecl)
    {
        for(unsigned int i = 0; i < genDecl->genvars.size(); ++i) {
            blockNameMgr->addGenvar(genDecl->genvars[i]);
        }
    }    

    void
    GenUnroll::createContAssigns(const ModGenDecl *genDecl)
    {
        for(unsigned int i = 0; i < genDecl->contassigns.size(); ++i) {
            this->createContAssign(genDecl->contassigns[i]);
        }    
    }

    void
    GenUnroll::createContAssign(const ContAssign *cassign)
    {
        ContAssign *newContAssign = cassign->copy();
        this->replaceGenvarsAndLoopVars<ContAssign>(newContAssign);
        module->addContAssign(newContAssign);
    }

    void
    GenUnroll::createStmts(const vector<Stmt*>& stmts)
    {
        for(unsigned int i = 0; i < stmts.size(); ++i)
        {
            Stmt *stmt = stmts[i]->copy();
            this->replaceGenvarsAndLoopVars<Stmt>(stmt);
            module->addStmt(stmt);
        }
    }

    void
    GenUnroll::setInitExprVal(Expr *loopvar, const Expr *val) const
    {
        assert(!val || val->isConstExpr());
        if(Genvar *gv = dynamic_cast<Genvar*>(loopvar)) {
            gv->setVal(val ? val->evaluate() : INT_MIN);
        }
        else if(Integer *int_ = dynamic_cast<Integer*>(loopvar)) 
        {
            IntConst *newint_ = new IntConst(val->evaluate());
            newint_->setLine(val->getLine());
            int_->setVal(newint_);
        }
        else {
            assert(0);
        }    
    }

    template<class T> void
    GenUnroll::replaceGenvarsAndLoopVars(T *obj) const
    {
        const BlockNameMgr::LoopvarMap& loopvars = blockNameMgr->getLoopVars();
        BlockNameMgr::LoopvarMap::const_iterator lvBeginIter = loopvars.begin();
        BlockNameMgr::LoopvarMap::const_iterator lvEndIter = loopvars.end();


        for(; lvBeginIter != lvEndIter; ++lvBeginIter) 
        {
            std::string oldName = blockNameMgr->getOldName(lvBeginIter->first);
            obj->repExpr(oldName, blockNameMgr->getLoopVarVal(lvBeginIter->second));
        }        
        const BlockNameMgr::GenvarMap& genMap = blockNameMgr->getGenvars();
        BlockNameMgr::GenvarMap::const_iterator beginIter = genMap.begin();
        BlockNameMgr::GenvarMap::const_iterator endIter = genMap.end();


        for(; beginIter != endIter; ++beginIter) 
        {
            std::string oldName = blockNameMgr->getOldName(beginIter->first);
            if(!oldName.empty() && beginIter->second->isValSet()) {
                obj->repExpr(blockNameMgr->getOldName(beginIter->first),
                    this->getGenvarValue(beginIter->second));
            }        
        }        

        //Now replace the general variables.
        const BlockNameMgr::NewOldNameMap& newOldNameMap = blockNameMgr->
            getNameMap();
        BlockNameMgr::NewOldNameMap::const_iterator nameBeginIter =
            newOldNameMap.begin();
        BlockNameMgr::NewOldNameMap::const_iterator nameEndIter =
            newOldNameMap.end();

        for(; nameBeginIter != nameEndIter; ++nameBeginIter)
        {
            IxName ixName(nameBeginIter->first);
            if(ixName.toString() != ixName.getName())
            {
                //vector
                const Expr *expr = dynamic_cast<const Expr*>(
                GetObject(ixName.getName(), static_cast<const Scope*>(module)));
                if(expr)
                {    
                    const VecCntv *vecCntv = dynamic_cast<const VecCntv*>(expr);
                    assert(vecCntv);
                    expr = const_cast<ScalarCntv*>(
                    vecCntv->getBit(ixName.getIndex()));
                    assert(expr);
                    obj->repExpr(nameBeginIter->second, expr);
                }    
            }
            else 
            {
                const Expr *expr = dynamic_cast<const Expr*>(
                GetObject(nameBeginIter->first, module));
                if(expr) {
                    obj->repExpr(nameBeginIter->second, expr);
                }    
            }    
        }
    }

}
