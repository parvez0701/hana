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

#include <assert.h>

#include "hierid.h"
#include "omvisitor.h"
#include "root.h"
#include "module.h"

using std::string;
namespace Rtl
{
    extern Root* GRoot;
    void
    HierId::accept(OMVisitorBase* v) const{
        v->visit(this);
    }

    string
    HierId::toString() const{
        return id;
    }

    Path
    HierId::getIdAsPath() const
    {
        int start = 0;
        int index = 0;
        Path path;
        string substr = id;
        while((index = substr.find('.')) != string::npos){
            path.descend(id.substr(start, index));
            start = index + 1;
            substr = substr.substr(start);
        }
        //push the last remaining
        path.descend(substr);
        return path;
    }

    Path
    HierId::getResolvedPath() const
    {
        //returns absolute path.
        //should be called after hier id has been resolved.
        return idpath;
    }

    bool 
    HierId::resolve(const Path& parentpath)
    {
        idpath = this->getIdAsPath();
        string top = idpath.top();
        if(!GRoot->isTopModule(top)) {
            //not a completely specified path
            idpath = parentpath + idpath;
        }
        Path tidpath = idpath;
        string varname = tidpath.ascend();
        //bool done = false;
        top = idpath.top();
        vector<string> instnames = tidpath.instnames();
        int count = 0;
        const int size = instnames.size();
        const Module* mod = dynamic_cast<const Module*>(GRoot->getObject(top));
        assert(mod);

        while(1)
        {
            const Base* b = mod->getObject(varname);
            if(const Expr* expr = dynamic_cast<const Expr*>(b))
            {
                actual = const_cast<Expr*>(expr);
                return true;
            }
            if(count == size){
                return false;
            }

            const ModInst* inst = dynamic_cast<const ModInst*>(
                    mod->getObject(instnames[count]));
            if(inst)
            {
                mod = inst->getMaster();
                if(!mod) {
                    return false;
                }    
                ++count;
                continue;
            }
            const Scope* sc = dynamic_cast<const Scope*>(mod->getObject(
                        instnames[count]));
            while(sc)
            {
                b = sc->getObject(varname);
                if(const Expr* expr = dynamic_cast<const Expr*>(b))
                {
                    actual = const_cast<Expr*>(expr);
                    return true;
                }
                sc = dynamic_cast<const Scope*>(sc->getObject(
                        instnames[++count]));
            }
            return false;
        }
        return false; //should not reach here
    }

    const Expr*
    HierId::repExpr(const string& str, const Expr *expr)
    {
        if(id == str) {
            return expr;
        }
        if(actual) {
            actual = const_cast<Expr*>(actual->repExpr(str, expr));
        }
        return this;
    }

}
