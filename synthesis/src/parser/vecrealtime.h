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

#ifndef _VECREALTIME_H_
#define _VECREALTIME_H_

#include "vecvar.h"
#include "omvisitor.h"
namespace Rtl
{
    class VecRealTime: public VecVar
    {
        public:
            VecRealTime(const std::string& n, const Dimension* d, 
                const bool s): VecVar(n, d, s)
            {
                this->createVarBits(n, d, d);
            }
            virtual ~VecRealTime(){
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual long len() const {
                return 32*this->getVars().size();
            }

            virtual VecRealTime* copy() const {
                return new VecRealTime(this->getName(), this->createDim(),
                    this->isSigned());
            }        
            
            virtual void createVarBits(
                    const string& name, 
                    const Dimension* dim,
                    const bool isSig)
            {
                std::vector<RealTime*> vars = this->flatten<RealTime>(name, dim);
                const int size = vars.size();
                for(int i = 0; i < size; ++i)
                {
                    vars[i]->setSign(isSig);
                    vars[i]->setScope(this->getScope());
                    this->addBit(vars[i]);
                }
            }
    };
}
#endif
