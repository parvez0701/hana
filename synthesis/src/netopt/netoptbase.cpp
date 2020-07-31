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


#include "netoptbase.h"


#include "sinst.h"

using std::string;
using std::pair;
using std::make_pair;
using namespace stview;
namespace opt
{
    NetOptBase::NetOptBase() : status(true) {
    }

    NetOptBase::~NetOptBase() {
    }

    bool
    NetOptBase::changed() const {
        return status;
    }

    void
    NetOptBase::changed(bool s) {
        status = s;
    }    

    void
    NetOptBase::init() 
    {
        giter = gatesOfInterest.begin();
        status = false;
    }    

    void
    NetOptBase::clear() 
    {
        gatesOfInterest.clear();
    }

    string
    NetOptBase::getNextGateName()
    {
        if(giter == gatesOfInterest.end()) {
            return "";
        }
        string name = *giter;
        ++giter;
        return name;
    }

    NetOptBase::GatesOfInterest&
    NetOptBase::getGatesOfInterest() {
        return gatesOfInterest;
    }    

    void
    NetOptBase::addGateOfInterest(const SInst *inst)
    {
        if(!inst) {
            return;
        }
        gatesOfInterest.insert(inst->getName());
    }

    void
    NetOptBase::addGateOfInterest(const string& name) {
        gatesOfInterest.insert(name);
    }

    void
    NetOptBase::removeGateOfInterest(const SInst *inst) 
    {
        if(!inst) {
            return;
        }
        gatesOfInterest.erase(inst->getName());
    }

    void
    NetOptBase::removeGateOfInterest(const string& name) {
        gatesOfInterest.erase(name);
    }    

    bool
    NetOptBase::isGateOfInterest(const SInst *inst) const
    {
        if(!inst) {
            return false;
        }
        return gatesOfInterest.find(inst->getName()) != 
            gatesOfInterest.end();
    }

    bool
    NetOptBase::isGateOfInterest(const string& name) const {
        return gatesOfInterest.find(name) != gatesOfInterest.end();
    }

}
