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

#ifndef _ASSIGNSTMTSTRANS_H_
#define _ASSIGNSTMTSTRANS_H_

#include "syvisitorbase.h"
namespace Synth
{
    class SyAssignStmtsTrans : public SyVisitorBase
    {
        public:
            SyAssignStmtsTrans(SyModule*);
            virtual ~SyAssignStmtsTrans();
            void visit(SyList<SyStmt*>&);

            virtual void visit(const SyNonBlockAssign*);
            virtual void visit(const SyBlockAssign*);
            void transformAssign(SyAssign*);
            virtual const std::string& getCurrModName() const {
                return module->getName();
            }    
        private:
            SyAssignStmtsTrans(const SyAssignStmtsTrans&);
            SyAssignStmtsTrans& operator=(const SyAssignStmtsTrans&);

            bool checkVariableRead(const SyCntv*) const;
            bool checkVariableRead(const SyCntv*, const SyAssign*) const;
            bool allBlockAssign(const SyList<SyStmt*>&) const;

            SyList<SyStmt*> newStmts;
            SyModule *module;
            SyNode<SyStmt*>* currNode;
    };
}
#endif
