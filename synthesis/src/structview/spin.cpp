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

#include "spin.h"
#include "sinst.h"
#include "sblockinst.h"
#include "scell.h"
#include "netlistvisitorbase.h"

using std::string;
namespace stview
{
    SPin::SPin(const string& name): SCntv(name), actual(0), inst(0){
    }

    SPin::SPin(const string& name, const SPN *act, const SInstBase *ins) :
        SCntv(name), actual(const_cast<SPN*>(act)), 
        inst(const_cast<SInstBase*>(ins)) 
    {
        setDriverAndSink();
    }    

    SPin::~SPin() {
        removeDriverAndSink();
    }

    SPort::Dir
    SPin::getDir() const
    {
        return inst->getDir(this->getName());
    }

    const SInst*
    SPin::getCellInst() const{
        return dynamic_cast<SInst*>(inst);
    }

    const SBlockInst*
    SPin::getBlockInst() const{
        return dynamic_cast<SBlockInst*>(inst);
    }    

    void
    SPin::accept(NetlistVisitorBase *nvb) {
        nvb->visit(this);
    }    


    void
    SPin::setDriverAndSink()
    {
        if(actual) 
        {
            SPort::Dir dir = this->getDir();
            if(dir == SPort::IN) {
                actual->addSink(this);
            }
            else if(dir == SPort::OUT) {
                actual->addDriver(this);
            }
            else
            {
                actual->addDriver(this);
                actual->addSink(this);
            }    
        }
    }    

    void
    SPin::removeDriverAndSink()
    {
        if(!actual) {
            return;
        }    
        actual->removeSink(this);
        actual->removeDriver(this);
        actual = 0;
    }    
}
