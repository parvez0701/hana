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

#include "synthesizer.h"
#include "sycontainer.h"
#include "syroot.h"
#include "netlistcleaner.h"
#include "sycmdline.h"

using std::string;
using std::map;
namespace Synth
{
    Synthesizer* Synthesizer::synthesizer = 0;

    Synthesizer*
    Synthesizer::instance() {
        if(!Synthesizer::synthesizer) {
            Synthesizer::synthesizer = new Synthesizer();
        }
        return Synthesizer::synthesizer;
    }

    Synthesizer::Synthesizer() {
    }

    Synthesizer::~Synthesizer() {
        Synthesizer::synthesizer = 0;
    }

    SyModule*
    Synthesizer::getSynthModule(const string& name) const
    {
        SyRoot::SyStage stage = SyRoot::getStage();
        SyRoot::setStage(SyRoot::STAGE_2);
        SyModule *mod = SyRoot::instance()->getModule(name);

        SyRoot::setStage(stage);
        return mod;
    }

    bool
    Synthesizer::isModuleSynthesized(const string& name) const
    {
        return (this->getSynthModule(name) != 0);
    }    

    void
    Synthesizer::synthesize()
    {
        map<string, SyModule*, cmp_str> modules = 
            SyRoot::instance()->getSynthMods();
        map<string, SyModule*, cmp_str>::const_iterator mod = modules.begin();
        map<string, SyModule*, cmp_str>::const_iterator modEnd = modules.end();

        for(; mod != modEnd; ++mod) {
            //If a module is synthesized then all modules
            //underlying the hierarchy are synthesized
            this->synthesize(mod->second);
        }
        if(SyCmdLine::instance()->noOpt() == false)
        {
            NetlistCleaner netlistCleaner;
            netlistCleaner.doCleanup();
        }    
    }

    void
    Synthesizer::synthesize(const SyModule *mod)
    {
        if(!mod) {
            return;
        }    
        if(this->isModuleSynthesized(mod->getName())){
            return;
        }    
        if(SyRoot::instance()->isDontTouchModule(mod->getName())) {
            return;
        }    
        const SyList<SyModInst*>& minsts = mod->getModInsts();
        for(SyNode<SyModInst*>* node = minsts.head; node; node = node->next)
        {
            const SyModule *master = node->val->getMaster();
            this->synthesize(master);
        }    

        mod->accept(&rtl2Netlist);
    }    
        
    
    


}
