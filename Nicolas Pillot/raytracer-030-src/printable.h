#ifndef PRINTABLE_H
#define PRINTABLE_H

#include "instancecounter.h"

class Printable : public InstanceCounter
{
    public:
        Printable();
        virtual ~Printable();
        virtual std::string toString() const = 0;
};

std::ostream & operator << (std::ostream& os, const Printable& printable);

#endif
