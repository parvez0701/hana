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

#include <map>

#include "scellmgr.h"

#include "genmapmgr.h"
#include "genaddsubmap.h"
#include "genandmap.h"
#include "genblockinstmap.h"
#include "genblockmap.h"
#include "genbufmap.h"
#include "gendecmap.h"
#include "genencmap.h"
#include "gengndmap.h"
#include "genmuxmap.h"
#include "gennandmap.h"
#include "gennormap.h"
#include "gennotmap.h"
#include "genormap.h"
#include "genseqmap.h"
#include "gentribufmap.h"
#include "genvccmap.h"
#include "genxnormap.h"
#include "genxormap.h"
#include "gencmpmap.h"
#include "genmuldivmap.h"
#include "genshiftmap.h"
#include "genincmap.h"

#include "sblock.h"
#include "sroot.h"

#include "sycontainer.h"
#include "syroot.h"
#include "symsghandler.h"

using std::list;
using std::map;
using std::make_pair;

using Synth::SyModule;
using Synth::SyModInst;
using Synth::SyRoot;
using Synth::SyMsgHandler;

using namespace stview;
namespace GenericMapping
{
    GenMapMgr* GenMapMgr::genMapMgr = 0;

    GenMapMgr::GenMapMgr() {
        init();
    }

    GenMapMgr::~GenMapMgr() {
        GenMapMgr::genMapMgr = 0;
    }

    GenMapMgr*
    GenMapMgr::instance()
    {
        if(!GenMapMgr::genMapMgr) {
            GenMapMgr::genMapMgr = new GenMapMgr();
        }
        return GenMapMgr::genMapMgr;
    }
 
    void
    GenMapMgr::init() 
    {
        mappers.push_back(new GenericAddSubMapper());
        mappers.push_back(new GenericAndMapper());
        mappers.push_back(new GenericBlockInstMapper());
        mappers.push_back(new GenericBufMapper());
        mappers.push_back(new GenericDecoderMapper());
        mappers.push_back(new GenericEncoderMapper());
        mappers.push_back(new GenericGndMapper());
        mappers.push_back(new GenericMuxMapper());
        mappers.push_back(new GenericNandMapper());
        mappers.push_back(new GenericNorMapper());
        mappers.push_back(new GenericNotMapper());
        mappers.push_back(new GenericOrMapper());
        mappers.push_back(new GenericSeqElementMapper());
        mappers.push_back(new GenericTribufMapper());
        mappers.push_back(new GenericVccMapper());
        mappers.push_back(new GenericXnorMapper());
        mappers.push_back(new GenericXorMapper());
        mappers.push_back(new GenericCmpMapper());
        mappers.push_back(new GenericMulDivMapper());
        mappers.push_back(new GenericShiftMapper());
        mappers.push_back(new GenericIncMapper());
    }    


    void
    GenMapMgr::doMapping()
    {
        SyMsgHandler::instance()->print(40);
        doModuleToBlockMapping();
        doGeneralMapping();
        SyMsgHandler::instance()->print(41);
    }
    void    
    GenMapMgr::doModuleToBlockMapping() 
    {
        const map<string, SyModule*, Synth::cmp_str> & modules =
            SyRoot::instance()->getSynthMods();
        
        map<string, SyModule*, Synth::cmp_str >::const_iterator modIter = modules.begin();
        for(; modIter != modules.end(); ++modIter)
        {
            SBlock *block = new SBlock(modIter->second->getName());
            SRoot::instance()->addBlock(block);
            blocks.push_back(block);
        }
        for(list<SBlock*>::iterator block = blocks.begin(); 
            block != blocks.end(); ++block)
        {
            GenericBlockMapper blockMapper(*block);
            blockMapper.doMapping();
        }
        this->doInstMasterMapping();
    }

    void
    GenMapMgr::doGeneralMapping()
    {
        for(list<SBlock*>::iterator block = blocks.begin(); 
            block != blocks.end(); ++block) {
            for(list<GenericMapperBase*>::iterator mapper = mappers.begin();
                mapper != mappers.end(); ++mapper)
            {
                    GenericMapperBase *mapperBase = (*mapper)->create(*block);
                    mapperBase->doMapping();
                    delete mapperBase;
            }        
        }    
    }

    void
    GenMapMgr::doInstMasterMapping()
    {
        list<SBlock*>::iterator blockIter = blocks.begin();
        for(; blockIter != blocks.end(); ++blockIter)
        {
            GenericBlockMapper blockMapper(*blockIter);
            blockMapper.createBlockInsts();
        }
    }

    const SCell*
    GenMapMgr::getCell(SCell::CellType type, int input) const {
        return SCellMgr::instance()->getGenericCell(type, input);
    }    
}
