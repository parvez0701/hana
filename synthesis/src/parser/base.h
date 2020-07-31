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

#ifndef _BASE_H_
#define _BASE_H_

#include "defines.h"
namespace Rtl
{
    class Scope;
    class OMVisitorBase;
    class Expr;
    class Base
    {
        public:
            //Base(): scope(0), lineno(OMCreator::instance()->getLine()){
            Base();
            virtual ~Base(){
            }
            void setScope(const Scope* s){
                scope = const_cast<Scope*>(s);
            }
            const Scope* getScope()const{
                return scope;
            }
            virtual void accept(OMVisitorBase* v) const{
            }
            void setLine(const int l){
                lineno = l;
            }
            int getLine() const {
                return lineno;
            }
            void setAttribTable(const AttribTable* at){
                attributes = const_cast<AttribTable*>(at);
            }
            void setAttrib(const std::string& name, const Expr* expr){
                if(!attributes){
                    attributes = new  AttribTable();
                }
                attributes->insert(std::make_pair(
                    const_cast<std::string&>(
                    name), const_cast<Expr*>(expr)));
            }

            const Expr* getAttribValue(const std::string& key) const{
                if(!attributes){
                    return NULL;
                }
                AttribTable::const_iterator iter = attributes->find(key);
                if(iter == attributes->end()){
                    return NULL;
                }
                return iter->second;
            }
            void setScopeAndLine(const Scope *sc, int line){
                scope = const_cast<Scope*>(sc);
                lineno = line;
            }    
            virtual Base* copy() const = 0;
            //virtual Base* copy() const {
            //    return const_cast<Base*>(this);
            //}
        private:
            int lineno;
            Scope* scope;
            //Attribute table in base class is wasteful.
            //This should be removed from here.
            AttribTable* attributes;
    };
}
#endif
