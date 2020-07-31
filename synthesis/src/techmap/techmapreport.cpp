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

#include <sstream>
#include <iostream>
#include <fstream>

#include "techmapreport.h"

#include "symsghandler.h"

#include "sroot.h"
#include "sblock.h"
#include "sblockinst.h"
#include "sinst.h"
#include "sutils2.h"

using std::make_pair;
using std::cout;
using std::ostringstream;
using std::string;
using std::list;
using std::ofstream;

using namespace stview;
namespace TechnologyMapping
{
    TechMapReport *TechMapReport::techMapReport = 0;
    
    TechMapReport::TechMapReport() : align(20) {
        populateAreaInfo();
    }

    TechMapReport::~TechMapReport() {
    }

    TechMapReport*
    TechMapReport::instance()
    {
        if(!techMapReport) {
            techMapReport = new TechMapReport();
        }
        return techMapReport;
    }

    void
    TechMapReport::addInfo(const SInst *inst)
    {
        const SCell *cell = inst->getCell();

        AreaInfoMap::iterator info = areaInfoMap.find(cell->getName());
        if(info == areaInfoMap.end())
        {
            AreaInfo ainfo;
            ainfo.type = cell->getCellType();
            ainfo.numinst = 1;
            ainfo.area = cell->getArea();
            areaInfoMap.insert(make_pair(cell->getName(), ainfo));
        }
        else 
        {
            ++info->second.numinst;
            info->second.area = info->second.area + cell->getArea();
        }
    }

    void
    TechMapReport::populateAreaInfo()
    {
        const list<SBlock*> & blocks = SRoot::instance()->getTopBlocks();

        for(list<SBlock*>::const_iterator block = blocks.begin();
            block != blocks.end(); ++block) {
            this->populateAreaInfo(*block);
        }    
    }

    void
    TechMapReport::populateAreaInfo(const SBlock *block)
    {
        if(!block) {
            return;
        }    
        const list<SInst*>& insts = block->getInsts();
        for(list<SInst*>::const_iterator inst = insts.begin();
            inst != insts.end(); ++inst) {
            this->addInfo(*inst);
        }    

        const list<SBlockInst*>& binsts = block->getBlockInsts();
        for(list<SBlockInst*>::const_iterator binst = binsts.begin();
            binst != binsts.end(); ++binst) {
            this->populateAreaInfo((*binst)->getBlock());
        }
    }

    void
    TechMapReport::printMapFailReport() const 
    {
        const list<SBlock*> & blocks = SRoot::instance()->getBlocks();

        bool headerPrinted = false;

        for(list<SBlock*>::const_iterator block = blocks.begin();
            block != blocks.end(); ++block) 
        {
            string bname = (*block)->getName();
            
            const list<SInst*>& insts = (*block)->getInsts();
            for(list<SInst*>::const_iterator inst = insts.begin();
                inst != insts.end(); ++inst)
            {
                if(!(*inst)->isGeneric()) {
                    continue;
                }
                if(!headerPrinted)
                {
                    this->printFailReportHeader();
                    headerPrinted = true;
                }    
                ostringstream oss;
                oss << bname;
                if(bname.size() < align) {
                    oss << string(align-bname.size(), ' ');
                }
                string iname = (*inst)->getName();
                oss << iname;
                if(iname.size() < align) {
                    oss << string(align-iname.size(), ' ');
                }
                oss << CellType2String((*inst)->getCellType());
                oss << "\n";
                this->printMsg(oss.str());

            }
        }
        if(headerPrinted) {
            this->printFailReportFooter();
        }    
    }

    void
    TechMapReport::printAreaReport() const 
    {
        int count = 0;
        float area = 0.0;
        this->printAreaReportHeader();

        for(AreaInfoMap::const_iterator info = areaInfoMap.begin();
            info != areaInfoMap.end(); ++info)
        {
            ostringstream oss;
            oss << info->first;
            if(info->first.size() < align) {
                oss << string(align - info->first.size(), ' ');
            }
            oss << CellType2String(info->second.type);
            if(oss.str().size() < 2*align) {
                oss << string(2*align - oss.str().size(), ' ');
            }

            oss << info->second.numinst;
            if(oss.str().size() < 3*align) {
                oss << string(3*align - oss.str().size(), ' ');
            }
            oss << info->second.area << std::endl;
            count += info->second.numinst;
            area += info->second.area;
            this->printMsg(oss.str());
        }

        this->printAreaReportFooter(count, area);
        
    }

    void
    TechMapReport::printFailReportHeader() const
    {
        ostringstream oss;
        oss << "================================================\n";
        oss << "        Technology Mapping Failure Report\n";
        oss << "================================================\n";
        oss << "Module" << string(align-6, ' ');
        oss << "Instance" << string(align-8, ' ');
        oss << "Type" << string(align-4, ' ');
        oss << "\n";
        oss << "================================================\n";

        this->printMsg(oss.str());
    }

    void
    TechMapReport::printFailReportFooter() const
    {
        ostringstream oss;
        oss << "================================================\n";
        this->printMsg(oss.str());
    }    

    void
    TechMapReport::printAreaReportHeader() const
    {
        ostringstream oss;
        oss << "\n=================================================================\n";
        oss << "                         AREA REPORT\n";
        oss << "=================================================================\n";
        oss << "Cell" << string(align-4, ' ');
        oss << "Type" << string(align-4, ' ');
        oss << "Count" << string(align-5, ' ');
        oss << "Area" << string(align-4, ' ') << "\n";
        oss << "=================================================================\n";
        this->printMsg(oss.str());
    }

    void
    TechMapReport::printAreaReportFooter(int count, float area) const
    {
        ostringstream oss;
        oss << "=================================================================\n";
        oss << "Total" << string(align-5, ' ');
        oss << "-" << string(align-1, ' ');
        oss << count;
        oss << string(66 + 3*align - oss.str().size(), ' ');
        oss << area;
        oss << "\n\n";
        this->printMsg(oss.str());
    }


    void
    TechMapReport::printMsg(const string& msg) const
    {
        Synth::SyMsgHandler::instance()->print(msg);
    }    

}
