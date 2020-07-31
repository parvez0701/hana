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

#include "genseqmap.h"
#include "sblock.h"
#include "scellmgr.h"

#include "sycontainer.h"

using namespace stview;

using std::make_pair;
using std::string;
using Synth::SyModInst;
using Synth::SyIxName;

namespace GenericMapping
{
    struct FFPredicate : InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == string("FF_2_1");
        }    
    };

    struct ResetFFPredicate: InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == string("FFR_3_1");
        }    
    };
    struct SetFFPredicate: InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == string("FFS_3_1");
        }    
    };
    struct RSFFPredicate: InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == string("FFRS_4_1");
        }    
    };
    struct SRFFPredicate: InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == string("FFSR_4_1");
        }    
    };

    struct LDPredicate : InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == string("LD_1_1");
        }    
    };
    struct ResetLDPredicate : InstancePredicate
    {
        virtual bool operator()(const SyModInst *inst) const {
            return inst->getMasterName() == string("LDR_3_1");
        }    
    };

    GenericSeqElementMapper::GenericSeqElementMapper() {
    }

    GenericSeqElementMapper::GenericSeqElementMapper(SBlock *block) :
        GenericMapperBase(block), cellType(SCell::CELLTYPE_BEGIN) {
    }

    GenericSeqElementMapper::~GenericSeqElementMapper() {
    }

    GenericSeqElementMapper*
    GenericSeqElementMapper::create(SBlock *block) const{
        return new GenericSeqElementMapper(block);
    }    

    const SCell*
    GenericSeqElementMapper::getCell() const
    {
        assert(cellType != SCell::CELLTYPE_BEGIN);
        const SCell* cell = 0;
        switch(cellType)
        {
            case SCell::FF:
                cell = SCellMgr::instance()->getGenericCell(cellType, 2);
                break;
            case SCell::RFF:
                cell = SCellMgr::instance()->getGenericCell(cellType, 3);
                break;
            case SCell::SFF:
                cell = SCellMgr::instance()->getGenericCell(cellType, 3);
                break;
            case SCell::RSFF:
                cell = SCellMgr::instance()->getGenericCell(cellType, 4);
                break;
            case SCell::SRFF:
                cell = SCellMgr::instance()->getGenericCell(cellType, 4);
                break;
            case SCell::LD:    
                cell = SCellMgr::instance()->getGenericCell(cellType, 2);
                break;
            case SCell::RLD:    
                cell = SCellMgr::instance()->getGenericCell(cellType, 3);
                break;
            default:
                cell = 0;
        }
        assert(cell);
        return cell;
    }

    void
    GenericSeqElementMapper::setPinMapList()
    {
        assert(cellType != SCell::CELLTYPE_BEGIN);
        
        PinMapList pinMapList;
        pinMapList.push_back(make_pair(SyIxName("D"), "d"));

        switch(cellType)
        {
            case SCell::FF:
                pinMapList.push_back(make_pair(SyIxName("CLK"), "clk"));
                break;
            case SCell::RFF:
                pinMapList.push_back(make_pair(SyIxName("CLK"), "clk"));
                pinMapList.push_back(make_pair(SyIxName("RST"), "reset"));
                break;
            case SCell::SFF:
                pinMapList.push_back(make_pair(SyIxName("CLK"), "clk"));
                pinMapList.push_back(make_pair(SyIxName("SET"), "set"));
                break;
            case SCell::RSFF:
                pinMapList.push_back(make_pair(SyIxName("CLK"), "clk"));
                pinMapList.push_back(make_pair(SyIxName("RST"), "reset"));
                pinMapList.push_back(make_pair(SyIxName("SET"), "set"));
                break;
            case SCell::SRFF:
                pinMapList.push_back(make_pair(SyIxName("CLK"), "clk"));
                pinMapList.push_back(make_pair(SyIxName("SET"), "set"));
                pinMapList.push_back(make_pair(SyIxName("RST"), "reset"));
                break;
            case SCell::LD:    
                pinMapList.push_back(make_pair(SyIxName("EN"), "enable"));
                break;
            case SCell::RLD:    
                pinMapList.push_back(make_pair(SyIxName("EN"), "enable"));
                pinMapList.push_back(make_pair(SyIxName("RST"), "reset"));
                break;
            default:
                assert(0);
        }
        pinMapList.push_back(make_pair(SyIxName("Q"), "q"));
        GenericMapperBase::setPinMapList(pinMapList);
    }

    void
    GenericSeqElementMapper::doMapping()
    {
        cellType = SCell::FF;
        this->createCellInstances(FFPredicate());
        cellType = SCell::RFF;
        this->createCellInstances(ResetFFPredicate());
        cellType = SCell::SFF;
        this->createCellInstances(SetFFPredicate());
        cellType = SCell::RSFF;
        this->createCellInstances(RSFFPredicate());
        cellType = SCell::SRFF;
        this->createCellInstances(SRFFPredicate());
        cellType = SCell::LD;
        this->createCellInstances(LDPredicate());
        cellType = SCell::RLD;
        this->createCellInstances(ResetLDPredicate());
    }
}
