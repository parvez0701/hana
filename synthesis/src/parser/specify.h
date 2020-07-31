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

#ifndef _SPECIFY_H_
#define _SPECIFY_H_

#include <vector>
#include <assert.h>

#include "expr.h"
#include "stmt.h"
namespace Rtl
{
    class Specparam : public Expr
    {
        friend class Elaborator;
        public:
            Specparam(const std::string& n, const std::vector<Expr*>& v):
                Expr(), name(n), val(v){
            }
            virtual ~Specparam(){
            }
            const std::string& getName() const{
                return name;
            }
            const std::vector<Expr*>& getVal() const{
                return val;
            }
            virtual std::string toString() const {
                return name;
            }
            virtual long int len() const {
                return -1;
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual Specparam* copy() const {
                std::vector<Expr*> nval;
                for(unsigned int i = 0; i < val.size(); ++i){
                    nval.push_back(val[i]->copy());
                }
                return new Specparam(name, nval);
            }

            virtual const Expr* repExpr(const std::string& str, 
                                        const Expr* expr) 
            {
                if(name == str) {
                    return expr;
                }
                for(unsigned int i = 0; i < val.size(); ++i) {
                    val[i] = const_cast<Expr*>(
                        val[i]->repExpr(str, expr));
                }
                return this;
            }    

            virtual bool isSigned() const {
                return false;
            }    
        private:
            std::string name;
            std::vector<Expr*> val;
    };

    class SpecifyPathBase: public Base
    {
        friend class Elaborator;
        public:
            SpecifyPathBase(char pol, SpecConnType t,
                const std::vector<Expr*>& ss): Base(),
                ifExpr(0), polarity(pol), type(t), srcs(ss){
            }
            virtual ~SpecifyPathBase(){
            }
            bool isConditional() const{
                return ifExpr != 0;
            }
            
            char getPolarity() const{
                return polarity;
            }
            const std::vector<Expr*>& getSrcs() const{
                return srcs;
            }
            const std::vector<DelayVal*>& getDelay() const{
                return delays;
            }
            void setDelays(const std::vector<DelayVal*>& d) {
                delays = d;
            }
            void setIfExpr(const Expr* e){
                ifExpr = const_cast<Expr*>(e);
            }

            const Expr* getIfExpr() const{
                return ifExpr;
            }
            SpecConnType getConnType() const{
                return type;
            }
        private:
            Expr* ifExpr;
            char polarity;
            SpecConnType type;
            std::vector<Expr*> srcs;
            std::vector<DelayVal*> delays;
    };

    class SpecifyPath: public SpecifyPathBase
    {
        friend class Elaborator;
        public:
            SpecifyPath(char pol, SpecConnType t,
                const std::vector<Expr*>& ss, 
                const std::vector<Expr*>& dest):
                SpecifyPathBase(pol, t, ss),
                destIds(dest){
            }
            virtual ~SpecifyPath(){
            }
            
            const std::vector<Expr*>& getDestIds() const{
                return destIds;
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }

            virtual SpecifyPath* copy() const {
                //Not supported yet
                assert(0);
            }    
        private:
            std::vector<Expr*> destIds;
    };

    class SpecifyEdgePath: public SpecifyPathBase
    {
        friend class Elaborator;
        public:
            SpecifyEdgePath(EdgeType et, const Expr* id, PolOp pop,
                const std::pair<Expr*, Expr*>& des, char polarity, 
                SpecConnType sct, const std::vector<Expr*>& ss):
                SpecifyPathBase(polarity, sct, ss),
                edgetype(et), destId(const_cast<Expr*>(id)), 
                polop(pop), destExprs(
                const_cast<std::pair<Expr*, Expr*>&>(des)){
            } 
            virtual ~SpecifyEdgePath(){
            }
            const Expr* getDestId() const{
                return destId;
            }
            PolOp getPolOp() const{
                return polop;
            }
            EdgeType getEdgeType() const{
                return edgetype;
            }
            const std::pair<Expr*, Expr*>& getDestExprPair() const{
                return destExprs;
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            virtual SpecifyEdgePath* copy() const {
                //Not supported yet
                assert(0);
            }    
        private:
            EdgeType edgetype;
            Expr* destId;
            PolOp polop;
            std::pair<Expr*, Expr*> destExprs;
    };

    class SpecRefEvent: public Base
    {
        friend class Elaborator;
        public:

            SpecRefEvent(const EdgeType t, const Expr* pe, const Expr* e)
                : Base(), edgetype(t), primExpr(const_cast<Expr*>(pe)),
                expr(const_cast<Expr*>(e)){
            }

            virtual ~SpecRefEvent(){
            }

            EdgeType getEdgeType() const{
                return edgetype;
            }

            const Expr* getPrimExpr() const{
                return primExpr;
            }
            const Expr* getExpr() const{
                return expr;
            }
            virtual SpecRefEvent* copy() const {
                //Not supported yet
                assert(0);
            }    
            //virtual void accept(OMVisitor* v) const{
            //    v->visit(this);
            //}
        private:
            EdgeType edgetype;
            Expr* primExpr;
            Expr* expr;
    };

    class SpecTask: public Base
    {
        friend class Elaborator;
        public:
            SpecTask(const std::string& n,
                const std::pair<SpecRefEvent*, SpecRefEvent*>& srv,
                const std::pair<DelayVal*, DelayVal*>& dvs,
                const std::vector<Expr*>& notifs): Base(),
                name(n), specRefEvents(srv),
                delayvals(dvs), notifiers(notifs){
            }
            virtual ~SpecTask(){
            }
            const std::pair<SpecRefEvent*, SpecRefEvent*>&
                getSpecRefEvents() const{
                return specRefEvents;
            }
            const std::pair<DelayVal*, DelayVal*>& 
                getDelayVals() const{
                return delayvals;
            }
            const std::vector<Expr*>& getNotifiers() const{
                return notifiers;
            }
            const std::string& getName() const {
                return name;
            }
            virtual SpecTask* copy() const {
                //Not supported yet
                assert(0);
            }    
            //virtual void accept(OMVisitor* v) const{
            //    v->visit(this);
            //}
        private:
            std::string name;
            std::pair <SpecRefEvent*, SpecRefEvent*> specRefEvents;
            std::pair<DelayVal*, DelayVal*> delayvals;
            std::vector<Expr*> notifiers;
    };

    class SpecifyBlock: public Stmt
    {
        friend class OMCreator;
        friend class Elaborator;
        public:
            SpecifyBlock(): Stmt(){
            }
            virtual ~SpecifyBlock(){
            }
            virtual void accept(OMVisitorBase* v) const{
                v->visit(this);
            }
            const std::vector<Specparam*>& getSpecparams() const {
                return specparams;
            }
            const std::vector<SpecifyPathBase*>& getSpecPaths() const{
                return specifyPaths;
            }

            const std::vector<SpecTask*>& getSpecTasks() const{
                return specTasks;
            }
            const Specparam* getSpecparam(const std::string& n) const
            {
                const int size = specparams.size();
                for(int i = 0; i < size; ++i){
                    if(specparams[i]->getName() == n){
                        return specparams[i];
                    }
                }
                return NULL;
            }
            virtual SpecifyBlock* copy() const {
                assert(0);
            }    
            virtual void repExpr(    const std::string& str,
                                            const Expr* expr) {
                //Not implemented yet
                //return this;
            }    
                
        private:
            void addSpecparams(const std::vector<Specparam*>& s){
                const int size = s.size();
                for(int i = 0; i < size; ++i){
                    specparams.push_back(s[i]);
                }
                //specparams = s;
            }
            void addSpecPath(const SpecifyPathBase* s){
                if(s){
                    specifyPaths.push_back(
                    const_cast<SpecifyPathBase*>(s));
                }
            }
            void addSpecTask(const SpecTask* st){
                specTasks.push_back(const_cast<SpecTask*>(st));
            }
            std::vector<Specparam*> specparams;
            std::vector<SpecifyPathBase*> specifyPaths;
            std::vector<SpecTask*> specTasks;
    };
        
}
#endif
