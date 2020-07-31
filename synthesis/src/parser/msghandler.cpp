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


#include "msghandler.h"

namespace Rtl
{
    MsgHandler::MsgHandler() {
        buildMsgTable();
    }

    MsgHandler::~MsgHandler() {
    }

    void
    MsgHandler::buildMsgTable()
    {
        addMsgInfo( new MsgInfo("[PARSE_INFO 0]: Starting analysis of design .....\n", 0, MsgInfo::IMM, MsgInfo::INFO) );
        addMsgInfo( new MsgInfo("[PARSE_INFO 1]: Reading %s file %s .....\n", 1, MsgInfo::IMM, MsgInfo::INFO) ); //design, include or library
        addMsgInfo( new MsgInfo("[PARSE_INFO 2]: Analyzing %s %s .....\n", 2, MsgInfo::IMM, MsgInfo::INFO) ); //module/udp 
        addMsgInfo( new MsgInfo("[PARSE_INFO 3]: Done reading %s file %s. \n", 3, MsgInfo::IMM, MsgInfo::INFO) ); 
        addMsgInfo( new MsgInfo("[PARSE_INFO 4]: Starting elaboration of design .....\n", 4, MsgInfo::IMM, MsgInfo::INFO ));
        addMsgInfo( new MsgInfo("[PARSE_INFO 5]: Eaboration done.\n", 5, MsgInfo::IMM, MsgInfo::INFO) );
        addMsgInfo( new MsgInfo("[PARSE_INFO 6]: Successfully parsed the design.\n", 6, MsgInfo::IMM, MsgInfo::INFO) );
        addMsgInfo( new MsgInfo("[PARSE_INFO 7]: Please correct above errors.\n", 7, MsgInfo::IMM, MsgInfo::INFO) );
        addMsgInfo( new MsgInfo("[STX_ERROR 8]: Line %d, identifier %s used for %s name is already in use.\n", 8, MsgInfo::IMM, MsgInfo::ERROR) );

        addMsgInfo( new MsgInfo("[PARSE_ERROR 10]: Could not open %s file %s for reading.\n", 10, MsgInfo::IMM, MsgInfo::ERROR) );
        addMsgInfo( new MsgInfo("[PARSE_ERROR 11]: Could not parse design, no files specified.\n", 11, MsgInfo::IMM, MsgInfo::ERROR) );
        addMsgInfo( new MsgInfo("[PARSE_ERROR 12]: Include directory name not specified.\n", 12, MsgInfo::IMM, MsgInfo::ERROR) );
        addMsgInfo( new MsgInfo("[PARSE_ERROR 13]: Decompile file name not specified.\n", 13, MsgInfo::IMM, MsgInfo::ERROR) );
        addMsgInfo( new MsgInfo("[PARSE_ERROR 14]: Could not open decompile file %s for writing.\n", 14, MsgInfo::IMM, MsgInfo::ERROR) );
        addMsgInfo( new MsgInfo("[PARSE_ERROR 15]: Log file name not specified.\n", 15, MsgInfo::IMM, MsgInfo::ERROR) );
        addMsgInfo( new MsgInfo("[PARSE_ERROR 16]: Could not open log file %s for writing.\n", 16, MsgInfo::IMM, MsgInfo::ERROR) );
        addMsgInfo( new MsgInfo("[PARSE_ERROR 17]: Library file name not specified.\n", 17, MsgInfo::IMM, MsgInfo::ERROR) );
        addMsgInfo( new MsgInfo("[PARSE_ERROR 18]: Could not open library file %s for writing.\n", 18, MsgInfo::IMM, MsgInfo::ERROR) );
        addMsgInfo( new MsgInfo("[STX_ERROR 21]: Line %d, variable %s already declared.\n", 21, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 22]: Line %d, Repeated parameter name %s in instantiation.\n", 22, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 23]: Line %d, Too many parameter values specified, expected maximum %d, found %d.\n", 23, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 24]: Line %d, parameter name %s not found in module parameter list.\n", 24, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 25]: Line %d, port %s already declared.\n", 25, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 26]: Line %d, port %s does not exist in the declaration list.\n", 26, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 27]: Line %d, mismatch in port connection.\n", 27, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 28]: Line %d, duplicate pin name specified.\n", 28, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 29]: Line %d, %s select out of range.\n", 29, MsgInfo::IMM, MsgInfo::ERROR)); //part/bit
        addMsgInfo( new MsgInfo("[STX_ERROR 30]: Line %d, illegal %s select for scalar variable.\n", 30, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 31]: Line %d, part select is wrong way around.\n", 31, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 32]: Line %d, illegal dimension select.\n", 32, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 33]: Line %d, illegal non-const case label %s.\n", 33, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 34]: Line %d, condition expression can not be non-const in this context.\n", 34, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 35]: Line %d, generate case select can not be non-const.\n", 35, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 36]: Line %d, generate case label can not be non-const.\n", 36, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 37]: Line %d, illegal variable %s on the left hand side of continuous assign statement, only net type is allowed.\n", 37, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 38]: Line %d, illegal variable %s on the left hand side of procedural assign statement, only register type is allowed.\n", 38, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 39]: Line %d, genvar %s used in wrong context.\n", 39, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 40]: Line %d, genvar %s used without being initialized.\n", 40, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 41]: Line %d, identifier %s not declared in this scope.\n", 41, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 42]: Line %d, variable %s not allowed inside generate-for header, only genvars and integers are allowed.\n", 42, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 43]: Line %d, in module %s unable to resolve defparam(s).\n", 43, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 44]: Line %d, recursive instantiation of module %s found.\n", 44, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 45]: Line %d, variable %s does not exist.\n", 45, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 46]: Line %d, variable %s used as %s name is already declared.\n", 46, MsgInfo::IMM, MsgInfo::ERROR)); //task, func etc
        addMsgInfo( new MsgInfo("[STX_ERROR 47]: Line %d, non-const expression in parameter use.\n", 47, MsgInfo::IMM, MsgInfo::ERROR)); //task, func etc
        addMsgInfo( new MsgInfo("[STX_ERROR 48]: Line %d, %s port can not be of reg type.\n", 48, MsgInfo::IMM, MsgInfo::ERROR)); //task, func etc
        addMsgInfo( new MsgInfo("[STX_ERROR 50]: Line %d, syntax error.\n", 50, MsgInfo::IMM, MsgInfo::ERROR)); //task, func etc
        addMsgInfo( new MsgInfo("[STX_ERROR 51]: Line %d, non-const expression specified in range.\n", 51, MsgInfo::IMM, MsgInfo::ERROR)); //task, func etc
        addMsgInfo( new MsgInfo("[PARSE_ERROR 52]: Line %d, %s not supported.\n", 52, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 53]: Line %d, non-const value used in parameter assign.\n", 53, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[STX_ERROR 54]: Line %d, non-const value used in defparam assign.\n", 54, MsgInfo::IMM, MsgInfo::ERROR));
        addMsgInfo( new MsgInfo("[PARSE_WARN 70]: No top level module found in the design.\n", 70, MsgInfo::IMM, MsgInfo::WARN));
        addMsgInfo( new MsgInfo("[PARSE_WARN 71]: Line %d, no connection to pin %s.\n", 71, MsgInfo::IMM, MsgInfo::WARN));
        addMsgInfo( new MsgInfo("[PARSE_WARN 72]: Line %d, size mismatch between formal and actual for pin %s, expected %d found %d.\n", 72, MsgInfo::IMM, MsgInfo::WARN));
        addMsgInfo( new MsgInfo("[PARSE_WARN 73]: Line %d, %s is not defined.\n", 73, MsgInfo::IMM, MsgInfo::WARN));
        
    }
}

