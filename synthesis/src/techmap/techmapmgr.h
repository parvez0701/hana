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

#ifndef _TECHMAPMGR_H_
#define _TECHMAPMGR_H_

#include <list>
#include <vector>

#include "scell.h"

namespace stview 
{
    class SInst;
}

namespace TechnologyMapping
{
    class TechMapBase;
    class TechMapMgr
    {
        public:
            typedef std::vector<const stview::SCell*> SCells; //indexed by num inputs
            typedef std::vector<SCells> SCellTable; // indexed by cell type
            typedef std::vector<const stview::SCell*> SCellMaxTable;
            static TechMapMgr* instance();
            ~TechMapMgr();

            void doMapping();
            bool cellExists(stview::SCell::CellType, int) const;
            const stview::SCell* getCell(stview::SCell::CellType, int) const;
            const stview::SCell* getCell(stview::SCell::CellType) const;
            const stview::SCell* getCellMinInput(stview::SCell::CellType) const;
            const stview::SCell* getCellMaxInput(stview::SCell::CellType) const;
            int getInputSize(const stview::SCell*) const;
            int getInputSize(const stview::SInst*) const;
        private:
            TechMapMgr();

            TechMapMgr(const TechMapMgr&);
            TechMapMgr& operator=(const TechMapMgr&);

            void init();

            std::list<TechMapBase*> mappers;
            SCellTable cellTable;
            SCellMaxTable cellMaxTable;
            static TechMapMgr *techMapMgr;
    };
}
#endif
