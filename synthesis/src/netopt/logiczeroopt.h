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

#ifndef _LOGICZEROOPT_H_
#define _LOGICZEROOPT_H_

#include "netoptbase.h"
namespace stview 
{
    class SBlock;
    class SNet;
    class SPN;
    class SInst;
}

namespace opt 
{
    class LogicZeroOpt : public NetOptBase
    {
        public:
            LogicZeroOpt();
            virtual ~LogicZeroOpt();
            virtual bool operator()(stview::SInst*);
            virtual bool optimize(stview::SBlock*);
            virtual void clear();
        private:
            LogicZeroOpt(const LogicZeroOpt&);
            LogicZeroOpt& operator=(const LogicZeroOpt&);

            bool condition(const stview::SInst*) const;
            bool compact(stview::SBlock*, stview::SInst*); 
            bool compactOr(stview::SBlock*, stview::SInst*, stview::SPN*); 
            void compactOr2(stview::SBlock*, stview::SInst*, stview::SPN*); 
            bool compactAnd(stview::SBlock*, stview::SInst*, stview::SPN*); 
            bool compactNand(stview::SBlock*, stview::SInst*); 
            bool compactXnor(stview::SBlock*, stview::SInst*, stview::SPN*); 
            bool compactXor(stview::SBlock*, stview::SInst*, stview::SPN*);
            bool compactMux(stview::SBlock*, stview::SInst*, stview::SPN*);
            bool compactMux2(stview::SBlock*, stview::SInst*, stview::SPN*);
            bool compactBuf(stview::SBlock*, stview::SInst*, stview::SPN*); 
            bool compactInv(stview::SBlock*, stview::SInst*);

            stview::SNet* getVccNet(stview::SBlock*);
            stview::SNet* vccnet;
    };        

}
#endif
