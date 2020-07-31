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

#ifndef _TASKFUNC_H_
#define _TASKFUNC_H_
#include <string>
#include <vector>
#include <assert.h>

#include "stmt.h"
#include "expr.h"
#include "scope.h"
namespace Rtl
{
    class Port;
    class TaskOrFunc: public Scope
    {
        friend class OMCreator;
        friend class Elaborator;
        public:
            TaskOrFunc(const std::string& name, const Stmt* st, 
                const bool a): 
                Scope(name), stmt(const_cast<Stmt*>(st)), auto_(a){
            }

            virtual ~TaskOrFunc(){
            }
            void addPorts(const std::vector<Port*>&);
            const std::vector<Port*>& getPorts() const{
                return ports;
            }


            bool isAuto() const {
                return auto_;
            }
            const Stmt* getTaskOrFuncStmt() const{
                return stmt;
            }
        private:
            void setStmt(const Stmt* s){
                if(s) {
                    this->addStmt(s);
                }    
                stmt = const_cast<Stmt*>(s);
            }
            std::vector<Port*> ports;
            Stmt* stmt;
            bool auto_;
    };

    class Task: public TaskOrFunc
    {
        public:
            Task(const std::string& name, const Stmt* st, const bool a):
                TaskOrFunc(name, st, a){
            }

            virtual ~Task(){
            }
            const Stmt* getTaskStmt() const {
                return this->getTaskOrFuncStmt();
            }
            virtual void accept(OMVisitorBase*) const;
            virtual Task* copy() const {
                assert(0);
            }    
    };

    class Func: public TaskOrFunc
    {
        public:
            Func(const std::string& name, 
                const bool a,
                const std::string& type,
                const Range* r,
                bool s): 
                TaskOrFunc(name, NULL, a),
                rettype(type),
                range(const_cast<Range*>(r)),
                sig(s){
            }

            virtual ~Func(){
            }
            const Range* getRange() const{
                return range;
            }
            const std::string& getRetType() const{
                return rettype;
            }
            const Stmt* getFuncStmt() const {
                return this->getTaskOrFuncStmt();
            }

            bool isSigned() const{
                return sig;
            }

            virtual void accept(OMVisitorBase*) const;

            virtual Func* copy() const {
                assert(0);
            }    
        private:
            std::string rettype;
            Range* range;
            bool sig;
    };
        
    class TaskFuncCall: public Stmt
    {
        public:
            TaskFuncCall(const std::string& n,
                const std::vector<Expr*>& a,
                const bool b): Stmt(),
                name(n), args(const_cast<std::vector<Expr*>& >(a)),
                builtin(b){
            }

            virtual ~TaskFuncCall(){
            }
            const std::string& getName() const{
                return name;
            }

            const bool isBuiltIn() const{
                return builtin;
            }

            const std::vector<Expr*>& getArgs() const{
                return args;
            }
            
            virtual void accept(OMVisitorBase* v) const;
            virtual TaskFuncCall* copy() const;
            virtual void repExpr(const std::string&, const Expr*);
            const Task *getTask() const;
        private:
            std::string name;
            std::vector<Expr*> args;
            bool builtin;
    };

    //Function call to used in expressions
    class FuncCall: public Expr
    {
        public:
            FuncCall(const std::string& n,
                const std::vector<Expr*>& a,
                const bool b): Expr(),
                actCall(new TaskFuncCall(n, a, b)){
            }

            virtual ~FuncCall(){
            }
            const std::string& getName() const{
                return actCall->getName();
            }

            const bool isBuiltIn() const{
                return actCall->isBuiltIn();
            }

            const std::vector<Expr*>& getArgs() const{
                return actCall->getArgs();
            }

            virtual std::string toString() const;
            virtual long len() const {
                return -1;
            }

            
            virtual void accept(OMVisitorBase* v) const;
            virtual FuncCall* copy() const;
            virtual const Expr* repExpr(const std::string&, const Expr*);

            operator TaskFuncCall* () const {
                return actCall;
            }    

            const Func* getFunc() const;

            virtual bool isSigned() const {
                return getFunc()->isSigned();
            }    
        private:
            TaskFuncCall* actCall;
    };
    
}
#endif
