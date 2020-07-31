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

#ifndef _SYBASE_H_
#define _SYBASE_H_

#include <map>
#include <string>
#include <string.h>

namespace Synth
{
    class SyExpr;
    class SyVisitorBase;
    class TVisitor;
    class SyModule;
    class SyBase
    {
        public:
            struct cmp_str{
                bool operator()(const std::string& s1, const std::string& s2) const{
                    return strcmp(s1.c_str(), s2.c_str()) < 0;
                }
            };
            SyBase() {
            }

            virtual ~SyBase() {
            }
            void addAttrib(
                    const std::string& key,
                    const SyExpr* val,
                    const bool rep=false)
            {
                if(rep) {
                    attribTable.erase(key);
                }
                attribTable.insert(std::make_pair(key, val));
            }

            const SyExpr* getAttrib(const std::string& key) const
            {
                std::map<const std::string, const SyExpr*, cmp_str>::
                    const_iterator iter = attribTable.find(key);
                if(iter == attribTable.end()){
                    return 0;
                }
                return iter->second;
            }
            virtual SyBase* copy() const {
                return const_cast<SyBase*>(this);
            }

            virtual void accept(SyVisitorBase*) const {
            }
            virtual void accept(TVisitor*, SyModule*) {
            }
        private:
            std::map<const std::string, 
                const SyExpr*, cmp_str> attribTable;
    };
}
#endif
