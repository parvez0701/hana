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

#include "sbase.h"

using namespace std;
namespace stview
{

    SBase::SBase(const std::string& n): name(n), attribTable(0){
    }

    SBase::~SBase(){
    }

    void
    SBase::setAttribute(const string& name_, void *val)
    {
        if(!attribTable)
        { 
            attribTable = new AttributeTable();
            attribTable->insert(make_pair(name_, val));
        }
        else
        {
            attribTable->erase(name_);
            attribTable->insert(make_pair(name_, val));
        }
    }
    void*
    SBase::getAttribute(const string& name_) const
    {
        if(!attribTable) {
            return NULL;
        }
        AttributeTable::iterator iter = attribTable->find(name_);
        if(iter == attribTable->end()) {
            return NULL;
        }
        return iter->second;
    }

    bool
    SBase::attribExists(const string& name_) const 
    {
        if(!attribTable) {
            return false;
        }
        return attribTable->find(name_) != attribTable->end();
    }    

}
