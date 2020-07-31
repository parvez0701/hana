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

#ifndef _SYSPARSE2SYNTH_H_
#define _SYSPARSE2SYNTH_H_

#include "parseinclude.h"
#include "syutils.h"

using namespace Rtl;
namespace Synth
{
    class SyRoot;
    class SyParse2Synth
    {
        public:
            static SyParse2Synth *instance();
            void parse2Synth(const Root*);
            ~SyParse2Synth();
        private:
            //creation/deletion stuff
            SyParse2Synth();
            SyParse2Synth(const SyParse2Synth&);
            SyParse2Synth& operator=(const SyParse2Synth);
            //functions
            void synthesizabilityCheck(const Root*);
            void synthesizabilityCheck(const Module*);
            bool isSynthesizable(const Module*) const;
            bool checkVarDecls(const Scope*) const;
            bool checkPorts(const Scope*) const;
            template<class T>
            bool checkMultiDimArray(const Scope*,
                                    const std::vector<T*>&, int) const;
            void checkGates(const Module*) const;
           // void checkUDP(const Module*) const;
            bool checkCase(const Case*) const;
            bool checkRepeat(const Repeat*) const;
            bool checkFor(const For*) const;
            bool checkStmt(const Stmt*) const;
            bool checkTaskOrFunc(const TaskOrFunc*) const;
            bool checkForLoopConst(const Expr*, const Expr*) const;
            bool checkAlways(const Module*, const Always *) const;
            bool isSequential(const Always*) const;
            bool checkSensList(const Module*, const Always*) const;
            bool checkForSetReset(const Module*, const Always*) const;
            bool checkAlwaysStmts(const Module*, const Always*) const;
            void checkAlwaysStmt(
                    const Stmt*,
                    std::map<std::string, std::pair<bool, bool>, cmp_str >&) const;

            //data
            static SyParse2Synth* synth2parse;
            std::set<std::string, cmp_str> unsynthMods;
            std::set<std::string, cmp_str> synthMods;
    };
}
#endif
