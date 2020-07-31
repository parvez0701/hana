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

#include <cmath>

#include "cellsemcheck.h"
#include "sport.h"
#include "scell.h"

#include "symsghandler.h"

using std::list;
using std::string;
using std::vector;

using Synth::SyMsgHandler;
namespace stview
{
//----------------------------------------------------------------------
    static string GetName(const SPort*);
    static bool PowOf2(int num);
    static bool AttribExists(const CellSemCheck*, const string&);
    static void SetBlanketAttribute(CellSemCheck*, const string&, SPort::Dir dir);
    static list<SPort*> GetNotSoSpecialPorts(CellSemCheck*,
        const list<string>&, 
        SPort::Dir);
    static void SetAttribsOnSelectEnableCell(CellSemCheck*, bool);
    static void SetAddSubAttributes(CellSemCheck*);
    static void SetSetResetFFAttributes(CellSemCheck*);
    static int    GetOutputsCount(const CellSemCheck*);
    static int    GetInputsCount(const CellSemCheck*);
    static int BinLog(int);
//----------------------------------------------------------------------

    static string GetName(const SPort *port) {
        return port->getIxName().getName();
    }    
//----------------------------------------------------------------------

    static bool PowOf2(int num) {
        if(num == 0) {
            return false;
        }    
        return !(num & (num -1));
    }
//----------------------------------------------------------------------

    static void SetBlanketAttribute(CellSemCheck *csc, 
        const string& name, SPort::Dir dir)
    {
        if(!csc) {
            return;
        }    
        const list<SPort*>& ports = csc->getCell()->getPorts();
        for(list<SPort*>::const_iterator p = ports.begin();
            p != ports.end(); ++p)
        {
            if((*p)->getDir() == dir) {
                (*p)->setAttribute(name, (void*) 1);
            }
        }
    }

//----------------------------------------------------------------------
    static list<SPort*> GetNotSoSpecialPorts(CellSemCheck *csc,
        const list<string>& attribs, SPort::Dir dir)
    {
        list<SPort*> rports;
        const list<SPort*>& ports = csc->getCell()->getPorts();
        for(list<SPort*>::const_iterator p = ports.begin();
            p != ports.end(); ++p)
        {
            if((*p)->getDir() != dir) {
                continue;
            }
            bool hasattrib = false;
            for(list<string>::const_iterator a = attribs.begin();
                a != attribs.end(); ++a) {
                if((*p)->getAttribute(*a)) 
                {
                    hasattrib = true;
                    break;
                }    
            }
            if(!hasattrib) {
                rports.push_back(*p);
            }
        }
        return rports;
    }
//----------------------------------------------------------------------

    static void SetAttribsOnSelectEnableCell(CellSemCheck *csc, bool sel)
    {
        if(!AttribExists(csc, "DATAOUT")) {
            SetBlanketAttribute(csc, "DATAOUT", SPort::OUT);
        }

        if(AttribExists(csc, sel ? "SELECT" :"ENABLE"))
        {
            if(AttribExists(csc, "DATAIN")) {
                return;
            }
            
            list<string> atts;
            atts.push_back(sel ? "SELECT" : "ENABLE");
            list<SPort*> ports = GetNotSoSpecialPorts(csc, atts, SPort::IN);
            for(list<SPort*>::iterator p = ports.begin(); p != ports.end(); ++p) {
                (*p)->setAttribute("DATAIN", (void*) 1);
            }
        }    
    }
//----------------------------------------------------------------------


    static void SetAddSubAttributes(CellSemCheck *csc)
    {
        if(!AttribExists(csc, "DATAOUT") && 
            !AttribExists(csc, "COUT")) {
            return;
        }
        if(!AttribExists(csc, "DATAOUT") &&
            AttribExists(csc, "COUT"))
        {
            list<string> atts;
            atts.push_back("COUT");
            list<SPort*> ports = GetNotSoSpecialPorts(csc, atts, SPort::OUT);
            for(list<SPort*>::iterator p = ports.begin(); p != ports.end(); ++p) {
                (*p)->setAttribute("DATAOUT", (void*) 1);
            }
        }    
        if(!AttribExists(csc, "DATAIN") && 
            !AttribExists(csc, "CIN")) {
            return;
        }
        if(!AttribExists(csc, "DATAIN") &&
            AttribExists(csc, "CIN"))
        {
            list<string> atts;
            atts.push_back("CIN");
            list<SPort*> ports = GetNotSoSpecialPorts(csc, atts, SPort::IN);
            for(list<SPort*>::iterator p = ports.begin(); p != ports.end(); ++p) {
                (*p)->setAttribute("DATAIN", (void*) 1);
            }
        }    
    }
//----------------------------------------------------------------------
    static void SetSetResetFFAttributes(CellSemCheck *csc)
    {
        if(GetOutputsCount(csc) == 1) {
            SetBlanketAttribute(csc, "DATAOUT", SPort::OUT);
        }    
        if(GetInputsCount(csc) == 4)
        {
            const SPort *p1 = csc->getCell()->getSpecialPort("DATAIN");
            const SPort *p2 = csc->getCell()->getSpecialPort("CLOCK");
            const SPort *p3 = csc->getCell()->getSpecialPort("RESET");
            const SPort *p4 = csc->getCell()->getSpecialPort("SET");

            int count = 0;
            if(p1) {
                ++count;
            }    
            if(p2) {
                ++count;
            }    
            if(p3) {
                ++count;
            }    
            if(p4) {
                ++count;
            }    
            if(count != 3) {
                return;
            }    
            list<string> atts;
            if(p1 && p2 && p3) 
            {
                atts.push_back("DATAIN");
                atts.push_back("CLOCK");
                atts.push_back("RESET");
                list<SPort*> ports = GetNotSoSpecialPorts(csc, atts, SPort::IN);
                ports.front()->setAttribute("SET", (void*)1);
            }
            else if(p1 && p2 && p4)
            {
                atts.push_back("DATAIN");
                atts.push_back("CLOCK");
                atts.push_back("SET");
                list<SPort*> ports = GetNotSoSpecialPorts(csc, atts, SPort::IN);
                ports.front()->setAttribute("RESET", (void*)1);
            }
            else if(p1 && p3 && p4)
            {
                atts.push_back("DATAIN");
                atts.push_back("RESET");
                atts.push_back("SET");
                list<SPort*> ports = GetNotSoSpecialPorts(csc, atts, SPort::IN);
                ports.front()->setAttribute("CLOCK", (void*)1);
            }
            else if(p2 && p3 && p4)
            {
                atts.push_back("CLOCK");
                atts.push_back("RESET");
                atts.push_back("SET");
                list<SPort*> ports = GetNotSoSpecialPorts(csc, atts, SPort::IN);
                ports.front()->setAttribute("DATAIN", (void*)1);
            }
        }    
    }    

//----------------------------------------------------------------------
    static bool AttribExists(const CellSemCheck *csc, const string& attrib){
        return csc->getCell()->getSpecialPort(attrib) != 0;
    }    
//----------------------------------------------------------------------
    static int    GetOutputsCount(const CellSemCheck *csc)
    {
        const SCell *cell = csc->getCell();
        if(!cell) {
            return 0;
        }    
        int count = 0;
        const list<SPort*>& ports = cell->getPorts();
        for(list<SPort*>::const_iterator p = ports.begin();
            p != ports.end(); ++p)
        {
            if((*p)->getDir() == SPort::OUT) {
                ++count;
            }
        }
        return count;
    }    
//----------------------------------------------------------------------
    static int    GetInputsCount(const CellSemCheck *csc)
    {
        const SCell *cell = csc->getCell();
        if(!cell) {
            return 0;
        }    
        int count = 0;
        const list<SPort*>& ports = cell->getPorts();
        for(list<SPort*>::const_iterator p = ports.begin();
            p != ports.end(); ++p)
        {
            if((*p)->getDir() == SPort::IN) {
                ++count;
            }
        }
        return count;
    }    
//----------------------------------------------------------------------
    static int BinLog(int num) {
        if(num <= 1) {
            return 1;
        }
        return (int)log2(num);
    }    
//----------------------------------------------------------------------
    CellSemCheck::CellSemCheck(const SCell *c) : cell(c) {
    }

    CellSemCheck::~CellSemCheck() {
    }

    const SCell*
    CellSemCheck::getCell() const {
        return cell;
    }

    SCell*
    CellSemCheck::getCell() {
        return const_cast<SCell*>(cell);
    }
    void
    CellSemCheck::setCell(const SCell *c) {
        cell = c;
    }    

    bool
    CellSemCheck::checkSizeMatch(const string& pname1, const string& pname2) const
    {
        int size1 = 0;
        int size2 = 0;
        
        const list<SPort*>& ports = cell->getPorts();
        for(list<SPort*>::const_iterator p = ports.begin();
            p != ports.end(); ++p)
        {
            if((*p)->getIxName().getName() == pname1) {
                ++size1;
            }    
            else if((*p)->getIxName().getName() == pname2) {
                ++size2;
            }    
        }
        if(size1 != size2) {
            SyMsgHandler::instance()->print(306, cell->getName().c_str(),
                pname1.c_str(), pname2.c_str());
        }        
        return size1 == size2;
    }

    bool
    CellSemCheck::checkNumInputs(int expected) const
    {
        int size = this->getDataInputs().size();
        if(size != expected)
        {
             SyMsgHandler::instance()->print(301, cell->getName().c_str(),
                 "input", expected, size);
             return false;
        }
        return true;
    }
    bool
    CellSemCheck::checkNumOutputs(int expected) const
    {
        int size = this->getDataOutputs().size();
        if(size != expected)
        {
             SyMsgHandler::instance()->print(301, cell->getName().c_str(),
                 "output", expected, size);
             return false;
        }
        return true;
    }

    bool
    CellSemCheck::checkSize(const string& pname, int expected) const
    {
        int size = this->getSize(pname);
        if(size != expected)
        {
            SyMsgHandler::instance()->print(303, cell->getName().c_str(),
                pname.c_str(), expected, size);
            return false;
        }
        return true;
    }

    bool
    CellSemCheck::checkExists(const string& attrib) const {
        if(!cell->getSpecialPort(attrib)) 
        {
            SyMsgHandler::instance()->print(304, cell->getName().c_str(),
            attrib.c_str());
            return false;
        }
        return true;
             
    }    

    vector<string>
    CellSemCheck::getDataInputs() const
    {
        vector<string> datains;
        if(!cell) {
            return datains;
        }    
        const list<SPort*>& ports = cell->getPorts();
        for(list<SPort*>::const_iterator p = ports.begin();
            p != ports.end(); ++p)
        {
            if(!(*p)->getAttribute("DATAIN")) {
                continue;
            }
            if(std::find(datains.begin(), datains.end(),
                (*p)->getIxName().getName()) == datains.end()) {
                datains.push_back((*p)->getIxName().getName());
            }
        }
        return datains;
    }
    vector<string>
    CellSemCheck::getDataOutputs() const
    {
        vector<string> dataouts;
        if(!cell) {
            return dataouts;
        }    
        const list<SPort*>& ports = cell->getPorts();
        for(list<SPort*>::const_iterator p = ports.begin();
            p != ports.end(); ++p)
        {
            if(!(*p)->getAttribute("DATAOUT")) {
                continue;
            }
            if(std::find(dataouts.begin(), dataouts.end(),
                (*p)->getIxName().getName()) == dataouts.end()) {
                dataouts.push_back((*p)->getIxName().getName());
            }
        }
        return dataouts;
    }
    int    
    CellSemCheck::getDataInputsCount() const
    {
        if(!cell) {
            return 0;
        }    
        int count = 0;
        const list<SPort*>& ports = cell->getPorts();
        for(list<SPort*>::const_iterator p = ports.begin();
            p != ports.end(); ++p)
        {
            if((*p)->getAttribute("DATAIN")) {
                ++count;
            }
        }
        return count;
    }

    int    
    CellSemCheck::getDataOutputsCount() const
    {
        if(!cell) {
            return 0;
        }    
        int count = 0;
        const list<SPort*>& ports = cell->getPorts();
        for(list<SPort*>::const_iterator p = ports.begin();
            p != ports.end(); ++p)
        {
            if((*p)->getAttribute("DATAOUT")) {
                ++count;
            }
        }
        return count;
    }



    int 
    CellSemCheck::getSize(const std::string& pname) const
    {
        int size = 0;
        
        const list<SPort*>& ports = cell->getPorts();
        for(list<SPort*>::const_iterator p = ports.begin();
            p != ports.end(); ++p)
        {
            if((*p)->getIxName().getName() == pname) {
                ++size;
            }
        }
        return size;
    }

    //-----------------------------------------------------------------------

    BufSemCheck::BufSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    BufSemCheck::~BufSemCheck() {
    }

    void
    BufSemCheck::setAttribs()
    {
        if(GetInputsCount(this) == 1) {
            if(!AttribExists(this, "DATAIN")) {
                SetBlanketAttribute(this, "DATAIN", SPort::IN);
            }
        }    
                
        if(GetOutputsCount(this) == 1) {
            if(!AttribExists(this, "DATAOUT")) {
                SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
            }
        }    
    }

    bool
    BufSemCheck::doChecks() const
    {
        bool status = true;
        if(!this->checkNumInputs(1)) {
            status = false;
        }
        if(!this->checkNumOutputs(1)) {
            status = false;
        }
        return status;
    }
    
    //-----------------------------------------------------------------------
    TriBufSemCheck::TriBufSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    TriBufSemCheck::~TriBufSemCheck() {
    }

    void
    TriBufSemCheck::setAttribs()
    {
        if(GetOutputsCount(this) == 1) {
            if(!AttribExists(this, "DATAOUT")) {
                SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
            }
        }    

        if(GetInputsCount(this) == 2)
        {
            const SPort *p1 = this->getCell()->getSpecialPort("DATAIN");
            const SPort *p2 = this->getCell()->getSpecialPort("ENABLE");
            if(!p1 && !p2) {
                return; //nothing can be done
            }
            if(p1 && p2)
            {
                return;
            }

            list<string> atts;
            if(p1) 
            {
                atts.push_back("DATAIN");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("ENABLE", (void *)1);
            }
            else { 
                atts.push_back("ENABLE");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("DATAIN", (void *)1);
            }    
        }    
    }

    bool
    TriBufSemCheck::doChecks() const
    {
        bool status = true;
        if(!this->checkNumInputs(1)) {
            status = false;
        }
        if(!this->checkNumOutputs(1)) {
            status = false;
        }
        if(!this->checkExists("ENABLE")) {
            status = false;
        }
        if(status && !this->checkSize(GetName(this->getCell()->
            getSpecialPort("ENABLE")), 1)) {
            status = false;
        }    
        return status;
    }
    //-----------------------------------------------------------------------

    InvSemCheck::InvSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    InvSemCheck::~InvSemCheck() {
    }

    void
    InvSemCheck::setAttribs()
    {
        if(GetInputsCount(this) == 1) {
            if(!AttribExists(this, "DATAIN")) {
                SetBlanketAttribute(this, "DATAIN", SPort::IN);
            }
        }    
                
        if(GetOutputsCount(this) == 1) {
            if(!AttribExists(this, "DATAOUT")) {
                SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
            }
        }    
    }

    bool
    InvSemCheck::doChecks() const
    {
        bool status = true;
        if(!this->checkNumInputs(1)) {
            status = false;
        }
        if(!this->checkNumOutputs(1)) {
            status = false;
        }
        return status;
    }    
    //-----------------------------------------------------------------------
    //And, Or, Nand, Nor, Xor and XNor semantic checker

    AONNXXSemCheck::AONNXXSemCheck(const SCell *cell) : 
        CellSemCheck(cell) {
    }

    AONNXXSemCheck::~AONNXXSemCheck() {
    }

    void 
    AONNXXSemCheck::setAttribs()
    {
        if(GetOutputsCount(this) == 1) {
            SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
        }

        if(!AttribExists(this, "DATAIN")) {
            SetBlanketAttribute(this, "DATAIN", SPort::IN);
        }    
    }

    bool
    AONNXXSemCheck::doChecks() const
    {
        bool status = true;
        int insize = this->getDataInputsCount();
        if(insize <= 1) 
        {
            SyMsgHandler::instance()->print(301, 
                this->getCell()->getName().c_str(), "input", 2, insize);
            return false;
        }    

        if(!this->checkNumOutputs(1)) {
            status = false;
        }
        return status;
    }    
    //-----------------------------------------------------------------------

    AndSemCheck::AndSemCheck(const SCell *cell) : AONNXXSemCheck(cell) {
    }

    AndSemCheck::~AndSemCheck() {
    }

    //-----------------------------------------------------------------------

    OrSemCheck::OrSemCheck(const SCell *cell) : AONNXXSemCheck(cell) {
    }

    OrSemCheck::~OrSemCheck() {
    }
    //-----------------------------------------------------------------------

    NandSemCheck::NandSemCheck(const SCell *cell) : AONNXXSemCheck(cell) {
    }

    NandSemCheck::~NandSemCheck() {
    }

    //-----------------------------------------------------------------------

    NorSemCheck::NorSemCheck(const SCell *cell) : AONNXXSemCheck(cell) {
    }

    NorSemCheck::~NorSemCheck() {
    }
    //-----------------------------------------------------------------------

    XorSemCheck::XorSemCheck(const SCell *cell) : AONNXXSemCheck(cell) {
    }

    XorSemCheck::~XorSemCheck() {
    }
    //-----------------------------------------------------------------------

    XnorSemCheck::XnorSemCheck(const SCell *cell) : AONNXXSemCheck(cell) {
    }

    XnorSemCheck::~XnorSemCheck() {
    }

    //-----------------------------------------------------------------------

    EncSemCheck::EncSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    EncSemCheck::~EncSemCheck() {
    }

    void
    EncSemCheck::setAttribs() {
        SetAttribsOnSelectEnableCell(this, false);
    }

    bool
    EncSemCheck::doChecks() const
    {
        int insize = this->getDataInputsCount();
        int outsize = this->getDataOutputsCount();
        if(insize <= 1) 
        {
            this->checkNumInputs(2);
            return false;
        }    
        if(!PowOf2(insize)) 
        {
            SyMsgHandler::instance()->print(305, 
                this->getCell()->getName().c_str(),
                this->getCell()->getSpecialPort("DATAIN")->getIxName().getName().c_str());
            return false;
        }    
        int lg2 = (int)log2(insize);
        if(lg2 != outsize)
        {
            SyMsgHandler::instance()->print(302, 
                this->getCell()->getName().c_str(),
                this->getCell()->getSpecialPort("DATAIN")->getIxName().getName().c_str(),
                log2, outsize);
            return false;
        }    

        if(!this->checkExists("ENABLE")) {
            return false;
        }

        if(!this->checkSize(GetName(this->getCell()->
            getSpecialPort("ENABLE")), 1)) {
            return false;
        }    
        
        return true;
    }

    //-----------------------------------------------------------------------

    DecSemCheck::DecSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    DecSemCheck::~DecSemCheck() {
    }

    void
    DecSemCheck::setAttribs() {
        SetAttribsOnSelectEnableCell(this, false);
    }
    bool
    DecSemCheck::doChecks() const
    {
        int insize = this->getDataInputsCount();
        int outsize = this->getDataOutputsCount();
        if(outsize <= 1) 
        {
            this->checkNumOutputs(2);
            return false;
        }    
        if(!PowOf2(outsize)) 
        {
            SyMsgHandler::instance()->print(305, 
                this->getCell()->getName().c_str(),
                this->getCell()->getSpecialPort("DATAOUT")->getIxName().getName().c_str());
            return false;
        }    
        int lg2 = (int)log2(outsize);
        if(lg2 != insize)
        {
            SyMsgHandler::instance()->print(302, 
                this->getCell()->getName().c_str(),
                this->getCell()->getSpecialPort("DATAOUT")->getIxName().getName().c_str(),
                log2, insize);
            return false;
        }    

        if(!this->checkExists("ENABLE")) {
            return false;
        }

        if(!this->checkSize(GetName(this->getCell()->
            getSpecialPort("ENABLE")), 1)) {
            return false;
        }    
        
        return true;
    }
    //-----------------------------------------------------------------------

    MuxSemCheck::MuxSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    MuxSemCheck::~MuxSemCheck() {
    }

    void
    MuxSemCheck::setAttribs()
    {
        SetAttribsOnSelectEnableCell(this, true);
    }

    bool
    MuxSemCheck::doChecks() const
    {
        int insize = this->getDataInputsCount();
        if(insize <= 1) 
        {
            this->checkNumInputs(2);
            return false;
        }    
        if(!PowOf2(insize)) 
        {
            SyMsgHandler::instance()->print(305, 
                this->getCell()->getName().c_str(),
                this->getCell()->getSpecialPort("DATAIN")->getIxName().getName().c_str());
            return false;
        }    
        if(!this->checkSize(GetName(this->getCell()->
            getSpecialPort("DATAOUT")), 1)) {
            return false;
        }    

        if(!this->checkExists("SELECT")) {
            return false;
        }
        int lg = log2(insize);

        if(!this->checkSize(GetName(this->getCell()->
            getSpecialPort("SELECT")), lg)) {
            return false;
        }    
        
        return true;
    }

    //-----------------------------------------------------------------------

    AddSemCheck::AddSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    AddSemCheck::~AddSemCheck() {
    }

    void
    AddSemCheck::setAttribs() {
        SetAddSubAttributes(this);
    }

    bool
    AddSemCheck::doChecks() const
    {

        if(!this->checkNumInputs(2)) {
            return false;
        }
        if(!this->checkNumOutputs(1)) {
            return false;
        }
        vector<string> inputs = this->getDataInputs();
        if(!this->checkSizeMatch(inputs[0], inputs[1])) {
            return false;
        }    
        vector<string> outputs = this->getDataOutputs();
        if(!this->checkSizeMatch(inputs[0], outputs[0])) {
            return false;
        }    

        if(!this->checkExists("CIN") || 
            !this->checkExists("COUT")) {
            return false;
        }    
        return true;
    }    
    //-----------------------------------------------------------------------

    SubSemCheck::SubSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    SubSemCheck::~SubSemCheck() {
    }

    void
    SubSemCheck::setAttribs() {
        SetAddSubAttributes(this);
    }
    bool
    SubSemCheck::doChecks() const
    {

        if(!this->checkNumInputs(2)) {
            return false;
        }
        if(!this->checkNumOutputs(1)) {
            return false;
        }
        vector<string> inputs = this->getDataInputs();
        if(!this->checkSizeMatch(inputs[0], inputs[1])) {
            return false;
        }    
        vector<string> outputs = this->getDataOutputs();
        if(!this->checkSizeMatch(inputs[0], outputs[0])) {
            return false;
        }    

        if(!this->checkExists("CIN") || 
            !this->checkExists("COUT")) {
            return false;
        }    
        return true;
    }    

    //-----------------------------------------------------------------------

    MulSemCheck::MulSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    MulSemCheck::~MulSemCheck() {
    }

    void
    MulSemCheck::setAttribs()
    {
        if(!AttribExists(this, "DATAOUT")) {
            SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
        }    
        if(!AttribExists(this, "DATAIN")) {
            SetBlanketAttribute(this, "DATAIN", SPort::IN);
        }    
    }

    bool
    MulSemCheck::doChecks() const
    {

        if(!this->checkNumInputs(2)) {
            return false;
        }
        if(!this->checkNumOutputs(1)) {
            return false;
        }
        vector<string> inputs = this->getDataInputs();
        if(!this->checkSizeMatch(inputs[0], inputs[1])) {
            return false;
        }    
        int insize = this->getDataInputsCount();
        int outsize = this->getDataOutputsCount();

        if(insize != outsize)
        {
            SyMsgHandler::instance()->print(303,
                this->getCell()->getName().c_str(),
                this->getCell()->getSpecialPort("DATAOUT")
                ->getIxName().getName().c_str(),
                insize, outsize);
            return false;
        }    
        return true;
    }    

    //-----------------------------------------------------------------------

    FFSemCheck::FFSemCheck() : CellSemCheck(0) {
    }
    FFSemCheck::FFSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    FFSemCheck::~FFSemCheck() {
    }
    void
    FFSemCheck::setAttribs()
    {
        if(GetOutputsCount(this) == 1) {
            SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
        }    
        if(GetInputsCount(this) == 2)
        {
            const SPort *p1 = this->getCell()->getSpecialPort("DATAIN");
            const SPort *p2 = this->getCell()->getSpecialPort("CLOCK");
            if(!p1 && !p2) {
                return; //nothing can be done
            }
            if(p1 && p2) {
                return;
            }

            list<string> atts;
            if(p1) 
            {
                atts.push_back("DATAIN");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("CLOCK", (void *)1);
            }
            else { 
                atts.push_back("CLOCK");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("DATAIN", (void *)1);
            }    
        }    
    }


    bool
    FFSemCheck::doChecks() const
    {
        if(!this->checkNumInputs(1) || !this->checkNumOutputs(1)) {
            return false;
        }

        if(!this->checkExists("CLOCK")) {
            return false;
        }    
        if(!this->checkSize(GetName(this->getCell()->
            getSpecialPort("CLOCK")), 1)) {
            return false;
        }    
        return true;
    }    

    //-----------------------------------------------------------------------

    RFFSemCheck::RFFSemCheck() : FFSemCheck(0) {
    }
    RFFSemCheck::RFFSemCheck(const SCell *cell) : FFSemCheck(cell) {
    }

    RFFSemCheck::~RFFSemCheck() {
    }

    void
    RFFSemCheck::setAttribs()
    {
        if(GetOutputsCount(this) == 1) {
            SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
        }    
        if(GetInputsCount(this) == 3)
        {
            const SPort *p1 = this->getCell()->getSpecialPort("DATAIN");
            const SPort *p2 = this->getCell()->getSpecialPort("CLOCK");
            const SPort *p3 = this->getCell()->getSpecialPort("RESET");

            if(!p1 && !p2 && !p3) {
                return;
            }    
            if(p1 && p2 && p3) {
                return;
            }
            int count = 0;
            if(p1) {
                ++count;
            }    
            if(p2) {
                ++count;
            }    
            if(p3) {
                ++count;
            }    
            if(count == 1) {
                return;
            }    
            list<string> atts;
            if(p1 && p2) 
            {
                atts.push_back("DATAIN");
                atts.push_back("CLOCK");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("RESET", (void *)1);
            }
            else if(p1 && p3)
            {
                atts.push_back("DATAIN");
                atts.push_back("RESET");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("CLOCK", (void *)1);
            }
            else if(p2 && p3)
            {
                atts.push_back("CLOCK");
                atts.push_back("RESET");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("DATAIN", (void *)1);
            }
        }    
    }    

    bool
    RFFSemCheck::doChecks() const
    {
        if(!FFSemCheck::doChecks()) {
            return false;
        }    
        if(!this->checkExists("RESET")) {
            return false;
        }    
        if(!this->checkSize(GetName(this->getCell()->
            getSpecialPort("RESET")), 1)) {
            return false;
        }    
        return true;
    }    
    //-----------------------------------------------------------------------

    SFFSemCheck::SFFSemCheck() : FFSemCheck(0) {
    }
    SFFSemCheck::SFFSemCheck(const SCell *cell) : FFSemCheck(cell) {
    }

    SFFSemCheck::~SFFSemCheck() {
    }

    void
    SFFSemCheck::setAttribs()
    {
        if(GetOutputsCount(this) == 1) {
            SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
        }    
        if(GetInputsCount(this) == 3)
        {
            const SPort *p1 = this->getCell()->getSpecialPort("DATAIN");
            const SPort *p2 = this->getCell()->getSpecialPort("CLOCK");
            const SPort *p3 = this->getCell()->getSpecialPort("SET");

            if(!p1 && !p2 && !p3) {
                return;
            }    
            if(p1 && p2 && p3) {
                return;
            }
            int count = 0;
            if(p1) {
                ++count;
            }    
            if(p2) {
                ++count;
            }    
            if(p3) {
                ++count;
            }    
            if(count == 1) {
                return;
            }    
            list<string> atts;
            if(p1 && p2) 
            {
                atts.push_back("DATAIN");
                atts.push_back("CLOCK");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("SET", (void*)1);
            }
            else if(p1 && p3)
            {
                atts.push_back("DATAIN");
                atts.push_back("SET");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("CLOCK", (void*)1);
            }
            else if(p2 && p3)
            {
                atts.push_back("CLOCK");
                atts.push_back("SET");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("DATAIN", (void*)1);
            }
        }    
    }    
    bool
    SFFSemCheck::doChecks() const
    {
        if(!FFSemCheck::doChecks()) {
            return false;
        }    
        if(!this->checkExists("SET")) {
            return false;
        }    
        if(!this->checkSize(GetName(this->getCell()->
            getSpecialPort("SET")), 1)) {
            return false;
        }    
        return true;
    }    
    //-----------------------------------------------------------------------

    RSFFSemCheck::RSFFSemCheck(const SCell *cell) : 
        RFFSemCheck(cell),
        SFFSemCheck(cell){
    }

    RSFFSemCheck::~RSFFSemCheck() {
    }

    void
    RSFFSemCheck::setAttribs() {
        SetSetResetFFAttributes(this);
    }    

    bool
    RSFFSemCheck::doChecks() const
    {
        if(!RFFSemCheck::doChecks()) {
            return false;
        }

        if(!SFFSemCheck::doChecks()) {
            return false;
        }
        return true;
    }    
    //-----------------------------------------------------------------------

    SRFFSemCheck::SRFFSemCheck(const SCell *cell) : RSFFSemCheck(cell){
    }

    SRFFSemCheck::~SRFFSemCheck() {
    }

    void
    SRFFSemCheck::setAttribs() {
        SetSetResetFFAttributes(this);
    }    
    //-----------------------------------------------------------------------

    LDSemCheck::LDSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    LDSemCheck::~LDSemCheck() {
    }

    void
    LDSemCheck::setAttribs()
    {
        if(GetOutputsCount(this) == 1) {
            if(!AttribExists(this, "DATAOUT")) {
                SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
            }
        }    

        if(GetInputsCount(this) == 2)
        {
            const SPort *p1 = this->getCell()->getSpecialPort("DATAIN");
            const SPort *p2 = this->getCell()->getSpecialPort("ENABLE");
            if(!p1 && !p2) {
                return; //nothing can be done
            }
            if(p1 && p2)
            {
                return;
            }

            list<string> atts;
            if(p1) 
            {
                atts.push_back("DATAIN");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("ENABLE", (void *)1);
            }
            else { 
                atts.push_back("ENABLE");
                list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
                ports.front()->setAttribute("DATAIN", (void *)1);
            }    
        }    
    }

    bool
    LDSemCheck::doChecks() const
    {
        if(!this->checkNumInputs(1) || !this->checkNumOutputs(1)) {
            return false;
        }

        if(!this->checkExists("ENABLE")) {
            return false;
        }    
        if(!this->checkSize(GetName(this->getCell()->
            getSpecialPort("ENABLE")), 1)) {
            return false;
        }    
        return true;
    }    
    //-----------------------------------------------------------------------

    ShiftSemCheck::ShiftSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    ShiftSemCheck::~ShiftSemCheck() {
    }

    void
    ShiftSemCheck::setAttribs()
    {
        if(!AttribExists(this, "DATAOUT")) {
            SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
        }    
        //Rest TBD
    }

    bool
    ShiftSemCheck::doChecks() const
    {
        if(!this->checkNumInputs(1)) {
            return false;
        }
        if(!this->checkNumOutputs(1)) {
            return false;
        }
        vector<string> inputs = this->getDataInputs();
        vector<string> outputs = this->getDataOutputs();
        if(!this->checkSizeMatch(inputs[0], outputs[0])) {
            return false;
        }    

        int count = this->getDataInputsCount();
        if(!this->checkExists("SHIFT")) {
            return false;
        }

        if(!this->checkSize(GetName(getCell()->getSpecialPort("SHIFT")),
            (int)log2(count)+1)) {
            return false;
        }    

        if(const SPort *port = this->getCell()->getSpecialPort("SHIFTVAL")) {
            if(!this->checkSize(GetName(port), 1)) {
                return false;
            }
        }
        return true;
    }    
        
    //-----------------------------------------------------------------------

    LShiftSemCheck::LShiftSemCheck(const SCell *cell) : ShiftSemCheck(cell) {
    }

    LShiftSemCheck::~LShiftSemCheck() {
    }
    //-----------------------------------------------------------------------

    RShiftSemCheck::RShiftSemCheck(const SCell *cell) : ShiftSemCheck(cell) {
    }

    RShiftSemCheck::~RShiftSemCheck() {
    }
    
    //-----------------------------------------------------------------------

    IncSemCheck::IncSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    IncSemCheck::~IncSemCheck() {
    }

    void
    IncSemCheck::setAttribs()
    {
        if(!AttribExists(this, "DATAOUT")) {
            SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
        }    
        if(!AttribExists(this, "DATAIN")) {
            SetBlanketAttribute(this, "DATAIN", SPort::IN);
        }    
    }
    bool
    IncSemCheck::doChecks() const
    {
        if(!this->checkNumInputs(1)) {
            return false;
        }    
        if(!this->checkNumOutputs(1)) {
            return false;
        }    

        int insize = this->getDataInputsCount();
        if(!this->checkSize(GetName(getCell()->
            getSpecialPort("DATAOUT")), insize+1)) {
            return false;
        }
        return true;
    }

    VccSemCheck::VccSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    VccSemCheck::~VccSemCheck() {
    }
        
    void
    VccSemCheck::setAttribs()
    {
        if(GetOutputsCount(this) == 1) {
            if(!AttribExists(this, "DATAOUT")) {
                SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
            }
        }
    }    
    bool
    VccSemCheck::doChecks() const {
        return this->checkNumOutputs(1);
    }


    GndSemCheck::GndSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    GndSemCheck::~GndSemCheck() {
    }
        
    void
    GndSemCheck::setAttribs()
    {
        if(GetOutputsCount(this) == 1) {
            if(!AttribExists(this, "DATAOUT")) {
                SetBlanketAttribute(this, "DATAOUT", SPort::OUT);
            }
        }    
    }    

    bool
    GndSemCheck::doChecks() const {
        return this->checkNumOutputs(1);
    }
    //-----------------------------------------------------------------------

    CmpSemCheck::CmpSemCheck(const SCell *cell) : CellSemCheck(cell) {
    }

    CmpSemCheck::~CmpSemCheck() {
    }

    void
    CmpSemCheck::setAttribs() {
        if(!AttribExists(this, "DATAIN"))
        {
            list<string> atts;
            atts.push_back("EQ");
            atts.push_back("UEQ");
            atts.push_back("GT");
            atts.push_back("LT");
            list<SPort*> ports = GetNotSoSpecialPorts(this, atts, SPort::IN);
            for(list<SPort*>::iterator p = ports.begin(); p != ports.end(); ++p) {
                (*p)->setAttribute("DATAIN", (void*) 1);
            }
        }    
    }

    bool
    CmpSemCheck::doChecks() const
    {

        if(!this->checkNumInputs(2)) {
            return false;
        }

        vector<string> inputs = this->getDataInputs();
        if(!this->checkSizeMatch(inputs[0], inputs[1])) {
            return false;
        }    

        if(!this->checkExists("EQ") || 
            !this->checkExists("UEQ") ||
            !this->checkExists("LT") ||
            !this->checkExists("GT")) {
            return false;
        }    
        return true;
    }    
}

