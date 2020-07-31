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


#ifndef _NETLISTWRITER_H_
#define _NETLISTWRITER_H_

#include <string>
#include <vector>
#include <list>

#include <fstream>
#include "netlistvisitorbase.h"
namespace stview
{
    class SInstBase;

    class NetlistWriter: public NetlistVisitorBase
    {
        public:
            NetlistWriter(const std::string&);
            virtual ~NetlistWriter();

            void    write();

            virtual void visit(SBlock*);
            virtual void visit(SBlockInst*);
            virtual void visit(SCell*);
            virtual void visit(SInst*);
        private:
            NetlistWriter();
            NetlistWriter(const NetlistWriter&);
            NetlistWriter& operator=(const NetlistWriter&);
            //functions
            void printInd();
            void printLine();
            void print(const std::string&);
            void incIndent(short=1);
            void decIndent(short=1);


            template<class T>    std::vector<std::string>
            getDeclStrings(const std::list<T*>&, bool) const;

            std::string    getDirectionString(const SPort*) const;    
            void    printPins(const SInstBase*);   

            //data
            short col;
            short indwidth;
            short linewidth;
            short indent;
            std::ofstream *outfile;
    };

}
#endif
