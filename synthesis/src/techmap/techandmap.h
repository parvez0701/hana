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

#ifndef _TECHANDMAP_H_
#define _TECHANDMAP_H_

#include "techaonxnbase.h"

namespace TechnologyMapping
{
    class TechAndMap : public TechAONXNBase
    {
        public:
            TechAndMap(): TechAONXNBase(stview::SCell::AND) { }
            virtual ~TechAndMap() { }
            virtual std::string getNameHint() const { return "AND"; }
        protected:
            virtual bool generalCondition(const stview::SInst*) const;
            virtual bool doGeneralMapping(stview::SBlock *, stview::SInst*);
        private:
            TechAndMap(const TechAndMap&);
            TechAndMap& operator=(const TechAndMap&);

            bool doDemergedMapping(stview::SBlock*, stview::SInst*);
            bool doNandNotMapping(stview::SBlock*, stview::SInst*);
            bool doNandMapping(stview::SBlock*, stview::SInst*);

            bool doDirectNandNotMapping(stview::SBlock*, stview::SInst*);

    };
}
#endif
