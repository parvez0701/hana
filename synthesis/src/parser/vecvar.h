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

#ifndef _VECVAR_H_
#define _VECVAR_H_
#include "var.h"
#include "veccntv.h"
namespace Rtl
{
    class VecVar: public VecCntv
    {
        public:
            VecVar(const std::string&,
                const Dimension*, const bool);

            virtual ~VecVar(){
            }

            //virtual Var* getCntvBit(const long) const throw(Error);
            const std::vector<Var*>& getVars() const{
                return vars;
            }
            const Expr* getVal() const{
                return vars[0]->getVal();
            }
            bool isSigned() const{
                return vars[0]->isSigned();
            }
            void addBit(const Var *v){
                if(v){
                    vars.push_back(const_cast<Var*>(v));
                }
            }
        protected:
            virtual void createVarBits(const std::string&,
                const Dimension*, const bool);
            Dimension* createDim() const;    

        private:
        //    void createVarBits(const std::string&,
        //        const Dimension*, const bool);
    //        std::map<long, Var*> regMap;
            std::vector<Var*> vars;
    };
}
#endif
