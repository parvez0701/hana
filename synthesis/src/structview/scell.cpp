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

#include "scell.h"
#include "sport.h"

#include "netlistvisitorbase.h"

using std::string;
using std::list;
using std::map;

namespace stview
{
    //--------------------------------------------------------------------------
    static const int AttribCount = 15;
    static const char * SPortAttribs[] = { "DATAIN", "DATAOUT", "ENABLE",
        "SELECT", "CIN", "COUT", "CLOCK", "RESET", "SET", "SHIFT",
        "SHIFTVAL", "EQ", "UEQ", "GT", "LT"};
    
    //--------------------------------------------------------------------------
    static bool HasAttrib(const SPort *p)
    {
        for(int i = 0; i < AttribCount; ++i) {
            if(p->attribExists(SPortAttribs[i])) {
                return true;
            }
        }
        return false;
    }

    SCell::SCell(const std::string& n, CellType ct, bool gen) : 
        cellType(ct), 
        SBase(n),
        generic(gen){
    }

    SCell::~SCell() {
    }

    const SPort*
    SCell::getPort(const string& pname) const 
    {
        for(list<SPort*>::const_iterator port = ports.begin();
            port != ports.end(); ++port){
            if((*port)->getName() == pname) {
                return *port;
            }
        }
        return 0;
    }

    void
    SCell::accept(NetlistVisitorBase* nvb) {
        nvb->visit(this);
    }    

    const SPort*
    SCell::getSpecialPort(const string& attrib) const
    {
        for(list<SPort*>::const_iterator port = ports.begin();
            port != ports.end(); ++port){
            if((*port)->attribExists(attrib)) {
                return *port;
            }
        }
        return 0;
    }

    //const SCell::SpecialPorts&
    //SCell::getSpecialPorts() const {
    //    return specialPorts;
    //}    

    list<const SPort*>
    SCell::getSpecialPorts(const string& attrib) const
    {
        list<const SPort*> sps;
        for(list<SPort*>::const_iterator sp = ports.begin();
            sp != ports.end(); ++sp) {
            if((*sp)->attribExists(attrib)) {
                sps.push_back(*sp);
            }
        }
        return sps;
            
    }

    void
    SCell::addPort(const SPort* p)
    {
        //note unchecked addition
        ports.push_back(const_cast<SPort*>(p));

        //for(int i = 0; i < AttribCount; ++i) {
        //    if(p->attribExists(SPortAttribs[i])) {
        //        specialPorts.insert(make_pair(p->getIxName().toString(), p));
        //    }
        //}    
    }

    bool
    SCell::doChecks() const
    {
        return true;
    }

    void operator++(SCell::CellType& ct) {
        ct = (SCell::CellType)(ct + SCell::CELLTYPE_BEGIN);
    }    
    SCell::CellType operator++(SCell::CellType& ct, int) 
    {
        SCell::CellType tmp = ct;
        ct = (SCell::CellType)(ct + SCell::CELLTYPE_BEGIN);
        return tmp;
    }


}
