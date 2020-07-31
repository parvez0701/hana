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


#ifndef _INVABS_H_
#define _INVABS_H_

//absorbes inverter and creates a merged gate, eg. and+not=nand
#include "netoptbase.h"
#include "scell.h"

namespace opt
{
    class InvAbs: public NetOptBase
    {
        public:
            InvAbs();
            virtual ~InvAbs();
            virtual bool operator()(stview::SInst*);    
            virtual bool optimize(stview::SBlock *);
        private:
            InvAbs(const InvAbs&);
            InvAbs& operator=(const InvAbs&);

            bool condition(const stview::SInst*) const;
            bool absorbInverter(stview::SBlock*, stview::SInst*);
            std::pair<std::string, stview::SCell::CellType>
                getNameAndType(const stview::SInst*) const;

            
    };
    
}
#endif
