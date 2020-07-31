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

#ifndef _MULTICONCAT_H_
#define _MULTICONCAT_H_
#include <vector>

#include "concat.h"
namespace Rtl
{
    class OMVisitorBase;
    class MultiConcat: public Concat
    {
        friend class Elaborator;
        friend class SemanticChecker;
        friend Expr* ReplaceSubExpression(Expr*, 
            const std::string&, const Expr*);
        public:
            MultiConcat(const Expr* e, const std::vector<Expr*>& ev):
                Concat(ev), count(const_cast<Expr*>(e))
                 {
            }
            virtual ~MultiConcat(){
            }
            const Expr* getCount() const{
                return count;
            }

            virtual void accept(OMVisitorBase*) const;
            virtual std::string toString() const;
            virtual long len() const;
            virtual const Expr* getBit(const long) const;
            virtual bool isConstExpr() const {
                if(!Concat::isConstExpr()){
                    return false;
                }
                return count->isConstExpr();
            }

            virtual MultiConcat* copy() const;
            virtual int evaluate() const;
            virtual bool isSigned() const {
                return false;
            }    

        private:
            void setCount(const Expr *expr) {
                count = const_cast<Expr*>(expr);
            }    
            Expr* count;
    };
}
#endif
