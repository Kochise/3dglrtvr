#ifndef NULLPOINTEREXCEPTION_H
#define NULLPOINTEREXCEPTION_H

#include "myexception.h"

class NullPointerException : public MyException
{
    public:
        NullPointerException(const std::string & msg);
        virtual ~NullPointerException();
};

#endif
