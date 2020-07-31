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
#include <malloc.h>
#include "sycmdline.h"
#include "symsghandler.h"
#include "syroot.h"

using std::string;
using std::cout;
using std::endl;

namespace Synth
{
    SyCmdLine* SyCmdLine::cmdLine = 0;

    SyCmdLine:: SyCmdLine() 
    {
        analyzerOpts.argc = 1;
        analyzerOpts.argv = (char **) malloc(128*sizeof(char*));
        analyzerOpts.argv[0] = strdup(" ");
        synthOpts.argc = 0;
        synthOpts.argv = (char **) malloc(128*sizeof(char*));
        noOptimization = false;
		sumOfProduct = false;
//		sumOfProduct = true;
        version = "1.0-alpha";
    }

    SyCmdLine::~SyCmdLine() {
        free(analyzerOpts.argv);
        free(synthOpts.argv);
    }

    SyCmdLine*
    SyCmdLine::instance() 
    {
        if(!SyCmdLine::cmdLine){
            SyCmdLine::cmdLine = new SyCmdLine();
        }
        return SyCmdLine::cmdLine;
    }    

    const SyCmdOpts&
    SyCmdLine::getAnalyzerOptions() const {
        return analyzerOpts;
    }

    const SyCmdOpts&
    SyCmdLine::getSynthOptions() const {
        return synthOpts;
    }    

    const string& 
    SyCmdLine::getSynthOutFile() const {
        return synthFile;
    }
    const string& 
    SyCmdLine::getRtlOutFile() const {
        return rtlFile;
    }    

    const string&
    SyCmdLine::getIntOutFile() const {
        return intOutFile;
    }    

    const string& 
    SyCmdLine::getLogFile() const {
        return logFile;
    }    

    bool
    SyCmdLine::logFileSpecified() const {
        return !logFile.empty();
    }

    bool
    SyCmdLine::cellLibrarySpecified() const {
        return !cellLibPath.empty();
    }

    bool
    SyCmdLine::intOutFileSpecified() const {
        return !intOutFile.empty();
    }    

    const string&
    SyCmdLine::getCellLibraryPath() const {
        return cellLibPath;
    }    
    

    bool
    SyCmdLine::rtlFileSpecified() const {
        return !rtlFile.empty();
    }

    bool
    SyCmdLine::synthFileSpecified() const {
        return !synthFile.empty();
    }    

    bool
    SyCmdLine::parseCmdLine(int argc, char **argv)
    {
        if(argc == 1)
        {
            this->printHelp();
            return false;
        }    
        
        for(int i = 1; i < argc; ++i)
        {
            string arg(argv[i]);
            if(arg == string("-n")) {
                noOptimization = true;
				sumOfProduct = false;
            }    
            if(arg == string("--sop")) {
			    if(!noOptimization) {
				    sumOfProduct = true;
				}	
            }    
            else if(arg == string("--help")) 
            {
                this->printOptions();
                return false;
            }
            else if(arg == string("--version")) 
            {
                this->printVersion();
                return false;
            }
            else if(arg == string("-s"))
            {
                if(!this->checkForMissingArg(argc, argv, i)){
                    return false;
                }    
                synthFile = argv[++i];
                synthOpts.argv[synthOpts.argc++] = strdup("-s");
            }
            else if(arg == string("-i"))
            {
                //option not to be published
                if(!this->checkForMissingArg(argc, argv, i)){
                    return false;
                }    
                intOutFile = argv[++i];
                synthOpts.argv[synthOpts.argc++] = strdup("-s");
            }
            else if(arg == string("-c"))
            {
                if(!this->checkForMissingArg(argc, argv, i)){
                    return false;
                }    
                cellLibPath = argv[++i];
                synthOpts.argv[synthOpts.argc++] = strdup("-s");
            }
            else if(arg == string("-r"))
            {
                if(!this->checkForMissingArg(argc, argv, i)){
                    return false;
                }    
                rtlFile = argv[++i];
                synthOpts.argv[synthOpts.argc++] = strdup("-r");
                synthOpts.argv[synthOpts.argc++] = strdup(argv[i]);
            }
            else if(arg == string("-d"))
            {
                if(!this->checkForMissingArg(argc, argv, i)){
                    return false;
                }    
                analyzerOpts.argv[analyzerOpts.argc++] = strdup("-d");
                analyzerOpts.argv[analyzerOpts.argc++] = strdup(argv[++i]);
            }
            else if(arg == string("-v"))
            {
                if(!this->checkForMissingArg(argc, argv, i)){
                    return false;
                }    
                analyzerOpts.argv[analyzerOpts.argc++] = strdup("-v");
                analyzerOpts.argv[analyzerOpts.argc++] = strdup(argv[++i]);
            }
            
            else if(arg == string("-l"))
            {
                if(!this->checkForMissingArg(argc, argv, i)){
                    return false;
                }    
                analyzerOpts.argv[analyzerOpts.argc++] = strdup("-log");
                analyzerOpts.argv[analyzerOpts.argc++] = strdup(argv[++i]);
                synthOpts.argv[synthOpts.argc++] = strdup("-l");
                synthOpts.argv[synthOpts.argc++] = strdup(argv[i]);
                logFile = argv[i];
            }
            else if(arg == string("-I"))
            {
                if(!this->checkForMissingArg(argc, argv, i)){
                    return false;
                }    
                analyzerOpts.argv[analyzerOpts.argc++] = strdup("-I");
                analyzerOpts.argv[analyzerOpts.argc++] = strdup(argv[++i]);
            }
            else if(arg == string("-memmod"))
            {
                if(!this->checkForMissingArg(argc, argv, i)){
                    return false;
                }    
                synthOpts.argv[synthOpts.argc++] = strdup("-memmod");
                synthOpts.argv[synthOpts.argc++] = strdup(argv[++i]);
                SyRoot::instance()->addDontTouchModule(argv[i]);
            }    
            else {
                analyzerOpts.argv[analyzerOpts.argc++] = strdup(argv[i]);
            }

        }    
        return true;
    }    
            

 
    bool
    SyCmdLine::checkForMissingArg(int argc, char **argv, int curr) const
    {
        if(argc == (curr+1))
        {
            SyMsgHandler::instance()->print(101, argv[curr]);
            return false;
        }
        return true;
    }    
    void
    SyCmdLine::printOptions() const {
        this->printHelp();
    }


    void
    SyCmdLine::printHelp() const
    {
        SyMsgHandler::instance()->print("Usage:    hana [OPTION]...  <verilog files>\n");
        SyMsgHandler::instance()->print("--help              print help\n");
        SyMsgHandler::instance()->print("--version           print version info\n");
        SyMsgHandler::instance()->print("-s <synthoutfile>   synthesis output in file <synthoutfile>\n");
        SyMsgHandler::instance()->print("-r <rtloutfile>     intermediate rtl output in file <rtloutfile>\n");
        SyMsgHandler::instance()->print("-d <decfile>        decompile the original rtl in file <decfile>\n");
        SyMsgHandler::instance()->print("-v <libfile>        specify library in <libfile> \n");
        SyMsgHandler::instance()->print("-l <logfile>        specify log message file \n");
        SyMsgHandler::instance()->print("-c <libraryfile>    specify cell library file \n");
        SyMsgHandler::instance()->print("-I <includedir>     specify include directories\n");
    }

    void
    SyCmdLine::printVersion() const
    {
        string vstring("hana (HDL Analyzer and Netlist Architect), version ");
        vstring += version;
        vstring += "\nCopyright (C) 2009-2011 Parvez Ahmad."; 
        vstring += "\nThis is free software; see the source for copying conditions.  There is NO";
        vstring += "\nwarranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n";
        MsgHandler::instance()->print(vstring);
    }
}
