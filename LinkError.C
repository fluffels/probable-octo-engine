#include "LinkError.h"

LinkError::
LinkError(string message) throw():
   DynamicMessageException(message)
{
}

LinkError::
~LinkError() throw()
{
}

string LinkError::
getPrefix() const throw()
{
   return "Link error";
}

