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

#ifndef _SPIN_H_
#define _SPIN_H_

#include "sport.h"

namespace stview
{
    class SInstBase;
    class SInst;
    class SBlockInst;
    class SPin: public SCntv
    {
        public:
            SPin(const std::string&);
            SPin(const std::string&, const SPN*, const SInstBase*);
            inline const SInstBase* getInst() const;
            const SInst* getCellInst() const;
            const SBlockInst* getBlockInst() const;
            //inline const SCntv* getActual() const;
            inline const SPN* getActual() const;
            inline SPN* getActual();
            inline void setActual(const SPN*);
            //inline void setActual(const SCntv*);
            SPort::Dir getDir() const;

            //virtual void addDriver(SCntv*);
            //virtual void addSink(SCntv*);
            inline void removeActual();
            //virtual void removeDriver(SCntv*);
            //virtual void removeSink(SCntv*);

            virtual void accept(NetlistVisitorBase*);
        protected:    
            virtual ~SPin();
            
        private:
            SPin(const SPin&);
            SPin& operator=(const SPin&);

            void setDriverAndSink();
            void removeDriverAndSink();
            //keeping instance attrib for faster access
            SPN        *actual;
            SInstBase    *inst;
    };

    const SInstBase*
    SPin::getInst() const{
        return inst;
    }    

    const SPN*
    SPin::getActual() const {
        return actual;
    }    

    SPN*
    SPin::getActual() {
        return actual;
    }    
    void
    SPin::setActual(const SPN *cntv) 
    {
        if(actual)
        {
           //first adjust fanin fanouts
           actual->removeDriver(const_cast<SPin*>(this));
           actual->removeSink(const_cast<SPin*>(this));
           //this->removeDriver(actual);
           //this->removeSink(actual);
        }
        actual = const_cast<SPN*>(cntv);
        this->setDriverAndSink();
    }    

    void
    SPin::removeActual() {
        this->setActual(0);
    }    

    typedef ResetVisitCount<SPin> ResetVisitCountPin;
}
#endif
