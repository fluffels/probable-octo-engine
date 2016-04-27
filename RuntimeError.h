#pragma once

#include <string>

#include "DynamicMessageException.h"

using std::string;

/**
  * Encapsulates errors that happen at runtime.
  */
class RuntimeError : public DynamicMessageException {
public:
    /**
      * Default constructor.
      *
      * @message The message to pass onto the superclass.
      */
    RuntimeError(string message) throw();

    /**
      * Do-nothing destructor.
      */
    virtual ~RuntimeError() throw();

    /**
      * Override the message prefix.
      */
    virtual string getPrefix() const throw();
};
