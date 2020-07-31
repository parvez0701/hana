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

#ifndef _SOPMGR_H_
#define _SOPMGR_H_

#include <map>

#include "scell.h"

namespace stview
{
    class SRoot;
    class SInst;
    class SBlock;
    class SPN;
    class SNode;
    typedef SNode SExpr;
}
namespace SOP
{  
    class SopTransBase;
    class SopMgr
    {
        public:
            static SopMgr* instance();
            ~SopMgr();
            void toSumOfProductForm();
        private:    
            typedef std::map<stview::SCell::CellType, SopTransBase*> SopTransTable;
            SopMgr();
            SopMgr(const SopMgr&);
            SopMgr& operator=(const SopMgr&);

            //private functions
            void toSumOfProductForm(stview::SBlock*);
            stview::SExpr* toSumOfProductForm(const stview::SInst*);

            SopTransBase* getTransformer(const stview::SInst*);
            void createAssign(stview::SPN*);

            //data
            static SopMgr *m_sopMgr;
            stview::SBlock *m_currBlock;
            SopTransTable m_sopTransTable;
    };
}
#endif
