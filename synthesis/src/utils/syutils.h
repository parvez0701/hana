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

#ifndef _SYUTILS_H_
#define _SYUTILS_H_
#include <string>
#include <string.h>
#include <functional>

#include "parseinclude.h"

#include "sycntvs.h"

#define DISABLE_COPYCONST_ASSGNOP(ClassName) \
    ClassName(const ClassName&);  \
    ClassName& operator=(const ClassName&);


namespace Synth
{
    struct cmp_str{
        bool operator()(const std::string& s1, const std::string& s2) const{
            return strcmp(s1.c_str(), s2.c_str()) < 0;
        }
    };
    struct cmp_cntvs {
        bool operator()(
                const SyCntv* c1,
                const SyCntv* c2) const
        {
            cmp_str cmp;
            return cmp(c1->toString(), c2->toString());
        }
    };

    struct cmp_exprs {
        bool operator()(
                const SyExpr* e1,
                const SyExpr* e2) const
        {
            cmp_str cmp;
            return cmp(e1->toString(), e2->toString());
        }
    };

    struct equal_cntvs {
        bool operator()(
                    const SyCntv* c1,
                    const SyCntv* c2
                    ) const
        {
            return c1->toString() == c2->toString();
        }    
    };
    struct cntv_hash_function {
        long operator()(const SyCntv *cntv) const
        {
            long a = (long)cntv;
            a = (a+0x7ed55d16) + (a<<12);
            a = (a^0xc761c23c) ^ (a>>19);
            a = (a+0x165667b1) + (a<<5);
            a = (a+0xd3a2646c) ^ (a<<9);
            a = (a+0xfd7046c5) + (a<<3);
            a = (a^0xb55a4f09) ^ (a>>16);
            return a;
        }
    };    

                    
    template <class T>
    struct Delete : public std::unary_function<T, bool>
    {
        void operator ()(T* t) const {
            delete t;
        }
    };    

    SyScPort::SyDirType Rtl2SynthPortDir(const Rtl::Dir);
    SyScNet::SyNetType Rtl2SynthNetType(const Rtl::NetType);
    SyScNet::SyNetType String2SynthNetType(const std::string&);
    string DirTypeToString(const SyScPort::SyDirType);
    string NetTypeToString(const SyScNet::SyNetType);
    SyExpr::SyOp Rtl2SynthOp(const Rtl::OpType);
    string CreateUniqueName(
            const SyModule *mod,
            const string& hint);

    template <class FROM, class TO>
    SyList<TO> ConvertList(const SyList<FROM>& fromList)
    {
        SyList<TO> toList;

        for(SyNode<FROM>* node = fromList.head; node; node = node->next){
            toList.push_back(static_cast<TO>(node->val));
        }
        return toList;
    }    
    template <class T>
    void CreateFlatList(SyList<T>& flist, const SyExpr *expr)
    {
        if(const SyVCntv *cntv = dynamic_cast<const SyVCntv*>(expr)) {
            const SyList<SyScCntv*>& cntvs = cntv->getBits();
            for(SyNode<SyScCntv*>* node = cntvs.head; node; node = node->next){
                flist.push_back(node->val);
            }    
        }
        else if(const SyConcat *conc = dynamic_cast<const SyConcat*>(expr))
        {
            const SyList<SyExpr*>& bits = conc->getExprs();
            for(SyNode<SyExpr*>* node = bits.head; node; node = node->next){
                if(SyCntv *cntv = dynamic_cast<SyCntv*>(node->val)){
                    flist.push_back(cntv);
                }
            }
        }    
        else if(const SyCntv *cntv = dynamic_cast<const SyCntv*>(expr)){
            flist.push_back(cntv);
        }
    }

    void FlattenLValue(const SyExpr*, SyList<SyCntv*>&);

    int    BitCount(long);
    std::string Long2String(unsigned long, int size, char='0');
    bool IsPowerOfTwo(long);

    int Log2Ceiling(int);
    long RJHashFunction(void *);
    SyList<std::pair<SyVCntv*,SyList<SyScCntv*> > > GetVectorScalarPairs(
        const SyList<SyScCntv*>&, const std::string&, const std::string& ="");

    bool IsConstExpr(const SyExpr*);    
    
}
#endif
