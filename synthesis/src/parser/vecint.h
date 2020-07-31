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

#ifndef _VECINT_H_
#define _VECINT_H_
#include "omvisitor.h"
#include "vecvar.h"
#include "integer.h"
namespace Rtl
{
    class VecInt: public VecVar
    {
        public:
            VecInt(const std::string& n, const Dimension* d, 
                const bool s): VecVar(n, d, s)
            {
                this->createVarBits(n, d, d);
            }
            virtual ~VecInt(){
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }

            virtual VecInt* copy() const;
            virtual long len() const {
                return 32*this->getVars().size();
            }
            virtual void createVarBits(
                    const string& name, 
                    const Dimension* dim,
                    const bool isSig)
            {
                std::vector<Integer*> vars = this->flatten<Integer>(name, dim);
                const int size = vars.size();
                for(int i = 0; i < size; ++i)
                {
                    vars[i]->setSign(isSig);
                    vars[i]->setScope(this->getScope());
                    this->addBit(vars[i]);
                }
            }

            virtual const Integer* getBit(const long) const;
            std::vector<Integer*> getIntegers() const
            {
                std::vector<Integer*> ints;
                const std::vector<Var*>& vars = this->getVars();
                for(unsigned int i = 0; i < vars.size(); ++i){
                    ints.push_back(dynamic_cast<Integer*>(vars[i]));
                }
                return ints;
            }

        private:
            //void flatten();
    };
}
#endif
