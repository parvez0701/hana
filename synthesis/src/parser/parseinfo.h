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

#ifndef _PARSEINFO_H_
#define _PARSEINFO_H_
#include <vector>
#include <string>
#include "range.h"
#include "defines.h"
#include "file.h"
#include "compdirctvmgr.h"
#include "tickdefine.h"

using namespace Rtl;
class ParseInfo{
    public:
        ParseInfo(const std::string& n, Dir d, NetType t):
            name(n), dir(d), net(t), isSigned(false), 
            range(0), dim(0), table(0){
        }
        ParseInfo() : dir(UNSET), net(NONET), isSigned(false), range(0),  
             dim(0), table(0){
        }
        ~ParseInfo(){
        }
        std::string name;
        Dir dir;
        NetType net;
        bool isSigned;
        Range* range;
        std::vector<Range*>* dim;
        AttribTable* table;
        std::vector<Expr*> exprs;
        std::vector<char*> ids;
};

class RegInfo {
    public:
        RegInfo(char* n, Expr* e, std::vector<Range*>* d):
            name(n), expr(e), dim(d){
        }
        RegInfo(char* n, Expr* e): name(n), expr(e), dim(0) {
        }
        RegInfo(char* n): name(n), expr(0), dim(0) {
        }
        char* name;
        Expr* expr;
        std::vector<Range*>* dim;
};

class ParseState{
    public:
        int line;
        FILE* yyin;
        File* file;
};

#endif
