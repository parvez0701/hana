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

#ifndef _NETLISTCLEANER_H_
#define _NETLISTCLEANER_H_

#include <map>
#include "sylist.h"
#include "syutils.h"

namespace Synth
{
    class SyScCntv;
    class SyScPin;
    class SyModule;
    class SyInst;
    class SyModInst;
    class DriverSinkMgr;
    class NetlistCleaner
    {
        public:
            NetlistCleaner();
            ~NetlistCleaner();
        
            void doCleanup();
        private:
            typedef std::map<const SyScCntv*, int/*, cmp_cntvs*/> NetUsageTable;
            typedef std::map<SyScPin*, const SyScCntv*> PinActualTable;

            
            void    init();
            void    destroy();
            void    incUsage(const SyScCntv*);
            void    decUsage(const SyScCntv*);
            int        getUsage(const SyScCntv*) const;
            bool    isUnused(const SyScCntv*) const;
            void    computeUsage();
            void    computeUsage(const SyInst*);

            bool    isBuffer(const SyModInst*) const;
            bool    isSeqElemen(const SyModInst*) const;

            void    removeBuffers();
            void    removeBuffersForward(SyScPort*);
            void    removeBuffersBackward(SyScPort*);
            void    removeBuffersForward(SyScCntv* src, SyScPin* dest);
            void    removeBuffersBackward(SyScCntv* src, SyScPin* dest);
            void    setActual(SyScPin*, SyScCntv*);
            void    setActuals();

            std::pair<SyScCntv*, SyList<SyScPin*>*> skipOverBuffers(SyScCntv*, bool);


            void     removeInsts();
            void     removeNets();
            void     removeRegs();
            bool    is2bRemoved(const SyBase*) const;
            template <class T> void
            removeObjects(SyList<T*>&);

            void    printUsage() const;
            
            SyModule                *currMod;
            DriverSinkMgr            *dsMgr;
            NetUsageTable            netUsageTable;
            std::set<SyModInst*>    insts2bRemoved;
            std::set<SyScPin*>        visitedPins;
            PinActualTable            pinActualTable;
    };    

    class DriverSinkMgr
    {
        public:
            typedef std::pair<SyList<SyScPin*>*, SyList<SyScPin*>* > DriversAndSinks;
            typedef std::map<const SyScCntv*, DriversAndSinks/*, cmp_cntvs*/> DriverSinkInfo;
        public:
            DriverSinkMgr(const SyModule*);
            ~DriverSinkMgr();

            const SyList<SyScPin*>* getDrivers(const SyScCntv*) const;
            const SyList<SyScPin*>* getSinks(const SyScCntv*) const;
        private:
            DriverSinkMgr();
            DriverSinkMgr(const DriverSinkMgr&);
            DriverSinkMgr& operator=(const DriverSinkMgr&);

            void init();
            void destroy();

            void addDriver(const SyScCntv*, const SyScPin*);
            void addSink(const SyScCntv*, const SyScPin*);
            void addDriverSinkData(const SyInst*);
            
            const SyModule *currMod;
            DriverSinkInfo driverSinkInfo;
    };
}
#endif
