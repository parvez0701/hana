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

#include "genmuxmap.h"
#include "sblock.h"
#include "sinst.h"
#include "scellmgr.h"
#include "snet.h"

#include "sycontainer.h"
#include "syroot.h"
using namespace stview;
using namespace Synth;

using std::vector;
using std::make_pair;
using std::pair;
using std::string;

namespace GenericMapping
{
    struct GeneralMuxPredicate : public InstancePredicate
    {
        GeneralMuxPredicate(const string& n) : name(n) { }

        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == name;
        }    

        string name;
    
    };

    struct MuxPredicate
    {
        bool operator()(const SyModInst *inst) const 
        {
            string name = inst->getMasterName();
            if(name.substr(0, 4) != "MUX_") {
                return false;
            }
            const SyList<SyScPin*>& pins = inst->getPins();
            int count = 0;
            for(SyNode<SyScPin*>* node = pins.head; node; node = node->next)
            {
                SyScPin *pin = node->val;
                if(pin->getDir() != SyScPort::IN) {
                    continue;
                }
                string name = pin->getIxName().getName();
                if(name != "SEL") {
                    continue;
                }
                ++count;
                if(count > 6) {
                    return true;
                }    
            }
            return false;
        }    
    };

    GenericMuxMapper::GenericMuxMapper() {
    }

    GenericMuxMapper::GenericMuxMapper(SBlock *block) :
        GenericMapperBase(block) {
    }

    GenericMuxMapper::~GenericMuxMapper() {
    }

    GenericMuxMapper*
    GenericMuxMapper::create(SBlock *block) const {
        return new GenericMuxMapper(block);
    }    

    pair<int, int>
    GenericMuxMapper::getInputSelectSize() const
    {
        assert(type != UNSET);
        switch (type) 
        {
            case MUX_1: {
                return make_pair(2, 1);
            }    
            case MUX_2: {
                return make_pair(4, 2);
            }    
            case MUX_3: {
                return make_pair(8, 3);
            }    
            case MUX_4: {
                return make_pair(16, 4);
            }    
            case MUX_5: {
                return make_pair(32, 5);
            }    
            case MUX_6: {
                return make_pair(64, 6);
            }    
            default: {
                assert(0);
            }    
        }
        return make_pair(-1, -1);
    }    

    const SCell*
    GenericMuxMapper::getCell() const
    {
        assert(type != UNSET);
        pair<int, int> inSel = this->getInputSelectSize();
        int input = inSel.first + inSel.second;
        const SCell *cell = SCellMgr::instance()->getGenericCell(SCell::MUX, input);
        assert(cell);
        return cell;
    }

    void
    GenericMuxMapper::setPinMapList()
    {
        assert(type != UNSET);
        pair<int, int> inputSelect = this->getInputSelectSize();
        
        PinMapList pinMapList;
        pinMapList.push_back(make_pair(SyIxName("OUT"), "out"));

        for(int i = 0; i < inputSelect.first; ++i)
        {
            SyIxName old("IN", i);
            SyIxName new_("in", i);
            pinMapList.push_back(make_pair(old, this->getMapName(new_)));
        }
        if(inputSelect.second == 1) {
            pinMapList.push_back(make_pair(SyIxName("SEL"), "select"));
        }
        else
        {
            for(int i = 0; i < inputSelect.second; ++i)
            {
                SyIxName old("SEL", i);
                SyIxName new_("select", i);
                pinMapList.push_back(make_pair(old, this->getMapName(new_)));
            }    
        }
        GenericMapperBase::setPinMapList(pinMapList);
    }

    void
    GenericMuxMapper::doMapping()
    {
        type = MUX_1;
        this->createCellInstances(GeneralMuxPredicate("MUX_2_1"));
        type = MUX_2;
        this->createCellInstances(GeneralMuxPredicate("MUX_4_1"));
        type = MUX_3;
        this->createCellInstances(GeneralMuxPredicate("MUX_8_1"));
        type = MUX_4;
        this->createCellInstances(GeneralMuxPredicate("MUX_16_1"));
        type = MUX_5;
        this->createCellInstances(GeneralMuxPredicate("MUX_32_1"));
        type = MUX_6;
        this->createCellInstances(GeneralMuxPredicate("MUX_64_1"));

        //Create muxes for bigger select size
        this->createMuxLogic();

    }

    vector<pair<SPN*, SPN*> >
    GenericMuxMapper::createSelCntvAndInvVec(const SyModInst *inst) 
    {
        vector<pair<SPN*, SPN*> > cntvInvVec;

        const SyList<SyScPin*>& pins = inst->getPins();
        for(SyNode<SyScPin*>* node = pins.head; node; node = node->next)
        {
            SyScPin *pin = node->val;
            if(pin->getDir() != SyScPort::IN) {
                continue;
            }
            string name = pin->getIxName().getName();
            if(name != "SEL") {
                continue;
            }

            SInst *inv = this->createInverter("INV", 
                this->getMapCntv(pin->getActual()), 0);

            cntvInvVec.push_back(make_pair(
                const_cast<SPN*>(this->getMapCntv(pin->getActual())), 
                const_cast<SPN*>(inv->getPin("out")->getActual())));
        }
        return cntvInvVec;
    }

    void
    GenericMuxMapper::createMuxLogic()
    {
        SBlock *block = this->getBlock();
        const SyModule *mod = SyRoot::instance()->getModule(block->getName());
        const SyList<SyModInst*>& minsts = mod->getModInsts();
        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next)
        {
            if(!MuxPredicate()(node->val)) {
                continue;
            }
            this->createMuxLogic(node->val);
        }    
    }

    void
    GenericMuxMapper::createMuxLogic(const SyModInst *inst)
    {
        vector<pair<SPN*, SPN*> > selCntvs = 
            this->createSelCntvAndInvVec(inst);
        vector<const SPN*> inCntvs;
        
        const SyList<SyScPin*>& pins = inst->getPins();
        for(SyNode<SyScPin*>* node = pins.head; node; node = node->next)
        {
            SyScPin *pin = node->val;
            if(pin->getDir() != SyScPort::IN) {
                continue;
            }
            string name = pin->getIxName().getName();
            if(name == "SEL") {
                continue;
            }
            inCntvs.push_back(this->getMapCntv(pin->getActual()));
        }    
        std::reverse(selCntvs.begin(), selCntvs.end());
        std::reverse(inCntvs.begin(), inCntvs.end());

        vector<SPN*> orInputs;

        for(int i = 0; i < inCntvs.size(); ++i)
        {
            vector<SPN*> cntvs = getSelectCntvs(selCntvs, i);
            cntvs.push_back(const_cast<SPN*>(inCntvs[i]));
            SNet *net = this->createNet();
            this->create2InputGateTree(string("And2"), SCell::AND, cntvs, net);
            orInputs.push_back(net);
        }
        const SPN *out = this->getMapCntv(inst->getPin(SyIxName("OUT"))->getActual());
        this->create2InputGateTree("Or2", SCell::OR, orInputs, out);
    }

    vector<SPN*> 
    GenericMuxMapper::getSelectCntvs(
        const vector<pair<SPN*, SPN*> >& selCntvs, int count) const
    {
        //lifted from GenericDecoderMapper
        int tmp = 1;
        vector<SPN*> selVec;
        for(int i = 0; i < selCntvs.size(); ++i) 
        {
            if(count & tmp) {
                selVec.push_back(selCntvs[i].first);
            }
            else {
                selVec.push_back(selCntvs[i].second);
            }
            tmp = tmp << 1;
        }
        return selVec;
    }
    
}
