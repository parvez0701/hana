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

#ifndef _SYSTMT_H_
#define _SYSTMT_H_

#include <vector>

#include "sybase.h"
#include "sylist.h"
namespace Synth
{
    class SyExpr;
    class SyTask;
    class SyModule;
    class SyVisitorBase;
    class TVisitor;
    class SyTaskOrFunc;
    class SyTaskOrFuncCall;
    class SyCntv;

    class SyStmt: public SyBase
    {
        public:
            SyStmt() : SyBase() {
            }
            virtual ~SyStmt() {
            }
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*) {
            }

            virtual SyStmt* copy() const = 0; 
            virtual void accept(SyVisitorBase*) const {
            }
    };

    class SyAssign: public SyStmt
    {
        friend class SyOMVisitor;
        public:
            SyAssign(
                    const SyExpr *l,
                    const SyExpr *r) :
                SyStmt(),
                lhs(const_cast<SyExpr*>(l)),
                rhs(const_cast<SyExpr*>(r)) {
                }
            const SyExpr *getLExpr() const {
                return lhs;
            }
            const SyExpr *getRExpr() const {
                return rhs;
            }
            void setRExpr(const SyExpr *expr);
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);

        private:
            SyExpr* lhs;
            SyExpr* rhs;
    };

    class SyContAssign: public SyAssign
    {
        public:
            SyContAssign(
                    const SyExpr *l,
                    const SyExpr *r):
                SyAssign(l, r) {
                }
            virtual ~SyContAssign() {
            }
            virtual SyContAssign* copy() const;
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);

    };

    class SyNonBlockAssign: public SyAssign
    {
        public:
            SyNonBlockAssign(
                    const SyExpr *l,
                    const SyExpr *r):
                SyAssign(l, r) {
                }
            virtual SyNonBlockAssign* copy() const;
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);
    };

    class SyBlockAssign: public SyAssign
    {
        public:
            SyBlockAssign(
                    const SyExpr *l,
                    const SyExpr *r):
                SyAssign(l, r) {
                }

            virtual SyBlockAssign* copy() const;
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);
    };

    class SyIf: public SyStmt
    {
        friend class SyOMVisitor;
        public:
            SyIf(const SyExpr* c): SyStmt(),
            cond(const_cast<SyExpr*>(c)) {
            }

            virtual ~SyIf() {
            }
            void addThenStmt(const SyStmt *s) {
                if(s) {
                    thenPart.push_back(const_cast<SyStmt*>(s));
                }
            }

            void addElseStmt(const SyStmt *s) {
                if(s) {
                    elsePart.push_back(const_cast<SyStmt*>(s));
                }
            }

            const SyList<SyStmt*>& getThenPart() const {
                return thenPart;
            }
            const SyList<SyStmt*>& getElsePart() const {
                return elsePart;
            }
            void setThenPart(const SyList<SyStmt*>& stmts){
                thenPart = stmts;
            }
            void setElsePart(const SyList<SyStmt*>& stmts){
                elsePart = stmts;
            }
            const SyExpr *getCond() const {
                return cond;
            }
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);

            virtual SyIf* copy() const;
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);
        private:
            SyExpr *cond;
            SyList<SyStmt*> thenPart;
            SyList<SyStmt*> elsePart;
    };

    class SyEventCtrlList
    {
        public:
            SyEventCtrlList() {
            }
            SyEventCtrlList(const SyEventCtrlList&);
            SyEventCtrlList& operator=(const SyEventCtrlList&);
            ~SyEventCtrlList() {
            }
            void addPosEdgeExpr(const SyExpr* expr){
                if(expr){
                    posExprs.push_back(const_cast<SyExpr*>(expr));
                }
            }

            void addNegEdgeExpr(const SyExpr *expr){
                if(expr) {
                    negExprs.push_back(const_cast<SyExpr*>(expr));
                }
            }
            void addExpr(const SyExpr *expr){
                if(expr){
                    exprs.push_back(const_cast<SyExpr*>(expr));
                }
            }
            const std::vector<SyExpr*>& getPosEdgeExprs() const {
                return posExprs;
            }

            const std::vector<SyExpr*>& getNegEdgeExprs() const {
                return negExprs;
            }
            const std::vector<SyExpr*>& getExprs() const {
                return exprs;
            }
            void repSubExpr(
                const std::string&, 
                const SyExpr*);
        private:
            std::vector<SyExpr*> posExprs;
            std::vector<SyExpr*> negExprs;
            std::vector<SyExpr*> exprs;
    };

    class SyEventStmt: public SyStmt
    {
        public:
            SyEventStmt() {
            }
            virtual ~SyEventStmt() {
            }
            void addPosEdgeExpr(const SyExpr* expr){
                eventList.addPosEdgeExpr(expr);
            }

            void addNegEdgeExpr(const SyExpr *expr){
                eventList.addNegEdgeExpr(expr);
            }
            void addExpr(const SyExpr *expr){
                eventList.addExpr(expr);
            }

            void addStmt(const SyStmt *stmt) {
                if(stmt){
                    stmts.push_back(const_cast<SyStmt*>(stmt));
                }
            }

            const std::vector<SyExpr*>& getPosEdgeExprs() const {
                return eventList.getPosEdgeExprs();
            }

            const std::vector<SyExpr*>& getNegEdgeExprs() const {
                return eventList.getNegEdgeExprs();
            }
            const std::vector<SyExpr*>& getExprs() const {
                return eventList.getExprs();
            }

            const SyList<SyStmt*>& getStmts() const {
                return stmts;
            }
            const SyEventCtrlList& getEventCtrlList() const{
                return eventList;
            }
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);
            virtual void accept(TVisitor*, SyModule*);

            SyList<SyCntv*>* getReadVars() const;

        private:
            SyList<SyCntv*>* getReadVars(const SyStmt*) const;
            SyList<SyCntv*>* getReadVars(const SyExpr*) const;
            SyList<SyCntv*>* getReadVars(const SyTaskOrFuncCall*) const;

            
            SyEventCtrlList eventList;
            SyList<SyStmt*> stmts;
    };

    class SyAlways: public SyEventStmt
    {
        public:
            SyAlways(): SyEventStmt() {
            }
            virtual ~SyAlways() {
            }

            virtual SyAlways* copy() const;
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);
    };

    class SyEventCtrl: public SyEventStmt
    {
        public:
            SyEventCtrl(): SyEventStmt() {
            }
            virtual ~SyEventCtrl() {
            }

            virtual SyEventCtrl* copy() const;
            virtual void accept(SyVisitorBase*) const;
    };

    class SyCaseItem: public SyBase
    {
        friend class SyOMVisitor;
        public:
            SyCaseItem(const SyExpr *l, bool def=false):
                SyBase(), 
                label(const_cast<SyExpr*>(l)),
                defitem(def){
                }
            virtual ~SyCaseItem() {
            }
            void addStmt(const SyStmt *st) {
                if(st) {
                    stmts.push_back(const_cast<SyStmt*>(st));
                }
            }

            const SyList<SyStmt*>& getStmts() const {
                return stmts;
            }

            const SyExpr* getLabel() const {
                return label;
            }
            virtual SyCaseItem* copy() const;
            bool isDefaultItem() const {
                return defitem;
            }    
        private:
            SyExpr *label;
            SyList<SyStmt*> stmts;
            bool defitem;
    };

    class SyCase: public SyStmt
    {
        friend class SyOMVisitor;
        public:
            SyCase(const SyExpr *sel):
                SyStmt(),
                select(const_cast<SyExpr*>(sel)){
                }
            virtual ~SyCase() {
            }

            void addItem(const SyCaseItem *item) {
                if(item){
                    caseItems.push_back(const_cast<SyCaseItem*>(item));
                }
            }

            const SyExpr *getSelect() const {
                return select;
            }
            const SyList<SyCaseItem*>& getCaseItems() const {
                return caseItems;
            }
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);

            void clearItems() {
                caseItems.clear();
            }    

            virtual SyCase* copy() const;
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);

        private:
            SyList<SyCaseItem*> caseItems;
            SyExpr *select;
    };

    class SyTaskOrFuncCall: public SyStmt
    {
        friend class SyOMVisitor;
        public:
            SyTaskOrFuncCall(const SyTaskOrFunc *tf);

            SyTaskOrFuncCall(const std::string& n);

            virtual ~SyTaskOrFuncCall() {
            }
            void addArg(const SyExpr* arg) {
                args.push_back(const_cast<SyExpr*>(arg));
            }

            const std::vector<SyExpr*>& getArgs() const {
                return args;
            }
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);
            const SyTaskOrFunc *getTaskOrFunc() const {
                return taskOrFunc;
            }    
            SyTaskOrFunc *getTaskOrFunc() {
                return taskOrFunc;
            }    

            void setTaskOrFunc(const SyTaskOrFunc *tf) {
                taskOrFunc = const_cast<SyTaskOrFunc*>(tf);
            }    

            const std::string& getName() const {
                return name;
            }    

            virtual SyTaskOrFuncCall *copy() const;
            //const SyList<SyStmt*>& getElabStmts() const;
        private:
            //void elaborate();

            std::string name;
            SyTaskOrFunc *taskOrFunc;
            std::vector<SyExpr*> args;
            //SyList<SyStmt*> elabStmts;
    };

    class SyTaskCall: public SyTaskOrFuncCall
    {
        public:
            SyTaskCall(const SyTask*);
            SyTaskCall(const std::string&);
            virtual ~SyTaskCall(); 
            const SyTask* getTask() const; 
            virtual SyTaskCall* copy() const;
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);

            void setTask(const SyTask* task);
    };

    class SyCompAssign : public SyStmt
    {
        public:
            SyCompAssign(const SyExpr *l): SyStmt(),
            lexpr(const_cast<SyExpr*>(l)){
            }

            virtual ~SyCompAssign() {
            }
            const SyExpr* getLExpr() const {
                return lexpr;
            }
            const SyList<std::pair<SyEventCtrlList, SyExpr*> > &
                getEventExprs() const {
                    return eventExprs;
                }
            void addEventCtrlExpr(const SyEventCtrlList& eclist,
                    const SyExpr* expr)
            {
                eventExprs.push_back(std::make_pair(
                            const_cast<SyEventCtrlList&>(eclist), 
                            const_cast<SyExpr*>(expr)));
            }
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);
        private:
            SyExpr *lexpr;
            SyList<std::pair<SyEventCtrlList, SyExpr*> >eventExprs;
    };

    class SyBlockCompAssign: public SyCompAssign
    {
        public:
            SyBlockCompAssign(const SyExpr *l): SyCompAssign(l){
            }
            virtual ~SyBlockCompAssign(){
            }
            virtual SyBlockCompAssign* copy() const;
            virtual void accept(SyVisitorBase*) const;
    };

    class SyNonBlockCompAssign: public SyCompAssign
    {
        public:
            SyNonBlockCompAssign(const SyExpr *l): SyCompAssign(l){
            }
            virtual ~SyNonBlockCompAssign(){
            }
            virtual SyNonBlockCompAssign* copy() const;
            virtual void accept(SyVisitorBase*) const;
    };
    SyEventStmt* Copy(const SyEventStmt*);

}
#endif
