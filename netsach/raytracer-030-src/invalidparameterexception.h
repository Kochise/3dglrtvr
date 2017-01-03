#ifndef INVALIDPARAMETEREXCEPTION_H
#define INVALIDPARAMETEREXCEPTION_H

#include "myexception.h"

class InvalidParameterException : public MyException
{
    public:
        InvalidParameterException(const std::string & msg);
        virtual ~InvalidParameterException();
};

#endif
