
#include "myexception.h"

MyException::~MyException()
{
}

MyException::MyException(const std::string & name, const std::string & msg)
: message(name + " { " + msg + " } ")
{
}

const char * MyException::what() const
{
    return message.c_str();
}

std::string MyException::toString() const
{
    return message;
}

