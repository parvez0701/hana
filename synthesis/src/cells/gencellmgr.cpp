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
#include <assert.h>
#include "gencellmgr.h"
using std::map;
using std::string;
using std::vector;
using std::make_pair;
using std::pair;
using std::ostringstream;
namespace Synth
{

    GenCellMgr* GenCellMgr::cellMgr = 0;

    GenCellMgr::GenCellMgr()
    {
        cellTable.insert(make_pair(GND,        new GenCell<GND>()));
        cellTable.insert(make_pair(VCC,     new GenCell<VCC>()));
        cellTable.insert(make_pair(BUF,     new GenCell<BUF>()));
        cellTable.insert(make_pair(NOT,     new GenCell<NOT>()));
        cellTable.insert(make_pair(AND,     new GenCell<AND>()));
        cellTable.insert(make_pair(NAND,     new GenCell<NAND>()));
        cellTable.insert(make_pair(OR,         new GenCell<OR>()));
        cellTable.insert(make_pair(NOR,     new GenCell<NOR>()));
        cellTable.insert(make_pair(XOR,     new GenCell<XOR>()));
        cellTable.insert(make_pair(XNOR,     new GenCell<XNOR>()));
        cellTable.insert(make_pair(MUX,     new GenCell<MUX>()));
        cellTable.insert(make_pair(FF,         new GenCell<FF>()));
        cellTable.insert(make_pair(FFR,     new GenCell<FFR>()));
        cellTable.insert(make_pair(FFS,     new GenCell<FFS>()));
        cellTable.insert(make_pair(FFRS,     new GenCell<FFRS>()));
        cellTable.insert(make_pair(FFSR,     new GenCell<FFSR>()));
        cellTable.insert(make_pair(LD,         new GenCell<LD>()));
        cellTable.insert(make_pair(LDR,     new GenCell<LDR>()));
        cellTable.insert(make_pair(ADD,     new GenCell<ADD>()));
        cellTable.insert(make_pair(SUB,     new GenCell<SUB>()));
        cellTable.insert(make_pair(MUL,     new GenCell<MUL>()));
        cellTable.insert(make_pair(DIV,     new GenCell<DIV>()));
        cellTable.insert(make_pair(CMP,     new GenCell<CMP>()));
        cellTable.insert(make_pair(SHIFTER,    new GenCell<SHIFTER>()));
        //cellTable.insert(make_pair(RSH,     new GenCell<RSH>()));
        cellTable.insert(make_pair(ENC,     new GenCell<ENC>()));
        cellTable.insert(make_pair(DEC,     new GenCell<DEC>()));
        cellTable.insert(make_pair(INC,     new GenCell<INC>()));
        cellTable.insert(make_pair(TRIBUF,     new GenCell<TRIBUF>()));
    }


    GenCellMgr::~GenCellMgr()
    {
        GenCellMgr::cellMgr = 0;
    }

    GenCellMgr*
    GenCellMgr::instance() 
    {
        if(!GenCellMgr::cellMgr) {
            GenCellMgr::cellMgr = new GenCellMgr();
        }
        return GenCellMgr::cellMgr;
    }    

    GenCellBase*
    GenCellMgr::create(CellType type, short outsize, short insize)
    {
        CellTable::iterator iter = cellTable.find(type);
        assert(iter != cellTable.end());
        string cellName = iter->second->createName(insize, outsize);

        Name2CellTable::iterator ntiter = name2CellTable.find(cellName);
        if(ntiter != name2CellTable.end()){
            return ntiter->second;
        }    
        
        pair<vector<string>,  vector<string> > inOutNames = 
            this->createInputOutputPortNames(type, insize);
        
        GenCellBase *cellBase = iter->second->create(    cellName,
                                                        inOutNames.first,
                                                        inOutNames.second);
        
        name2CellTable.insert(make_pair(cellName, cellBase));
        return cellBase;
    }    
            

    pair<vector<string>, vector<string> >
    GenCellMgr::createInputOutputPortNames(CellType type, short size)
    {
        vector<string> inputs;
        vector<string> outputs;
        switch(type)
        {
            //Size doesn't matter for the following.
            //case GND:
            //{
            //    inputs.push_back("IN");
            //    break;
            //}
            case VCC:
            case GND:
            {
                outputs.push_back("OUT");
                break;
            }
            case BUF:
            case NOT:
            {
                inputs.push_back("IN");
                outputs.push_back("OUT");
                break;
            }
            case FF:
            {
                inputs.push_back("D");
                inputs.push_back("CLK");
                outputs.push_back("Q");
                break;
            }
            case FFR:
            {
                inputs.push_back("D");
                inputs.push_back("CLK");
                inputs.push_back("RST");
                outputs.push_back("Q");
                break;
            }
            case FFS:
            {
                inputs.push_back("D");
                inputs.push_back("CLK");
                inputs.push_back("SET");
                outputs.push_back("Q");
                break;
            }
            case FFRS:
            {
                inputs.push_back("D");
                inputs.push_back("CLK");
                inputs.push_back("RST");
                inputs.push_back("SET");
                outputs.push_back("Q");
                break;
            }
            case FFSR:
            {
                inputs.push_back("D");
                inputs.push_back("CLK");
                inputs.push_back("RST");
                inputs.push_back("SET");
                outputs.push_back("Q");
                break;
            }
            case LD:
            {
                inputs.push_back("D");
                inputs.push_back("EN");
                outputs.push_back("Q");
                break;
            }
            case LDR:
            {
                inputs.push_back("D");
                inputs.push_back("EN");
                inputs.push_back("RST");
                outputs.push_back("Q");
                break;
            }
            case TRIBUF:
            {
                inputs.push_back("IN");
                inputs.push_back("EN");
                outputs.push_back("OUT");
                break;
            }    
            //Now size matters
            case AND:
            case NAND:
            case OR:
            case NOR:
            case XOR:
            case XNOR:
            {
                assert(size > 0 && size < 5);
                outputs.push_back("OUT");
                inputs.push_back(this->createVectorPortName("IN", size-1, 0));
                break;
            }    
            case MUX:
            {
                size = cellTable[type]->getActualInputSize(type, size);
                outputs.push_back("OUT");
                inputs.push_back(this->createVectorPortName("IN", size-1, 0));
                size -= 1;
                int selSize = 0;
                if(size == 0){
                    selSize = 1;
                }    
                while(size)
                {
                    //ostringstream str;
                    //str << "SEL" << i++;
                    //inputs.push_back(str.str());
                    size = size >> 1;
                    ++selSize;
                }
                inputs.push_back(this->createVectorPortName("SEL", selSize-1, 0));
                break;
            }
            case ADD:
            case SUB:
            case MUL:
            case DIV:
            case CMP:
            {
                size = cellTable[type]->getActualInputSize(type, size);
                inputs.push_back(this->createVectorPortName("A", size-1, 0));
                inputs.push_back(this->createVectorPortName("B", size-1, 0));
                short outsize =  (type==MUL) ? 2 * size : (type==CMP) ? 2 :size;
                if(type == ADD) {
                    outputs.push_back("COUT");
                }    
                if(type == SUB) {
                    outputs.push_back("BOUT");
                }    
                if(type == DIV) {
                    outputs.push_back(this->createVectorPortName("REM", outsize-1, 0));
                }
                if(type != CMP){
                    outputs.push_back(this->createVectorPortName("C", outsize-1, 0));
                }
                else 
                {
                    outputs.push_back("EQ");
                    outputs.push_back("INEQ");
                    outputs.push_back("GT");
                    outputs.push_back("LT");
                }    


                if(type == ADD) {
                    inputs.push_back("CIN");
                }
                else if(type == SUB) {
                    inputs.push_back("BIN");
                }
                break;
            }

            case SHIFTER:
            //case RSH :
            {
                size = cellTable[type]->getActualInputSize(type, size);
                inputs.push_back(this->createVectorPortName("IN", size-1, 0));
                outputs.push_back(this->createVectorPortName("OUT", size-1, 0));
                inputs.push_back(this->createVectorPortName("SHIFT", BitCount(size)-1, 0));
                inputs.push_back("OP"); //low-> left shift, high->right shift
                //for signed operation
                inputs.push_back("VAL"); //low -> fill 0, high->fill 1
                break;
            }    

            case ENC:
            {
                size = cellTable[type]->getActualInputSize(type, size);
                inputs.push_back(this->createVectorPortName("IN", size-1, 0));
                inputs.push_back("EN");
                short outsize = 0;
                while(size)
                {
                    ++outsize;
                    size = size >> 1;
                }    
                outputs.push_back(this->createVectorPortName("OUT", outsize-1, 0));
                break;
            }

            case DEC:
            {
                size = cellTable[type]->getActualInputSize(type, size);
                inputs.push_back(this->createVectorPortName("IN", size-1, 0));
                inputs.push_back("EN");
                short outsize = 1;
                for(int i = 0; i < size; ++i){
                    outsize *= 2;
                }    
                outputs.push_back(this->createVectorPortName("OUT", outsize-1, 0));
                break;
            }    

            case INC:
            {
                size = cellTable[type]->getActualInputSize(type, size);
                inputs.push_back(this->createVectorPortName("IN", size-1, 0));
                outputs.push_back("COUT");
                outputs.push_back(this->createVectorPortName("OUT", size-1, 0));
                break;
            }
            default:
                break;
        }
        return make_pair(inputs, outputs);
    }    

    string
    GenCellMgr::createVectorPortName(const string& name, short msb, short lsb) const
    {
        if((msb-lsb) == 0) {
            return name;
        }    
        ostringstream str;
        str << name << "[" << msb << ":" << lsb << "]";
        return str.str();
    }    
                
                
}
