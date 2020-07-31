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


#ifndef _SPN_H_
#define _SPN_H_
#include <set>
#include <assert.h>

#include "scntv.h"
namespace stview
{
    class SPin;

    typedef std::set<SPin*> Drivers;
    typedef std::set<SPin*> Sinks;
    typedef Drivers::iterator Driver;
    typedef Sinks::iterator Sink;
    //base class for ports and nets
    class SPN : public SCntv
    {
        //Not thread safe
        public:
            SPN(const std::string&);
            inline Drivers& getDrivers();
            inline Sinks& getSinks();

            inline void addDriver(SPin*);
            inline void addSink(SPin*);

            inline void removeDriver(SPin*);
            inline void removeSink(SPin*);
            inline Driver getDriver();
            inline Sink getSink();
            inline Driver getNextDriver();
            inline Sink getNextSink();
            inline void reset();
            inline bool driverEnd();
            inline bool sinkEnd();

            inline int dsize() const;
            inline int ssize() const;
            inline bool dempty() const;
            inline bool sempty() const;
            virtual bool isPort() const;
            virtual bool isNet() const;

            inline void lock();
            inline void unlock();
        protected:
            virtual ~SPN();
        private:
            SPN(const SPN&);
            SPN& operator=(const SPN&);

            bool dbegin;
            bool sbegin;

            Drivers drivers;
            Sinks sinks;
            //Driver and sink iters are always supposed to be in
            //consistent state.
            Driver diter;
            Sink siter;
            //for locking and unlocking
            Driver lditer;
            Sink lsiter;
    };

    //    Driver and Sink set access:
    //    If there use of drivers or sinks in nested situation then
    //    higher loop should lock the SPN object. Before getting the
    //    next Driver/Sink, it should unlock itself. This ensures that
    //    the iterators are always in consistent state for upper level
    //    loop. If a lower level loop deletes an object, the upper level
    //    list is automatically restarted. In this case unlocking will not
    //    have any effect. The restart is done to avoid invalid iterator.
    
    Drivers& SPN::getDrivers() {
        return drivers;
    }    

    Sinks& SPN::getSinks() {
        return sinks;
    }
    void SPN::addDriver(SPin *d) 
    {
        assert(d);
        drivers.insert(d);
        //dbegin = true;
        //diter = drivers.begin();
    }    

    void SPN::addSink(SPin *s) 
    {
        assert(s);
        sinks.insert(s);
        //sbegin = true;
        //siter = sinks.begin();
    }

    void SPN::removeDriver(SPin *d) 
    {
        if(!d) {
            return;
        }    
        drivers.erase(d);
        dbegin = true;
        //locking violated
        lditer = drivers.begin();
        //diter = drivers.begin();
    }

    void SPN::removeSink(SPin *s)
    {
        if(!s) {
            return;
        }    
        sinks.erase(s);
        sbegin = true;
        lsiter = sinks.begin();
        //siter = sinks.begin();
    }    

    Driver SPN::getDriver() 
    {
        if(dbegin) 
        {
            dbegin = false;
            diter = drivers.begin();
        }    
        return diter;
    /*
        Driver titer = diter;
        if(diter != drivers.end()){
            ++diter;
        }    
        return titer;
    */    
    }

    Sink SPN::getSink() 
    {
        if(sbegin) 
        {
            sbegin = false;
            siter = sinks.begin();
        }    
        return siter;
    /*
        Sink titer = siter;
        if(siter != sinks.end()){
            ++siter;
        }    
        return titer;
    */    
    }

    Driver SPN::getNextDriver() 
    {
        if(diter == drivers.end()) {
            return diter;
        }    
        if(dbegin) 
        {
            diter = drivers.begin();
            dbegin = false;
            return diter;
        }    
        return ++diter;
    }

    Sink SPN::getNextSink() 
    {
        if(siter == sinks.end()) {
            return siter;
        }
        if(sbegin) 
        {
            siter = sinks.begin();
            sbegin = false;
            return siter;
        }    
        return ++siter;
    }
    void SPN::reset()
    {
        //dbegin = true;
        //sbegin = true;
        diter = drivers.begin();
        siter = sinks.begin();
    }

    bool SPN::driverEnd()
    {
        if(diter == drivers.end())
        {
            dbegin = true;
            //diter = drivers.begin();
            return true;
        }
        return false;
    }    
    bool SPN::sinkEnd()
    {
        if(siter == sinks.end())
        {
            sbegin = true;
            //siter = sinks.begin();
            return true;
        }
        return false;
    }    

    int SPN::dsize() const {
        return drivers.size();
    }

    int SPN::ssize() const {
        return sinks.size();
    }    

    bool SPN::dempty() const {
        return drivers.empty();
    }

    bool SPN::sempty() const {
        return sinks.empty();
    }    

    void SPN::lock() 
    {
        lditer = diter;
        lsiter = siter;
    }
    void SPN::unlock()
    {
        diter = lditer;
        siter = lsiter;
    }

    typedef ResetVisitCount<SPN> ResetVisitCountSPN;
        
}
#endif
