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

#ifndef _TECHFFMAP_H_
#define _TECHFFMAP_H_

#include "techseqmap.h"

namespace STV = stview;

namespace TechnologyMapping
{
    class TechFFMap : public TechSeqMap
    {
        public:
            TechFFMap(STV::SCell::CellType=STV::SCell::FF);
            virtual ~TechFFMap();
            virtual std::string getNameHint() const;
            virtual PortMap getPortMap(const STV::SCell*, const STV::SCell*) const;
        protected:
            virtual bool doMapping(STV::SBlock *, STV::SInst*);
            bool isNegEdgeClock(const STV::SInst*) const;
            virtual const STV::SCell *getTechCell(const STV::SInst*) const;
        private:
            TechFFMap(const TechFFMap&);
            TechFFMap& operator=(const TechFFMap&);
    };
    //------------------------------------------------------------------
    class TechROrSFFMapBase : public TechFFMap
    {
        public:
            TechROrSFFMapBase(STV::SCell::CellType);
            virtual ~TechROrSFFMapBase();
        protected:
            virtual bool doMapping(STV::SBlock *, STV::SInst*);
            virtual bool isNegEdgeCtrl(const STV::SInst*) const = 0;
            virtual std::string getCtrlAttrib() const = 0;
            virtual const STV::SCell *getTechCell(const STV::SInst*) const;
        private:
            TechROrSFFMapBase(const TechROrSFFMapBase&);
            TechROrSFFMapBase& operator=(const TechROrSFFMapBase&);

            bool doMappingNoMatch(STV::SBlock *, STV::SInst*);
    };        
    //------------------------------------------------------------------
    class TechRFFMap : public TechROrSFFMapBase
    {
        public:
            TechRFFMap();
            virtual ~TechRFFMap();
            virtual std::string getNameHint() const;
            virtual PortMap getPortMap(const STV::SCell*, const STV::SCell*) const;
        protected:
            virtual bool isNegEdgeCtrl(const STV::SInst*) const;
            virtual std::string getCtrlAttrib() const;
        private:
            TechRFFMap(const TechRFFMap&);
            TechRFFMap& operator=(const TechRFFMap&);

            bool isNegEdgeReset(const STV::SInst*) const;

    };

    //-----------------------------------------------------------------
    class TechSFFMap : public TechROrSFFMapBase
    {
        public:
            TechSFFMap();
            virtual ~TechSFFMap();
            virtual std::string getNameHint() const;
            virtual PortMap getPortMap(const STV::SCell*, const STV::SCell*) const;
        protected:
            virtual bool isNegEdgeCtrl(const STV::SInst*) const;
            virtual std::string getCtrlAttrib() const;
        private:
            TechSFFMap(const TechSFFMap&);
            TechSFFMap& operator=(const TechSFFMap&);

            bool isNegEdgeSet(const STV::SInst*) const;
    };

    //-----------------------------------------------------------------
    class TechRSFFMapBase : public TechFFMap
    {
        public:
            TechRSFFMapBase(STV::SCell::CellType);
            virtual ~TechRSFFMapBase();
        protected:    
            virtual bool doMapping(STV::SBlock *, STV::SInst*);
            virtual const STV::SCell *getTechCell(const STV::SInst*) const;
        private:
            TechRSFFMapBase(const TechRSFFMapBase&);
            TechRSFFMapBase& operator=(const TechRSFFMapBase&);

            bool doMappingNoMatch(STV::SBlock *, STV::SInst*);
            bool addRemoveInverter(STV::SBlock*, const STV::SCell*, STV::SInst*,
                bool, const std::string&);
    };
    //-----------------------------------------------------------------
    class TechRSFFMap : public TechRSFFMapBase
    {
        public:
            TechRSFFMap();
            virtual ~TechRSFFMap();
            virtual std::string getNameHint() const;
            virtual PortMap getPortMap(const STV::SCell*, const STV::SCell*) const;
        private:
            TechRSFFMap(const TechRSFFMap&);
            TechRSFFMap& operator=(const TechRSFFMap&);
    };
    //-----------------------------------------------------------------
    class TechSRFFMap : public TechRSFFMapBase
    {
        public:
            TechSRFFMap();
            virtual ~TechSRFFMap();
            virtual std::string getNameHint() const;
            virtual PortMap getPortMap(const STV::SCell*, const STV::SCell*) const;
        private:
            TechSRFFMap(const TechSRFFMap&);
            TechSRFFMap& operator=(const TechSRFFMap&);
    };
}
#endif
