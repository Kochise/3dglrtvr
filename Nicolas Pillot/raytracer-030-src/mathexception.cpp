
#include "mathexception.h"

MathException::~MathException()
{
}

MathException::MathException(const std::string & msg)
: MyException("MathException",msg)
{
}

