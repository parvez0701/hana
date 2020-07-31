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

#ifndef _NET_H_
#define _NET_H_
#include "scalarcntv.h"
#include "defines.h"

namespace Rtl
{
    class OMVisitorBase;
    class Expr;
    class Net: public ScalarCntv
    {
        friend class OMCreator;
        friend class Elaborator;
        public:
            Net(const IxName& ix): ScalarCntv(ix), 
            netType(WIRE), initExpr(0){
            }

            virtual ~Net(){
            }
            void setNetType(const NetType t){
                netType = t;
            }
            const NetType getNetType() const{
                return netType;
            }
            virtual void accept(OMVisitorBase*) const;
            const Expr* getInitExpr() const{
                return initExpr;
            }
            virtual long len() const{
                return 1;
            }
            virtual Net* copy() const;
            //virtual const VecNet* getVecCntv() const {
            //    return static_cast<const VecNet*>(
            //    ScalarCntv::getVecCntv());
            //}
        private:
            void setInitExpr(const Expr* expr){
                initExpr = const_cast<Expr*>(expr);
            }
            NetType netType; 
            Expr* initExpr;
    };
}
#endif
