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

#ifndef _CONTASSIGN_H_
#define _CONTASSIGN_H_

#include "assign.h"
#include "delay.h"
namespace Rtl
{
    class ContAssign: public Assign
    {
        public:
            ContAssign(const Delay* d, const DriveSt* dstr, const Expr* l,
                const Expr* r): Assign(l, r), delay(const_cast<Delay*>
                (d)), dstrength(const_cast<DriveSt*>(dstr)) {
            }

            virtual ~ContAssign(){
            }

            const Delay* getDelay() const{
                return delay;
            }
            const DriveSt* getStrength() const{
                return dstrength;
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual ContAssign* copy() const{
                Delay* cdelay = delay ? delay->copy() : 0;
                DriveSt* cdst = dstrength ? new DriveSt(dstrength->first, 
                        dstrength->second) : 0;
                std::pair<Expr*, Expr*> lrexpr = this->getExprCopy();
                return new ContAssign(cdelay, cdst, 
                        lrexpr.first, lrexpr.second);
            }

            virtual void repExpr(const std::string& str, const Expr* expr)
            {
                Assign::repExpr(str, expr);
                //return this;
            }
        private:
            Delay* delay;
            DriveSt* dstrength;
    };
}
#endif

