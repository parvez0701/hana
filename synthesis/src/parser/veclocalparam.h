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

#ifndef _VECLOCALPARAM_H_
#define _VECLOCALPARAM_H_

#include "veccntv.h"
#include "localparam.h"
#include "intconst.h"
namespace Rtl
{
    class VecLocalParam: public VecCntv
    {
        friend class OMCreator;
        public:
            VecLocalParam(const std::string&,
                const Dimension*, const bool);

            virtual ~VecLocalParam(){
            }

            //virtual LocalParam* getCntvBit(const long) const throw(Error);
            const std::vector<LocalParam*>& getLocalParams() const{
                return lparams;
            }
            const std::vector<LocalParam*>& getParams() const{
                return getLocalParams(); //alias
            }
//            const Expr* getVal() const{
//                return lparams[0]->getVal();
//            }
            bool isSigned() const{
                return lparams[0]->isSigned();
            }
            virtual long len() const {
                return lparams.size();
            }
            virtual const LocalParam* getBit(const long) const;
            virtual void accept(OMVisitorBase*) const;
            virtual VecLocalParam* copy() const;
        private:
            void createLocalParamBits(const std::string&,
                const Dimension*, const bool);
    //        std::map<long, LocalParam*> paramMap;
            std::vector<LocalParam*> lparams;
    };
}
#endif
