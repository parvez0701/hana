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

#include "netoptmgr.h"

#include "sycmdline.h"
#include "symsghandler.h"
#include "logiconeopt.h"
#include "logiczeroopt.h"
#include "rminsts.h"
#include "rmbufs.h"
#include "rmnets.h"
#include "invabs.h"
#include "cellmerge.h"
#include "cellreduce.h"


#include "sblock.h"
#include "sroot.h"
#include "sinst.h"

#include "snet.h"
#include "spin.h"

using std::list;

using stview::SBlock;
using stview::SRoot;
using stview::SInst;
using stview::SInstBase;
using stview::SNet;
using stview::SCntv;
using stview::SPin;
namespace opt
{
    static void PrintDriverSink(const SBlock*);
    NetOptMgr* NetOptMgr::netOptMgr = 0;

    NetOptMgr::NetOptMgr() {
        init();
    }

    NetOptMgr::~NetOptMgr() {
        netOptMgr = 0;
    }

    NetOptMgr*
    NetOptMgr::instance() 
    {
        if(!netOptMgr) {
            netOptMgr = new NetOptMgr();
        }
        return netOptMgr;
    }    

    void
    NetOptMgr::init() 
    {
        netOptList.push_back(new LogicOneOpt());
        netOptList.push_back(new LogicZeroOpt());
        netOptList.push_back(new RmInsts());
        netOptList.push_back(new RmBufs());
        netOptList.push_back(new RmNets());
        netOptList.push_back(new InvAbs());
        netOptList.push_back(new CellMerge());
        netOptList.push_back(new CellReduce());
    }

    bool
    NetOptMgr::optimize() 
    {
        if(Synth::SyCmdLine::instance()->noOpt()) {
            return false;
        }    
        const list<SBlock*>& blocks = SRoot::instance()->getBlocks();
        list<SBlock*>::const_iterator blockIter = blocks.begin();
        list<SBlock*>::const_iterator blockIterEnd = blocks.end();
        Synth::SyMsgHandler::instance()->print(38, 1);

        for(; blockIter != blockIterEnd; ++blockIter)
        {
            SBlock *block = *blockIter;
            bool netlistChanged = true;
            while(netlistChanged)
            {
                PrintDriverSink(block);
                //keep doing until there is no change in the netlist
                netlistChanged = false;
                NetOptIter netOptIter = netOptList.begin();
                for(; netOptIter != netOptList.end(); ++netOptIter) 
                {
                    (*netOptIter)->clear();
                    (*netOptIter)->init();
                    const list<SInst*>& insts = block->getInsts();
                    list<SInst*>::const_iterator inst = insts.begin();
                    for(; inst != insts.end(); ++inst) {
                        (**netOptIter)(*inst);
                    }
                    (*netOptIter)->init();
                    PrintDriverSink(block);
                    if((*netOptIter)->optimize(block)) {
                        netlistChanged = true;
                    }
                }    
            }
        }
        Synth::SyMsgHandler::instance()->print(39, 1);
        return true;
    }

    void PrintDriverSink(const SBlock *block)
    {
    /*
        const list<SNet*>& nets = block->getNets();

        for(list<SNet*>::const_iterator iter = nets.begin();
            iter != nets.end(); ++iter) 
        {
            std::cout << "NET  " << (*iter)->getName() << std::endl;
            std::cout << "DRIVERS\n";
            std::cout << "--------------\n";
            SCntv::Drivers& drivers = (*iter)->getDrivers();
            for(SCntv::Drivers::const_iterator d = drivers.begin();
                d != drivers.end(); ++d) 
            {
                std::cout << (*d)->getName() << std::endl;
                SPin *pin = static_cast<SPin*>(*d);
                const SInstBase *inst = pin->getInst();
                std::cout << inst->getName() << std::endl;
            }    

            std::cout << "SINKS\n";
            std::cout << "--------------\n";
            SCntv::Sinks& sinks = (*iter)->getSinks();
            for(SCntv::Sinks::const_iterator s = sinks.begin();
                s != sinks.end(); ++s) 
            {
                std::cout << (*s)->getName() << std::endl;
                SPin *pin = static_cast<SPin*>(*s);
                const SInstBase *inst = pin->getInst();
                std::cout << inst->getName() << std::endl;
            }    

        }
        */
    }
}
