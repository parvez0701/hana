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

#ifndef _SEMCHECK_H_
#define _SEMCHECK_H_
#include <string>
#include <set>

#include "utils.h"
namespace Rtl
{
    class File;
    class ModInst;
    class Module;
    class UDPInst;
    class SemanticChecker;
    class SemCheck
    {
        public:
            static SemCheck* instance();
            ~SemCheck();
            int doSemChecks();
            bool isUndefMod(const std::string& name) const{
                if(undefMods.find(name) != undefMods.end()){
                    return true;
                }
                return false;
            }
            void remUndefMod(const std::string& name){
                undefMods.erase(name);
            }

            bool noUndefMods() const{
                return undefMods.empty();
            }
            int resolveDefparams();
        private:
            SemCheck();
            SemCheck(const SemCheck&);
            SemCheck& operator=(const SemCheck&);
            int resolveInstMaster();
            int resolveInstMaster(const std::vector<File*>&);
            int resolveUDPInsts(const std::vector<ModInst*>&, Module*) const;
            int resolvePins(ModInst*) const;
            int resolvePins(UDPInst*) const;
            int resolveInstParams(const ModInst*) const;
            int readLibFiles();
            int resolveDefparams(Module*, const Path&);
            int checkAndUnrollGenerateStatements();
            int checkAndUnrollGenerateStatements(const std::vector<File*>&);
            int checkForRecursiveInstantiation() const;
            int checkForRecursiveInstantiation(const std::vector<File*>&) const;
            int checkForRecursiveInstantiation(const Module*, 
                                            std::set<std::string>) const;
            
            int doSemChecks(SemanticChecker*);
            //int checkContAssign(Module*) const;
            //int checkContAssign(Module*, ContAssign*) const;
            static SemCheck* semcheck;
            std::set<std::string, compare_str> undefMods;
    };

}
#endif
