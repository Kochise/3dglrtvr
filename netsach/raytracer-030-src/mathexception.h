#ifndef MATHEXCEPTION_H
#define MATHEXCEPTION_H

#include "myexception.h"

class MathException : public MyException
{
    public:
        MathException(const std::string & msg);
        virtual ~MathException();
};

#endif
