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

#ifndef _TECHMAPBASE_H_
#define _TECHMAPBASE_H_
#include <set>

#include "sutils.h"


namespace stview
{
    class SInst;
    class SBlock;
}
namespace TechnologyMapping
{
    class TechMapBase
    {
        public:
            typedef std::set<std::string, stview::cmp_str>
                GatesOfInterest;
            typedef GatesOfInterest::const_iterator GatesOfInterestIter;    
        
        public:
            TechMapBase();
            virtual ~TechMapBase();
            //call this operator to add an instance to be processed 
            virtual bool operator()(stview::SInst*);    
            virtual bool doMapping(stview::SBlock *);    
            virtual bool doGeneralMapping(stview::SBlock *);    

            virtual std::string getNameHint() const = 0;

            void init();
            virtual void clear();
            bool changed() const;
        protected:
            void changed(bool);
            void addGateOfInterest(const stview::SInst*);
            void addGateOfInterest(const std::string&);
            void removeGateOfInterest(const std::string&);
            void removeGateOfInterest(const stview::SInst*);
            bool isGateOfInterest(const stview::SInst*) const;
            bool isGateOfInterest(const std::string&) const;

            GatesOfInterest& getGatesOfInterest();
            std::string getNextGateName();

            virtual bool condition(const stview::SInst*) const = 0;
            virtual bool doMapping(stview::SBlock *, stview::SInst*) = 0;    

            virtual bool generalCondition(const stview::SInst*) const;
            virtual bool doGeneralMapping(stview::SBlock *, stview::SInst*);

        private:        
            TechMapBase(const TechMapBase&);
            TechMapBase& operator=(const TechMapBase&);
    
            bool status;
            GatesOfInterest gatesOfInterest;
            GatesOfInterestIter giter;
    };
}
#endif
