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

#include "cellmgr.h"
#include "synthcell.h"
using std::map;
using std::string;
using std::make_pair;
namespace Synth
{
    CellMgr* CellMgr::cellmgr = 0;

    CellMgr::CellMgr(){
        synthCells.insert(make_pair(GND, new SynthGND()));
        synthCells.insert(make_pair(VCC, new SynthVCC()));
        synthCells.insert(make_pair(BUF, new SynthBUF()));
        synthCells.insert(make_pair(NOT, new SynthNOT()));
        synthCells.insert(make_pair(AND, new SynthAND()));
        synthCells.insert(make_pair(NAND, new SynthNAND()));
        synthCells.insert(make_pair(OR, new SynthOR()));
        synthCells.insert(make_pair(NOR, new SynthNOR()));
        synthCells.insert(make_pair(XOR, new SynthXOR()));
        synthCells.insert(make_pair(XNOR, new SynthXNOR()));
        synthCells.insert(make_pair(MUX, new SynthMUX()));
        synthCells.insert(make_pair(FF, new SynthFF()));
        synthCells.insert(make_pair(FFR, new SynthFFR()));
        synthCells.insert(make_pair(LD, new SynthLD()));
        synthCells.insert(make_pair(LDR, new SynthLDR()));
        synthCells.insert(make_pair(ADD, new SynthADD()));
        synthCells.insert(make_pair(SUB, new SynthSUB()));
        synthCells.insert(make_pair(MUL, new SynthMUL()));
        synthCells.insert(make_pair(DIV, new SynthDIV()));
        synthCells.insert(make_pair(CMP, new SynthCMP()));
        synthCells.insert(make_pair(LSH, new SynthLSH()));
        synthCells.insert(make_pair(RSH, new SynthRSH()));
        synthCells.insert(make_pair(ENC, new SynthENC()));
        synthCells.insert(make_pair(DEC, new SynthDEC()));

    }

    CellMgr::~CellMgr() {
        cellmgr = 0;
    }

    CellMgr*
        CellMgr::instance()
        {
            if(!cellmgr){
                cellmgr = new CellMgr();
            }
            return cellmgr;
        }

    const SynthCELL*
        CellMgr::create(
                CellType type,
                const vector<int>& args)
        {
            map<CellType, const SynthCELL*>::const_iterator iter = 
                synthCells.find(type);

            if(iter == synthCells.end()){
                return 0;
            }

            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }

            string name = iter->second->getName(tmp);
            map<string, const SynthCELL*, cmp_str>::const_iterator nameCell =
                existCells.find(name);
            if(nameCell != existCells.end()){
                return nameCell->second;
            }
            SynthCELL *cell = iter->second->create(args);
            existCells.insert(make_pair(name, cell));
            return cell;
        }
}
