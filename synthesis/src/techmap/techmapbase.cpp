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

#include "techmapbase.h"


#include "sinst.h"
#include "sblock.h"

using std::string;
using std::list;
using std::pair;
using std::make_pair;
using namespace stview;
namespace TechnologyMapping
{
    TechMapBase::TechMapBase() {
    }

    TechMapBase::~TechMapBase() {
    }

    bool
    TechMapBase::changed() const {
        return status;
    }

    void
    TechMapBase::changed(bool s) {
        status = s;
    }

    void
    TechMapBase::init() 
    {
        giter = gatesOfInterest.begin();
        status = false;
    }    

    void
    TechMapBase::clear() 
    {
        gatesOfInterest.clear();
    }

    string
    TechMapBase::getNextGateName()
    {
        if(giter == gatesOfInterest.end()) {
            return "";
        }
        string name = *giter;
        ++giter;
        return name;
    }

    TechMapBase::GatesOfInterest&
    TechMapBase::getGatesOfInterest() {
        return gatesOfInterest;
    }    

    void
    TechMapBase::addGateOfInterest(const SInst *inst)
    {
        if(!inst) {
            return;
        }
        gatesOfInterest.insert(inst->getName());
    }

    void
    TechMapBase::addGateOfInterest(const string& name) {
        gatesOfInterest.insert(name);
    }

    void
    TechMapBase::removeGateOfInterest(const SInst *inst) 
    {
        if(!inst) {
            return;
        }
        gatesOfInterest.erase(inst->getName());
    }

    void
    TechMapBase::removeGateOfInterest(const string& name) {
        gatesOfInterest.erase(name);
    }    

    bool
    TechMapBase::isGateOfInterest(const SInst *inst) const
    {
        if(!inst) {
            return false;
        }
        return gatesOfInterest.find(inst->getName()) != 
            gatesOfInterest.end();
    }

    bool
    TechMapBase::isGateOfInterest(const string& name) const {
        return gatesOfInterest.find(name) != gatesOfInterest.end();
    }

    bool
    TechMapBase::operator()(SInst *inst) 
    {
        //default implementation
        if(this->condition(inst)) 
        {
            this->addGateOfInterest(inst);
            return true;
        }    
        return false;

    }

    bool
    TechMapBase::doMapping(SBlock *block)
    {
        //default implementation
        list<string> rmlist;
        for(string name = this->getNextGateName(); !name.empty();
            name = this->getNextGateName())
        {
            SInst *inst = const_cast<SInst*>(block->getInst(name));
            if(!inst) {
                //deleted by some other processing
                continue;
            }
            if(!this->condition(inst)) {
                continue;
            }    

            if(this->doMapping(block, inst)) 
            {
                rmlist.push_back(name);
                this->changed(true);
            }    
        }

        for(list<string>::const_iterator iter = rmlist.begin();
            iter != rmlist.end(); ++iter) {
            this->removeGateOfInterest(*iter);
        }    

        return this->changed();
    }

    bool
    TechMapBase::generalCondition(const SInst*) const {
        return false;
    }    

    bool
    TechMapBase::doGeneralMapping(SBlock *block, SInst *inst) {
        return false;
    }    
    bool
    TechMapBase::doGeneralMapping(SBlock *block)
    {
        //default implementation
        list<string> rmlist;
        for(string name = this->getNextGateName(); !name.empty();
            name = this->getNextGateName())
        {
            SInst *inst = const_cast<SInst*>(block->getInst(name));
            if(!inst) {
                //deleted by some other processing
                continue;
            }
            if(!this->generalCondition(inst)) {
                continue;
            }    

            if(this->doGeneralMapping(block, inst)) 
            {
                rmlist.push_back(name);
                this->changed(true);
            }    
        }

        for(list<string>::const_iterator iter = rmlist.begin();
            iter != rmlist.end(); ++iter) {
            this->removeGateOfInterest(*iter);
        }    

        return this->changed();
    }

}
