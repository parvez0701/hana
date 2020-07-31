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

#ifndef _GENSHIFTMAP_H_
#define _GENSHIFTMAP_H_

#include <set>

#include "genalmap.h"
namespace GenericMapping
{
    class GenericShiftMapper : public GenericALElementMapper
    {
        public:
            friend class GenMapMgr;
            static std::set<const Synth::SyExpr*> notOutExprs;
        public:
            virtual void doMapping();
            virtual const stview::SCell* getCell() const;
            

        protected:
            GenericShiftMapper();
            GenericShiftMapper(stview::SBlock*);
            virtual ~GenericShiftMapper();

            virtual GenericShiftMapper* create(stview::SBlock*) const;

            virtual void setPinMapList();
            virtual void init();
        private:
            GenericShiftMapper(const GenericShiftMapper&);
            GenericShiftMapper& operator=(const GenericShiftMapper&);

            void doMapping(bool left);
            void doGeneralMapping();
            void doGeneralMapping(bool left);
            void doGeneralMapping(const Synth::SyModInst*,
                const stview::SCell*, std::pair<int, int>);

            std::string getCellName() const;
            std::pair<int, int> getMapSize(const Synth::SyModInst*) const;

            std::pair<int, int> getInputSize(const Synth::SyModInst* = 0) const;
    };
}
#endif
