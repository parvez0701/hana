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

#ifndef _GENALMAP_H_
#define _GENALMAP_H_

#include <map>

#include "genmapbase.h"

namespace Synth 
{
    class SyScPin;
}
namespace stview 
{
    class SPin;
}
namespace GenericMapping
{
    //Arithmatic/logical elemnt mapper
    class GenericALElementMapper : public GenericMapperBase
    {
        public:
            friend class GenMapMgr;
            typedef enum { UNSET, IN_1=1, IN_2=2, IN_4=4, IN_8=8, 
                IN_16=16, IN_32=32, IN_64=64 } InputType;
            
            typedef std::map<InputType, stview::SCell*> CellTable;
        protected:
            GenericALElementMapper();
            GenericALElementMapper(stview::SBlock*);
            virtual ~GenericALElementMapper();

            void setPinMapList(PinMapList&);

            virtual void init() = 0;
            InputType getInputType() const;
            stview::SCell::CellType getCellType() const;

            void setCellType(stview::SCell::CellType);
            void setInputType(InputType);
            std::vector<InputType> getCellTypeVec(int) const;

            void nextInState() const;
            void prevInState() const;
            bool isUnsetState() const;
            int  getInputSize(const Synth::SyModInst*, const std::string&) const;
            std::vector<Synth::SyScPin*> getPins(const Synth::SyModInst*, 
                const std::string&) const;
            std::vector<stview::SPin*> getPins(const stview::SInst*, 
                const std::string& name) const;    
                
            
        private:
            GenericALElementMapper(const GenericALElementMapper&);
            GenericALElementMapper& operator=(const GenericALElementMapper&);

            mutable InputType inType;
            stview::SCell::CellType cellType;
    };
}
#endif
