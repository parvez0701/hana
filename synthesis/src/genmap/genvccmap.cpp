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

#include "genvccmap.h"
#include "sbase.h"
#include "scellmgr.h"

#include "sycontainer.h"
using namespace stview;

using std::make_pair;
using std::string;

using Synth::SyModInst;
using Synth::SyIxName;

namespace GenericMapping
{
    struct VccPredicate : public InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == string("VCC_0_1");
        }    
    };
    GenericVccMapper::GenericVccMapper() {
    }

    GenericVccMapper::GenericVccMapper(SBlock *block) :
        GenericMapperBase(block) {
    }

    GenericVccMapper::~GenericVccMapper() {
    }

    GenericVccMapper*
    GenericVccMapper::create(SBlock *block) const{
        return new GenericVccMapper(block);
    }    

    const SCell*
    GenericVccMapper::getCell() const 
    {
        const SCell *cell = SCellMgr::instance()->getGenericCell(SCell::VCC, 0);
        assert(cell);
        return cell;
    }    

    void
    GenericVccMapper::setPinMapList()
    {
        PinMapList pinMapList;
        pinMapList.push_back(make_pair("OUT", "out"));
        GenericMapperBase::setPinMapList(pinMapList);
    }
    
    void
    GenericVccMapper::doMapping() {
        this->createCellInstances(VccPredicate());
    }    
}
