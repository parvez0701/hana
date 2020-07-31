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

#ifndef _CONCAT_H_
#define _CONCAT_H_
#include <vector>

#include "expr.h"
namespace Rtl
{
    class OMVisitorBase;
    class Concat: public Expr
    {
        friend class Elaborator;
        public:
            Concat(const std::vector<Expr*>& ve):
                Expr(), exprs(ve){
            }

            ~Concat(){
            }
            const std::vector<Expr*>& getExprs() const{
                return exprs;
            }
            virtual void accept(OMVisitorBase*) const;
            virtual std::string toString() const;
            virtual long len() const;
            virtual const Expr* getBit(const long) const;
            virtual Concat* copy() const;
            virtual bool isConstExpr() const;
            virtual const Expr* repExpr(const std::string&, const Expr*);

            virtual int evaluate() const;
            virtual bool isSigned() const {
                return false;
            }    

        private:
            std::vector<Expr*> exprs;
    }; 
}
#endif
