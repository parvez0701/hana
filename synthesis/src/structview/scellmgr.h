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

#ifndef _SCELLMGR_H_
#define _SCELLMGR_H_

#include <map>
#include <vector>

#include "sutils.h" 

#include "scell.h"
namespace stview
{
    class SCellMgr
    {
        public:
            typedef std::map<std::string, SCell*, cmp_str> CellTable;

            struct CellInfo {
                CellInfo(    const std::string& n,
                            SCell::CellType t,
                            short s,
                            short f) : 
                            name(n), type(t), inputSize(s), flag(f) {
                }    
                CellInfo() : name(""), type(SCell::CELLTYPE_BEGIN),
                    inputSize(0), flag(0) {
                }    
                std::string                    name;
                SCell::CellType                type;
                short                        inputSize;
                short                        flag; //1 generic, 2 tech
            };
            typedef std::vector<CellInfo>    CellInfoVector;
            //indexed by type of the cell
            typedef std::vector<CellInfoVector> CellInfoTable;
        public:
            static SCellMgr*                instance();
            virtual ~SCellMgr();
            bool                            init();
            const SCell*                    getCell(const std::string&)const;
            const SCell*                    getCell(SCell::CellType, short) const;
            
            const SCell*                    getGenericCell(const std::string&) const;
            const SCell*                    getGenericCell(SCell::CellType, short) const;
            const SCell*                    getTechCell(const std::string&) const;
            const SCell*                    getTechCell(SCell::CellType, short) const;
            std::vector<const SCell*>        getTechCells(SCell::CellType, short) const;
            bool                            isOk() const { return status; }
        private:
            SCellMgr();
            SCellMgr(const SCellMgr&);
            SCellMgr& operator=(const SCellMgr&);

            bool                             addCell(SCell*, CellInfo&);
            void                            addCellInfo(CellInfo&);
            bool                            doChecksAndAdd(bool);
            bool                            doChecksAndAdd(SCell*, bool);
            bool                            addGenericCell(SCell*);
            bool                            addTechCell(SCell*);
            //bool                            removeGenericCell(const std::string&);
            //bool                            removeTechCell(const std::string&);

            static SCellMgr *cellMgr;
            bool            status;
            CellTable        genericCellTable;
            CellTable        techCellTable;
            CellInfoTable    cellInfoTable;
    };
}
#endif
