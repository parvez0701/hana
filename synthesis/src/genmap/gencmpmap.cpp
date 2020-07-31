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


#include "gencmpmap.h"

#include "scellmgr.h"
#include "sblock.h"
#include "snet.h"
#include "sinst.h"

#include "sycontainer.h"
#include "syroot.h"

using namespace Synth;
using namespace stview;
using std::string;
using std::make_pair;


namespace GenericMapping
{
    struct CmpPredicate : public InstancePredicate
    {
        CmpPredicate(const string& n) : name(n) {}
        virtual bool operator()(const SyModInst* minst) const {
            return minst->getMasterName() == name;
        }    

        string name;
    };

    struct GeneralCmpPredicate
    {
        bool operator()(const SyModInst *minst) const {
            return (minst->getMasterName().substr(0, 4) == string("CMP_")) &&
                !CmpPredicate("CMP_1_1")(minst) &&
                !CmpPredicate("CMP_2_2")(minst) &&
                !CmpPredicate("CMP_4_4")(minst) &&
                !CmpPredicate("CMP_8_8")(minst) &&
                !CmpPredicate("CMP_16_16")(minst) &&
                !CmpPredicate("CMP_32_32")(minst);
            }    
    };


    GenericCmpMapper::GenericCmpMapper() {
    }

    GenericCmpMapper::GenericCmpMapper(SBlock *block) :
        GenericALElementMapper(block) {
        this->setCellType(SCell::CMP);
    }

    GenericCmpMapper::~GenericCmpMapper() {
    }

    GenericCmpMapper*
    GenericCmpMapper::create(SBlock *block) const {
        return new GenericCmpMapper(block);
    }    

    void
    GenericCmpMapper::init() {
    }

    void
    GenericCmpMapper::doMapping()
    {
        this->setInputType(IN_1);
        this->createCellInstances(CmpPredicate("CMP_1_1"));
        this->setInputType(IN_2);
        this->createCellInstances(CmpPredicate("CMP_2_2"));
        this->setInputType(IN_4);
        this->createCellInstances(CmpPredicate("CMP_4_4"));
        this->setInputType(IN_8);
        this->createCellInstances(CmpPredicate("CMP_8_8"));
        this->setInputType(IN_16);
        this->createCellInstances(CmpPredicate("CMP_16_16"));
        this->setInputType(IN_32);
        this->createCellInstances(CmpPredicate("CMP_32_32"));

        //general mapping
        this->doGeneralMapping();
    }

    void
    GenericCmpMapper::setPinMapList()
    {
        int size = (int) this->getInputType();
        PinMapList pinMapList;

        if(size == 1)
        {
            pinMapList.push_back(make_pair(SyIxName("A"), "in1"));
            pinMapList.push_back(make_pair(SyIxName("B"), "in2"));
        }    
        else
        {
            for(int i = 0; i < size; ++i) {
                pinMapList.push_back(make_pair(SyIxName("A", i), 
                    this->getMapName(SyIxName("in1", i))));
             
            }
            for(int i = 0; i < size; ++i) {
                pinMapList.push_back(make_pair(SyIxName("B", i), 
                    this->getMapName(SyIxName("in2", i))));
            }
        }
            
        pinMapList.push_back(make_pair(SyIxName("EQ"), "equal"));
        pinMapList.push_back(make_pair(SyIxName("INEQ"), "unequal"));
        pinMapList.push_back(make_pair(SyIxName("GT"), "greater"));
        pinMapList.push_back(make_pair(SyIxName("LT"), "lesser"));

        GenericMapperBase::setPinMapList(pinMapList);
    }

    const SCell*
    GenericCmpMapper::getCell() const
    {
        const SCell *cell = SCellMgr::instance()->getGenericCell(
            this->getCellType(), (short)this->getInputType()*2);
        assert(cell);
        return cell;
    }

    GenericCmpMapper::CmpType
    GenericCmpMapper::getComparisonType(const SyModInst *minst) const
    {
        const SyExpr *eqExpr = minst->getPin(SyIxName("EQ"))->getActual();
        const SyExpr *ineqExpr = minst->getPin(SyIxName("INEQ"))->getActual();
        const SyExpr *gtExpr = minst->getPin(SyIxName("GT"))->getActual();
        const SyExpr *ltExpr = minst->getPin(SyIxName("LT"))->getActual();

        if(eqExpr && !ineqExpr && !gtExpr && !ltExpr) {
            return EQ;
        }
        if(!eqExpr && ineqExpr && !gtExpr && !ltExpr) {
            return NEQ;
        }
        if(!eqExpr && !ineqExpr && gtExpr && !ltExpr) {
            return GT;
        }    

        if(!eqExpr && !ineqExpr && !gtExpr && ltExpr) {
            return LT;
        }
        if(eqExpr && !ineqExpr && gtExpr && !ltExpr) {
            return GTE;
        }

        if(eqExpr && !ineqExpr && !gtExpr && ltExpr) {
            return LTE;
        }
        return UNSET;
    }

    void
    GenericCmpMapper::doGeneralMapping()
    {
        SBlock *block = this->getBlock();
        const SyModule *mod = SyRoot::instance()->getModule(block->getName());
        const SyList<SyModInst*>& minsts = mod->getModInsts();
        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next)
        {
            if(GeneralCmpPredicate()(node->val)) {
                this->doGeneralMapping(node->val);
            }
        }    
    }

    void
    GenericCmpMapper::doGeneralMapping(const SyModInst *minst)
    {
        switch(getComparisonType(minst))
        {
            case EQ:
                this->createEQLogic(minst);
                break;
            case NEQ:
                this->createNEQLogic(minst);
                break;
            case LT:
                this->createLTLogic(minst);
                break;
            case GT:
                this->createGTLogic(minst);
                break;
            case LTE:
                this->createLTELogic(minst);
                break;
            case GTE:
                this->createGTELogic(minst);
                break;
            default: assert(0);    
        }
    }

    void
    GenericCmpMapper::createEQLogic(const SyModInst* minst)
    {
        vector<SyScPin*> in1 = this->getPins(minst, "A");
        vector<SyScPin*> in2 = this->getPins(minst, "B");

        vector<SPN*> outnets;
        for(int i = 0; i < in1.size(); ++i)
        {
            SNet *net = this->createNet();
            this->create2InputGate("Xnor2", SCell::XNOR, 
                this->getMapCntv(in1[i]->getActual()),
                this->getMapCntv(in2[i]->getActual()),
                net);
            outnets.push_back(net);    
        }
        this->create2InputGateTree("And2", SCell::AND, outnets,
            this->getMapCntv(minst->getPin(SyIxName("EQ"))->getActual()));
    }
    void
    GenericCmpMapper::createNEQLogic(const SyModInst* minst)
    {
        vector<SyScPin*> in1 = this->getPins(minst, "A");
        vector<SyScPin*> in2 = this->getPins(minst, "B");

        vector<SPN*> outnets;
        for(int i = 0; i < in1.size(); ++i)
        {
            SNet *net = this->createNet();
            this->create2InputGate("Xor2", SCell::XOR, 
                this->getMapCntv(in1[i]->getActual()),
                this->getMapCntv(in2[i]->getActual()),
                net);
            outnets.push_back(net);    
        }
        this->create2InputGateTree("Or2", SCell::OR, outnets,
            this->getMapCntv(minst->getPin(SyIxName("INEQ"))->getActual()));
    }
    void
    GenericCmpMapper::createLTLogic(const SyModInst* minst)
    {
        this->createLTorGTLogic(minst, true);
    }
    void
    GenericCmpMapper::createGTLogic(const SyModInst* minst)
    {
        this->createLTorGTLogic(minst, false);
    }
    void
    GenericCmpMapper::createLTELogic(const SyModInst* minst)
    {
        this->createLTorGTLogic(minst, true, true);
    }
    void
    GenericCmpMapper::createGTELogic(const SyModInst* minst)
    {
        this->createLTorGTLogic(minst, false, true);
    }

    void
    GenericCmpMapper::createLTorGTLogic(const SyModInst* minst, 
        bool less, bool equal)
    {
        vector<SyScPin*> in1 = this->getPins(minst, "A");
        vector<SyScPin*> in2 = this->getPins(minst, "B");

        vector<SPN*> xornets;
        vector<SPN*> ornets;
        int size = in1.size();
        
        for(int i = 0; i < size; ++i)
        {
            SNet *xorNet = this->createNet();
            const SPN *act1 = this->getMapCntv(in1[i]->getActual());
            const SPN *act2 = this->getMapCntv(in2[i]->getActual());
            this->create2InputGate("XOR", SCell::XOR, act1, act2, xorNet);
            xornets.push_back(xorNet);

            SNet *andNet = this->createNet();
            this->create2InputGate("AND", SCell::AND, less ? act2 : act1,
                xorNet, andNet);
            ornets.push_back(andNet);    
        }    
        vector<SPN*> xnornets;
        vector<SPN*> outnets;
        for(int i = 0; i < xornets.size(); ++i)
        {
            SNet *xnorNet = this->createNet();
            this->createInverter("INV", xornets[i], xnorNet);
            xnornets.push_back(xnorNet);
        }    
        for(int i = 0; i < size; ++i)
        {
            SPN *outNet = ornets[i];
            for(int j = 0; j < i; ++j)
            {
                SNet *net = this->createNet();
                this->create2InputGate("AND", SCell::AND, outNet, 
                    xnornets[j], net);
                outNet = net;
            }    
            outnets.push_back(outNet);    
        }    

        this->create2InputGateTree("OR", SCell::OR, outnets,
            this->getMapCntv(minst->getPin(
                SyIxName(less ? "LT" : "GT"))->getActual()));
        if(equal) 
        {
            this->create2InputGateTree("AND", SCell::AND, xnornets,
                this->getMapCntv(minst->getPin(
                    SyIxName("EQ"))->getActual()));
        }        
        
        
    }
}

