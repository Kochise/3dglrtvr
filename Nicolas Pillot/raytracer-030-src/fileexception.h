#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H

#include "myexception.h"

class FileException : public MyException
{
    public:
        FileException(const std::string & msg);
        virtual ~FileException();
};

#endif
