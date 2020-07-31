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

#ifndef _LOOPSTMTS_H_
#define _LOOPSTMTS_H_

#include "stmtcont.h"
#include "omvisitor.h"

namespace Rtl
{
    class Forever: public StmtCont
    {
        public:
            Forever(const Stmt* st): StmtCont(st){
                
            }

            virtual ~Forever(){
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }

            virtual Forever* copy() const {
                return new Forever(this->getStmt()->copy());
            }    

            virtual void repExpr(const std::string& str, const Expr* expr) {
                StmtCont::repExpr(str, expr);
                //return dynamic_cast<Forever*>(StmtCont::repExpr(str, expr));
            }    
    };

    class Repeat: public StmtCont
    {
        friend class SemanticChecker;
        public:
            Repeat(const Expr* e, const Stmt* s): StmtCont(s),
                expr(const_cast<Expr*>(e)){
            }
            virtual ~Repeat(){
            }

            const Expr* getExpr() const {
                return expr;
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual Repeat* copy() const {
                return new Repeat(expr->copy(), this->getStmt()->copy());
            }

            virtual void repExpr(const std::string& str, const Expr *e)
            {
                expr = const_cast<Expr*>(expr->repExpr(str, e));
                StmtCont::repExpr(str, expr);
                //return dynamic_cast<Repeat*>(StmtCont::repExpr(str, e));
            }

        private:
            void setExpr(const Expr* e) {
                expr = const_cast<Expr*>(e);
            }    
            Expr* expr;
    };

    class For : public StmtCont
    {
        friend class SemanticChecker;
        public:
            For(const Expr* lv, const Expr* ie, const Expr* te, 
                const Expr* ae, const Stmt* s): StmtCont(s),
                loopvar(const_cast<Expr*>(lv)),
                initExpr(const_cast<Expr*>(ie)),
                termExpr(const_cast<Expr*>(te)),
                assignExpr(const_cast<Expr*>(ae)){
            }

            virtual ~For(){
            }
            const Expr* getLoopVar()const{
                return loopvar;
            }
            const Expr* getInitExpr() const{
                return initExpr;
            }
            
            const Expr* getTermExpr() const{
                return termExpr;
            }
            const Expr* getAssignExpr() const{
                return assignExpr;
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual For* copy() const {
                return new For(loopvar->copy(), initExpr->copy(),
                    termExpr->copy(), assignExpr->copy(),
                    this->getStmt()->copy());
            }        
            virtual void repExpr(const std::string& str, const Expr* expr)
            {
                loopvar = const_cast<Expr*>(loopvar->repExpr(str, expr));
                initExpr = const_cast<Expr*>(initExpr->repExpr(str, expr));
                termExpr = const_cast<Expr*>(termExpr->repExpr(str, expr));
                assignExpr = const_cast<Expr*>(assignExpr->repExpr(str, expr));
                StmtCont::repExpr(str, expr);
                //return dynamic_cast<For*>(StmtCont::repExpr(str, expr));
            }    
        private:
            void setInitExpr(const Expr* ie) {
                initExpr = const_cast<Expr*>(ie);
            }

            void setTermExpr(const Expr *te) {
                termExpr = const_cast<Expr*>(te);
            }

            void setAssignExpr(const Expr *ae) {
                assignExpr = const_cast<Expr*>(ae);
            }    
            Expr* loopvar;
            Expr* initExpr;
            Expr* termExpr;
            Expr* assignExpr;
    };
    class While: public StmtCont
    {
        friend class SemanticChecker;
        public:
            While(const Expr* e, const Stmt* s): StmtCont(s),
                expr(const_cast<Expr*>(e)){
            }
            virtual ~While(){
            }

            const Expr* getExpr() const {
                return expr;
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }

            virtual While* copy() const {
                return new While(expr->copy(), this->getStmt()->copy());
            }    

            virtual void repExpr(const std::string& str, const Expr *e)
            {
                expr = const_cast<Expr*>(expr->repExpr(str, e));
                StmtCont::repExpr(str, e);
                //return dynamic_cast<While*>(StmtCont::repExpr(str, e));
            }

        private:
            void setExpr(const Expr *e) {
                expr = const_cast<Expr*>(e);
            }

            Expr* expr;
    };
}

#endif
