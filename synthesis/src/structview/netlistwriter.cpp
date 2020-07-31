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
#include <set>
#include <assert.h>

#include "netlistwriter.h"
#include "symsghandler.h"
#include "scell.h"
#include "sblock.h"
#include "sport.h"
#include "snet.h"
#include "spin.h"
#include "sinst.h"
#include "sblockinst.h"
#include "sroot.h"

#include "sassign.h"
#include "sexprtree.h"

static const short INDWIDTH = 4;
static const short LINEWIDTH = 80;

using std::string;
using std::set;
using std::ofstream;
using std::ostringstream;
using std::pair;
using std::vector;
using std::list;
using std::cout;
using Synth::SyMsgHandler;

namespace stview
{

    NetlistWriter::NetlistWriter(const string& filename) :
        col(0), indwidth(INDWIDTH),
        linewidth(LINEWIDTH), indent(0)
    {
        outfile = new ofstream();
        outfile->open(filename.c_str(), std::ios_base::trunc);

        if(outfile->bad())
        {
            delete outfile;
            outfile = 0;
            SyMsgHandler::instance()->print(6, filename.c_str());
        }
    }

    NetlistWriter::~NetlistWriter() 
    {
        if(outfile){
            outfile->close();
            delete outfile;
        }
    }
 
    void
    NetlistWriter::write()
    {
        const list<SBlock*> & blocks = SRoot::instance()->getBlocks();

        for(list<SBlock*>::const_iterator block = blocks.begin();
            block != blocks.end(); ++block) {
            (*block)->accept(this);
        }    
    }

    void
    NetlistWriter::visit(SBlock *block)
    {
        print("module ");
        print(block->getName());
        print("(");
        set<string, cmp_str> portNames;
        //module header goes here
        list<SPort*> tmpp = block->getInPorts();
        copy(block->getOutPorts().begin(), block->getOutPorts().end(), 
            back_inserter(tmpp));
        copy(block->getInOutPorts().begin(), block->getInOutPorts().end(), 
            back_inserter(tmpp));

        for(list<SPort*>::const_iterator port = tmpp.begin();
            port != tmpp.end(); ++port) {
            portNames.insert((*port)->getIxName().getName());
        }
        set<string, cmp_str>::const_iterator portName = portNames.begin();
        if(portName != portNames.end())
        {
            print(*portName);
            ++portName;
        }    
        while(portName != portNames.end())
        {
            print(", ");
            print(*portName);
            ++portName;
        }    
        print(");");
        print("\n");
        vector<string> ports = this->getDeclStrings<SPort>(
            block->getInPorts(), true);
        vector<string> tmp = this->getDeclStrings<SPort>(
            block->getOutPorts(), true);
        copy(tmp.begin(), tmp.end(), back_inserter(ports));
        tmp = this->getDeclStrings<SPort>(block->getInOutPorts(), true);
        copy(tmp.begin(), tmp.end(), back_inserter(ports));
        
        for(vector<string>::const_iterator port = ports.begin(); 
            port != ports.end(); ++port) {
            print(*port + string(";\n"));
        }    
        
        vector<string> nets = this->getDeclStrings<SNet>(block->getNets(), false);
        for(vector<string>::const_iterator net = nets.begin(); 
            net != nets.end(); ++net) {
            print(*net + string(";\n"));
        }    

        incIndent();

        const list<SInst*>& insts = block->getInsts();
        for(list<SInst*>::const_iterator inst = insts.begin();
            inst != insts.end(); ++inst) {
            (*inst)->accept(this);
        }    
        const list<SBlockInst*>& binsts = block->getBlockInsts();
        for(list<SBlockInst*>::const_iterator binst = binsts.begin();
            binst != binsts.end(); ++binst) {
            (*binst)->accept(this);
        }    

        decIndent();
		//Write out assign statements, if any. 
		//For internal use only
		const SBlock::AssignStmts& stmts = block->getAssigns();
		for(SBlock::AssignStmts::const_iterator s = stmts.begin();
		    s != stmts.end(); ++s)
		{
		    print("assign ");
			print((*s)->getLVal()->getIxName().toString());
			print(" = ");
			print((*s)->getRExpr()->toString());
			print(";\n");
		}
        decIndent();
        print("endmodule\n\n");
    }

    void
    NetlistWriter::visit(SCell* cell)
    {
        const list<SPort*>& ports = cell->getPorts();
        CntvInfoVector *vec = GetCntvInfo<SPort>(ports);

        print("module ");
        print(cell->getName());
        print("(");
        if(vec && !vec->empty()) 
        {
            CntvInfoVector::const_iterator cntvInfo = vec->begin();
            print(cntvInfo->name);
            ++cntvInfo;
            while(cntvInfo != vec->end())
            {
                print(", ");
                print(cntvInfo->name);
                ++cntvInfo;
            }
        }
        print(");\n");

        vector<string> portNames = 
            this->getDeclStrings<SPort>(ports, true);
        
        for(vector<string>::const_iterator portName = portNames.begin(); 
            portName != portNames.end(); ++portName) {
            print(*portName + string(";\n"));
        }    
    }    
            

    void
    NetlistWriter::visit(SInst *cinst)
    {
        print(cinst->getCell()->getName());
        print(" ");
        print(cinst->getName());
        print("(");
        printPins(cinst);
        print(");\n");
    }
    void
    NetlistWriter::visit(SBlockInst *binst)
    {
        if(!binst->getBlock()) {
            return;
        }    
        print(binst->getBlock()->getName());
        print(" ");
        print(binst->getName());
        print("(");
        printPins(binst);
        print(");\n");
    }

    void
    NetlistWriter::printPins(const SInstBase *inst)
    {
        list<SPin*> pins = inst->getInPins();
        copy(inst->getOutPins().begin(), inst->getOutPins().end(),
            back_inserter(pins));
        copy(inst->getInOutPins().begin(), inst->getInOutPins().end(),
            back_inserter(pins));

        CntvInfoVector *vec = GetCntvInfo<SPin>(pins);
        if(!vec || vec->empty()) {
            return;
        }
        CntvInfoVector::const_iterator pinInfo = vec->begin();
        print(".");
        print(pinInfo->name);
        print("(");
        if(pinInfo->msb == -1) 
        {
            const SPin *pin = inst->getPin(pinInfo->name);
            assert(pin);
            const SCntv *act = pin->getActual();
            if(act) {
                print(act->getIxName().toString());
            }
        }
        else 
        {
            print("{");
            const SPin *pin = inst->getPin(SIxName(pinInfo->name, pinInfo->msb));
            assert(pin);
            const SCntv *act = pin->getActual();
            if(act) {
                print(act->getIxName().toString());
            }
            
            for(int i = pinInfo->msb-1; i >= pinInfo->lsb; --i)
            {
                print(", ");
                const SPin *pin = inst->getPin(
                    SIxName(pinInfo->name, i));
                assert(pin);
                const SCntv *act = pin->getActual();
                if(act) {
                    print(act->getIxName().toString());
                }
            }
            print("}");
        }    
        print(")");
        ++pinInfo;

        while(pinInfo != vec->end())
        {
            print(", ");
            print(".");
            print(pinInfo->name);
            print("(");
            if(pinInfo->msb == -1) 
            {
                const SPin *pin = inst->getPin(pinInfo->name);
                assert(pin);
                const SCntv *act = pin->getActual();
                if(act) {
                    print(act->getIxName().toString());
                }
            }
            else 
            {
                print("{");
                const SPin *pin = inst->getPin(SIxName(pinInfo->name, pinInfo->msb));
                assert(pin);
                const SCntv *act = pin->getActual();
                if(act) {
                    print(act->getIxName().toString());
                }
            
                for(int i = pinInfo->msb-1; i >= pinInfo->lsb; --i)
                {
                    print(", ");
                    const SPin *pin = inst->getPin(
                        SIxName(pinInfo->name, i));
                    assert(pin);
                    const SCntv *act = pin->getActual();
                    if(act) {
                        print(act->getIxName().toString());
                    }
                }
                print("}");
            }    
            print(")");
            ++pinInfo;
        }
        delete vec;

    }

    void
    NetlistWriter::printInd()
    {
        if((indent*indwidth ) <= col){
            return;
        }
        if(outfile){
            (*outfile) << string(indent*indwidth, ' ');
        }
        else {
            cout << string(indent*indwidth, ' ');
        }
        col += indent*indwidth;
    }

    void
    NetlistWriter::printLine()
    {
        if(outfile){
            (*outfile) << "\n";
        }
        else {
            cout << "\n";
        }
        col = 0;
    }

    void
    NetlistWriter::print(const string& str)
    {
        this->printInd();
        if((col + str.size()) > (unsigned int)linewidth)
        {
            printLine();
            printInd();
        }
        if(outfile){
            (*outfile) << str;
        }
        else {
            cout << str;
        }
        col += str.size();
    }

    void
    NetlistWriter::incIndent(short count){
        indent += count;
    }

    void
    NetlistWriter::decIndent(short count)
    {
        indent -= count;
        if(indent < 0){
            indent = 0;
        }
    }

    string
    NetlistWriter::getDirectionString(const SPort *port) const
    {
        switch(port->getDir())
        {
            case SPort::IN: return "input";
            case SPort::OUT: return "output";
            case SPort::INOUT: return "inout";
            default: assert(0);
            
        }
        assert(0);
        return "";
    }

    template <class T> vector<string>
    NetlistWriter::getDeclStrings(const list<T*>& nps, bool isport) const
    {
        vector<string> doneNP;
        int big = -1;
        int small = -1;
        string name;
        SCntv *last= 0;

        for(typename list<T*>::const_iterator np = nps.begin();
            np != nps.end(); ++np)
        {
            SCntv *cntv = *np;
             if(cntv->isVecBit())
             { 
                 if(name.empty()) 
                {
                    SIxName ixname = cntv->getIxName();
                    name = ixname.getName();
                    big = ixname.getIndex();
                    small = big;
                }     
                else 
                {
                    SIxName ixname = cntv->getIxName();
                    if(name != ixname.getName())
                    {
                        ostringstream str;
                        if(isport) {
                            str << getDirectionString(
                            dynamic_cast<SPort*>(last));
                        }
                        else {
                            str << "wire ";
                        }    
                        str << " [" << big << ":" << small << "] ";
                        str << name;
                        doneNP.push_back(str.str());

                         //new port has arrived
                        name = ixname.getName();
                        big = ixname.getIndex();
                        small = big;
                     } 
                     else 
                     {
                        int index = ixname.getIndex();
                        if(index > big) {
                            big = index;
                        }
                        else if(index < small) {
                             small = index;
                        }     

                     }
                 }     
             }
             else
             {
                 if(!name.empty()) 
                 {
                     ostringstream str;
                     if(isport) {
                         str << getDirectionString(
                             dynamic_cast<SPort*>(last));
                     } 
                     else {
                         str << "wire ";
                     } 
                     str << " [" << big << ":" << small << "] ";
                     str << name;
                     doneNP.push_back(str.str());
                 }     
                 name.clear();
                 ostringstream str;
                 if(isport) {
                     str << getDirectionString(dynamic_cast<SPort*>(cntv));
                 }
                 else {
                     str << "wire ";
                 }     
                 str <<  " " << cntv->getName();
                 doneNP.push_back(str.str());
             }
             last = cntv;
        }
        if(!name.empty())
        {
            ostringstream str;
            if(isport) {
                str << getDirectionString(
                dynamic_cast<SPort*>(last));
            }
            else {
                str << "wire ";
            }    
            str << " [" << big << ":" << small << "] ";
            str << name;
            doneNP.push_back(str.str());
        }    
        return doneNP;
    }



}
