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

#ifndef _CONVISTORBASE_H_
#define _CONVISTORBASE_H_
#include "syvisitorbase.h"

namespace Synth
{
    class SyAssign;
    class ConVisitorBase : public SyVisitorBase
    {
        friend class FuncTaskCall2Gates;
        public:
            virtual const std::string& getCurrModName() const;
            void        createBuf(SyCntv*, SyCntv*);
            void        createInv(SyCntv*, SyCntv*);
            SyScNet*    createNet();

            virtual void setCurrMod(SyModule *mod) = 0; 
            virtual void setCurrSynthMod(SyModule *mod) = 0; 
            virtual void setCurrRtlMod(SyModule *mod) = 0;  
            virtual void setCurrScope(SyScope *sc) = 0;

            void setCurrModInternal(SyModule *mod) {
                currMod = mod;
            }
            void setCurrSynthModInternal(SyModule *mod) {
                currSynthMod = mod;
            }
            void setCurrRtlModInternal(SyModule *mod) {
                currRtlMod = mod;
            }
            void setCurrScopeInternal(SyScope *sc) {
                currScope = sc;
            }
        protected:
            ConVisitorBase() : currMod(0), currRtlMod(0), 
                            currSynthMod(0),currScope(0){
            }
            ConVisitorBase(const SyModule*, const SyModule*, const SyScope*);
            virtual ~ConVisitorBase() {
            }

            SyModule*    getCurrMod() const {
                return currMod;
            }
            SyModule*    getCurrSynthMod() const {
                return currSynthMod;
            }
            SyModule*    getCurrRtlMod() const {
                return currRtlMod; 
            }    
            SyScope*    getCurrScope() const {
                return currScope;
            }    

            //0 --> does not have bit/partsel, 1 --> bit/part sel on left
            //2--> bit/part sel on right
            int hasBitPartSelect(const SyAssign *, bool bitSel) const;
            bool hasBitPartSelect(const SyExpr *, bool bitSel) const;
        private:
            
            SyModule    *currMod;
            SyModule    *currRtlMod;
            SyModule    *currSynthMod;
            SyScope        *currScope;
    };
}

#endif
