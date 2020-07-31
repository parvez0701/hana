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

#ifndef _VECNET_H_
#define _VECNET_H_
#include <vector>

#include "veccntv.h"
#include "net.h"
//#include "utils.h"

namespace Rtl
{
    class VecNet: public VecCntv
    {
        friend class Elaborator;
        public:
            VecNet(const std::string&,
                const Dimension*);

            virtual ~VecNet(){
            }

            virtual const Net* getBit(const long) const;
            const std::vector<Net*>& getNets() const{
                return nets;
            }
            virtual long len() const {
                return nets.size();
            }
            virtual void accept(OMVisitorBase*) const;
            virtual VecNet* copy() const;
            virtual bool isSigned() const {
                return false;
            }    
        private:
            void createNetBits(const std::string&,
                const Dimension*);
            std::vector<Net*> nets;
    };
}
#endif
