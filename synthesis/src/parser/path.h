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

#ifndef _FPATH_H_
#define _FPATH_H_
#include <string>
#include <stack>
#include <vector>
namespace Rtl
{
    class Path
    {
        public:
            Path(const std::string& ="");
            bool operator==(const Path&) const;
            bool operator!=(const Path&) const;
            void descend(const std::string&);
            std::string ascend();
            std::string toString(const bool remSep=false) const;
            std::string top() const;
            std::vector<std::string> instnames() const;
        private:
            std::stack<std::string> pathStack;
    };

    Path operator+(const Path&, const Path&);
}
#endif
