#ifndef INSTANCECOUNTER_H
#define INSTANCECOUNTER_H

#include <iostream>
#include "utils.h"

class InstanceCounter
{
    private:
        static signed long instancecounter;

    public:
        InstanceCounter();
        virtual ~InstanceCounter();
        
        static signed long getInstanceNumber();
};

#endif
