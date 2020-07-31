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

#ifndef _SCONST_H
#define _SCONST_H

#include "scntv.h"
namespace stview
{
    class SConst: public SCntv
    {
        public:
            typedef enum {ZERO, ONE, X, Z} CType;
            static std::string ctype2Str(CType);
            static CType str2ctype(const std::string&);
            
            SConst(CType);
            virtual ~SConst();
            inline CType getCtype() const;

            virtual void accept(NetlistVisitorBase*);
        private:
            SConst(const SConst&);
            SConst& operator=(const SConst&);
    };

    SConst::CType
    SConst::getCtype() const {
        return SConst::str2ctype(this->getName());
    }
}
#endif
