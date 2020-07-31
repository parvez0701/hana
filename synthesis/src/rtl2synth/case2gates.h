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

#ifndef _CASE2GATES_H_
#define _CASE2GATES_H_
#include <map>
#include <string>
#include <vector>

#include "syutils.h"

namespace Synth
{
    class SyAssign;
    
    class CntvWrap
    {
        public:
            CntvWrap(const SyCntv *c) : cntv(c){
            }
            ~CntvWrap() {
            }
            bool operator < (const CntvWrap& rhs) const {
                return cmp_cntvs()(cntv, rhs.cntv);
            }
            bool operator == (const CntvWrap& rhs) const {
                return cntv->toString() == rhs.cntv->toString();
            }    
            bool operator != (const CntvWrap& rhs) const {
                return !(*this == rhs);
            }    
            operator const SyCntv* () const {    
                return cntv;
            }    
            
        private:    
            const SyCntv *cntv;
    };
    class CntvInfo
    {
        public:
            //bool == true -> Block assignf, bool == false -> Non block assi
            typedef std::vector<std::pair<SyExpr*, bool> > CntvAssignsInCaseItem;
            //position in vector signifies the label
            typedef std::vector<CntvAssignsInCaseItem*> CntvAssignsInCase;
            //typedef std::map<SyCntv*, CntvAssignsInCase, cmp_cntvs> CntvAssignInfoMap;
            typedef std::map<CntvWrap, CntvAssignsInCase> CntvAssignInfoMap;
            //For recursively defined case statements
            typedef std::map<int, std::vector<CntvInfo*> >InnerCaseCntvInfo;

            typedef enum {UNSET, ALWAYS, FUNC, TASK} ScopeType;
            typedef enum {COMB, MUX, PRIDEC, DEC} CaseType; 

            class OutCntvInfo
            {
                public:
            
                    SyCntv    *cntv;
                    CaseType    type;
                    //Appear in consecutive items and no where else
                    //so that we have a smaller MUX
                    int         start; 
                    int         end;
                    bool operator < (const OutCntvInfo &c) const
                    {
                        cmp_cntvs cmp;
                        return cmp(cntv, c.cntv);
                    }    
            };    
            typedef std::set<OutCntvInfo> OutCntvInfoSet;
                    
                
        public:
            CntvInfo(const SyCase*);
            ~CntvInfo();

            OutCntvInfo getOutCntvInfo(const SyCntv*) const;

            void analyze();

            //utility functions
            int getSelectSize() const;
            void print() const;

        private:
            CntvInfo();
            DISABLE_COPYCONST_ASSGNOP(CntvInfo);

            void init();
            void init(const SyCntv*);
            void merge();
            void merge(SyList<SyCaseItem*>&, bool=true);
            void merge(const SyCntv*);
            void populateCntvInfo();
            void populateCntvInfo(const SyCaseItem*);
            void populateOutCntvInfo();
            bool isMux(const SyCntv*) const;
            std::pair<int, int> getStartEnd(const SyCntv*) const;
            CaseType getType(const SyCntv*) const;

            void checkAndAdd(const SyStmt*, int);
            void checkAndAdd(const SyAssign*, int);
            void checkAndAdd(const SyCase*, int);
            void checkAndAdd(const CntvInfo*, int);

            void checkAndAdd(const SyExpr*, const SyExpr*, int, bool);
            void checkAndAdd(const SyCntv*, const SyExpr*, int, bool);

            bool assignsEquivalent(const SyCntv*, int, int) const;
            void removeCntvEntry(const SyCntv*, int);
            void sortCaseItems(SyList<SyCaseItem*>&, int size) const;    

            //If label == -1 it will check if the entry exists in all
            //branches of the case item
            bool entryExists(const SyCntv*, int label=-1) const;
            CntvInfo* analyze(const SyCase*);
            
            CntvAssignInfoMap    cntvAssignInfoMap;
            InnerCaseCntvInfo    innerCaseCntvInfo;
            SyCase                *case_;
            OutCntvInfoSet         outCntvInfoSet;
    };

}

#endif
