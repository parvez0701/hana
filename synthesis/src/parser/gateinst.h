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

#ifndef _GATEINST_H_
#define _GATEINST_H_
#include "inst.h"
#include "delay.h"
#include "defines.h"
namespace Rtl
{
    class OMVisitorBase;
    class GateInst: public Inst
    {
        friend class OMCreator;
        friend class Elaborator;
        public:
            GateInst(const std::string& n, const GateType t):
                Inst(n), type(t){
            }
            GateInst(const std::string& n):
                Inst(n), type(UNSETGATE){
            }
            virtual ~GateInst(){
            }
            const GateType getType() const{
                return type;
            }

            const Delay* getDelay() const{
                return delay;
            }

            const DriveSt* getDrStrength() const{
                return strength;
            }
            virtual void accept(OMVisitorBase*) const;

            virtual GateInst* copy() const;
            virtual GateInst* repExpr(    const std::string& str,
                                        const Expr *expr);
                
        private:
            void setGateType(const GateType t){
                type = t;
            }
            void setDelay(const Delay* d){
                delay = const_cast<Delay*>(d);
            }
            void setDrStr(const DriveSt* dst){
                strength = const_cast<DriveSt*>(dst);
            }
            GateType type;
            Delay* delay;
            DriveSt* strength;
    };
}
#endif
