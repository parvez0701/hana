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

#ifndef _SPORT_H_
#define _SPORT_H_

#include "spn.h"
namespace stview
{
    class SPort: public SPN
    {
        public:
            typedef enum {UNSET, IN, OUT, INOUT} Dir;

            SPort(const std::string&, Dir);
            inline Dir getDir() const;

            //virtual void addDriver(SCntv*);
            //virtual void addSink(SCntv*);

            virtual void accept(NetlistVisitorBase*);
            virtual bool isPort() const;
        protected:    
            virtual ~SPort();
            
        private:
            SPort(const SPort&);
            SPort& operator=(const SPort&);
            
            //data
            Dir dir;
    };

    SPort::Dir
    SPort::getDir() const{
        return dir;
    }

    typedef ResetVisitCount<SPort> ResetVisitCountPort;

}
#endif
