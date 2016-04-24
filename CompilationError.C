#include "CompilationError.h"

CompilationError::
CompilationError(const string &error) throw():
        DynamicMessageException(error) {
}

CompilationError::
~CompilationError() throw() {
}

string CompilationError::
getPrefix() const throw() {
    return "Compilation error";
}
