
#include "invalidparameterexception.h"

InvalidParameterException::~InvalidParameterException()
{
}

InvalidParameterException::InvalidParameterException(const std::string & msg)
: MyException("InvalidParameterException",msg)
{
}

