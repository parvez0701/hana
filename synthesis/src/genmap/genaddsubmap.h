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

#ifndef _GENADDSUBMAP_H
#define _GENADDSUBMAP_H

#include <map>

#include "genalmap.h"
namespace GenericMapping
{
    class GenericAddSubMapper : public GenericALElementMapper
    {
        public:
            friend class GenMapMgr;
        public:
            virtual void doMapping();
            virtual const stview::SCell* getCell() const;
        protected:
            virtual ~GenericAddSubMapper();
            virtual GenericAddSubMapper* create(stview::SBlock*) const;
            virtual void setPinMapList();
            virtual void init();
        private:
            GenericAddSubMapper();
            GenericAddSubMapper(stview::SBlock*);
            GenericAddSubMapper(const GenericAddSubMapper&);
            GenericAddSubMapper& operator=(const GenericAddSubMapper&);
            void initAdd();
            void initSub();
            void setPinMapList(PinMapList&, int size) const;
            int  getInputSize() const;
            std::vector<stview::SCell*> getCells(stview::SCell::CellType, int) const;
            void createInOutPins(stview::SInst*) const;
            void createSpecialPins(stview::SInst*, stview::SCell::CellType) const;

            
            void doAdderMapping(); 
            void doSubMapping(); 
            void doGeneralMapping(const InstancePredicate&);

            std::vector<stview::SInst*> createCellInstances(
                const std::vector<stview::SCell*>&,
                const std::string&) const;
            
            void stitchCellInstances(std::vector<stview::SInst*>&,
                stview::SCell::CellType, const Synth::SyModInst*);

            void stitchCellInstances(std::vector<stview::SInst*>&,
                const Synth::SyModInst*, const std::string&, 
                const std::string&,
                bool) const;
            
            void stitchCarryPins(std::vector<stview::SInst*>&,
                const Synth::SyModInst*,
                stview::SCell::CellType);

            CellTable    addTable;
            CellTable    subTable;
    };
}
#endif
