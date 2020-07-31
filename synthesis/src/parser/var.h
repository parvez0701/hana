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

#ifndef _VAR_H_
#define _VAR_H_
#include <string>
#include "scalarcntv.h"
namespace Rtl
{
    class OMVisitor;
    class Var: public ScalarCntv
    {
        friend class VecVar;
        friend class Elaborator;
        friend class GenUnroll;
        public:
            Var(const IxName& n):ScalarCntv(n),
            val(0){
            }
            Var(const IxName& n, const Expr* v):ScalarCntv(n),
            val(const_cast<Expr*>(v)){
            }

            virtual ~Var(){
            }
            const Expr* getVal() const{
                return val;
            }
            //virtual void accept(OMVisitor*)const;
            virtual int evaluate() const;
            virtual long len() const {
                return 1;
            }
            virtual Var* copy() const = 0;
            //virtual std::string toString() const;
        protected:    
            void setVal(const Expr* v){
                val = const_cast<Expr*>(v);
            }
        private:
            Expr* val;
    };
}
#endif
