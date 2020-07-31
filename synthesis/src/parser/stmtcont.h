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

#ifndef _STMTCONT_H_
#define _STMTCONT_H_

#include "stmt.h"

namespace Rtl
{
    class StmtCont: public Stmt
    {
        public:
            StmtCont(const Stmt* st): Stmt(), 
                stmt(const_cast<Stmt*>(st)){
            }

            virtual ~StmtCont(){
            }

            const Stmt* getStmt() const {
                return stmt;
            }
            void setStmt(const Stmt* st){
                stmt = const_cast<Stmt*>(st);
            }

            virtual void repExpr(    const std::string& str, 
                                    const Expr *expr)
            {
               // stmt = stmt->repExpr(str, expr);
                //return this;
                stmt->repExpr(str, expr);
            }    
        private:
            Stmt* stmt;
    };
}
#endif
