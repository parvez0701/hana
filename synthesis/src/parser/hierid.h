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

#ifndef _HIERID_H_
#define _HIERID_H_
#include <string>
#include "expr.h"
namespace Rtl
{
    class OMVisitorBase;
    class HierId: public Expr
    {
        friend class SemCheck;
        friend class SemanticChecker;
        public:
            HierId(const std::string& i): Expr(), id(i), actual(0){
            }
            virtual ~HierId(){
            }

            virtual int evaluate() const{
                if(actual){
                    return actual->evaluate();
                }
                return 0;
            }
            const Expr* getActualId() const {
                return actual;
            }
            Path getIdAsPath() const;
            virtual void accept(OMVisitorBase*) const;
            virtual std::string toString() const;
            virtual long len() const{
                if(actual){
                    return actual->len();
                }
                return 0;
            }
            Path getResolvedPath() const;
            const Expr* repExpr(const std::string&, const Expr*);
            virtual bool isSigned() const
            {
                if(actual) {
                    return actual->isSigned();
                }
                return false;
            }
        private:
            bool resolve(const Path&);
            std::string id;
            Expr* actual;
            Path idpath;
    };
}
#endif
