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

#ifndef _LIBRARYREADER_H_
#define _LIBRARYREADER_H_
#include <list>
#include <string>
namespace stview
{
    class SCell;
    class SPort;
    class Attribute{
        public:
            Attribute(char *on, char *n, void *v) :
                objName(on), name(n),  val(v) {
            }
            ~Attribute() {
            }
            char *objName;
            char *name;
            void *val;
    };    
    class LibraryReader
    {
        public:
            static            LibraryReader* instance();
            ~LibraryReader();

            int             readLibrary(bool);
            void            setCells(std::list<SCell*>*);
            std::list<SCell*>* getCells();
            void            processAttributes(std::list<Attribute>&, 
                                            std::list<SPort*>*,
                                            SCell*);
        private:
            LibraryReader();
            LibraryReader(const LibraryReader&);
            LibraryReader& operator=(const LibraryReader&);

            bool            doChecks() const;
            bool            doChecks(const SCell*) const;
            void            bitBlast(const SCell*) const;
            FILE*            openLibraryFile() const;
            void            processAttribute(Attribute&, 
                                std::list<SPort*>*,
                                SCell*);

            void            print() const;                    

            static LibraryReader*    libraryReader;            
            bool                    generic;
            std::list<SCell*>*        cells;
    };

    
}
#endif
