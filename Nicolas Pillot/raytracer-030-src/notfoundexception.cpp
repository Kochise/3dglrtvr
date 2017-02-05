
#include "notfoundexception.h"

NotFoundException::~NotFoundException()
{
}

NotFoundException::NotFoundException(const std::string & msg)
: MyException("NotFoundException",msg)
{
}

