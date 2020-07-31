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

#include <iostream>
#include "file.h"
#include "root.h"
#include "msghandler.h"

namespace Rtl
{
    extern Root* GRoot;

    File::~File()
    {
        for_each(modules.begin(), modules.end(), DELETE<Module>());
        for_each(udps.begin(), udps.end(), DELETE<UDP>());
    }

    FILE*
    File::open(const bool silent) const
    {
        FILE* file = fopen(name.c_str(), "r");
        if(!file && !silent){
            MsgHandler::instance()->print(10, getFileTypeStr().c_str(), name.c_str());
        }
        return file;
    }

    string
    File::getFileTypeStr() const
    {
        switch(type)
        {
            case SRC: return "source";
            case LIB: return "library";
            case INCL: return "include";
            default: return "";
        }
        return ""; //not reachable
    }

}
