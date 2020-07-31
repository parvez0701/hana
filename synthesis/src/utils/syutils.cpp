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

#include "syutils.h"
#include "syutils2.h"
#include "sycntvs.h"
#include "syconst.h"
#include "sycontainer.h"

using Rtl::Dir;
using Rtl::NetType;
using std::ostringstream;
using std::pair;
using std::make_pair;
namespace Synth
{
    SyScPort::SyDirType
        Rtl2SynthPortDir(const Dir dir)
        {
            switch (dir)
            {
                case Rtl::IN:
                    return SyScPort::IN;
                case Rtl::OUT:
                    return SyScPort::OUT;
                case Rtl::INOUT:
                    return SyScPort::INOUT;
                case Rtl::UNSET:
                    return SyScPort::NODIR;
                default:
                    return SyScPort::NODIR;
            }
        }

    SyScNet::SyNetType
        Rtl2SynthNetType(const NetType type)
        {
            switch (type)
            {
                case Rtl::WIRE:
                    return SyScNet::WIRE;
                case Rtl::WOR:
                    return SyScNet::WOR;
                case Rtl::WAND:
                    return SyScNet::WAND;
                case Rtl::TRI:
                    return SyScNet::TRI;
                case Rtl::TRIAND:
                    return SyScNet::TRIAND;
                case Rtl::TRIOR:
                    return SyScNet::TRIOR;
                case Rtl::TRI1:
                    return SyScNet::TRI1;
                case Rtl::TRI0:
                    return SyScNet::TRI0;
                case Rtl::SUPPLY1:
                    return SyScNet::SUPPLY1;
                case Rtl::SUPPLY0:
                    return SyScNet::SUPPLY0;
                case Rtl::REG:
                    return SyScNet::REG;
                case Rtl::TRIREG:
                    return SyScNet::TRIREG;
                default:
                    return SyScNet::NONET;
            }
        }

    SyScNet::SyNetType
        String2SynthNetType(const string& name)
        {
            if(name == "wire") return SyScNet::WIRE;
            if(name == "wor") return SyScNet::WOR;
            if(name == "wand") return SyScNet::WAND;
            if(name == "tri") return SyScNet::TRI;
            if(name == "triand") return SyScNet::TRIAND;
            if(name == "trior") return SyScNet::TRIOR;
            if(name == "tri1") return SyScNet::TRI1;
            if(name == "tri0") return SyScNet::TRI0;
            if(name == "supply0") return SyScNet::SUPPLY0;
            if(name == "supply1") return SyScNet::SUPPLY1;
            if(name == "reg") return SyScNet::REG;
            if(name == "trireg") return SyScNet::TRIREG;

            return SyScNet::NONET;
        }

    string DirTypeToString(SyScPort::SyDirType dir)
    {
        switch(dir)
        {
            case SyScPort::IN: return "input";
            case SyScPort::OUT: return "output";
            case SyScPort::INOUT: return "inout";
            default: return "";
        }
    }

    string NetTypeToString(SyScNet::SyNetType type)
    {
        switch(type)
        {
            case SyScNet::WIRE: return "wire";
            case SyScNet::WAND: return "wand";
            case SyScNet::WOR: return "wor";
            case SyScNet::TRI: return "tri";
            case SyScNet::TRIAND: return "triand";
            case SyScNet::TRIOR: return "trior";
            case SyScNet::TRI1: return "tri1";
            case SyScNet::TRI0: return "tri0";
            case SyScNet::SUPPLY0:return "supply0";
            case SyScNet::SUPPLY1: return "supply1";
            case SyScNet::REG: return "reg";
            case SyScNet::TRIREG: return "trireg";
            default: return "";
        }
    }

    SyExpr::SyOp Rtl2SynthOp(const Rtl::OpType op)
    {
        switch (op)
        {
            case Rtl::UNPLUS: return SyExpr::UNPLUS;
            case Rtl::UNMINUS: return SyExpr::UNMINUS;
            case Rtl::LOGNEG: return SyExpr::LOGNEG;
            case Rtl::LOGAND: return SyExpr::LOGAND;
            case Rtl::LOGOR: return SyExpr::LOGOR;
            case Rtl::GTHAN: return SyExpr::GTHAN;
            case Rtl::GTHANEQ: return SyExpr::GTHANEQ;
            case Rtl::LTHAN: return SyExpr::LTHAN;
            case Rtl::LTHANEQ: return SyExpr::LTHANEQ;
            case Rtl::LOGEQ: return SyExpr::LOGEQ;
            case Rtl::LOGENEQ: return SyExpr::LOGENEQ;
            case Rtl::CASEEQ: return SyExpr::CASEEQ;
            case Rtl::CASEENEQ: return SyExpr::CASEENEQ;
            case Rtl::BITNEG: return SyExpr::BITNEG;
            case Rtl::BITAND: return SyExpr::BITAND;
            case Rtl::BITNAND: return SyExpr::BITNAND;
            case Rtl::BITOR: return SyExpr::BITOR;
            case Rtl::BITNOR: return SyExpr::BITNOR;
            case Rtl::BITXOR: return SyExpr::BITXOR;
            case Rtl::BITNXOR: return SyExpr::BITNXOR;
            case Rtl::BITEQUIV: return SyExpr::BITEQUIV;
            case Rtl::REDAND: return SyExpr::REDAND;
            case Rtl::REDNAND: return SyExpr::REDNAND;
            case Rtl::REDNOR: return SyExpr::REDNOR;
            case Rtl::REDOR: return SyExpr::REDOR;
            case Rtl::REDXOR: return SyExpr::REDXOR;
            case Rtl::REDXNOR: return SyExpr::REDXNOR;
            case Rtl::LSHIFT: return SyExpr::LSHIFT;
            case Rtl::RSHIFT: return SyExpr::RSHIFT;
            case Rtl::RRSHIFT: return SyExpr::RRSHIFT;
            case Rtl::PLUS: return SyExpr::PLUS;
            case Rtl::MINUS: return SyExpr::MINUS;
            case Rtl::MULT: return SyExpr::MULT;
            case Rtl::DIV: return SyExpr::DIV;
            case Rtl::MOD: return SyExpr::MOD;
            case Rtl::COND: return SyExpr::COND;
            case Rtl::POW: return SyExpr::POW;
            default: return SyExpr::NOOP;
        }
    }

     string
        CreateUniqueName(
                const SyModule *mod,
                const string& hint)
        {
            int i = 0;
            string name = hint;

            while(1)
            {
                if(!mod->findObject(name)) {
                    return name;
                }
                ostringstream str;
                str << hint << "_" << i++;
                name = str.str();
            }
            assert(0);
        }

    int
        BitCount(long num)
        {
            if(num == 0) {
                return 1;
            }    
            int i = 0;
            while(num)
            {
                num = num >> 1;
                ++i;
            }
            return i;
        }    
    string
        Long2String(unsigned long num, int size, char pad)
        {
            string str;
            if(size == 0) {
                return str;
            }    
            do 
            {
                if(num % 2) {
                 str = string("1") + str;
                }
                else {
                    str = string("0") + str;
                }
                num = num >> 1;
            }while(num);
            
            int strSize = str.size();
            for(int i = 0; i < size - strSize; ++i){
                str = ((pad == '0') ? string("0") : string("1")) + str;
            }
            return str;

        }    

        bool
        IsPowerOfTwo(long num) {
            if(num == 0) {
                return false;
            }    
            return !(num & (num -1));
        }    

        int Log2Ceiling(int num)
        {
            assert(num != 0);
            int i = 1;
            int ret = 0;

            do
            {
                i = i << 1;
                ++ret;
            }while(i < num);
            return ret;
        }    

        long RJHashFunction(void *obj)
        {
            long a = (long)obj;
            a = (a+0x7ed55d16) + (a<<12);
            a = (a^0xc761c23c) ^ (a>>19);
            a = (a+0x165667b1) + (a<<5);
            a = (a+0xd3a2646c) ^ (a<<9);
            a = (a+0xfd7046c5) + (a<<3);
            a = (a^0xb55a4f09) ^ (a>>16);
            return a;
        }


        SyList<pair<SyVCntv*, SyList<SyScCntv*> > > 
            GetVectorScalarPairs(
            const SyList<SyScCntv*>& cntvs, const string& mod,
            const string& tf)
        {
            SyList<pair<SyVCntv*, SyList<SyScCntv*> > > ret;
            SyNode<SyScCntv*>* node = cntvs.head;
            while(node)
            {
                SyScCntv *cntv = node->val;
                SyList<SyScCntv*> scntvs;
                if(cntv->isVecBit())
                {
                    string name = cntv->getIxName().getName();
                    //SyNode *tmp = node;
                    //const SyVCntv *vcntv = node->val->getVecCntv(mod);
                    const SyVCntv *vcntv = GetVecCntv<SyCntv, SyVCntv>(
                        name, mod, tf);
                    assert(vcntv);

                    while(node && (node->val->getIxName().getName() == name))
                    {
                        scntvs.push_back(node->val);
                        node = node->next;
                    }
                    ret.push_back(make_pair(const_cast<SyVCntv*>(vcntv), scntvs));
                }    
                else 
                {
                    scntvs.push_back(cntv);
                    ret.push_back(make_pair((SyVCntv*)0, scntvs));
                    node = node->next;
                }    
                        
            }
            return ret;
        }

    bool IsConstExpr(const SyExpr *expr)
    {
        if(dynamic_cast<const SyConst*>(expr)) {
            return true;
        }

        const SyConcat *conc = dynamic_cast<const SyConcat*>(expr);
        if(!conc) {
            return false;
        }
        const SyList<SyExpr*>& exprs = conc->getExprs();
        for(SyNode<SyExpr*>* node = exprs.head; node; node = node->next) {
            if(!IsConstExpr(node->val)) {
                return false;
            }
        }
        return true;
    }

    void FlattenLValue(const SyExpr *expr, SyList<SyCntv*>& flist)
    {
        if(const SyVCntv *vcntv = dynamic_cast<const SyVCntv*>(expr))
        {
            const SyList<SyScCntv*>& bits = vcntv->getBits();
            for(SyNode<SyScCntv*>* n = bits.head; n; n = n->next) {
                flist.push_back(n->val);
            }    
        }
        else if(const SyConcat *conc = dynamic_cast<const SyConcat*>(expr))
        {
            const SyList<SyExpr*>& exprs = conc->getExprs();
            for(SyNode<SyExpr*>* n = exprs.head; n; n = n->next) {
                FlattenLValue(n->val, flist);
            }    
        }
        else if(const SyPartSel* psel = dynamic_cast<const SyPartSel*>(expr))
        {
            SyConcat *conc = psel->getConcat();
            FlattenLValue(conc, flist);
            delete conc;
        }
        else if(const SyCntv *cntv = dynamic_cast<const SyCntv*>(expr))
        {
            flist.push_back(const_cast<SyCntv*>(cntv));
        }
        else 
        {
            assert(0);
        }    
    
    }

}
