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

#include "genincmap.h"

#include "sycontainer.h"
#include "syroot.h"

#include "sinst.h"
#include "spin.h"
#include "snet.h"
#include "sblock.h"
#include "scellmgr.h"

#include "namemgr.h"

using namespace Synth;
using namespace stview;

using std::make_pair;
using std::string;
using std::vector;

namespace GenericMapping
{
    struct IncPredicate : InstancePredicate
    {
        IncPredicate(const string& n) : name (n) {
        }

        virtual bool operator()(const SyModInst *minst) const {
            return minst->getMasterName() == name;
        }    
        string name;
    };

    struct GeneralIncPredicate
    {
        virtual bool operator()(const SyModInst *minst) const
        {
            if(minst->getMasterName().substr(0, 4) != "INC_") {
                return false;
            }
            return !IncPredicate("INC_1_1")(minst) &&
                !IncPredicate("INC_2_2")(minst) &&
                !IncPredicate("INC_4_4")(minst) &&
                !IncPredicate("INC_8_8")(minst) &&
                !IncPredicate("INC_16_16")(minst) &&
                !IncPredicate("INC_32_32")(minst) &&
                !IncPredicate("INC_64_64")(minst);
        }        
    };
    GenericIncMapper::GenericIncMapper() {
    }

    GenericIncMapper::GenericIncMapper(SBlock* block) :
        GenericALElementMapper(block) {
        this->init();
    }

    GenericIncMapper::~GenericIncMapper() {
    }

    GenericIncMapper*
    GenericIncMapper::create(SBlock *block) const {
        return new GenericIncMapper(block);
    }    

    void
    GenericIncMapper::init() {
    }

    void
    GenericIncMapper::setPinMapList()
    {
        PinMapList pinMapList;
        int size = (int) this->getInputType();
        if(size == 1)
        {
            pinMapList.push_back(make_pair(SyIxName("IN"), "in"));
            pinMapList.push_back(make_pair(SyIxName("OUT"), 
                this->getMapName(SyIxName("out", 0))));
            pinMapList.push_back(make_pair(SyIxName("COUT"), 
                this->getMapName(SyIxName("out", 1))));
        }
        else
        {
            for(int i = 0; i < size; ++i) {
                pinMapList.push_back(make_pair(SyIxName("IN", i),
                    this->getMapName(SyIxName("in", i))));
            }        
            for(int i = 0; i < size; ++i) {
                pinMapList.push_back(make_pair(SyIxName("OUT", i),
                    this->getMapName(SyIxName("out", i))));
            }        
            pinMapList.push_back(make_pair(SyIxName("COUT"),
             this->getMapName(SyIxName("out", size))));
        }
        GenericMapperBase::setPinMapList(pinMapList);
    }

    const SCell*
    GenericIncMapper::getCell() const
    {
        const SCell *cell = SCellMgr::instance()->getGenericCell(SCell::INC, 
            (int) this->getInputType());
        assert(cell);
        return cell;
    }

    void
    GenericIncMapper::doMapping()
    {
        this->setCellType(SCell::INC);
        this->setInputType(IN_1);

        while(!this->isUnsetState())
        {
            this->createCellInstances(IncPredicate(this->getCellName()));
            this->nextInState();
        }

        this->doGeneralMapping();
    }

    void
    GenericIncMapper::doGeneralMapping()
    {
        const SyModule *mod = SyRoot::instance()->getModule(
            this->getBlock()->getName());
        
        assert(mod);

        const SyList<SyModInst*>& minsts = mod->getModInsts();

        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next)
        {
            if(!GeneralIncPredicate()(node->val)) {
                continue;
            }
            int origsize = this->getInputSize(node->val, "IN");
            int tmp = 1;
            while(tmp < origsize) {
                tmp = tmp << 1;
            }
            int size = tmp;
            const SCell *cell = SCellMgr::instance()->getCell(SCell::INC, size);
            if(!cell) {
                continue;
            }
            string name = BlockNameManager::instance()->createName(
                this->getBlock(), "INC");
            SInst *inst = new SInst(name, cell);    
            this->getBlock()->addInst(inst);
            this->addPins(node->val, inst, "IN", "in");
            this->addPins(node->val, inst, "OUT", "out");

            inst->addPin(new SPin(this->getMapName(SyIxName("out", origsize)),
                this->getMapCntv(node->val->getPin(SyIxName("COUT"))->getActual()), inst));
            this->fillExtraBits(inst, "in", origsize, size);    
            this->fillExtraBits(inst, "out", origsize+1, size);    
        }    
    }

    string
    GenericIncMapper::getCellName() const
    {
        switch(this->getInputType())
        {
            case IN_1: return "INC_1_1";
            case IN_2: return "INC_2_2";
            case IN_4: return "INC_4_4";
            case IN_8: return "INC_8_8";
            case IN_16: return "INC_16_16";
            case IN_32: return "INC_32_32";
            case IN_64: return "INC_64_64";
            default: assert(0);
        }
        return "";
    }

    void
    GenericIncMapper::addPins(const SyModInst *minst, 
        SInst *inst, const string& mname, const string& sname) const
    {
        vector<SyScPin*> pins = this->getPins(minst, mname);

        assert(!pins.empty());
        if(pins.size()  == 1)
        {
            SPin *spin = new SPin(sname, 
                this->getMapCntv(pins[0]->getActual()), inst);
            inst->addPin(spin);    
        }
        else
        {
            for(int i = 0; i < pins.size(); ++i)
            {
                SPin *spin = new SPin(this->getMapName(SyIxName(sname, i)),
                    this->getMapCntv(pins[i]->getActual()), inst);
                inst->addPin(spin);
            }    
        }
    }

    void
    GenericIncMapper::fillExtraBits(SInst *inst, const string& pin,
        int start, int end) 
    {
        SBlock* block = this->getBlock();
        const SCell *gndCell = SCellMgr::instance()->getCell(SCell::GND, 0);

        for(int i = start; i < end; ++i) 
        {
            SNet *net = this->createNet();
            string name = BlockNameManager::instance()->createName(block, "GND");
            SInst *gnd = new SInst(name, gndCell);
            gnd->addPin(new SPin("out", net, gnd));
            inst->addPin(new SPin(
                this->getMapName(SyIxName(pin, i)), net, inst));
            block->addInst(gnd);
        }    
    }
}
