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

#ifndef _INITIAL_H_
#define _INITIAL_H_

#include "stmtcont.h"
namespace Rtl
{
    class Initial: public StmtCont
    {
        public:
            Initial(const Stmt* st): StmtCont(st) {
            }

            virtual ~Initial(){
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }

            virtual Initial* copy() const {
                return new Initial(this->getStmt()->copy());
            }

            virtual void repExpr(const std::string& str, const Expr *expr)
            {
                StmtCont::repExpr(str, expr);
                //return static_cast<Initial*>(StmtCont::repExpr(str, expr));
            }
    };
}
#endif
