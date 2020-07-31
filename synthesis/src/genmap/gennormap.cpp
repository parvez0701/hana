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

#include "gennormap.h"

#include "sbase.h"
#include "scellmgr.h"

#include "sycontainer.h"
#include "syixname.h"

using std::make_pair;

using namespace stview;

using Synth::SyModInst;
using Synth::SyIxName;

namespace GenericMapping
{
    struct Nor2Predicate : public InstancePredicate
    {
        virtual bool operator()(const SyModInst* inst) const {
            return inst->getMasterName() == "NOR_2_1";
        }    
    };
    GenericNorMapper::GenericNorMapper() {
    }

    GenericNorMapper::GenericNorMapper(SBlock *block) :
        GenericMapperBase(block) {
    }

    GenericNorMapper::~GenericNorMapper() {
    }

    GenericNorMapper*
    GenericNorMapper::create(SBlock *block) const{
        return new GenericNorMapper(block);
    }    

    const SCell*
    GenericNorMapper::getCell() const 
    {
        const SCell *cell = SCellMgr::instance()->getGenericCell(SCell::NOR, 2);
        assert(cell);
        return cell;
    }
    void
    GenericNorMapper::setPinMapList()
    {
        PinMapList pinMapList;
        pinMapList.push_back(make_pair(SyIxName("IN", 0), 
            this->getMapName(SyIxName("in", 0))));
        pinMapList.push_back(make_pair(SyIxName("IN", 1), 
            this->getMapName(SyIxName("in", 1))));
        pinMapList.push_back(make_pair("OUT", "out"));
        GenericMapperBase::setPinMapList(pinMapList);
    }

    void
    GenericNorMapper::doMapping() {
        this->createCellInstances(Nor2Predicate());
    }    
}
