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

#ifndef _PIN_H_
#define _PIN_H_
#include "scalarcntv.h"
#include "defines.h"
#include "path.h"

namespace Rtl
{
    class Inst;
    class Pin: public ScalarCntv
    {
        public:
            Pin(const IxName& ix): ScalarCntv(ix), actual(0){
            }
            Pin(const IxName& ix, const Dir d, const Expr* a):
                ScalarCntv(ix), dir(d), actual(const_cast<Expr*>(a)){
            }

            virtual ~Pin(){
            }
            const Dir getDir() const{
                return dir;
            }
            virtual void setDir(const Dir d){
                dir = d;
            }
            const Expr* getActual() const{
                return actual;
            }
            void setActual(const Expr* a){
                actual = const_cast<Expr*>(a);
            }
            virtual long len() const{
                return 1;
            }
            virtual void accept(OMVisitorBase*) const;
            virtual Pin* copy() const;
            //virtual std::string toString() const;

        //    virtual const VecPin* getVecCntv() const {
         //       return static_cast<const VecPin*>(
          //      ScalarCntv::getVecCntv());
          //  }
            //const Inst* getInst(const std::string&) const;
        private:
            Dir dir;
            Expr* actual;
    };
}
#endif
