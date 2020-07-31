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

#include <stdlib.h>
#include "gencell.h"
using std::pair;
using std::string;
using std::make_pair;
namespace Synth
{
    short
    GenCellBase::getActualInputSize(CellType t, short numIn) const
    {
#if 0    
        if(t == MUX || t == ADD || t == SUB || t == MUL || t == DIV
            || t == CMP || t == SHIFTER || t == ENC || t == DEC ||
            t == INC)
        {
            short i = 1;
            while ( i < numIn) {
                i *= 2;
            }
            return i;
        }
#endif        
        if(t == DEC || t == ENC) {
            //don't count SEL signal
            return numIn-1;
        }    
        return numIn;
    }

    pair<string, pair<short, short> >
    GenCellBase::isVecPort(const string& pname) const
    {
        string::size_type lix = pname.find_first_of('[');
        if(lix == string::npos) {
            return make_pair(pname, make_pair(-1, -1));
        }

        //watchout ! spaces are not taken into consideration
        string::size_type colon = pname.find_first_of(':');
        assert(colon != string::npos);
        //colon;
        string::size_type rix = pname.find_first_of(']');
        assert(rix != string::npos);
        return     make_pair(pname.substr(0, lix),
                make_pair(    atoi(pname.substr(lix+1, colon-lix).c_str()),
                            atoi(pname.substr(colon+1, rix-colon).c_str())));
    }

    void 
    GenCellBase::createVectorPort(    const std::string& pname,
                                    int msb,
                                    int lsb,
                                    SyScPort::SyDirType dir)
    {
        SyVPort *port = new SyVPort(pname, msb, lsb, false, dir, 
                                    dir == SyScPort::IN ? SyScNet::WIRE :
                                    SyScNet::REG);
        
        const SyList<SyScCntv*>& newbits = port->getBits();
        for(SyNode<SyScCntv*>* node = newbits.head; node; node = node->next)
        {
            const SyScPort *tport = dynamic_cast<const SyScPort*>(node->val);
            this->addPort(tport);
            this->addObject(tport->getIxName().toString(), tport);
        }
        this->addObject(pname, port);                            
    }

    void 
    GenCellBase::createScalarPort(    const std::string& pname,
                                    SyScPort::SyDirType dir) 
    {
        SyScPort *port = new SyScPort(    pname, false, dir, 
                                        dir == SyScPort::IN ? SyScNet::WIRE : 
                                        SyScNet::REG);
        this->addPort(port);
        this->addObject(pname, port);
    }    

        
}
