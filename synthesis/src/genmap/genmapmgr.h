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

#ifndef _GENMAPMGR_H_
#define _GENMAPMGR_H_
#include <list>
#include "scell.h"

namespace stview
{
    class SBlock;
}
namespace GenericMapping
{
    class GenericMapperBase;
    class GenMapMgr
    {
        public:
            static GenMapMgr*    instance();
            virtual ~GenMapMgr();

            void doMapping();
            const stview::SCell* getCell(stview::SCell::CellType, int) const;
        private:
            GenMapMgr();
            GenMapMgr(const GenMapMgr&);
            GenMapMgr& operator=(const GenMapMgr&);

            void    init();
            void    doModuleToBlockMapping();
            void    doGeneralMapping();
            void    doInstMasterMapping();

            static GenMapMgr*                genMapMgr;
            std::list<stview::SBlock*>        blocks;
            std::list<GenericMapperBase*>    mappers;
    };
}
#endif
