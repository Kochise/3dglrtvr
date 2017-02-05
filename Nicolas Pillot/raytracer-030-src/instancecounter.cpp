
#include "instancecounter.h"

signed long InstanceCounter::instancecounter = 0;

InstanceCounter::InstanceCounter()
{
    instancecounter++;
}

InstanceCounter::~InstanceCounter()
{
    instancecounter--;
}
        
signed long InstanceCounter::getInstanceNumber()
{
    return instancecounter;
}

