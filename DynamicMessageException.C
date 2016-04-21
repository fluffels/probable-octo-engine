#include "DynamicMessageException.h"

DynamicMessageException::
DynamicMessageException(const string& message) throw():
   _message(message)
{
}

DynamicMessageException::
~DynamicMessageException() throw()
{
}

string DynamicMessageException::
getMessage() const throw()
{
   return _message;
}

