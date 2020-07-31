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

#include <list>

#include "root.h"
#include "module.h"
#include "omvisitor.h"
#include "omcreator.h"
#include "semcheck.h"
#include "modinst.h"
#include "elaborator.h"

extern FILE* yyin;
extern int yyparse();
using std::cout;
using std::cerr;
using std::map;
using std::ios_base;
using std::endl;
using std::list;
namespace Rtl
{
    Root* GRoot = NULL;
    Root::Root(): version("1.0-alpha") {
        incldirs.push_back("./");
    }
    Root::Root(int argc, char** argv):  version("1.0-alpha")
    {
        incldirs.push_back("./");
        if(int res = parse(argc, argv))
        {
            if(res != 2) {
                this->printHelp();
            }    
            incldirs.clear();
            throw_error;
        }
    }

    Root::~Root()
    {
        logfile.close();
		std::for_each(files.begin(), files.end(), DELETE<File>());
		std::for_each(libfiles.begin(), libfiles.end(), DELETE<File>());
    }

    
    int
    Root::parse()
    {
        int status = 0;
        //Picks up the first file from file list and starts parsing
        if(files.empty())
        {
            MsgHandler::instance()->print(11);
            return 1;
        }
        File* desfile = files[0];
        OMCreator::instance()->setCurrFile(desfile);
        yyin = desfile->open();
        if(yyin != 0) 
        {
            MsgHandler::instance()->print(1, "design", desfile->getName().c_str());
            status = yyparse();
        }
        else {
            return 1;
        }
        if(status){
            return status;
        }
        status = MsgHandler::instance()->getMsgCount(MsgInfo::ERROR);
        if(status){
            return status;
        }
        
            
        status = SemCheck::instance()->doSemChecks();
        if(topmodules.empty())
        {
            MsgHandler::instance()->print(70);
            return 1;
        }    

        return status;
    }

    void
    Root::printHelp() const{
        MsgHandler::instance()->print("    Usage: bulah <filename>\n");
        MsgHandler::instance()->print("    -d <decompile file> \n");
        MsgHandler::instance()->print("    -log <log file> \n");
        MsgHandler::instance()->print("    -lib <library file> \n");
        MsgHandler::instance()->print("    -I <include dir name> \n");
    }

    void
    Root::printVersion() const
    {
        string vstring("bulah Verilog (2001) HDL Analyzer, version ");
        vstring += version;
        vstring += "\nCopyright (C) 2009-2010 Parvez Ahmad."; 
        vstring += "\nThis is free software; see the source for copying conditions.  There is NO";
        vstring += "\nwarranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n";
        MsgHandler::instance()->print(vstring);
    }

    int
    Root::checkAndAddInclDir(int incp, int argc, char** argv)
    {
        if(incp >= argc)
        {
            MsgHandler::instance()->print(12);
            return 1;
        }
        incldirs.push_back(argv[incp+1]);
        //We are not checking existence of dir at this moment
        //This will be done if required when we are actually
        //trying to read the include file.
        return 0;
    }




    int
    Root::checkAndAddDecFile(int decp, int argc, char** argv)
    {
        std::ofstream decfile;
        if(decp >= argc)
        {
            MsgHandler::instance()->print(13);
            return 1;
        }

        decfile.open(argv[decp+1], ios_base::trunc);
        if(decfile.bad())
        {
            MsgHandler::instance()->print(14, argv[decp+1]);
            return 1;
        }
        decfile.close();
        decfname = string(argv[decp+1]);
        return 0;
    }

    int
    Root::checkAndAddLogFile(int logp, int argc, char** argv)
    {
        if(logp >= argc)
        {
            MsgHandler::instance()->print(15);
            return 1;
        }

        if(MsgHandler::instance()->setLogFile(argv[logp+1]))
        {
            MsgHandler::instance()->print(16, argv[logp+1]);
            return 1;
        }
        //logfile.close();
        logfname = string(argv[logp+1]);
        return 0;
    }
    int
    Root::checkAndAddLibFile(int libp, int argc, char** argv)
    {
        std::ifstream libfile;
        if(libp >= argc)
        {
            MsgHandler::instance()->print(17);
            return 1;
        }

        libfile.open(argv[libp+1], ios_base::in);
        if(libfile.bad())
        {
            MsgHandler::instance()->print(10, "library", argv[libp+1]);
            return 1;
        }
        libfile.close();
        libfiles.push_back(new File(string(argv[libp+1]), File::LIB));
        return 0;
    }
    int
    Root::parse(int argc, char** argv)
    {
        int i = 1;
        if(argc == 1){
            return 1;
        }
        for(; i < argc; ++i)
        {
            if(!strcmp(argv[i], "-d"))
            {
                if(this->checkAndAddDecFile(i, argc, argv)){
                    return 1;
                }
                ++i;
            }

            else if(!strcmp(argv[i], "-log"))
            {
                if(this->checkAndAddLogFile(i, argc, argv)){
                    return 1;
                }
                ++i;
            }
            else if(!strcmp(argv[i], "-lib"))
            {
                if(this->checkAndAddLibFile(i, argc, argv)){
                    return 1;
                }
                ++i;
            }
            else if(!strcmp(argv[i], "-I")){
                if(this->checkAndAddInclDir(i, argc, argv)){
                    return 1;
                }
                ++i;
            }
            else if(!strcmp(argv[i], "--help"))
            {
                this->printHelp();
                return 2;
            }    
            else if(!strcmp(argv[i], "--version"))
            {
                this->printVersion();
                return 2;
            }    
            else
            {
                const char* filename = argv[i];
                files.push_back(new File(filename, File::SRC));
            }
        }
        return 0;
    }

    void
    Root::decompile() const
    {
        if(OMCreator::instance()->getErrCount() != 0){
            return;
        }
        if(decfname.empty()){
            return;
        }

        OMVisitor omv(decfname);
        omv.writeDesign();
    }


    Base*
    Root::addObject(const string& n,
        const Base* b, const bool rep)
    {
        if(rep)
        {
            this->removeObject(n);
            symbolTable.insert(make_pair(const_cast<string&>(n),
            const_cast<Base*>(b)));
            return const_cast<Base*>(b);
        }
        Base* tobj = this->getObject(n);
        if(tobj){
            return NULL;
        }
        symbolTable.insert(make_pair(n, const_cast<Base*>(b)));
        return const_cast<Base*>(b);
    }
    Base*
    Root::getObject(const string& oname) const
    {
        map<string, Base*, compare_str>::const_iterator
        obj = symbolTable.find(oname);
        if(obj == symbolTable.end()){
            return NULL;
        }

        return obj->second;
    }

    void
    Root::populateTopModules()
    {
        //Map between module and its instance count
        map<string, int, compare_str> nameInstCount;
        map<string, Base*, compare_str>::const_iterator iter = 
            symbolTable.begin();
        map<string, Base*, compare_str>::const_iterator eiter = 
            symbolTable.end();

        for(; iter != eiter; ++iter)
        {
            const Module* mod = dynamic_cast<const Module*>(
                    iter->second);
            if(!mod){
                continue;
            }
            nameInstCount.insert(make_pair(mod->getName(), 0));
        }
        iter = symbolTable.begin();
        for(; iter != eiter; ++iter)
        {
            const Module* mod = dynamic_cast<const Module*>(
                    iter->second);
            if(!mod){
                continue;
            }
            const list<ModInst*>& insts = mod->getModInsts();
            list<ModInst*>::const_iterator inst = insts.begin();
            list<ModInst*>::const_iterator einst = insts.end();

            for(; inst != einst; ++inst)
            {
                const Module* master = (*inst)->getMaster();
                if(!master){
                    continue;
                }
                map<string, int, compare_str>::iterator niciter =
                    nameInstCount.find(master->getName());
                ++(niciter->second);
            }
        }
        map<string, int, compare_str>::iterator niciter =
            nameInstCount.begin();
        map<string, int, compare_str>::iterator eniciter =
            nameInstCount.end();

        for(; niciter != eniciter; ++niciter){
            if(niciter->second == 0){
                topmodules.push_back(const_cast<Module*>(
                            dynamic_cast<const Module*>(
                                this->getObject(niciter->first))));
            }
        }
    }
    bool
    Root::isTopModule(const string& name) const {
        const int size = topmodules.size();
        for(int i = 0; i < size; ++i){
            if(topmodules[i]->getName() == name){
                return true;
            }
        }
        return false;
    }




}
