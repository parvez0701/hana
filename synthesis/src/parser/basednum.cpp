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
#include <iostream>
#include <algorithm>
#include <cassert>
#include <ctype.h>
#include <stdlib.h>
#include "basednum.h"
#include "utils.h"
#include "omvisitor.h"
using std::vector;
extern void yyerror( char const*);
namespace Rtl
{
    BasedNum::BasedNum(const int siz, const char b, const bool s,
        const std::vector<char>& bts) : Const(),
        size(siz), base(b), sign(s), bits(bts)
    {
        if(base != 'd' && base != 'D' && base != 'b' && base != 'B' &&
            base != 'o' && base != 'O' && base != 'h' && base != 'H')
        {
            yyerror("Incorrect base specified for based number.");
            throw_error;
        }

        if(!checkBinary() || !checkOctal() || !checkDecimal() ||
            !checkHex())
        {
            yyerror("Invalid based number.");
            throw_error;
        }
        convertBits();
            
    }

    BasedNum::~BasedNum(){
    }

    int
    BasedNum::evaluate() const {
        return (int)Evaluate(base, bits);
    }

    bool
    BasedNum::checkBinary()
    {
        if(base != 'b' && base != 'B'){
            return true;
        }

        const int width = bits.size();
        for(int i = 0; i < width; ++i){
            if(bits[i] != '0' && bits[i] != '1' && 
                    !isAllowedChar(bits[i])){
                return false;
            }
        }
        return true;
    }
    bool
    BasedNum::checkOctal()
    {
        if(base != 'o' && base != 'O'){
            return true;
        }
        const int width = bits.size();
        for(int i = 0; i < width; ++i){
            if((!isAllowedChar(bits[i]) && !isdigit(bits[i]))
                       || (bits[i] == '9')){
            return false;
            }
        }
        return true;
    }

        
    bool
    BasedNum::checkDecimal()
    {
        if(base != 'd' && base != 'D'){
            return true;
        }
        const int width = bits.size();
        for(int i = 0; i < width; ++i){
            if(!isAllowedChar(bits[i]) && 
                        !isdigit(bits[i])){
                return false;
            }
        }
        return true;
    }

    bool
    BasedNum::checkHex()
    {
        if(base != 'h' && base != 'H'){
            return true;
        }
        const int width = bits.size();
        for(int i = 0; i < width; ++i){
            if(!isxdigit(bits[i]) && !isAllowedChar(bits[i])){
                return false;
            }
        }
        return true;
    }

    void
    BasedNum::accept(OMVisitorBase* v) const{
        v->visit(this);
    }

    string
    BasedNum::toString() const 
    {
        char str[1024];
        if(size != 0){
            sprintf(str, "%d", size);
        }
        strcat(str, "'");
        if(sign){
            strcat(str, "s");
        }
        int len = strlen(str);
        str[len] = base;
        for(unsigned int i = 0; i < bits.size(); ++i){
            str[++len] = bits[i];
        }
        str[++len] = '\0';
        return str;
    }

    long
    BasedNum::len() const{
        return size;
    /*
        long val = this->evaluate();
        long s = 0;
        do {
            ++s;
            val = val >> 1;
        }while(val > 0);
        return s > size ? s : size;
    */    
    }
        

    const BasedNum*
    BasedNum::getBit(const long bit) const
    {
        if(bit < 0 || bit > this->len()){
            return NULL;
        }

        int val = this->evaluate();
        val = val >> (bit-1);
        val = val & 1;
        vector<char> bits;
        bits.push_back( val ? '1': '0');
        return new BasedNum(1, 'b', false, bits);
    }

    bool
        BasedNum::isUnknownOrHighImpedance(char c) const {
            return (c == 'x' || c == 'X' || c == 'z' || c == 'Z');
        }
    
    bool
        BasedNum::isAllowedChar(char c) const
        {
            if(isUnknownOrHighImpedance(c)) {
                return true;
            }
            if(c == '?' || c == '_') {
                return true;
            }
            return false;
        }

    bool
        BasedNum::isUnknownOrHighImpedance() const {
            for(unsigned int i = 0; i < bits.size(); ++i) {
                if(this->isUnknownOrHighImpedance(bits[i])){
                    return true;
                }
            }
            return false;
        }    

    BasedNum*
        BasedNum::convert2Base2() const {
            if((base == 'b') || (base == 'B')) {
                return this->copy();
            }
            vector<char> tmp;
            if((base == 'd') || (base == 'D')) {
                tmp = this->convertDecimal2Binary(bits);
            }
            else if((base == 'o') || (base == 'O')) {
                tmp = this->convertOctal2Binary(bits);
            }
            else {
                tmp = this->convertHex2Binary(bits);
            }    
            
            if(size < tmp.size())
            {
                vector<char> tt = tmp;
                tmp.clear();
                for(int i = tt.size()-size; i < tt.size(); ++i) {
                    tmp.push_back(tt[i]);
                }
            }
            else if(size > tmp.size())
            {
                //char ext = bits[0];
                int tsize = tmp.size();
                for(int i = tsize; i < size; ++i) {
                    tmp.insert(tmp.begin(), '0');
                }
            }    
            //return new BasedNum(size, 'b', sign, tmp);
            //BasedNum bbb(size, 'b', sign, tmp);
            //std::cout << std::endl;
            //std::cout << "NUMBER " << bbb.evaluate() << std::endl;
            //std::cout << std::endl;
            return new BasedNum(size, 'b', sign, tmp);
        
        }

    vector<char>
        BasedNum::convertDecimal2Binary(vector<char> dbits) const 
        {
            vector<char> result;
            if(dbits.empty()) {
                return result;
            }

            bool done = false;

            while(!done)
            {
                char last = dbits[dbits.size()-1];
                if(last == '1' || last == '3' || last == '5' ||
                    last == '7' || last == '9') {
                    result.push_back('1');
                }
                else {
                    result.push_back('0');
                }    

                bool carry = false;
                for(int i = 0; i < dbits.size(); ++i)
                {
                    char c = dbits[i];
                    short digit = c -'0';
                    if(carry) {
                        digit += 10;
                    }    
                    dbits[i] = digit/2 + '0';
                    carry = digit%2;
                }
                done = true;

                for(int i = 0; i < dbits.size(); ++i) {
                    if(dbits[i] != '0') {
                        done = false;
                    }
                }    
            }

            std::reverse(result.begin(), result.end());
            return result;
            
        }    
    vector<char>
        BasedNum::convertOctal2Binary(vector<char> obits) const
        {
            vector<char> result;
            for(int i = 0; i < obits.size(); ++i)
            {
                vector<char> tmp;
                tmp.insert(tmp.begin(), obits[i]);
                tmp = this->convertDecimal2Binary(tmp);
                int origsize = tmp.size();
                std::reverse(tmp.begin(), tmp.end());
                for(int i = 0; i < (3-origsize); ++i) {
                    //tmp.insert(tmp.begin(), '0');
                    tmp.push_back( '0');
                }
                std::reverse(tmp.begin(), tmp.end());
                for(int i = 0; i < tmp.size(); ++i) {
                    result.push_back(tmp[i]);
                }    
            }
            return result;
        }

    vector<char>
        BasedNum::convertHex2Binary(vector<char> hbits) const
        {
            vector<char> result;
            for(int i = 0; i < hbits.size(); ++i)
            {
                vector<char> tmp;
                if(isalpha(hbits[i])) 
                {
                    tmp.push_back('1');
                    tmp.push_back('0'+ toupper(hbits[i]) -'A');
                }
                else {
                    tmp.push_back(hbits[i]);
                }    
                tmp = this->convertDecimal2Binary(tmp);
                int origsize = tmp.size();
                std::reverse(tmp.begin(), tmp.end());
                for(int i = 0; i < (4-origsize); ++i) {
                    tmp.push_back('0');
                    //tmp.insert(tmp.begin(), '0');
                }
                std::reverse(tmp.begin(), tmp.end());
                for(int i = 0; i < tmp.size(); ++i) {
                    result.push_back(tmp[i]);
                }    
            }
            return result;
        }

        void
        BasedNum::convertBits() 
        {
            int j = 0;
            for(int i = 0; i < bits.size(); ++i)
            {
                if(bits[i] == '_') {
                    continue;
                }
                bits[j++] = (bits[i] == '?') ? 'z' : bits[i];
            }
        }

        vector<char>
        BasedNum::getExtendedBits() const
        {
            vector<char> extbits;
            if(toupper(base) == 'D') {
                num2BitsDec(extbits);
            }
            else {
                for(int i = 0; i < bits.size(); ++i)
                {
                    char c = bits[i];
                    vector<char> tmp;
                    num2Bits(c, tmp);
                    std::copy(tmp.begin(), tmp.end(), std::back_inserter(extbits));
                }    
            }    

            if( size > (int)extbits.size())
            {
                int diff = size - (int)extbits.size();
                vector<char> tmp;
                for(int i = 0; i < diff; ++i)
                {
                    if(!isSigned()) {
                        tmp.push_back('0');
                    }
                    else {
                        tmp.push_back(extbits[0]);
                    }    
                }
                std::copy(extbits.begin(), extbits.end(), std::back_inserter(tmp));
                return tmp;
            }
            else if( size < (int)extbits.size())
            {
                vector<char> tmp;
                vector<char>::iterator iter = extbits.begin();
                std::advance(iter, extbits.size()-size);
                std::copy(iter, extbits.end(), std::back_inserter(tmp));
                return tmp;
            }    
            return extbits;
        }

    void
    BasedNum::num2BitsDec(vector<char>& vbits) const
    {
        //TBD
        //X or Z not handled
        int val = this->evaluate();
        do    
        {
            vbits.push_back(val%2 ? '1' : '0');
            val /= 2;
        } while(val);
        std::reverse(vbits.begin(), vbits.end());
    }
        

    void BasedNum::num2Bits(char c, vector<char>& vbits) const
    {
        char b = toupper(base);
        c = toupper(c);
        int chunksize = b == 'B' ? 1 : b == 'O' ? 3 :  4;
        if( c == 'X' || c == 'Z')
        {
            for(int i = 0; i < chunksize; ++i) {
                vbits.push_back(c);
            }
            return;
        }
        int num = isdigit(c) ? c - '0' : toupper(c) - 'A' + 10;
        do    
        {
            vbits.push_back(num%2 ? '1' : '0');
            num /= 2;
        } while(num);


        int diff = chunksize-vbits.size();
        for(int i = 0; i < diff; ++i) {
            vbits.push_back('0');
        }    

        std::reverse(vbits.begin(), vbits.end());
        
    }
}
