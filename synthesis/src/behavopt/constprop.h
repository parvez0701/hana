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

#ifndef _CONSTPROP_H_
#define _CONSTPROP_H_

/******************************************************************
  * Constant popagation is done on list of statements.
  * This optimization is done locally, taking statements list of
  * a function, task, always block or list of continuous assigns.
  *
  *****************************************************************/
#include "sylist.h"
namespace Synth
{
    class SyStmt;
    class SyBModule;
    class ConstProp
    {
        public: 
            friend class BOptimizer;
        private:
            SyList<SyStmt*>* optimize(
                    SyBModule*,
                    const SyList<SyStmt*>*)const;
            bool checkAllBlocking(const SyList<SyStmt*>*) const;
            bool checkAllBlocking(const SyStmt*) const;
            SyList<SyStmt*>* optimizeStatements1(
                    SyBModule*,
                    const SyList<SyStmt*>*) const;
            SyList<SyStmt*>* optimizeStatements2(
                    SyBModule*,
                    const SyList<SyStmt*>*) const;

    };
}
#endif
