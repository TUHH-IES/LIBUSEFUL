/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2017 - 2026, Heiko Falk.

*/

/*!
  @file io.h
  @brief This file provides the interface of several I/O-related utility
         functions for printing error and warning messages etc.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _IO_H
#define _IO_H


//
// Include section
//

// Include standard headers
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>


//
// Preprocessor macros
//

/*!
  @brief ufAssert checks the specified assertion expression and prints an error
         message and dumps core if the assertion fails.

  @param[in] expr A C expression that must be asserted.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
#define ufAssert( expr ) \
  do { \
    if ( !( expr ) ) \
      __ufAssertT( \
        __STRING( expr ), __FILE__, __LINE__, __PRETTY_FUNCTION__, "" ); \
  } while ( 0 )

/*!
  @brief ufAssertT checks the specified assertion expression and prints an error
         message and dumps core if the assertion fails.

  @param[in] expr A C expression that must be asserted.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
#define ufAssertT( expr, ... ) \
  do { \
    if ( !( expr ) ) { \
      std::stringstream __s; \
      __s << __VA_ARGS__; \
      __ufAssertT( \
        __STRING( expr ), __FILE__, __LINE__, __PRETTY_FUNCTION__, \
        __s.str() ); \
    } \
  } while ( 0 )


//
// Package name handling.
//

/*!
  @brief setMsgPackage sets the package name to be emitted at the very beginning
         of error/warning messages.

  @param[in] p A const reference to a string containing the package name to be
               used.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
void setMsgPackage( const std::string &p = "" );

/*!
  @brief getMsgPackage returns the package name to be emitted at the very
         beginning of error/warning messages.

  @return A const reference to a string containing the currently used package
          name.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const std::string &getMsgPackage( void );


//
// Assertion functions.
//

/*!
  @brief __ufAssertT prints an error message as well as a formatted assertion
         message and dumps core.

  @param[in] assertion A const reference to a string that contains the failed
                       assertion's expression.
  @param[in] file A const reference to a string that contains the source code
                  file name where the assertion failed.
  @param[in] line The line number within the specified file where the assertion
                  failed.
  @param[in] function A const reference to a string that contains the function
                      name where the assertion failed.
  @param[in] text A const reference to a string that contains the detailed error
                  message.

  @note Users should not directly call this function. Instead, macro ufAssertT
        should be used.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
void __ufAssertT( const std::string &assertion, const std::string &file,
                  unsigned int line, const std::string &function,
                  const std::string &text = "" ) noexcept
  __attribute__((noreturn));


//
// Useful streams.
//

/*!
  @brief struct smanip provides support for a stream manipulator in order to
         specify filenames and line numbers for warning/error messages.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
struct smanip
{
  /*!
    @brief f is a function pointer that is used to actually set a useful
           stream's file/line information.
  */
  std::ios_base & (*f) ( std::ios_base &, const std::string &, unsigned int );

  //! file holds the file name associated with an error message.
  std::string file;

  //! line holds the line number associated with an error message.
  unsigned int line;

  /*!
    @brief This constructor simply initializes the struct's items.

    @param[in] ff A function pointer to a setter for a useful stream's file/line
                  information.
    @param[in] fn A const reference to a string holding the file name.
    @param[in] l An unsigned integer denoting the line number.

    @author Heiko Falk <Heiko.Falk@tuhh.de>
  */
  smanip( std::ios_base & (*ff)( std::ios_base &, const std::string &,
                                 unsigned int ),
          const std::string &fn,
          unsigned int l ) :
    f( ff ),
    file( fn ),
    line( l )
  {
  };

};


/*!
  @brief ufFile is a manipulator for useful streams in order to specify file
         name and line number associated with a message.

  @param[in] file A const pointer to a C string holding the file name.
  @param[in] line An unsigned integer denoting the line number.

  ufFile resets a stream such that it assumes that a completely new message is
  beginning. If you use a useful stream without invoking this stream
  manipulator, the currently used message stream is continued. Actually, use
  ufFile whenever you want a fresh header string like, e.g., "Info:" or
  "Warning:" to appear.

  If no file name is specified, no file/line information is displayed at all. If
  no line number is specified, only the file name is written.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
smanip ufFile( const char *file = "", unsigned int line = 0 );


/*!
  @brief ufFile is a manipulator for useful streams in order to specify file
         name and line number associated with a message.

  @param[in] file A const reference to a string holding the file name.
  @param[in] line An unsigned integer denoting the line number.

  ufFile resets a stream such that it assumes that a completely new message is
  beginning. If you use a useful stream without invoking this stream
  manipulator, the currently used message stream is continued. Actually, use
  ufFile whenever you want a fresh header string like, e.g., "Info:" or
  "Warning:" to appear.

  If no file name is specified, no file/line information is displayed at all. If
  no line number is specified, only the file name is written.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
smanip ufFile( const std::string &file, unsigned int line = 0 );


/*!
  @brief Class ufStream provides common operators on "useful streams", i.e.,
         specialized output streams for formatted warning/error/info messages.

  Since the actual formats of warning/error/info messages differs, ufStream is
  a purely virtual base class from which specialized classes with actual
  formatting code inherit.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ufStream : public std::ostream
{

  public:

    //
    // Stream I/O.
    //

    ufStream & operator << ( bool );

    ufStream & operator << ( char );

    ufStream & operator << ( signed char );

    ufStream & operator << ( unsigned char );

    ufStream & operator << ( short );

    ufStream & operator << ( unsigned short );

    ufStream & operator << ( int );

    ufStream & operator << ( unsigned int );

    ufStream & operator << ( long );

    ufStream & operator << ( unsigned long );

    ufStream & operator << ( long long );

    ufStream & operator << ( unsigned long long );

    ufStream & operator << ( float );

    ufStream & operator << ( double );

    ufStream & operator << ( long double );

    ufStream & operator << ( const char * );

    ufStream & operator << ( const signed char * );

    ufStream & operator << ( const unsigned char * );

    ufStream & operator << ( void * );

    ufStream & operator << ( std::streambuf * );

    ufStream & operator << ( std::ostream & (*pf)( std::ostream & ) );

    ufStream & operator << ( std::ios & (*pf)( std::ios & ) );

    ufStream & operator << ( std::ios_base & (*pf)( std::ios_base & ) );

    ufStream & operator << ( const std::string & );

    ufStream & operator << ( const smanip & );


  protected:

    /*!
      @brief Default constructor creating a useful stream that is attached to
             the given standard output stream.

      @param[in] s A reference to a standard output stream (i.e., either cout
                   or cerr) to which error/warning messages will actually be
                   written.
      @param[in] useStdout A Boolean flag specifying whether s refers to cout or
                           to cerr, resp.

      This constructor also takes care of the overall initialization of the
      libuseful I/O mechanisms.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ufStream( std::ostream &, bool );

    /*!
      @brief printHeader is used to print a header string like, e.g., "Info:",
             "Error:" or "Warning:" in a formatted style.

      Since the actual formatting of this header string is defined in derived
      classes, printHeader is purely virtual here.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void printHeader( void ) = 0;

    //! mCursorPos stores the current horizontal position of the cursor.
    unsigned int mCursorPos;

    /*!
      @brief mStream holds a reference to the actual standard output stream to
             which formatted messages are actually written.
    */
    std::ostream &mStream;

    /*!
      @brief mOmitHeader specifies whether the header of a message shall be
             omitted or not.
    */
    bool mOmitHeader;

    /*!
      @brief mPrintLongTime specifies whether a message is printed with long
             date and time information.
    */
    bool mPrintLongTime;

    /*!
      @brief mPrintLongTime specifies whether a message is printed with short
             date and time information.
    */
    bool mPrintShortTime;

    //! mPrintBoldMessage specifies whether the message text is printed in bold.
    bool mPrintBoldMessage;

    //! mPrintBoldTime specifies whether short times are printed in bold.
    bool mPrintBoldTime;


  private:

    friend std::ios_base &setFile( std::ios_base &, const std::string &, unsigned int );

    /*!
      @brief No standard construction allowed, users must use
             ufStream( std::ostream &, bool ) instead.
    */
    ufStream( void ) = delete;

    /*!
      @brief startMsg starts emitting a formatted message by writing the package
             name, the formatted header, and file/line information if specified.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void startMsg( void );

    /*!
      @brief formatMsg formats the message text such that the current terminal
             width is considered and appropriate line breaks and indentations
             are produced.

      @param[in] str A const reference to a string holding the message text to
                     be written.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void formatMsg( const std::string & );

    /*!
      @brief setFile sets a useful stream's associated file name and line number
             information.

      @param[in] f A const reference to a string holding the file name.
      @param[in] l An unsigned integer holding the line number.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void setFile( const std::string &, unsigned int );

    //! mFilename holds a useful stream's associated file name.
    std::string mFilename;

    //! mLine holds a useful stream's associated line number.
    unsigned int mLine;

    /*!
      @brief mReset is used for internal book-keeping, whether a new message
             starts or not.
    */
    bool mReset;

    //! mFirstWord denotes whether a first word in a new line is written or not.
    bool mFirstWord;


    //! mUseStdout specifies whether mStream refers to cout or to cerr, resp.
    bool mUseStdout;

};


/*!
  @brief Class ufBannerStream provides a stream for formatted banner messages.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ufBannerStream : public ufStream
{

  public:

    /*!
      @brief Default constructor associating cout with banner messages.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ufBannerStream( void );


  protected:

    /*!
      @brief printHeader is not used for banner messages.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void printHeader( void ) override;

};


/*!
  @brief Class ufDebugStream provides a stream for formatted debug messages.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ufDebugStream : public ufStream
{

  public:

    /*!
      @brief Default constructor associating cout with debug messages.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ufDebugStream( void );


  protected:

    /*!
      @brief printHeader is used to print the header string "Debug:" in a
             formatted style.

      If terminal capabilities allow, the header is printed in cyan font.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void printHeader( void ) override;

};


/*!
  @brief Class ufErrStream provides a stream for formatted error messages.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ufErrStream : public ufStream
{

  public:

    /*!
      @brief Default constructor associating cerr with error messages.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ufErrStream( void );


  protected:

    /*!
      @brief printHeader is used to print the header string "Error:" in a
             formatted style.

      If terminal capabilities allow, the header is printed in bold red font.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void printHeader( void ) override;

};


/*!
  @brief Class ufFatalStream provides a stream for formatted fatal error
         messages.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ufFatalStream : public ufStream
{

  public:

    /*!
      @brief Default constructor associating cerr with fatal error messages.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ufFatalStream( void );


  protected:

    /*!
      @brief printHeader is used to print the header string "Fatal Error:" in a
             formatted style.

      If terminal capabilities allow, the header is printed in bold white font
      on red background.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void printHeader( void ) override;

};


/*!
  @brief Class ufInfoStream provides a stream for formatted info messages.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ufInfoStream : public ufStream
{

  public:

    /*!
      @brief Default constructor associating cout with info messages.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ufInfoStream( void );


  protected:

    /*!
      @brief printHeader is used to print the header string "Info:" in a
             formatted style.

      If terminal capabilities allow, the header is printed in bold green font.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void printHeader( void ) override;

};


/*!
  @brief Class ufNoteStream provides a stream for formatted note messages.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ufNoteStream : public ufStream
{

  public:

    /*!
      @brief Default constructor associating cout with note messages.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ufNoteStream( void );


  protected:

    /*!
      @brief printHeader is used to print the header string "Note:" in a
             formatted style.

      If terminal capabilities allow, the header is printed in bold green font.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void printHeader( void ) override;

};


/*!
  @brief Class ufProgrStream provides a stream for formatted progress messages.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ufProgrStream : public ufStream
{

  public:

    /*!
      @brief Default constructor associating cout with progress messages.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ufProgrStream( void );


  protected:

    /*!
      @brief printHeader is used to print the header string "Progress:" in a
             formatted style.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void printHeader( void ) override;

};


/*!
  @brief Class ufWarnStream provides a stream for formatted warning messages.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ufWarnStream : public ufStream
{

  public:

    /*!
      @brief Default constructor associating cerr with warning messages.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ufWarnStream( void );


  protected:

    /*!
      @brief printHeader is used to print the header string "Warning:" in a
             formatted style.

      If terminal capabilities allow, the header is printed in bold yellow font.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void printHeader( void ) override;

};


//! ufBannerMsg is a global useful stream for banner messages.
extern ufBannerStream ufBannerMsg;


//! ufDebugMsg is a global useful stream for debug messages.
extern ufDebugStream ufDebugMsg;


//! ufErrMsg is a global useful stream for error messages.
extern ufErrStream ufErrMsg;


//! ufFatalMsg is a global useful stream for fatal error messages.
extern ufFatalStream ufFatalMsg;


//! ufInfoMsg is a global useful stream for info messages.
extern ufInfoStream ufInfoMsg;


//! ufNoteMsg is a global useful stream for note messages.
extern ufNoteStream ufNoteMsg;


//! ufProgrMsg is a global useful stream for progress messages.
extern ufProgrStream ufProgrMsg;


//! ufWarnMsg is a global useful stream for warning messages.
extern ufWarnStream ufWarnMsg;


//
// Functions for escape sequence handling.
//

/*!
  @brief smso_stdout returns a C-string containing the escape sequence for
         standard-mode output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *smso_stdout( void );

/*!
  @brief bold_stdout returns a C-string containing the escape sequence for
         bold output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *bold_stdout( void );

/*!
  @brief reset_stdout returns a C-string containing the escape sequence for
         resetting all output formats on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *reset_stdout( void );

/*!
  @brief blackfg_stdout returns a C-string containing the escape sequence for
         black foreground output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *blackfg_stdout( void );

/*!
  @brief redfg_stdout returns a C-string containing the escape sequence for
         red foreground output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *redfg_stdout( void );

/*!
  @brief greenfg_stdout returns a C-string containing the escape sequence for
         green foreground output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *greenfg_stdout( void );

/*!
  @brief yellowfg_stdout returns a C-string containing the escape sequence for
         yellow foreground output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *yellowfg_stdout( void );

/*!
  @brief bluefg_stdout returns a C-string containing the escape sequence for
         blue foreground output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *bluefg_stdout( void );

/*!
  @brief cyanfg_stdout returns a C-string containing the escape sequence for
         cyan foreground output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *cyanfg_stdout( void );

/*!
  @brief magentafg_stdout returns a C-string containing the escape sequence for
         magenta foreground output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *magentafg_stdout( void );

/*!
  @brief whitefg_stdout returns a C-string containing the escape sequence for
         white foreground output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *whitefg_stdout( void );

/*!
  @brief tuhhfg_stdout returns a C-string containing the escape sequence for
         tuhh cyan foreground output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Arno Luppold <Arno.Luppold@tuhh.de>
*/
const char *tuhhfg_stdout( void );


/*!
  @brief blackbg_stdout returns a C-string containing the escape sequence for
         black background output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *blackbg_stdout( void );

/*!
  @brief redbg_stdout returns a C-string containing the escape sequence for
         red background output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *redbg_stdout( void );

/*!
  @brief greenbg_stdout returns a C-string containing the escape sequence for
         green background output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *greenbg_stdout( void );

/*!
  @brief yellowbg_stdout returns a C-string containing the escape sequence for
         yellow background output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *yellowbg_stdout( void );

/*!
  @brief bluebg_stdout returns a C-string containing the escape sequence for
         blue background output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *bluebg_stdout( void );

/*!
  @brief cyanbg_stdout returns a C-string containing the escape sequence for
         cyan background output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *cyanbg_stdout( void );

/*!
  @brief magentabg_stdout returns a C-string containing the escape sequence for
         magenta background output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *magentabg_stdout( void );

/*!
  @brief whitebg_stdout returns a C-string containing the escape sequence for
         white background output on stdout.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *whitebg_stdout( void );

/*!
  @brief smso_stderr returns a C-string containing the escape sequence for
         standard-mode output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *smso_stderr( void );

/*!
  @brief bold_stderr returns a C-string containing the escape sequence for
         bold output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *bold_stderr( void );

/*!
  @brief reset_stderr returns a C-string containing the escape sequence for
         resetting all output formats on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *reset_stderr( void );

/*!
  @brief blackfg_stderr returns a C-string containing the escape sequence for
         black foreground output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *blackfg_stderr( void );

/*!
  @brief redfg_stderr returns a C-string containing the escape sequence for
         red foreground output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *redfg_stderr( void );

/*!
  @brief greenfg_stderr returns a C-string containing the escape sequence for
         green foreground output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *greenfg_stderr( void );

/*!
  @brief yellowfg_stderr returns a C-string containing the escape sequence for
         yellow foreground output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *yellowfg_stderr( void );

/*!
  @brief bluefg_stderr returns a C-string containing the escape sequence for
         blue foreground output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *bluefg_stderr( void );

/*!
  @brief cyanfg_stderr returns a C-string containing the escape sequence for
         cyan foreground output on stder.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *cyanfg_stderr( void );

/*!
  @brief magentafg_stderr returns a C-string containing the escape sequence for
         magenta foreground output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *magentafg_stderr( void );

/*!
  @brief whitefg_stderr returns a C-string containing the escape sequence for
         white foreground output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *whitefg_stderr( void );

/*!
  @brief tuhhfg_stderr returns a C-string containing the escape sequence for
         tuhh cyan foreground output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Arno Luppold <Arno.Luppold@tuhh.de>
*/
const char *tuhhfg_stderr( void );

/*!
  @brief blackbg_sterr returns a C-string containing the escape sequence for
         black background output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *blackbg_stderr( void );

/*!
  @brief redbg_stderr returns a C-string containing the escape sequence for
         red background output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *redbg_stderr( void );

/*!
  @brief greenbg_stderr returns a C-string containing the escape sequence for
         green background output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *greenbg_stderr( void );

/*!
  @brief yellowbg_stderr returns a C-string containing the escape sequence for
         yellow background output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *yellowbg_stderr( void );

/*!
  @brief bluebg_stderr returns a C-string containing the escape sequence for
         blue background output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *bluebg_stderr( void );

/*!
  @brief cyanbg_stderr returns a C-string containing the escape sequence for
         cyan background output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *cyanbg_stderr( void );

/*!
  @brief magentabg_stderr returns a C-string containing the escape sequence for
         magenta background output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *magentabg_stderr( void );

/*!
  @brief whitebg_stderr returns a C-string containing the escape sequence for
         white background output on stderr.

  @return A const char pointer to the associated escape sequence.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
const char *whitebg_stderr( void );

#endif  // _IO_H
