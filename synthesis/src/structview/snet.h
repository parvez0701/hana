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

#ifndef _SNET_H_
#define _SNET_H_

#include <list>

#include "spn.h"
namespace stview
{
    class SPin;
    //class SNet: public SCntv
    class SNet: public SPN
    {
        friend class SBlock;
        public:
            SNet(const std::string&);

            //inline const std::list<SPin*>& getDrivers() const;
            //inline const std::list<SPin*>& getDrains() const;
            //inline void addDriver(const SPin*);
            //inline void addDrain(const SPin*);

            virtual void accept(NetlistVisitorBase*);
            virtual bool isNet() const;
//            virtual void addDriver(SCntv*);
//            virtual void addSink(SCntv*);
        protected:    
            virtual ~SNet();
        private:
            SNet(const SNet&);
            SNet& operator=(const SNet&);
        
            //std::list<SPin*> drivers;
            //std::list<SPin*> drains;

    };

    typedef ResetVisitCount<SNet> ResetVisitCountNet;
/*
    const std::list<SPin*>&
    SNet::getDrivers() const {
        return drivers;
    }

    const std::list<SPin*>&
    SNet::getDrains() const {
        return drains;
    }
    void
    SNet::addDriver(const SPin* pin){
        if(pin){
            drivers.push_back(const_cast<SPin*>(pin));
        }
    }
    void
    SNet::addDrain(const SPin* pin) {
        if(pin){
            drains.push_back(const_cast<SPin*>(pin));
        }
    }
    */
}
#endif
