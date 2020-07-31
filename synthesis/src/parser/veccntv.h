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

#ifndef _VECCNTV_H_
#define _VECCNTV_H_
#include <list>
#include <string>
#include <map>
#include <stdlib.h>
#include "utils.h"
#include "scalarcntv.h"
#include "dimension.h"
#include "defines.h"
#include "range.h"
#include "realtime.h"
#include "real.h"
#include "reg.h"

namespace Rtl
{
    class VecCntv: public Cntv
    {
        friend class Elaborator;
        friend class GenUnroll;
        public:
            VecCntv(const std::string& n,
                const Dimension* d): name(n), dim(
                const_cast<Dimension*>(d)), initval(0){
            }
            virtual ~VecCntv(){
            }

            virtual const ScalarCntv* getBit(const long bit) const{
                std::map<long, ScalarCntv*>::const_iterator iter =
                bitMap.find(bit);
                if(iter == bitMap.end()){
                    return NULL;
                }
                return iter->second;
            }
            const Dimension* getDim() const{
                return dim;
            }
            template<class T> std::vector<T*>
            flatten(const std::string&, const Dimension*, bool nomultidim=true);
            const std::string& getName() const{
                return name;
            }
            virtual std::string toString() const{
                return name;
            }
            const Expr* getVal() const {
                return initval;
            }

            virtual VecCntv* copy() const = 0;
            virtual const Expr* repExpr(const std::string& str,
                                        const Expr *expr)
            {
                if(this->toString() == str) {
                    return expr;
                }
                return this;
            }

            inline long getMsb(bool nomultidim=true) const;
            inline long getLsb(bool nomultidim=true) const;
        protected:
            void addScalarBit(const ScalarCntv* sc){
                bitMap.insert(std::make_pair(sc->getIxName().getIndex(),
                const_cast<ScalarCntv*>(sc)));
            }
            void setName(const std::string& n){
                name = n;
            }
            virtual void setVal(const Expr* init) {
                initval = const_cast<Expr*>(init);
            }
        private:
            std::string name;
            Dimension* dim;
            std::map<long, ScalarCntv*> bitMap;
            Expr* initval;
    };

    long 
    VecCntv::getMsb(bool nomultidim) const
    {
        long packSize = dim->packDim.size();
        long unpackSize = dim->unpackDim.size();
        if(nomultidim)
        {
            long msb = 0;
            if(packSize > 0) {
                msb = dim->packDim[packSize-1]->getLExpr()->evaluate();
            }
            else {
                msb = dim->unpackDim[unpackSize-1]->getLExpr()->evaluate();
            }
            return msb;
        }    
        return bitMap.size()-1;
    }
    long 
    VecCntv::getLsb(bool nomultidim) const
    {
        long packSize = dim->packDim.size();
        long unpackSize = dim->unpackDim.size();
        if(nomultidim)
        {
            
            long lsb = 0;
            if(packSize > 0) {
                lsb = dim->packDim[packSize-1]->getRExpr()->evaluate();
            }
            else {
                lsb = dim->unpackDim[unpackSize-1]->getRExpr()->evaluate();
            }
            return lsb;
        }
        return 0;
    }

    template<class T> std::vector<T*>
    VecCntv::flatten(const std::string& name, const Dimension* dim, bool nomultidim)
    {
        long size = 1;
        long packSize = dim->packDim.size();
        long unpackSize = dim->unpackDim.size();

        if(nomultidim)
        {
            long msb = this->getMsb(nomultidim);
            long lsb = this->getLsb(nomultidim);
            std::vector<T*> fv;

            for(int i = msb; (msb > lsb) ? (i >= lsb) : (i <= lsb);
                (msb > lsb) ? --i : ++i)
            {    
                T* t = new T(IxName(name, i));
                t->setScope(this->getScope());
                this->addScalarBit(t);
                fv.push_back(t);
            }    
            return fv;
        }    
        else
        {

            for(int i = 0; i < packSize; ++i)
            {
                const int width = abs(dim->packDim[i]->getLExpr()->
                    evaluate() - dim->packDim[i]->getRExpr()->evaluate()) + 1;
                size *= width;
            }
            for(int i = 0; i < unpackSize; ++i)
            {
                const int width = abs(dim->unpackDim[i]->getLExpr()->
                    evaluate() - dim->unpackDim[i]->getRExpr()->evaluate()) + 1;
                size *= width;
            }
        
            std::vector<T*> fv;
            for(long bitcount = size-1; bitcount >= 0; --bitcount)
            {
                T* t = new T(IxName(name, bitcount));
                t->setScope(this->getScope());
                this->addScalarBit(t);
                fv.push_back(t);
            }
            return fv;
        }    
            
    }

}
#endif
