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

#ifndef _VTIME_H_
#define _VTIME_H_
#include <assert.h>

#include "omvisitor.h"
#include "var.h"
namespace Rtl
{
    class Time : public Var
    {
        public:
            Time(const IxName& n): Var (n){
                for(int i = 0; i < 32; ++i){
                    timeBits.push_back(new Reg(IxName(n.getName(), i)));
                }
            }
            Time(const IxName&n, const Expr* e):  Var(n, e){
                for(int i = 0; i < 32; ++i){
                    timeBits.push_back(new Reg(IxName(n.getName(), i)));
                }
            }
            virtual ~Time(){
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual int evaluate() const { return -1;}
            virtual long len() const {
                return 32;
            }
            const Reg* getTimeBit(const int bit)const {
                if(bit < 0 || bit > 31){
                    return 0;
                }
                return timeBits[bit];
            }
            virtual Time* copy() const 
            {
                assert(!this->isVecBit());
                return new Time(this->getIxName());
            }    


        private:
            std::vector<Reg*> timeBits;
    };
}
#endif
