#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include "printable.h"

class MyException : public Printable
{
    private:
        std::string message;
    
    public:
        MyException(const std::string& name, const std::string & msg);
        virtual ~MyException();
        virtual const char * what() const;
        virtual std::string toString() const;
};

#endif
