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

#ifndef _RTL2SYNTH_H_
#define _RTL2SYNTH_H_
#include <map>

#include "sylist.h"
#include "syutils.h"

namespace Synth
{
    class SyRoot;
    class SyModule;
    class SyModInst;
    class SyPrimInst;
    class SyContAssign;
    class SyAlways;
    class SyTask;
    class SyFunc;
    class SyTaskCall;
    class SyFuncCall;
    class SyScCntv;
    class SyStmt;
    class SyIf;
    class SyEventStmt;
    class SyCase;
    class SyAssign;

    class Rtl2Synth
    {
        public:
            static Rtl2Synth* instance();
            virtual ~Rtl2Synth();

            void translate();
            const SyList<const SyScCntv*>* getLatchVars(const std::string&) const;
        private:
            Rtl2Synth();
            Rtl2Synth(const Rtl2Synth&);
            Rtl2Synth& operator=(const Rtl2Synth&);

            static Rtl2Synth *rtl2synth;

            void addLatchVars(const std::string&, SyList<const SyScCntv*>*);

            SyModule* translate(const SyModule*) const;
            void translate(SyModule*, const SyModInst*) const;
            void translate(SyModule*, const SyPrimInst*) const;
            void translate(SyModule*, const SyContAssign*) const;
            void translate(SyModule*, const SyAlways*) const;
            void translate(SyModule*, const SyTask*) const;
            void translate(SyModule*, const SyFunc*) const;
            void translate(SyModule*, const SyTaskCall*) const;
            void translate(SyModule*, const SyFuncCall*) const;

            SyList<const SyScCntv*>* detectLatch(const SyAlways*) const;
            void detectLatch(
                    const SyStmt*, 
                    std::map<const SyScCntv*, bool, cmp_cntvs>&) const;
            void detectLatchAssign(
                    const SyAssign*, 
                    std::map<const SyScCntv*, bool, cmp_cntvs>&) const;
            void detectLatchIf(
                    const SyIf*, 
                    std::map<const SyScCntv*, bool, cmp_cntvs>&) const;
            void detectLatchCase(
                    const SyCase*, 
                    std::map<const SyScCntv*, bool, cmp_cntvs>&) const;
            void detectLatchEvent(
                    const SyEventStmt*, 
                    std::map<const SyScCntv*, bool, cmp_cntvs>&) const;

            void detectLatchStmts(
                    const SyList<SyStmt*>&,
                    std::map<const SyScCntv*, bool, cmp_cntvs>&) const;
            
            mutable std::map<std::string, SyList<const SyScCntv*>*, cmp_str> moduleLatches;

    };
}
#endif
