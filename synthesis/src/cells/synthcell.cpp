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

#include "synthcell.h"
#include "syroot.h"
#include "sycontainer.h"
using std::ostringstream;
namespace Synth
{
    SynthCELL::SynthCELL(){
    }

    SynthCELL::SynthCELL(
            const SyModule *module,
            const int del,
            const int ar):
        mod(const_cast<SyModule*>(module)),
        delay(del),
        area(ar)
    {
        const SyList<SyScPort*>& ports = module->getPorts();
        inports = new SyList<SyScPort*>();
        outports = new SyList<SyScPort*>();
        inoutports = new SyList<SyScPort*>();

        for(SyNode<SyScPort*>* node = ports.head; node; node = node->next){
            addPort(node->val);
        }
        SyRoot::instance()->addCellMod(mod);
    }

    SynthCELL::~SynthCELL() {
    }

    void
        SynthCELL::addPort(const SyScPort *port)
        {
            switch(port->getDir())
            {
                case SyScPort::IN:
                    inports->push_back(const_cast<SyScPort*>(port));
                    break;
                case SyScPort::OUT:
                    outports->push_back(const_cast<SyScPort*>(port));
                    break;
                case SyScPort::INOUT:
                    inoutports->push_back(const_cast<SyScPort*>(port));
                    break;
                default:
                    break;
            }
        }

    const SyList<SyScPort*>&
        SynthCELL::getPorts() const {
            return mod->getPorts();
        }

    SyList<SyScPin*>*
        SynthCELL::getInPins(
                const SyModInst *minst) const {
            return this->getPins(minst, SyScPort::IN);
        }
    SyList<SyScPin*>*
        SynthCELL::getOutPins(
                const SyModInst *minst) const {
            return this->getPins(minst, SyScPort::OUT);
        }
    SyList<SyScPin*>*
        SynthCELL::getPins(
                const SyModInst *minst,
                const SyScPort::SyDirType dir) const
        {
            SyList<SyScPin*>* rpins = new SyList<SyScPin*>();
            const SyList<SyScPin*>& pins = minst->getPins();
            for(SyNode<SyScPin*> *node = pins.head; node; node = node->next){
                if(node->val->getDir() == dir){
                    rpins->push_back(node->val);
                }
            }
            return rpins;
        }

    SyModInst*
        SynthCELL::instantiate(
                SyModule *pmod,
                const string& hint) const
        {
            SyModInst *minst = new SyModInst(CreateUniqueName(pmod, hint), mod, mod->getName());
            pmod->addModInst(minst);
            return minst;
        }
                     

    SynthGND::SynthGND() : SynthCELL(){
    }
    SynthGND::SynthGND(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthGND::~SynthGND() {
    }

    string
        SynthGND::getName(const vector<int>& args) const{
            return "SynthGND";
        }

    SynthGND*
        SynthGND::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("o"), false, 
                        SyScPort::OUT, SyScNet::REG));
            return new SynthGND(m, args[0], args[1]);
        }

    SynthVCC::SynthVCC() : SynthCELL() {
    }
    SynthVCC::SynthVCC(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthVCC::~SynthVCC() {
    }

    string
        SynthVCC::getName(const vector<int>& args) const{
            return "SynthVCC";
        }

    SynthVCC*
        SynthVCC::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("o"), false, 
                        SyScPort::OUT, SyScNet::REG));
            return new SynthVCC(m, args[0], args[1]);
        }

    SynthBUF::SynthBUF() : SynthCELL() {
    }
    SynthBUF::SynthBUF(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthBUF::~SynthBUF() {
    }
    string
        SynthBUF::getName(const vector<int>& args) const{
            return "SynthBUF";
        }
    SynthBUF*
        SynthBUF::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("o"), false, SyScPort::OUT, 
                        SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("i0"), false, SyScPort::IN, 
                        SyScNet::WIRE));
            return new SynthBUF(m, args[0], args[1]);
        }

    SynthNOT::SynthNOT() : SynthCELL() {
    }

    SynthNOT::SynthNOT(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthNOT::~SynthNOT() {
    }
    string
        SynthNOT::getName(const vector<int>& args) const{
            return "SynthNOT";
        }

    SynthNOT*
        SynthNOT::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("o"), false, 
                        SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("i0"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            return new SynthNOT(m, args[0], args[1]);
        }


    SynthAND::SynthAND() : SynthCELL() {
    }

    SynthAND::SynthAND(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthAND::~SynthAND() {
    }
    string
        SynthAND::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthAND_" << args[0];
            return str.str();
        }
    SynthAND*
        SynthAND::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("o"), false, 
                        SyScPort::OUT, SyScNet::REG));
            const int inputs = args[2];

            for(int i = 0; i < inputs; ++i)
            {
                ostringstream str;
                str << "i" << i;
                m->addPort(new SyScPort(SyIxName(str.str()), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            return new SynthAND(m, args[0], args[1]);
        }


    SynthNAND::SynthNAND() : SynthCELL() {
    }
    SynthNAND::SynthNAND(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthNAND::~SynthNAND() {
    }

    string
        SynthNAND::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthNAND_" << args[0];
            return str.str();
        }
    SynthNAND*
        SynthNAND::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("o"), false, 
                        SyScPort::OUT, SyScNet::REG));
            const int inputs = args[2];

            for(int i = 0; i < inputs; ++i)
            {
                ostringstream str;
                str << "i" << i;
                m->addPort(new SyScPort(SyIxName(str.str()), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            return new SynthNAND(m, args[0], args[1]);
        }

    SynthOR::SynthOR() : SynthCELL() {
    }

    SynthOR::SynthOR(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthOR::~SynthOR() {
    }

    string
        SynthOR::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthOR_" << args[0];
            return str.str();
        }
    SynthOR*
        SynthOR::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("o"), false, 
                        SyScPort::OUT, SyScNet::REG));
            const int inputs = args[2];

            for(int i = 0; i < inputs; ++i)
            {
                ostringstream str;
                str << "i" << i;
                m->addPort(new SyScPort(SyIxName(str.str()), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            return new SynthOR(m, args[0], args[1]);
        }


    SynthNOR::SynthNOR() : SynthCELL() {
    }

    SynthNOR::SynthNOR(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthNOR::~SynthNOR() {
    }

    string
        SynthNOR::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthNOR_" << args[0];
            return str.str();
        }
    SynthNOR*
        SynthNOR::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("o"), false, 
                        SyScPort::OUT, SyScNet::REG));
            const int inputs = args[2];

            for(int i = 0; i < inputs; ++i)
            {
                ostringstream str;
                str << "i" << i;
                m->addPort(new SyScPort(SyIxName(str.str()), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            return new SynthNOR(m, args[0], args[1]);
        }


    SynthXOR::SynthXOR() : SynthCELL() {
    }

    SynthXOR::SynthXOR(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthXOR::~SynthXOR() {
    }

    string
        SynthXOR::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthXOR_" << args[0];
            return str.str();
        }


    SynthXOR*
        SynthXOR::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("o"), false, 
                        SyScPort::OUT, SyScNet::REG));
            const int inputs = args[2];

            for(int i = 0; i < inputs; ++i)
            {
                ostringstream str;
                str << "i" << i;
                m->addPort(new SyScPort(SyIxName(str.str()), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            return new SynthXOR(m, args[0], args[1]);
        }


    SynthXNOR::SynthXNOR() : SynthCELL() {
    }

    SynthXNOR::SynthXNOR(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthXNOR::~SynthXNOR() {
    }

    string
        SynthXNOR::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthXNOR_" << args[0];
            return str.str();
        }
    SynthXNOR*
        SynthXNOR::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("o"), false, 
                        SyScPort::OUT, SyScNet::REG));
            const int inputs = args[2];

            for(int i = 0; i < inputs; ++i)
            {
                ostringstream str;
                str << "i" << i;
                m->addPort(new SyScPort(SyIxName(str.str()), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            return new SynthXNOR(m, args[0], args[1]);
        }

    SynthMUX::SynthMUX() : SynthCELL() {
    }

    SynthMUX::SynthMUX(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthMUX::~SynthMUX() {
    }
    string
        SynthMUX::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthMUX_" << args[0];
            return str.str();
        }
    SynthMUX*
        SynthMUX::create(const vector<int>& args) const
        {
            vector<int> ttmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                ttmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(ttmp));
            m->addPort(new SyScPort(SyIxName("o"), false, 
                        SyScPort::OUT, SyScNet::REG));
            const int inputs = args[2];

            if(inputs == 0){
                m->addPort(new SyScPort(SyIxName("in"), false, SyScPort::IN,
                            SyScNet::WIRE));
            }
            else
            {
                SyVPort *in = new SyVPort(
                    "in", inputs-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("in", in);

                for(int i = inputs-1; i >= 0; ++i){
                    const SyScPort *bit = in->getBit(i);
                    m->addPort(bit);
                }
            }
            int msb = 0;
            int tmp = inputs;
            while(tmp)
            {
                ++msb;
                tmp = tmp >> 1;
            }
            --msb;
            if(msb == 0){
                m->addPort(new SyScPort(SyIxName("sel"), false, SyScPort::IN,
                            SyScNet::WIRE));
            }
            else
            {
                SyVPort *sel = new SyVPort(
                    "sel", msb, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("sel", sel);

                for(int i = msb; i >= 0; ++i){
                    const SyScPort *bit = sel->getBit(i);
                    m->addPort(bit);
                }
            }

            return new SynthMUX(m, args[0], args[1]);
        }

    SynthFF::SynthFF() : SynthCELL() {
    }

    SynthFF::SynthFF(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthFF::~SynthFF() {
    }

    string
        SynthFF::getName(const vector<int>& args) const{
            return "SynthFF";
        }
    SynthFF*
        SynthFF::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("q"), false, 
                        SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("d"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            m->addPort(new SyScPort(SyIxName("clk"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            return new SynthFF(m, args[0], args[1]);
        }

    SynthFFR::SynthFFR() : SynthCELL() {
    }

    SynthFFR::SynthFFR(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthFFR::~SynthFFR() {
    }

    string
        SynthFFR::getName(const vector<int>& args) const{
            return "SynthFFR";
        }
    SynthFFR*
        SynthFFR::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("q"), false, 
                        SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("d"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            m->addPort(new SyScPort(SyIxName("clk"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            m->addPort(new SyScPort(SyIxName("reset"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            return new SynthFFR(m, args[0], args[1]);
        }


    SynthLD::SynthLD() : SynthCELL() {
    }

    SynthLD::SynthLD(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthLD::~SynthLD() {
    }

    string
        SynthLD::getName(const vector<int>& args) const{
            return "SynthLD";
        }
    SynthLD*
        SynthLD::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("q"), false, 
                        SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("d"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            m->addPort(new SyScPort(SyIxName("enable"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            return new SynthLD(m, args[0], args[1]);
        }

    
    SynthLDR::SynthLDR() : SynthCELL() {
    }
    SynthLDR::SynthLDR(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthLDR::~SynthLDR() {
    }

    string
        SynthLDR::getName(const vector<int>& args) const{
            return "SynthLDR";
        }
    SynthLDR*
        SynthLDR::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("q"), false, 
                        SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("d"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            m->addPort(new SyScPort(SyIxName("enable"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            return new SynthLDR(m, args[0], args[1]);
        }


    SynthADD::SynthADD() : SynthCELL() {
    }

    SynthADD::SynthADD(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthADD::~SynthADD() {
    }

    string
        SynthADD::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthADD_" << args[0];
            return str.str();
        }
    SynthADD*
        SynthADD::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("cout"), false, 
                        SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("cin"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            const int size = args[2];
            if(size == 1)
            {
                m->addPort(new SyScPort(SyIxName("i0"), false, 
                        SyScPort::IN, SyScNet::WIRE));
                m->addPort(new SyScPort(SyIxName("i1"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *in0 = new SyVPort(
                    "i0", size-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i0", in0);

                for(int i = size-1; i >= 0; ++i){
                    const SyScPort *bit = in0->getBit(i);
                    m->addPort(bit);
                }

                SyVPort *in1 = new SyVPort(
                    "i1", size-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i1", in1);

                for(int i = size-1; i >= 0; ++i){
                    const SyScPort *bit = in1->getBit(i);
                    m->addPort(bit);
                }
            }
            return new SynthADD(m, args[0], args[1]);
        }

    SynthSUB::SynthSUB() : SynthCELL() {
    }

    SynthSUB::SynthSUB(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthSUB::~SynthSUB() {
    }
    string
        SynthSUB::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthSUB_" << args[0];
            return str.str();
        }

    SynthSUB*
        SynthSUB::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            m->addPort(new SyScPort(SyIxName("cout"), false, 
                        SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("cin"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            const int size = args[2];
            if(size == 1)
            {
                m->addPort(new SyScPort(SyIxName("i0"), false, 
                        SyScPort::IN, SyScNet::WIRE));
                m->addPort(new SyScPort(SyIxName("i1"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *in0 = new SyVPort(
                    "i0", size-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i0", in0);

                for(int i = size-1; i >= 0; ++i){
                    const SyScPort *bit = in0->getBit(i);
                    m->addPort(bit);
                }

                SyVPort *in1 = new SyVPort(
                    "i1", size-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i1", in1);

                for(int i = size-1; i >= 0; ++i){
                    const SyScPort *bit = in1->getBit(i);
                    m->addPort(bit);
                }
            }
            return new SynthSUB(m, args[0], args[1]);
        }


    SynthMUL::SynthMUL() : SynthCELL() {
    }

    SynthMUL::SynthMUL(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthMUL::~SynthMUL() {
    }

    string
        SynthMUL::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthMUL_" << args[0];
            return str.str();
        }
    SynthMUL*
        SynthMUL::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            const int size1 = args[2];
            const int size2 = args[3];
            if(size1 == 1) {
                m->addPort(new SyScPort(SyIxName("i0"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *in0 = new SyVPort(
                    "i0", size1-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i0", in0);

                for(int i = size1-1; i >= 0; ++i){
                    const SyScPort *bit = in0->getBit(i);
                    m->addPort(bit);
                }

            }
            if(size2 == 1)
            {
                m->addPort(new SyScPort(SyIxName("i1"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *in1 = new SyVPort(
                    "i1", size2-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i1", in1);

                for(int i = size2-1; i >= 0; ++i){
                    const SyScPort *bit = in1->getBit(i);
                    m->addPort(bit);
                }
            }
            SyVPort *res = new SyVPort(
                "res", size1+size2-1, 0, false, SyScPort::IN, SyScNet::WIRE);
            m->addObject("res", res);

            for(int i = size1+size2-1; i >= 0; ++i){
                const SyScPort *bit = res->getBit(i);
                m->addPort(bit);
            }
            return new SynthMUL(m, args[0], args[1]);
        }

    SynthDIV::SynthDIV(): SynthCELL() {
    }

    SynthDIV::SynthDIV(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthDIV::~SynthDIV() {
    }

    string
        SynthDIV::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthDIV_" << args[0];
            return str.str();
        }
    SynthDIV*
        SynthDIV::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            const int size1 = args[2];
            const int size2 = args[3];
            if(size1 == 1) {
                m->addPort(new SyScPort(SyIxName("i0"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *in0 = new SyVPort(
                    "i0", size1-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i0", in0);

                for(int i = size1-1; i >= 0; ++i){
                    const SyScPort *bit = in0->getBit(i);
                    m->addPort(bit);
                }

            }
            if(size2 == 1)
            {
                m->addPort(new SyScPort(SyIxName("i1"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *in1 = new SyVPort(
                    "i1", size2-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i1", in1);

                for(int i = size2-1; i >= 0; ++i){
                    const SyScPort *bit = in1->getBit(i);
                    m->addPort(bit);
                }
            }
            const int size = ((size1 > size2) ? size1 : size2);
            SyVPort *res = new SyVPort(
                "res", size-1, 0, false, SyScPort::IN, SyScNet::WIRE);
            m->addObject("res", res);

            for(int i = size-1; i >= 0; ++i){
                const SyScPort *bit = res->getBit(i);
                m->addPort(bit);
            }
            return new SynthDIV(m, args[0], args[1]);
        }

    SynthCMP::SynthCMP() : SynthCELL() {
    }

    SynthCMP::SynthCMP(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthCMP::~SynthCMP() {
    }

    string
        SynthCMP::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthCMP_" << args[0];
            return str.str();
        }
    SynthCMP*
        SynthCMP::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            const int size1 = args[2];
            const int size2 = args[3];
            if(size1 == 1) {
                m->addPort(new SyScPort(SyIxName("i0"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *in0 = new SyVPort(
                    "i0", size1-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i0", in0);

                for(int i = size1-1; i >= 0; ++i){
                    const SyScPort *bit = in0->getBit(i);
                    m->addPort(bit);
                }

            }
            if(size2 == 1)
            {
                m->addPort(new SyScPort(SyIxName("i1"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *in1 = new SyVPort(
                    "i1", size2-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i1", in1);

                for(int i = size2-1; i >= 0; ++i){
                    const SyScPort *bit = in1->getBit(i);
                    m->addPort(bit);
                }
            }
            m->addPort(new SyScPort(SyIxName("eq"), false, 
                    SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("gt"), false, 
                    SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("lt"), false, 
                    SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("ne"), false, 
                    SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("ge"), false, 
                    SyScPort::OUT, SyScNet::REG));
            m->addPort(new SyScPort(SyIxName("le"), false, 
                    SyScPort::OUT, SyScNet::REG));
            return new SynthCMP(m, args[0], args[1]);
        }

    SynthLSH::SynthLSH() : SynthCELL() {
    }

    SynthLSH::SynthLSH(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthLSH::~SynthLSH() {
    }

    string
        SynthLSH::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthLSH_" << args[0] << "_" << args[1];
            return str.str();
        }
    SynthLSH*
        SynthLSH::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            const int size1 = args[2];
            const int size2 = args[3];
            if(size1 == 1) 
            {
                m->addPort(new SyScPort(SyIxName("i0"), false, 
                        SyScPort::IN, SyScNet::WIRE));
                m->addPort(new SyScPort(SyIxName("res"), false, 
                        SyScPort::OUT, SyScNet::REG));
            }
            else 
            {
                SyVPort *in0 = new SyVPort(
                    "i0", size1-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i0", in0);

                for(int i = size1-1; i >= 0; ++i){
                    const SyScPort *bit = in0->getBit(i);
                    m->addPort(bit);
                }
                SyVPort *res = new SyVPort(
                    "res", size1-1, 0, false, SyScPort::IN, SyScNet::REG);
                m->addObject("res", res);

                for(int i = size1-1; i >= 0; ++i){
                    const SyScPort *bit = res->getBit(i);
                    m->addPort(bit);
                }

            }
            if(size2 == 1)
            {
                m->addPort(new SyScPort(SyIxName("i1"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *in1 = new SyVPort(
                    "i1", size2-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i1", in1);

                for(int i = size2-1; i >= 0; ++i){
                    const SyScPort *bit = in1->getBit(i);
                    m->addPort(bit);
                }
            }
            return new SynthLSH(m, args[0], args[1]);
        }

    SynthRSH::SynthRSH() : SynthCELL() {
    }

    SynthRSH::SynthRSH(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthRSH::~SynthRSH() {
    }

    string
        SynthRSH::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthRSH_" << args[0] << "_" << args[1];
            return str.str();
        }
    SynthRSH*
        SynthRSH::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            const int size1 = args[2];
            const int size2 = args[3];
            if(size1 == 1) 
            {
                m->addPort(new SyScPort(SyIxName("i0"), false, 
                        SyScPort::IN, SyScNet::WIRE));
                m->addPort(new SyScPort(SyIxName("res"), false, 
                        SyScPort::OUT, SyScNet::REG));
            }
            else 
            {
                SyVPort *in0 = new SyVPort(
                    "i0", size1-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i0", in0);

                for(int i = size1-1; i >= 0; ++i){
                    const SyScPort *bit = in0->getBit(i);
                    m->addPort(bit);
                }
                SyVPort *res = new SyVPort(
                    "res", size1-1, 0, false, SyScPort::IN, SyScNet::REG);
                m->addObject("res", res);

                for(int i = size1-1; i >= 0; ++i){
                    const SyScPort *bit = res->getBit(i);
                    m->addPort(bit);
                }

            }
            if(size2 == 1)
            {
                m->addPort(new SyScPort(SyIxName("i1"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *in1 = new SyVPort(
                    "i1", size2-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i1", in1);

                for(int i = size2-1; i >= 0; ++i){
                    const SyScPort *bit = in1->getBit(i);
                    m->addPort(bit);
                }
            }
            return new SynthRSH(m, args[0], args[1]);
        }

    SynthENC::SynthENC() : SynthCELL() {
    }

    SynthENC::SynthENC(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthENC::~SynthENC() {
    }

    string
        SynthENC::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthENC_" << args[0] << "_" << args[1];
            return str.str();
        }
    SynthENC*
        SynthENC::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            const int size1 = args[2];
            const int size2 = args[3];

            SyVPort *in0 = new SyVPort(
                "i0", size1-1, 0, false, SyScPort::IN, SyScNet::WIRE);
            m->addObject("i0", in0);

            for(int i = size1-1; i >= 0; ++i){
                const SyScPort *bit = in0->getBit(i);
                m->addPort(bit);
            }
            if(size2 == 1) 
            {
                m->addPort(new SyScPort(SyIxName("o"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *out = new SyVPort(
                    "o", size2-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("o", out);

                for(int i = size2-1; i >= 0; ++i){
                    const SyScPort *bit = in0->getBit(i);
                    m->addPort(bit);
                }

            }
            return new SynthENC(m, args[0], args[1]);
        }


    SynthDEC::SynthDEC() : SynthCELL() {
    }

    SynthDEC::SynthDEC(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthDEC::~SynthDEC() {
    }

    string
        SynthDEC::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthDEC" << args[0] << "_" << args[1];
            return str.str();
        }
    SynthDEC*
        SynthDEC::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            const int size1 = args[2];
            const int size2 = args[3];
            if(size1 == 1) 
            {
                m->addPort(new SyScPort(SyIxName("i0"), false, 
                        SyScPort::IN, SyScNet::WIRE));
            }
            else 
            {
                SyVPort *in0 = new SyVPort(
                    "i0", size1-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i0", in0);

                for(int i = size1-1; i >= 0; ++i){
                    const SyScPort *bit = in0->getBit(i);
                    m->addPort(bit);
                }

            }
            SyVPort *out = new SyVPort(
                "o", size2-1, 0, false, SyScPort::IN, SyScNet::WIRE);
            m->addObject("o", out);

            for(int i = size2-1; i >= 0; ++i){
                const SyScPort *bit = out->getBit(i);
                m->addPort(bit);
            }
            return new SynthDEC(m, args[0], args[1]);
        }

    SynthINC::SynthINC() : SynthCELL() {
    }

    SynthINC::SynthINC(
            const SyModule *module,
            const int del,
            const int ar):
        SynthCELL(module, del, ar)
    {
    }

    SynthINC::~SynthINC() {
    }

    string
        SynthINC::getName(const vector<int>& args) const{
            ostringstream str;
            str << "SynthINC_" << args[0] << "_" << args[1];
            return str.str();
        }
    SynthINC*
        SynthINC::create(const vector<int>& args) const
        {
            vector<int> tmp;
            for(unsigned int i = 2; i < args.size(); ++i){
                tmp.push_back(args[i]);
            }
            SyModule *m = new SyModule(this->getName(tmp));
            const int size = args[2];
            if(size > 1)
            {
                SyVPort *in0 = new SyVPort(
                "i0", size-1, 0, false, SyScPort::IN, SyScNet::WIRE);
                m->addObject("i0", in0);

                for(int i = size-1; i >= 0; ++i){
                    const SyScPort *bit = in0->getBit(i);
                    m->addPort(bit);
                }
                SyVPort *o = new SyVPort(
                "o", size-1, 0, false, SyScPort::OUT, SyScNet::REG);
                m->addObject("o", o);

                for(int i = size-1; i >= 0; ++i){
                    const SyScPort *bit = o->getBit(i);
                    m->addPort(bit);
                }
            }
            else
            {
                m->addPort(new SyScPort(SyIxName("i0"), false, 
                            SyScPort::IN, SyScNet::WIRE));
                m->addPort(new SyScPort(SyIxName("o"), false, 
                            SyScPort::OUT, SyScNet::REG));
            }
            m->addPort(new SyScPort(SyIxName("cout"), false, 
                        SyScPort::OUT, SyScNet::REG));
            return new SynthINC(m, args[0], args[1]);
        }

}
