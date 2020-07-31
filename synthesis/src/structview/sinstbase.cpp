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

#include "sinstbase.h"


namespace stview
{
    SInstBase::~SInstBase()
    { 
    /*
        std::cout << "INSTANCE " << this->getName() <<std::endl;

        for(std::list<SPin*>::const_iterator pin = inpins.begin();
            pin != inpins.end(); ++pin) {
            if((*pin)->getActual()) {
                std::cout << (*pin)->getActual()->getName() << std::endl;
            }
        }    
        for(std::list<SPin*>::const_iterator pin = outpins.begin();
            pin != outpins.end(); ++pin) {
            if((*pin)->getActual()) {
                std::cout << (*pin)->getActual()->getName() << std::endl;
            }
        }    
        std::cout << "----------------------------------------------\n";
        */
        std::for_each(inpins.begin(), inpins.end(), DELETE<SCntv>());
        std::for_each(outpins.begin(), outpins.end(), DELETE<SCntv>());
        std::for_each(inoutpins.begin(), inoutpins.end(), DELETE<SCntv>());
        inpins.clear();
        outpins.clear();
        inoutpins.clear();
    }

    void
    SInstBase::resetVisitCount() 
    {
        ResetVisitCountPin()(inpins);
        ResetVisitCountPin()(inoutpins);
        ResetVisitCountPin()(outpins);
    }
}
