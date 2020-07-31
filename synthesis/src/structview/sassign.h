/* 
Copyright (C) 2009-2011 Parvez Ahmad
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

#ifndef _SASSIGN_H_
#define _SASSIGN_H_

#include <string>

namespace stview
{
    class SPN;
    class SNode;
    
    class SAssign
    {
        public:
            SAssign(const SPN *lhs, const SNode *rhs) :
                m_lhs(const_cast<SPN*>(lhs)), 
                m_rhs(const_cast<SNode*>(rhs)) {
            }

            ~SAssign() { }
            const SPN* getLVal() const { return m_lhs; }
            SPN* getLVal() { return m_lhs; }

            const SNode* getRExpr() const { return m_rhs; }
            SNode* getRExpr() { return m_rhs; }
            
        private:
            SAssign(const SAssign&);
            SAssign& operator=(const SAssign&);
            
            SPN *m_lhs;
            SNode *m_rhs;
    };
}
#endif
