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

#include "cellsemcheckmgr.h"
#include "cellsemcheck.h"

#include "libraryreader.h"

using std::make_pair;
using std::list;
namespace stview
{
    CellSemCheckMgr* CellSemCheckMgr::cellSemCheckMgr = 0;

    CellSemCheckMgr::CellSemCheckMgr() : status(true){
        init();
    }

    CellSemCheckMgr::~CellSemCheckMgr() {
        for(CellSemCheckers::const_iterator checker = checkers.begin();
            checker != checkers.end(); ++checker) {
            delete checker->second;
        }    
    }

    CellSemCheckMgr*
    CellSemCheckMgr::instance()
    {
        if(!cellSemCheckMgr) {
            cellSemCheckMgr = new CellSemCheckMgr();
        }
        return cellSemCheckMgr;
    }

    bool
    CellSemCheckMgr::isOk() const {
        return status;
    }    

    void
    CellSemCheckMgr::init()
    {
        checkers.insert(make_pair(SCell::BUF, new BufSemCheck(0)));
        checkers.insert(make_pair(SCell::TRIBUF, new TriBufSemCheck(0)));
        checkers.insert(make_pair(SCell::NOT, new InvSemCheck(0)));
        checkers.insert(make_pair(SCell::AND, new AndSemCheck(0)));
        checkers.insert(make_pair(SCell::OR, new OrSemCheck(0)));
        checkers.insert(make_pair(SCell::NAND, new NandSemCheck(0)));
        checkers.insert(make_pair(SCell::NOR, new NorSemCheck(0)));
        checkers.insert(make_pair(SCell::XOR, new XorSemCheck(0)));
        checkers.insert(make_pair(SCell::XNOR, new XnorSemCheck(0)));
        checkers.insert(make_pair(SCell::ENC, new EncSemCheck(0)));
        checkers.insert(make_pair(SCell::DEC, new DecSemCheck(0)));
        checkers.insert(make_pair(SCell::MUX, new MuxSemCheck(0)));
        checkers.insert(make_pair(SCell::ADD, new AddSemCheck(0)));
        checkers.insert(make_pair(SCell::SUB, new SubSemCheck(0)));
        checkers.insert(make_pair(SCell::MUL, new MulSemCheck(0)));
        checkers.insert(make_pair(SCell::FF, new FFSemCheck(0)));
        checkers.insert(make_pair(SCell::RFF, new RFFSemCheck(0)));
        checkers.insert(make_pair(SCell::SFF, new SFFSemCheck(0)));
        checkers.insert(make_pair(SCell::RSFF, new RSFFSemCheck(0)));
        checkers.insert(make_pair(SCell::SRFF, new SRFFSemCheck(0)));
        checkers.insert(make_pair(SCell::LD, new LDSemCheck(0)));
        checkers.insert(make_pair(SCell::LSH, new LShiftSemCheck(0)));
        checkers.insert(make_pair(SCell::RSH, new RShiftSemCheck(0)));
        checkers.insert(make_pair(SCell::INC, new IncSemCheck(0)));
        checkers.insert(make_pair(SCell::VCC, new VccSemCheck(0)));
        checkers.insert(make_pair(SCell::GND, new GndSemCheck(0)));
        checkers.insert(make_pair(SCell::CMP, new CmpSemCheck(0)));
    }

    bool
    CellSemCheckMgr::doChecks()
    {
        status = true;
        list<SCell*>* cells = LibraryReader::instance()->getCells();
        if(!cells) {
            return status = false;
        }    

        for(list<SCell*>::const_iterator cell = cells->begin();
            cell != cells->end(); ++cell)
        {
            CellSemCheckers::iterator iter = checkers.find((*cell)->getCellType());
            if(iter == checkers.end()) {
                continue;
            }
            CellSemCheck *csc = iter->second;
            csc->setCell(*cell);
            csc->setAttribs();
            if(!csc->doChecks()) {
                status = false;
            }    
        }
        return status;
    }
}
