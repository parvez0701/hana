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

#include "genmuldivmap.h"

#include "scellmgr.h"
#include "spin.h"
#include "sinst.h"
#include "snet.h"
#include "sblock.h"

#include "sycontainer.h"
#include "syroot.h"
#include "namemgr.h"

using namespace Synth;
using namespace stview;

using std::string;
using std::make_pair;
namespace GenericMapping
{
    struct Predicate : public InstancePredicate
    {
        Predicate(const string& n) : name(n) { }
        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == name;
        }    

        string name;
    };
    struct MulPredicate : public InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const 
        {
            return (inst->getMasterName().substr(0, 4) == "MUL_") &&
                (!Predicate("MUL_1_1")(inst)) &&
                (!Predicate("MUL_2_2")(inst)) &&
                (!Predicate("MUL_4_4")(inst)) &&
                (!Predicate("MUL_8_8")(inst)) &&
                (!Predicate("MUL_16_16")(inst)) &&
                (!Predicate("MUL_32_32")(inst)) &&
                (!Predicate("MUL_64_64")(inst));
        }    
    };

    struct DivPredicate : public InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const 
        {
            return (inst->getMasterName().substr(0, 4) == "DIV_") &&
                (!Predicate("DIV_1_1")(inst)) &&
                (!Predicate("DIV_2_2")(inst)) &&
                (!Predicate("DIV_4_4")(inst)) &&
                (!Predicate("DIV_8_8")(inst)) &&
                (!Predicate("DIV_16_16")(inst)) &&
                (!Predicate("DIV_32_32")(inst)) &&
                (!Predicate("DIV_64_64")(inst));
        }    
    };


    GenericMulDivMapper::GenericMulDivMapper()  
    {
    }

    GenericMulDivMapper::GenericMulDivMapper(SBlock *block) :
        GenericALElementMapper(block) {
        this->init();
    }

    GenericMulDivMapper::~GenericMulDivMapper() {
    }

    GenericMulDivMapper*
    GenericMulDivMapper::create(SBlock *block) const {
        return new GenericMulDivMapper(block);
    }    

    void
    GenericMulDivMapper::init()
    {
    }

    const SCell*
    GenericMulDivMapper::getCell() const 
    {
        const SCell *cell = SCellMgr::instance()->getGenericCell(this->getCellType(),
            2*(short)this->getInputType());
        assert(cell);
        return cell;
    }    

    void
    GenericMulDivMapper::doMapping()
    {
        this->doMapping(SCell::MUL);
        this->doMapping(SCell::DIV);

        this->doGeneralMapping(SCell::MUL);
        this->doGeneralMapping(SCell::DIV);
    }

    void
    GenericMulDivMapper::setPinMapList()
    {
        PinMapList pinMapList;
        int size = (int) this->getInputType();

        GenericALElementMapper::setPinMapList(pinMapList);
        this->setPinMapList(pinMapList, size); 

        GenericMapperBase::setPinMapList(pinMapList);
    }

    void
    GenericMulDivMapper::setPinMapList(PinMapList& pinMapList,
        int size) const
    {
        SCell::CellType cellType = this->getCellType();
        if(cellType == SCell::DIV) 
        {
            if(size == 1) {
                pinMapList.push_back(make_pair(SyIxName("REM"), "rem"));
                pinMapList.push_back(make_pair(SyIxName("C"), "out"));
            }
            else {
                for(int i = 0; i < size; ++i) {
                    pinMapList.push_back(make_pair(SyIxName("REM", i), 
                        this->getMapName(SyIxName("rem", i))));
                }        
                for(int i = 0; i < size; ++i)
                {
                    SyIxName old("C", i);
                    string new_(this->getMapName(SyIxName("out", i)));
                    pinMapList.push_back(make_pair(old, new_));
                }
            }
        }
        else 
        {
            size *= 2;
            for(int i = 0; i < size; ++i)
            {
                SyIxName old("C", i);
                string new_(this->getMapName(SyIxName("out", i)));
                pinMapList.push_back(make_pair(old, new_));
            }
        }
    }    

    string
    GenericMulDivMapper::getCellName() const
    {
        string inpart;
        switch(this->getInputType())
        {
            case IN_1:
                inpart = "_1_1";
                break;
            case IN_2:
                inpart = "_2_2";
                break;
            case IN_4:
                inpart = "_4_4";
                break;
            case IN_8:
                inpart = "_8_8";
                break;
            case IN_16:
                inpart = "_16_16";
                break;
            case IN_32:
                inpart = "_32_32";
                break;
            case IN_64:
                inpart = "_64_64";
                break;
            default:
                assert(0);
        }
        return string((this->getCellType() == SCell::MUL) ? "MUL" : "DIV") +
            inpart;

    }

    void
    GenericMulDivMapper::doMapping(SCell::CellType type)
    {
        this->setInputType(IN_1);
        this->setCellType(type);

        while(!this->isUnsetState())
        {
            this->createCellInstances(Predicate(this->getCellName()));
            this->nextInState();
        }
    }

    void
    GenericMulDivMapper::doGeneralMapping(SCell::CellType type)
    {
        SBlock *block = this->getBlock();
        const SyModule *mod = SyRoot::instance()->getModule(block->getName());
        MulPredicate mulPredicate;
        DivPredicate divPredicate;

        const InstancePredicate& predicate = (type == SCell::MUL) ? 
            static_cast<const InstancePredicate&>(mulPredicate) : 
            static_cast<const InstancePredicate&>(divPredicate);
        const SyList<SyModInst*>& minsts = mod->getModInsts();
        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next)
        {
            if(!predicate(node->val)) {
                continue;
            }
            const SCell *cell = this->getCell(type, node->val);
            if(!cell) {
                continue;
            }
            string name = BlockNameManager::instance()->createName(block,
                (type == SCell::MUL) ? "MUL" : "DIV" );

            SInst *inst = new SInst(name, cell);
            block->addInst(inst);
            this->createPins(node->val, inst, type);
        }    
    }

    int 
    GenericMulDivMapper::getMapSize(const SyModInst *minst) const
    {
        int input = this->getInputSize(minst, "A");
        int actSize = 1;
        while(input)
        {
            input = input >> 1;
            actSize = actSize << 1;
        }
        return actSize;
    }

    const SCell*
    GenericMulDivMapper::getCell(SCell::CellType type, 
        const SyModInst *minst) const
    {
        return SCellMgr::instance()->getGenericCell(type, 
            2*this->getMapSize(minst));
    }

    void
    GenericMulDivMapper::createPins(const SyModInst *minst, SInst *inst,
        SCell::CellType type) 
    {
        int size = this->getMapSize(minst);
        
        vector<SyScPin*> apins = this->getPins(minst, "A");
        vector<SyScPin*> bpins = this->getPins(minst, "B");
        vector<SyScPin*> cpins = this->getPins(minst, "C");

        this->createPins(apins, inst, "in1", size);
        this->createPins(bpins, inst, "in2", size);
        this->createPins(cpins, inst, "out", (type == SCell::MUL) ?
            2*size: size);
        
        if(type == SCell::DIV)
        {
            vector<SyScPin*> rpins = this->getPins(minst, "REM");
            this->createPins(rpins, inst, "rem", size);
        }    
    }

    void
    GenericMulDivMapper::createPins(const vector<SyScPin*>& pins,
        SInst *inst, const std::string& name, int size) 
    {
        if(pins.size() == 1)
        {
            inst->addPin(new SPin(name, 
                this->getMapCntv(pins[0]->getActual()), inst));
            return;
        }
        int index = 0;

        for(int i = 0; i < pins.size(); ++i) 
        {
            inst->addPin(new SPin(this->getMapName(SyIxName(name, 
                pins[i]->getIxName().getIndex())),
                this->getMapCntv(pins[i]->getActual()), inst));
            index = std::max((long)index, pins[i]->getIxName().getIndex());    
        }

        //connect 0s to msb
        for(++index; index < size; ++index)
        {
            SNet *net = this->createNet();

            SInst *ginst = new SInst(BlockNameManager::instance()->
                createName(this->getBlock(), "GND"),
                SCellMgr::instance()->getCell(SCell::GND, 0));
            ginst->addPin(new SPin("out", net, ginst));    

            inst->addPin(new SPin(this->getMapName(SyIxName(name, index)),
                net, inst));
            this->getBlock()->addInst(ginst);    
        }
    }
}
