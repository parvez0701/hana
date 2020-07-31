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

#ifndef _VECPORT_H_
#define _VECPORT_H_

#include "veccntv.h"
#include "port.h"
namespace Rtl
{
    class VecPort: public VecCntv
    {
        friend class OMCreator;
        friend class Elaborator;
        public:
            VecPort(const std::string&,
                const Dimension*, const Dir);

            virtual ~VecPort(){
            }

            //virtual Port* getCntvBit(const long) const throw(Error);
            const std::vector<Port*>& getPorts() const{
                return ports;
            }
            virtual void accept(OMVisitorBase*) const;
            virtual long len() const {
                return ports.size();
            }
            virtual const Port* getBit(const long) const;
            Dir getDir() const {
                return ports[0]->getDir();
            }
            const std::string& getVarType() const {
                return ports[0]->getVarType();
            }
            virtual VecPort* copy() const;
            virtual bool isSigned() const {
                return false;
            }    
        private:
            void createPortBits(const std::string&,
                const Dimension*, const Dir);
            void setVarType(const std::string&);
            std::vector<Port*> ports;
    };
}
#endif
