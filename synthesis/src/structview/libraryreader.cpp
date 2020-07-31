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

#include <stdlib.h>
#include <iostream>

#include "libraryreader.h"
#include "scell.h"
#include "sport.h"

#include "symsghandler.h"
#include "sycmdline.h"
#include "generic.h"

#include "cellsemcheckmgr.h"

using std::list;
using std::string;
using std::cout;
using std::endl;

using Synth::SyMsgHandler;
using Synth::SyCmdLine;
using Synth::cmp_str;

struct yy_buffer_state;
extern struct yy_buffer_state* CellLib_scan_string (const char *);
extern FILE* CellLibin;
extern int CellLibparse();
extern int CellLiblex_destroy (void);
namespace stview
{
    LibraryReader*    LibraryReader::libraryReader = 0;

    LibraryReader::LibraryReader() :generic(true) , cells(0) {
    }

    LibraryReader::~LibraryReader() {
        LibraryReader::libraryReader = 0;
    }

    LibraryReader*
    LibraryReader::instance()
    {
        if(!LibraryReader::libraryReader) {
            LibraryReader::libraryReader = new LibraryReader();
        }
        return LibraryReader::libraryReader;
    }

    void
    LibraryReader::setCells(list<SCell*>* listOfCells) {
        cells = listOfCells;
    }    

    list<SCell*>*
    LibraryReader::getCells() {
        return cells;
    }    

    int
    LibraryReader::readLibrary(bool gen)
    {
        cells = 0;
        generic = gen;
        int status = 0;
        if(gen) {
            CellLib_scan_string(GenericLib);
        }    
        else
        {
            CellLibin = openLibraryFile();
            if(!CellLibin) {
                return 1;
            }    
        }    

        status = ::CellLibparse();
        if(status) {
            return status;
        }    

        if(!cells){ //should be set by now 
            return status;
        }    
        if(!CellSemCheckMgr::instance()->doChecks()){
            return status+1;
        }
        if(gen) {
            CellLiblex_destroy();
        }    

        return status;
            
    }

    FILE*
    LibraryReader::openLibraryFile() const
    {
        string path;
        if(generic)
        {
            char *libPath = getenv("SYNTH_ROOT");
            path = string(libPath ? libPath: "") + string("/generic.lib");
        }
        else if(SyCmdLine::instance()->cellLibrarySpecified()) {
            path = SyCmdLine::instance()->getCellLibraryPath();
        }
        else {
            return 0;
        }    
        FILE *file = fopen(path.c_str(), "r");

        if(!file) {
            SyMsgHandler::instance()->print(23, path.c_str());
        }    
        return file;
    }

    void
    LibraryReader::processAttributes(list<Attribute>& attribs, 
                                    list<SPort*>* ports, 
                                    SCell *cell)
    {
        for(list<Attribute>::iterator attrib = attribs.begin();
            attrib != attribs.end(); ++attrib) {
            processAttribute(*attrib, ports, cell);
        }    
    }
    void
    LibraryReader::processAttribute(Attribute& attrib, 
                                    list<SPort*>* ports, 
                                    SCell *cell)
    {
        if(!ports) {
            return;
        }
        for(list<SPort*>::iterator port = ports->begin();
            port != ports->end(); ++port) {
            if((*port)->getVecName() == attrib.objName) {
                (*port)->setAttribute(attrib.name, attrib.val);
            }
            else {
                cell->setAttribute(attrib.name, attrib.val);
            }    
        }    
            
    }

    void
    LibraryReader::print() const
    {
        cout << "Generic Cell Library\n";
        cout << "-------------------------------------------\n";
        //cout << "Cell        Input        Output        Type\n";
        //cout << "--------------------------------------------

        for(list<SCell*>::const_iterator cell = cells->begin();
            cell != cells->end(); ++cell)
        {
            cout << "Cell: " << (*cell)->getName() << endl;
            cout << "Input: ";
            
            const list<SPort*>& ports = (*cell)->getPorts();
            for(list<SPort*>::const_iterator port = ports.begin();
                port != ports.end(); ++port) 
            {
                if((*port)->getDir() != SPort::IN) {
                    continue;
                }
                cout << (*port)->getName();
                cout << "    ";
            }    
            cout << "\n\nOutput: ";
            for(list<SPort*>::const_iterator port = ports.begin();
                port != ports.end(); ++port) 
            {
                if((*port)->getDir() != SPort::OUT) {
                    continue;
                }
                cout << (*port)->getName();
                cout << "    ";
            }    
            cout << "\n";
        }
    }
/*
    bool
    LibraryReader::doChecks()
    {
        if(!cells) {
            return true;
        }

        for(list<SCell*>::const_iteratir cell = cells->begin();
            cell != cells->end(); ++cell) {
            doChecks(*cell);
        }
    }    
*/        
}
