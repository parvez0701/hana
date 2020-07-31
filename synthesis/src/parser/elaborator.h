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

#ifndef _ELABORATOR_H_
#define _ELABORATOR_H_
#include <string>
#include <map>
#include <vector>

#include "defines.h"

namespace Rtl
{
    class Module;
    class ModInst;
    class Dimension;
    class Task;
    class TaskOrFunc;
    class Func;
    class Base;
    class Stmt;
    class Always;
    class Block;
    class Case;
    class CompAssign;
    class DelayStmt;
    class EventCntrl;
    class EventStmt;
    class For;
    class NamedBlock;
    class NamedBlocBase;
    class Scope;
    class SpecifyBlock;
    class SpecifyPathBase;
    class SpecTask;
    class DelayVal;
    class Delay;
    class SpecRefEvent;
    class Pin;
    class VecPin;
    class Inst;
    class ContAssign;
    class Defparam;
    class GenerateStmt;
    class GenerateItem;
    class GenerateCond;
    class GenerateCase;
    class GenerateLoop;
    class GenerateBlock;
    class GenDecl;

    typedef std::map<Path, std::map<std::string, Expr*, compare_str>,
            compare_path> DefparamInfo;

    class Elaborator
    {
        friend class OMVisitor;
        public:
            typedef struct {
                Module* oldmod;
                Module* newmod;
                std::map<Path, ModInst*, compare_path>
                    insts;
            } ElabInfo;

            static Elaborator* instance();
            virtual ~Elaborator();
            void elaborate(const bool partial=true);
            void unelaborate(const bool partial=true);
            Module* elaborate(ModInst*, const Path&, const bool=true);
            Module* unelaborate(ModInst*, const Path&, const bool=true);
            Expr* getElabExpr(const Expr*) const;
            void addElabExpr(const Expr*, const Expr*);
        private:
            Elaborator();
            Elaborator(const Elaborator&);
            const Elaborator& operator=(const Elaborator&);
            static Elaborator* elaborator;
            void elaborate(const Module*, 
                    const Path&,
                    const bool);

            std::string getElabModName(
                    const Module*, 
                    const ModInst*,
                    const std::vector<NameConn*>& defparams)const;

            bool elaborationRequired(
                    const Module*, 
                    const ModInst*,
                    std::vector<NameConn*>& defparams,
                    const Path&)const;

            Module* getElabModule(
                    const Module*, 
                    const ModInst*,
                    const std::vector<NameConn*>& defparams)const;

            const Expr* getReplacementParam(
                    const Module*, 
                    const Module*, 
                    const Expr*) ;
            const Expr* replaceParamInExpr(
                    const Module*,
                    Module*,
                    const Expr*,
                    Expr*);
            void replaceParamInDim(
                    const Module*,
                    Module*,
                    const Dimension*,
                    Dimension*);

            void addPorts(const Module*, Module*);
            void addPorts(const Module*, Module*, const TaskOrFunc*, TaskOrFunc*);
            void addNets(const Module*, Module*);
            void addRegs(const Module*, Module*);
            void addLocalParams(const Module*, Module*);
            void addStmt(
                    const Module*,
                    Module*,
                    const Stmt*);
            Base* addObject(
                    const Module*,
                    Module*,
                    const Base*);
            Dimension* createReplacementDim(
                    const Module*,
                    Module*,
                    const Dimension*);

            Task* addTask(
                    const Module*,
                    Module*,
                    const Task*);

            Func* addFunc(
                    const Module*,
                    Module*,
                    const Func*);
            Always* addAlways(
                    const Module*,
                    Module*,
                    const Always*);
            Block* createBlock(
                    const Module*,
                    Module*,
                    const Block*);
            Case* createCase(
                    const Module*,
                    Module*,
                    const Case*);

            CompAssign* createCompAssign(
                    const Module*,
                    Module*,
                    const CompAssign*);

            DelayStmt* createDelayStmt( 
                    const Module*, 
                    Module*,
                    const DelayStmt*);

            EventCntrl* createEventCntrl(
                    const Module*,
                    Module*,
                    const EventCntrl*);

            EventStmt* addEventStmt(
                    const Module*,
                    Module*,
                    const EventStmt*);
            For* createFor(
                    const Module*,
                    Module*,
                    const For*);

            NamedBlock* createNamedBlock(
                    const Module*,
                    Module*,
                    const NamedBlock*);

            template<typename T1, typename T2>
            void addVarsInScope(
                    const Module*,
                    Module*,
                    const Scope*,
                    Scope*,
                    const std::vector<T1*>&);

            void addObjectsInScope(
                    const Module*,
                    Module*,
                    const Scope*,
                    Scope*);

            void addSpecifyBlock(
                    const Module*,
                    Module*,
                    const SpecifyBlock*
                    );

            SpecifyPathBase* createSpecifyPath(
                    const Module*,
                    Module*,
                    const SpecifyPathBase*
                    );

            SpecTask* createSpecTask(
                    const Module*,
                    Module*,
                    const SpecTask*
                    );

            DelayVal* createDelayVal(
                    const Module*,
                    Module*,
                    const DelayVal*
                    );

            Delay* createDelay(
                    const Module*,
                    Module*,
                    const Delay*
                    );
            SpecRefEvent* createSpecRefEvent(
                    const Module*,
                    Module*,
                    const SpecRefEvent*
                    );

            std::pair<std::vector<Pin*>, std::vector<VecPin*> > createPins(
                    const Module*,
                    Module*,
                    const Inst*,
                    const std::vector<Pin*>&
                    );

            Inst* addInst(
                    const Module*,
                    Module*,
                    const Inst*
                    );

            ContAssign* addContAssign(
                    const Module*,
                    Module*,
                    const ContAssign*
                    );

            Defparam* addDefparam(
                    const Module*,
                    Module*,
                    const Defparam*
                    );

            GenerateStmt* addGenerate(
                    const Module*,
                    Module*,
                    const GenerateStmt*
                    );
            GenerateItem* createGenItem(
                    const Module*,
                    Module*,
                    const GenerateItem*
                    );

            GenerateCond* createGenCond(
                    const Module*,
                    Module*,
                    const GenerateCond*
                    );
            GenerateCase* createGenCase(
                    const Module*,
                    Module*,
                    const GenerateCase*
                    );

            GenerateBlock* createGenBlock(
                    const Module*,
                    Module*,
                    const GenerateBlock*
                    );
            GenerateLoop* createGenLoop(
                    const Module*,
                    Module*,
                    const GenerateLoop*
                    );
            GenDecl* createGenDecl (
                    const Module*,
                    Module*,
                    const GenDecl*
                    );


            void populateDefparamInfo();
            void populateDefparamInfo(
                    const Module*,
                    const Path&
                    );
                    


            //Key of the map is name of elab module
            std::map<std::string, ElabInfo*, compare_str> elabInfos;
            std::map<std::string, Expr*, compare_str> newmodexprs;
            std::map<Expr*, Expr*> oldnewexprs;
            //Path points to the instance for which defparams are used.
            DefparamInfo defparamInfo;
    };

    template<class T> const T*
    GetObject(const std::string&, const T *, const Module *);


}

#endif
