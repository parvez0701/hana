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

#ifndef _SINSTBASE_H_
#define _SINSTBASE_H_

#include <sstream>
#include <assert.h>

#include "spin.h"

namespace stview
{
    class SInstBase: public SBase
    {
        public:
            SInstBase(const std::string& name) : SBase(name) {
            }

            inline const std::list<SPin*>& getInPins() const;
            inline const std::list<SPin*>& getOutPins() const;
            inline const std::list<SPin*>& getInOutPins() const;
            inline std::list<SPin*> getPins() const;

            inline const SPin*    getPin(const std::string&) const;
            inline const SPin*    getPin(const SIxName&) const;
            inline SPin*        getPin(const std::string&);
            inline SPin*        getPin(const SIxName&);
            inline void            addPin(SPin*);
            virtual SPort::Dir    getDir(const std::string&) const = 0;

            void                resetVisitCount();
        protected:    
            virtual ~SInstBase(); 
        private:
            inline const SPin*    getPinInternal(const std::string&) const;
            inline const SPin*    getPinInternal(const SIxName&) const;
            
            
            std::list<SPin*> inpins;
            std::list<SPin*> outpins;
            std::list<SPin*> inoutpins;
    };

    const std::list<SPin*>&
    SInstBase::getInPins() const{
        return inpins;
    }
    const std::list<SPin*>&
    SInstBase::getOutPins() const{
        return outpins;
    }
    const std::list<SPin*>&
    SInstBase::getInOutPins() const{
        return inoutpins;
    }

    std::list<SPin*>
    SInstBase::getPins() const
    {
        std::list<SPin*> spins = inpins;
        std::copy(outpins.begin(), outpins.end(), std::back_inserter(spins));
        std::copy(inoutpins.begin(), inoutpins.end(), std::back_inserter(spins));
        return spins;
    }

    void
    SInstBase::addPin(SPin *pin)
    {
        SPort::Dir dir = pin->getDir();
        switch(dir)
        {
            case SPort::IN: 
            {
                inpins.push_back(pin);
                break;
            }
            case SPort::OUT:
            {
                outpins.push_back(pin);
                break;
            }
            case SPort::INOUT:
            {
                inoutpins.push_back(pin);
                break;
            }
            default: {
                assert(0);
            }    
                
        }
    }    

    const SPin*
    SInstBase::getPinInternal(const std::string& name) const
    {
        for(std::list<SPin*>::const_iterator pin = inpins.begin();
            pin != inpins.end(); ++pin) {
            if((*pin)->getName() == name) {
                return *pin;
            }
        }    
        for(std::list<SPin*>::const_iterator pin = outpins.begin();
            pin != outpins.end(); ++pin) {
            if((*pin)->getName() == name) {
                return *pin;
            }
        }    
        for(std::list<SPin*>::const_iterator pin = inoutpins.begin();
            pin != inoutpins.end(); ++pin) {
            if((*pin)->getName() == name) {
                return *pin;
            }
        }    
        return NULL;
    }    

    const SPin*
    SInstBase::getPinInternal(const SIxName& name) const 
    {
        std::ostringstream str;
        str << name.getName() << "#" << name.getIndex();

        return getPin(str.str());
    }    

    const SPin*
    SInstBase::getPin(const std::string& name) const {
        return getPinInternal(name);
    }    
    
    const SPin*
    SInstBase::getPin(const SIxName& name) const {
        return getPinInternal(name);
    }
    SPin*
    SInstBase::getPin(const std::string& name) {
        return const_cast<SPin*>(getPinInternal(name));
    }
    SPin*
    SInstBase::getPin(const SIxName& name) {
        return const_cast<SPin*>(getPinInternal(name));
    }

}
#endif

