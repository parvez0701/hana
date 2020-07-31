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

#ifndef _MODINST_H_
#define _MODINST_H_
#include <map>

#include "inst.h"
#include "defines.h"
#include "vecpin.h"
namespace Rtl
{
    class Module;
    class OMVisitorBase;
    class ModInst: public Inst
    {
        friend class OMCreator;
        friend class SemCheck;
        friend class Elaborator;
        friend class GenUnroll;
        public:
            ModInst(const std::string& iname, 
                const std::string& mname): Inst(iname), modname(mname),
                master(0){
            }

            ModInst(const std::string& iname, const std::string& mname,
                const Module* ms): Inst(iname), modname(mname), 
                master(const_cast<Module*>(ms)){
            }

            virtual ~ModInst(){
            }
            const Module* getMaster() const{
                return master;
            }

            const bool isBlackBox() const {
                return master == 0;
            }
            const std::vector<NameConn*>& getParams() const{
                return params;
            }

            const std::string& getModName() const;

            void addVecPin(const VecPin* vpin){
                vecpins.insert(std::make_pair(
                vpin->getName(), const_cast<VecPin*>(vpin)));
            }

            const VecPin* getVecPin(const std::string& name) const{
                std::map<std::string, VecPin*, compare_str>::
                const_iterator iter = vecpins.find(name);
                if(iter == vecpins.end()){
                    return NULL;
                }
                return iter->second;
            }
            virtual void accept(OMVisitorBase*) const;
            virtual ModInst* copy() const;
            virtual ModInst* repExpr(const std::string&, const Expr*);
                
        private:
            void setMaster(const Module*);
            void setParams(const std::vector<NameConn*>& p){
                params = p;
            }
            std::vector<NameConn*> params;
            std::string modname;
            std::map<std::string, VecPin*, compare_str> vecpins;
            Module* master;
    };
}
#endif
