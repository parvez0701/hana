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

#include <assert.h>

#include "semcheck.h"
#include "root.h"
#include "module.h"
#include "modinst.h"
#include "omcreator.h"
#include "pin.h"
#include "utils.h"
#include "vecport.h"
#include "vecpin.h"
#include "vecparam.h"
#include "udp.h"
#include "udpinst.h"
#include "param.h"
#include "hierid.h"
#include "defparam.h"
#include "semanticchecker.h"
#include "genunroll.h"
#include "elaborator.h"

#include "msghandler.h"

using std::string;
using std::set;
using std::list;
extern FILE* yyin;
extern int yyparse();
namespace Rtl
{
    extern Root* GRoot;
    SemCheck* SemCheck::semcheck = 0;
    SemCheck::SemCheck(){
    }

    SemCheck::~SemCheck(){
        semcheck = 0;
    }

    SemCheck*
    SemCheck::instance()
    {
        if(!semcheck){
            semcheck = new SemCheck();
        }
        return semcheck;
    }

    int
    SemCheck::doSemChecks()
    {
        if(OMCreator::instance()->getErrCount() != 0){
            return 1;
        }
        //PrintMsg("[INFO]: Doing semantic checks .....\n");
        MsgHandler::instance()->print(4);
        int status = 0;
        
        status += this->checkAndUnrollGenerateStatements();

        const vector<File*>& libfiles = GRoot->getLibFiles();
        const vector<File*>& files = GRoot->getFiles();
        status += this->resolveInstMaster(files);
        while(1)
        {
            if(undefMods.empty()){
                break;
            }

            set<string, compare_str> tundefMods;
            tundefMods = undefMods;

            if(this->readLibFiles()){
                return 1;
            }

            status += this->resolveInstMaster(libfiles);
            if(tundefMods == undefMods){
                //No new modules read.
                break;
            }
        }
        //status += this->resolveDefparams();
        status += this->checkForRecursiveInstantiation();
        if(status) {
            return status;
        }    

        GRoot->populateTopModules();
        status += SemCheck::instance()->resolveDefparams();
        if(!status){
            Elaborator::instance()->elaborate();
        }
        SemanticChecker semanticChecker;
        status += this->doSemChecks(&semanticChecker);
        
        
        //PrintMsg("[INFO]: Semantic checks done .....\n");
        MsgHandler::instance()->print(5);
        return status;
    }

    int
    SemCheck::resolveInstMaster()
    {
        int status = 0;
        const vector<File*>& files = GRoot->getFiles();
        const vector<File*>& libfiles = GRoot->getLibFiles();

        status += this->resolveInstMaster(files);
        status += this->resolveInstMaster(libfiles);
        return status;
    }

    int 
    SemCheck::resolveInstMaster(const vector<File*>& files)
    {
        int status = 0;

        const int filec = files.size();

        for(int i = 0; i < filec; ++i)
        {
            File* file = files[i];
            const vector<Module*>& modules = file->getModules();
            const int modc = modules.size();
            for(int j = 0; j < modc; ++j)
            {
                list<ModInst*>& minsts = 
                const_cast<list<ModInst*>&>(modules[j]->getModInsts());
                vector<ModInst*> uinsts;
                list<ModInst*>::iterator iter = minsts.begin();
                list<ModInst*>::iterator eiter = minsts.end();
                for(; iter != eiter; ++iter)
                {
                    ModInst* minst = *iter;
                    if(minst->getMaster()){
                        continue;
                    }
                    const Base* base = 
                        GRoot->getObject(minst->getModName());
                    const Module* master = 
                        dynamic_cast<const Module*>(base);
                    const UDP* udp = 
                        dynamic_cast<const UDP*>(base);

                    if(!master && !udp) {
                        undefMods.insert(minst->getModName());
                    }
                    else if(master){
                        minst->setMaster(master);
                        status += this->resolvePins(minst);
                        status += this->resolveInstParams(minst);
                    }
                    else {
                        uinsts.push_back(minst);
                    }
                }
                this->resolveUDPInsts(uinsts, modules[j]);
            }
        }
        return status;
    }

    int
    SemCheck::resolveUDPInsts(const vector<ModInst*>& uinsts, 
        Module* pmodule) const
    {
        list<ModInst*>& minsts = const_cast<list<ModInst*>&>(
            pmodule->getModInsts());
        
        const int size = uinsts.size();

        for(int i = 0; i < size; ++i)
        {
            //Create UDP instance from previously created module instance
            ModInst* minst = uinsts[i];
            const UDP* udp = static_cast<const UDP*>(
                GRoot->getObject(minst->getModName()));
            UDPInst* uinst = new UDPInst(minst->getName(), 
                udp->getName());
            uinst->setLine(minst->getLine());

            uinst->setPins(minst->getPins());
            //std::remove(minsts.begin(), minsts.end(), minst);
            minsts.erase(std::remove(
                minsts.begin(), minsts.end(), minst));
            delete minst;
            if(this->resolvePins(uinst)){
                return 1;
            }
            pmodule->addUDPInst(uinst);
        }
        return 0;
    }
            

    int
    SemCheck::readLibFiles()
    {
        const std::vector<File*>& libfiles = GRoot->getLibFiles();
        const int size = libfiles.size();
        for(int i = 0; i < size; ++i)
        {
            if(undefMods.empty()){
                return 0;
            }

            File* libfile = libfiles[i];
            FILE* file = libfile->open();
            if(!file) {
                continue;
            }
            OMCreator::instance()->setCurrFile(libfile);
            yyin = file;
            //char str[1024];
            //sprintf(str, "[INFO]: Reading library file '%s' .....\n", libfile->getName().c_str());
            //PrintMsg(str);
            MsgHandler::instance()->print(1, "library", libfile->getName().c_str());
            if(yyparse()){
                return 1;
            }
        }
        return 0;
    }

    int
    SemCheck::resolvePins(ModInst* inst) const
    {
        const Module* mod = inst->getMaster();
        const vector<Port*>& ports = mod->getPorts();
        const vector<Pin*> pins = inst->getPins();
        const int numports = GetNumElems<Port>(ports);
        const int numpins = pins.size();
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

        vector<string> portnames = GetNames<Port>(ports);
        std::vector<Pin*> newpins;
        bool isNameConn = !pins[0]->getIxName().toString().empty();
        for(int i = 0; i < numpins; ++i)
        {
            const Expr* actual = pins[i]->getActual();
            long aclen = actual ? actual->len() : -1;
            const Base* base = isNameConn ? mod->getPort(pins[i]->getIxName()):
            mod->getObject(portnames[i]);
            if(const VecPort* vport = dynamic_cast<const VecPort*>(base))
            {
                VecPin* pin = new VecPin(vport->getName(),
                    vport->getDim(), vport->getDir(),
                    pins[i]->getActual());
                pin->setScope(inst->getScope());    
                    long msb = vport->getMsb();    
                long lsb = vport->getLsb();    
                
                for(int j = msb; (msb > lsb) ? (j >= lsb) : (j <= lsb);
                (msb > lsb) ? --j : ++j)
                {
                    ScalarCntv* ctv = const_cast<Pin*>(pin->getBit(j));
                    newpins.push_back(static_cast<Pin*>(ctv));
                    ctv->setScope(pin->getScope());    
                }    
                /*
                for(int j = 0; j < polen; ++j)
                {
                    ScalarCntv* ctv = const_cast<Pin*>(pin->getBit(j));
                    newpins.push_back(const_cast<Pin*>(
                        static_cast<const Pin*>(ctv)));
                    ctv->setScope(pin->getScope());    
                }
                */
                inst->addVecPin(pin);
            }
            else if(const Port* port = dynamic_cast<const Port*>(base))
            {
                long polen = port->len();
                //Pin* pin = new Pin(portnames[i], port->getDir(),
                //    pins[i]->getActual());
                IxName name = isNameConn ? pins[i]->getIxName() : portnames[i];
                Pin *pin = new Pin(name, port->getDir(), pins[i]->getActual()); 
                newpins.push_back(pin);
            }
            else{
                //PrintMsg("[ERROR]: Line", inst->getLine(),
                //    " Port ", isNameConn ? pins[i]->getIxName().toString() :
                //    portnames[i], " not found in the module.");
                MsgHandler::instance()->print(26, inst->getLine(), portnames[i].c_str());
                return 1;
            }
        }
        inst->setPins(newpins);
        const int size = pins.size();
        for(int i = 0; i < size; ++i){
            delete pins[i];
        }
        return 0;
    }
    int
    SemCheck::resolvePins(UDPInst* uinst) const
    {
        const UDP* udp = static_cast<const UDP*>(GRoot->getObject(
            uinst->getUDPName()));
        vector<Pin*> oldpins = uinst->getPins();
        set<string, compare_str> portnames;
        const Port* outport = udp->getOutPort();
        const vector<Port*>& inports = udp->getInPorts();

        portnames.insert(outport->getIxName().getName());
        const int size = inports.size();

        for(int i = 0; i < size; ++i) {
            portnames.insert(inports[i]->getIxName().getName());
        }
        const int numports = portnames.size();
        const int numpins = oldpins.size();
        if(numports != numpins)
        {
            //PrintMsg("[ERROR]: Line ", uinst->getLine(), 
            //    "Mismatch in port connection.");
            MsgHandler::instance()->print(27, uinst->getLine());
            return 1;
        }


        const bool nameConn = !oldpins[0]->getIxName().getName().empty();
        vector<Pin*> newpins;
        if(nameConn)
        {
            set<string, compare_str> pinnames;
            for(int i = 0; i < numpins; ++i)
            {
                string pinname = oldpins[i]->getIxName().getName();
                const Expr* actual = oldpins[i]->getActual();
                const Port* port = udp->getPort(pinname);
                if(!port)
                {
                    //PrintMsg("[ERROR|: Line ", uinst->getLine(), "Port ",
                    //    pinname, " does not exist in udp port list.");
                    MsgHandler::instance()->print(26, uinst->getLine(), pinname.c_str());
                    return 1;
                }
                long aclen = actual ? actual->len() : -1;
         //       if((aclen != -1) && (aclen != 1))
         //       {
         //           PrintMsg("[ERROR]: Line ", uinst->getLine(),
         //           " Width mismatch for port ",
         //           port->getIxName().getName(), "expected size",
         //           1, " found ", aclen);
         //           return 1;
         //       }
                Pin* pin = new Pin(port->getIxName(), port->getDir(),
                actual);
                newpins.push_back(pin);
            }
        }
        else
        {
            const Expr* actual = oldpins[0]->getActual();
            long aclen = actual ? actual->len() : -1;
         //   if((aclen != -1) && (aclen != 1))
         //   {
         //       PrintMsg("[ERROR]: Line ", uinst->getLine(),
         //       " Width mismatch for port ",
         //       outport->getIxName().getName(), "expected size",
         //       1, " found ", aclen);
         //       return 1;
         //   }
            Pin* pin = new Pin(outport->getIxName().getName(),
                OUT, actual);
            newpins.push_back(pin);
            for(int i = 0; i < size; ++i)
            {
                const Expr* actual = oldpins[i+1]->getActual();
                aclen = actual ? actual->len() : -1;
        //        if((aclen != -1) && (aclen != 1))
        //        {
        //            PrintMsg("[ERROR]: Line ", uinst->getLine(),
        //            " Width mismatch for port ",
        //            inports[i]->getIxName().getName(), "expected size",
        //            1, " found ", aclen);
        //            return 1;
        //        }
                pin = new Pin(inports[i]->getIxName().getName(),
                OUT, actual);
                newpins.push_back(pin);
            }
        }
        uinst->setPins(newpins);
        return 0;
    }


    int
    SemCheck::resolveInstParams(const ModInst* inst) const
    {
        if(!inst){
            return 0;
        }

        const Module* mod = inst->getMaster();
        if(!mod){
            return 0;
        }

        const std::vector<NameConn*>& instparams = inst->getParams();
        if(instparams.empty()){
            return 0;
        }
        const int lineno = inst->getLine();
        int status = 0;
        const std::vector<Param*>& modparams = mod->getParams();
        vector<string> actParamNames = GetNames<Param>(modparams);
        const unsigned int size = instparams.size();
        if(actParamNames.size() < size)
        {
           // PrintMsg("[ERROR]: Line ", lineno, " Too many parameter values specified, exptected maximum ", actParamNames.size(), " found ", size, ".\n");
            MsgHandler::instance()->print(23,  actParamNames.size(), size);  
            return 1;
        }
        for(unsigned int i = 0; i < size; ++i)
        {
            if(instparams[i]->first.empty())
            {
                //set the param names
                for(unsigned int j = 0; j < size; ++j){
                    instparams[j]->first = actParamNames[j];
                }
                break;
            }
            if(std::find(actParamNames.begin(), actParamNames.end(),
                instparams[i]->first) == actParamNames.end())
            {
            //    PrintMsg("ERROR]: Line ", lineno, " Parameter name ",
            //        instparams[i]->first, " not found in module parameter list.");
                MsgHandler::instance()->print(24, lineno, instparams[i]->first.c_str());
                ++status;
            }
        }
        return status;
    }

    int 
    SemCheck::resolveDefparams()
    {
        int status = 0;
        const vector<Module*>& topmods = GRoot->getTopModules();
        for(unsigned int i = 0; i < topmods.size(); ++i)
        {
            int tmp = this->resolveDefparams(topmods[i], 
                    Path(topmods[i]->getName()));
            if(tmp) {        
                MsgHandler::instance()->print(43, topmods[i]->getLine(), topmods[i]->getName().c_str());
            }
            status += tmp;
        }
        return status;
    }

    int
    SemCheck::resolveDefparams(
        Module* mod,
        const Path& path
        )
    {
        int status = 0;
        const vector<Defparam*>& defparams = mod->getDefparams();
        for(unsigned int k = 0; k < defparams.size(); ++k)
        {
            Defparam* defparam = defparams[k];
            HierId* hid = const_cast<HierId*>(
                    dynamic_cast<const HierId*>(defparam->getLhs()));
            //assert(hid);
            if(hid)
            {
                if(!hid->resolve(path)){
                    ++status;
                }
                else
                {
                    const Expr *act = hid->getActualId();
                    if(!dynamic_cast<const Param*>(act) &&
                        !dynamic_cast<const VecParam*>(act)) {
                        ++status;
                    }    
                }
            }
        }

        const list<ModInst*>& minsts = mod->getModInsts();
        for(list<ModInst*>::const_iterator iter = minsts.begin();
                iter != minsts.end(); ++iter)
        {
            const ModInst* minst = *iter;
            const Module* master = minst->getMaster();
            if(!master){
                continue;
            }
            Path newpath(path);
            newpath.descend(minst->getName().toString());
            status += this->resolveDefparams(const_cast<Module*>(master),
                    newpath);
        }
        return status;
    }

    int
        SemCheck::doSemChecks(SemanticChecker *semanticChecker)
        {
            int status = 0;
            const vector<File*>& files = GRoot->getFiles();
            int filec = files.size();

            for(int i = 0; i < filec; ++i)
            {
                File* file = files[i];
                //PrintMsg(string("[INFO]: Checking for errors in file "), file->getName(),string(" ....."));
                const vector<Module*>& modules = file->getModules();
                const int modc = modules.size();
                for(int j = 0; j < modc; ++j)
                {
                    modules[j]->accept(semanticChecker);
                    if(!semanticChecker->getStatus()) {
                        ++status;
                    }    
                }

                const vector<UDP*>& udps = file->getUDPs();
                const int udpc = udps.size();
                for(int j = 0; j < udpc; ++j)
                {
                    udps[j]->accept(semanticChecker);
                    if(!semanticChecker->getStatus()) {
                        ++status;
                    }    
                }
            }
            const vector<File*>& libfiles = GRoot->getLibFiles();
            filec = libfiles.size();

            for(int i = 0; i < filec; ++i)
            {
                File* file = libfiles[i];
                const vector<Module*>& modules = file->getModules();
                const int modc = modules.size();
                for(int j = 0; j < modc; ++j)
                {
                    modules[j]->accept(semanticChecker);
                    if(!semanticChecker->getStatus()) {
                        ++status;
                    }    
                }
                const vector<UDP*>& udps = file->getUDPs();
                const int udpc = udps.size();
                for(int j = 0; j < udpc; ++j)
                {
                    udps[j]->accept(semanticChecker);
                    if(!semanticChecker->getStatus()) {
                        ++status;
                    }    
                }
            }
            return status;

        }

    int
        SemCheck::checkAndUnrollGenerateStatements()
        {
            int status = 0;
            const vector<File*>& files = GRoot->getFiles();
            const vector<File*>& libfiles = GRoot->getLibFiles();

            status = checkAndUnrollGenerateStatements(files);
            status += checkAndUnrollGenerateStatements(libfiles);
            return status;
        }    
    int
        SemCheck::checkAndUnrollGenerateStatements(const vector<File*>& files)
        {
            int status = 0;
            int filec = files.size();

            for(int i = 0; i < filec; ++i)
            {
                File* file = files[i];
                const vector<Module*>& modules = file->getModules();
                const int modc = modules.size();
                for(int j = 0; j < modc; ++j) {
                    if(!GenUnroll(modules[j])()) {
                        ++status;
                    }    
                }
            }
            return status;
        }    


        int
        SemCheck::checkForRecursiveInstantiation() const
        {
            int status = 0;
            status = checkForRecursiveInstantiation(GRoot->getFiles());
            status += checkForRecursiveInstantiation(GRoot->getLibFiles());
            return status;
        }
                    
        int
        SemCheck::checkForRecursiveInstantiation(const vector<File*>& files) const
        {
            int filec = files.size();

            for(int i = 0; i < filec; ++i)
            {
                File* file = files[i];
                const vector<Module*>& modules = file->getModules();
                const int modc = modules.size();
                for(int j = 0; j < modc; ++j)
                {
                    set<string> name;
                    if(checkForRecursiveInstantiation(modules[j], name)) {
                        return 1;
                    }    
                }
            }
            return 0;
        }    
        int
        SemCheck::checkForRecursiveInstantiation(const Module *mod, set<string> name) const
        {
            if(!mod) {
                return 0;
            }

            const list<ModInst*> & insts = mod->getModInsts();
            if(insts.empty()) {
                return 0;
            }
            if(name.find(mod->getName()) != name.end()) {
                return 1;
            }
            name.insert(mod->getName());

            list<ModInst*>::const_iterator instBegin = insts.begin();
            list<ModInst*>::const_iterator instEnd = insts.end();

            for(; instBegin != instEnd; ++instBegin) 
            {
                const Module *newMod = (*instBegin)->getMaster();
                if(!newMod) {
                    continue;
                }
                if(checkForRecursiveInstantiation(newMod, name))
                {
                    MsgHandler::instance()->print(44, (*instBegin)->getLine(), newMod->getName().c_str());
                    return 1;
                }    
            }
            return 0;
        }    
}
