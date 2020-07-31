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

#include "convisitorbase.h"
#include "sycontainer.h"
#include "gencellmgr.h"
#include "syroot.h"
#include "namemgr.h"
#include "systmt.h"

using std::string;
namespace Synth
{
       ConVisitorBase::ConVisitorBase(    const SyModule    *rmod,
                            const SyModule    *smod,
                            const SyScope    *sscope): SyVisitorBase(),
                            currRtlMod(const_cast<SyModule*>(rmod)),
                            currSynthMod(const_cast<SyModule*>(smod)),
                            currScope(const_cast<SyScope*>(sscope)/*,
                            result(0)*/) {
    }
    const string& 
    ConVisitorBase::getCurrModName() const {
        return currMod->getName();
    }    

    void
    ConVisitorBase::createBuf(SyCntv *out, SyCntv *in)
    {
        GenCellBase *bufMod = GenCellMgr::instance()->create(BUF, 1, 1);
        SyRoot::switchStage();
        SyRoot::instance()->addCellMod(bufMod);
        SyRoot::switchStage();
        SyModInst *bufInst = new SyModInst(ModuleNameManager::instance()->createName(
            currSynthMod, CellType2String(BUF)), bufMod, bufMod->getName());
        currSynthMod->addModInst(bufInst);
        SyScPin *inpin = new SyScPin(SyIxName("IN"), in, bufInst);
        SyScPin *outpin = new SyScPin(SyIxName("OUT"), out, bufInst);
        bufInst->addPin(inpin);
        bufInst->addPin(outpin);
        currSynthMod->addObject(bufInst->getIxName().toString(), bufInst);
    }
    void
    ConVisitorBase::createInv(SyCntv *out, SyCntv *in)
    {
        GenCellBase *invMod = GenCellMgr::instance()->create(NOT, 1, 1);
        SyRoot::switchStage();
        SyRoot::instance()->addCellMod(invMod);
        SyRoot::switchStage();
        SyModInst *invInst = new SyModInst(ModuleNameManager::instance()->createName(
            currSynthMod, CellType2String(NOT)), invMod, invMod->getName());
        currSynthMod->addModInst(invInst);
        SyScPin *inpin = new SyScPin(SyIxName("IN"), in, invInst);
        SyScPin *outpin = new SyScPin(SyIxName("OUT"), out, invInst);
        invInst->addPin(inpin);
        invInst->addPin(outpin);
        currSynthMod->addObject(invInst->getIxName().toString(), invInst);
    }
    SyScNet*
    ConVisitorBase::createNet()
    {
        SyScNet *net = new SyScNet(ModuleNameManager::instance()->createName(currSynthMod, "net"), SyScNet::WIRE, false);
        currSynthMod->addNet(net);
        currSynthMod->addObject(net->getIxName().toString(), net);
        return net;
    }

    int
    ConVisitorBase::hasBitPartSelect(const SyAssign *assign, bool bitSel) const
    {
        if(this->hasBitPartSelect(assign->getLExpr(), bitSel)) {
            return 1;
        }
        if(this->hasBitPartSelect(assign->getRExpr(), bitSel)) {
            return 2;
        }
        return 0;
    }    

    bool
    ConVisitorBase::hasBitPartSelect(const SyExpr *expr, bool bitSel) const
    {
        if(bitSel && dynamic_cast<const SyBitSel*>(expr)) {
            return true;
        }
        if(!bitSel && dynamic_cast<const SyPartSel*>(expr)) {
            return true;
        }
        if(!bitSel && dynamic_cast<const SyMemory*>(expr)) {
            return true;
        }
        if(const SyBinary *binary = dynamic_cast<const SyBinary*>(expr))
        {
            if(this->hasBitPartSelect(binary->getLExpr(), bitSel)) {
                return true;
            }
            if(this->hasBitPartSelect(binary->getRExpr(), bitSel)) {
                return true;
            }
        }
        if(const SyUnary *unary = dynamic_cast<const SyUnary*>(expr)){
            return this->hasBitPartSelect(unary->getExpr(), bitSel);
        }
        if(const SyCondExpr* condExpr = dynamic_cast<const SyCondExpr*>(expr))
        {
            if(this->hasBitPartSelect(condExpr->getCond(), bitSel)) {
                return true;
            }
            if(this->hasBitPartSelect(condExpr->getTrueExpr(), bitSel)) {
                return true;
            }
            if(this->hasBitPartSelect(condExpr->getFalseExpr(), bitSel)) {
                return true;
            }
        }    
        if(const SyConcat *conc = dynamic_cast<const SyConcat*>(expr))
        {
            const SyList<SyExpr*>& bits = conc->getExprs();
            for(SyNode<SyExpr*>* bit = bits.head; bit; bit = bit->next) {
                if(this->hasBitPartSelect(bit->val, bitSel)) {
                    return true;
                }
            }
        }
        return false;
    }
}
