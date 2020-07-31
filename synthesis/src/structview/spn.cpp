#include "spn.h"
#include "spin.h"

namespace stview
{
    SPN::SPN(const std::string& name) : SCntv(name), dbegin(true), sbegin(true)
    {
        diter = drivers.begin();
        siter = sinks.begin();
        
    }

    SPN::~SPN() 
    {
        reset();
        for(Sink s = getSink(); !sinkEnd(); s = getNextSink()) {
            (*s)->setActual(0);
        }    
        reset();
        for(Driver d = getDriver(); !driverEnd(); d = getNextDriver()) {
            (*d)->setActual(0);
        }    
        sbegin = true;
        dbegin = true;
        
    }

    bool SPN::isPort() const {
        return false;
    }

    bool SPN::isNet() const {
        return false;
    }
}
