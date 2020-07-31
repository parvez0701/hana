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

#include <math.h>

#include "gendecmap.h"

#include "sblock.h"
#include "scellmgr.h"
#include "sinst.h"

#include "sycontainer.h"
#include "syroot.h"

using namespace stview;
using namespace Synth;

using std::make_pair;
using std::pair;
using std::string;
using std::reverse;


namespace GenericMapping
{
    static int GetOutCount(const SyModInst*);

    struct Predicate : public InstancePredicate
    {
        Predicate(const string& n) : name (n) { }

        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == name;
        }    

        string name;
    };

    struct DecoderGreaterThanFive
    {
        bool operator()(const SyModInst *inst) const
        {
            const string& master = inst->getMasterName();
            if(master.substr(0, 3) != "DEC") {
                return false;
            }
            return GetOutCount(inst) > 16;
        }
    };

    GenericDecoderMapper::GenericDecoderMapper() {
    }

    GenericDecoderMapper::GenericDecoderMapper(SBlock *block) :
        GenericMapperBase(block), type(UNSET) {
    }

    GenericDecoderMapper::~GenericDecoderMapper() {
    }

    GenericDecoderMapper*
    GenericDecoderMapper::create(SBlock *block) const{
        return new GenericDecoderMapper(block);
    }    

    const SCell*
    GenericDecoderMapper::getCell() const
    {
        assert(type != UNSET);
        const SCell *cell = SCellMgr::instance()->getGenericCell(SCell::DEC, (short) type + 1); //add one for enable pin
        assert(cell);
        return cell;
    }

    void
    GenericDecoderMapper::setPinMapList()
    {
        assert(type != UNSET);
        
        PinMapList pinMapList;
        int inputs = (int)type;
        int outputs = pow(2, inputs);
        if(inputs == 1) {
            pinMapList.push_back(make_pair("IN", "in"));
        }
        else {
            for(int i = 0; i < inputs; ++i) {
                pinMapList.push_back(make_pair(
                    SyIxName("IN", i), this->getMapName(SyIxName("in", i))));
            }
        }    
        pinMapList.push_back(make_pair(SyIxName("EN"), "enable"));
        for(int i = 0; i < outputs; ++i) {
            pinMapList.push_back(make_pair(
                SyIxName("OUT", i), this->getMapName(SyIxName("out", i))));
        }
        GenericMapperBase::setPinMapList(pinMapList);

    }

    void
    GenericDecoderMapper::doMapping()
    {
        type = ONE_IN;
        this->createCellInstances(Predicate("DEC_1_2"));
        type = TWO_IN;
        this->createCellInstances(Predicate("DEC_2_4"));
        type = THREE_IN;
        this->createCellInstances(Predicate("DEC_3_8"));
        type = FOUR_IN;
        this->createCellInstances(Predicate("DEC_4_16"));
        type = FIVE_IN;
        this->createCellInstances(Predicate("DEC_5_32"));

        //start generating decoders on the fly
        this->createDecoderLogic();
    }


    int
    GenericDecoderMapper::getOutCount(int actualCount) const
    {
        if(actualCount == 2) {
            return 2;
        }    
        int count = 1;
        while(actualCount) 
        {
            actualCount = actualCount >> 1;
            count = count << 1;
        }
        return count;
    }

    void
    GenericDecoderMapper::createDecoderLogic()
    {
        SBlock *block = this->getBlock();
        const SyModule *mod = SyRoot::instance()->getModule(block->getName());
        const SyList<SyModInst*>& minsts = mod->getModInsts();
        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next)
        {
            if(!DecoderGreaterThanFive()(node->val)) {
                continue;
            }
            this->createDecoderLogic(node->val);
        }
    }

    void
    GenericDecoderMapper::createDecoderLogic(const SyModInst *inst)
    {
        const SyList<SyScPin*> pins = inst->getPins();
        InAndInvVec inAndInvVec = this->createInputAndInvPairs(inst);
        
        int out = 0;
        for(SyNode<SyScPin*>* node = pins.head; node; node = node->next)
        {
            SyScPin *pin = node->val;

            if(pin->getDir() != SyScPort::OUT) {
                continue;
            }

            const SPN *outCntv = this->getMapCntv(pin->getActual()); 
            InputVec inVec = this->createInputs(inAndInvVec, out);
            this->create2InputGateTree("And2", SCell::AND, inVec, outCntv);
            ++out;
        }
    }
    
    GenericDecoderMapper::InAndInvVec
    GenericDecoderMapper::createInputAndInvPairs(const SyModInst *inst) 
    {
        const SyList<SyScPin*>& pins = inst->getPins();
        InAndInvVec inInvPairs;

        for(SyNode<SyScPin*>* node = pins.head; node; node = node->next)
        {
            SyScPin *pin = node->val;
            if(pin->getDir() != SyScPort::IN) {
                continue;
            }

            if(pin->getIxName().getName() == "EN") {
                //ignore enable
                continue;
            }
            SInst *inv = this->createInverter("Not", 
                this->getMapCntv(pin->getActual()), 0);
            inInvPairs.push_back(make_pair(
                const_cast<SPN*>(this->getMapCntv(pin->getActual())), 
                const_cast<SPN*>(inv->getPin("out")->getActual())));
        }       
        reverse(inInvPairs.begin(), inInvPairs.end()); // ? to match bit order 
        return inInvPairs;
    }

    GenericDecoderMapper::InputVec
    GenericDecoderMapper::createInputs(const InAndInvVec& inAndInvVec, 
        int out) const
    {
        int tmp = 1;
        InputVec inVec;
        for(int i = 0; i < inAndInvVec.size(); ++i) 
        {
            if(out & tmp) {
                inVec.push_back(inAndInvVec[i].first);
            }
            else {
                inVec.push_back(inAndInvVec[i].second);
            }
            tmp = tmp << 1;
        }
        return inVec;
    }
    
    static int GetOutCount(const SyModInst *inst) 
    {
        const SyList<SyScPin*>& pins = inst->getPins();
        int count = 0;
        for(SyNode<SyScPin*>* node = pins.head; node; node = node->next){
            if(node->val->getDir() == SyScPort::OUT) {
                ++count;
            }
        }
        return count;
    }    
}
