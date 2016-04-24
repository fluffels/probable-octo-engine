#include <iostream>
#include <string>

#include "MessageException.h"

MessageException::
MessageException() throw() {
}

MessageException::
~MessageException() throw() {
}

const char *MessageException::
what() const throw() {
    return toString().c_str();
}

string MessageException::
toString() const throw() {
    return getPrefix() + ": " + getMessage();
}

