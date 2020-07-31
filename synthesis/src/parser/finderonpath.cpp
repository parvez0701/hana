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

#include "finderonpath.h"
#include "inst.h"
#include "path.h"
#include "root.h"
#include "module.h"

namespace Rtl
{
    extern Root* GRoot;
    const Inst*
        FindInstanceOnPath(const Path& path){
            string topname = path.top();
            vector<string> instnames = path.instnames();
            const Module* mod = dynamic_cast<const Module*>(
                    GRoot->getObject(topname));
            if(!mod){
                return 0;
            }

            int count = 0;
            const int size = instnames.size();
            const Inst* inst = 0;
            while(count < size){
                inst = dynamic_cast<const Inst*>(
                        mod->getObject(instnames[count]));
                if(!inst) {
                    break;
                }
                const ModInst* minst = dynamic_cast<const ModInst*>(inst);
                if(minst){
                    mod = minst->getMaster();
                }
                if(!mod){
                    break;
                }
            }
            return inst;
        }

    const Module*
        FindModuleOnPath(const Path& path){
            const Module* mod = 0;
            const ModInst* inst = dynamic_cast<const ModInst*>(
                    FindInstanceOnPath(path));
            if(inst){
                mod = inst->getMaster();
            }
            return mod;
        }


}
