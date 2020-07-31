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

#ifndef _TECHSEQMAP_H_
#define _TECHSEQMAP_H_


#include "techone2onemap.h"

namespace STV = stview;
namespace TechnologyMapping
{
    class TechSeqMap: public TechOne2OneMap
    {
        public:
            virtual ~TechSeqMap();
        protected:
            TechSeqMap(STV::SCell::CellType);

            virtual PortMap getPortMap(const STV::SCell*,
                const STV::SCell*) const;
            virtual const STV::SCell *getTechCell(const STV::SInst*) const = 0;    
            bool isNegEdge(const STV::SInst*, const std::string&) const;
            bool isNegEdge(const STV::SCell*, const std::string&) const;
            const STV::SInst* getNegEdgeInverter(const STV::SInst*, const std::string&) const;

            const STV::SCell* getNegEdgeTechCell(STV::SCell::CellType,
                int, const std::string&) const;
            const STV::SCell* getPosEdgeTechCell(STV::SCell::CellType,
                int, const std::string&) const;
            const STV::SCell* getEdgeTechCell(STV::SCell::CellType,
                int, const std::string&, bool) const;

            bool addInverter(STV::SBlock*,  
                STV::SInst*, const std::string&) const;    
            
            bool removeInverter(STV::SBlock*, STV::SInst*, 
                const std::string&) const;
        private:
            TechSeqMap();
            TechSeqMap(const TechSeqMap&);
            TechSeqMap& operator=(const TechSeqMap&);


    };
}
#endif
