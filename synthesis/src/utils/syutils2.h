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

#ifndef _SYUTILS2_H_
#define _SYUTILS2_H_

#include "sycntvs.h"
#include "sycontainer.h"
#include "syroot.h"

namespace Synth
{
    template <class T1, class T2> const T2* GetVecCntv(
        const std::string& cntvname,
        const std::string& modname,
        const std::string& taskorfuncname)
    {
        const SyModule *mod = dynamic_cast<const SyModule*>(
                SyRoot::instance()->getModule(modname));
        if(!mod){
            return 0;
        }
        const T2 *retcntv = 0;
        if(!taskorfuncname.empty())
        {
            //If task/function name supplied, then try first to look
            // into task function scope. Note that all such variables
            //can only be port of task/function. All other declarations
            //are moved out of the function/task and exist at module scope,
            //with consistently changed name.
            const SyTaskOrFunc *taskorfunc = 
                dynamic_cast<const SyTaskOrFunc*>(mod->findObject(
                            taskorfuncname));
            if(taskorfunc){
                retcntv = dynamic_cast<const T2*>(taskorfunc->findObject(
                            cntvname));
            }
        }
        if(!retcntv){
            retcntv = dynamic_cast<const T2*>(mod->findObject(cntvname));
        }
        return retcntv;
    }

}
#endif
