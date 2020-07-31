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

#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <map>
#include <string>
#include <string.h>

#include "path.h"
namespace Rtl
{
    class Expr;
    typedef enum 
    {
        UNSET,
        IN,
        OUT,
        INOUT
    } Dir;

    typedef enum
    {
        NONET,
        WIRE,
        WOR,
        WAND,
        TRI,
        TRIAND,
        TRIOR,
        TRI1,
        TRI0,
        SUPPLY1,
        SUPPLY0,
        REG,
        TRIREG
    } NetType;

    typedef enum
    {
        NOOP,
        UNPLUS,
        UNMINUS,
        LOGNEG,
        LOGAND,
        LOGOR,
        GTHAN,
        GTHANEQ,
        LTHAN,
        LTHANEQ,
        LOGEQ,
        LOGENEQ,
        CASEEQ,
        CASEENEQ,
        BITNEG,
        BITAND,
        BITNAND,
        BITOR,
        BITNOR,
        BITXOR,
        BITNXOR,
        BITEQUIV,
        REDAND,
        REDNAND,
        REDNOR,
        REDOR,
        REDXOR,
        REDXNOR,
        LSHIFT,
        RSHIFT,
        RRSHIFT,
        PLUS,
        MINUS,
        MULT,
        DIV,
        MOD,
        COND,
        POW
    } OpType;
    
    typedef enum {
        UNSETGATE = 0,
        AND,
        NAND,
        OR,
        NOR,
        XOR,
        XNOR,
        BUF,
        BUFIF0,
        BUFIF1,
        NOT,
        NOTIF0,
        NOTIF1,
        NMOS,
        RNMOS,
        PMOS,
        RPMOS,
        CMOS,
        RCMOS,
        TRAN,
        RTRAN,
        TRANIF0,
        TRANIF1,
           RTRANIF0,
        RTRANIF1,
        PULLUP,
        PULLDOWN
    } GateType;

    typedef enum{
        DUNSET,
        DSUPPLY0,
        DSTRONG0,
        DPULL0,
        DWEAK0,
        DSUPPLY1,
        DSTRONG1,
        DPULL1,
        DWEAK1,
        DHIGHZ0,
        DHIGHZ1
    }DrStr;

    typedef enum {
        CASE,
        CASEX,
        CASEZ
    } CaseType;

    typedef enum {
        NOEDGE,
        POSEDGE,
        NEGEDGE
    }EdgeType;

    typedef enum {
        BLOCK,
        NONBLOCK
    }AssignType;

    typedef enum{
        NOCONN,
        PCONN,
        FCONN
    }SpecConnType;

    typedef enum{
        NOPOOP,
        POPOS,
        PONEG,
        POCOL
    }PolOp;

    struct compare_str{
        bool operator()(const std::string& s1, const std::string& s2) const{
           return strcmp(s1.c_str(), s2.c_str()) < 0;
        }
    };

    struct compare_path{
        bool operator()(const Path& p1, const Path& p2) const{
            compare_str cmp;
           return cmp(p1.toString(), p2.toString());
        }
    };

    typedef std::pair<std::string, Expr*> NameConn;
    typedef std::pair<DrStr, DrStr> DriveSt;
    typedef std::map<std::string, Expr*, compare_str> AttribTable;

        
}
#endif
