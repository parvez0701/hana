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


#include "rmnets.h"

#include "sinst.h"
#include "sblock.h"
#include "snet.h"

using std::list;
using std::string;

using stview::SInst;
using stview::SBlock;
using stview::SNet;

namespace opt
{
    RmNets::RmNets() {
    }

    RmNets::~RmNets() {
    }

    bool
    RmNets::operator()(SInst *inst) {
        return false;
    }

    bool
    RmNets::optimize(SBlock *block)
    {
        bool status = false;

        SBlock::Nets nets = block->getNets();
        list<string> names;

        for(SBlock::Nets::const_iterator net = nets.begin();
            net != nets.end(); ++net)
        {
            if(condition(*net)) {
                names.push_back((*net)->getName());
            }    
        }

        if(!names.empty()) {
            status = true;
        }

        for(list<string>::const_iterator name = names.begin();
            name != names.end(); ++name)
        {
            const SNet *net = block->getNet(*name);
            block->deleteNet(const_cast<SNet*>(net));
        }
        return status;
        
    }

    bool
    RmNets::condition(const SNet* net)
    {
        return net->sempty() && net->dempty();
    }
    
}
