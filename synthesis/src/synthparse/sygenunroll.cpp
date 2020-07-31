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

#include <sstream>

#include "sygenunroll.h"
#include "sycontainer.h"
#include "syconst.h"

using std::string;
using std::map;
using std::ostringstream;
using std::make_pair;
using namespace Rtl;
namespace Synth
{
    BlockNameMgr::BlockNameMgr(const SyModule *mod):
        synthMod(const_cast<SyModule*>(mod))
    {    
    }

    BlockNameMgr::~BlockNameMgr() {
    }

    string
    BlockNameMgr::getNewName(const string& oldName) const
    {
        if(nameTableStack.empty()) {
            return "";
        }
        OldNewNameMap::const_iterator iter = nameTableStack.top().find(oldName);
        if(iter == nameTableStack.top().end()) {
            return "";
        }
        return iter->second;
    }

    string
    BlockNameMgr::createNewName(const string& hint) const
    {
        int i = 0;
        string name = hint;

        while(1)
        {
            if(!synthMod->findObject(name)) {
                return name;
            }
            ostringstream str;
            str << hint << "_" << i++;
            name = str.str();
        }
        assert(0);
    }

    void
    BlockNameMgr::addName(const string& oldName, const string& newName) 
    {
        if(nameTableStack.empty()) {
            return;
        }
        nameTableStack.top().insert(make_pair(oldName, newName));
    }    

    const SyBase*
    BlockNameMgr::findObject(const string& name) const
    {
        const SyBase *obj = synthMod->findObject(name);
        if(obj) {
            return obj;
        }
        obj = synthMod->findObject(this->getNewName(name));
        return obj;
    }

    void
    BlockNameMgr::diveInsideNewScope() {
        nameTableStack.push(OldNewNameMap());
    }

    void
    BlockNameMgr::comeOutOfScope() {
        if(nameTableStack.empty()) {
            return;
        }
        nameTableStack.pop();
    }    


//-----------------------------------------------------------------------
//    Class which elaborates generate block
//-----------------------------------------------------------------------

    SyGenUnroll::SyGenUnroll(    const SyModule *smod, 
                                const Module *rmod) :
                                blockNameMgr(new BlockNameMgr(smod)),
                                synthMod(const_cast<SyModule*>(smod)),
                                rtlMod(const_cast<Module*>(rmod)),
                                status(true){
    }

    SyGenUnroll::~SyGenUnroll() {
    }

    bool
    SyGenUnroll::operator()() {
        return this->unroll();
    }

    void    
    SyGenUnroll::addGenvar(const Genvar *genvar)
    {
        string newName = blockNameMgr->createNewName(genvar->getName());
        if(newName != genvar->getName()) {
            genvar = new Genvar(newName, genvar->getVal());
        }    
        genvars.insert(make_pair(newName, const_cast<Genvar*>(genvar)));
    }    

    Genvar*
    SyGenUnroll::findGenvar(const string& name) const
    {
        Genvars::const_iterator iter = genvars.find(name);
        if(iter == genvars.end()) {
            return NULL;
        }
        return iter->second;
    }    

    SyConst*
    SyGenUnroll::getGenvarValue(const Genvar* genvar) const {
        return new SyConst(genvar->getVal(), false, false);
    }    
}
