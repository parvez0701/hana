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

#ifndef _FILE_H_
#define _FILE_H_
#include <string>
#include <vector>
#include "utils.h"
using std::string;
namespace Rtl
{
    class Module;
    class UDP;
    class File
    {
        public:
            typedef enum {UNSET, SRC, LIB, INCL} TYPE;
            File(const string& _name, const TYPE t): name(_name),
                type(t){
            }

            virtual ~File();
            const string& getName() const{
                return name;
            }
            bool isLibFile() const {
                return type == LIB;
            }
            bool isSrcFile() const{
                return type == SRC;
            }
            bool isInclFile() const {
                return type == INCL;
            }
            FILE* open(const bool=false) const;
            const std::vector<Module*>& getModules() const{
                return modules;
            }
            void addModule(const Module* mod){
                modules.push_back(const_cast<Module*>(mod));
            }
            void addUDP(const UDP* udp){
                udps.push_back(const_cast<UDP*>(udp));
            }
    
            const std::vector<UDP*>& getUDPs() const{
                return udps;
            }
            std::string getFileTypeStr() const;
            friend class Root;
        private:
            File();
            File(const File&);
            const File& operator=(const File&);
            string name;
            TYPE type;
            std::vector<Module*> modules;
            std::vector<UDP*> udps;
    };
}
#endif
