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

#ifndef _UDPINST_H_
#define _UDPINST_H_

#include "root.h"
#include "inst.h"
#include "udp.h"
namespace Rtl
{
    extern Root* GRoot;
    class UDPInst: public Inst
    {
        public:
            UDPInst(const IxName& iname, const std::string& uname)
                : Inst(iname), udpname(uname){
            }

            virtual ~UDPInst(){
            }
            const std::string& getUDPName() const {
                return udpname;
            }
            const UDP* getUDP() const {
                return dynamic_cast<const UDP*>(GRoot->getObject(udpname));
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual UDPInst* copy() const
            {
                UDPInst *newInst = new UDPInst(this->getName(), udpname);
                vector<Pin*> newPins;
                CopyVector<Pin*>(this->getPins(), newPins);
                newInst->setPins(newPins);
                return newInst;
            }
            virtual UDPInst* repExpr(const std::string& str, const Expr* expr)
            {
                Inst::repPinActuals(str, expr);
                return this;
            }
        private:
            std::string udpname;
    };

}
#endif
