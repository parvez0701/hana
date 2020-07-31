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

#ifndef _VECPIN_H_
#define _VECPIN_H_
#include "pin.h"
#include "veccntv.h"
namespace Rtl
{
    class VecPin: public VecCntv
    {
        public:
            VecPin(const std::string&,
                const Dimension*, const Dir, const Expr*);

            virtual ~VecPin(){
            }

            virtual const Pin* getBit(const long) const;
            const std::vector<Pin*>& getPins() const{
                return pins;
            }
            virtual void accept(OMVisitorBase* visitor) const;
            virtual long len() const {
                return pins.size();
            }
            const Expr* getActual() const{
                return actual;
            }
            void setActual(const Expr* e){
                actual = const_cast<Expr*>(e);
            }

            Dir getDir() const {
                return pins[0]->getDir();
            }
            virtual VecPin* copy() const; 
            virtual bool isSigned() const {
                return false;
            }    

        private:
            void createPinBits(const std::string&,
                const Dimension*, const Dir, const Expr*);
    //        std::map<long, Pin*> pinMap;
            std::vector<Pin*> pins;
            Expr* actual;
    };
}
#endif
