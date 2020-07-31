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

#ifndef _VECPARAM_H_
#define _VECPARAM_H_

#include "veccntv.h"
#include "param.h"
namespace Rtl
{
    class VecParam: public VecCntv
    {
        friend class OMCreator;
        friend class Elaborator;
        public:
            VecParam(const std::string&,
                const Dimension*, const bool);

            virtual ~VecParam(){
            }

            //virtual Param* getCntvBit(const long) const throw(Error);
            std::vector<Param*> getParams() const;
            //const std::vector<Param*>& getParams() const{
            //    return params;
            //}
//            const Expr* getVal() const{
//                return params[0]->getVal();
//            }
            bool isSigned() const;
            //bool isSigned() const{
            //    return params[0]->isSigned();
            //}
            virtual long len() const; 
            //virtual long len() const {
            //    return params.size();
            //}
            virtual const Param* getBit(const long) const;
            virtual void accept(OMVisitorBase*) const;
            virtual VecParam* copy() const {
                VecParam* cvecparam = new VecParam(this->getName(), 
                        this->getDim()->copy(), this->isSigned());
                cvecparam->setVal(this->getVal()->copy());
                return cvecparam;
            }
            virtual bool isConstExpr() const {
                return true;
            }
            virtual int evaluate() const;
            bool isSignSpecified() const; 
        protected:
            virtual void setVal(const Expr *);
        private:
            void createParamBits(const std::string&,
                const Dimension*, const bool);
    //        std::map<long, Param*> paramMap;
            //std::vector<Param*> params;
    };
}
#endif
