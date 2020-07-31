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

#include "gennotmap.h"
#include "sbase.h"
#include "scellmgr.h"

#include "sycontainer.h"
#include "syixname.h"


using namespace stview;

using std::make_pair;
using std::string;

using Synth::SyModInst;
using Synth::SyIxName;

namespace GenericMapping
{
    struct NotPredicate : public InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == string("NOT_1_1");
        }    
    };
    GenericNotMapper::GenericNotMapper() {
    }

    GenericNotMapper::GenericNotMapper(SBlock *block) :
        GenericMapperBase(block) {
    }

    GenericNotMapper::~GenericNotMapper() {
    }

    GenericNotMapper*
    GenericNotMapper::create(SBlock *block) const{
        return new GenericNotMapper(block);
    }    

    const SCell*
    GenericNotMapper::getCell() const 
    {
        const SCell *cell = SCellMgr::instance()->getGenericCell(SCell::NOT, 1);
        assert(cell);
        return cell;
    }    

    void
    GenericNotMapper::setPinMapList()
    {
        PinMapList pinMapList;
        pinMapList.push_back(make_pair("IN", "in"));
        pinMapList.push_back(make_pair("OUT", "out"));
        GenericMapperBase::setPinMapList(pinMapList);
    }

    void
    GenericNotMapper::doMapping() {
        this->createCellInstances(NotPredicate());
    }    
}
