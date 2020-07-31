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

#ifndef _SCNTV_H_
#define _SCNTV_H_

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

#include "sbase.h"

namespace stview
{
    struct CntvInfo {
        CntvInfo(std::string& n, int m, int l) : 
            name(n),
            msb(m),
            lsb(l) {
        }    
        std::string    name;
        int            msb; //-1 if scalar
        int            lsb;  //-1 if scalar
    };    

    typedef std::vector<CntvInfo> CntvInfoVector;
    class SCntv: public SBase
    {
        friend class SInstBase;
        friend struct DELETE<SCntv>;
        public:
            SCntv(const std::string& name) : 
                SBase(name), visitCount(0) 
                {
            }
            bool isVecBit() const {
                return this->getName().find_first_of('#') != 
                std::string:: npos;
            }    
            std::string getVecName() const 
            {
                std::string name = this->getName();
                size_t pos = name.find_first_of('#');
                if(pos == std::string::npos) {
                    return name;
                }
                return name.substr(0, pos);
            }    
            inline SIxName    getIxName() const;
            //Traversal helper functions 
            inline bool    isVisited() const;
            inline int    getVisitCount() const;
            inline void    incVisitCount();
            inline void    decVisitCount();
            inline void    resetVisitCount();
        protected:    
            virtual ~SCntv() {
            }    
        private:
            SCntv(const SCntv&);
            SCntv& operator=(const SCntv&);

            int visitCount;
            
    };

    SIxName
    SCntv::getIxName() const
    {
        const std::string& name = this->getName();
        int pos = name.find_first_of('#');
        if(pos == std::string::npos) {
            return SIxName(name, SIxName::nix);
        }    
        std::string namePart = name.substr(0, pos);
        long index = atol(name.substr(pos+1).c_str());
        return SIxName(namePart, index);
    }

    bool
    SCntv::isVisited() const {
        return visitCount != 0;
    }    

    int
    SCntv::getVisitCount() const {
        return visitCount;
    }

    void
    SCntv::incVisitCount() {
        ++visitCount;
    }

    void
    SCntv::decVisitCount() {
        if(visitCount > 0 ) {
            --visitCount;
        }
    }    

    void
    SCntv::resetVisitCount() {
        visitCount = 0;
    }    

    template<class T> CntvInfoVector*
    GetCntvInfo(const std::list<T*>& cntvs)
    {
        CntvInfoVector *vec = new CntvInfoVector();
        if(cntvs.empty()) {
            return vec;
        }    
        int big = -1;
        int small = -1;
        std::string name;
        T *last= 0;
        for(typename std::list<T*>::const_iterator iter = cntvs.begin();
            iter != cntvs.end(); ++iter) 
        {
            T* cntv = *iter;
            

             if(cntv->isVecBit())
             { 
                 if(name.empty()) 
                {
                    SIxName ixname = cntv->getIxName();
                    name = ixname.getName();
                    big = ixname.getIndex();
                    small = big;
                }     
                else 
                {
                    SIxName ixname = cntv->getIxName();
                    if(name != ixname.getName())
                    {
                        vec->push_back(CntvInfo(name, big, small));

                         //new port has arrived
                        name = ixname.getName();
                        big = ixname.getIndex();
                        small = big;
                     } 
                     else 
                     {
                        int index = ixname.getIndex();
                        if(index > big) {
                            big = index;
                        }
                        else if(index < small) {
                             small = index;
                        }     

                     }
                 }     
             }
             else
             {
                 if(!name.empty()) {
                    vec->push_back(CntvInfo(name, big, small));
                 }     
                 name = cntv->getName();
                 vec->push_back(CntvInfo(name, -1, -1));
                 big = -1;
                 small = -1;
                 name.clear();
             }
             last = cntv;
        }
        //the last remaining vector
        if(!name.empty()) {
            vec->push_back(CntvInfo(name, big, small));
        }    
        return vec;
    }
}
#endif
