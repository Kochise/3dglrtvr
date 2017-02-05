
#include "nullpointerexception.h"

NullPointerException::~NullPointerException()
{
}

NullPointerException::NullPointerException(const std::string & msg)
: MyException("NullPointerException",msg)
{
}

