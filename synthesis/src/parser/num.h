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

#ifndef _NUM_H_
#define _NUM_H_
#include "const.h"
#include "basednum.h"
namespace Rtl
{
    class OMVisitorBase;
    class Num: public Const
    {
        public:
            Num(const long n): Const(), num(n){
            }
            virtual ~Num(){
            }
            virtual long evaluate() {
                return num;
            }
            virtual void accept(OMVisitorBase*) const;
            virtual std::string toString() const;
            virtual int evaluate() const;
            virtual const BasedNum* getBit(const long bit) const;
            virtual long len() const {
                return 32;
            }
            virtual Num* copy() const {
                return new Num(num);
            }    
        private:
            long num;
    };

}
#endif
