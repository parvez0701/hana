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

#include <iostream>
#include <stdio.h>
#include "root.h"
#include "utils.h"
#include "modinst.h"
#include "module.h"
#include "pin.h"
#include "expr.h"
#include "msghandler.h"
using namespace std;
using Rtl::Root;
namespace Rtl{
    extern Root* GRoot;
}

void write();
int
main(int argc, char** argv)
{
    try{
        Rtl::GRoot = new Root(argc, argv);
    }
    catch (...){
        return 1;
    }
    if(!Rtl::GRoot->parse()){
    //    Rtl::GRoot->printLogMsg("[INFO]: Successfully parsed the design.\n");
        Rtl::MsgHandler::instance()->print(6);
        Rtl::GRoot->decompile();
    //    write();
        Rtl::MsgHandler::instance()->close();
        delete Rtl::GRoot;
        return 0;
    }
    else{
        //Rtl::GRoot->printLogMsg("[INFO]: Please correct above errors first!\n");
        Rtl::MsgHandler::instance()->print(7);
        Rtl::MsgHandler::instance()->close();
    }
    delete Rtl::GRoot;
    return 1;
}


void write()
{
#if 0
    const   vector<Rtl::File*>& files = Rtl::GRoot->getFiles();
    int filec = files.size();

    for(int i = 0; i < filec; ++i)
    {
        Rtl::File* file = files[i];
        const vector<Rtl::Module*>& modules = file->getModules();
        const int modc = modules.size();
        for(int j = 0; j < modc; ++j){
            const std::vector<Rtl::ModInst*>& insts = 
            modules[j]->getModInsts();
            const int instc = insts.size();
            for(int k = 0; k < instc; ++k)
            {
                const std::vector<Rtl::Pin*>&  pins = insts[k]->getPins();
                const int pinc = pins.size();
                for(int l = 0; l < pinc; ++l)
                {
                    cout << "NAME : " << (pins[l]->getIxName()).toString() << endl;
                    const Rtl::Expr* actual = pins[l]->getActual();
                    if(actual) {
                        cout  << "ACTUAL " << actual->toString() << endl;
                    }
                    else {
                        cout << "ACTUAL NULL\n";
                    }
                }
            }
        }
    }
#endif
}


