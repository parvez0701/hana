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

#include <cassert>

#include "sroot.h"

#include "sblock.h"
#include "sblockinst.h"

using std::string;
using std::map;
using std::list;
using std::make_pair;

namespace stview
{
    SRoot *SRoot::root = 0;

    SRoot*
    SRoot::instance()
    {
        if(!SRoot::root){
            SRoot::root = new SRoot();
        }
        return SRoot::root;
    }

    SRoot::SRoot() {
    }

    SRoot::~SRoot(){
    } 

    void
    SRoot::initialize(const Synth::SyRoot *synthRoot){
    }

    const SBlock*
    SRoot::getBlock(const string& name) const
    {
        map<string, SBlock*, cmp_str>::const_iterator iter =
            blockTable.find(name);
        
        if(iter == blockTable.end()) {
            return 0;
        }
        return iter->second;
    }
    SBlock*
    SRoot::getBlock(const string& name) 
    {
        map<string, SBlock*, cmp_str>::iterator iter =
            blockTable.find(name);
        
        if(iter == blockTable.end()) {
            return 0;
        }
        return iter->second;
    }

    void
    SRoot::addBlock(const SBlock *block) 
    {
        if(!block) {
            return;
        }    
        blocks.push_back(const_cast<SBlock*>(block));
        blockTable.insert(make_pair(block->getName(), 
            const_cast<SBlock*>(block)));
    }    

    const list<SBlock*>& 
    SRoot::getTopBlocks() const
    {
        if(!topBlocks.empty()) {
            return topBlocks;
        }

        map<SBlock*, int> blockInstCount;
        for(list<SBlock*>::const_iterator block = blocks.begin();
            block != blocks.end(); ++block)
        {
            blockInstCount.insert(make_pair((*block), 0)); //should fail if already present
            const list<SBlockInst*>& blockInsts = (*block)->getBlockInsts();
            for(list<SBlockInst*>::const_iterator binst = blockInsts.begin();
                binst != blockInsts.end(); ++binst)
            {
                SBlock *blk = (*binst)->getBlock();
                if(!blk) {
                    continue;
                }    
                map<SBlock*, int>::iterator iter = blockInstCount.find(blk);
                if(iter == blockInstCount.end()) {
                    blockInstCount.insert(make_pair(blk, 1));
                }
                else {
                    ++(iter->second);
                }    
            }
        }

        for(map<SBlock*, int>::const_iterator iter = blockInstCount.begin();
            iter != blockInstCount.end(); ++iter) {
            if(iter->second == 0) {
                topBlocks.push_back(iter->first);
            }
        }
        return topBlocks;
    }

    void
    SRoot::resetVisitCount()
    {
        ResetVisitCountBlock(blocks);
        ResetVisitCountBlock(topBlocks);
    }
}
