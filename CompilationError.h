#ifndef COMPILATION_ERROR_H
#define COMPILATION_ERROR_H

#include "DynamicMessageException.h"

/**
  * Encapsulates errors from the shader compilation process.
  */
class CompilationError: public DynamicMessageException
{
   public:
      /**
        * Pass the message on to the superclass.
        */
      CompilationError(const string& message) throw();

      /**
        * Do-nothing destructor.
        */
      virtual ~CompilationError() throw();

      /**
        * Override the message prefix.
        */
      virtual string getPrefix() const throw();
};

#endif

