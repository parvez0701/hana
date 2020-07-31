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

#ifndef _GENBLOCKINSTMAP_H_
#define _GENBLOCKINSTMAP_H_

#include "genmapbase.h"
namespace GenericMapping
{
    class GenericBlockInstMapper : public GenericMapperBase
    {
        friend class GenMapMgr;
        public:
            virtual void doMapping();
        protected:
            virtual ~GenericBlockInstMapper();
            virtual GenericBlockInstMapper* create(stview::SBlock*) const;
        private:
            GenericBlockInstMapper();
            GenericBlockInstMapper(stview::SBlock*);
    };
}
#endif
