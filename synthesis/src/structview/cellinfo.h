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

#ifndef _CELLINFO_H_
#define _CELLINFO_H_

namespace stview 
{
    class SCell;
    class CellInfo
    {
        public:
            virtual         bool doChecks() const = 0;
        protected:
            virtual         CellInfo* create(SCell*) = 0;
        private:
            CellInfo();
            CellInfo(const CellInfo&);
            CellInfo& operator=(const CellInfo&);
            ~CellInfo();

            SCell            *cell;
    };
}
#endif
