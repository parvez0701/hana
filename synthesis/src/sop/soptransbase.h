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

#ifndef _SOPTRANSBASE_H_
#define _SOPTRANSBASE_H_

#include <map>

#include "sblock.h"
#include "sinst.h"
#include "sexprtree.h"
namespace stview
{
    class SPin;
}
namespace SOP
{
    class SopTransBase
    {
        public:
            typedef stview::SPin SPin;
            typedef stview::SBlock SBlock;
            typedef stview::SInst SInst;
            typedef stview::SExpr SExpr;
            typedef stview::SExprTree SExprTree;
            
            typedef std::map<const SPin*, SExpr*> SPinExprMap;

        public:
            SopTransBase()  { }

            virtual ~SopTransBase() { }
            virtual void transform(const SInst*) = 0;
            virtual SopTransBase* copy() const = 0 ;

            void init() { m_spinExprMap.clear(); }

            SExpr* getExpr(const SPin *spin) 
            {
                SPinExprMap::iterator iter = m_spinExprMap.find(spin);
                if(iter == m_spinExprMap.end()) {
                    return 0;
                }
                return iter->second;
            }

            void addExpr(const SPin* spin, SExpr* expr) { 
                m_spinExprMap.insert(std::make_pair(spin, expr)); 
            }

            void clear() { m_spinExprMap.clear();}

        private:
            SPinExprMap m_spinExprMap;
    };
}
#endif

