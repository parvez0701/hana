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

#include "taskfunc.h"
#include "omvisitor.h"
#include "port.h"

#include "module.h"

using std::string;
using std::vector;

namespace Rtl
{
    void
    TaskOrFunc::addPorts(const std::vector<Port*>& p)
    {
        const int size = p.size();
        for(int i = 0; i < size; ++i)
        {
            if(!p[i]->isVecBit()){
                this->addObject(p[i]->getIxName().getName(),
                p[i], false);
            }
            p[i]->setScope(this);
            ports.push_back(p[i]);
        }
    }
    
    void
    Task::accept(OMVisitorBase* v) const {
        v->visit(this);
    }

    void
    Func::accept(OMVisitorBase* v) const {
            v->visit(this);
    }
    void
    TaskFuncCall::accept(OMVisitorBase* v) const{
        v->visit(this);
    }

    const Task*
    TaskFuncCall::getTask() const
    {
        return dynamic_cast<const Task*>(GetModule(this)->getObject(name));
    }

    void
    FuncCall::accept(OMVisitorBase* v) const {
        v->visit(this);
    }

    const Func*
    FuncCall::getFunc() const
    {
        return dynamic_cast<const Func*>(GetModule(this)->getObject(actCall->getName()));
    }
    string 
    FuncCall::toString() const
    {
        string str;
        str = this->getName() + string("(");
        vector<Expr*> args = actCall->getArgs();

        const int size = args.size();
        if(size == 0)
        {
            str = str + string(")");
            return str;
        }
        else
        {
            str = str + args[0]->toString();
            for(int i = 1; i < size; ++i)
            {
                str = str + string(", ");
                str = str + args[i]->toString();
            }
            str = str + string(")");
        }
        return str;
    }

    FuncCall*
    FuncCall::copy() const
    {
        const vector<Expr*>& args = this->getArgs();
        vector<Expr*> newargs;
        for(unsigned int i = 0; i < args.size(); ++i){
            newargs.push_back(args[i]->copy());
        }
        return new FuncCall(this->getName(), newargs, this->isBuiltIn());
    }

    const Expr*
    FuncCall::repExpr(const string& str, const Expr* expr) 
    {
        if(this->toString() == str) { //check
            return expr;
        }
        return this;
    }

    TaskFuncCall*
    TaskFuncCall::copy() const
    {
        vector<Expr*> newargs;
        for(unsigned int i = 0; i < args.size(); ++i){
            newargs.push_back(args[i]->copy());
        }

        return new TaskFuncCall(name, newargs, builtin);
    }

    void    
    TaskFuncCall::repExpr(const string& str, const Expr *expr)
    {
        for(unsigned int i = 0; i < args.size(); ++i) {
            args[i] = const_cast<Expr*>(args[i]->repExpr(str, expr));
        }
        //return this;
    }

    
}
