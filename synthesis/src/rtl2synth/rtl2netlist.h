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

#ifndef _RTL2NETLIST_H_
#define _RTL2NETLIST_H_

#include <map>
#include <string>

#include "convisitorbase.h"
#include "namemgr.h"

namespace Synth
{
    class SyScope;
    class SyModule;
    class Expr2Gates;
    class SyPrimInst;
    class Expr2GatesRetType;
    class Rtl2Netlist : public ConVisitorBase
    {
        public:
            Rtl2Netlist() :  ConVisitorBase(), expr2Gates(0) {
            }

            virtual ~Rtl2Netlist() {
            }


            //Visitor functions start here
            virtual void visit(const SyBModule*);
            virtual void visit(const SyVPort*);
            virtual void visit(const SyScPort*);
            virtual void visit(const SyVNet*);
            virtual void visit(const SyScNet*);
            virtual void visit(const SyVReg*);
            virtual void visit(const SyScReg*);
            virtual void visit(const SyContAssign*);
            virtual void visit(const SyModInst*);
            virtual void visit(const SyPrimInst*);
            virtual void visit(const SyAlways*);
            virtual void visit(const SyFunc*);
            virtual void visit(const SyTask*);


            //end visitor functions
            virtual void setCurrMod(SyModule *mod); 
            virtual void setCurrSynthMod(SyModule *mod); 
            virtual void setCurrRtlMod(SyModule *mod);  
            virtual void setCurrScope(SyScope *sc);


        private:
            DISABLE_COPYCONST_ASSGNOP(Rtl2Netlist);
            void        setExpr2GateHandler();
            //void        create2InputGate(const SyPrimInst*);
            void        createPrimitiveGate(const SyPrimInst*);
            void        createNotBuf(const SyPrimInst*);
            void        createTristate(const SyPrimInst*, Expr2GatesRetType*);
            void        createPortsForTfPorts(const SyTaskOrFunc*);

            void        createDecoderLogicForAssign(const SyBitSel*, const SyExpr*);
            
            Expr2Gates    *expr2Gates;
    };
}
#endif
