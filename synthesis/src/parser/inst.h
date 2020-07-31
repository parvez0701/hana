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

#ifndef _INST_H_
#define _INST_H_
#include <vector>

#include "base.h"
#include "ixname.h"
#include "pin.h"

namespace Rtl
{
    class Inst: public Base
    {
        friend class OMCreator;
        friend class SemCheck;
        friend class Elaborator;
        friend class GenUnroll;
        public:
            Inst(const IxName& n) : Base(), name(n){
            }
            const std::vector<Pin*>& getPins() const{
                return pins;
            }
            const IxName& getName() const{
                return name;
            }
            virtual Inst* repExpr(const std::string&, const Expr*) = 0;
        protected:
            void setPins(const std::vector<Pin*>& p){
                pins = const_cast<std::vector<Pin*>& >(p);
            }

            void repPinActuals(const std::string& str, const Expr *expr)
            {
                for(unsigned int i = 0; i < pins.size(); ++i) {
                    if(const Expr *actual = pins[i]->getActual()) {
                        pins[i]->setActual(const_cast<Expr*>(actual)->
                        repExpr(str, expr));
                    }
                }    
            }
            void setName(const IxName& n) {
                name = n;
            }    
        private:
            std::vector<Pin*> pins;
            IxName name;
    };
}
#endif
