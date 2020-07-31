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

#ifndef _SUTILS_H_
#define _SUTILS_H_
#include <string>
#include <list>
#include <string.h>
#include "syixname.h"

namespace stview
{
    struct cmp_str{
        bool operator()(const std::string& s1, const std::string& s2) const{
            return strcmp(s1.c_str(), s2.c_str()) < 0;
        }
    };

    template <class T>
    struct DELETE : public std::unary_function<T, bool>
    {
        bool operator()(T *t) 
        {
            delete t;
            return true;
        }    
    };

    template < typename T >
    struct ResetVisitCount
    {
        void operator()(std::list<T*> objs)
        {
            typename std::list<T*>::iterator obj = objs.begin();
            for(; obj != objs.end(); ++obj) {
                (*obj)->resetVisitCount();
            }
        }
    };


    typedef Synth::SyIxName SIxName;
}
#endif
