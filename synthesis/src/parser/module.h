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

#ifndef _MODULE_H_
#define _MODULE_H_
#include <iostream>
#include <vector>
#include <assert.h>

#include "scope.h"
#include "port.h"
#include "modinst.h"
#include "gateinst.h"
namespace Rtl
{
    class Net;
    class Param;
    class LocalParam;
    class ContAssign;
    class Task;
    class Func;
    class Always;
    class Initial;
    class Defparam;
    class SpecifyBlock;
    class UDP;
    class UDPInst;
    class GenerateStmt;
    class Genvar;
    class EventVar;
    class Module: public Scope 
    {
        public:
            friend class OMCreator;
            friend class SemCheck;
            friend class SemanticChecker;
            friend class Elaborator;
            friend class GenUnroll;
            Module(const std::string& n): Scope(n){
            }
            virtual ~Module(){
            }
            void addNamedObject(const Base*);
            const std::vector<Port*>& getPorts() const{
                return ports;
            }

            const std::vector<Net*>& getNets()const{
                return nets;
            }
            const std::vector<Param*>& getParams() const{
                return params;
            }
            const std::vector<LocalParam*>& getLocalParams() const{
                return lparams;
            }
            void addGateInsts(const std::vector<Inst*>& insts){
                const int size = insts.size();
                for(int i = 0; i < size; ++i)
                {
                    GateInst* ginst = static_cast<GateInst*>(insts[i]);
                    ginsts.push_back(ginst);
                }
            }
            void addModInsts(const std::vector<Inst*>& insts){
                const int size = insts.size();
                for(int i = 0; i < size; ++i)
                {
                    ModInst* minst = static_cast<ModInst*>(insts[i]);
                    minsts.push_back(minst);
                    this->addObject(minst->getName().getName(),
                        minst, true);
                }
            }

            const std::list<GateInst*>& getGateInsts() const {
                return ginsts;
            }
            const std::list<ModInst*>& getModInsts() const{
                return minsts;
            }

            void addGateInst(const GateInst* inst){
                ginsts.push_back(const_cast<GateInst*>(inst));
            }

            void addModInst(const ModInst* inst){
                minsts.push_back(const_cast<ModInst*>(inst));
                this->addObject(inst->getName().getName(),
                    inst, true);
            }
        
            void addContAssign(const ContAssign* ca){
                contAssigns.push_back(const_cast<ContAssign*>(ca));
            }
            const std::vector<ContAssign*>& getContAssigns() const{
                return contAssigns;
            }
            const std::vector<Always*>& getAlwaysBlocks() const{
                return alwaysBlocks;
            }

            const std::vector<Initial*>& getInitBlocks() const{
                return initials;
            }
            const std::vector<Defparam*>& getDefparams() const {
                return defparams;
            }
            const std::vector<EventStmt*>& getEvents() const{
                return events;
            }
            const std::vector<EventVar*>& getEventVars() const{
                //This function is to deprecated. Event vars are
                //added to scope now.
                return eventDecls;
            }

            const std::vector<SpecifyBlock*>& getSpecifyBlocks() const{
                return specifyBlocks;
            }

            const std::list<UDPInst*>& getUDPInsts() const {
                return uinsts;
            }

            const std::vector<Task*>& getTasks() const {
                return tasks;
            }

            const std::vector<Func*>& getFuncs() const{
                return funcs;
            }

            const std::vector<GenerateStmt*>& getGenStmts() const {
                return gstmts;
            }
            const std::vector<Genvar*>& getGenvars() const {
                return genvars;
            }

            std::vector<Param*> getScalarParams()const;
            std::vector<VecParam*> getVectorParams() const;
            std::vector<Port*> getScalarPorts()const;
            std::vector<VecPort*> getVectorPorts() const;
            std::vector<VecNet*> getVectorNets() const;
            std::vector<Net*> getScalarNets() const;

            const Base* getPort(const IxName&) const;
            virtual void accept(OMVisitorBase* v) const;
            virtual Module* copy() const {
                //not implemented yet
                assert(0);
            }    

            virtual void addStmt(const Stmt*);
            void setPortOrder(const std::vector<std::string>& po) {
                portOrder = po;
            }    
        private:
            void addPort(Port *);
            void addParams(const std::vector<Param*>& p);
            void addLocalParams(const std::vector<LocalParam*>& lp);
            void addNets(const std::vector<Net*>& n);
            void addPorts(const std::vector<Port*>& p);
            void addTask(const Task*);
            void addFunc(const Func*);
            void addAlways(const Always*);
            void addInitial(const Initial*);
            void addDefparam(const Defparam*);
            void addEvent(const EventStmt*);
            void addEventVar(const EventVar*);
            void addSpecifyBlock(const SpecifyBlock*);
            void addUDPInst(const UDPInst*);
            void addGenerate(const GenerateStmt*);
            void addGenvar(const Genvar*);
            std::vector<Port*> ports;
            std::vector<Net*> nets;
            std::vector<Param*> params;
            std::vector<LocalParam*> lparams;
            std::list<GateInst*> ginsts;
            std::list<ModInst*> minsts;
            std::list<UDPInst*> uinsts;
            std::vector<ContAssign*> contAssigns;
            std::vector<Always*> alwaysBlocks;
            std::vector<Initial*> initials;
            std::vector<SpecifyBlock*> specifyBlocks;
            std::vector<Func*> funcs;
            std::vector<Task*> tasks;
            std::vector<Defparam*> defparams;
            std::vector<EventStmt*> events;
            std::vector<EventVar*> eventDecls;
            std::vector<GenerateStmt*> gstmts;
            std::vector<Genvar*> genvars;
            std::vector<std::string> portOrder;
    };
}


#endif
