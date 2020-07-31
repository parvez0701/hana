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

#ifndef _SBASE_H_
#define _SBASE_H_

#include <stdlib.h>
#include <string>
#include <map>

#include <sutils.h>

namespace stview
{
    typedef std::map<std::string, void*, cmp_str> AttributeTable;
    class NetlistVisitorBase;
    //Base class for all structural objects like cell, port net etc.
    class SBase
    {
        public:
            SBase(const std::string&);
            virtual ~SBase();
            inline const std::string& getName() const;
            void    setAttribute(const std::string&, void *);
            void*    getAttribute(const std::string&) const;
            bool    attribExists(const std::string&) const;
            virtual void accept(NetlistVisitorBase*) = 0;
        private:
            SBase(const SBase&);
            SBase& operator=(const SBase&);
            std::string name;
            AttributeTable *attribTable;
    };

    //inline functions
    const std::string&
    SBase::getName() const{
        return name;
    }

}

#endif
