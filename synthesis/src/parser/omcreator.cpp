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

#include <algorithm>
#include "root.h"
#include "omcreator.h"
#include "module.h"
#include "udp.h"
#include "udpinst.h"
#include "param.h"
#include "localparam.h"
#include "parseinfo.h"
#include "dimension.h"
#include "veclocalparam.h"
#include "vecparam.h"
#include "vecnet.h"
#include "vecport.h"
#include "file.h"
#include "basednum.h"
#include "hierid.h"
#include "gateinst.h"
//#include "modinst.h"
#include "pin.h"
#include "vecpin.h"
#include "delay.h"
#include "contassign.h"
#include "semcheck.h"
#include "vecreg.h"
#include "vecint.h"
#include "vectime.h"
#include "vtime.h"
#include "vecreal.h"
#include "vecrealtime.h"
#include "namedblock.h"
#include "taskfunc.h"
#include "always.h"
#include "initial.h"
#include "defparam.h"
#include "event.h"
#include "specify.h"
#include "bitsel.h"
#include "partsel.h"
#include "generate.h"
#include "num.h"
#include "vstring.h"
#include "implicitnet.h"
#include "ifdefproc.h"

using namespace std;
extern void yyerror( char const *);
extern FILE* yyin;
extern void createBufferState();
extern void switchToPreviousBufferState();
namespace Rtl
{
    extern Root* GRoot;
    OMCreator* OMCreator::omcreator = NULL;

    OMCreator::OMCreator(): 
    currMod(0), currScope(0), currRange(0), currFile(0), lineno(1),
    errcount(0), currGateType(UNSETGATE), currBlkDecl(0), 
    currSpecifyBlock(0), currUDP(0), parseInfos(0), currTable(0),
    currGenStmt(0), defNetType(WIRE)/*, currGenItem(0)*/{
        const vector<File*>& files = GRoot->getFiles();
        for(int i = (int)files.size() -1; i > 0; --i){
            filesToBeParsed.push(files[i]);
        }
    }

    OMCreator::~OMCreator(){
        omcreator = NULL;
    }

    OMCreator*
    OMCreator::instance(){
        if(!omcreator){
            omcreator = new OMCreator();
        }
        return omcreator;
    }

    //---------------------------------------------------------------//
    //    Settings done for parsing next file.
    //    Returns 0 if all is well
    //    -------------------------------------------------------------//
    
    int
    OMCreator::parseSetup(const std::string& inclname)
    {
        if(!inclname.empty())
        {
            File* inclFile = this->createIncludeFile(inclname);
            if(!inclFile)
            {
                //PrintMsg("ERROR]: Line ", lineno, 
                //        " Could not open include file ", inclname, 
                //        " for reading.\n");
                MsgHandler::instance()->print(10, "include", inclname.c_str());
                return 1;
            }
            ParseState ps;
            ps.yyin = yyin;
            ps.file = currFile;
            ps.line = lineno;

            yyin = inclFile->open();
            if(!yyin){
                delete inclFile;
                //PrintMsg("ERROR]: Line ", lineno, 
                //        " Could not open include file ", inclname, 
                //        " for reading.\n");
                MsgHandler::instance()->print(10, inclFile->getFileTypeStr().c_str(), inclname.c_str());
                return 1;
            }
            parseStates.push(ps);
            createBufferState();
            //PrintMsg("[INFO]: Line ", lineno, 
            //        " Reading include file ", inclFile->getName(), 
            //        "\n");
            MsgHandler::instance()->print(1, inclFile->getFileTypeStr().c_str(), inclFile->getName().c_str());
            currFile = inclFile;
            lineno = 1;
            return 0;
        }
        if(!parseStates.empty())
        {
            ParseState ps = parseStates.top();
            parseStates.pop();
            yyin = ps.yyin;
            currFile = ps.file;
            lineno = ps.line;
            switchToPreviousBufferState();
            return 0;
        }
        if(currFile && !currFile->isInclFile()) {
            IfdefProc::instance()->doChecksAtEOF();
        }    
        if(filesToBeParsed.empty()){
            return 1;
        }
        File* file = filesToBeParsed.top();
        filesToBeParsed.pop();
        yyin = file->open();
        if(!yyin)
        {
            //PrintMsg("[ERROR]: Can not open file ", file->getName(),
            //    " for reading.");
            MsgHandler::instance()->print(10, file->getFileTypeStr().c_str(), file->getName().c_str());
            return 1;
        }
        currFile = file;
        lineno = 1;
        currMod = 0;
        currScope = 0;
        currRange = 0;
        currBlkDecl = 0;
        currInstMaster.clear();
        currGateType = UNSETGATE;
        currSpecifyBlock = 0;
        currGenItem = 0;
        //PrintMsg("[INFO]: Analyzing file ", file->getName(), 
        //    " ........");
        MsgHandler::instance()->print(1, file->getFileTypeStr().c_str(), file->getName().c_str());
        return 0;
    }
        

    void
    OMCreator::scopeEndAction()
    {
        if(dynamic_cast<GenerateStmt*>(currScope)){
            currGenStmt = 0;
            genItems.pop();
        }

        if(!currSpecifyBlock && !currUDP){
            currScope = currScope ? 
                const_cast<Scope*>(currScope->getScope()) : 0;
        }

        if(currSpecifyBlock){
            currSpecifyBlock = 0;
        }
        if(currUDP){
            currUDP = 0;
        }
            
    }
    void
    OMCreator::addParams(vector<Param*>* params){
        if(!params){
            return;
        }
        const int size = params->size();
        currMod->addParams(*params);
        for(int i = 0; i < size; ++i) {
            (*params)[i]->setScope(currMod);
        }
    }

    void
    OMCreator::addLocalParams(vector<LocalParam*>* lparams){
        if(!lparams){
            return;
        }
        const int size = lparams->size();
        currMod->addLocalParams(*lparams);
        for(int i = 0; i < size; ++i) {
            (*lparams)[i]->setScope(currMod);
        }
    }
    Module*
    OMCreator::createModule(AttribTable* table, const string& name)
    {
        //char str[1024];
        //sprintf(str, "[INFO]: Analyzing module '%s' .....\n", name.c_str());
        //PrintMsg(str);
        MsgHandler::instance()->print(2, "module", name.c_str());
        currMod = new Module(name);
        currMod->setAttribTable(table);
        currScope = currMod;
        currBlkDecl = new BlockDecl();
        currMod->setBlockDecl(currBlkDecl);
        currFile->addModule(currMod);
        GRoot->addObject(name, currMod, true);
        if(currFile->isLibFile()){
            SemCheck::instance()->remUndefMod(name);
        }
        return currMod;
    }

    UDP*
    OMCreator::createUDP(const string& name)
    {
        //char str[1024];
        //sprintf(str, "[INFO]: Analyzing udp '%s' .....\n", name.c_str());
        //PrintMsg(str);
        MsgHandler::instance()->print(2, "udp", name.c_str());
        currUDP = new UDP(name);
        GRoot->addObject(name, currUDP, true);
        if(currFile->isLibFile()){
            SemCheck::instance()->remUndefMod(name);
        }
        currFile->addUDP(currUDP);
        return currUDP;
    }
    const Base*
    OMCreator::findObj(const string& name, const Scope* sc) const
    {
        const Scope* scope = sc ? sc : currScope;

        while(scope){
            const Base* obj =  scope->getObject(name);
            if(obj){
                return obj;
            }
            scope = scope->getScope();
        }
        return NULL;
    }


    vector<Param*>*
    OMCreator::createVecParams( vector<ParseInfo*>* parseInfos)
    {
        Range* range = NULL;
        vector<Param*>* params = new vector<Param*>();
        const int size = parseInfos->size();
        map<string, VecParam*, compare_str> vecparams;
        for(int i = 0; i < size; ++i)
        {
            if(this->findObj((*parseInfos)[i]->name, currScope)){
                //PrintMsg("[ERROR] : Line ", lineno, "Variable ", 
                //(*parseInfos)[i]->name, "already declared.\n");
                MsgHandler::instance()->print(21, (*parseInfos)[i]->name.c_str());
                continue;
            }
            if(!range){
                range = (*parseInfos)[i]->range;
            }
            Dimension* dim = new Dimension();
            dim->packDim.push_back(range);
            VecParam* vparam = new VecParam((*parseInfos)[i]->name, dim,
            (*parseInfos)[0]->isSigned);
            //vector<Param*>& tparams = const_cast<vector<Param*>&>
            //    (vparam->getParams());
            vector<Param*> tparams = vparam->getParams();
            //std::copy(tparams.begin(), tparams.end(), params->end());
            int psize = tparams.size();
            for(int j = 0; j < psize; ++j){
                params->push_back(tparams[j]);
            }
    
            if(!(*parseInfos)[i]->exprs.empty()){
                vparam->setVal((*parseInfos)[i]->exprs[0]);
            }

            vecparams.insert(make_pair((*parseInfos)[i]->name, vparam));
            //currMod->addObject((*parseInfos)[i]->name, vparam, false);
            //delete (*parseInfos)[i];
        }
        for(int i = 0; i < size; ++i) {
            delete (*parseInfos)[i];
        }    
        delete parseInfos;
        this->addParams(params);
        map<string, VecParam*, compare_str>::iterator vparam = vecparams.begin();
        map<string, VecParam*, compare_str>::iterator vparamEnd = vecparams.end();
        for(; vparam != vparamEnd; ++vparam){
            currMod->addObject(vparam->first, vparam->second, false);
        }
        return params;
    }

    vector<LocalParam*>*
    OMCreator::createVecLocalParams( vector<ParseInfo*>* parseInfos)
    {
        Range* range = NULL;
        vector<LocalParam*>* lparams = new vector<LocalParam*>();
        const int size = parseInfos->size();
        map<string, VecLocalParam*, compare_str> veclparams;
        for(int i = 0; i < size; ++i)
        {
            if(this->findObj((*parseInfos)[i]->name, currScope)){
                //PrintMsg("[ERROR] : Line ", lineno, "Variable ", 
                //(*parseInfos)[i]->name, "already declared.\n");
                MsgHandler::instance()->print(21, (*parseInfos)[i]->name.c_str());
                continue;
            }
            if(!range){
                range = (*parseInfos)[i]->range;
            }
            Dimension* dim = new Dimension();
            dim->packDim.push_back(range);
            VecLocalParam* vlparam = new VecLocalParam((*parseInfos)[i]->name, dim,
            (*parseInfos)[0]->isSigned);
            vector<LocalParam*>& tlparams = const_cast<vector<LocalParam*>&>
                (vlparam->getLocalParams());
            int lpsize = tlparams.size();
            for(int j = 0; j < lpsize; ++j){
                lparams->push_back(tlparams[j]);
            }
    
            if(!(*parseInfos)[i]->exprs.empty()){
                vlparam->setVal((*parseInfos)[i]->exprs[0]);
            }

            veclparams.insert(make_pair((*parseInfos)[i]->name, vlparam));
            //delete (*parseInfos)[i];
        }
        for(int i = 0; i < size; ++i) {
            delete (*parseInfos)[i];
        }    
        delete parseInfos;
        this->addLocalParams(lparams);
        map<string, VecLocalParam*, compare_str>::iterator vlparam = veclparams.begin();
        map<string, VecLocalParam*, compare_str>::iterator vlparamEnd = veclparams.end();
        for(; vlparam != vlparamEnd; ++vlparam){
            currMod->addObject(vlparam->first, vlparam->second, false);
        }
        return lparams;
    }
    void
    OMCreator::setInstParams(ModInst* inst,
        const std::vector<NameConn*>& nv)
    {
        //check repeat
        set<string, compare_str> pnames;
        unsigned int size = nv.size();
        int status = 0;

        for(unsigned int i = 0; i < size; ++i)
        {
            if(nv[i]->first.empty()){
                break;
            }
            if(pnames.find(nv[i]->first) != pnames.end())
            {
                //PrintMsg("[ERROR]: Line ", lineno, " Repeated parameter name ",
                //    nv[i]->first, " in instantiation.");
                MsgHandler::instance()->print(lineno, nv[i]->first.c_str());
                ++status;
                continue;
            }
            pnames.insert(nv[i]->first);
        }
        //end

        if(!inst) {
            return;
        }    

        const Module* mod = inst->getMaster();
        if(!mod)
        {
            //nothing more to be done at this stage.
            inst->setParams(nv);
            return;
        }

        const std::vector<Param*>& params = mod->getParams();
        vector<string> actParamNames = GetNames<Param>(params);
        if(actParamNames.size() < size)
        {
            //PrintMsg("[ERROR]: Line ", lineno, " Too many parameter values specified, exptected maximum ", actParamNames.size(), " found ", size, ".\n");
            MsgHandler::instance()->print(23, lineno, actParamNames.size(), size);
            return;
        }
        for(unsigned int i = 0; i < size; ++i)
        {
            if(nv[i]->first.empty()){
                //position association
                nv[i]->first = actParamNames[i];
                //break;
            }
            if(std::find(actParamNames.begin(), actParamNames.end(), 
                nv[i]->first) == actParamNames.end()) 
                {
                //PrintMsg("ERROR]: Line ", lineno, " Parameter name ",
                //    nv[i]->first, " not found in module parameter list.");
                MsgHandler::instance()->print(24, lineno, nv[i]->first.c_str());
                ++status;
            }
        }
        if(!status){
            inst->setParams(nv);
        }
    }

    void
    OMCreator::addNets(AttribTable* table, const NetType t, const bool issigned,
        const Range* range, std::vector<pair<char*, Expr*> >* ids)
    {
        if(!currScope) {
            return;
        }    
        const int size = ids->size();
        for(int i = 0; i < size; ++i)
        {
            if(const Base* b = this->findObj((*ids)[i].first))
            {
                if(!dynamic_cast<const VecPort*>(b) &&
                    !dynamic_cast<const Port*>(b))
                {
                   // char str[1024];
                   // sprintf(str, "Variable '%s' already declared.",
                   // (*ids)[i].first);
                   // yyerror(str);
                   MsgHandler::instance()->print(21, lineno, (*ids)[i].first);
                    continue;
                }
            }
            if(this->handlePortRegNet(table, t, issigned, range, NULL,
                        (*ids)[i])){
                continue;
            }
            if(range)
            {
                Dimension* dim = new Dimension();
                dim->packDim.push_back(const_cast<Range*>(range));
                VecNet* vnet = new VecNet((*ids)[i].first, dim);
                vnet->setAttribTable(table);
                const vector<Net*>& nets = vnet->getNets();
                if(ModGenDecl* currGenDecl = this->getCurrGenDecl())
                {
                    const int nnets = nets.size();
                    for(int j = 0; j < nnets; ++j){
                        currGenDecl->nets.push_back(nets[j]);
                    }
                    currScope->addObject((*ids)[i].first, vnet, false);
                }
                else if(currMod){
                    currMod->addObject((*ids)[i].first, vnet, false);
                    currMod->addNets(nets);
                }
                const int nnets = nets.size();
                for(int j = 0; j < nnets; ++j)
                {
                    nets[j]->setScope(currScope);
                    nets[j]->setNetType(t);
                    nets[j]->setInitExpr((*ids)[i].second);
                    nets[j]->setAttribTable(table);
                }
            }
            else
            {
                Net* net = new Net(string((*ids)[i].first));
                net->setAttribTable(table);
                if(ModGenDecl* currGenDecl = this->getCurrGenDecl())
                {
                    currGenDecl->nets.push_back(net);
                    currScope->addObject((*ids)[i].first, net, false);
                }
                else if(currMod)
                {
                    vector<Net*> nets;
                    nets.push_back(net);
                    currMod->addObject((*ids)[i].first, net, false);
                    currMod->addNets(nets);
                }
                net->setScope(currScope);
                net->setNetType(t);
                net->setInitExpr((*ids)[i].second);
            }
        }
        delete ids;
    }



    void
    OMCreator::addNets(AttribTable* table, const NetType t, const bool issigned,
        const Range* range, vector<pair<char*, vector<Range*>*> >* ids)
    {
        const int size = ids->size();
        for(int i = 0; i < size; ++i)
        {
            pair<char*, vector<Range*>*>  id = (*ids)[i];
            if(const Base* b = this->findObj((*ids)[i].first))
            {
                if(!dynamic_cast<const VecPort*>(b) && 
                    !dynamic_cast<const Port*>(b))
                {
                //    char str[1024];
                //    sprintf(str, "Variable '%s' already declared.",
                //    (*ids)[i].first);
                //    yyerror(str);
                    MsgHandler::instance()->print(21, lineno, (*ids)[i].first);
                    continue;
                }
            }
            pair<char*, Expr*> charexprpair = make_pair(id.first, (Expr*)0);

            if(this->handlePortRegNet(table, t, issigned, range, 
                        id.second, 
                        charexprpair)){
                continue;
            }

            if(range || (*ids)[i].second)
            {
                Dimension* dim = new Dimension();
                if(range) {
                    dim->packDim.push_back(const_cast<Range*>(range));
                }
                if((*ids)[i].second){
                    dim->unpackDim = *(*ids)[i].second;
                }
                VecNet* vnet = new VecNet((*ids)[i].first, dim);
                const vector<Net*>& nets = vnet->getNets();
                if(ModGenDecl* currGenDecl = this->getCurrGenDecl())
                {
                    const int nnets = nets.size();
                    for(int j = 0; j < nnets; ++j){
                        currGenDecl->nets.push_back(nets[i]);
                    }
                    currScope->addObject((*ids)[i].first, vnet, false);
                }
                else if(currMod)
                {
                    currMod->addObject((*ids)[i].first, vnet, false);
                    currMod->addNets(nets);
                }
                vnet->setAttribTable(table);
                const int nnets = nets.size();
                for(int j = 0; j < nnets; ++j)
                {
                    nets[j]->setScope(currScope);
                    nets[j]->setNetType(t);
                    nets[j]->setAttribTable(table);
                    //nets[j]->setInitExpr((*ids)[i].second);
                }
            }
            else
            {
                Net* net = new Net(string((*ids)[i].first));
                if(ModGenDecl* currGenDecl = this->getCurrGenDecl())
                {
                    currGenDecl->nets.push_back(net);
                    currScope->addObject((*ids)[i].first, net, false);
                }
                else if(currMod)
                {
                    vector<Net*> nets;
                    nets.push_back(net);
                    currMod->addObject((*ids)[i].first, net, false);
                    currMod->addNets(nets);
                }
                net->setScope(currScope);
                net->setNetType(t);
                net->setAttribTable(table);
                //net->setInitExpr((*ids)[i].second);
            }
        }
        delete ids;
    }
        

    bool
    OMCreator::checkPortExists(const string& name) const
    {
        const int size = parseInfos->size();
        for(int i = 0; i < size; ++i){
            if((*parseInfos)[i]->name == name){
                return true;
            }
        }
        //char str[1024];
        //sprintf(str, "Port '%s' does not exist in top declaration.",
        //    name.c_str());
        //yyerror(str);
        return false;        
    }

    bool
    OMCreator::checkDupPort(vector<pair<char*, Expr*> >* ids) const
    {
        if(!ids){
            return true;
        }

        set<string, compare_str> names;
        const int size = ids->size();
        bool status = true;
        for(int i = 0; i < size; ++i)
        {
            if(names.find((*ids)[i].first) != names.end()) 
            {
                //PrintMsg("[ERROR] : ", lineno, "port ",
                //    "already declared.\n");
                MsgHandler::instance()->print(25, (*ids)[i].first);
                status = false;    
            }
            names.insert((*ids)[i].first);
        }
        return status;
    }
                
        

/*
    bool
    OMCreator::checkDupPort(const string& name,
        const vector<ParseInfo*>* vpi) const
    {
        bool flag = false;
        const int size = vpi->size();
        for(int i = 0; i < size; ++i){
            if((*vpi)[i]->name == name){
                if((*vpi)[i]->dir != UNSET){
                    flag = true;
                }
                break;
            }
        }
        if(!flag){
            return true;
        }

        char str[1024];
        sprintf(str, "Port '%s' declared previously.\n",
            name.c_str());
        yyerror(str);
        return false;        
    }
*/
    void
    OMCreator::addPorts(const Dir dir, const NetType t,
        const bool issigned, const Range* range, 
        vector<pair<char*, vector<Range*>*> >* ids)
    {
        if(!parseInfos){
            return;
        }
        if(!currScope) {
            return;
        }    
        if(currScope == currMod) 
        {
            //perhaps to be done for task/functions aswell
            vector<string> po;
            for(int i = 0; i < parseInfos->size(); ++i) {
                po.push_back((*parseInfos)[i]->name);
            }    
            currMod->setPortOrder(po);
        }    
        //const int pivsize = parseInfos->size();
        const int idsize = ids->size();

        set<string, compare_str> names;
        for(int i = 0; i < idsize; ++i)
        {
            names.insert((*ids)[i].first);
            if(!this->checkPortExists((*ids)[i].first)){
                MsgHandler::instance()->print(26, lineno, (*ids)[i].first);
                continue;
            }
            if(this->findObj((*ids)[i].first))
            {
                //char str[1024];
                //sprintf(str, "Variable '%s' already declared.",
                //(*ids)[i].first);
                //yyerror(str);
                MsgHandler::instance()->print(21, lineno, (*ids)[i].first);
                continue;
            }
            this->addPort(dir, issigned, range, (*ids)[i], 
                GetNetTypeString(t));
        }
    }

    void
    OMCreator::addPort(const Dir dir, const bool issigned, 
        const Range* range,
        pair<char*, vector<Range*>*> & id, string vartype)
    {
        if(range || id.second)
        {
            Dimension* dim = new Dimension();
            if(range){
                dim->packDim.push_back(const_cast<Range*>(range));
            }
            if(id.second){
                dim->unpackDim = *(id.second);
            }
            VecPort* vport = new VecPort(id.first, dim, dir);
            const vector<Port*> ports = vport->getPorts();
            int size = ports.size();
            for(int j = 0; j < size; ++j)
            {
                ports[j]->setSign(issigned);
                ports[j]->setVarType(vartype);
            }
            if(TaskOrFunc* tf = dynamic_cast<TaskOrFunc*>(currScope))
            {
                tf->addPorts(ports);
                tf->addObject(id.first, vport, false);
            }
            else
            {
                currMod->addPorts(ports);
                currMod->addObject(id.first, vport, false);
            }
        }
        else
        {
            Port* port = new Port(string(id.first));
            port->setSign(issigned);
            port->setDir(dir);
            port->setVarType(vartype);
            vector<Port*> ports;
            ports.push_back(port);
            if(TaskOrFunc* tf = dynamic_cast<TaskOrFunc*>(currScope))
            {
                tf->addPorts(ports);
                tf->addObject(id.first, port, false);
            }
            else
            {
                currMod->addPorts(ports);
                currMod->addObject(id.first, port, false);
            }
        }
    }

    string
    OMCreator::intToVarString(const int type) const {
        switch(type){
            case 1: return "integer";
            case 2: return "real";
            case 3: return "realtime";
            case 4: return "time";
            case 5: return "reg";
            default: return "";
        }
    }
    void
    OMCreator::addTaskFuncPort(
        const string& name, 
        const Dir dir, 
        const Range* range,
        vector<Range*>* dim, 
        const bool isSig,
        const int vartype)
    {
        pair<char*, vector<Range*>*> id = 
            make_pair(const_cast<char*>(name.c_str()), dim);
            this->addPort(
                dir, isSig, range, id, 
                intToVarString(vartype));
    } 

        
    void
    OMCreator::addV2kPorts()
    {
        if(!parseInfos){
            return;
        }
        int size = parseInfos->size();
        std::vector<ParseInfo*> removev;
        for(int i = 0; i < size; ++i)
        {
            ParseInfo* parseInfo = (*parseInfos)[i];
            if(!parseInfo){
                continue;
            }
            if(parseInfo->dir == UNSET){
                //Not V2k style declaration.
                continue;
            }
            pair<char*, vector<Range*>*> ndimp = 
                make_pair(const_cast<char*>(parseInfo->name.c_str()),
                parseInfo->dim);

            removev.push_back(parseInfo);
            Dir dir = parseInfo->dir;
            NetType t = parseInfo->net;
            bool issigned = parseInfo->isSigned;
            Range* range = parseInfo->range;
            this->addPort(dir, issigned, range, ndimp, 
                GetNetTypeString(t));
        }
        size = removev.size();
        for(int i = 0; i < size; ++i){
            std::remove(parseInfos->begin(), parseInfos->end(), 
            removev[i]);
        }
    }
    BasedNum*
    OMCreator::createBasedNum(std::string num) const
    {
        string size = num.substr(num.find_first_of("'"));
        int loc = num.size() - size.size();
        size = num.substr(0, loc);
        bool sig = (num[loc+1] == 's' || 
        num[loc+1] == 'S');
        char base = sig ? num[loc + 2] : num[loc + 1];
        num = num.substr(sig ? loc + 3 : loc + 2);
        std::remove(num.begin(), num.end(), ' ');
        std::remove(num.begin(), num.end(), '\t');
        std::remove(num.begin(), num.end(), '_');
        vector<char> bits;
        for( string::iterator c = num.begin(); c != num.end(); ++c){
            bits.push_back(*c);
        }
        try{
            return new BasedNum(size.empty() ? 32 : 
                atoi(size.c_str()), base, sig, bits);
        }
        catch (Error& e){
            return NULL;
        }
    }

    Expr*
    OMCreator::createIdUse(string id, const bool silent) const
    {
        if(id.find('.') != string::npos){
            return new HierId(id);
        }
        if(currSpecifyBlock){
            if(const Specparam* sp = currSpecifyBlock->getSpecparam(id)){
                if(sp){
                    return const_cast<Specparam*>(sp);
                }
            }
        }

        if(const Expr* expr = dynamic_cast<const Expr*>(this->findObj(id))){
            return const_cast<Expr*>(expr);
        }
        if(!silent)
        {
            //char str[1024];
            //sprintf(str, "Variable '%s' does not exist.",
            //    id.c_str());
            //yyerror(str);
            MsgHandler::instance()->print(45, lineno, id.c_str());
        }
        return NULL;
    }

    ModInst*
    OMCreator::createModInst(const string& name,
        const vector<NameConn*>* conns) const
    {
        ModInst* modInst = new ModInst(name, currInstMaster);
        Module* mod = dynamic_cast<Module*>(
            GRoot->getObject(currInstMaster));
//        if(!conns)
//        {
//            if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
//                currGenDecl->minsts.push_back(modInst);
//            }
//            else{
//                currMod->addModInst(modInst);
//            }
//            modInst->setMaster(mod);
//            return modInst;
//        }

        if(mod) {
            if(this->setMasterAndPins(modInst, mod, conns)){
                return NULL;
            }
            else{
                if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
                    currGenDecl->minsts.push_back(modInst);
                }
                else if(currMod){
                    currMod->addModInst(modInst);
                }
                return modInst;
            }
        }
        vector<Pin*> pins;
        const int size = conns ? conns->size() : 0;
        for(int i = 0; i < size; ++i){
            pins.push_back(new Pin((*conns)[i]->first, UNSET, 
                (*conns)[i]->second));
        }
        modInst->setPins(pins);
        if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
            currGenDecl->minsts.push_back(modInst);
        }
        else if(currMod){
            currMod->addModInst(modInst);
        }
        return modInst;
    }
    UDPInst*
    OMCreator::createUDPInst(const string& name,
        const vector<NameConn*>* conns) const
    {
        UDPInst* uinst = new UDPInst(name, currInstMaster);
        if(!conns)
        {
            if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
                currGenDecl->uinsts.push_back(uinst);
            }
            else{
                currMod->addUDPInst(uinst);
            }
            return uinst;
        }
        if(!this->setUDPInstPins(uinst, conns)){
            if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
                currGenDecl->uinsts.push_back(uinst);
            }
            else {
                currMod->addUDPInst(uinst);
            }
        }
        return uinst;
    }

    int
    OMCreator::setUDPInstPins(UDPInst* uinst, 
        const vector<NameConn*>* conns) const
    {
        const UDP* udp = static_cast<const UDP*>(GRoot->getObject(
            uinst->getUDPName()));
        set<string, compare_str> portnames;
        const Port* outport = udp->getOutPort();
        const vector<Port*>& inports = udp->getInPorts();

        portnames.insert(outport->getIxName().getName());
        const int size = inports.size();

        for(int i = 0; i < size; ++i) {
            portnames.insert(inports[i]->getIxName().getName());
        }
        const int numports = portnames.size();
        const int numpins = conns->size();
        if(numports != numpins)
        {
            //PrintMsg("[ERROR]: Line ", lineno, "Mismatch in port connection.");
            MsgHandler::instance()->print(27, lineno);
            return 1;
        }
        

        const bool nameConn = !(*conns)[0]->first.empty();
        vector<Pin*> pins;
        if(nameConn)
        {
            set<string, compare_str> pinnames;
            for(int i = 0; i < numpins; ++i)
            {
                if(pinnames.find((*conns)[i]->first) != pinnames.end())
                {
                    //PrintMsg("[ERROR]: Line ", lineno, "Duplicate pin name ",
                    //    (*conns)[i]->first, " specified.\n");
                    MsgHandler::instance()->print(28, lineno, (*conns)[i]->first.c_str());
                    return 1;
                }

                const Port* port = udp->getPort((*conns)[i]->first);
                if(!port)
                {
                    MsgHandler::instance()->print(26, lineno, (*conns)[i]->first.c_str());
                    return 1;
                }
                long aclen = 
                (*conns)[i]->second ? (*conns)[i]->second->len() : -1;
        //        if((aclen != -1) && (aclen != 1))
        //        {
        //            PrintMsg("[ERROR]: Line ", uinst->getLine(),
        //            " Width mismatch for port ", 
        //            port->getIxName().getName(), " expected size",
        //            1, " found ", aclen);
        //            return 1;
        //        }
                Pin* pin = new Pin(port->getIxName(), port->getDir(), 
                (*conns)[i]->second);
                pins.push_back(pin);
            }
        }
        else
        {
            long aclen = 
            (*conns)[0]->second ? (*conns)[0]->second->len() : -1;
        //    if((aclen != -1) && (aclen != 1))
        //    {
        //        PrintMsg("[ERROR]: Line ", uinst->getLine(),
        //        " Width mismatch for port ", 
        //        outport->getIxName().getName(), " expected size",
        //        1, " found ", aclen);
        //        return 1;
        //    }
            Pin* pin = new Pin(outport->getIxName().getName(),
                OUT, (*conns)[0]->second);
            pins.push_back(pin);
            for(int i = 0; i < size; ++i) 
            {
                aclen = 
                (*conns)[i+1]->second ? (*conns)[i+1]->second->len() : -1;
        //        if((aclen != -1) && (aclen != 1))
        //        {
        //            PrintMsg("[ERROR]: Line ", uinst->getLine(),
        //            " Width mismatch for port ", 
        //            inports[i]->getIxName().getName(), " expected size",
        //            1, " found ", aclen);
        //            return 1;
        //        }
                pin = new Pin(inports[i]->getIxName().getName(),
                OUT, (*conns)[i+1]->second);
                pins.push_back(pin);
            }
        }
        uinst->setPins(pins);
        return 0;
    }
            
                    

    int
    OMCreator::setMasterAndPins(
        ModInst* inst, 
        Module* mod, 
        const vector<NameConn*>* conns
        ) const
    {
        inst->setMaster(mod);
        const vector<Port*>& ports = mod->getPorts();
        const int numports = GetNumElems<Port>(ports);
        const int numpins = conns ? conns->size() : 0;

        if(numpins != numports)
        {
            //PrintMsg("[ERROR]: Line ", inst->getLine(),
            //    " Mismatch in port connection.");
            MsgHandler::instance()->print(27, inst->getLine());
            return 1;
        }
        if(numports == 0){
            return 0;
        }
        if(!conns) {
            return 0;
        }    
        vector<string> portnames = GetNames<Port>(ports);
        std::vector<Pin*> pins;
        bool isNameConn = !(*conns)[0]->first.empty();
        for(int i = 0; i < numpins; ++i)
        {
            const Base* base = isNameConn ? mod->getPort((*conns)[i]->first):
                mod->getObject(portnames[i]);
            if(const VecPort* vport = dynamic_cast<const VecPort*>(base))
            {
                VecPin* pin = new VecPin(vport->getName(), 
                    vport->getDim(), vport->getDir(), 
                    (*conns)[i]->second);

                long msb = vport->getMsb();    
                long lsb = vport->getLsb();    
                
                for(int j = msb; (msb > lsb) ? (j >= lsb) : (j <= lsb);
                (msb > lsb) ? --j : ++j)
                {
                    const ScalarCntv* ctv = pin->getBit(j);
                    pins.push_back(const_cast<Pin*>(
                        static_cast<const Pin*>(ctv)));
                }
                inst->addVecPin(pin);
            }

            else if(const Port* port = dynamic_cast<const Port*>(base))
            {
                long polen = port->len();
                Pin* pin = new Pin(isNameConn ? (*conns)[i]->first : 
                portnames[i], port->getDir(), (*conns)[i]->second);
                pins.push_back(pin);
            }
            else{
                //PrintMsg("[ERROR]: Line", inst->getLine(),
                //    " Port ", isNameConn ? (*conns)[i]->first :
                //    portnames[i], " not found in the module.");
                MsgHandler::instance()->print(26, inst->getLine(), 
                    isNameConn ? (*conns)[i]->first.c_str() :
                    portnames[i].c_str());
                return 1;
            }
        }
        inst->setPins(pins);

        return 0;
    }
                
        

    Inst*
    OMCreator::createInst(const string& name, 
        const vector<NameConn*>* conns) const
    {
        if(this->findObj(name, currScope))
        {
            MsgHandler::instance()->print(8, lineno, name.c_str(), "instance");
            return 0;
        }
        this->checkAndAddImplicitNet(conns);
        if(currGateType == UNSETGATE){
            if(dynamic_cast<const UDP*>(GRoot->getObject(
                currInstMaster))){
                return this->createUDPInst(name, conns);
            }
            return createModInst(name, conns);
        }
        GateInst* ginst = new GateInst(name, currGateType);
        if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
            currGenDecl->ginsts.push_back(ginst);
        }
        else {
            currMod->addGateInst(ginst);
        }
        if(!conns){
            return ginst;
        }
        //Naming convention for pin is:
        //output --> o, inputs in0, in1...
        //TBD: To check no. of inputs/outputs/inouts etc
        int size = conns->size();
        vector<Pin*> pins;
        switch(currGateType)
        {
            case AND:
            case NAND:
            case OR:
            case NOR:
            case XOR:
            case XNOR:
            case BUFIF0:
            case BUFIF1:
            case NOTIF0:
            case NOTIF1:
            case NMOS:
            case PMOS:
            case RNMOS:
            case RPMOS:
            {
                Pin* pin = new Pin(IxName("o"), OUT, conns->empty() ? 0 :
                    (*conns)[0]->second);
                pins.push_back(pin);
                char str[32];
                for(unsigned int i = 1; i < size; ++i)
                {
                    sprintf(str, "i%d", i-1);
                    pins.push_back(new Pin(IxName(str), IN, (*conns)[i]->second));
                }
                for(unsigned int i = size; i <= 2; ++i)
                {
                    sprintf(str, "i%d", i-1);
                    pins.push_back(new Pin(IxName(str), IN, 0));
                }
                break;
            }
            case RTRANIF0:
            case RTRANIF1:
            case TRANIF0:
            case TRANIF1:
            {
                pins.push_back(new Pin(IxName("io0"), INOUT, !conns->empty() ?  (*conns)[0]->second : 0));
                pins.push_back(new Pin(IxName("io1"), INOUT, (size > 1) ? (*conns)[1]->second : 0));
                pins.push_back(new Pin(IxName("i0"), INOUT, (size > 2) ? (*conns)[2]->second : 0));
                break;
            }
            case TRAN:
            case RTRAN:
            {
                pins.push_back(new Pin(IxName("io0"), INOUT, (size > 1) ? (*conns)[0]->second : 0));
                pins.push_back(new Pin(IxName("io1"), INOUT, (size > 2) ? (*conns)[1]->second : 0));
                break;
            }
            case CMOS:
            case RCMOS:
            {
                pins.push_back(new Pin(IxName("o"), OUT, (size > 0) ? (*conns)[0]->second : 0));
                pins.push_back(new Pin(IxName("i0"), INOUT, (size > 1) ? (*conns)[1]->second : 0));
                pins.push_back(new Pin(IxName("i1"), INOUT, (size > 2) ? (*conns)[2]->second : 0));
                pins.push_back(new Pin(IxName("i2"), INOUT, (size > 3) ? (*conns)[3]->second : 0));
                break;
            }
            case BUF:
            case NOT:
            {
                pins.push_back(new Pin(IxName("o"), OUT, (size > 0) ? (*conns)[0]->second : 0));
                pins.push_back(new Pin(IxName("i0"), IN, (size > 1) ? (*conns)[1]->second : 0));
                break;
            }
            case PULLUP:
            case PULLDOWN:
                pins.push_back(new Pin(IxName("o"), OUT, size != 0 ? (*conns)[0]->second : 0));
                break;
            default: {
                break;
            }
        }
        ginst->setPins(pins);
        return ginst;
    }

    void
    OMCreator::setDelayAndDrStr(vector<Inst*>* ginsts, 
        Delay* delay, DriveSt* dst) const
    {
        if(!ginsts){
            return;
        }
        const int size = ginsts->size();
        for(int i = 0; i < size; ++i)
        {
            GateInst* ginst = static_cast<GateInst*>((*ginsts)[i]);

            ginst->setDelay(delay);
            ginst->setDrStr(dst);
        }
        currGateType = UNSETGATE;
    }

    void
    OMCreator::createContAssigns(const DriveSt* dst, const Delay* delay,
        vector<pair<Expr*, Expr*>* >* assigns)
    {
        const int size = assigns->size();
        for(int i = 0; i < size; ++i) {
            Expr *lhs =  (*assigns)[i]->first;
            if(const VString *str = dynamic_cast<VString*>(lhs)) {
                lhs = this->checkAndAddImplicitNet(str);
            }    
            ContAssign* ca = new ContAssign(delay, dst,
                lhs, (*assigns)[i]->second);
            if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
                currGenDecl->contassigns.push_back(ca);
            }
            else if(currMod){
                currMod->addContAssign(ca);
            }
        }
        for(int i = 0; i < size; ++i) {
            delete (*assigns)[i];
        }
        delete assigns;
    }
    void
    OMCreator::createBlockDecls(AttribTable* table, 
        vector<RegInfo*>* regInfos, 
        Range* range, bool isSigned, string type)
    {
        if(!currScope) {
            return;
        }    
        const int size = regInfos->size();

        for(int i = 0; i < size; ++i)
        {
            RegInfo* varInfo = (*regInfos)[i];
            Dimension* dim = NULL;
            if(range || (varInfo->dim))
            {
                dim = new Dimension();
                if(range){
                    dim->packDim.push_back(range);
                }
                if(varInfo->dim){
                    dim->unpackDim = *(varInfo->dim);
                }
            }
            pair<char*, Expr*> charexprpair = make_pair(
                    varInfo->name, varInfo->expr);

            if(this->handlePortRegNet(table, REG, isSigned, range,
                        varInfo->dim, charexprpair)){
                continue;
            }

            if(type == string("reg"))
            {
                if(const VecPort* vport = dynamic_cast<const VecPort*>(
                    this->findObj(varInfo->name, currScope)))
                {
                    const_cast<VecPort*>(vport)->setVarType("reg");
                    continue;
                }
                if(const Port* port = dynamic_cast<const Port*>(this->findObj(
                    varInfo->name, currScope))) {
                    if(currScope == currMod)
                    {
                        const_cast<Port*>(port)->setVarType("reg");
                        continue;
                    }
                }

                if(!dim)
                {
                    Reg* reg = new Reg(IxName(varInfo->name));
                    reg->setSign(isSigned);
                    currScope->addObject(varInfo->name, reg, false);
                    reg->setAttribTable(table);
                    if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
                        currGenDecl->variables.push_back(reg);
                    }
                }
                else
                {
                    VecReg* vreg = new VecReg(varInfo->name, dim, isSigned);
                    vreg->setAttribTable(table);
                    currScope->addObject(varInfo->name, vreg, false);
                    this->addVecVarToModGenDecl(vreg);
                }
            }
            else if(type == string("integer"))
            {
                if(!dim)
                {
                    Integer* int_ = new Integer(IxName(varInfo->name));
                    currScope->addObject(varInfo->name, int_, false);
                    int_->setAttribTable(table);
                    if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
                        currGenDecl->variables.push_back(int_);
                    }
                }
                else 
                {
                    VecInt* vint = new VecInt(varInfo->name, dim, true);
                    currScope->addObject(varInfo->name, vint, false);
                    vint->setAttribTable(table);
                    this->addVecVarToModGenDecl(vint);
                }
            }
            else if(type == string("time"))
            {
                if(!dim)
                {
                    Time* time = new Time(IxName(varInfo->name));
                    currScope->addObject(varInfo->name, time, false);
                    time->setAttribTable(table);
                    if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
                        currGenDecl->variables.push_back(time);
                    }
                }
                else
                {
                    VecTime* vtime = new VecTime(varInfo->name, dim, false);
                    currScope->addObject(varInfo->name, vtime, false);
                    vtime->setAttribTable(table);
                    this->addVecVarToModGenDecl(vtime);
                }
            }
        }
    }

    void 
    OMCreator::createBlockDecls(
        AttribTable* table,
        vector<pair<char*, vector<Range*>*> >* ids, 
        std::string type)
    {
        const int size = ids->size();
        for(int i = 0; i < size; ++i)
        {
            Dimension* dim = NULL;
            if((*ids)[i].second) {
                dim = new Dimension();
                dim->unpackDim = *(*ids)[i].second;
                //dim = new Dimension(vector<Range*>(), *(*ids)[i].second);
            }
            Base* base = 0;
            if(type == string("real"))
            {
                if(dim){
                    base = new VecReal((*ids)[i].first, dim, false);
                }
                else{
                    base = new Real(IxName((*ids)[i].first), NULL);
                }
            }
            else if(type == string("realtime"))
            {
                if(dim){
                    base = new VecRealTime((*ids)[i].first, dim, false);
                }
                else{
                    base = new RealTime(IxName((*ids)[i].first), NULL);
                }
            }
            base->setAttribTable(table);
            currScope->addObject((*ids)[i].first, base, false);
        }
    }

    void
    OMCreator::addNamedBlock(NamedBlockBase* nb)
    {
        //if(!dynamic_cast<Module*>(currScope)){
        //    currScope->addStmt(nb);
        //}
        nb->setScope(currScope);
        currScope = nb;
    }
            
    void
    OMCreator::createTask(const std::string& name, 
        const bool isAuto)
    {
        if(this->findObj(name, currScope))
        {
            //char str[1024];
            //sprintf(str, "Variable '%s' used as task name already declared.",
            //name.c_str());
            //yyerror(str);
            MsgHandler::instance()->print(8, lineno, name.c_str(), "task");
            return;
        }
            
        Task* task = new Task(name, NULL, isAuto);
        task->setScope(currScope);
        currScope = task;
        if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
            currGenDecl->tasks.push_back(task);
        }
        else{
            currMod->addTask(task);
        }
    }

    void
    OMCreator::createFunc(const std::string& name,
        const bool isAuto, const bool isSigned,
        pair<int, Range*>* rangeOrType)
    {
        if(this->findObj(name, currScope))
        {
            //char str[1024];
            //sprintf(str, "Variable '%s' used as function name already declared.",
            //name.c_str());
            //yyerror(str);
            MsgHandler::instance()->print(8, lineno, name.c_str(), "function");
            return;
        }
        Expr* funcExpr = 0;
        std::string type;
        if(rangeOrType && !rangeOrType->second)
        {
            if(rangeOrType->first == 1){
                type = string("integer");
                funcExpr = new Integer(name);
            }
            else if(rangeOrType->first == 2){
                type = string("real");
                funcExpr = new Real(name);
            }
            else if(rangeOrType->first == 3){
                type = string("realtime");
                funcExpr = new RealTime(name);
            }
            else if(rangeOrType->first == 4){
                type = string("time");
                funcExpr = new Time(name);
            }
            else if(rangeOrType->first == 5){
                type = string("reg");
                funcExpr = new Reg(name);
            }
        }
        Func* func = new Func(name, isAuto, type,
            rangeOrType ? rangeOrType->second : 0, isSigned);
        func->setScope(currScope);
        currScope = func;
        if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
            currGenDecl->funcs.push_back(func);
        }
        else{
            currMod->addFunc(func);
        }
        //Function name is used to return value, so add
        //this name in symbol table
        if(!funcExpr) {
            if(rangeOrType && rangeOrType->second)
            {
                //sign to be set :tbd
                Dimension* dim = new Dimension();
                dim->unpackDim.push_back(rangeOrType->second);
                VecReg* vreg = new VecReg(name, dim, false);
                func->addObject(name, vreg, false);
            }
            else{
                Reg* reg = new Reg(name);
                func->addObject(name, reg, false);
            }
        }
        else {
            func->addObject(name, funcExpr, isSigned);
        }    
                
        if(rangeOrType){
            delete rangeOrType;
        }
    }

    void
    OMCreator::createAlwaysBlock(AttribTable* table, 
        const Stmt* st)
    {
        Always* always = new Always(st);
        always->setAttribTable(table);
        if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
            currGenDecl->alwaysBlocks.push_back(always);
        }
        else if(currMod){
            currMod->addAlways(always);
        }
    }
    void
    OMCreator::createInitialBlock(AttribTable* table, 
        const Stmt* st)
    {
        Initial* init = new Initial(st);
        init->setAttribTable(table);
        if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
            currGenDecl->initials.push_back(init);
        }
        else {
            currMod->addInitial(init);
        }
    }

    void
    OMCreator::addDefparams(vector<Defparam*>* d)
    {
        if(d){
            const int size = d->size();
            for(int i = 0; i < size; ++i){
                if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
                    currGenDecl->defparams.push_back((*d)[i]);
                }
                else {
                    currMod->addDefparam((*d)[i]);
                }
            }
            delete d;
        }
    }

    void
    OMCreator::createEvents(vector<pair<char*, vector<Range*>*> >* ids)
    {
        if(!ids){
            return;
        }
        const int size = ids->size();
        for(int i = 0; i < size; ++i)
        {
            if(this->findObj((*ids)[i].first))
            {
        //        char str[1024];
        //        sprintf(str, "Variable '%s' already declared.",
        //        (*ids)[i].first);
        //        yyerror(str);
                MsgHandler::instance()->print(21, (*ids)[i].first);
                continue;
            }
            EventVar* ev = new EventVar((*ids)[i].first);
            if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
                //currGenDecl->events.push_back(es);
            }
            else {
                currScope->addObject((*ids)[i].first, ev, true);
                //currMod->addEventVar(ev);
            }
        }
        //TBD
        /*
        if(!ids){
            return;
        }
        const int size = ids->size();
        for(int i = 0; i < size; ++i)
        {
            Expr* idUse = this->createIdUse((*ids)[i].first);
            EventStmt* es = new EventStmt(idUse);
            if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
                currGenDecl->events.push_back(es);
            }
            else {
                currMod->addEvent(new EventStmt(idUse));
            }
        }
        delete ids;
        */
    }

    void
    OMCreator::checkPortDecl() const
    {
        if(!parseInfos){
            return;
        }
        set<string, compare_str> names;
        const int size = parseInfos->size();
        for(int i = 0; i < size; ++i)
        {
            ParseInfo* parseInfo = (*parseInfos)[i];
            if(names.find(parseInfo->name) != names.end()) {
                MsgHandler::instance()->print(25, lineno, parseInfo->name.c_str());
                //PrintMsg("[ERROR] : ", lineno, "port ", parseInfo->name,
                //    "already declared.\n");
            }
            names.insert(parseInfo->name);
        }
    }

    SpecifyEdgePath*
    OMCreator::createSpecifyEdgePath(EdgeType edge, vector<Expr*>* ids,
        char pol, SpecConnType sctype, const string& id, Expr* primExpr,
        PolOp pop, Expr* expr)
    {
        Expr* destId = NULL;
        if(!id.empty())
        {
            destId = this->createIdUse(id);
            if(!destId){
                return NULL;
            }
        }

        SpecifyEdgePath* sep = new SpecifyEdgePath(edge, destId, pop, 
            make_pair(primExpr, expr), pol, sctype, *ids);
        delete ids;
        return sep;
    }

    void
    OMCreator::createSpecifyBlock()
    {
        currSpecifyBlock = new SpecifyBlock();
        currMod->addSpecifyBlock(currSpecifyBlock);
    }

    void
    OMCreator::addSpecparamDecl(const vector<Specparam*>* specparams)
    {
        if(!specparams){
            return;
        }

        currSpecifyBlock->addSpecparams(*specparams);
        delete specparams;
    }

    void
    OMCreator::addSpecPath(const SpecifyPathBase* s)
    {
        if(!s){
            return;
        }

        currSpecifyBlock->addSpecPath(s);
    }

    void
    OMCreator::createSpecTask(const string& name, const SpecRefEvent* sre1,
        const SpecRefEvent* sre2, const DelayVal* delay1, 
        const DelayVal* delay2, const vector<Expr*>& notifiers)
    {
        SpecTask* specTask = 
        new SpecTask(name, make_pair(const_cast<SpecRefEvent*>(sre1),
            const_cast<SpecRefEvent*>(sre2)), 
            make_pair(const_cast<DelayVal*>(delay1), 
            const_cast<DelayVal*>(delay2)), notifiers);

        currSpecifyBlock->addSpecTask(specTask);
    }

    void
    OMCreator::createUDPPorts(
        const std::vector<ParseInfo*>* pinfos,
        const std::vector<char*>* udphdecls)
    {
        set<string, compare_str> headdecls;
        
        if(udphdecls)
        {
            const int size = udphdecls->size();
            for(int i = 0; i < size; ++i){
                headdecls.insert((*udphdecls)[i]);
            }
        }

        const int size = pinfos->size();
        for(int i = 0; i < size; ++i)
        {
            ParseInfo* pinfo = (*pinfos)[i];
            if(pinfo->name.empty())
            {
                const int idcount = pinfo->ids.size();
                for(int j = 0; j < idcount; ++j)
                {
                    if(headdecls.find(pinfo->ids[j]) == headdecls.end())
                    {
                        MsgHandler::instance()->print(26, lineno, pinfo->ids[j]);
                        //PrintMsg("[ERROR]: ", lineno, "Port ", 
                        //    pinfo->ids[j], " not found in udp header.");
                        continue;
                    }
                    Port* port = new Port(IxName(pinfo->ids[j]));
                    port->setSign(false);
                    port->setVarType(GetNetTypeString(pinfo->net));
                    port->setDir(pinfo->dir);
                    currUDP->addInPort(port);
                }
            }
            else
            {
                if(headdecls.find(pinfo->name) == headdecls.end())
                {
                //    PrintMsg("[ERROR]: ", lineno, "Port ", 
                //        pinfo->name, " not found in udp header.");
                    MsgHandler::instance()->print(26, lineno, pinfo->name.c_str());
                    continue;
                }
                Port* port = new Port(IxName(pinfo->name));
                port->setSign(false);
                port->setVarType(GetNetTypeString(pinfo->net));
                port->setDir(pinfo->dir);
                if(pinfo->dir == OUT){
                    currUDP->setOutPort(port);
                }
                else{
                    currUDP->addInPort(port);
                }
            }
        }
        delete pinfos;
        if(udphdecls){
            delete udphdecls;
        }
    }

    void
    OMCreator::createUDPPorts(const string& outport, const Expr* initexpr, 
        const vector<char*>* inports, const bool isOutReg)
    {
        currUDP->setInitVal(initexpr);
        Port* port = new Port(outport);
        if(isOutReg){
            port->setVarType("reg");
        }
        port->setDir(OUT);
        currUDP->setOutPort(port);
        const int size = inports->size();

        for(int i = 0; i < size; ++i)
        {
            port = new Port(IxName((*inports)[i]));
            port->setDir(IN);
            currUDP->addInPort(port);
        }
        delete inports;
    }

    void
    OMCreator::addTableEntry(const TableEntry* t){
        if(t) {
            currUDP->addTableEntry(t);
        }
    }

    void
    OMCreator::setUDPInitVal(const Expr* expr){
        currUDP->setInitVal(expr);
    }

    PartSel* 
    OMCreator::createPartSelect(const std::string& id,
        const Range* range, const std::vector<Expr*>* dim,
        PolOp polop) const
    {
        
        std::vector<Expr*> tdim;
        if(dim)
        {
            tdim = *dim;
            delete dim;
        }
        Expr* expr = this->createIdUse(id);
        if(!expr){
            return 0;
        }
        PartSel* partsel = 0;
        switch(ParseDimension(tdim, expr, true))
        {
            case 0:
            {
                //in case of v2k style range specification
                //we are not doing checks on the range
                int status = (polop != POCOL) ? 0 : 
                    ParseRange(range, expr);
                switch(status)
                {
                    case 0:    
                        partsel = new PartSel(expr, tdim, range, polop);
                        break;
                    case 1:
                        MsgHandler::instance()->print(29, lineno, "part");
                        break;
                    case 2:
                        MsgHandler::instance()->print(30, lineno, "part");
                        break;
                    case 3:
                        MsgHandler::instance()->print(29, lineno, "part");
                        break;
                    case 4:
                        MsgHandler::instance()->print(31, lineno);
                        break;
                    default:
                        break;
                }
            }
                break;
                    
            case 1:
                //PrintMsg("[ERROR]: Line ", lineno, 
                //    " Illegal part select for scalar variable.");
                MsgHandler::instance()->print(30, lineno, "part");
                break;
            case 2:
                //PrintMsg("[ERROR]: Line ", lineno, 
                //    " Illegal dimension select.");
                MsgHandler::instance()->print(32, lineno);
                break;
            case 3:
                //PrintMsg("ERROR]: Line ", lineno, 
                //    " Part select out of range.");
                MsgHandler::instance()->print(29, lineno, "part");
                break;
            case 4:
                break;
            default:
                break;
        }
        return partsel;
    }

    Expr* 
    OMCreator::createBitSelect(const std::string& id, 
        const std::vector<Expr*>* dim)const
    {
        if(!dim){
            return 0;
        }    

        Expr* expr = this->createIdUse(id);
        if(!expr){
            return 0;
        }
        if(Range* range = this->checkIfPartSelect(expr, dim)){
            return this->createPartSelect(id, range, dim, POCOL);
        }
        const std::vector<Expr*> tdim = *dim;
        BitSel* bitsel = 0;
        switch(ParseDimension(tdim, expr))
        {
            case 0:
                bitsel = new BitSel(expr, tdim);
                break;
            case 1:
                //PrintMsg("[ERROR]: Line ", lineno, 
                //    " Illegal bit select for scalar variable.");
                MsgHandler::instance()->print(30, lineno, "bit");
                break;
            case 2:
                //PrintMsg("[ERROR]: Line ", lineno, 
                //    " Illegal dimension select.");
                MsgHandler::instance()->print(32, lineno);
                break;
            case 3:
                //PrintMsg("[ERROR]: Line ", lineno, 
                //    " Bit select out of range.");
                MsgHandler::instance()->print(29, lineno, "bit");
                break;
            case 4:
                break;
            default:
                break;
        }
        delete dim;
        return bitsel;
    }

    void
    OMCreator::addStmt(const Stmt* st) {
        if(st){
            currScope->addStmt(st);
        }
    }
    void
    OMCreator::setTaskOrFuncStmt(const Stmt* st) {
        if(TaskOrFunc* tf = dynamic_cast<TaskOrFunc*>(currScope)){
            tf->setStmt(st);
        }
    }

    void
    OMCreator::createGenStmt() {
        currGenStmt = new GenerateStmt();
        GenDecl* gd = new GenDecl();
        gd->setGenDecl(new ModGenDecl());
        currGenStmt->addGenerateItem(gd);
        currMod->addGenerate(currGenStmt);
        currScope = currGenStmt;
        genItems.push(gd);
    }

    GenerateCase*
    OMCreator::createGenCase(const Expr* condexpr)
    {
        GenerateCase* gcase = new GenerateCase(condexpr);
        genItems.push(gcase);
        //gcase->setGenDecl(new ModGenDecl());
        return gcase;
    }

    GenerateCond*
    OMCreator::createGenCond(
        const Expr* condexpr
    ) {
        GenerateCond* gc = new GenerateCond(condexpr);
        genItems.push(gc);
        //gc->setGenDecl(new ModGenDecl());
        return gc;
    }

    GenerateLoop*
    OMCreator::createGenLoop(
        const Expr* loopvar,
        const Expr* initexpr,
        const Expr* termexpr,
        const Expr* assignexpr
    ){
        GenerateLoop* gl = new GenerateLoop(
            loopvar, initexpr, termexpr, assignexpr);
        //gl->setGenDecl(new ModGenDecl());
        genItems.push(gl);
        currScope = gl;
        return gl;
    }

    GenerateBlock*
    OMCreator::createGenBlock( const std::string& name)
    {
        GenerateBlock* gb = new GenerateBlock(name);
        //gb->setGenDecl(new ModGenDecl());
        currScope = gb;
        genItems.push(gb);
        return gb;
    }

    void
    OMCreator::addVecVarToModGenDecl(const VecVar* vvar)
    {
        ModGenDecl* currGenDecl = this->getCurrGenDecl();
        if(!currGenDecl || ! vvar){
            return;
        }
        const vector<Var*>& vars = vvar->getVars();
        const int size = vars.size();
        for(int i = 0; i < size; ++i) {
            currGenDecl->variables.push_back(vars[i]);
        }
    }

    bool
    OMCreator::idExists(
        const std::string& name, 
        const Scope* scope
    ) const
    {
        if(this->findObj(name, scope))
        {
            //PrintMsg("[ERROR] : Line ", lineno, "Variable ",
            //    name, "already declared.\n");
            MsgHandler::instance()->print(21, lineno, name.c_str());
            return true;
        }
        return false;
    }
    Genvar*
    OMCreator::createGenvar(const string& name)
    {
        if(this->idExists(name, currScope)){
            return NULL;
        }
        Genvar* g = new Genvar(name);
        if(ModGenDecl* currGenDecl = this->getCurrGenDecl()){
            currGenDecl->genvars.push_back(g);
        }
        else{
            currMod->addGenvar(g);
        }
        currScope->addObject(name, g, false);
        return g;
    }

    void
    OMCreator::setGenerateItems(const vector<GenerateItem*>* gitems)
    {
        if(gitems){
            const int size = gitems->size();
            for(int i = 0; i < size; ++i){
                currGenStmt->addGenerateItem((*gitems)[i]);
            }
            delete gitems;
        }
    }

    ModGenDecl*
    OMCreator::getCurrGenDecl() const {
        if(!currGenStmt){
            return NULL;
        }
        if(genItems.empty()) {
            return currGenStmt->getDecl()->getGenDecl();
        }
        if(!dynamic_cast<GenDecl*>(genItems.top()))
        {
            GenDecl* gd = new GenDecl();
            gd->setGenDecl(new ModGenDecl());
            genItems.push(gd);
        }
        return genItems.top()->getGenDecl();
    }

    void
    OMCreator::removeGenItem(const bool onlydecl)
    {
        while(!genItems.empty() &&
            dynamic_cast<GenDecl*>(genItems.top())){
           // if(!genItems.empty()){
                genItems.pop();
           // }
        }
        if(!onlydecl && !genItems.empty()){
            genItems.pop();
        }
    }

    File*
    OMCreator::createIncludeFile(const std::string& name)
    {
        const vector<string>& inclDirs = GRoot->getInclDirs();

        for(unsigned int i = 0; i < inclDirs.size(); ++i)
        {
            string newname = inclDirs[i] + string("/") + name;
            File* fileObj = new File(newname, File::INCL);
            FILE* file = fileObj->open(true);
            if(file)
            {
                fclose(file);
                return fileObj;
            }
            delete fileObj;
        }
        return 0;
    }

    bool 
    OMCreator::handlePortRegNet(
            const AttribTable* attrib,
            const NetType ntype,
            const bool issigned,
            const Range* range,
            const vector<Range*>* ranges,
            pair <char*, Expr*>& id
            )
    {
        Module* mod = dynamic_cast<Module*>(currScope);
        TaskOrFunc* tf = dynamic_cast<TaskOrFunc*>(currScope);
        if(!mod && !tf){
            return false;
        }

        //See if the net is associated with a scalar port
        Port* port = const_cast<Port*>(dynamic_cast<const Port*>(
                    mod ? mod->getObject(id.first): tf->getObject(id.first)));

        if(port)
        {
            if(!range && !ranges)
            {
                //modify certain arributes;
                port->setVarType(GetNetTypeString(ntype));
                port->setAttribTable(attrib);
                if(!port->isSigned()){
                    port->setSign(issigned);
                }
                return true;
            }
            //convert scalar port to vector port

            Dimension* dim = new Dimension();
            if(range){
                dim->packDim.push_back(const_cast<Range*>(range));
            }
            if(ranges){
                for(unsigned int i = 0; i < ranges->size(); ++i){
                    dim->unpackDim.push_back((*ranges)[i]);
                }
            }
            bool sign = port->isSigned() ? true : issigned;
            VecPort* vport = new VecPort(port->getIxName().getName(),
                    dim, port->getDir());
            string vartype = GetNetTypeString(ntype);
            vport->setAttribTable(attrib);

            const vector<Port*> ports = vport->getPorts();
            int size = ports.size();
            for(int j = 0; j < size; ++j)
            {
                ports[j]->setSign(sign);
                ports[j]->setVarType(vartype);
            }
            vector<Port*>& oldports = const_cast<vector<Port*>&>(
                    mod ? mod->getPorts(): tf->getPorts());
            vector<Port*>::iterator iter = oldports.begin();
            vector<Port*>::iterator eiter = oldports.end();
            for(; iter != eiter; ++iter)
            {
                if((*iter)->getIxName().getName() == id.first){
                    break;
                }
            }
            oldports.erase(remove(oldports.begin(), oldports.end(), *iter));
            if(mod) {
                mod->addPorts(ports);
            }
            else {
                tf->addPorts(ports);
            }
            currScope->addObject(id.first, vport, true);
            return true;
        }
        else if(VecPort* vport = const_cast<VecPort*>(
                    dynamic_cast<const VecPort*>(currScope->getObject(id.first))))
        {
            //change attributes;
            //if(!vport->isSigned()){
            //    vport->setSign(issigned);
            //}
            vport->setAttribTable(attrib);
            string vartype = GetNetTypeString(ntype);
            vport->setVarType(vartype);
            //Note that we are not changing original dimensions
            return true;
        }
        return false;
    }

    Range*
    OMCreator::checkIfPartSelect(
            const Expr* id,
            const vector<Expr*>* dimSelect) const 
    {
        if(!id){
            return 0;
        }
        const Integer* i = dynamic_cast<const Integer*>(id);
        const Time* t =  dynamic_cast<const Time*>(id);

        if(i || t){
            if(dimSelect){
                //This is a bit select
                return 0;
            }
            return new Range(new Num(31), new Num(0));
        }
        const VecCntv* vcntv = dynamic_cast<const VecCntv*>(id);
        if(!vcntv){
            return 0;
        }

        const Dimension* dim = vcntv->getDim();
        const unsigned int select = dim->unpackDim.size() + dim->packDim.size();
        if(dimSelect->size() == (select - 1))
        {
            const Range* range = 0;
            if(!(dim->packDim.empty())){
                range = dim->packDim[dim->packDim.size() -1];
            }
            else{
                range = dim->unpackDim[dim->unpackDim.size() -1];
            }
            return new Range(range->getLExpr()->copy(), range->getRExpr()->copy());
        }
        return 0;
    }

    void
    OMCreator::checkAndAddImplicitNet(const vector<NameConn*>* conns) const
    {
        if(!conns || !currMod){
            return;
        }
        set<string, compare_str> names;
        for(unsigned int i = 0; i < conns->size(); ++i)
        {
            ImplicitNet* inet = dynamic_cast<ImplicitNet*>(
                    (*conns)[i]->second);
            if(!inet){
                continue;
            }
            if(names.find(inet->getNet()->getIxName().getName()) == names.end())
            {
                vector<Net*> nets;
                nets.push_back(inet->getNet());
                currMod->addNets(nets);
                names.insert(inet->getNet()->getIxName().getName());
                (*conns)[i]->second = inet->getNet();
            }
            else 
            {
                const Net* net = dynamic_cast<const Net*>(
                        currMod->getObject(inet->getNet()->getIxName().getName()));
                (*conns)[i]->second = const_cast<Net*>(net);
            }
        }
    }

    Net*
    OMCreator::checkAndAddImplicitNet(const VString* str) const
    {
        Net *net = new Net(str->getString());

        if(ModGenDecl *decl = this->getCurrGenDecl()) {
            decl->nets.push_back(net);
        }
        else if(currMod)
        {
            vector<Net*> nets;
            nets.push_back(net);
               currMod->addObject(str->getString(), net, false);
               currMod->addNets(nets);
        }
        net->setScope(currScope);
        net->setNetType(defNetType);
        net->setInitExpr(0);
        return net;
    }

    Range*
    OMCreator::createRange(const Expr *msb, const Expr *lsb)
    {
        if(!msb->isConstExpr() || !lsb->isConstExpr()) {
            MsgHandler::instance()->print(51, lineno);
            return 0;
        }

        return new Range(msb, lsb);
    }

    const Expr*
    OMCreator::replaceVStringInParamExpr(const Expr *expr) const
    {
        string name;
        if(const VString *str = dynamic_cast<const VString*>(expr)) {
            name = str->toString();
        }
        else if(const ImplicitNet *net = dynamic_cast<const ImplicitNet*>(expr)) {
            name = net->getNet()->getIxName().getName();
        }    
        if(!name.empty())
        {
            const Base *obj = GetObject(name, currScope);
            const ScalarCntv *scntv = dynamic_cast<const ScalarCntv*>(obj);
            const VecCntv *vcntv = dynamic_cast<const VecCntv*>(obj);
            if(scntv || vcntv) {
                if((scntv && !scntv->isConstExpr()) || 
                    (vcntv && !vcntv->isConstExpr())) 
                {
                    MsgHandler::instance()->print(53, expr->getLine() );
                    return 0;
                }    
                if(scntv) {
                    return scntv;
                }
                else {
                    return vcntv;
                }    
                
            }
        }
        //Note complex expression as assign value not supported
    
        if(!expr->isConstExpr()) 
        {
            MsgHandler::instance()->print(53, expr->getLine() );
            return 0;
        }
        //else {
        //    MsgHandler::instance()->print(52, expr->getLine(), "complex expression in parameter assign " );
        //}
        return expr;
    }
}
