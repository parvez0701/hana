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

#include <algorithm>

#include "path.h"


using namespace std;
namespace Rtl
{
    Path::Path(const string& step){
        if(!step.empty()){
            pathStack.push(step);
        }
    }

    bool
    Path::operator==(const Path& rhs) const
    {
        return this->toString() == rhs.toString();
    }

    bool
    Path::operator!=(const Path& rhs) const
    {
        return !(*this == rhs);
    }

    string
    Path::ascend()
    {
        string retStr = pathStack.top();
        pathStack.pop();
        return retStr;
    }

    void
    Path::descend(const string& step) {
        pathStack.push(step);
    } 

    string
    Path::toString(const bool remSep) const
    {
        stack<string> stk = pathStack;
        string retStr;

        while(!stk.empty())
        {
            string step = stk.top();
            if(!step.empty()){
                if(remSep){
                    retStr = step + retStr;
                }
                else{
                    retStr = step + string("/") + retStr;
                }
            }
            stk.pop();
        }
        return retStr;
    }

    string
    Path::top() const
    {
        stack<string> stk = pathStack;
        string name;

        while(!stk.empty())
        {
            name = stk.top();
            stk.pop();
        }
        return name;
    }

    vector<string>
    Path::instnames() const
    {
        vector<string> inv;
        stack<string> stk = pathStack;

        while(!stk.empty())
        {
            inv.push_back(stk.top());
            stk.pop();
        }
        inv.pop_back();
        std::reverse(inv.begin(), inv.end());
        return inv;
    }

    Path
    operator+(const Path& path1, const Path& path2){
        Path tpath1 = path1;
        Path tpath2 = path2;
        tpath1.descend(tpath2.top());
        vector<string> names = tpath2.instnames();
        const int size = names.size();
        for(int i = 0; i < size; ++i){
            tpath1.descend(names[i]);
        }
        return tpath1;
    }

}
