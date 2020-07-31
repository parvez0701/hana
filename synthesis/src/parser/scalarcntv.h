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

#ifndef _SCALARCNTV_H_
#define _SCALARCNTV_H_
#include "cntv.h"
#include "ixname.h"
namespace Rtl
{
    class VecCntv;
    class ScalarCntv: public Cntv
    {
        friend class OMCreator;
        friend class VecLocalParam;
        friend class SemCheck;
        friend class VecReg;
        friend class VecInt;
        friend class VecReal;
        friend class VecTime;
        friend class VecRealTime;
        friend class Elaborator;
        friend class GenUnroll;
        public:
            ScalarCntv(const IxName& i): ixName(i), isSig(false){
            }
            virtual ~ScalarCntv(){
            }
            const IxName& getIxName() const{
                return ixName;
            }

            virtual const VecCntv* getVecCntv() const;
            const bool isVecBit() const{
                return (ixName.getIndex() != IxName::nix);
            }

            virtual bool isSigned() const{
                return isSig;
            }
            virtual std::string toString() const;
            const Expr* repExpr(const std::string&, const Expr*);
        protected:
            void setSign(const bool is=true){
                isSig = is;
            }
            void setIxName(const IxName& name){
                ixName = name;
            }
        
        private:
            IxName ixName;
            bool isSig;
    };
}
#endif
