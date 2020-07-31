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

#include "syroot.h"
#include "syparse2synth.h"
#include "syvisitor.h"
#include "sycontainer.h"
#include "boptimizer.h"
#include "rtl2synth.h"
#include "synthesizer.h"
#include "sycmdline.h"
#include "symsghandler.h"
#include "genmapmgr.h"
#include "netlistwriter.h"
#include "netoptmgr.h"
#include "techmapmgr.h"

#include "msghandler.h"
#include "techmapreport.h"

#include "sopmgr.h"

using namespace std;
using namespace Synth;
using namespace stview;
using namespace GenericMapping;
using namespace opt;
using namespace TechnologyMapping;
using namespace SOP;
using Rtl::Root;
namespace Rtl{
    extern Root* GRoot;
}

int
main(int argc, char** argv)
{
    if(!SyCmdLine::instance()->parseCmdLine(argc, argv)){
        //Bad command line specified, quit.
        return 1;
    }    
    SyCmdOpts opts = SyCmdLine::instance()->getAnalyzerOptions();
    try{
        Rtl::GRoot = new Root(opts.argc, opts.argv);
    }
    catch (...){
        return 1;
    }
    if(!Rtl::GRoot->parse()){
        Rtl::MsgHandler::instance()->print(6);
        MsgHandler::instance()->close();
        //Set the log file.
        if(SyCmdLine::instance()->logFileSpecified()){
            SyMsgHandler::instance()->setLogFile(
            SyCmdLine::instance()->getLogFile(), false);
        }    
        SyParse2Synth::instance()->parse2Synth(Rtl::GRoot);
        //Clear up the RTL data base.
        delete Rtl::GRoot;
        BOptimizer::instance()->optimize();
        if(SyCmdLine::instance()->rtlFileSpecified()) 
        {
            //Print out the RTL after modification/simplification
            //done to it.
            SyVisitor visitor(SyCmdLine::instance()->getRtlOutFile());
            visitor.visitObjectModel();
            visitor.close();
            //End print
        }    

        //Convert the RTL into internal netlist representation.
        //In this step, we map without regards to availability of
        //a gate. So we will generate an N bit adder where N can be
        //anything, if RTL requires that.
        Synthesizer::instance()->synthesize();
        //End convert
        if(SyCmdLine::instance()->intOutFileSpecified())
        {
            //Print out the internal netlist representation.
            SyVisitor visitor(SyCmdLine::instance()->getIntOutFile());
            SyRoot::switchStage();
            visitor.visitObjectModel();
            visitor.close();
            //End print
            //Delete STAGE_1 root
            SyRoot::switchStage();
            SyRoot::clearRoot();
            //End delete
        }

        SyRoot::instance()->setStage(SyRoot::STAGE_2);

        if(SyCmdLine::instance()->synthFileSpecified())
        {
            GenMapMgr::instance()->doMapping();
            //Delete STAGE_2 root
            SyRoot::clearRoot();
            //End delete
            NetOptMgr::instance()->optimize();
            TechMapMgr::instance()->doMapping();
			if(SyCmdLine::instance()->sop()) {
			    SopMgr::instance()->toSumOfProductForm();
			}	

            NetlistWriter writer(SyCmdLine::instance()->getSynthOutFile());
            writer.write();
        }    

        SyMsgHandler::instance()->print(500,
            SyMsgHandler::instance()->getMsgCount(Rtl::MsgInfo::ERROR),
            SyMsgHandler::instance()->getMsgCount(Rtl::MsgInfo::WARN));

        TechMapReport::instance()->printAreaReport();

        SyMsgHandler::instance()->close();
        return 0;
    }
    else{
        Rtl::MsgHandler::instance()->print(7);
        MsgHandler::instance()->close();
    }

}
