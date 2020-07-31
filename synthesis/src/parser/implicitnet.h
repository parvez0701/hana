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

#ifndef _IMPLICITNET_H_
#define _IMPLICITNET_H_
#include "omvisitorbase.h"
#include "net.h"
namespace Rtl
{
    //To handle implicit net declaration

    class ImplicitNet : public Expr
    {
        public:
            ImplicitNet(const std::string& n,
                const NetType t): Expr(){
                net = new Net(n);
                net->setNetType(t);
            }
            virtual ~ImplicitNet() {
            }
            virtual std::string toString() const {
                return net->toString();
            }
            virtual long len() const {
                return net->len();
            }

            operator const Net* () const {
                return net;
            }    
            Net* getNet() const {
                return net;
            }    
            virtual void accept(OMVisitorBase *v) const {
                v->visit(this);
            }    

            virtual const Expr* repExpr(const std::string& str, 
                                        const Expr* subExpr){
                return (str == net->toString()) ? subExpr : this;
            }    

            virtual bool isSigned() const {
                return false;
            }    
        private:    
            Net* net;
    };

}
#endif
