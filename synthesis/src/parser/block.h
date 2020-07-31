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

#ifndef _BLOCK_H_
#define _BLOCK_H_
#include <vector>
#include "stmt.h"
namespace Rtl
{
    class Block: public Stmt
    {
        public:
            Block(): Stmt() {
            }

            virtual ~Block(){
            }

            void addStmt(const Stmt* st){
                stmts.push_back(const_cast<Stmt*>(st));
            }
            const std::vector<Stmt*>& getStmts() const {
                return stmts;
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual Block* copy() const{
                const int size = stmts.size();
                Block* cblock = new Block();
                for(int i = 0; i < size; ++i){
                    cblock->addStmt(stmts[i]->copy());
                }
                return cblock;
            }

            virtual void repExpr(const std::string& str, const Expr* expr)
            {
                for(unsigned int i = 0; i < stmts.size(); ++i) {
                    stmts[i]->repExpr(str, expr);
                }
                //return this;
            }
            
        private:
            std::vector<Stmt*> stmts;
    };
    
}
#endif
