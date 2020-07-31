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

#ifndef _COMPASSIGN_H_
#define _COMPASSIGN_H_

#include "stmt.h"
#include "event.h"

namespace Rtl
{
    class CompAssign: public Stmt
    {
        public:
            CompAssign(const Expr* le, const Expr* re, 
                const EventCntrl* ec, const Expr* ee,
                const AssignType t): Stmt(),
                lexpr(const_cast<Expr*>(le)),
                repeatExpr(const_cast<Expr*>(re)),
                eventCtrl(const_cast<EventCntrl*>(ec)),
                eventCtrlExpr(const_cast<Expr*>(ee)),
                type(t){
            }

            virtual ~CompAssign(){
            }

            const Expr* getLhs() const{
                return lexpr;
            }
            const Expr* getRepeatExpr() const{
                return repeatExpr;
            }
            const EventCntrl* getEventCtrl() const{
                return eventCtrl;
            }
            const Expr* getEventCtrlExpr() const{
                return eventCtrlExpr;
            }

            const AssignType getAssignType() const{
                return type;
            }

            void accept(OMVisitorBase* v) const{
                v->visit(this);
            }

            virtual CompAssign* copy() const {
                Expr* clexpr = lexpr->copy();
                Expr* crepExpr = repeatExpr->copy();
                EventCntrl* ceventCntrl = eventCtrl->copy();
                Expr* ceventCntrlExpr = eventCtrlExpr->copy();
                return new CompAssign(clexpr, crepExpr, ceventCntrl,
                        ceventCntrlExpr, type);
            }

            virtual void repExpr(const std::string& str,
                                        const Expr* expr)
            {
                lexpr = const_cast<Expr*>(lexpr->repExpr(str, expr));
                repeatExpr = const_cast<Expr*>(repeatExpr->repExpr(str, expr));
                eventCtrl->repExpr(str, expr);
                eventCtrlExpr = const_cast<Expr*>(eventCtrlExpr->repExpr(str, expr));
                //return this;
            }

        private:
            Expr* lexpr;
            Expr* repeatExpr;
            EventCntrl* eventCtrl;
            Expr* eventCtrlExpr;
            AssignType type;
    };
}
#endif
