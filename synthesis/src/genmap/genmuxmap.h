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

#ifndef _GENMUXMAP_H_
#define _GENMUXMAP_H_

#include "genmapbase.h"
namespace GenericMapping
{
    class GenericMuxMapper : public GenericMapperBase
    {
        public:
            friend class GenMapMgr;
            //classification based on select size
            typedef enum {UNSET, MUX_1, MUX_2, MUX_3, MUX_4, MUX_5, MUX_6} MuxType;
        public:
            virtual void doMapping();
            virtual    const stview::SCell* getCell() const;

            void    setType(MuxType);
        protected:
            virtual ~GenericMuxMapper();
            virtual GenericMuxMapper* create(stview::SBlock*) const;

            virtual    void setPinMapList();
        private:
            GenericMuxMapper();
            GenericMuxMapper(stview::SBlock*);

            std::pair<int, int> getInputSelectSize() const;

            std::vector<std::pair<stview::SPN*, stview::SPN*> >
                createSelCntvAndInvVec(const Synth::SyModInst*);
            void createMuxLogic();
            void createMuxLogic(const Synth::SyModInst*);


            std::vector<stview::SPN*> getSelectCntvs(
                const std::vector<std::pair<stview::SPN*, stview::SPN*> >&,
                int) const;


            MuxType    type;
    };
}
#endif
