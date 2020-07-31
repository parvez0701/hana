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

#include <list>

#include "genalmap.h"
#include "sblock.h"
#include "scellmgr.h"
#include "sinst.h"
#include "snet.h"

#include "sycontainer.h"
#include "syroot.h"

#include "namemgr.h"

using std::string;
using std::vector;
using std::list;
using std::make_pair;

using namespace stview;
using namespace Synth;

namespace GenericMapping
{

    GenericALElementMapper::GenericALElementMapper() : 
        inType(UNSET), cellType(SCell::CELLTYPE_BEGIN)
    {
    }

    GenericALElementMapper::GenericALElementMapper(SBlock *block) :
        GenericMapperBase(block), 
        inType(UNSET), 
        cellType(SCell::CELLTYPE_BEGIN) {
    }

    GenericALElementMapper::~GenericALElementMapper() {
    }

    void
    GenericALElementMapper::setPinMapList(PinMapList& pinMapList)
    {
        int size = (int) this->getInputType();
        //seprate loops to keep inputs/outputs in order
        if(size == 1)
        {
            pinMapList.push_back(make_pair(SyIxName("A"), "in1"));
            pinMapList.push_back(make_pair(SyIxName("B"), "in2"));
        }
        else
        {
            for(int i = 0; i < size; ++i)
            {
                SyIxName old("A", i);
                string new_(this->getMapName(SyIxName("in1", i)));
                pinMapList.push_back(make_pair(old, new_));
            }
            for(int i = 0; i < size; ++i)
            {
                SyIxName old("B", i);
                string new_(this->getMapName(SyIxName("in2", i)));
                pinMapList.push_back(make_pair(old, new_));
            }
        }    
/*
        if(size == 1) {
            pinMapList.push_back(make_pair(SyIxName("C"), "out"));
        } 
        else {
            for(int i = 0; i < size; ++i)
            {
                SyIxName old("C", i);
                string new_(this->getMapName(SyIxName("out", i)));
                pinMapList.push_back(make_pair(old, new_));
            }
        }    
*/
    }

    GenericALElementMapper::InputType
    GenericALElementMapper::getInputType() const {
        return inType;
    }

    SCell::CellType
    GenericALElementMapper::getCellType() const {
        return cellType;
    }
    void
    GenericALElementMapper::setInputType(InputType type) {
        inType = type;
    }

    void
    GenericALElementMapper::setCellType(SCell::CellType type) {
        cellType = type;
    }    


    void
    GenericALElementMapper::nextInState() const
    {
        switch(inType)
        {
            case UNSET:
                inType = IN_1;
                break;
            case IN_1:    
                inType = IN_2;
                break;
            case IN_2:    
                inType = IN_4;
                break;
            case IN_4:    
                inType = IN_8;
                break;
            case IN_8:    
                inType = IN_16;
                break;
            case IN_16:    
                inType = IN_32;
                break;
            case IN_32:    
                inType = IN_64;
                break;
            case IN_64:    
                inType = UNSET;
                break;
            default:    
                inType = UNSET;
        }
    }

    void
    GenericALElementMapper::prevInState() const 
    {
        switch(inType)
        {
            case UNSET:
                inType = IN_64;
                break;
            case IN_1:    
                inType = UNSET;
                break;
            case IN_2:    
                inType = IN_1;
                break;
            case IN_4:    
                inType = IN_2;
                break;
            case IN_8:    
                inType = IN_4;
                break;
            case IN_16:    
                inType = IN_8;
                break;
            case IN_32:    
                inType = IN_16;
                break;
            case IN_64:    
                inType = IN_32;
                break;
            default:    
                inType = UNSET;
        }
    }

    bool
    GenericALElementMapper::isUnsetState() const {
        return inType == UNSET;
    }

    vector<GenericALElementMapper::InputType>
    GenericALElementMapper::getCellTypeVec(int insize) const 
    {
        //for ADD, SUB and CMP
        vector<InputType> inTypeVec;
        inType = IN_64;

        while(inType != UNSET)
        {
            int tmp = insize;
            while(tmp/(int)inType)
            {
                inTypeVec.push_back(inType);
                tmp -= (int)inType;
            }
            insize = tmp;
            this->prevInState();
        }
        return inTypeVec;
    }

    int
    GenericALElementMapper::getInputSize(const SyModInst *inst,
        const string& pinName) const
    {
        //does not include special pins like CIN
        const SyList<SyScPin*>& pins = inst->getPins();
        int count = 0;

        for(SyNode<SyScPin*>* node = pins.head; node; node = node->next)
        {
            SyScPin *pin = node->val;
            if(pin->getDir() != SyScPort::IN) {
                continue;
            }

            string name = pin->getIxName().getName();
            if((name != pinName)/* && (name != "B")*/) {
                continue;
            }
            ++count;
        }
        return count;
    }

    vector<SyScPin*> 
    GenericALElementMapper::getPins(const SyModInst *minst, 
        const string& name) const
    {
        vector<SyScPin*> pins;
        const SyList<SyScPin*>& mpins = minst->getPins();

        for(SyNode<SyScPin*>* node = mpins.head; node; node = node->next)
        {
            if(node->val->getIxName().getName() == name) {
                pins.push_back(node->val);
            }
        }
        return pins;
    }

    vector<SPin*> 
    GenericALElementMapper::getPins(const SInst *inst, 
        const string& name) const
    {
        const list<SPin*>& spins = inst->getPins();
        vector<SPin*> pins;
        for(list<SPin*>::const_iterator iter = spins.begin(); iter != spins.end();
            ++iter) {
            if((*iter)->getVecName() == name) {
                pins.push_back(*iter);
            }    
        }
        return pins;
    }

}
