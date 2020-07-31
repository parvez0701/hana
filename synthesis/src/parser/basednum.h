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

#ifndef _BASEDNUM_H_
#define _BASEDNUM_H_
#include <vector>
#include "const.h"

namespace Rtl
{
    class OMVisitor;
    class BasedNum : public Const
    {
        public:
            BasedNum(const int siz, const char b, const bool s,
                const std::vector<char>& bts);
            virtual ~BasedNum();
            virtual int evaluate() const;
            virtual void accept(OMVisitorBase*) const;
            virtual std::string toString() const;
            virtual const BasedNum* getBit(const long) const;
            virtual long len() const;
            virtual BasedNum* copy() const {
                return new BasedNum(*this);
            }
            bool isUnknownOrHighImpedance() const; 
            virtual bool isSigned() const {
                return sign;
            }    
            //caller owns the pointer
            BasedNum *convert2Base2() const;
            const std::vector<char>& getBits() const {
                return bits;
            }    
            std::vector<char>getExtendedBits() const;
        private:
            bool checkBinary();
            bool checkOctal();
            bool checkDecimal();
            bool checkHex();
            void setSize(const int sz){
                size = sz;
            }
            void num2Bits(char, std::vector<char>&) const;
            bool isUnknownOrHighImpedance(const char) const;
            std::vector<char> convertDecimal2Binary(std::vector<char>) const;
            std::vector<char> convertOctal2Binary(std::vector<char>) const;
            std::vector<char> convertHex2Binary(std::vector<char>) const;

            bool isAllowedChar(char) const;
            void convertBits();
            void num2BitsDec(std::vector<char>&) const;

            
            int size;
            char base;
            bool sign;
            std::vector<char> bits;
    };
            
}
#endif
