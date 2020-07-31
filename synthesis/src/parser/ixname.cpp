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

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "ixname.h"
using std::string;
namespace Rtl
{
    const signed long IxName::nix = LONG_MIN;
    IxName::IxName(const IxName& rhs)
    {
        index = rhs.getIndex();
        name = rhs.getName();
    }

    IxName::IxName(const string& str)
    {
        string::size_type ix1 = str.find_first_of('[');
        string::size_type ix2 = str.find_first_of(']');

        if((ix1 == string::npos) || (ix2 == string::npos)) 
        {
            index = LONG_MIN;
            name = str;
        }
        else
        {
            name = str.substr(0, ix1);
            string tmp = str.substr(ix1+1, ix2-ix1-1);
            index = atol(tmp.c_str());
        }    
    }

    const IxName&
    IxName::operator=(const IxName& rhs)
    {
        index = rhs.getIndex();
        name = rhs.getName();
        return *this;
    }

    bool
    IxName::operator==(const IxName& rhs) const {
        return (name == rhs.getName()) && (index == rhs.getIndex());
    }

    bool
    IxName::operator!=(const IxName& rhs) const{
        return !(*this == rhs);
    }

    string
    IxName::toString() const 
    {
        if(index == nix){
            return name;
        }
        char str [1024];
        sprintf(str, "%s[%ld]", name.c_str(), index);
        return str;
    }


}
