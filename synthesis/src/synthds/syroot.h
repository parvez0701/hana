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

#ifndef _SYROOT_H_
#define _SYROOT_H_

#include <map>
#include "syutils.h"
#include "sylist.h"
namespace Synth
{
    class SyModule;
    class SyRoot
    {
        public:
            typedef enum {UNDEF=0, STAGE_1, STAGE_2 } SyStage;
            static SyRoot* instance();
			static void clearRoot();
            const std::map<std::string, SyModule*, cmp_str> &
                getSynthMods() const{
                    return smodules;
                }

            void removeSynthMod(const SyModule*);
            void addSynthMod(const SyModule *mod);
            void addCellMod(const SyModule *mod);
            void addTopMod(const SyModule* mod){
                topmodules.push_back(const_cast<SyModule*>(mod));
            }
            SyModule* getModule(
                    const std::string& name,
                    const bool searchCell=true) const;
            SyModule* getCellModule(const std::string& name) const;
            static void setStage(SyStage s) {
                stage = s;
            }

            static SyStage getStage() {
                return stage;
            }    

            static void switchStage() {
                stage = (stage == STAGE_1) ? STAGE_2 : STAGE_1;
            }    

            const SyList<SyModule*>& getTopModules() const{
                return topmodules;
            }    
            void addDontTouchModule(const std::string&);
            bool isDontTouchModule(const SyModule*) const;
            bool isDontTouchModule(const std::string&) const;
            bool isCellModule(const std::string& ) const;
        private:
            SyRoot();
            ~SyRoot();
            SyRoot(const SyRoot&);
            SyRoot& operator=(const SyRoot&);
            //data
            static SyRoot* root[STAGE_2];
            static SyStage stage;
            SyList<SyModule*> topmodules;
            std::set<std::string, cmp_str> dontTouchModules;
            std::map<std::string, SyModule*, cmp_str> smodules;
            std::map<std::string, SyModule*, cmp_str> cellmodules;
    };

}
#endif
