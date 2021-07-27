
#include "fileexception.h"

FileException::~FileException()
{
}

FileException::FileException(const std::string & msg)
: MyException("FileException",msg)
{
}

