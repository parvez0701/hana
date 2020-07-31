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

#ifndef _SBLOCK_H_
#define _SBLOCK_H_
#include <map>
#include <list>
#include <list>
#include "sbase.h"
#include "sutils.h"
namespace stview
{
    class SPort;
    class SPin;
    class SNet;
    class SInst;
    class SBlockInst;
	class SAssign;
    class SBlock : public SBase
    {
        public:
            typedef std::list<SPort*>        Ports;
            typedef std::list<SNet*>         Nets;
            typedef std::list<SInst*>        CellInstances;
            typedef std::list<SBlockInst*>   BlockInstances;
            typedef std::list<SAssign*>      AssignStmts;;

            typedef std::map<std::string, SPort*, cmp_str> PortSymTable;
            typedef std::map<std::string, SNet*, cmp_str> NetSymTable;
            typedef std::map<std::string, SInst*, cmp_str> CellSymTable;
            typedef std::map<std::string, SBlockInst*, cmp_str> BlockSymTable;
        public:
            SBlock(const std::string&);
            virtual ~SBlock();
            inline const Ports& getInPorts() const;
            inline const Ports& getOutPorts() const;
            inline const Ports& getInOutPorts() const;

            inline const std::list<SBlockInst*>& getBlockInsts() const;
            inline const std::list<SInst*>& getInsts() const;
            inline const std::list<SNet*>& getNets() const;
            inline const std::list<SAssign*>& getAssigns() const;

            const SPort* getPort(const std::string&) const;
            const SNet* getNet(const std::string&) const;
            const SInst* getInst(const std::string&) const;
            const SBlockInst* getBlockInst(const std::string&) const;

            const SBase* findObject(const std::string&) const;

            void addPort(const SPort*);
            void addNet(const SNet*);
            void addInst(const SInst*);
            void addBlockInst(const SBlockInst*);
            void addAssign(const SAssign*);

            void deleteCellInst(SInst*);
            void deleteNet(SNet*);

            void resetCntvFlags();

            virtual void accept(NetlistVisitorBase*);

            bool connect(SPin*, SPin*);

            void resetVisitCount();

        private:
            SBlock(const SBlock&);
            SBlock& operator=(const SBlock&);

            std::list<SPort*> inports;
            std::list<SPort*> outports;
            std::list<SPort*> inoutports;
            std::list<SInst*> insts;
            std::list<SBlockInst*> blockinsts;
            std::list<SNet*> nets;
			//assign staments are used when converting to Sum-of-Product form
			std::list<SAssign*> assigns;
            //std::map<std::string, SBase*, cmp_str> symTable;

            PortSymTable    portSymTable;
            NetSymTable     netSymTable;
            CellSymTable    cellSymTable;
            BlockSymTable   blockSymTable;

    };

    const std::list<SPort*>& 
    SBlock::getInPorts() const {
        return inports;
    }
    const std::list<SPort*>& 
    SBlock::getOutPorts() const {
        return outports;
    }
    const std::list<SPort*>& 
    SBlock::getInOutPorts() const {
        return inoutports;
    }

    const std::list<SBlockInst*>&
    SBlock::getBlockInsts() const {
        return blockinsts;
    }    

    const std::list<SInst*>&
    SBlock::getInsts() const {
        return insts;
    }    
    const std::list<SNet*>&
    SBlock::getNets() const {
        return nets;
    }    
	const std::list<SAssign*>& 
	SBlock::getAssigns() const {
	    return assigns;
	}	

    typedef ResetVisitCount<SBlock> ResetVisitCountBlock;
}
#endif
