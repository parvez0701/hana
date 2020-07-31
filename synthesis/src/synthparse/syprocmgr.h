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

#ifndef _SYPROCMGR_H_
#define _SYPROCMGR_H_
#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>

#include "sylist.h"
#include "syutils.h"
#include "cellutil.h"

namespace Synth
{
    class SyAlways;
    class SyModule;
    class SyAssign;
    class SyIf;
    class SyCase;
    class SyTaskCall;
    class SyStmt;
    class SyScope;
    class SyFuncCall;
    class SyBlockAssign;
    class SyNonBlockAssign;
    class SyModule;
    class SyExpr;
    class SyTaskOrFuncCall;

    class AlwaysInfo_
    {
        friend class Always2Gates;
        public:
            AlwaysInfo_(const SyAlways*, const SyModule*);
            virtual ~AlwaysInfo_();

            bool        isSet(const std::string&) const;
            bool        isReset(const std::string&) const;
            std::string getSetName(const std::string&) const;
            std::string getResetName(const std::string&) const;

            bool        isComb() const;
            bool        getStatus() const;
            CellType    getCellType(const std::string&) const;
            const std::string& getClock() const;

            bool        isNegEdgeClock() const;
            bool        isNegEdgeReset(const std::string&) const;
            bool        isNegEdgeSet(const std::string&) const;

            std::string getCondString(const SyIf *) const;
            std::vector<std::string> getUnusedSignals(const std::string&) const;
        private:
            void setSetReset(); 
            void setSetReset(const std::string&, const SyList<SyStmt*>&, 
                const std::string&, int setReset=0, int level=1);
                
            void setSetReset(const std::string&, const SyAssign*, 
                const std::string&, int setReset=0, int level=1);
                
            void setSetReset(const std::string&, const SyTaskCall*, 
                const std::string&, int setReset=0, int level=1);
            void setSetReset(const std::string&, const SyFuncCall*, 
                const std::string&, int setReset=0, int level=1);
            void setSetReset(const std::string&, const SyExpr*, 
                const std::string&, int setReset=0, int level=1);

            void setClockSetReset(const SyStmt*, const SyExpr*, const SyExpr*);

            bool        isNegEdge(const std::string&) const;

            bool        isSensVar(const std::string&) const;
            bool        checkAndAddSet(const std::string&, const std::string&);
            bool        checkAndAddReset(const std::string&, const std::string&);


            bool        checkConsistencyAndSetClock();
            bool        checkIfConditionConsistency(const SyIf*);
            bool        checkIfConditionConsistency(const SyExpr*, 
                            const std::string&);
            bool        checkUnaryConsistency(const SyUnary*, 
                            const std::string&);
            bool        checkBinaryConsistency(const SyBinary*, 
                            const std::string&);
            void        findSeqVars();
            void        findSeqVars(const SyList<SyStmt*>&, const SyScope*);
            void        findSeqVars(const SyStmt*, const SyScope*);
            void        findSeqVars(const SyIf*, const SyScope*);
            void        findSeqVars(const SyFuncCall*, const SyScope*);
            void        findSeqVars(const SyTaskCall*, const SyScope*);
            void        findSeqVars(const SyCase*, const SyScope*);
            void        findSeqVars(const SyBlockAssign*, const SyScope*);
            void        findSeqVars(const SyNonBlockAssign*, const SyScope*);

            void        populateUESTable();

            SyList<SyStmt*> getElabStmts(const SyTaskOrFuncCall*) const;
            void        findSeqVars(const SyAssign*, const SyScope*);
            void        print() const;

            int            getSetTimeStamp(const std::string&) const;
            int            getResetTimeStamp(const std::string&) const;

            bool        contains(const SyExpr*, const std::string&) const;


            SyAlways        *always;
            SyModule        *module;
            std::string        clock;

            typedef std::map<std::string, std::pair<std::string, int>, cmp_str>
            SetResetInfo;
            //un used sensitivity list signals for an output
            typedef std::map<std::string, std::vector<std::string> , cmp_str > 
            UESTable;

            SetResetInfo setInfo;
            //map between name and reset signal name
            SetResetInfo resetInfo;

            std::set<std::string, cmp_str> vars;
            //variables used in top level if condition
            std::set<std::string, cmp_str> condvars;
            std::list<std::string> sensvars;
            UESTable uesTable;
            bool status;
            int timestamp;
    };

    class SyProcMgr
    {
        public:
            typedef std::map<const SyAlways*, const AlwaysInfo_*> AlwaysTable;
        public:
            static SyProcMgr *instance();
            ~SyProcMgr();

            const AlwaysInfo_* getAlwaysInfo(const SyAlways*) const;
            bool check(const SyAlways*, const SyModule*);
        private:
            SyProcMgr();
            SyProcMgr(const SyProcMgr&);
            SyProcMgr& operator=(const SyProcMgr&);

            AlwaysTable alwaysTable;
            static SyProcMgr* procMgr;
    };

}
#endif
