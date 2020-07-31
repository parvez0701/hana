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

#include "modinst.h"
#include "omvisitor.h"
#include "module.h"

using std::vector;
using std::make_pair;
using std::pair;
using std::map;

namespace Rtl
{
    void
    ModInst::accept(OMVisitorBase* v) const{
        v->visit(this);
    }

    void
    ModInst::setMaster(const Module* ms) {
        master = const_cast<Module*>(ms);
    }

    const string&
    ModInst::getModName() const {
        if(master){
            return master->getName();
        }
        else {
            return modname;
        }
    }

    ModInst*
    ModInst::copy() const
    {
        ModInst *newInst = new ModInst(    this->getName().toString(),
                                        this->getModName(),
                                        master);
        vector<NameConn*> newParams;                                
        for(unsigned int i = 0; i < params.size(); ++i) {
            newParams.push_back(new pair<string, Expr*>(
            params[i]->first, params[i]->second->copy()));
        }

        newInst->setParams(newParams);
        map<string, VecPin*, compare_str>::const_iterator beginIter = vecpins.begin();
        map<string, VecPin*, compare_str>::const_iterator endIter = vecpins.end();

        for(; beginIter != endIter; ++beginIter) {
            newInst->addVecPin(beginIter->second->copy());
        }
        vector<Pin*> newPins;
        CopyVector<Pin*>(this->getPins(), newPins);
        newInst->setPins(newPins);
        return newInst;
    }

    ModInst*
    ModInst::repExpr(const string& str, const Expr *expr)
    {
        Inst::repPinActuals(str, expr);
        for(unsigned int i = 0; i < params.size(); ++i) {
            params[i]->second = const_cast<Expr*>(
            params[i]->second->repExpr(str, expr));
        }    
        return this;
    }

}
