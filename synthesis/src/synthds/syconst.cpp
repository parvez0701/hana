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

#include <sstream>
#include <math.h>
#include <algorithm>
#include <assert.h>

#include "syconst.h"
#include "syvisitor.h"
#include "tvisitor.h"

using std::string;
using std::ostringstream;
using std::pair;
using std::vector;
namespace Synth
{
    SyConst::SyConst(
            signed long v,
            const bool sign,
            const bool neg)
        :SyCntv(sign)
    {
        //actsize = INT_MIN;
        unsigned long tmp = v;
        do 
        {
            if(tmp % 2) {
                val = string("1") + val;
            }
            else {
                val = string("0") + val;
            }
            tmp = tmp >> 1;
        }while(tmp);
        if(sign){
            if(neg){
                val = string("1") + val;
            }
            else {
                val = string("0") + val;
            }
        }
    }

    SyConst::SyConst(
            const string& v,
            const bool sign)
        :SyCntv(sign), val(v) {
            //actsize = INT_MIN;
            if(!sign && (val[0] != '0')){
                val = string("0") + val;
            }
        }
    SyConst::SyConst(
            const bool sign,
            const string& v)
        :SyCntv(sign), val(v) {
            //actsize = INT_MIN;
        }

    SyConst::SyConst(const SyConst& lhs) : SyCntv(lhs.isSigned()) {
        val = lhs.val;
        //actsize = lhs.actsize;
    }

    SyConst::~SyConst(){
    }

    void
        SyConst::accept(SyVisitorBase* v) const{
            v->visit(this);
        }
    void
        SyConst::accept(
                TVisitor *v,
                SyModule *m){
            v->visit(this, m);
        }

    SyConst&
        SyConst::operator=(const SyConst& lhs)
        {
            val = lhs.getVal();
            //actsize = lhs.actsize;
            this->setSign(lhs.isSigned());
            return *this;
        }

        /*
    void
        SyConst::setSize(int s)
        {
            actsize = s;
            int diff = actsize - val.size();
            if(!isSigned() && (val[0] == '0')) {
                //don't consider added bit
                ++diff;
            }    
            if(diff > 0){
                val = this->extend(val, actsize);
            }
            else if (diff < 0) 
            {
                diff = val.size()-actsize;
                this->trunc(val, diff);
            }

        }
            */

    SyConst
        SyConst::operator[](const long bit) const
        {
            ostringstream str;
            str << val[bit];
            //returns non-signed, non neg const
            return SyConst(str.str(), false);
        }
    SyConst
        SyConst::operator[](const SyConst* bit) const
        {
            return this->operator[](bit->toLong());
        }

    //Definition of verilog operators

    bool
        SyConst::operator==(const SyConst& lhs) const
        {
            //if(this->isSigned() != lhs.isSigned()){
            //    return false;
            //}
            string tval = val;
            string lval = lhs.getVal();
            this->trunc(tval, '0');
            this->trunc(lval, '0');

            return tval == lval;
        }


    bool
        SyConst::operator!=(const SyConst& lhs) const
        {
            return !(*this == lhs);
        }
    SyConst
        SyConst::operator+(const SyConst& rhs) const
        {
            string result;
            string tval = val;
            string rval = rhs.getVal();
            const int size1 = tval.size();
            const int size2 = rval.size(); 
            const int size = size1 > size2 ? size1 + 1 : size2 + 1;

            tval = this->extend(tval, this->isSigned() ? tval[0] : '0', size);
            rval = this->extend(rval, rhs.isSigned() ? rval[0] : '0', size);
            result = this->add(tval, rval);
            this->trunc(result, size);
            return SyConst(result, this->isSigned() && rhs.isSigned());
        }

    SyConst
        SyConst::operator-(const SyConst& rhs) const
        {
            string result;
            string rval = rhs.getVal();
            if(this->isSigned() && rhs.isSigned())
            {
                if(!this->isNeg() && !rhs.isNeg())
                {
                    //A-B
                    this->twoscomp(rval);
                    return *this + SyConst(rval, true);
                }
                else if(this->isNeg() && !rhs.isNeg())
                {
                    //-A-B
                    this->twoscomp(rval);
                    return *this + SyConst(rval, true);
                }
                else if(!this->isNeg() && rhs.isNeg())
                {
                    //A-(-B)
                    this->twoscomp(rval);
                    return *this + SyConst(rval, true);
                }
                else
                {
                    //-A -(-B)
                    this->twoscomp(rval);
                    return *this + SyConst(rval, true);
                }
            }
            else {
                this->twoscomp(rval);
                return *this + SyConst(rval, false);
            }
            return SyConst(result, this->isSigned() && rhs.isSigned());
        }
    SyConst
        SyConst::operator*(const SyConst& rhs) const
        {
            //To be changed to Boothe's algorithm
            string result;
            string rval = rhs.getVal();
            string tval = val;
            if(this->isSigned() && rhs.isSigned())
            {
                bool neg = (this->isNeg() && !rhs.isNeg()) || 
                    (!this->isNeg() && rhs.isNeg());

                if(this->isNeg()){
                    this->twoscomp(tval);
                }
                if(rhs.isNeg()){
                    this->twoscomp(rval);
                }
                result = this->mult(tval, rval);
                if(neg){
                    this->twoscomp(result);
                }
            }
            else {
                result = this->mult(tval, rval);
            }
            return SyConst(result, this->isSigned() && rhs.isSigned());
        }

    SyConst
        SyConst::operator/(const SyConst& divisor) const
        {
            string result;
            if(this->isSigned() && divisor.isSigned())
            {
                string rval = divisor.getVal();
                //rval = rval.substr(1);
                //string tval = val.substr(1);
                string tval = val;
                if(this->isNeg()){
                    this->twoscomp(tval);
                }
                if(divisor.isNeg()){
                    this->twoscomp(rval);
                }
                result = this->div(tval, rval).first;
                this->trunc(result, '0');

                if((this->isNeg() && !divisor.isNeg()) ||
                        (!this->isNeg() && divisor.isNeg())){
                    result = string("1") + result;
                }
                else {
                    result = string("0") + result;
                }
            }
            else if((val.size() < 8*sizeof(long)) && 
                divisor.getVal().size() < 8*sizeof(long)) {
                result = divLess64(*this, divisor).first;
            }    
            else {
                result = this->div(val, divisor.getVal()).first;
            }
            SyConst *c1 = this->size();
            SyConst *c2 = divisor.size();

            long exprsize = std::max(c1->toLong(), c2->toLong());
            delete c1;
            delete c2;
            int diff = exprsize - result.size();
            if(diff > 0)
            {
                char sbit = result[0];
                result = string(diff, sbit) + result;
            }

            return SyConst(result, this->isSigned() && divisor.isSigned());
        }
    SyConst
        SyConst::operator%(const SyConst& divisor) const
        {
            string result;
            if(this->isSigned() && divisor.isSigned())
            {
                string rval = divisor.getVal();
                //rval = rval.substr(1);
                //string tval = val.substr(1);
                string tval = val;
                if(this->isNeg()){
                    this->twoscomp(tval);
                }
                if(divisor.isNeg()){
                    this->twoscomp(rval);
                }
                result = this->div(tval, rval).second;
                this->trunc(result, '0');

                if((this->isNeg() && !divisor.isNeg()) ||
                        (!this->isNeg() && divisor.isNeg())){
                    result = string("1") + result;
                }
                else {
                    result = string("0") + result;
                }
            }
            else if((val.size() < 8*sizeof(long)) && 
                divisor.getVal().size() < 8*sizeof(long)) {
                //Make shift arrangement to handle very big unsigned numbers.
                //These numbers may go into infinite loop otherwise.
                result = divLess64(*this, divisor).second;
            }    
            else {
                result = this->div(val, divisor.getVal()).second;
            }
            SyConst *c1 = this->size();
            SyConst *c2 = divisor.size();

            long exprsize = std::max(c1->toLong(), c2->toLong());
            delete c1;
            delete c2;
            int diff = exprsize - result.size();
            if(diff > 0)
            {
                char sbit = result[0];
                result = string(diff, sbit) + result;
            }

            return SyConst(result, this->isSigned() && divisor.isSigned());
        }

    SyConst
        SyConst::operator+() const
        {
            //unary plus
            return SyConst(*this);
        }

    SyConst
        SyConst::operator-() const
        {
            //unary minus
            string tval = val;
            this->twoscomp(tval);
            return SyConst(this->isSigned(), tval);
        }

    bool
        SyConst::operator!()const
        {
            string tval = val;
            this->trunc(tval, '0');
            return tval.empty();
        }

    bool
        SyConst::operator&&(const SyConst& lhs) const
        {
            return (!(!(*this))) && (!(!lhs));
        }
    bool
        SyConst::operator||(const SyConst& lhs) const
        {
            return (!(!(*this))) || (!(!lhs));
        }
    bool
        SyConst::operator>(const SyConst& lhs) const
        {
            if(!this->isSigned() || !lhs.isSigned()){
                return less(lhs.getVal(), val);
            }

            //signed comparison
            if(!this->isNeg() && !lhs.isNeg()){
                return this->less(lhs.getVal(), val);
            }
            
            if(this->isNeg() && lhs.isNeg())
            {
                string trhs = val;
                string tlhs = lhs.getVal();
                this->twoscomp(trhs);
                this->twoscomp(tlhs);
                return this->less(trhs, tlhs);
            }

            if(this->isNeg()) {
                return false;
            }
            return true;
        }
    bool
        SyConst::operator>=(const SyConst& rhs) const
        {
            return (*this > rhs) || (*this == rhs);
        }
    bool
        SyConst::operator<(const SyConst& rhs) const
        {
            return (*this != rhs) && !(*this > rhs);
        }
    bool
        SyConst::operator<=(const SyConst& rhs) const
        {
            return !(*this > rhs);
        }

    SyConst
        SyConst::operator~() const
        {
            string tval = val;
            for(int i = 0; i < (int)tval.size(); ++i){
                if(tval[i] == '0'){
                    tval[i] = '1';
                }
                else if(tval[i] == '1'){
                    tval[i] = '0';
                }
            }
            return SyConst(this->isSigned(), tval);
        }
    SyConst
        SyConst::operator&(const SyConst& lhs) const
        {
            string result;
            string tval = val;
            string lval = lhs.getVal();
            const int size1 = tval.size();
            const int size2 = lval.size(); 
            const int size = size1 > size2 ? size1 : size2;

            tval = this->extend(tval, size);
            lval = this->extend(lval, size);
            for(int i = 0; i < size; ++i){
                if(tval[i] == lval[i]){
                    result.push_back(tval[i]);
                }
                else {
                    result.push_back('0');
                }
            }
            return SyConst(result, this->isSigned() && lhs.isSigned());
        }

    SyConst
        SyConst::operator|(const SyConst& lhs) const
        {
            string result;
            string tval = val;
            string lval = lhs.getVal();
            const int size1 = tval.size();
            const int size2 = lval.size(); 
            const int size = size1 > size2 ? size1 : size2;

            tval = this->extend(tval, size);
            lval = this->extend(lval, size);
            for(int i = 0; i < size; ++i){
                if((tval[i] == '1') || (lval[i] == '1')){
                    result.push_back('1');
                }
                else {
                    result.push_back('0');
                }
            }
            return SyConst(result, this->isSigned() && lhs.isSigned());
        }

    SyConst
        SyConst::operator^(const SyConst& lhs) const
        {
            string result;
            string tval = val;
            string lval = lhs.getVal();
            const int size1 = tval.size();
            const int size2 = lval.size(); 
            const int size = size1 > size2 ? size1 : size2;

            tval = this->extend(tval, size);
            lval = this->extend(lval, size);
            for(int i = 0; i < size; ++i){
                if(((tval[i] == '1') && (lval[i] == '0')) ||
                        ((tval[i] == '0') && (lval[i] == '1'))){
                    result.push_back('1');
                }
                else {
                    result.push_back('0');
                }
            }
            return SyConst(result, this->isSigned() && lhs.isSigned());
        }


    SyConst
        SyConst::operator&() const
        {
            bool flag = true;
            for(int i = (int)val.size() -1; i > 0; --i)
            {
                if(val[i] == '1' && val[i-1] == '1'){
                    flag = true;
                }
                else{
                    flag = false;
                }
            }
            if(flag){
                return SyConst(isSigned(), isSigned() ? "01" : "1");
            }
            else {
                return SyConst(isSigned(), isSigned() ? "00" : "0");
            }
        }

    SyConst
        SyConst::operator>>(const unsigned int shift) const
        {
            string tval = val;
            this->shiftright(tval, shift, '0');
            return SyConst(tval, this->isSigned());
        }

    SyConst
        SyConst::operator<<(const unsigned int shift) const
        {
            string tval = val;
            if((int)tval.size() <= shift)
            {
                tval = string(tval.size(), '0');
                return SyConst(this->isSigned(), tval);
            }
            /*
            const int ulimit = tval.size() - shift -1;

            int i;
            for(i = 0; i < ulimit; ++i){
                tval[i] = tval[ulimit-i];
            }
            for(; i < (int)tval.size(); ++i){
                tval[i] = '0';
            }*/
            int tsize = tval.size();
            for(int i = 0; i < tsize; ++i) 
            {
                int index = i+shift;
                tval[i] = (index < tsize) ? tval[index] : '0';
            }    
            return SyConst(this->isSigned(), tval);
        }

    SyConst 
        SyConst::sshiftright(
                unsigned int shift,
                bool sign_) const 
        {
            if(!sign_ || !this->isNeg()){
                return *this >> shift;
            }
            string tval = val;
            this->shiftright(tval, shift, '1');
            return SyConst(tval, true);
        }

    void
        SyConst::shiftright(
                string& tval,
                const unsigned int shift,
                char schar) const
        {
            if((int)tval.size() <= shift)
            {
                tval = string(tval.size(), '0');
                return;
            }
            const int ulimit = tval.size()-1;
            const int llimit = ulimit - shift;

            for(int i = 0; i <= llimit; ++i){
                tval[ulimit-i] = tval[llimit-i];
            }
            for(int i = 0; i < shift; ++i){
                tval[i] = schar;
            }
        }

    SyConst
        SyConst::xnor(const SyConst& lhs) const
        {
            //implemenation of ~^ and ^~ operators
            return ~(*this^lhs);
        }

    SyConst
        SyConst::redxor() const
        {
            bool flag = true;
            for(int i = (int)val.size() -1; i > 0; --i)
            {
                if((val[i] == '1' && val[i-1] == '0') ||
                        (val[i] == '0' && val[i-1] == '1')){
                    flag = true;
                }
                else{
                    flag = false;
                }
            }

            if(flag){
                return SyConst(isSigned(), isSigned() ? "01" : "1");
            }
            else {
                return SyConst(isSigned(), isSigned() ? "00" : "0");
            }
        }

    SyConst
        SyConst::redxnor() const
        {
            return ~(this->redor());
        }
    SyConst
        SyConst::rednand() const {
            return ~(this->operator&());
        }

    SyConst
        SyConst::redor() const
        {
            bool flag = false;
            for(int i = (int)val.size() -1; i >= 0; --i)
            {
                if(val[i] == '1')
                {
                    flag = true;
                    break;
                }
            }
            if(flag){
                //check: should sign be considered ?
                return SyConst("01", true);
            }
            else {
                return SyConst("00", true);
            }
        }
    SyConst
        SyConst::rednor() const
        {
            return ~(this->redor());
        }

    //End verilog operators

    string
        SyConst::add(
                const string& v1, const string& v2) const
        {
            string tv1 = v1;
            string tv2 = v2;
            string result;
            char carry = '0';

            for(int i = (int)tv1.size() - 1; i >=0; --i)
            {
                if(tv1[i] == '0' && tv2[i] == '0')
                {
                    result.push_back(carry);
                    this->resetCarry(carry);
                }
                else if((tv1[i] == '0' && tv2[i] == '1') ||
                        (tv1[i] == '1' && tv2[i] == '0'))
                {
                    if(isCarry(carry)){
                        result.push_back('0');
                    }
                    else {
                        result.push_back('1');
                    }
                }
                else
                {
                    //note that if x and z are there they are also
                    //treated as 1
                    if(this->isCarry(carry)){
                        result.push_back('1');
                    }
                    else {
                        result.push_back('0');
                        this->setCarry(carry);
                    }
                }
            }

            if(this->isCarry(carry)) {
                result.push_back('1');
            }

            std::reverse(result.begin(), result.end());
            return result;
        }



    void
        SyConst::twoscomp(string& num) const
        {
            int tsize = num.size();
            for(int i = 0; i < tsize; ++i){
                if(num[i] == '0'){
                    num[i] = '1';
                }
                else {
                    num[i] = '0';
                }
            }
            string one = string(num.size()-1, '0') + string("1");
            num = this->add(num, one);
            num = num.substr(num.size()-tsize);
        }

    bool
        SyConst::less(
                const string& lhs,
                const string& rhs) const
        {
            //does not include sign into consideration
            string tlhs = lhs;
            string trhs = rhs;
            int i = 0;
            int j = 0;
            while(i < (int)tlhs.size() && (tlhs[i] == '0')){
                ++i;
            }
            while(j < (int)trhs.size() && (trhs[j] == '0')){
                ++j;
            }
            tlhs = tlhs.substr(i);
            trhs = trhs.substr(j);
            if(tlhs.size() < trhs.size()){
                return true;
            }
            if(tlhs.size() > trhs.size()){
                return false;
            }

            for(i = 0; i < (int)tlhs.size(); ++i){
                if(tlhs[i] < trhs[i]) {
                    return true;
                }
                else if(tlhs[i] > trhs[i]){
                    return false;
                }
            }
            return false;
        }
    bool
        SyConst::equal(
                const string& lhs,
                const string& rhs
                )const
        {
            //does not include sign into consideration
            string tlhs = lhs;
            string trhs = rhs;
            int i = 0;
            int j = 0;
            while(i < (int)tlhs.size() && (tlhs[i] == '0')){
                ++i;
            }
            while(j < (int)trhs.size() && (trhs[j] == '0')){
                ++j;
            }
            tlhs = tlhs.substr(i);
            trhs = trhs.substr(j);
            return tlhs == trhs;
        }

    bool
        SyConst::lesseq(
                const string& lhs,
                const string& rhs
                )const
        {
            return this->less(lhs, rhs) || this->equal(lhs, rhs);
        }
    string
        SyConst::toString() const
        {
            //string tval = this->isSigned() ? val.substr(1) : val;
        /*    int count = val.size() - 1;
            signed long num;
            if(this->isSigned()) {
                num = -1 * (val[0] - '0') * pow(2, count--);
            }
            else {
                num = (val[0] - '0') * pow(2, count--);
            }
                
            for(int i = 1; i < val.size(); ++i){
                num = num + (val[i] - '0') * (long)pow(2, count--);
            }
            */

            ostringstream str;
            str << this->toLong();
            return str.str();
        }

    long
        SyConst::toLong() const
        {
            int count = val.size() - 1;
            signed long num;
            if(this->isSigned()) {
                num = -1 * (val[0] - '0') * pow(2, count--);
            }
            else {
                num = (val[0] - '0') * pow(2, count--);
            }
                
            for(int i = 1; i < (int)val.size(); ++i){
                num = num + (val[i] - '0') * (long)pow(2, count--);
            }
            return num;
        }

    string
        SyConst::mult(
                const string& first,
                const string& second) const
        {
            string result;
            string tmp;
            int shift = 1;

            for(int i = (int)first.size() -1; i >=0 ; --i)
            {
                tmp.clear();
                for(int j = (int)second.size() -1; j >= 0; --j)
                {
                    if((first[i] == '0') || (second[j] == '0')){
                        tmp.push_back('0');
                    }
                    else {
                        tmp.push_back('1');
                    }
                }
                if(result.empty()){
                    std::reverse(tmp.begin(), tmp.end());
                    result = tmp;
                }
                else {
                    std::reverse(tmp.begin(), tmp.end());
                    result = string("0") + result;
                    tmp = tmp + string(shift++, '0');
                    SyConst t1(result, false);
                    SyConst t2(tmp, false);
                    SyConst t3 = t1 + t2;
                    result = t3.getVal();
                }
            }
            return result;
        }

    pair<string, string>
        SyConst::div(
                const string& dividend,
                const string& divisor) const
        {
            //In return pair, first string is quotient and the 
            //second one is remainder.
            string quotient("00");
            string div(dividend);
            if(div[0] != '0') {
                div = string("0") + div;
            }
            string divs(divisor);
            if(divs[0] != '0') {
                divs = string("0") + divs;
            }
            SyConst cdivisor(divs, true);
            SyConst cdividend(div, true);

            while(this->lesseq(divs, div)){
                quotient = this->add(quotient, 
                        string(quotient.size()-1, '0') + string("1"));
                cdividend = cdividend - cdivisor;
                div = cdividend.getVal();
            }
            return make_pair(string("0") + quotient, div);
        }

    pair<string, string>
        SyConst::divLess64(const SyConst& dividend, const SyConst& divisor) const
    {
        int size = dividend.getVal().size() > divisor.getVal().size() ?
            dividend.getVal().size() : divisor.getVal().size();
        unsigned long val1 = dividend.toLong();
        unsigned long val2 = divisor.toLong();
        unsigned long quotient = val1/val2;
        unsigned long remainder = val1%val2;

        return make_pair(Long2String(quotient, size, '0'), 
            Long2String(remainder, size, '0'));
    }
    void
        SyConst::signextend(
                string& n1, string& n2) const
        {
            const int size = n1.size() > n2.size() ? n1.size() : n2.size();
            int i = 0;
            while(1) {
                if((2*i) > size) {
                    break;
                }
                ++i;
            }
            i = pow(2, i);
            n1 = this->extend(n1, i);
            n2 = this->extend(n2, i);
        }

    string 
        SyConst::extend(
                const string& num,
                int size
                ) const
        {
            string tnum = num;

            if((int)tnum.size() == size) {
                return num;
            }
            char extchar = num[0];
            //1 for original sign bit
            return string(size - tnum.size(), extchar) + tnum;
            
        }

    string 
        SyConst::extend(
                const string& num,
                char extchar, 
                int size
                ) const
        {
            string tnum = num;

            if((int)tnum.size() == size) {
                return num;
            }
            return string(size - tnum.size(), extchar) + tnum;
            
        }
    void
        SyConst::trunc(
                string& num,
                int size) const
        {
            num = num.substr(num.size() - size);
        }
    void
        SyConst::trunc(
                string& num,
                char c) const
        {
            int i = 0;
            while(num[i] == c){
                ++i;
            }
            num = num.substr(i);
        }

    vector<SyConst*>
        SyConst::getBits() const
        {
            vector<SyConst*> bits;
            for(int i = 0; i < (int)val.size(); ++i)
            {
                if(val[i] == '1'){
                    bits.push_back(new SyConst(false, string("1")));
                }
                else {
                    bits.push_back(new SyConst(false, string("0")));
                }
            }
            return bits;
        }

    SyConst
        SyConst::slice(
                int left,
                int right) const
        {
            SyConst const_(false, val.substr(left, right));
            return const_;
            //return SyConst(val.substr(left, right), false);
        }


    bool
        SyConst::isone() const
        {
            return *this == SyConst("01", this->isSigned());
        }

    bool
        SyConst::iszero() const 
        {
            return *this == SyConst("00", this->isSigned());
        }

    SyConst
        SyConst::trunc(int count) const
        {
            SyConst const_(this->isSigned(), val.substr(count));
            return const_;
        }

}
