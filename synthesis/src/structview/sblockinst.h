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

#ifndef _SBLOCKINST_H_
#define _SBLOCKINST_H_
#include <list>
#include "sinstbase.h"

namespace stview
{
    class SBlock;
    class SBlockInst: public SInstBase
    {
        public:
            SBlockInst(const std::string&, const SBlock*);
            virtual ~SBlockInst();
            inline const SBlock* getBlock() const;
            inline SBlock* getBlock();
            inline void setBlock(const SBlock* blk); 
            virtual void accept(NetlistVisitorBase*);
            virtual SPort::Dir getDir(const std::string&) const;
        private:
            SBlock *block;
    };

    const SBlock*
    SBlockInst::getBlock() const {
        return block;
    }

    SBlock*
    SBlockInst::getBlock(){
        return block;
    }
    void
    SBlockInst::setBlock(const SBlock* blk) {
        block = const_cast<SBlock*>(blk);
    }    

    typedef ResetVisitCount<SBlockInst> ResetVisitCountBlockInst;

}
#endif
