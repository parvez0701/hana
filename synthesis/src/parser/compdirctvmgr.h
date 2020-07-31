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

#ifndef _COMPDRCTVMGR_H_
#define _COMPDRCTVMGR_H_

#include <map>
#include "utils.h"
namespace Rtl
{
    class CompDirctvMgr
    {
        public:
            static CompDirctvMgr* instance();
            ~CompDirctvMgr();

            bool addDefine(const std::string&, const std::string&);
            std::string getRepText(const std::string&);
            bool removeDefine(const std::string&);

            bool process(const std::string&);

            void setCurrName(const std::string& name);

        public:
            typedef std::map<std::string, std::string, compare_str> 
            DefineTable;
            typedef std::pair<std::string, bool> ArgOrText;

            struct ParamDefine
            {
                bool operator==(const ParamDefine&) const;
                bool operator!=(const ParamDefine&) const;
                bool operator<(const ParamDefine&) const;
                
                bool dirty() const;
                bool isArg(const std::string&) const;
                int getParamSize() const;
                string name;
                std::vector<ArgOrText>    text;
                std::vector<ArgOrText>    repText;
            };
            typedef std::set<ParamDefine> ParamDefineTable;
        private:
            CompDirctvMgr();
            CompDirctvMgr(const CompDirctvMgr&);
            CompDirctvMgr& operator=(const CompDirctvMgr&);
            //functions
            bool isParamDefine(const std::string&) const;
            bool isSimpleDefine(const std::string&) const;
            bool addSimpleDefine(const std::string&, const std::string&);
            //bool addParamDefine(const std::string&, const std::string&);
            bool removeSimpleDefine(const std::string&);
            bool removeParamDefine(const std::string&);
            std::pair<std::string, std::string> parse(const std::string&) const;
            ParamDefineTable parseParamDefine(const std::string&, const std::string&);
            std::vector<ArgOrText> getText(const std::string&) const;
            string getReplacementText(const std::vector<ArgOrText>&,
                                                const ParamDefine&) const;
            bool checkForMatchingBraces(const std::string&) const;
            bool checkForValidArgs(const std::string&) const;

            //data
            static CompDirctvMgr *compDirctvMgr;
            DefineTable defineTable;
            ParamDefineTable paramDefineTable;
            std::string currName;
    };
}
#endif
