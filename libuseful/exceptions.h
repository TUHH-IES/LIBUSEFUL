/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2017 - 2026, Heiko Falk.

*/


#ifndef LIBUSEFUL_EXCEPTION_H
#define LIBUSEFUL_EXCEPTION_H

// Include standard headers
#include <stdexcept>
#include <string>


/*!
  @brief Base class exception

  This is the base class for LIBUSEFUL exceptions which can be used by a caller
  to catch all LIBUSEFUL exceptions in a catch() clause.

  It shall not be used directly. Use the derived error classes instead.

*/
class usefulError : public std::runtime_error
{

  public:

    /*!
      Return error message with file and line location, if mFile variable is not
      empty. Otherwise only return error message.
    */
    const char *what() const noexcept;

    /*!
      Print formatted error message using libuseful IO streams.
    */
    void print() const noexcept;


  protected:

    // All constructors are protected to prohibit users from using this class
    // directly.

    /*!
      @brief Constructor.

      @param file String containing the file name where the error occured.
      @param line Line number where the error occured.
      @param what A const char array containing a human readable message of
                  the error.
      @param f A Boolean defaulting to true that indicates whether to use
               ufFatalMsg or ufErrMSg during error message printing.

      The error message can be retreived using the derived what() function.
    */
    usefulError( const std::string &, const unsigned int, const char *,
                 bool = true );

    /*!
      @brief Constructor.

      @param file String containing the file name where the error occured
      @param line Line number where the error occured
      @param what A const string reference containing a human readable message
                  of the error.
      @param f A Boolean defaulting to true that indicates whether to use
               ufFatalMsg or ufErrMSg during error message printing.

      The error message can be retreived using the derived what() function.
    */
    usefulError( const std::string &, const unsigned int, const std::string &,
                 bool = true );

    //! Default destructor. We need this to make g++ happy.
    ~usefulError() noexcept = default;

    //! Const string holding the file name where the error occured.
    const std::string mFile;

    //! Const unsigned int holding the line number where the error occured.
    const unsigned int mLine;


  private:

    /*!
      @brief String for the composed error string.

      This is necessary for what() function. what() returns a C string, and any
      local object within what() would be destroyed on function exit, thus
      rendering the returned char* pointer invalid.
      We also do not want dynamic memory allocation as a caller of what() will
      not know that the returned memory must be freed.

      With this solution, the pointer returned by what() will be valid as long
      as the associated exception object exists.
    */
    mutable std::string mErrorString;

    /*!
      @brief mFatalStream stores whether print() shall use ufFatalMsg or
             ufErrMsg for pretty-printing the error message.
    */
    bool mFatalStream;

};


/*!
  @brief Exception class for non-fatal error handling.

  A ufError should be thrown in case of a non-fatal error condition which cannot
  be handled by the class itself.

  It must - however - be ensured that overall consistency of non-private or
  static objects is still given.

  I.e., someone who catches a ufError must be allowed to safely assume that
  normal operation can be resumed.

  If the observed error may have lead to global inconsistencies or a somehow
  otherwise undefined state of the overall program, throw ufFatalError instead.
*/
class ufError : public usefulError
{

  public:

    /*!
      @brief Default constructor

      @param what A const char array containing a human readable message of
                  the error.
      @param f A Boolean defaulting to false that indicates whether to use
               ufFatalMsg or ufErrMSg during error message printing.

      The error message can be retreived using the derived what() function.
    */
    explicit ufError( const char *, bool = false );

    /*!
      @brief Default constructor

      @param what A const string reference containing a human readable message
                  of the error.
      @param f A Boolean defaulting to false that indicates whether to use
               ufFatalMsg or ufErrMSg during error message printing.

      The error message can be retreived using the derived what() function.
    */
    explicit ufError( const std::string &, bool = false );

    /*!
      @brief Default constructor.

      @param file String containing the file name where the error occured
      @param line Line number where the error occured
      @param msg A const string reference containing a human readable message
                 of the error.
      @param f A Boolean defaulting to false that indicates whether to use
               ufFatalMsg or ufErrMSg during error message printing.

      The error message can be retreived using the derived what() function.
    */
    ufError( const std::string &, const unsigned int, const std::string &,
             bool = false );

    //! Default destructor. We need this to make g++ happy.
    ~ufError() noexcept = default;

};


/*!
  @brief Exception class for fatal error handling.

  A ufFatalError should be thrown in case of a fatal error condition which
  cannot be handled by the class itself.

  In case of a ufFatalError, the program's consistency and future behavior is
  undefined.

  Anyone catching this error must manually check all existing structures for
  consistency if trying to resume normal operation.

  A ufFatalError should therefore be considered a non-recoverable fatal error.
*/
class ufFatalError : public usefulError
{

  public:

    /*!
      @brief Default constructor.

      @param what A const char array containing a human readable message of
                  the error.
      @param f A Boolean defaulting to true that indicates whether to use
               ufFatalMsg or ufErrMSg during error message printing.

      The error message can be retreived using the derived what() function.
    */
    explicit ufFatalError( const char *, bool = true );

    /*!
      @brief Default constructor.

      @param what A const string reference containing a human readable message
                  of the error.
      @param f A Boolean defaulting to true that indicates whether to use
               ufFatalMsg or ufErrMSg during error message printing.

      The error message can be retreived using the derived what() function.
    */
    explicit ufFatalError( const std::string &, bool = true );

    /*!
      @brief Default constructor.

      @param file String containing the file name where the error occured
      @param line Line number where the error occured
      @param msg A const string reference containing a human readable message
                 of the error.
      @param f A Boolean defaulting to true that indicates whether to use
               ufFatalMsg or ufErrMSg during error message printing.

      The error message can be retreived using the derived what() function.
    */
    ufFatalError( const std::string &, const unsigned int,
                  const std::string &, bool = true );

    //! Default destructor. We need this to make g++ happy.
    ~ufFatalError() noexcept = default;

};


#endif
