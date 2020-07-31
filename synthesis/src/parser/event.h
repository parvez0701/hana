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

#ifndef _EVENT_H_
#define _EVENT_H_

#include "stmtcont.h"
#include "expr.h"

namespace Rtl
{
    class EventCntrl: public StmtCont
    {
        friend class OMCreator;
        friend int yyparse();
        public:
            EventCntrl(const std::vector<std::pair<EdgeType,Expr*> >& e,
                const Stmt* st, const bool v2k): 
                StmtCont(const_cast<Stmt*>(st)),
                 eventExprs(e), 
                v2kstyle(v2k){
        }

        virtual ~EventCntrl(){
        }

        const std::vector<std::pair<EdgeType, Expr*> >& 
            getEventExprs() const {
            return eventExprs;
        }
        const bool isV2kStyle() const{
            return v2kstyle;
        }
        virtual void accept(OMVisitorBase* v) const{
            v->visit(this);
        }
        virtual EventCntrl* copy() const {
            std::vector<std::pair<EdgeType, Expr*> > ceventExprs;
            int size = eventExprs.size();
            for(int i = 0; i < size; ++i){
                ceventExprs.push_back(std::make_pair(eventExprs[i].first,
                            eventExprs[i].second->copy()));
            }
            return new EventCntrl(ceventExprs, this->getStmt()->copy(),
                    v2kstyle);
        }

        virtual void repExpr(const std::string& str,
                             const Expr *expr)
        {
            StmtCont::repExpr(str, expr);
            for(unsigned int i = 0; i < eventExprs.size(); ++i) {
                eventExprs[i].second = const_cast<Expr*>(
                    eventExprs[i].second->repExpr(str, expr));
            }
            //return this;
        }

        private:
            std::vector<std::pair<EdgeType, Expr*> > eventExprs;
            bool v2kstyle;
    };

    class Wait: public StmtCont
    {
        public:
            Wait(const Expr* ce, const Stmt* st):
                StmtCont(st), condexpr(const_cast<Expr*>(ce)){
            }

            virtual ~Wait(){
            }
            const Expr* getCondExpr() const{
                return condexpr;
            }

            virtual void accept(OMVisitorBase* v) const {
                v->visit(this);
            }
            virtual Wait* copy() const{
                return new Wait(condexpr->copy(), this->getStmt()->copy());
            }

            virtual void repExpr(const std::string& str, const Expr* expr)
            {
                StmtCont::repExpr(str, expr);
                condexpr = const_cast<Expr*>(condexpr->repExpr(str, expr));
                //return this;
            }
        private:
            Expr* condexpr;
    };

    class EventVar : public Expr
    {
        public:
            EventVar(const std::string& n) : name(n) {
            }
            virtual ~EventVar() {
            }
            const std::string& getName() const {
                return name;
            }    
            virtual EventVar* copy() const {
                return new EventVar(name);
            }    
            virtual void accept(OMVisitorBase* v ) const{
                v->visit(this);
            }
            virtual std::string toString() const {
                return name;
            }    

            virtual long len() const {
                return sizeof(char) * name.size();
            }    
            virtual const EventVar* repExpr(const std::string& str,
                                            const Expr *expr) {
                return this;
            }    
            virtual bool isSigned() const {
                return false;
            }    
        private:
            std::string    name;
    };

    class EventStmt: public Stmt
    {
        public:
            EventStmt(const EventVar* e): Stmt(), 
                evar(const_cast<EventVar*>(e)){
            }
            virtual ~EventStmt(){
            }

            //const Expr* getExpr() const {
            //    return expr;
            //}
            const EventVar* getVar() const {
                return evar;
            }    

            virtual void accept(OMVisitorBase* v ) const{
                v->visit(this);
            }
            virtual EventStmt* copy() const {
                return new EventStmt(evar->copy());
            }

            virtual void repExpr(    const std::string& str,
                                        const Expr* _expr)
            {
                //expr = const_cast<Expr*>(expr->repExpr(str, _expr));
                //return this;
            }
        private:
            EventVar *evar;
            //Expr* expr;
    };
}
#endif
