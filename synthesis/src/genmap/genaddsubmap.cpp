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

#include "genaddsubmap.h"
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
    struct Predicate : public InstancePredicate
    {
        Predicate(const string& n) : name(n) { }
        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == name;
        }    

        string name;
    };
    struct AddPredicate : public InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const 
        {
            return (inst->getMasterName().substr(0, 4) == "ADD_") &&
                (!Predicate("ADD_1_1")(inst)) &&
                (!Predicate("ADD_2_2")(inst)) &&
                (!Predicate("ADD_4_4")(inst)) &&
                (!Predicate("ADD_8_8")(inst)) &&
                (!Predicate("ADD_16_16")(inst)) &&
                (!Predicate("ADD_32_32")(inst)) &&
                (!Predicate("ADD_64_64")(inst));
        }    
    };

    struct SubPredicate : public InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const 
        {
            return (inst->getMasterName().substr(0, 4) == "SUB_") &&
                (!Predicate("SUB_1_1")(inst)) &&
                (!Predicate("SUB_2_2")(inst)) &&
                (!Predicate("SUB_4_4")(inst)) &&
                (!Predicate("SUB_8_8")(inst)) &&
                (!Predicate("SUB_16_16")(inst)) &&
                (!Predicate("SUB_32_32")(inst)) &&
                (!Predicate("SUB_64_64")(inst));
        }    
    };    
    template<class T>
    struct CmpPins
    {
        bool operator()(const T* pin1, const T* pin2) const {
            return pin1->getIxName().getIndex() < pin2->getIxName().getIndex();
        }    
    };


    GenericAddSubMapper::GenericAddSubMapper()  
    {
    }

    GenericAddSubMapper::GenericAddSubMapper(SBlock *block) :
        GenericALElementMapper(block) {
        this->init();
    }

    GenericAddSubMapper::~GenericAddSubMapper() {
    }

    GenericAddSubMapper*
    GenericAddSubMapper::create(SBlock *block) const{
        return new GenericAddSubMapper(block);
    }    

    void
    GenericAddSubMapper::setPinMapList()
    {
        PinMapList pinMapList;
        int size = (int) this->getInputType();

        GenericALElementMapper::setPinMapList(pinMapList);
        this->setPinMapList(pinMapList, size); 

        GenericMapperBase::setPinMapList(pinMapList);
    }

    const SCell*
    GenericAddSubMapper::getCell() const 
    {
        const SCell *cell = SCellMgr::instance()->getGenericCell(
            this->getCellType(), this->getInputSize());
        assert(cell);
        return cell;
    }

    void
    GenericAddSubMapper::doMapping()
    {
        this->doAdderMapping();
        this->doSubMapping();
    }
    
    void
    GenericAddSubMapper::setPinMapList(PinMapList& pinMapList,
        int size) const
    {
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

        SCell::CellType cellType = this->getCellType();
        if(cellType == SCell::ADD) 
        {
            pinMapList.push_back(make_pair(SyIxName("COUT"), "cout"));
            pinMapList.push_back(make_pair(SyIxName("CIN"), "cin"));
        }
        else if(cellType == SCell::SUB) 
        {
            pinMapList.push_back(make_pair(SyIxName("BOUT"), "cout"));
            pinMapList.push_back(make_pair(SyIxName("BIN"), "cin"));
        }
    }
    int
    GenericAddSubMapper::getInputSize() const
    {
        int size = 2*(int) this->getInputType();
        size = size+1;
        return size;
    }

    void
    GenericAddSubMapper::doAdderMapping()
    {
        this->setCellType(SCell::ADD);
        this->setInputType(IN_1);
        GenericMapperBase::createCellInstances(Predicate("ADD_1_1"));
        this->setInputType(IN_2);
        GenericMapperBase::createCellInstances(Predicate("ADD_2_2"));
        this->setInputType(IN_4);
        GenericMapperBase::createCellInstances(Predicate("ADD_4_4"));
        this->setInputType(IN_8);
        GenericMapperBase::createCellInstances(Predicate("ADD_8_8"));
        this->setInputType(IN_16);
        GenericMapperBase::createCellInstances(Predicate("ADD_16_16"));
        this->setInputType(IN_32);
        GenericMapperBase::createCellInstances(Predicate("ADD_32_32"));
        this->setInputType(IN_64);
        GenericMapperBase::createCellInstances(Predicate("ADD_64_64"));
        this->doGeneralMapping(AddPredicate());
    }

    void
    GenericAddSubMapper::doGeneralMapping(const InstancePredicate& pred)
    {
        SBlock *block = this->getBlock();
        const SyModule *mod = SyRoot::instance()->getModule(block->getName());
        SCell::CellType cellType = this->getCellType();
        const SyList<SyModInst*>& minsts = mod->getModInsts();
        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next)
        {
            if(!pred(node->val)) {
                continue;
            }
            vector<SCell*> cells = this->getCells(cellType, 
                GenericALElementMapper::getInputSize(node->val, "A"));
            vector<SInst*> insts = this->createCellInstances(cells, 
                (cellType == SCell::ADD) ? "ADD" : "SUB");
            this->stitchCellInstances(insts, cellType, node->val);

        }    
    }

    void
    GenericAddSubMapper::init() 
    {
        initAdd();
        initSub();
    }

    void
    GenericAddSubMapper::initAdd() 
    {
        this->setCellType(SCell::ADD);
        this->setInputType(IN_1);

        while(!this->isUnsetState())
        {
            addTable.insert(make_pair(this->getInputType(), 
                const_cast<SCell*>(SCellMgr::instance()->
                getCell(this->getCellType(), this->getInputSize()))));
            this->nextInState();
        }    
    }
    void
    GenericAddSubMapper::initSub() 
    {
        this->setCellType(SCell::SUB);
        this->setInputType(IN_1);
        while(!this->isUnsetState())
        {
            subTable.insert(make_pair(this->getInputType(), 
                const_cast<SCell*>(SCellMgr::instance()->
                getCell(this->getCellType(), this->getInputSize()))));
            this->nextInState();
        }    
    }


    vector<SCell*>
    GenericAddSubMapper::getCells(SCell::CellType type, int size) const
    {
        vector<InputType> inTypeVec = this->getCellTypeVec(size);
        vector<SCell*> cells;

        for(int i = 0; i < inTypeVec.size(); ++i)
        {
            CellTable::const_iterator iter;
            switch (type)
            {
                case SCell::ADD:
                {
                    iter = addTable.find(inTypeVec[i]);
                    assert(iter != addTable.end());
                    break;
                }
                case SCell::SUB:
                {
                    iter = subTable.find(inTypeVec[i]);
                    assert(iter != subTable.end());
                    break;
                }
                default: {
                    assert(0);
                }    
            }
            cells.push_back(iter->second);
        }
        return cells;
    }

    void
    GenericAddSubMapper::createInOutPins(SInst *inst) const
    {
        const SCell *cell = inst->getCell();

        const list<SPort*>& ports = cell->getPorts();
        list<SPort*>::const_iterator iter = ports.begin();

        for(; iter != ports.end(); ++iter)
        {
            SPort *port = *iter;
            string name = port->getName();
            string char3 = name.substr(0, 3);
            if((char3 != "in1") && (char3 != "in2") && (char3 != "out")) {
                continue;
            }
            SPin *pin = new SPin(name, 0, inst);
            inst->addPin(pin);
        }
    }

    void
    GenericAddSubMapper::createSpecialPins(SInst *inst, 
        SCell::CellType type) const
    {
        inst->addPin(new SPin("cin", 0, inst));
        inst->addPin(new SPin("cout", 0, inst));
    }

    vector<SInst*>
    GenericAddSubMapper::createCellInstances(
        const vector<SCell*>& cells, 
        const string& hint) const
    {
        vector<SInst*> insts;
        SBlock *block = const_cast<SBlock*>(this->getBlock());
        for(int i = 0; i < cells.size(); ++i)
        {
            string name = BlockNameManager::instance()->createName(block,
                hint);
            SInst *inst = new SInst(name, cells[i]);    
            insts.push_back(inst);
            block->addInst(inst);
        }
        return insts;
    }

    void
    GenericAddSubMapper::stitchCellInstances(vector<SInst*>& insts,
        SCell::CellType type, const SyModInst *minst)
    {
        for(int i = 0; i < insts.size(); ++i)
        {
            this->createInOutPins(insts[i]);
            this->createSpecialPins(insts[i], type);
        }    
        this->stitchCellInstances(insts, minst, "A", "in1", true);
        this->stitchCellInstances(insts, minst, "B", "in2", true);
        this->stitchCellInstances(insts, minst, "C", "out", false);
        this->stitchCarryPins(insts, minst, type);
    }

    void
    GenericAddSubMapper::stitchCellInstances(vector<SInst*>& insts,
        const SyModInst *minst, 
        const string& oldName, 
        const string& newName,
        bool input) const
    {
        if(insts.empty()) {
            return;
        }    
        int instCount = 0;
        SInst *inst = insts[instCount];

        vector<SyScPin*> mpins = this->getPins(minst, oldName);
        vector<SPin*> spins = this->getPins(inst, newName);

        std::sort(mpins.begin(), mpins.end(), CmpPins<SyScPin>());
        std::sort(spins.begin(), spins.end(), CmpPins<SPin>());
        int j = 0;

        for(int i = 0; i < mpins.size(); ++i)
        {
            if(j == spins.size())
            {
                j = 0;
                inst = insts[++instCount];
                spins = this->getPins(inst, newName);
                std::sort(spins.begin(), spins.end(), CmpPins<SPin>());
            }
            SyScPin *oldp = mpins[i];
            SPin *newp = spins[j++];
            newp->setActual(this->getMapCntv(oldp->getActual()));
        }
    }

    void
    GenericAddSubMapper::stitchCarryPins(vector<SInst*>& insts,
        const SyModInst *minst, SCell::CellType type)
    {
        for(int i = 0; i < insts.size()-1; ++i)
        {
            SPin *pin1 = const_cast<SPin*>(insts[i]->getPin("cout"));
            SPin *pin2 = const_cast<SPin*>(insts[i+1]->getPin("cin"));
            SNet *net = this->createNet();
            pin1->setActual(net);
            pin2->setActual(net);
        }
        SPin *spin = const_cast<SPin*>(insts[insts.size()-1]->getPin("cout"));
        const SyScPin *mpin = minst->getPin(SyIxName(
            (type == SCell::ADD) ? "COUT" : "BOUT"));
        assert(mpin);
        spin->setActual(this->getMapCntv(mpin->getActual()));
        spin = const_cast<SPin*>(insts[0]->getPin("cin"));
        mpin = minst->getPin(SyIxName(
            (type == SCell::ADD) ? "CIN" : "BIN"));
        assert(mpin);
        spin->setActual(this->getMapCntv(mpin->getActual()));
    }

    void
    GenericAddSubMapper::doSubMapping()
    {
        this->setCellType(SCell::SUB);
        this->setInputType(IN_1);
        GenericMapperBase::createCellInstances(Predicate("SUB_1_1"));
        this->setInputType(IN_2);
        GenericMapperBase::createCellInstances(Predicate("SUB_2_2"));
        this->setInputType(IN_4);
        GenericMapperBase::createCellInstances(Predicate("SUB_4_4"));
        this->setInputType(IN_8);
        GenericMapperBase::createCellInstances(Predicate("SUB_8_8"));
        this->setInputType(IN_16);
        GenericMapperBase::createCellInstances(Predicate("SUB_16_16"));
        this->setInputType(IN_32);
        GenericMapperBase::createCellInstances(Predicate("SUB_32_32"));
        this->setInputType(IN_64);
        GenericMapperBase::createCellInstances(Predicate("SUB_64_64"));
        this->doGeneralMapping(SubPredicate());
    }


}
