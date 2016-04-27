#include "RuntimeError.h"

RuntimeError::
RuntimeError(string message) throw():
        DynamicMessageException(message) {
}

RuntimeError::
~RuntimeError() throw() {
}

string RuntimeError::
getPrefix() const throw() {
    return "Runtime error";
}

