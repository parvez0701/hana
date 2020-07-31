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

#ifndef _GENMAPBASE_H_
#define _GENMAPBASE_H_

#include <sstream>
#include <string>
#include <list>
#include <vector>

#include "syixname.h"
#include "scell.h"

namespace Synth 
{
    class SyModInst;
    class SyScCntv;
    class SyExpr;
}
namespace stview
{
    class SBlock;
    class SInst;
    class SPN;
    class SCell;
    class SNet;
}
namespace GenericMapping
{
    struct InstancePredicate {
        virtual bool operator()(const Synth::SyModInst*) const = 0;
    };
    class GenericMapperBase 
    {
        public:
            friend class GenMapMgr;
            //old-new map
            typedef std::pair<Synth::SyIxName, std::string> PinMap;
            typedef std::list<PinMap> PinMapList;
            typedef PinMapList::const_iterator PinMapListConstIterator;
        public:
            virtual void doMapping() /*= 0;*/ { }
            const stview::SBlock*            getBlock() const { return block; }
            stview::SBlock*                    getBlock() { return block; }
            inline std::string        getMapName(const Synth::SyIxName&) const;
            const stview::SPN*            getMapCntv(const Synth::SyExpr*) const;
            void                            setOptimize(bool opt=true) { optimize = opt; }
            void                            createCellInstances(const InstancePredicate&);    
            stview::SInst*                    createCellInstance(const Synth::SyModInst*,
                                            const stview::SCell*);
            virtual const stview::SCell*    getCell() const { return NULL; }/* = 0; */

            //generation functions
            stview::SNet*                    createNet(const std::string& ="net");
            
            stview::SInst*                    createInverter(
                                                const std::string&,
                                                const stview::SPN*, //in
                                                const stview::SPN*); //out
                                                
            stview::SInst*                    createBuffer(
                                                const std::string&,
                                                const stview::SPN*, //in
                                                const stview::SPN*); //out
            
            stview::SInst*                    create2InputGate(
                                                const std::string&,
                                                stview::SCell::CellType,
                                                const stview::SPN*,
                                                const stview::SPN*,
                                                const stview::SPN*);


            void                            create2InputGateTree(
                                                const std::string&,
                                                stview::SCell::CellType,
                                                const std::vector<stview::SPN*>&,
                                                const stview::SPN*);
            
        protected:
            GenericMapperBase() : block(0), optimize(false){ }
            GenericMapperBase(stview::SBlock *sb) : block(sb), optimize(false) { }
            inline void                        setPinMapList(const PinMapList& );


            virtual ~GenericMapperBase() { }
            virtual GenericMapperBase* create(stview::SBlock*) const = 0;
            virtual    void    setPinMapList(){}/* = 0;*/        
        private:
            //data
            stview::SBlock    *block;
            bool            optimize;
            PinMapList        pinMapList;
    };
    std::string
    GenericMapperBase::getMapName(const Synth::SyIxName& name) const
    {
        if(name.getIndex() == Synth::SyIxName::nix) {
            return name.getName();
        }

        std::ostringstream str;
        str << name.getName() << "#" << name.getIndex();
        return str.str();
    }

    void
    GenericMapperBase::setPinMapList(const PinMapList& pml) {
        pinMapList = pml;
    }    
}

namespace GM = GenericMapping;
#endif
