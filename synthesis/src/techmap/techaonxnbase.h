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

#ifndef _TECHAONXNBASE_H_
#define _TECHAONXNBASE_H_

#include <vector>

#include "techmapbase.h"

#include "scell.h"
namespace stview 
{
    class SPN;
}
namespace TechnologyMapping
{
    class TechAONXNBase : public TechMapBase
    {
        public:
            TechAONXNBase(stview::SCell::CellType);
            virtual ~TechAONXNBase();
        
        protected:
            virtual bool condition(const stview::SInst*) const;
            virtual bool doMapping(stview::SBlock *, stview::SInst*);

            virtual std::string getNameHint() const = 0;
            stview::SCell::CellType getType() const;

            stview::SInst* createInstance(stview::SBlock*, int input, 
                std::list<const stview::SPN*>::iterator&,
                std::list<const stview::SPN*>::iterator&);
            void connectInstances(stview::SBlock*, stview::SInst*, stview::SInst*);
            void setOutActual(stview::SInst*, stview::SInst*);
        private:
            //disabled ctors and ops
            TechAONXNBase();
            TechAONXNBase(const TechAONXNBase&);
            TechAONXNBase& operator=(const TechAONXNBase&);    
            //functions
            bool condition(const stview::SInst*, int) const;

            void getInsts2BeMerged(const stview::SInst*, std::vector<const stview::SInst*>&) const;
            void merge(stview::SBlock*, std::vector<const stview::SInst*>&) const;
            //data
            stview::SCell::CellType type;
    };
}
#endif
