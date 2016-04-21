#ifndef MESSAGE_EXCEPTION_H
#define MESSAGE_EXCEPTION_H

#include <exception>
#include <string>

using std::string;

/**
  * This class encapsulatesthe type of exception that only has a message to 
  * print.
  */
class MessageException: public std::exception
{
   public:
      /**
        * Constructor.
        */
      MessageException() throw();

      /**
        * Do-nothing destructor.
        */
      virtual ~MessageException() throw();

      /**
        * Overriding the what() method.
        *
        * This member function will return the results of the child's 
        * getPrefix() method, followed by its getMessage() method, separated by
        * a colon.
        */
      virtual const char* what() const throw();

      /**
        * Pure virtual method that allows the child to set the message prefix.
        *
        * @return A string containing the prefix.
        */
      virtual string getPrefix() const throw() = 0;

      /**
        * Pure virtual method that allows the child to set the message itself.
        *
        * @return A string containing the message.
        */
      virtual string getMessage() const throw() = 0;

      /**
        * Converts the whole exception to a nicely formatted string.
        *
        * @return The string.
        */
      string toString() const throw();
};

#endif

