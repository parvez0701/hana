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

#ifndef _STMT_H_
#define _STMT_H_
#include "base.h"
namespace Rtl
{
    class Stmt: virtual public Base
    {
        public:
   //         virtual Stmt* copy() const {
   //             return const_cast<Stmt*>(this);
   //         }
           virtual Stmt* copy() const = 0;
           virtual void repExpr(const std::string&, const Expr*) = 0;
    };
}
#endif
