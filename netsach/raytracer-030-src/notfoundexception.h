#ifndef NOTFOUNDEXCEPTION_H
#define NOTFOUNDEXCEPTION_H

#include "myexception.h"

class NotFoundException : public MyException
{
    public:
        NotFoundException(const std::string & msg);
        virtual ~NotFoundException();
};

#endif
