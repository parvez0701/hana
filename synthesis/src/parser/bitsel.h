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

#ifndef _BITSEL_H_
#define _BITSEL_H_
#include <vector>

#include "expr.h"
namespace Rtl
{
    class OMVisitorBase;
    class BitSel: public Expr
    {
        friend class Elaborator;
        public: 
            BitSel(const Expr* e, const std::vector<Expr*>& d): Expr(),
                var(const_cast<Expr*>(e)),
                dim(const_cast<std::vector<Expr*>&>(d)){
            }
            virtual ~BitSel(){
            }
            virtual const std::vector<Expr*>& getDim() const{
                return dim;
            }
            const Expr* getVar() const {
                return var;
            }
            virtual std::string toString() const;
            virtual void accept(OMVisitorBase*) const;
            virtual long len() const {
                return 1;
            }
            virtual BitSel* copy() const;

            virtual bool isConstExpr() const;
            virtual const Expr *getActual() const;
            bool hasConstDim() const;

            virtual const Expr* repExpr(const std::string&, const Expr*);
            virtual bool isSigned() const {
                return false;
            }    
            
        private:
            void setVar(const Expr* e){
                var = const_cast<Expr*>(e);
            }
            Expr* var;
            std::vector<Expr*> dim;
    };
}
#endif
