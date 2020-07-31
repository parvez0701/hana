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

#ifndef _SYCONST_H_
#define _SYCONST_H_

#include "sycntvs.h"
namespace Synth
{
    /*
       * First character (msb) signfifies the sign bit if the 
       * variable is signed.
       * 1 signifies negative number.
       * 0 signifies positive number.
       * If number is negative it is assumed to be in 2's complement
       * form
    */

    /*
        IMPORTANT: This whole implementation is to be changed. May be
        val type will be changed to vector<int> for faster implementation
    */

    class SyVisitorBase;
    class TVisitor;

    class SyConst: public SyCntv
    {
        public:
            SyConst(signed long, const bool, const bool);
            //For signed numbers, the string is passed with proper
            //sign. If it is negative then it is in 2's complement
            //form
            SyConst(const bool, const std::string& v);
            SyConst(const SyConst&);
            virtual ~SyConst();

            SyConst& operator=(const SyConst&);
            SyConst operator[](const long) const;
            SyConst operator[](const SyConst*) const;
            SyConst operator+(const SyConst&) const;
            SyConst operator-(const SyConst&) const;
            SyConst operator*(const SyConst&) const;
            SyConst operator/(const SyConst&) const;
            SyConst operator%(const SyConst&) const;
            SyConst operator&(const SyConst&) const;
            SyConst operator|(const SyConst&) const;
            SyConst operator^(const SyConst&) const;

            SyConst operator>>(const unsigned int)const;
            SyConst operator<<(const unsigned int)const;

            SyConst operator-() const;
            SyConst operator+() const;
            SyConst operator~() const;

            bool operator==(const SyConst&) const;
            bool operator!=(const SyConst&) const;
            bool operator&&(const SyConst&) const;
            bool operator||(const SyConst&) const;
            bool operator>(const SyConst&) const;
            bool operator>=(const SyConst&) const;
            bool operator<(const SyConst&) const;
            bool operator<=(const SyConst&) const;

            bool operator!() const;
            SyConst operator&() const;

            //operator long() const { return this->toLong(); }

            SyConst xnor(const SyConst&) const;
            SyConst redor()  const;
            SyConst rednor()  const;
            SyConst rednand()  const;
            SyConst redxor()  const;
            SyConst redxnor()  const;
            SyConst sshiftright(unsigned int, bool) const;

            void shiftright(std::string&, unsigned int, char) const;
            //virtual SyConst* size() const {
            //    return new SyConst((actsize == INT_MIN) ? 
            //        val.size() : actsize, true, false);
            //}
            virtual SyConst* size() const {
                return new SyConst( val.size(), true, false);
            }
            virtual SyConst *evaluate() const {
                return this->copy();
                //return const_cast<SyConst*>(this);
            }
            const std::string& getVal() const {
                return val;
            }
            void twoscomp(std::string&) const;
            bool isNeg() const 
            {
                if(this->iszero() || this->isone()) {
                    return false;
                }    
                return this->isSigned() && val[0] == '1';
            }

            virtual std::string toString() const;
            //result will be invalid if num is biiger than MAX_LONG
            long toLong() const;
            virtual SyConst *copy() const {
                return new SyConst(*this);
            }

            std::vector<SyConst*> getBits() const;
            bool isone() const;
            bool iszero() const;
            SyConst slice(int left, int right) const;
            //truncate from msb, prserves signedness
            SyConst trunc(int ) const;

            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);
            static SyConst *ZERO() {
                return new SyConst(false, std::string("0"));
            }
            static SyConst *ONE() {
                return new SyConst(false, std::string("1"));
            }

        private:
            SyConst(const std::string& v, const bool);
            //These operations don't consider sign
            std::string add(const std::string&, const std::string&) const;
            std::string mult(const std::string&, const std::string&) const;

            std::pair<std::string, std::string> div(
                    const std::string&, 
                    const std::string&) const;


            std::pair<std::string, std::string> divLess64(
                    const SyConst&,
                    const SyConst&) const;


            bool less(const std::string&, const std::string&) const;
            bool equal(const std::string&, const std::string&) const;
            bool lesseq(const std::string&, const std::string&) const;
            void signextend(std::string&, std::string&) const;
            std::string extend(
                    const std::string&, 
                    int size
                    ) const;
            std::string extend(
                    const std::string&, 
                    char,
                    int size
                    ) const;
            void trunc(
                    std::string&,
                    int size) const;
            void trunc(
                    std::string&,
                    char) const;

            void resetCarry(char& carry) const {
                carry = '0';
            }
            void setCarry(char& carry) const {
                carry = '1';
            }
            bool isCarry(const char carry) const {
                return carry == '1';
            }

            std::string val;
            //for based numbers
            //int actsize;
    };
}
#endif
