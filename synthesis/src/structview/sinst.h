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

#ifndef _SINST_H_
#define _SINST_H_
#include <list>
#include "sinstbase.h"
#include "scell.h"

namespace stview
{
//    class SCell;
    class SInst: public SInstBase
    {
        friend class SBlock;
        public:
            SInst(const std::string&, const SCell*);
            inline const SCell* getCell() const;
            virtual void accept(NetlistVisitorBase*);
            virtual SPort::Dir    getDir(const std::string&) const;

            inline SCell::CellType getCellType() const;
            inline bool isGeneric() const;

            std::list<const SPin*> getSpecialPins(const std::string&) const;
            const SPin* getSpecialPin(const std::string&) const;
            SPin* getSpecialPin(const std::string&);

            std::list<const SPN*> getSpecialActuals(const std::string&) const;
            const SPN* getSpecialActual(const std::string&) const;
            SPN* getSpecialActual(const std::string&);
        protected:    
            virtual ~SInst();
        private:
            SCell *cell;
    };

    const SCell*
    SInst::getCell() const {
        return cell;
    }

    SCell::CellType
    SInst::getCellType() const {
        assert(cell);
        return cell->getCellType();
    }    

    bool
    SInst::isGeneric() const {
        return cell->isGeneric();
    }    

    typedef ResetVisitCount<SInst> ResetVisitCountInst;
}
#endif
