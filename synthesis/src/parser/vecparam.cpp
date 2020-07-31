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

#include <cassert>

#include "vecparam.h"
#include "omvisitor.h"
#include "basednum.h"

using std::vector;
namespace Rtl
{
    VecParam::VecParam(const string& n, const Dimension* d,
        const bool is): VecCntv(n, d)
    {
        this->createParamBits(n, d, is);
    }

    void
    VecParam::createParamBits(const string& name, const Dimension* dim,
        const bool isSig)
    {
        vector<Param*> params = this->flatten<Param>(name, dim);
        const int size = params.size();
        for(int i = 0; i < size; ++i){
            params[i]->setSign(isSig);
        }

    }
    vector<Param*>
    VecParam::getParams() const
    {
        vector<Param*> params;
        long msb = this->getMsb();
        long lsb = this->getLsb();
        for(int i = msb; (msb > lsb) ? (i >= lsb) : (i <= lsb);
            (msb > lsb) ? --i : ++i) {
            params.push_back(const_cast<Param*>(this->getBit(i)));
        }    

        return params;
    }

    void
    VecParam::setVal(const Expr *val)
    {
        VecCntv::setVal(val);
        assert(val && val->isConstExpr());
        int num = val->evaluate();
        vector<Param*> params = this->getParams();

        for(int i = params.size()-1; i >= 0; --i)
        {
            vector<char> bits;
            bits.push_back(num % 2 ? '1' : '0');
            params[i]->val = new BasedNum(1, 'b', this->isSigned(), bits);
            num /= 2;
        }
    }
    void
    VecParam::accept(OMVisitorBase* visitor) const {
        visitor->visit(this);
    }

    const Param*
    VecParam::getBit(const long bit) const {
        return static_cast<const Param*>(VecCntv::getBit(bit));
    }

    bool
    VecParam::isSigned() const
    {
        vector<Param*> params = this->getParams();
        return params[0]->isSigned();
    }

    bool
    VecParam::isSignSpecified() const
    {
        vector<Param*> params = this->getParams();
        return params[0]->isSignSpecified();
    }

    long
    VecParam::len() const {
        return abs(this->getMsb()-this->getLsb())+1;
    }

    int
    VecParam::evaluate() const
    {
        vector<Param*> params = this->getParams();
        vector<char> bits;

        for(int i = 0; i < params.size(); ++i) {
            bits.push_back((params[i]->evaluate() == 0) ? '0': '1');
        }
        BasedNum *num = new BasedNum(this->len(), 'b', this->isSigned(), bits);
        int result = num->evaluate();

        delete num;
        return result;
    }
    

}
