
#include "printable.h"

Printable::Printable()
{
}

Printable::~Printable()
{
}

std::ostream & operator << (std::ostream& os, const Printable& printable)
{
    os << printable.toString();
    return os;
}


