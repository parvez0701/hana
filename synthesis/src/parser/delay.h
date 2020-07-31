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

#ifndef _DELAY_H_
#define _DELAY_H_
#include <vector>

#include "base.h"
#include "stmtcont.h"
#include "omvisitor.h"
#include "expr.h"

namespace Rtl
{
    //class Expr;
    class DelayVal : public Base
    {
        public:
            DelayVal(const Expr* e) : Base(){
                if(e){
                    dexpr.push_back(const_cast<Expr*>(e));
                }
            }

            DelayVal(const Expr* e1, const Expr* e2, const Expr* e3):
                Base()
            {
                dexpr.push_back(const_cast<Expr*>(e1));
                dexpr.push_back(const_cast<Expr*>(e2));
                dexpr.push_back(const_cast<Expr*>(e3));
            }

            virtual ~DelayVal(){
            }
            const std::vector<Expr*>& getDelExpr() const{
                return dexpr;
            }
            virtual DelayVal* copy() const {
                std::vector<Expr*> cdexpr;
                const int size = dexpr.size();
                for(int i = 0; i < size; ++i){
                    cdexpr.push_back(dexpr[i]->copy());
                }
                if(cdexpr.size() == 1){
                    return new DelayVal(cdexpr[0]);
                }
                else if(cdexpr.size() == 3){
                    return new DelayVal(cdexpr[0], cdexpr[1], cdexpr[2]);
                }
                return 0;
            }
        private:
            std::vector<Expr*> dexpr;
    };

    class Delay : public Base
    {
        public:
            Delay(const DelayVal* dv): Base(){
                dvals.push_back(const_cast<DelayVal*>(dv));
            }
            Delay(const DelayVal* dv1, const DelayVal* dv2): Base(){
                dvals.push_back(const_cast<DelayVal*>(dv1));
                dvals.push_back(const_cast<DelayVal*>(dv2));
            }

            Delay(const DelayVal* dv1, const DelayVal* dv2,
                const DelayVal* dv3): Base(){
                dvals.push_back(const_cast<DelayVal*>(dv1));
                dvals.push_back(const_cast<DelayVal*>(dv2));
                dvals.push_back(const_cast<DelayVal*>(dv3));
            }
            virtual ~Delay(){
            }

            const std::vector<DelayVal*>& getDelVals() const{
                return dvals;
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual Delay* copy() const{
                std::vector<DelayVal*> cdvals;
                const int size = dvals.size();
                for(int i = 0; i < size; ++i){
                    cdvals.push_back(dvals[i]->copy());
                }
                if(cdvals.size() == 1){
                    return new Delay(cdvals[0]);
                }
                else if(cdvals.size() == 2){
                    return new Delay(cdvals[0], cdvals[1]);
                }
                else if(cdvals.size() == 3){
                    return new Delay(cdvals[0], cdvals[1], cdvals[2]);
                }
                return 0;
            }
        private:
            std::vector<DelayVal*> dvals;
    };

    class DelayStmt: public StmtCont
    {
        public:
            DelayStmt(const Delay* d, const Stmt* st): StmtCont(st),
                delay(const_cast<Delay*>(d)){
            }
            const Delay* getDelay() const {
                return delay;
            }
            virtual void accept(OMVisitor* v) const{
                v->visit(this);
            }
            virtual DelayStmt* copy() const {
                return new DelayStmt(delay->copy(), this->getStmt()->copy());
            }
        private:
            Delay* delay;
    };
}
#endif

