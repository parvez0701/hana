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

#include "namedblock.h"
#include "blockdecl.h"
#include "vecnet.h"
#include "vecreg.h"
#include "vecint.h"
#include "vectime.h"
#include "vecreal.h"
#include "vecrealtime.h"
#include "omvisitor.h"

using std::vector;
using std::string;
namespace Rtl
{
    template<class T> void
    AddDeclItem(const vector<T*>& declItems, Scope *sc)
    {
        string name;
        for(unsigned int i = 0; i < declItems.size(); ++i)
        {
            const ScalarCntv *cntv = declItems[i];
            if(cntv->isVecBit())
            {
                if(name == cntv->getIxName().getName()){
                    continue;
                }
                name = cntv->getIxName().getName();
                const VecCntv *vcntv = cntv->getVecCntv();
                sc->addDeclItem(vcntv->copy());
            }
            else {
                sc->addDeclItem(cntv->copy());
            }
        }    
    }
    void
    NamedBlock::accept(OMVisitorBase* v) const
    {
        v->visit(this);
    }

    NamedBlock*
    NamedBlock::copy() const
    {
        NamedBlock *namedBlock = new NamedBlock(this->getName());

        AddDeclItem<Reg>(this->getRegs(), namedBlock);
        AddDeclItem<Integer>(this->getIntegers(), namedBlock);
        AddDeclItem<Time>(this->getTimes(), namedBlock);
        AddDeclItem<Real>(this->getReals(), namedBlock);
        AddDeclItem<RealTime>(this->getRealTimes(), namedBlock);

        const vector<Stmt*>& stmts = this->getStmts();
        for(unsigned int i = 0; i < stmts.size(); ++i) {
            namedBlock->addStmt(stmts[i]->copy());
        }    

        return namedBlock;
    }

}
