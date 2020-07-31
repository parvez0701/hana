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

#include "technxnxmap.h"
#include "techmapmgr.h"

#include "sinst.h"
#include "sblock.h"
#include "spin.h"
#include "sutils2.h"

using std::list;

using namespace stview;

namespace TechnologyMapping
{
    TechNXNXMap::TechNXNXMap(SCell::CellType type, bool m) : 
        TechAONXNBase(type),
        merge(m) {
    }

    TechNXNXMap::~TechNXNXMap() {
    }

    bool
    TechNXNXMap::condition(const SInst *inst) const
    {
        if(merge) {
            return TechAONXNBase::condition(inst);
        }

        return this->nonMergeCondition(inst);
    }

    bool
    TechNXNXMap::generalCondition(const SInst *inst) const
    {
        if(inst->getCellType() != this->getType()) {
            return false;
        }

        if(!inst->isGeneric()) {
            return false;
        }
        return true;
    }

    bool
    TechNXNXMap::doMapping(SBlock *block, SInst *inst)
    {
        if(merge) {
            return TechAONXNBase::doMapping(block, inst);
        }
        return doNonMergeMapping(block, inst);
    }

    bool
    TechNXNXMap::nonMergeCondition(const SInst *inst) const
    {
        if(this->getType() != inst->getCellType()) {
            return false;
        }

        if(!inst->getCell()->isGeneric()) {
            return false;
        }

        return TechMapMgr::instance()->cellExists(inst->getCellType(),
            inst->getInPins().size());
    }        

    bool
    TechNXNXMap::doNonMergeMapping(SBlock *block, SInst *inst)
    {
        const list<SPin*>& inpins = inst->getInPins();
        SInst *newinst = CreateInstance(block, this->getType(),
            this->getNameHint(), inpins.size(), false);

        newinst->getPin("out")->setActual(inst->getPin("out")->getActual());
        const list<SPin*>& newinpins = newinst->getInPins();

        list<SPin*>::const_iterator oldpin = inpins.begin();
        list<SPin*>::const_iterator newpin = newinpins.begin();

        for(; (oldpin != inpins.end()) && (newpin != newinpins.end());
            ++oldpin, ++newpin) {
            (*newpin)->setActual((*oldpin)->getActual());
        }
        block->deleteCellInst(inst);
        return true;
    }
}
