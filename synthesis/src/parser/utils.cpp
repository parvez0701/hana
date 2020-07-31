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

#include <cmath>
#include "utils.h"
#include "root.h"
#include "expr.h"
#include "vecvar.h"
#include "integer.h"
#include "vtime.h"
#include "module.h"
#include "taskfunc.h"
#include "partsel.h"
#include "multiconcat.h"
#include "unaryexpr.h"
#include "binexpr.h"
#include "condexpr.h"

namespace Rtl
{
    const unsigned int KILO = 1024;
    const unsigned int MEGA = KILO*KILO;
    const unsigned int GIGA = KILO*MEGA;

    extern Root* GRoot;

    static Expr* CheckAndReplace(BitSel*, const string&, const Expr*);
    static Expr* CheckAndReplace(Concat*, const string&, const Expr*);
    string 
    GetDirString(const Dir dir)
    {
        switch(dir)
        {
            case IN: return "input";
            case OUT: return "output";
            case INOUT: return "inout";
            default: return "";
        }
    }

    string
    GetNetTypeString(const NetType type)
    {
        switch(type)
        {
            case WIRE: return "wire";
            case WOR: return "wor";
            case WAND: return "wand";
            case TRI: return "tri";
            case TRIAND: return "triand";
            case TRIOR: return "trior";
            case TRI1: return "tri1";
            case TRI0: return "tri0";
            case SUPPLY1: return "supply1";
            case SUPPLY0: return "supply0";
            case REG: return "reg";
            default: return "wire";
        }
    }
/*
    void
    PrintMsg(const std::string& msg){
        GRoot->printLogMsg(msg);
    }

    void
    PrintMsg(const std::string& msg1, const string& msg2, 
        const string& msg3){
        GRoot->printLogMsg(msg1, msg2, msg3);
    }
    void
    PrintMsg(const std::string& msg, const int i){
        GRoot->printLogMsg(msg, i);
    }

    void
    PrintMsg(const std::string& msg1, const int i, 
        const std::string& msg2 ){
        GRoot->printLogMsg(msg1, i, msg2);
    }

    void
    PrintMsg(const std::string& msg1, const int i1, 
        const std::string& msg2, const int i2,
        const std::string& msg3, const int i3,
        const std::string& msg4 ){
        GRoot->printLogMsg(msg1, i1, msg2, i2, msg3, i3, msg4);
    }

    void
    PrintMsg(const string& s1, const int i1, const string& s2,
        const string& s3, const string& s4, const int i2, 
        const string& s5, const int i3
        )
    {
        GRoot->printLogMsg(s1, i1, s2, s3, s4, i2, s5, i3);
    }

    void
    PrintMsg(const string& s1, const int i1, const string& s2,
        const string& s3, const string& s4
        )
    {
        GRoot->printLogMsg(s1, i1, s2, s3, s4);
    }

    void
    PrintMsg(const string& s1, const int i1, const string& s2,
        const string& s3)
    {
        GRoot->printLogMsg(s1, i1, s2, s3);
    }
    */
    long
    Evaluate(const char basec, vector<char> bits)
    {
        int base;
        if(basec == 'b' || basec == 'B'){
            base = 2;
        }
        else if(basec == 'o' || basec == 'O'){
            base = 8;
        }
        else if(basec == 'd' || basec == 'D'){
            base = 10;
        }
        else{
            base = 16;
        }
        std::reverse(bits.begin(), bits.end());
        int pos = 0;
        int size = bits.size();
        long val = 0;
        for(; pos < size; ++pos) {
            if((base == 16) && isalpha(bits[pos])) {
                val = (long)pow(base, pos)*(toupper(bits[pos])-'A'+10) + val;
            }
            else {
                val = (long)pow(base, pos)*(bits[pos]-'0') + val;
            }    
        }
        return val;
    }

    string
    GetGateName(const GateType t)
    {
        switch (t)
        {
            case AND: return "and";
            case NAND: return "nand";
            case OR: return "or";
            case NOR: return "nor";
            case XNOR: return "xnor";
            case XOR: return "xor";
            case BUF: return "buf";
            case BUFIF0: return "bufif0";
            case BUFIF1: return "bufif1";
            case NOT: return "not";
            case NOTIF0: return "notif0";
            case NOTIF1: return "notif1";
            case NMOS: return "nmos";
            case RNMOS: return "rnmos";
            case PMOS: return "pmos";
            case RPMOS: return "rpmos";
            case CMOS: return "cmos";
            case RCMOS: return "rcmos";
            case TRAN: return "tran";
            case RTRAN: return "rtran";
            case TRANIF0: return "tranif0";
            case TRANIF1: return "tranif1";
            case RTRANIF1: return "rtranif1";
            case RTRANIF0: return "rtranif0";
            case PULLUP: return "pullup";
            case PULLDOWN: return "pulldown";
            default: return "";
        }
    }

    string 
    GetDriveStrName(const DrStr dstr)
    {
        switch(dstr)
        {
            case DSUPPLY0: return "supply0";
            case DSTRONG0: return "strong0";
            case DPULL0: return "pull0";
            case DWEAK0: return "weak0";
            case DSUPPLY1: return "supply1";
            case DSTRONG1: return "strong1";
            case DPULL1: return "pull1";
            case DWEAK1: return "weak1";
            case DHIGHZ0: return "highz0";
            case DHIGHZ1: return "highz1";
            default: return "";
        }
    }

    string
    GetOpString(const OpType op)
    {
        switch (op)
        {
            case UNPLUS: return "+";
            case UNMINUS: return "-";
            case LOGNEG: return "!";
            case LOGAND: return "&&";
            case LOGOR: return "||";
            case GTHAN: return ">";
            case GTHANEQ: return ">=";
            case LTHAN: return "<";
            case LTHANEQ: return "<=";
            case LOGEQ: return "==";
            case LOGENEQ: return "!=";
            case CASEEQ: return "===";
            case CASEENEQ: return "!==";
            case BITXOR: return "^";
            case BITNEG: return "~";
            case BITAND: return "&";
            case BITNAND: return "~&";
            case BITNOR: return "~|";
            case BITOR: return "|";
            case BITEQUIV: return ""; //TBD
            case REDAND:  return "&";
            case REDNAND: return "~&";
            case REDNOR: return "~|";
            case REDOR: return "|";
            case REDXOR: return "^";
            case REDXNOR: return "~^";
            case LSHIFT: return "<<";
            case RSHIFT: return ">>";
            case RRSHIFT: return ">>>";
            case PLUS: return "+";
            case MINUS: return "-";
            case MULT: return "*";
            case DIV: return "/";
            case MOD: return "%";
            case POW: return "**";
            default: return "";
        }
    }

    int
    ParseDimension(const vector<Expr*>& dim, 
        const Expr* expr, const bool ispartsel)
    {
        //Return value
        //0 --> all well.
        //1 --> Select request for a scalar variable
        //2 --> Dimension mismatch
        //3 --> Select out of range
        //4 --> Variable does not exist
        
        if(!expr){
            return 4;
        }

        //if(expr->len() == 1){
        //    return 1;
        //}
        if(dynamic_cast<const Integer*>(expr)){
            if((ispartsel&& !dim.empty()) || (!ispartsel && dim.size()> 1)){
                return 3;
            }
            return 0;
        }
        if(dynamic_cast<const Time*>(expr)){
            if((ispartsel&& !dim.empty()) || (!ispartsel && dim.size()> 1)){
                return 3;
            }
            return 0;
        }

        const VecCntv* vcntv = dynamic_cast<const VecCntv*>(expr);
        if(!vcntv){
            return 1;
        }

        const Dimension* exprdim = vcntv->getDim();

        if(dim.size() != ((exprdim->packDim.size() + 
            exprdim->unpackDim.size()) - (ispartsel ? 1:0))){
            return 2;
        }

        vector<Range*> ranges = exprdim->unpackDim;
        int size = exprdim->packDim.size();
        for(int i = 0; i < size; ++i){
            ranges.push_back(exprdim->packDim[i]);
        }

        size = dim.size();
        for(int i = 0; i < size; ++i)
        {
            int select = dim[i]->evaluate();
            if(select == INT_MIN){
                continue;
            }
            int msb = ranges[i]->getLExpr()->evaluate();
            int lsb = ranges[i]->getRExpr()->evaluate();
            if(msb == -1 || lsb == -1){
                continue;
            }
            if(msb < lsb){
                std::swap(msb, lsb);
            }

            if((select > msb) || (select < lsb)){
                return 3;
            }
        }
        return 0;
    }

    int
    ParseRange(const Range* range, const Expr* expr)
    {
        //    0--> All well    
        //    1--> Part select out of range    
        //    2 --> Select request for a scalar variable
        //    3 --> Variable does not exist
        //    4 --> Part select wrong way 
        
        if(!expr){
            return 3;
        }

        //if(expr->len() == 1){
        //    return 2;
        //}

        if(dynamic_cast<const Integer*>(expr) || dynamic_cast<const Time*>(
                    expr)) 
        {
            if(range->getLExpr()->evaluate() > 31 || range->getRExpr() < 0){
                return 1;
            }
            return 0;
        }

        const VecCntv* vcntv = dynamic_cast<const VecCntv*>(expr);
        if(!vcntv){
            return 2;
        }

        const Dimension* exprdim = vcntv->getDim();
        const Range* erange = 0;

        if(!(exprdim->packDim.empty())){
            erange = exprdim->packDim[exprdim->packDim.size()-1];
        }
        else {
            erange = exprdim->unpackDim[exprdim->unpackDim.size()-1];
        }

        int lselect = range->getLExpr()->evaluate();
        int rselect = range->getRExpr()->evaluate();
        int msb = erange->getLExpr()->evaluate();
        int lsb = erange->getRExpr()->evaluate();
        if(msb < lsb)
        {
            std::swap(msb, lsb);
            std::swap(lselect, rselect);
        }
        if( msb == -1 || lsb == -1 || lselect == -1 || rselect == -1){
            return 0;
        }
        if(lselect < rselect){
            return 4;
        }

        if((lselect > msb) || (rselect < lsb)){
            return 3;
        }
        return 0;
    }

    string
    GetPolOpString(PolOp p){
        switch (p){
            case POPOS: return "+:";
            case PONEG: return "-:";
            case POCOL: return ":";
            default: return "";
        }
    }

    const Module*
        GetModule(const Base* obj)
        {
            if(!obj){
                return 0;
            }
            if(const Module* mod = dynamic_cast<const Module*>(obj)){
                return mod;
            }
            return GetModule(obj->getScope());
        }

    const Base*
        GetObject(const string& name, const Scope *sc)
        {
            //find the object by recursively going up in the scope
            //hierarchy
            if(!sc){
                return 0;
            }
            const Base *obj = sc->getObject(name);
            return obj ? obj : GetObject(name, sc->getScope());
        }

    Path
        GetObjectPath(const Base *base)
        {
            vector<string> pathStrs;
            const Scope *sc = base->getScope();
            while(sc){
                pathStrs.push_back(sc->getName());
                sc = sc->getScope();
            }
            Path path;
            for(int i = pathStrs.size()-1; i >= 0; --i){
                path.descend(pathStrs[i]);
            }
            return path;
        }

    Expr*
        ReplaceSubExpression(Expr* orig, const string& str, const Expr *subExpr)
        {
            if(orig->toString() == str) {
                return const_cast<Expr*>(subExpr);
            }

            if(FuncCall *funcCall = dynamic_cast<FuncCall*>(orig))
            {
                vector<Expr*>& args = const_cast<vector<Expr*>&>(
                    funcCall->getArgs());
                for(unsigned int i = 0; i < args.size(); ++i){
                    args[i] = ReplaceSubExpression(args[i], str, subExpr);
                }    
            }
            else if(PartSel *partSel = dynamic_cast<PartSel*>(orig))
            {
                Range *range = const_cast<Range*>(partSel->getRange());
                range->lrExpr.first = ReplaceSubExpression(range->lrExpr.first,
                    str, subExpr);
                range->lrExpr.second = ReplaceSubExpression(range->lrExpr.second,
                    str, subExpr);
                CheckAndReplace(partSel, str, subExpr);
            }
            else if(BitSel *bitSel = dynamic_cast<BitSel*>(orig)) {
                CheckAndReplace(bitSel, str, subExpr);
            }    
            else if(MultiConcat *mcat = dynamic_cast<MultiConcat*>(orig)) 
            {
                mcat->setCount(ReplaceSubExpression(
                    const_cast<Expr*>(mcat->getCount()),
                    str, subExpr));
                CheckAndReplace(mcat, str, subExpr);
            }    
            else if(Concat *conc = dynamic_cast<Concat*>(orig)) {
                CheckAndReplace(conc, str, subExpr);
            }    
            else if(UnaryExpr *unary = dynamic_cast<UnaryExpr*>(orig)) {
                unary->setExpr(ReplaceSubExpression(
                    const_cast<Expr*>(unary->getExpr()), str, subExpr));
            }    
            else if(BinExpr *binary = dynamic_cast<BinExpr*>(orig)) 
            {
                binary->setLExpr(ReplaceSubExpression(
                    const_cast<Expr*>(binary->getLExpr()), str,
                    subExpr));
                binary->setRExpr(ReplaceSubExpression(
                    const_cast<Expr*>(binary->getRExpr()), str,
                    subExpr));
            }        
            else if(CondExpr *condExpr = dynamic_cast<CondExpr*>(orig))
            {
                condExpr->setCond(ReplaceSubExpression(
                    const_cast<Expr*>(condExpr->getCond()),
                    str, subExpr));
                condExpr->setTExpr(ReplaceSubExpression(
                    const_cast<Expr*>(condExpr->getTExpr()),
                    str, subExpr));
                condExpr->setFExpr(ReplaceSubExpression(
                    const_cast<Expr*>(condExpr->getFExpr()),
                    str, subExpr));
            }        
            return orig;
        }
    Expr*
    CheckAndReplace(BitSel *bitSel, const string& str, const Expr *subExpr)
    {
        vector<Expr*>& dim = const_cast<vector<Expr*>&>(bitSel->getDim());
        for(unsigned int i = 0; i < dim.size(); ++i) {
            dim[i] = ReplaceSubExpression(dim[i], str, subExpr);
        }
        return bitSel;
    }
    Expr*
    CheckAndReplace(Concat *conc, const string& str, const Expr *subExpr)
    {
        vector<Expr*>& exprs = const_cast<vector<Expr*>&>(conc->getExprs());
        for(unsigned int i = 0; i < exprs.size(); ++i) {
            exprs[i] = ReplaceSubExpression(exprs[i], str, subExpr);
        }
        return conc;
    }

    void
    Tokenize(const string& str, vector<string>& vec, const string& delim)
    {
        string tstr = str;
        tstr = tstr.substr(tstr.find_first_not_of(delim));
        int index;
        while((index = tstr.find_first_of(delim)) != string::npos)
        {
            vec.push_back(tstr.substr(0, index));
            tstr = tstr.substr(index+1);
            if(tstr.empty()) {
                break;
            }    
            tstr = tstr.substr(tstr.find_first_not_of(delim));
        }
        if(!tstr.empty()) {
            vec.push_back(tstr);
        }    
    }
}
