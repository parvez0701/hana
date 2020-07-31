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


#ifndef _TICKDEFINE_H_
#define _TICKDEFINE_H_
#include "defines.h"
namespace Rtl
{
    class TickDefine
    {
        public:
            static TickDefine* instance();
            ~TickDefine();
            
            std::string getRepText(const std::string&) const;
            void addRepText(const std::string&, const std::string&);
            bool isDefined(const std::string&) const;

            void clearDefines();
            bool process(const std::string&);
            bool undef(const std::string&);
        private:
            TickDefine();
            TickDefine(const TickDefine&);
            TickDefine& operator=(const TickDefine&);
            typedef std::map<std::string, std::string, compare_str> DefineTable;

            DefineTable defineTable;
            static TickDefine *tickDefine;
    };
}
#endif
