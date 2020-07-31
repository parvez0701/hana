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

#include "gengndmap.h"

#include "sblock.h"
#include "scellmgr.h"

#include "sycontainer.h"

using std::make_pair;
using namespace stview;

using Synth::SyModInst;

namespace GenericMapping
{
    struct GroundPredicate : public InstancePredicate
    {
        virtual bool operator() (const SyModInst *inst) const {
            return inst->getMasterName() == "GND_0_1";
        }    
    };
    GenericGndMapper::GenericGndMapper() {
    }

    GenericGndMapper::GenericGndMapper(SBlock *block) :
        GenericMapperBase(block) {
    }

    GenericGndMapper::~GenericGndMapper() {
    }

    GenericGndMapper*
    GenericGndMapper::create(SBlock *block) const{
        return new GenericGndMapper(block);
    }    

    void
    GenericGndMapper::setPinMapList()
    {
        PinMapList pinMapList;
        pinMapList.push_back(make_pair("OUT", "out"));
        GenericMapperBase::setPinMapList(pinMapList);
    }

    const SCell*
    GenericGndMapper::getCell() const {
        return SCellMgr::instance()->getGenericCell(SCell::GND, 0);
    }

    void
    GenericGndMapper::doMapping() {
        this->createCellInstances(GroundPredicate());
    }    
}
