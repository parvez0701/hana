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

#ifndef _PARAM_H_
#define _PARAM_H_
#include <string>
#include "scalarcntv.h"
namespace Rtl
{
    class OMVisitor;
    class OMVisitorBase;
    class Param: public ScalarCntv
    {
        friend class VecParam;
        friend class OMVisitor;
        public:
            Param(const IxName& n):ScalarCntv(n),
            val(0){
            }
            Param(const IxName& n, const Expr* v, const std::string& vt):
            ScalarCntv(n),
            val(const_cast<Expr*>(v)), varType(vt){
            }

            virtual ~Param(){
            }
            const Expr* getVal() const{
                return val;
            }
            const std::string& getVarType() const {
                return varType;
            }
            virtual void accept(OMVisitorBase*)const;
            virtual int evaluate() const;
            virtual long len() const {
                return 32;
            }
            virtual Param* copy() const {
                Param* cparam = new Param(this->getIxName(), val->copy(),
                        varType);
                cparam->setSign(this->isSigned());
                return cparam;
            }
            virtual bool isConstExpr() const {
                return true;
            }
            virtual bool isSigned() const {
                if(ScalarCntv::isSigned()) {
                    return true;
                }
                return val ? val->isSigned() : false;
            }    
            bool isSignSpecified() const {
                return ScalarCntv::isSigned();
            }    
            //virtual std::string toString() const;
        private:
            void setVarType(const std::string& vt){
                varType = vt;
            }
            Expr* val;
            std::string varType;
    };
}
#endif
