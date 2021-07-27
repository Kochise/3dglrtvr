
#include "syntaxexception.h"

SyntaxException::~SyntaxException()
{
}

SyntaxException::SyntaxException(const std::string & msg)
: MyException("SyntaxException",msg)
{
}

