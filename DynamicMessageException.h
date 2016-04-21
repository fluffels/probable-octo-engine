#ifndef DYNAMIC_MESSAGE_EXCEPTION_H
#define DYNAMIC_MESSAGE_EXCEPTION_H

#include <string>

#include "MessageException.h"

using std::string;

/**
  * Encapsulates a class which's message is set in the constructor.
  */
class DynamicMessageException: public MessageException
{
   public:
      /**
        * Default constructor.
        *
        * @param error The error message obtained from the compiler.
        */
      DynamicMessageException(const string& message) throw();

      /**
        * Do-nothing destructor.
        */
      virtual ~DynamicMessageException() throw();

      /**
        * Simply return the message set in the constructor.
        */
      virtual string getMessage() const throw();

   private:
      string _message;
};

#endif

