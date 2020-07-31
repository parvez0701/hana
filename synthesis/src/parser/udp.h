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

#ifndef _UDP_H_
#define _UDP_H_
#include <vector>

#include "port.h"

namespace Rtl
{
    class TableEntry;
    typedef std::pair<char, char> InputCharP;
    typedef std::vector<TableEntry*> UDPTable;
    class TableEntry: public Base
    {
        friend class OMCreator;
        public:
            TableEntry(const std::vector<char>& in, char out): Base(),
                inval(const_cast<std::vector<char>&>(in)),
                outval(out),
                currState(' '){
            }
            TableEntry(const std::vector<char>& in, char out, char cs):
                Base(),
                inval(const_cast<std::vector<char>&>(in)),
                outval(out),
                currState(cs) {
            }
        //    virtual void accept(OMVisitor* v) const{
        //        v->visit(this);
        //    }
            virtual TableEntry* copy() const {
                assert(0);
            }    
            std::vector<char> inval;
            char outval;
            char currState;
    };
    class UDP : public Base
    {
        friend class OMCreator;
        public:
            UDP(const std::string& n): Base(), name(n){
            }
            virtual ~UDP(){
            }

            const std::string& getName() const {
                return name;
            }
            const std::vector<Port*>& getInPorts() const{
                return inports;
            }
            const Port* getOutPort() const{
                return outport;
            }
            bool isSequential() const {
                return table[0]->currState != ' ';
                //return seq;
            }
            const Expr* getInitVal() const{
                return initval;
            }
            void addTableEntry(const TableEntry* t){
                table.push_back(const_cast<TableEntry*>(t));
            }

            const UDPTable& getUDPTable() const {
                return table;
            }
            void setInitVal(const Expr* e){
                initval = const_cast<Expr*>(e);
            }
            const Port* getPort(const std::string& pname) const{
                if(outport->getIxName().getName() == pname){
                    return outport;
                }
                const int size =  inports.size();
                for(int i = 0; i < size; ++i){
                    if(inports[i]->getIxName().getName() == pname){
                        return inports[i];
                    }
                }
                return NULL;
            }
            virtual void accept(OMVisitorBase* v) const {
                v->visit(this);
            }
            virtual UDP* copy() const {
                assert(0);
            }    
        private:
            void addInPort(const Port* p){
                inports.push_back(const_cast<Port*>(p));
            }
            void setOutPort(const Port* p){
                outport = const_cast<Port*>(p);
            }
            std::string name;
            std::vector<Port*> inports;
            Port* outport;
            Expr* initval;
            bool seq;
            UDPTable table;
    };
}
#endif
