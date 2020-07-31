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


namespace Synth
{
    class SyModule;
    class SyBModule;
    class SyTask;
    class SyFunc;
    class SyUnary;
    class SyBinary;
    class SyCondExpr;
    class SyFuncCall;
    class SyBitSel;
    class SyPartSel;
    class SyConcat;
    class SyContAssign;
    class SyNonBlockAssign;
    class SyBlockAssign;
    class SyIf;
    class SyAlways;
    class SyCaseItem;
    class SyCase;
    class SyTaskCall;
    class SyConst;
    class SyBlockCompAssign;
    class SyNonBlockCompAssign;
    class SyXOrZ;
    class SyAssign;
    class SyStmt;



    class SyExprMgr
    {
        public:
            static SyExprMgr *instance();
            static SyExpr* extend(SyExpr* expr, int by);
            static SyExpr* truncate(SyExpr*, int);
            static SyExpr* truncate(SyExpr*, int, int);

            ~SyExprMgr();

            void process() const;
            void processAssign(SyAssign*) const;


        private:
            SyExprMgr();
            SyExprMgr(const SyExprMgr&);
            SyExprMgr& operator=(const SyExprMgr&);

            void process(SyBModule*) const;
            void process(SyTask*) const;
            void process(SyTaskCall*) const;
            void process(SyFunc*) const;
            void process(SyAlways*) const;
            void process(SyCase*) const;
            void process(SyIf*) const;
            void process(SyBlockAssign*) const;
            void process(SyNonBlockAssign*) const;
            void process(SyContAssign*) const;
            void process(SyStmt*) const;
            void process(const SyList<SyStmt*>&) const;

            SyExpr* process(SyExpr*, std::pair<int, bool>) const;
            SyExpr* process(SyCondExpr*, std::pair<int, bool>) const;
            SyExpr* process(SyUnary*, std::pair<int, bool>) const;
            SyExpr* process(SyBinary*, std::pair<int, bool>) const;

            SyExpr* process(SyCntv*, std::pair<int, bool>) const;
            SyExpr* process(SyFuncCall*, std::pair<int, bool>) const;
            SyExpr* processCommon(SyExpr*, std::pair<int, bool>) const;

            
            std::pair<int, bool> findSS(const SyExpr*, std::pair<int, bool>, int) const;
            std::pair<int, bool> findSS(const SyCondExpr*, std::pair<int, bool>, int) const;
            std::pair<int, bool> findSS(const SyUnary*, std::pair<int, bool>, int) const;
            std::pair<int, bool> findSS(const SyBinary*, std::pair<int, bool>, int) const;

            std::pair<int, bool> findSS(const SyCntv*, std::pair<int, bool>, int) const;
            std::pair<int, bool> findSS(const SyConst*, std::pair<int, bool>, int) const;
            std::pair<int, bool> findSS(const SyFuncCall*, std::pair<int, bool>, int) const;

            template<class T> void
            processListOfObjects(const SyList<T*>& tlist) const;

            static SyExprMgr *exprMgr;

    };
}
