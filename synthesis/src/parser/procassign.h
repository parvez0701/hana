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

#ifndef _PROCASSIGN_H_
#define _PROCASSIGN_H_

#include "assign.h"
#include "omvisitor.h"
namespace Rtl
{
    class ProcAssign: public Assign
    {
        friend class SemanticChecker;
        public:
            ProcAssign(const Delay* ld, const Delay* rd,
                const Expr* le, const Expr* re, const AssignType t):
                Assign(le, re), 
                ldelay(const_cast<Delay*>(ld)), 
                rdelay(const_cast<Delay*>(rd)), 
                type(t){
            }

            virtual ~ProcAssign(){
            }
            const Delay* getLDelay() const {
                return ldelay;
            }
            const Delay* getRDelay() const {
                return rdelay;
            }
            const AssignType getAssignType() const {
                return type;
            }
            virtual void accept(OMVisitorBase* v) const {
                v->visit(this);
            }

            virtual ProcAssign* copy() const {
                return new ProcAssign(    ldelay ? ldelay->copy() : 0,
                                        rdelay ? rdelay->copy() :0,
                                        this->getLhs()->copy(),
                                        this->getRhs()->copy(), 
                                        type);
            }                            
        private:
            Delay* ldelay;
            Delay* rdelay;
            AssignType type;
    };
}
#endif
