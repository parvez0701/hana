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

#include "scell.h"

namespace stview 
{
    class SBlock;
    class SInst;
}
namespace TechnologyMapping
{
    class TechMapReport
    {
        public:
            static TechMapReport* instance();
            ~TechMapReport();

            void printMapFailReport() const;
            void printAreaReport() const;
        private:
            TechMapReport();
            TechMapReport(const TechMapReport&);
            TechMapReport& operator=(const TechMapReport&);

            void populateAreaInfo();
            void populateAreaInfo(const stview::SBlock*);

            void printFailReportHeader() const;
            void printFailReportFooter() const;
            void printAreaReportHeader() const;
            void printAreaReportFooter(int, float) const;
            void printMsg(const std::string&) const;

            void addInfo(const stview::SInst*);

            struct AreaInfo
            {
                stview::SCell::CellType type;
                int numinst;
                float area;
            };

            typedef std::map<std::string, AreaInfo, stview::cmp_str> AreaInfoMap;

            AreaInfoMap areaInfoMap;
            const int align;

            static TechMapReport *techMapReport;
    };
}
