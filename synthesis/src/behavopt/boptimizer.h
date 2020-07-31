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

#ifndef _BOPTIMIZER_H_
#define _BOPTIMIZER_H_
#include "sylist.h"
namespace Synth
{
    class SyTaskOrFunc;
    class SyBModule;
    class SyStmt;
    class BOptimizer
    {
        public:
            static BOptimizer *instance();
            void optimize() const;
        private:
            BOptimizer();
            BOptimizer(const BOptimizer&);
            ~BOptimizer();

            static BOptimizer *boptimizer;

            void propagateConsts(SyBModule*) const;
            void propagateConsts(SyBModule*, SyTaskOrFunc*) const;
            void propagateConsts(SyBModule*, SyList<SyStmt*>&) const;

            void optimizeContAssigns(SyBModule*) const;
    };
}
#endif
