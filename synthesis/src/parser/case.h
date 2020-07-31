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

#ifndef _CASE_H_
#define _CASE_H_

#include <vector>

#include "stmt.h"
namespace Rtl
{
    class CaseItem
    {
        public:
            CaseItem(const std::vector<Expr*>& l,
                const Stmt* st): 
                label(const_cast<std::vector<Expr*>&>(l)),
                stmt(const_cast<Stmt*>(st)){
            }
            bool isDefault() const {
                if(label.size() == 1 && 
                    (label[0]->toString() == std::string("\"default\""))){
                    return true;
                }
                return false;
            }
            const std::vector<Expr*>& getLabel() const{
                return label;
            }

            const Stmt* getStmt() const{
                return stmt;
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }

            CaseItem* repExpr(const std::string& str, const Expr* expr)
            {
                for(unsigned int i = 0; i < label.size(); ++i) {
                    label[i] = const_cast<Expr*>(label[i]->repExpr(str, expr));
                }
                stmt->repExpr(str, expr);
                return this;
            }    
        private:
            std::vector<Expr*> label;
            Stmt* stmt;
    };

    class Case: public Stmt
    {
        friend class GenUnroll;
        public:
            Case(const std::vector<CaseItem*>& ci, const Expr* se, 
                const CaseType t):
                Stmt(), 
                caseItems(const_cast<std::vector<CaseItem*>&>(ci)),
                selectExpr(const_cast<Expr*>(se)), type(t){
            }
            const Expr* getSelectExpr() const {
                return selectExpr;
            }

            const std::vector<CaseItem*>& getCaseItems() const{
                return caseItems;
            }
            CaseType getType() const {
                return type;
            }
            virtual void accept(OMVisitor* v) const{
                v->visit(this);
            }

            virtual Case* copy() const {
                std::vector<CaseItem*> ccaseItems;
                const int size = caseItems.size();
                for(int i = 0; i < size; ++i){
                    std::vector<Expr*> clabel;
                    const std::vector<Expr*>& label = caseItems[i]->getLabel();
                    for(unsigned int j = 0; j < label.size(); ++j){
                        clabel.push_back(label[i]->copy());
                    }
                    Stmt* cstmt = caseItems[i]->getStmt()->copy();
                    ccaseItems.push_back(new CaseItem(clabel, cstmt));
                }
                Expr* cselectExpr = selectExpr->copy();
                return new Case(ccaseItems, cselectExpr, type);
            }

            virtual void repExpr(const std::string& str, const Expr *expr)
            {
                for(unsigned int i = 0; i < caseItems.size(); ++i) {
                    caseItems[i] = caseItems[i]->repExpr(str, expr);
                }
                selectExpr = const_cast<Expr*>(selectExpr->repExpr(str, expr));
                //return this;
            }
        private:
            void setSelectExpr(const Expr *expr) {
                selectExpr = const_cast<Expr*>(expr);
            }

            std::vector<CaseItem*> caseItems;
            Expr* selectExpr;
            CaseType type;
    };
}
#endif
