/* 
Copyright (C) 2009-2011 Parvez Ahmad
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

#ifndef _SOPUTILS_H_
#define _SOPUTILS_H_
#if 0
namespace stview 
{
    class SExpr;
    class SAnd;
    class SInv;
    class SOr;
    class SOne;
    class SZero;
}

namespace SOP
{
    stview::SExpr* ProductOr(stview::SOr*, stview::SExpr*);
    stview::SExpr* Product(stview::SExpr*, stview::SExpr*);
    stview::SExpr* Complement(stview::SExpr*);
    stview::SExpr* Sum(stview::SExpr*, stview::SExpr*);

    stview::SOne* Tautology();
    stview::SZero* Absurdity();

	bool operator==(const stview::SExpr&, const stview::SExpr&);
	bool operator!=(const stview::SExpr&, const stview::SExpr&);

}
#endif
#endif
