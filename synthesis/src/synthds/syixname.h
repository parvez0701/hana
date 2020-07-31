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

#ifndef _SYIXNAME_H_
#define _SYIXNAME_H_
#include <string>
namespace Synth
{
    class SyIxName
    {
        public:
            static    const signed long nix;
            SyIxName(): index(nix){
            }
            SyIxName(const std::string& n);

            SyIxName(const std::string& n, const long i): name(n), index(i){
            }

            SyIxName(const SyIxName&);
            const    SyIxName& operator=(const SyIxName&);
            bool operator==(const SyIxName&)const;
            bool operator!=(const SyIxName&)const;

            const    std::string& getName() const{
                return name;
            }

            long     getIndex()const{
                return index;
            }

            std::string toString() const;
            
        private:
            std::string name;
            long index;
    };
}
#endif
