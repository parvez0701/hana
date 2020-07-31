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

#ifndef _GENINCMAP_H_
#define _GENINCMAP_H_

#include "genalmap.h"

namespace GenericMapping
{
    class GenericIncMapper : public GenericALElementMapper
    {
        friend class GenMapMgr;
        public:
            virtual void doMapping();
            virtual const stview::SCell* getCell() const;
        protected:
            virtual ~GenericIncMapper();
            virtual GenericIncMapper* create(stview::SBlock*) const;
            virtual void setPinMapList();
            virtual void init();
        private:
            GenericIncMapper();
            GenericIncMapper(stview::SBlock*);
            GenericIncMapper(const GenericIncMapper&);
            GenericIncMapper& operator=(const GenericIncMapper&);

            std::string getCellName() const;
            void doGeneralMapping();
            void addPins(const Synth::SyModInst*, stview::SInst*, 
                const std::string&, const std::string&) const;
            void fillExtraBits(stview::SInst*, const std::string&, 
                int, int) ;    

    };
}
#endif
