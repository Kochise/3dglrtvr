#ifndef SYNTAXEXCEPTION_H
#define SYNTAXEXCEPTION_H

#include "myexception.h"

class SyntaxException : public MyException
{
    public:
        SyntaxException(const std::string & msg);
        virtual ~SyntaxException();
};

#endif
