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

#ifndef _SYGENUNROLL_H_
#define _SYGENUNROLL_H_

#include "syutils.h"
namespace Synth
{
    class BlockNameMgr
    {
        public:
            typedef std::map<std::string, std::string, cmp_str> OldNewNameMap;
            typedef std::stack<OldNewNameMap> NameTableStack;
        public:
            BlockNameMgr(const SyModule*);
            ~BlockNameMgr();

            std::string getNewName(const std::string&) const;
            std::string createNewName(const std::string&) const;
            void addName(const std::string&, const std::string&);
            const SyBase *findObject(const std::string&) const;
            void diveInsideNewScope();
            void comeOutOfScope();
        private:
            BlockNameMgr();
            DISABLE_COPYCONST_ASSGNOP(BlockNameMgr);
            NameTableStack nameTableStack;
            SyModule *synthMod;
    };

    class SyGenUnroll
    {
        public:
            //genvars are converted into vector regs
            typedef std::map<std::string, Genvar*, cmp_str>    Genvars;
        public:
            SyGenUnroll(const SyModule*, const Rtl::Module*);
            ~SyGenUnroll();
            bool getStatus() const {
                return status;
            }
            bool unroll();
            bool operator()();
        private:
            SyGenUnroll();
            DISABLE_COPYCONST_ASSGNOP(SyGenUnroll);
            
            void addGenvar(const Rtl::Genvar*);
            Rtl::Genvar* findGenvar(const std::string&) const;
            SyConst *getGenvarValue(const Genvar*) const;
            
            BlockNameMgr *blockNameMgr;
            SyModule *synthMod;
            Rtl::Module *rtlMod;
            bool status;
            Genvars    genvars;
    };
}
#endif
