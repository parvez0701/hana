/* 
Copyright (C) 2009-2011 Parvez Ahmad
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

#include <algorithm>

#include "sopmgr.h"

#include "sroot.h"

#include "spn.h"
#include "sblockinst.h"
#include "sinst.h"
#include "sassign.h"

#include "sopandtrans.h"
#include "sopbuftrans.h"
#include "sopgndtrans.h"
#include "sopinvtrans.h"
#include "sopmuxtrans.h"
#include "sopnandtrans.h"
#include "sopnortrans.h"
#include "soportrans.h"
#include "sopvcctrans.h"
#include "sopxnortrans.h"
#include "sopxortrans.h"
//#include "sopsop.h"


using namespace stview;

namespace SOP
{
    ////////////////////////////////////////////////////////////////////
    // Predicate: StartCell
    ////////////////////////////////////////////////////////////////////

	struct StartInst : public std::unary_function<const SInst*, bool>
	{
	    bool operator()(const SInst *inst) const
		{
            SCell::CellType type = inst->getCellType();

            if(type == SCell::TRIBUF ||
                type == SCell::FF ||
                type == SCell::RFF ||
                type == SCell::SFF ||
                type == SCell::RSFF ||
                type == SCell::SRFF ||
                type == SCell::LD ||
                type == SCell::RLD ||
                type == SCell::ADD ||
                type == SCell::SUB ||
                type == SCell::MUL ||
                type == SCell::DIV ||
                type == SCell::CMP ||
                type == SCell::LSH ||
                type == SCell::RSH ||
                type == SCell::ENC ||
                type == SCell::DEC ||
                type == SCell::INC) {
                return true;
            }
			return false;
		    
		}
	};
    ////////////////////////////////////////////////////////////////////
    // Predicate: Leaf
    ////////////////////////////////////////////////////////////////////
    struct Leaf : public std::unary_function<SPN*, bool>
    {
        bool operator()(SPN *spn) const
        {
            if(!spn) {
                return true;
            }
            if(spn->dempty()) {
                //No drivers
                return true;
            }    

/*
            if(spn->dsize() > 1) {
                //multi driven
                return true;
            }
*/			
            spn->reset();
            Driver d = spn->getDriver();
            const SPin *pin = (*d);
            const SBlockInst *binst = pin->getBlockInst();
            if(binst) {
                //Not crossing the block boundary
                return true;
            }    
            const SInst *inst = pin->getCellInst();
            if(!inst) {
                return true;
            }    

			return StartInst()(inst);

        }
    };

    ////////////////////////////////////////////////////////////////////
    // class: Deleter
    ////////////////////////////////////////////////////////////////////

	struct Deleter : public std::unary_function<SInst*, bool>
	{
	    explicit Deleter(SBlock* blk) : m_blk(blk) { }
		bool operator()(SInst *inst) const
		{
		    if(StartInst()(inst)) {
			    return false;
			}
			m_blk->deleteCellInst(inst);
			return true;
		}
	    SBlock *m_blk;
	};



    ////////////////////////////////////////////////////////////////////
    // class: SopMgr
    ////////////////////////////////////////////////////////////////////
    SopMgr* SopMgr::m_sopMgr = 0;

    SopMgr::SopMgr()
    {
        m_sopTransTable[SCell::AND] = new SopAndTrans();
        m_sopTransTable[SCell::BUF] = new SopBufTrans();
        m_sopTransTable[SCell::GND] = new SopGndTrans();
        m_sopTransTable[SCell::NOT] = new SopInvTrans();
        m_sopTransTable[SCell::MUX] = new SopMuxTrans();
        m_sopTransTable[SCell::NAND] = new SopNandTrans();
        m_sopTransTable[SCell::NOR] = new SopNorTrans();
        m_sopTransTable[SCell::OR] = new SopOrTrans();
        m_sopTransTable[SCell::VCC] = new SopVccTrans();
        m_sopTransTable[SCell::XNOR] = new SopXnorTrans();
        m_sopTransTable[SCell::XOR] = new SopXorTrans();
    }

    SopMgr::~SopMgr() {
        m_sopMgr = 0;
    }

    SopMgr*
    SopMgr::instance()
    {
        if(!m_sopMgr) {
            m_sopMgr = new SopMgr();
        }
        return m_sopMgr;
    }

    void
    SopMgr::toSumOfProductForm()
    {
        const std::list<SBlock*>& blks = SRoot::instance()->getBlocks();

        for(std::list<SBlock*>::const_iterator blk = blks.begin(); 
            blk != blks.end(); ++blk) {
            toSumOfProductForm(*blk);
        }    
    }

    void
    SopMgr::toSumOfProductForm(SBlock *blk)
    {
        m_currBlock = blk;
		SExprTree::init();
        //Start from output ports

        const SBlock::Ports& ports = blk->getOutPorts();
        for(SBlock::Ports::const_iterator p = ports.begin();
            p != ports.end(); ++p) {
            createAssign(*p);
        }

        //Now start from block insances
        const std::list<SBlockInst*>& binsts = blk->getBlockInsts();

        for(std::list<SBlockInst*>::const_iterator binst = binsts.begin();
            binst != binsts.end(); ++binst)
        {
            const std::list<SPin*>& inpins = (*binst)->getInPins();
            for(std::list<SPin*>::const_iterator pin = inpins.begin();
                pin != inpins.end(); ++pin) {
                createAssign((*pin)->getActual());
            }
        }
        //Now start from cell insances
        //const std::list<SInst*>& cinsts = blk->getInsts();
        std::list<SInst*> cinsts = blk->getInsts();

        for(std::list<SInst*>::const_iterator cinst = cinsts.begin();
            cinst != cinsts.end(); ++cinst)
        {
            if(SopTransBase *t = getTransformer((*cinst))) 
			{
			    delete t;
                continue;
            }    
            const std::list<SPin*>& inpins = (*cinst)->getInPins();
            for(std::list<SPin*>::const_iterator pin = inpins.begin();
                pin != inpins.end(); ++pin) {
                createAssign((*pin)->getActual());
            }
        }

		std::for_each(cinsts.begin(), cinsts.end(), Deleter(blk));
    }
    
    SopTransBase*
    SopMgr::getTransformer(const SInst *inst)
    {
        SopTransTable::iterator iter = m_sopTransTable.find(inst->getCellType());
        return (iter == m_sopTransTable.end()) ? 0 :iter->second->copy();
    }

    void
    SopMgr::createAssign(SPN *spn)
    {
        if(!spn || Leaf()(spn)) {
            return;
        }
        spn->reset();
        Driver d = spn->getDriver();
        SPin *p = (*d);
        const SInst *inst = p->getCellInst();
        assert(inst);
        SExpr *expr = toSumOfProductForm(inst);
        if(expr)
		{
		    expr = SExprTree(expr)();
            m_currBlock->addAssign(new SAssign(spn, expr));
        }    
    }

    SExpr*
    SopMgr::toSumOfProductForm(const SInst *inst)
    {
        if(!inst) {
            return 0;
        }    

        SopTransBase *t = this->getTransformer(inst);
        //t->init(m_currBlock, inst);

        const std::list<SPin*>& pins = inst->getInPins();

        for(std::list<SPin*>::const_iterator pin = pins.begin();
            pin != pins.end(); ++pin)
        {
            SPN *spn = (*pin)->getActual();
            if(!spn)
            {
                t->addExpr(*pin, SExpr::tautology()); //Or SZero?
                continue;
            }
			SExpr *expr = 0;
			if(!Leaf()(spn)) 
			{
                spn->reset();
                Driver d = spn->getDriver(); //How about multiple drivers?
                SPin *p = (*d);
                const SInst *tinst = p ? p->getCellInst() : 0;
                expr = tinst ? toSumOfProductForm(tinst) : 0;
			}	
            if(!expr) {
                expr = SExprTree::createNode(spn);
            }
            t->addExpr(*pin, expr); //Or SZero?
        }
        t->transform(inst);
        SExpr *expr = t->getExpr(inst->getSpecialPin("DATAOUT"));
		delete t;
		return expr;
    }
}
