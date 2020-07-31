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

#include "namemgr.h"
#if 0
namespace Synth
{
    template<class T>
    NameMgr<T>* NameMgr<T>::nameMgr = 0;

    template<class T>
    NameMgr<T>* NameMgr<T>::instance()
    {
        if(!NameMgr<T>::nameMgr){
            NameMgr<T>::nameMgr = new NameMgr<T>();
        }
        return NameMgr<T>::nameMgr;
    }    
    template<class T> std::string
    NameMgr<T>::createName(const T *mod, const std::string& hint)
    {
        if(!mod) {
            return "";
        }
        std::string thint = hint;
        if(thint.empty()){
            thint = std::string("synth_wire");
        }

        ModuleMap::iterator iter1 = modMap.find(mod->getName());
        PrefixCountMap::iterator iter2;
        int count = 0;
        if(iter1 != modMap.end()){
            iter2 = iter1->second.find(thint);
            if(iter2 != iter1->second.end()){
                count = iter2->second;
            }
        }

        std::string name = std::string("synth_") + thint;
        while(1)
        {
            if(!mod->findObject(name)){
                break;
            }
            name = std::string("synth_") + thint;
            std::ostringstream str;
            str << name << "_" << count++;
            name = str.str();
        }
        if(iter1 == modMap.end()) {
            PrefixCountMap pmap;
            pmap.insert(std::make_pair(thint, count));
            modMap.insert(    std::make_pair(mod->getName(), pmap));
        }
        else if (iter2 == iter1->second.end()) {
            iter1->second.insert(std::make_pair(thint, count));
        }
        else {
            iter2->second = count;
        }
        return name;
    }    
    
}
#endif
