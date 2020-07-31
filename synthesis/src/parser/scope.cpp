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

#include "scope.h"
#include "blockdecl.h"
#include "vecnet.h"
#include "vecreg.h"
#include "vecint.h"
#include "vectime.h"
#include "vtime.h"
#include "vecreal.h"
#include "vecrealtime.h"
#include "event.h"
#include "omvisitor.h"
#include "stmt.h"
using std::string;
using std::map;

namespace Rtl
{
    Scope::Scope(const string& n): name(n), blockdecl(new BlockDecl()){
    }

    Scope::Scope(): blockdecl(new BlockDecl()){
    }
    Scope::~Scope(){
        symbolTable.clear();
    }

    //_________________________________________________________________
    //    Base* Scope::addObject()
    //    Adds an object to the scope.
    //    If object does not exist with name n then it adds the 'b'
    //    and returns b;
    //    If object exists and rep is provided then it replaces the
    //    object and returns b. If rep is not provided then it returns
    //    the NULL object and does nothing.
    Base*
    Scope::addObject(const string& n,
        const Base* b, const bool rep)
    {
        if(rep)
        {
            this->removeObject(n);
            symbolTable.insert(make_pair(const_cast<string&>(n),
            const_cast<Base*>(b)));
            this->addDeclItem(const_cast<Base*>(b));
            return const_cast<Base*>(b);
        }
        Base* tobj = this->getObject(n);
        if(tobj){
            return NULL;
        }
        this->addDeclItem(const_cast<Base*>(b));
        symbolTable.insert(make_pair(n, const_cast<Base*>(b)));
        return const_cast<Base*>(b);
    }
    Base*
    Scope::getObject(const string& oname) const
    {
        map<string, Base*, compare_str>::const_iterator
        obj = symbolTable.find(oname);
        if(obj == symbolTable.end()){
            return NULL;
        }

        return obj->second;
    }

    void
    Scope::repExprInStmts(const string& str, const Expr *expr)
    {
        for(unsigned int i = 0; i < stmts.size(); ++i) {
            stmts[i]->repExpr(str, expr);
        }    
    }

    void
    Scope::addDeclItem(Base* b)
    {
        if(Reg* reg = dynamic_cast<Reg*>(b)) 
        {
            blockdecl->regs.push_back(reg);
            reg->setScope(this);
            //addObject(reg->getIxName().getName(), reg, false);
        }
        else if(VecReg* vreg = dynamic_cast<VecReg*>(b))
        {
            const std::vector<Var*>& regs = vreg->getVars();
            const int size = regs.size();
            for(int i = 0; i < size; ++i) 
            {
                regs[i]->setScope(this);
                blockdecl->regs.push_back(static_cast<Reg*>(regs[i]));
            }
            //addObject(vreg->getName(), vreg, false);
        }

        else if(Integer* int_ = dynamic_cast<Integer*>(b)) 
        {
            blockdecl->ints.push_back(int_);
            int_->setScope(this);
            //addObject(int_->getIxName().getName(), int_, false);
        }
        else if(VecInt* vint = dynamic_cast<VecInt*>(b))
        {
            const std::vector<Var*>& ints = vint->getVars();
            const int size = ints.size();
            for(int i = 0; i < size; ++i) 
            {
                ints[i]->setScope(this);
                blockdecl->ints.push_back(static_cast<Integer*>(ints[i]));
            }
            //addObject(vint->getName(), vint, false);
        }
        else if(Time* time = dynamic_cast<Time*>(b)) 
        {
            time->setScope(this);
            blockdecl->times.push_back(time);
            //addObject(time->getIxName().getName(), time, false);
        }
        else if(VecTime* vtime = dynamic_cast<VecTime*>(b))
        {
            const std::vector<Var*>& times = vtime->getVars();
            vtime->setScope(this);
            const int size = times.size();
            for(int i = 0; i < size; ++i) 
            {
                times[i]->setScope(this);
                blockdecl->times.push_back(static_cast<Time*>(times[i]));
            }
            //addObject(vtime->getName(), vtime, false);
        }
        else if(Real* real = dynamic_cast<Real*>(b)) 
        {
            real->setScope(this);
            blockdecl->reals.push_back(real);
            //addObject(real->getIxName().getName(), real, false);
        }
        else if(VecReal* vreal = dynamic_cast<VecReal*>(b))
        {
            vreal->setScope(this);
            const std::vector<Var*>& vars = vreal->getVars();
            const int size = vars.size();
            for(int i = 0; i < size; ++i) 
            {
                vars[i]->setScope(this);
                blockdecl->reals.push_back(static_cast<Real*>(vars[i]));
            }
            //addObject(vreal->getName(), vreal, false);
        }

        else if(RealTime* realt = dynamic_cast<RealTime*>(b)) 
        {
            realt->setScope(this);
            blockdecl->realts.push_back(realt);
            //addObject(realt->getIxName().getName(), realt, false);
        }
        else if(VecRealTime* vrealt = dynamic_cast<VecRealTime*>(b))
        {
            vrealt->setScope(this);
            const std::vector<Var*>& vars = vrealt->getVars();
            const int size = vars.size();
            for(int i = 0; i < size; ++i) 
            {
                vars[i]->setScope(this);
                blockdecl->realts.push_back(static_cast<RealTime*>(vars[i]));
            }
            //addObject(vrealt->getName(), vrealt, false);
        }
        else if(EventVar *evar = dynamic_cast<EventVar*>(b)) 
        {
            evar->setScope(this);
            blockdecl->evars.push_back(evar);
        }
    }

    void
    Scope::acceptOMVisitor(OMVisitorBase* v) const
    {
        int size = blockdecl->regs.size();
        for(int i = 0; i < size; ++i){
            v->visit(blockdecl->regs[i]);
        }

        size = blockdecl->ints.size();
        for(int i = 0; i < size; ++i){
            v->visit(blockdecl->ints[i]);
        }

        size = blockdecl->times.size();
        for(int i = 0; i < size; ++i){
            v->visit(blockdecl->times[i]);
        }
    
        size = blockdecl->reals.size();
        for(int i = 0; i < size; ++i){
            v->visit(blockdecl->reals[i]);
        }

        size = blockdecl->realts.size();
        for(int i = 0; i < size; ++i){
            v->visit(blockdecl->realts[i]);
        }
    }
}
