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

#include <algorithm>

#include "omvisitorbase.h"
#include "event.h"
#include "taskfunc.h"
#include "contassign.h"
#include "defparam.h"
#include "initial.h"
#include "always.h"
#include "udpinst.h"
#include "modinst.h"
#include "gateinst.h"

#include "generate.h"
#include "net.h"
#include "var.h"

using std::vector;
using std::string;
using std::pair;
using std::for_each;
using std::front_inserter;
using std::copy;

namespace Rtl
{
    template<class T>
    struct RepExpr 
    {
        RepExpr(vector<T*>& _elems, const string& s, const Expr *e ) : 
            elems(_elems), str(s), expr(e) {
        }
        void operator()() {
            for(unsigned int i = 0; i < elems.size(); ++i) {
               // elems[i] = const_cast<T*>(dynamic_cast<const T*>(
                //elems[i]->repExpr(str, expr)));
                elems[i]->repExpr(str, expr);
            }    
        }
        vector<T*>& elems;
        const string& str;
        const Expr* expr;
    };
    void
    Genvar::accept(OMVisitorBase *v) const {
        v->visit(this);
    }    

    const Expr*
    Genvar::repExpr(const string& str, const Expr *expr)
    {
        if(name == str) {
            return expr;
        }
        return this;
    }    

    bool
    Genvar::isConstExpr() const {
        return this->isValSet();
    }    

    void
    GenDecl::accept(OMVisitorBase *v) const {
        v->visit(this);
    }    

    void
    GenerateCond::accept(OMVisitorBase *v) const {
        v->visit(this);
    }    


    void
    GenerateLoop::accept(OMVisitorBase *v) const {
        v->visit(this);
    }    
    void
    GenerateBlock::accept(OMVisitorBase *v) const {
        v->visit(this);
    }    
    void
    GenerateCase::accept(OMVisitorBase *v) const {
        v->visit(this);
    }    

    void
    GenerateStmt::accept(OMVisitorBase *v) const {
        v->visit(this);
    }    
    Genvar*
    Genvar::copy() const
    {
        Genvar *newGenvar = new Genvar(name, genvar);
        newGenvar->setScopeAndLine(getScope(), getLine());
        return newGenvar;

    }

    vector<char> 
    Genvar::getValString() const
    {
        int val = genvar;
        vector<char> ret;
        do {
            int tmp = val % 10;
            ret.push_back((char)(tmp+'0'));
            val /= 10;
        } while(val);
        vector<char> tmp;
        std::copy(ret.rbegin(), ret.rend(), back_inserter(tmp));
        return tmp;
                    
    }

    GenDecl*
    GenDecl::copy() const
    {
        GenDecl *newGenDecl = new GenDecl();
        ModGenDecl *gd = newGenDecl->getGenDecl();
        CopyVector<Net*>(gdecl->nets, gd->nets);
        CopyVector<GateInst*>(gdecl->ginsts, gd->ginsts);
        CopyVector<ModInst*>(gdecl->minsts, gd->minsts);
        CopyVector<UDPInst*>(gdecl->uinsts, gd->uinsts);
        CopyVector<Always*>(gdecl->alwaysBlocks, gd->alwaysBlocks);
        CopyVector<Initial*>(gdecl->initials, gd->initials);
        CopyVector<Var*>(gdecl->variables, gd->variables);
        CopyVector<Genvar*>(gdecl->genvars, gd->genvars);
        CopyVector<Defparam*>(gdecl->defparams, gd->defparams);
        CopyVector<ContAssign*>(gdecl->contassigns, gd->contassigns);
        CopyVector<EventStmt*>(gdecl->events, gd->events);
        CopyVector<Task*>(gdecl->tasks, gd->tasks);
        CopyVector<Func*>(gdecl->funcs, gd->funcs);
        return newGenDecl;
    }

    GenerateCond*
    GenerateCond::copy() const
    {
        GenerateCond *gc = new GenerateCond(condexpr->copy());
        gc->ifpart = dynamic_cast<GenerateItem*>(ifpart->copy());
        gc->elsepart = dynamic_cast<GenerateItem*>(elsepart->copy());
        return gc;
    }

    GenerateCase*
    GenerateCase::copy() const
    {
        vector<pair<vector<Expr*>, GenerateItem*> > newCaseItems;
        for(unsigned int i = 0; i < caseitems.size(); ++i)
        {
            vector<Expr*> newExprs;
            for(unsigned int j = 0; j < caseitems[i].first.size(); ++j) {
                newExprs.push_back(caseitems[i].first[j]->copy());
            }
            newCaseItems.push_back(make_pair(newExprs, 
                dynamic_cast<GenerateItem*>(caseitems[i].second->copy())));
        }
        GenerateCase *newGenCase = 
            new GenerateCase(select->copy(), newCaseItems, 
            dynamic_cast<GenerateItem*>(defstmt->copy()));
        return newGenCase;    
    }
    GenerateItem*
    GenerateBlock::copy() const
    {
        GenerateBlock *newGenBlock = new GenerateBlock(this->getName());
        CopyVector<GenerateItem*>(gitems, newGenBlock->gitems);
        return newGenBlock;
    }

    GenerateItem*
    GenerateLoop::copy() const 
    {
        GenerateLoop *genLoop = new GenerateLoop(loopvar->copy(),
            initexpr->copy(), termexpr->copy(), assignexpr->copy());
        genLoop->gitem = dynamic_cast<GenerateItem*>(gitem->copy());
        return genLoop;
    }

    GenerateStmt*
    GenerateStmt::copy() const
    {
        GenerateStmt *genStmt = new GenerateStmt();

        for(unsigned int i = 0; i < genvars.size(); ++i) {
            genStmt->addGenvar(genvars[i]->copy());
        }

        for(unsigned int i = 0; i < conds.size(); ++i) {
            genStmt->addGenerateItem(conds[i]->copy());
        }

        for(unsigned int i = 0; i < cases.size(); ++i) {
            genStmt->addGenerateItem(cases[i]->copy());
        }

        for(unsigned int i = 0; i < loops.size(); ++i) {
            genStmt->addGenerateItem(
            dynamic_cast<GenerateItem*>(loops[i]->copy()));
        }

        for(unsigned int i = 0; i < blocks.size(); ++i) {
            genStmt->addGenerateItem(blocks[i]);
        }

        genStmt->addGenerateItem(gdecl->copy());
        return genStmt;
    }

    void    
    GenDecl::repExpr(const string& str, const Expr *expr)
    {
        //RepExpr<Net>(gdecl->nets, str, expr)();
        RepExpr<Always>(gdecl->alwaysBlocks, str, expr)();
        RepExpr<Initial>(gdecl->initials, str, expr)();
        RepExpr<Defparam>(gdecl->defparams, str, expr)();
        RepExpr<ContAssign>(gdecl->contassigns, str, expr)();
        //return this;
    }

    void    
    GenerateCond::repExpr(const string& str, const Expr* expr)
    {
        condexpr = const_cast<Expr*>(condexpr->repExpr(str, expr));
        ifpart->repExpr(str, expr);
        elsepart->repExpr(str, expr);
        //return this;
    }    

    void    
    GenerateCase::repExpr(const string& str, const Expr* expr)
    {
        select = const_cast<Expr*>(select->repExpr(str, expr));
        for(unsigned int i = 0; i < caseitems.size(); ++i) 
        {
            RepExpr<Expr>(caseitems[i].first, str, expr)();
            caseitems[i].second->repExpr(str, expr);
        }
        defstmt->repExpr(str, expr);
        //return this;
    }

    void    
    GenerateBlock::repExpr(const string& str, const Expr *expr)
    {
        Scope::repExprInStmts(str, expr);
        RepExpr<GenerateItem>(gitems, str, expr);
        //return this;
    }


    void    
    GenerateLoop::repExpr(const string& str, const Expr *expr)
    {
        Scope::repExprInStmts(str, expr);
        loopvar = const_cast<Expr*>(loopvar->repExpr(str, expr));
        initexpr = const_cast<Expr*>(initexpr->repExpr(str, expr));
        termexpr = const_cast<Expr*>(termexpr->repExpr(str, expr));
        assignexpr = const_cast<Expr*>(assignexpr->repExpr(str, expr));
        gitem->repExpr(str, expr);
        //return this;
    }    

    void    
    GenerateStmt::repExpr(const string& str, const Expr *expr)
    {
        Scope::repExprInStmts(str, expr);
        RepExpr<GenerateCond>(conds, str, expr)();
        RepExpr<GenerateCase>(cases, str, expr)();
        RepExpr<GenerateLoop>(loops, str, expr)();
        RepExpr<GenerateBlock>(blocks, str, expr)();
        //gdecl->repExpr(str, expr);
        //return this;
    }    
}    

