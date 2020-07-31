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

#include "sutils2.h"
#include "sinst.h"
#include "snet.h"
#include "sblock.h"
#include "scellmgr.h"

#include "namemgr.h"
using Synth::SyScPort;
using Synth::BlockNameManager;

using std::string;
using std::list;

const char *DATAIN        = "DATAIN";
const char *DATAOUT        = "DATAOUT";
const char *ENABLE        = "ENABLE";
const char *SELECT         = "SELECT";
const char *CIN            = "CIN";
const char *COUT        = "COUT";
const char *CLOCK        = "CLOCK";
const char *RESET        = "RESET";
const char *SET            = "SET";
const char *SHIFT        = "SHIFT";
const char *SHIFTVAL     = "SHIFTVAL";
const char *EQ            = "EQ";
const char *UEQ            = "UEQ";
const char *GT            = "GT";
const char *LT            = "LT";

namespace stview
{
    SPort::Dir    GetDir(SyScPort::SyDirType dir)
    {
        switch (dir)
        {
            case SyScPort::IN: return SPort::IN;
            case SyScPort::OUT: return SPort::OUT;
            case SyScPort::INOUT: return SPort::INOUT;
        }
        return SPort::UNSET;
    }

    SInst* CreateInstance(SBlock *block, SCell::CellType type, 
        const string& hint, int insize, bool generic)
    {
        const SCell *cell = generic ? SCellMgr::instance()->
            getGenericCell(type, insize) : SCellMgr::instance()->getTechCell(type, insize);
        string name = BlockNameManager::instance()->createName(block, hint);

        SInst *inst = new SInst(name, cell);
        const list<SPort*>& ports = cell->getPorts();
        for(list<SPort*>::const_iterator port = ports.begin();
            port != ports.end(); ++port)
        {    
            SPin *pin = new SPin((*port)->getName(), 0, inst);
            inst->addPin(pin);
        }    
        block->addInst(inst);

        return inst;
    }

    SInst* CreateInstance(SBlock *block, const SCell *cell, const string& hint)
    {
        string name = BlockNameManager::instance()->createName(block, hint);

        SInst *inst = new SInst(name, cell);
        const list<SPort*>& ports = cell->getPorts();
        for(list<SPort*>::const_iterator port = ports.begin();
            port != ports.end(); ++port)
        {    
            SPin *pin = new SPin((*port)->getName(), 0, inst);
            inst->addPin(pin);
        }    
        block->addInst(inst);

        return inst;
    }
    SNet* CreateNet(SBlock *block, const string& hint)
    {
        string name = BlockNameManager::instance()->createName(block, hint);
        SNet *net = new SNet(name);
        block->addNet(net);
        return net;
    }    

    string CellType2String(SCell::CellType t)
    {
         switch(t)
         {
             case SCell::GND    : return "GND";
             case SCell::VCC    : return "VCC";
             case SCell::BUF    : return "BUF";
             case SCell::TRIBUF    : return "TRIBUF";
             case SCell::NOT    : return "NOT";
             case SCell::AND    : return "AND";
             case SCell::NAND    : return "NAND";
             case SCell::OR        : return "OR";
             case SCell::NOR    : return "NOR";
             case SCell::XOR    : return "XOR";
             case SCell::XNOR    : return "XNOR";
             case SCell::MUX    : return "MUX";
             case SCell::FF        : return "FF";
             case SCell::RFF    : return "RFF";
             case SCell::SFF    : return "SFF";
             case SCell::RSFF    : return "RSFF";
             case SCell::LD        : return "LD";
             case SCell::RLD    : return "RLD";
             case SCell::ADD    : return "ADD";
             case SCell::SUB    : return "SUB";
             case SCell::MUL    : return "MUL";
             case SCell::DIV    : return "DIV";
             case SCell::CMP    : return "CMP";
             case SCell::LSH    : return "LSH";
             case SCell::RSH    : return "RSH";
             case SCell::ENC    : return "ENC";
             case SCell::DEC    : return "DEC";
             case SCell::INC    : return "INC";
             default            : return "";
         }
         return "";  //not reachable
    }
}
