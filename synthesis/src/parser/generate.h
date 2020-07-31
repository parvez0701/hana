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

#ifndef _GENERATE_H_
#define _GENERATE_H_

#include "expr.h"
#include "stmt.h"
#include "scope.h"

namespace Rtl
{
    class EventStmt;
    class ModGenDecl;
    class Net;
    class GateInst;
    class ModInst;
    class UDPInst;
    class Initial;
    class Always;
    class Var;
    class Defparam;
    class ContAssign;
    class Task;
    class Func;
    
    class Genvar: public Expr
    {
        public:
            Genvar(const std::string& n, int g): 
                Expr(), name(n), genvar(g){
            }
            Genvar(const std::string& n): 
                Expr(), name(n), genvar(INT_MIN){
            }

            virtual ~Genvar(){
            }
            const std::string& getName() const{
                return name;
            }

            int getVal() const{
                return genvar;
            }
            virtual std::string toString() const{
                return name;
            }
            virtual int evaluate() const {
                return genvar;
            }
            virtual long len() const{
                return 32;
            }

            bool isValSet() const {
                return genvar != INT_MIN;
            }    
            virtual void accept(OMVisitorBase* v) const;
            void setVal(int v){
                genvar = v;
            }
            std::vector<char> getValString() const;
            virtual Genvar *copy() const;
            virtual const Expr* repExpr(const std::string&, const Expr*);
            virtual bool isConstExpr() const;
            virtual bool isSigned() const {
                return true;
            }    
        private:
            std::string name;
            int genvar;
    };

    class GenerateItem: public Base
    {
        public:
            GenerateItem(): Base(){
            }
            virtual ~GenerateItem() {
            }    
            virtual ModGenDecl* getGenDecl() const{
                return NULL;
            }
            virtual GenerateItem* copy() const = 0;
    //        virtual GenerateItem* repExpr(const std::string&, const Expr*) = 0;
            virtual void repExpr(const std::string&, const Expr*) = 0;
    };

    class ModGenDecl
    {
        public:
            virtual ~ModGenDecl() {
            }
            std::vector<Net*> nets;
            std::vector<GateInst*> ginsts;
            std::vector<ModInst*> minsts;
            std::vector<UDPInst*> uinsts;
            std::vector<Always*> alwaysBlocks;
            std::vector<Initial*> initials;
            std::vector<Var*> variables;
            std::vector<Genvar*> genvars;
            std::vector<Defparam*> defparams;
            std::vector<ContAssign*> contassigns;
            std::vector<EventStmt*> events;
            std::vector<Task*> tasks;
            std::vector<Func*> funcs;
    };

    class GenDecl: public GenerateItem
    {
        public:
            GenDecl() {
                gdecl = new ModGenDecl();
            }    
            ~GenDecl() {
                delete gdecl;
            }    
            virtual void accept(OMVisitorBase* v) const;

            virtual ModGenDecl* getGenDecl() const {
                return gdecl;
            }
            void setGenDecl(const ModGenDecl* d){
                gdecl = const_cast<ModGenDecl*>(d);
            } 
            virtual GenDecl *copy() const;
            virtual void repExpr(const std::string&, const Expr*);
            //virtual GenDecl *repExpr(const std::string&, const Expr*);
        private:
            ModGenDecl* gdecl;
    };

    class GenerateCond: public GenerateItem
    {
        public:
            GenerateCond(const Expr* e) : GenerateItem(),
                condexpr(const_cast<Expr*>(e)){
            }
            virtual ~GenerateCond() {
            }
            virtual void accept(OMVisitorBase* v) const;
            virtual GenerateCond* copy() const; 
            virtual void repExpr(const std::string&, const Expr*);
            //virtual GenerateCond *repExpr(const std::string&, const Expr*);

            Expr* condexpr;
            GenerateItem* ifpart;
            GenerateItem* elsepart;
    };

    class GenerateCase: public GenerateItem
    {
        public:
            GenerateCase(const Expr* se, 
                std::vector<std::pair<std::vector<Expr*>, 
                GenerateItem*> >& cis, GenerateItem* dgi) : 
                GenerateItem(),
                select(const_cast<Expr*>(se)), defstmt(dgi) {
            }
            GenerateCase(const Expr* se) : 
                GenerateItem(),
                select(const_cast<Expr*>(se)), defstmt(0) {
            }
            virtual ~GenerateCase() {
            }
            virtual void accept(OMVisitorBase* v) const;
            virtual GenerateCase *copy() const;
            virtual void repExpr(const std::string&, const Expr*);
            //virtual GenerateCase *repExpr(const std::string&, const Expr*);

            //data
            Expr*    select;
            std::vector<std::pair<std::vector<Expr*>, GenerateItem*> >
                caseitems;
            GenerateItem* defstmt;
    };

    class GenerateBlock: virtual public GenerateItem, virtual public Scope
    {
        public:
            GenerateBlock(const std::string& name):
                GenerateItem(), Scope(name){
            }
            virtual ~GenerateBlock() {
            }
            virtual void accept(OMVisitorBase* v) const;

            virtual ModGenDecl* getGenDecl() const{
                const int size = gitems.size();
                for(int i = 0; i < size; ++i){
                    if(ModGenDecl* decl = gitems[i]->getGenDecl()){
                        return decl;
                    }
                }
                return NULL;
            }
            virtual GenerateItem* copy() const;
            virtual void repExpr(const std::string&, const Expr*);
            //virtual GenerateItem *repExpr(const std::string&, const Expr*);
    
            std::vector<GenerateItem*> gitems;
    };

    class GenerateLoop: virtual public GenerateItem, virtual public Scope
    {
        public:
            GenerateLoop(const Expr* lv, const Expr* ie, const Expr* te,
                const Expr* ae
                ) : GenerateItem(), Scope(""),
                loopvar(const_cast<Expr*>(lv)), 
                initexpr(const_cast<Expr*>(ie)),
                termexpr(const_cast<Expr*>(te)),
                assignexpr(const_cast<Expr*>(ae)),
                gitem(0)
                {
            }
            virtual ~GenerateLoop() {
            }
            virtual void accept(OMVisitorBase* v) const; 
            virtual ModGenDecl* getGenDecl() const{
                if(gitem){
                    return gitem->getGenDecl();
                }
                return NULL;
            }
            virtual GenerateItem* copy() const;
            virtual void repExpr(const std::string&, const Expr*);
            //virtual GenerateItem *repExpr(const std::string&, const Expr*);
            Expr* loopvar;
            Expr* initexpr;
            Expr* termexpr;
            Expr* assignexpr;
            GenerateItem* gitem;
    };


    class GenerateStmt: virtual public Stmt, virtual public Scope
    {
        friend class OMVisitor;
        public:
            GenerateStmt(): Stmt(), Scope(""), gdecl(new GenDecl()){
            }
            virtual ~GenerateStmt() {
            }

            const std::vector<GenerateCond*>& getCondGens() const{
                return conds;
            }
            
            const std::vector<GenerateCase*>& getCaseGens() const {
                return cases;
            }

            const std::vector<GenerateLoop*>& getLoopGens() const{
                return loops;
            }

            const std::vector<GenerateBlock*>& getBlockGens() const{
                return blocks;
            }


            const std::vector<Genvar*>& getGenvars() const{
                return genvars;
            }
            void addGenvar(const Genvar* g){
                genvars.push_back(const_cast<Genvar*>(g));
            }

            void addGenerateItem(const GenerateItem* item){
                if(const GenerateCond* gc = 
                    dynamic_cast<const GenerateCond*>(item)){
                    conds.push_back(const_cast<GenerateCond*>(gc));
                }
                else if(const GenerateCase* gc = 
                    dynamic_cast<const GenerateCase*>(item)){
                    cases.push_back(const_cast<GenerateCase*>(gc));
                }
                else if(const GenerateBlock* gb =
                    dynamic_cast<const GenerateBlock*>(item)){
                    blocks.push_back(const_cast<GenerateBlock*>(gb));
                }
                else if(const GenerateLoop* gl =
                    dynamic_cast<const GenerateLoop*>(item)){
                    loops.push_back(const_cast<GenerateLoop*>(gl));
                }
                else if(const GenDecl* gd =
                    dynamic_cast<const GenDecl*>(item)){
                    gdecl = const_cast<GenDecl*>(gd);
                }
                    
            }
            const GenDecl* getDecl() const {
                return gdecl;
            }
            virtual ModGenDecl* getGenDecl() const {
                if(gdecl){
                    return gdecl->getGenDecl();
                }
                return NULL;
            }


            virtual void accept(OMVisitorBase* v) const;
            virtual GenerateStmt* copy() const;
            //virtual GenerateStmt* repExpr(const std::string&, const Expr*);
            virtual void repExpr(const std::string&, const Expr*);

        private:
            std::vector<GenerateCond*> conds;
            std::vector<GenerateCase*> cases;
            std::vector<GenerateLoop*> loops;
            std::vector<GenerateBlock*> blocks;
            GenDecl* gdecl;
            std::vector<Genvar*> genvars;
    };
            
}
#endif
