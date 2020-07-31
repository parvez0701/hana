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

#include "ifdefproc.h"

#include "tickdefine.h"

using std::string;
using std::make_pair;
extern void yyerror(const char*);
namespace Rtl
{
    IfdefProc* IfdefProc::ifdefProc = 0;

    IfdefProc::IfdefProc() {
    }

    IfdefProc::~IfdefProc() {
    }

    IfdefProc*
    IfdefProc::instance()
    {
        if(!IfdefProc::ifdefProc) {
            IfdefProc::ifdefProc = new IfdefProc();
        }
        return IfdefProc::ifdefProc;
    }

    int
    IfdefProc::processIfdef(const string & name) {
        return processDef(name, true);
    }

    int
    IfdefProc::processIfndef(const string& name) {
        return processDef(name, false);
    }

    int
    IfdefProc::processDef(const string& name, bool isIfdef)
    {
        if(!infoStack.empty() && 
            (infoStack.top().second == IGNORED)) 
        {
            //already inside ifdef/ifndef processing
            //push this one to stack and continue ignoring
            infoStack.push(make_pair(isIfdef ? IFDEF : IFNDEF, IGNORED));
            return 1; //ignore
        }
        else 
        {
            if(!TickDefine::instance()->isDefined(name)){
                infoStack.push(make_pair(isIfdef ? IFDEF : IFNDEF, 
                    isIfdef ? IGNORED : PROCESSED));
            }
            else {
                infoStack.push(make_pair(isIfdef ? IFDEF : IFNDEF, 
                    isIfdef ? PROCESSED : IGNORED));
            }    
        }

        if(!TickDefine::instance()->isDefined(name) && !isIfdef) {
            return 0;
        }

        if(TickDefine::instance()->isDefined(name) && isIfdef) {
            return 0;
        }

        return 1;
    }

    int
    IfdefProc::processElsif(const string& name) 
    {
        if(infoStack.empty()) 
        {
            yyerror("astray `elsif found.");
            return 1;
        }    

        if(infoStack.top().second == PROCESSED) {
            return 1;
        }
        infoStack.pop();
        infoStack.push(make_pair(ELSIF, PROCESSED));
        return 0;
    }    

    int
    IfdefProc::processElse() const
    {
        if(infoStack.empty()) 
        {
            yyerror("astray `else found.");
            return 1;
        }    

        if(infoStack.top().second == PROCESSED) {
            return 1;
        }
        return 0;
    }

    int
    IfdefProc::processEndif()
    {
        if(infoStack.empty()) 
        {
            yyerror("astray `endif found.");
            return 1;
        }    

        infoStack.pop();
        return 0;
    }

    int
    IfdefProc::doChecksAtEOF() 
    {
        if(infoStack.empty()) {
            return 0;
        }
        string msg = "End of file reached, unmatched ";
        msg += (infoStack.top().first == IFDEF) ? "`ifdef" :
            (infoStack.top().first == IFNDEF) ? "`ifndef" : "`elsif";
        
        msg += " found";
        yyerror(msg.c_str());
        
        while(!infoStack.empty()) {
            infoStack.pop();
        }
        return 1;
    }
}
