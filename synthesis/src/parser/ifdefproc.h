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

#ifndef _IFDEFPROC_H_
#define _IFDEFPROC_H_

#include <stack>
#include <string>

namespace Rtl
{
    class IfdefProc
    {
        public:
            typedef enum { NOPROC, IFDEF,IFNDEF, ELSIF, ELSE, ENDIF } IfdefProcType;
            typedef enum { PROCESSED, IGNORED } IfdefActType;

            typedef std::stack<std::pair<IfdefProcType, IfdefActType> >
                IfdefInfoStack;

        public:    

            static IfdefProc *instance();
            ~IfdefProc();
            

            int processIfdef(const std::string&);
            int processIfndef(const std::string&);
            int processElsif(const std::string&);
            int processElse() const;
            int processEndif();

            int doChecksAtEOF();
        private:
            IfdefProc();
            IfdefProc(const IfdefProc&);
            IfdefProc& operator=(const IfdefProc&);

            int processDef(const std::string&, bool=true);

            IfdefInfoStack infoStack;
            static IfdefProc *ifdefProc;
    };
}

#endif
