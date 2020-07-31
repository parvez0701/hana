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

#include "sinst.h"
#include "scell.h"
#include "netlistvisitorbase.h"

using std::string;
using std::list;
namespace stview
{
    SInst::SInst(const string& name, const SCell* scell): 
        SInstBase(name), cell(const_cast<SCell*>(scell)) {
    }

    SInst::~SInst(){
    }

    void
    SInst::accept(NetlistVisitorBase *nvb) {
        nvb->visit(this);
    }    

    SPort::Dir
    SInst::getDir(const string& pinName) const
    {
        if(!cell) {
            return SPort::INOUT;
            //return SPort::UNSET;
        }
        const SPort *port = cell->getPort(pinName);
        if(!port) {
            return SPort::UNSET;
        }
        return port->getDir();
    }

    list<const SPin*>
    SInst::getSpecialPins(const string& attrib) const
    {
        list<const SPort*> ports = cell->getSpecialPorts(attrib);
        list<const SPin*> pins;

        for(list<const SPort*>::const_iterator p = ports.begin();
            p != ports.end(); ++p) 
        {
            const SPin *pin = this->getPin((*p)->getName());
            assert(pin);
            pins.push_back(pin);
        }
        return pins;
    }

    const SPin*
    SInst::getSpecialPin(const string& attrib) const
    {
        list<const SPin*> pins = this->getSpecialPins(attrib);
        assert(!pins.empty());
        return pins.front();
    }
    SPin*
    SInst::getSpecialPin(const string& attrib)
    {
        list<const SPin*> pins = this->getSpecialPins(attrib);
        assert(!pins.empty());
        return const_cast<SPin*>(pins.front());
    }

    list<const SPN*>
    SInst::getSpecialActuals(const string& attrib) const
    {
        list<const SPin*> pins = this->getSpecialPins(attrib);
        list<const SPN*> acts;

        for(list<const SPin*>::const_iterator p = pins.begin();
            p != pins.end(); ++p) {
            acts.push_back((*p)->getActual());
        }
        return acts;
    }

    const SPN*
    SInst::getSpecialActual(const string& attrib) const
    {
        const SPin *pin = this->getSpecialPin(attrib);
        return pin ? pin->getActual() : 0;
    }
    SPN*
    SInst::getSpecialActual(const string& attrib)
    {
        const SPin *pin = this->getSpecialPin(attrib);
        return pin ? const_cast<SPN*>(pin->getActual()) : 0;
    }

}
