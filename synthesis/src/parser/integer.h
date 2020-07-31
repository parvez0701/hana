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

#ifndef _INTEGER_H_
#define _INTEGER_H_

#include "var.h"
#include "omvisitor.h"
#include "reg.h"
namespace Rtl
{
    class Integer : public Var
    {
        public:
            Integer(const IxName& n): Var (n){
                for(int i = 0; i < 32; ++i){
                    intBits.push_back(new Reg(IxName(n.getName(), i)));
                }
            }
            Integer(const IxName&n, const Expr* e):  Var(n, e){
                for(int i = 0; i < 32; ++i){
                    intBits.push_back(new Reg(IxName(n.getName(), i)));
                }
            }
            virtual ~Integer(){
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual int evaluate() const {
                if(const Expr* val = this->getVal()) {
                    return val->evaluate();
                }
                return INT_MIN;
            }    
            virtual long len() const{
                return 32;
            }
            const Reg* getIntBit(const int bit) const {
                if(bit < 0 || bit > 31){
                    return 0;
                }
                return intBits[bit];
            }

            virtual Integer* copy() const {
                return new Integer(this->getIxName());
            }    

            virtual const Expr* repExpr(const std::string& str, const Expr* expr)
            {
                if(this->getIxName().toString() == str) {
                    return expr;
                }
                if(const Expr *val = this->getVal())
                {
                    val = const_cast<Expr*>(val)->repExpr(str, expr);
                    this->setVal(val);
                }
                return this;
            }    

            virtual bool isConstExpr() const {
                return this->getVal() && (this->getVal()->evaluate() != INT_MIN);
            }    
        private:
            //Internally it contains 32  reg vars
            std::vector<Reg*> intBits;
    };
}
#endif
