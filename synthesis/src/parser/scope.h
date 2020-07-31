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

#ifndef _SCOPE_H_
#define _SCOPE_H_
#include <string>
#include <map>
#include "base.h"
#include "utils.h"
#include "blockdecl.h"
namespace Rtl
{
    class Stmt;
    class Scope: virtual public Base
    {
        friend class OMCreator;
        friend class Elaborator;
        public:
            Scope(const std::string& n);
            virtual ~Scope();
            Base* addObject(const std::string& n,
                const Base* b, const bool r);
            void removeObject(const std::string& n){
                symbolTable.erase(n);
            }

            Base* getObject(const std::string& n) const;
            const std::string& getName() const {
                return name;
            }
            void addDeclItem(Base*);

            const std::vector<Reg*>& getRegs() const{
                return blockdecl->regs;
            }

            const std::vector<Integer*>& getIntegers() const{
                return blockdecl->ints;
            }

            const std::vector<Time*>& getTimes() const {
                return blockdecl->times;
            }

            const std::vector<Real*>& getReals() const {
                return blockdecl->reals;
            }

            const std::vector<RealTime*>& getRealTimes() const {
                return blockdecl->realts;
            }

            const std::vector<EventVar*>& getEventVars() const {
                return blockdecl->evars;
            }    

            virtual void addStmt(const Stmt* st) {
                stmts.push_back(const_cast<Stmt*>(st));
            }

            const std::vector<Stmt*>& getStmts() const {
                return stmts;
            }
            void acceptOMVisitor(OMVisitorBase*) const;
            virtual void accept(OMVisitorBase *v ) const{
            }

            void repExprInStmts(const std::string&, const Expr*);

        private:
            Scope();
            //Copy constructor and assignment operators are disabled.
            Scope(const Scope&);
            const Scope& operator=(const Scope&);
            void setBlockDecl(const BlockDecl* bd){
                blockdecl = const_cast<BlockDecl*>(bd);
            }

            std::string name;
            BlockDecl* blockdecl;
            std::vector<Stmt*> stmts;
            std::map<std::string, Base*, compare_str> symbolTable;
    };

    template <typename T1, typename T2> std::vector<T2*>
    GetVSObjects(const Scope* sc, const std::vector<T1*>& objects)
    {
        std::vector<T2*> ret;
        std::vector<std::string> names = GetNames<T1>(objects);
        const int size = names.size();
        for(int i = 0; i < size; ++i)
        {
            const Base* base = sc->getObject(names[i]);
            if(const T2* t2 = dynamic_cast<const T2*>(base)){
                ret.push_back(const_cast<T2*>(t2));
            }
        }
        return ret;
    }

}
#endif
