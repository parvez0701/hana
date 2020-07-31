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
#include "tickdefine.h"

using std::map;
using std::string;
using std::make_pair;

namespace Rtl
{
    TickDefine* TickDefine::tickDefine = 0;
    TickDefine::TickDefine() {
    }

    TickDefine::~TickDefine() {
        tickDefine = 0;
    }

    TickDefine*
    TickDefine::instance()
    {
        if(!tickDefine) {
            tickDefine = new TickDefine();
        }
        return tickDefine;
    }

    string
    TickDefine::getRepText(const string & str) const
    {
        DefineTable::const_iterator iter = defineTable.find(str);
        if(iter == defineTable.end()) {
            return "";
        }
        return iter->second;
    }

    void
    TickDefine::addRepText(const string& name, const string &text)
    {
        defineTable.erase(name);
        defineTable.insert(make_pair(name, text));
    }

    bool
    TickDefine::isDefined(const string &name) const {
        return !this->getRepText(name).empty();
    }    

    void
    TickDefine::clearDefines() {
        defineTable.clear();
    }
    bool
    TickDefine::undef(const string& str)
    {
        if(!isDefined(str)) {
            return false;
        }
        defineTable.erase(str);
        return true;
    }

    bool
    TickDefine::process(const string& str)
    {
        string tstr = str;
        int index = tstr.find_first_not_of(" \t");
        if(index != string::npos) {
            tstr = tstr.substr(index);
        }
        else {
            return false;
        }    
        index = tstr.find_first_of(" \t\n\r");
        if(index == string::npos) 
        {
            this->addRepText(tstr, " ");
            return true;
        }
        else
        {
            string tmp = tstr.substr(0, index); 
            tstr = tstr.substr(index);
            index = tstr.find_first_not_of("     ");
            if(index == string::npos) {
                tstr = " ";
            }
            else
            {
                tstr = tstr.substr(index);
                tstr = tstr.substr(0, tstr.find_first_of("\r\n"));
                index = tstr.find_first_of("/");
                if((index != string::npos) && (tstr[index+1] == '/')) {
                    //Remove comments
                    tstr = tstr.substr(0, index);
                }    
            }    
            if(tstr.empty()) {
                tstr.push_back(' ');
            }    
            this->addRepText(tmp, tstr);
        }
        return true;
    }

    
}
