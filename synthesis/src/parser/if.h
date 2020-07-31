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

#ifndef _IF_H_
#define _IF_H_

#include "stmtcont.h"

namespace Rtl
{
    class If : public StmtCont
    {
        friend class SemanticChecker;
        public:
            If(const Expr* ce, const Stmt* ts, const Stmt* es)
                :StmtCont(ts),
                condExpr(const_cast<Expr*>(ce)),
                elseStmt(const_cast<Stmt*>(es)){
            }

            virtual ~If(){
            }

            const Expr* getCondExpr() const{
                return condExpr;
            }

            const Stmt* getElsePart() const{
                return elseStmt;
            }

            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual If* copy() const {
                return new If(condExpr->copy(), this->getStmt()->copy(),
                    elseStmt ? elseStmt->copy() : 0);
            }        

            virtual void repExpr(const std::string& str, const Expr* expr)
            {
                StmtCont::repExpr(str, expr);
                condExpr = const_cast<Expr*>(condExpr->repExpr(str, expr));
                if(elseStmt) {
                    elseStmt->repExpr(str, expr);
                }
                //return this;
            }
        private:
            void setCondExpr(const Expr *cond) {
                condExpr = const_cast<Expr*>(cond);
            }    
            Expr* condExpr;
            Stmt* elseStmt;
    };
}
#endif
