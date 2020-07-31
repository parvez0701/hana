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

#ifndef _GENCMPMAP_H
#define _GENCMPMAP_H

#include <map>

#include "genalmap.h"
namespace GenericMapping
{
    //Arithmatic/logical elemnt mapper
    class GenericCmpMapper : public GenericALElementMapper
    {
        public:
            friend class GenMapMgr;
            typedef enum {UNSET, EQ, NEQ, LT, GT, LTE, GTE } CmpType;
            virtual void doMapping();
            virtual const stview::SCell* getCell() const;
        protected:
            GenericCmpMapper();
            GenericCmpMapper(stview::SBlock*);
            virtual ~GenericCmpMapper();

            virtual GenericCmpMapper* create(stview::SBlock*) const;
            virtual void setPinMapList();

            virtual void init();
        private:
            GenericCmpMapper(const GenericCmpMapper&);
            GenericCmpMapper& operator=(const GenericCmpMapper&);

            void    doGeneralMapping();
            void    doGeneralMapping(const Synth::SyModInst*);

            void    createEQLogic(const Synth::SyModInst*);
            void    createNEQLogic(const Synth::SyModInst*);
            void    createLTLogic(const Synth::SyModInst*);
            void    createGTLogic(const Synth::SyModInst*);
            void    createLTELogic(const Synth::SyModInst*);
            void    createGTELogic(const Synth::SyModInst*);

            void    createLTorGTLogic(const Synth::SyModInst*, bool, bool=false);

            CmpType getComparisonType(const Synth::SyModInst*) const;
            
    };
}
#endif
