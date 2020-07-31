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

#include "syroot.h"
#include "sycontainer.h"

using std::map;
using std::string;
namespace Synth
{
    SyRoot* SyRoot::root[STAGE_2] = {0, 0};
    SyRoot::SyStage SyRoot::stage = STAGE_1;

    SyRoot::SyRoot(){
    }

    SyRoot::~SyRoot(){
    }

    SyRoot*
        SyRoot::instance() {
            if(!root[stage]){
                root[stage] = new SyRoot();
            }
            return root[stage];
        }

	void
	    SyRoot::clearRoot() 
		{
		    delete root[stage];
			root[stage] = 0;
			switchStage();
		}	
    void
        SyRoot::removeSynthMod(const SyModule* mod) {
            smodules.erase(mod->getName());
        }    

    void
        SyRoot::addSynthMod(const SyModule *mod)
        {
            smodules.insert(std::make_pair(mod->getName(),
                    const_cast<SyModule*>(mod)));
        }
    void
        SyRoot::addCellMod(const SyModule *mod)
        {
            cellmodules.insert(std::make_pair(mod->getName(),
                    const_cast<SyModule*>(mod)));
        }

    SyModule*
        SyRoot::getModule(
                const string& name,
                const bool searchCell) const {
            map<std::string, SyModule*, cmp_str>::const_iterator 
                iter = smodules.find(name);
             if(iter == smodules.end()){
                if(searchCell){
                    return this->getCellModule(name);
                }
                return 0;
            }
            return iter->second;
        }

    SyModule*
        SyRoot::getCellModule(const string& name) const
        {
            map<std::string, SyModule*, cmp_str>::const_iterator 
                iter = cellmodules.find(name);
            if(iter == cellmodules.end()){
                return 0;
            }
            return iter->second;
        }
    bool
        SyRoot::isCellModule(const string& name) const {
            return getCellModule(name) != 0;
        }    
            

    void
        SyRoot::addDontTouchModule(const string& name) {
            dontTouchModules.insert(name);
        }
    bool
        SyRoot::isDontTouchModule(const string& name) const {
            return dontTouchModules.find(name) != dontTouchModules.end();
        }

    bool
        SyRoot::isDontTouchModule(const SyModule *mod) const {
            return this->isDontTouchModule(mod->getName());
        }    
}
