#ifndef LINK_ERROR_H
#define LINK_ERROR_H

#include "DynamicMessageException.h"

/**
  * Encapsulates errors from the shader link process.
  */
class LinkError: public DynamicMessageException
{
   public:
      /**
        * Pass the message on to the superclass.
        */
      LinkError(string message) throw();

      /**
        * Do-nothing destructor.
        */
      virtual ~LinkError() throw();

      /**
        * Override the message prefix.
        */
      virtual string getPrefix() const throw();
};

#endif

