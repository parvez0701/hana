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

#include "genshiftmap.h"

#include "sinst.h"
#include "spin.h"
#include "snet.h"
#include "sblock.h"
#include "scellmgr.h"

#include "sycontainer.h"
#include "syroot.h"
#include "namemgr.h"

using namespace Synth;
using namespace stview;

using std::string;
using std::vector;
using std::pair;
using std::set;
using std::make_pair;

namespace GenericMapping
{
    set<const SyExpr*> GenericShiftMapper::notOutExprs = set<const SyExpr*>();
    struct LeftShiftPredicate: public InstancePredicate
    {
        LeftShiftPredicate(const string& n) : name(n) {
        }
        virtual bool operator()(const SyModInst *minst) const
        {
            if(minst->getMasterName() != name) {
                return false;
            }
            SyScPin *pin = const_cast<SyScPin*>(minst->getPin(SyIxName("OP")));
            assert(pin);
            const SyExpr *act = pin->getActual();
            set<const SyExpr*>::const_iterator iter = 
                GenericShiftMapper::notOutExprs.find(act);

            return iter != GenericShiftMapper::notOutExprs.end();
        }

        string name;
    };

    struct RightShiftPredicate: public InstancePredicate
    {
        RightShiftPredicate(const string& n) : name(n) {
        }
        virtual bool operator()(const SyModInst *minst) const
        {
            if(minst->getMasterName() != name) {
                return false;
            }
            return !LeftShiftPredicate(name)(minst);
        }
        string name;
    };

    struct GeneralLSPredicate
    {
        bool operator()(const SyModInst *minst) const
        {
            string name = minst->getMasterName();
            if(name.substr(0, 8) != "SHIFTER_") {
                return false;
            }
            return !LeftShiftPredicate("SHIFTER_1_1")(minst) &&
                !LeftShiftPredicate("SHIFTER_2_2")(minst) &&
                !LeftShiftPredicate("SHIFTER_4_4")(minst) &&
                !LeftShiftPredicate("SHIFTER_8_8")(minst) &&
                !LeftShiftPredicate("SHIFTER_16_16")(minst) &&
                !LeftShiftPredicate("SHIFTER_32_32")(minst) &&
                !LeftShiftPredicate("SHIFTER_64_64")(minst) &&
                !RightShiftPredicate("SHIFTER_1_1")(minst) &&
                !RightShiftPredicate("SHIFTER_2_2")(minst) &&
                !RightShiftPredicate("SHIFTER_4_4")(minst) &&
                !RightShiftPredicate("SHIFTER_8_8")(minst) &&
                !RightShiftPredicate("SHIFTER_16_16")(minst) &&
                !RightShiftPredicate("SHIFTER_32_32")(minst) &&
                !RightShiftPredicate("SHIFTER_64_64")(minst);
        }        
    };
    struct GeneralRSPredicate
    {
        bool operator()(const SyModInst *minst) const
        {
            string name = minst->getMasterName();
            if(name.substr(0, 8) != "SHIFTER_") {
                return false;
            }
            return !GeneralLSPredicate()(minst) &&
                !LeftShiftPredicate("SHIFTER_1_1")(minst) &&
                !LeftShiftPredicate("SHIFTER_2_2")(minst) &&
                !LeftShiftPredicate("SHIFTER_4_4")(minst) &&
                !LeftShiftPredicate("SHIFTER_8_8")(minst) &&
                !LeftShiftPredicate("SHIFTER_16_16")(minst) &&
                !LeftShiftPredicate("SHIFTER_32_32")(minst) &&
                !LeftShiftPredicate("SHIFTER_64_64")(minst) &&
                !RightShiftPredicate("SHIFTER_1_1")(minst) &&
                !RightShiftPredicate("SHIFTER_2_2")(minst) &&
                !RightShiftPredicate("SHIFTER_4_4")(minst) &&
                !RightShiftPredicate("SHIFTER_8_8")(minst) &&
                !RightShiftPredicate("SHIFTER_16_16")(minst) &&
                !RightShiftPredicate("SHIFTER_32_32")(minst) &&
                !RightShiftPredicate("SHIFTER_64_64")(minst);
        }    
    };

    GenericShiftMapper::GenericShiftMapper()
    {
    }

    GenericShiftMapper::GenericShiftMapper(SBlock *block) :
        GenericALElementMapper(block) {
        init();
    }

    GenericShiftMapper::~GenericShiftMapper() {
    }

    GenericShiftMapper*
    GenericShiftMapper::create(SBlock *block) const {
        return new GenericShiftMapper(block);
    }

    void
    GenericShiftMapper::init() 
    {
        //This seems overhead, may be needs to be changed.
        notOutExprs.clear();
        const SyModule *mod = SyRoot::instance()->getModule(
            this->getBlock()->getName());    
        const SyList<SyModInst*>& minsts = mod->getModInsts();

        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next)
        {
            if(node->val->getMasterName() == "GND_0_1") {
                if(const SyExpr *actual = node->val->getPin(SyIxName("OUT"))->getActual()) {
                    notOutExprs.insert(actual);
                }    
            }
        }    
        
    }

    const SCell*
    GenericShiftMapper::getCell() const
    {
        pair<int, int> size = this->getInputSize();
        const SCell *cell = SCellMgr::instance()->getGenericCell(this->getCellType(),
            size.first + size.second + 1);
        assert(cell);
        return cell;
    }

    void
    GenericShiftMapper::setPinMapList()
    {
        PinMapList pinMapList;

        pair<int, int> inputs = this->getInputSize();

        if(inputs.first == 1)
        {
            pinMapList.push_back(make_pair(SyIxName("IN"), "in"));
            pinMapList.push_back(make_pair(SyIxName("SHIFT"), "shift"));
            pinMapList.push_back(make_pair(SyIxName("VAL"), "val"));
            pinMapList.push_back(make_pair(SyIxName("OUT"), "out"));
        }
        else
        {
            for(int i = 0; i < inputs.first; ++i) {
                pinMapList.push_back(make_pair(
                    SyIxName("IN", i), this->getMapName(SyIxName("in", i))));
            }        
            
            for(int i = 0; i < inputs.second; ++i) {
                pinMapList.push_back(make_pair(
                    SyIxName("SHIFT", i), this->getMapName(SyIxName("shift", i))));
            }        
            pinMapList.push_back(make_pair(SyIxName("VAL"), "val"));
            for(int i = 0; i < inputs.first; ++i) {
                pinMapList.push_back(make_pair(
                    SyIxName("OUT", i), this->getMapName(SyIxName("out", i))));
            }        
        }    
        GenericMapperBase::setPinMapList(pinMapList);
    }

    void
    GenericShiftMapper::doMapping()
    {
        this->doMapping(true);
        this->doMapping(false);

        this->doGeneralMapping();
    }

    void
    GenericShiftMapper::doGeneralMapping()
    {
        const SyModule *mod = SyRoot::instance()->getModule(
            this->getBlock()->getName());
        
        const SyList<SyModInst*>& minsts = mod->getModInsts();

        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next)
        {
            int flag = 0;
            if(GeneralLSPredicate()(node->val)) {
                flag = 1;
            }
            else if(GeneralRSPredicate()(node->val)) {
                flag = 2;
            }
            if(flag == 0) {
                continue;
            }    

            pair<int, int> inputShift = this->getMapSize(node->val);
            const SCell *cell = SCellMgr::instance()->getCell((flag == 1) ? 
                SCell::LSH : SCell::RSH,
                inputShift.first + inputShift.second + 1);
            if(!cell) {
                continue;
            }    
            this->doGeneralMapping(node->val, cell, inputShift);
        }    

    }    

    void 
    GenericShiftMapper::doMapping(bool left)
    {
        this->setCellType(left ? SCell::LSH: SCell::RSH);
        this->setInputType(IN_1);

        while(!this->isUnsetState()) 
        {
            LeftShiftPredicate lp(this->getCellName());
            RightShiftPredicate rp(this->getCellName());
            const InstancePredicate& predicate = 
                left ? static_cast<const InstancePredicate&>(lp) : 
                static_cast<const InstancePredicate&>(rp);
            this->createCellInstances(predicate); 
            this->nextInState();
        }
    }


    void
    GenericShiftMapper::doGeneralMapping(const SyModInst* minst,
        const SCell *cell, pair<int, int> inputShift)
    {
        SBlock *block = this->getBlock();
        string name(BlockNameManager::instance()->createName(block, cell->getName()));
        SInst *inst = new SInst(name, cell); 
        block->addInst(inst);
        vector<SyScPin*> pins = this->getPins(minst, "IN");
        int index = 0;
        for(index = 0; index < pins.size(); ++index) {
            inst->addPin(new SPin(this->getMapName(SyIxName("in", index)),
                this->getMapCntv(pins[index]->getActual()), inst));
        }        
        const SCell *gndCell = SCellMgr::instance()->getCell(SCell::GND,
            0);
        for(; index < inputShift.first; ++index)
        {
            SNet *net = this->createNet();
            name = BlockNameManager::instance()->createName(block, "GND");
            SInst *gnd = new SInst(name, gndCell);
            gnd->addPin(new SPin("out", net, gnd));
            inst->addPin(new SPin(
                this->getMapName(SyIxName("in", index)), net, inst));
            block->addInst(gnd);    
                
        }    
        
        pins = this->getPins(minst, "SHIFT");
        for(index = 0; index < pins.size(); ++index) {
            inst->addPin(new SPin(this->getMapName(SyIxName("shift", index)),
                this->getMapCntv(pins[index]->getActual()), inst));
        }        
        for(; index < inputShift.second; ++index)
        {
            SNet *net = this->createNet();
            name = BlockNameManager::instance()->createName(block, "GND");
            SInst *gnd = new SInst(name, gndCell);
            gnd->addPin(new SPin("out", net, gnd));
            inst->addPin(new SPin(
                this->getMapName(SyIxName("shift", index)), net, inst));
            block->addInst(gnd);    
        }    

        inst->addPin(new SPin("val", 
            this->getMapCntv(minst->getPin(SyIxName("VAL"))->getActual()), inst));
        pins = this->getPins(minst, "OUT");

        for(index = 0; index < pins.size(); ++index) {
            inst->addPin(new SPin(this->getMapName(SyIxName("out", index)),
                this->getMapCntv(pins[index]->getActual()), inst));
        }        
        for(; index < inputShift.first; ++index)
        {
            SNet *net = this->createNet();
            name = BlockNameManager::instance()->createName(block, "GND");
            SInst *gnd = new SInst(name, gndCell);
            gnd->addPin(new SPin("out", net, gnd));
            inst->addPin(new SPin(
                this->getMapName(SyIxName("out", index)), net, inst));
            block->addInst(gnd);    
        }    
    }

    pair<int, int>
    GenericShiftMapper::getInputSize(const SyModInst *minst) const
    {
        int insize = minst ? 
            GenericALElementMapper::getInputSize(minst, "IN") :
            (int) this->getInputType();

        int tmp = insize;
        int shift = 0;
        while(tmp) 
        {
            tmp = tmp >> 1;
            ++shift;
        };

        return make_pair(insize, shift);
    }

    string
    GenericShiftMapper::getCellName() const
    {
        switch(this->getInputType())
        {
            case IN_1:
                return "SHIFTER_1_1";
            case IN_2:
                return "SHIFTER_2_2";
            case IN_4:
                return "SHIFTER_4_4";
            case IN_8:
                return "SHIFTER_8_8";
            case IN_16:
                return "SHIFTER_16_16";
            case IN_32:
                return "SHIFTER_32_32";
            case IN_64:
                return "SHIFTER_64_64";
            default : return "";    
        }
        return "";
    }

    pair<int, int> 
    GenericShiftMapper::getMapSize(const SyModInst *minst) const
    {
        pair<int, int> inputShift;
        inputShift.first = GenericALElementMapper::getInputSize(minst, "IN");
        inputShift.second = GenericALElementMapper::getInputSize(minst, "SHIFT");
        int actIn = 1;
        int actShift = 1;
        while(inputShift.first)
        {
            inputShift.first = inputShift.first >> 1;
            actIn = actIn << 1;
        }
        while(inputShift.second)
        {
            inputShift.second = inputShift.second >> 1;
            actShift = actShift << 1;
        }
        return make_pair(actIn, actShift);
    }

}
