/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2007 - 2026, Heiko Falk.

*/

/*!
  @file debugmacros.h
  @brief This file provides the interface of all useful C++ debug macros.
*/


#ifndef _DEBUGMACROS_H
#define _DEBUGMACROS_H


//
// Include section
//

// Include standard headers
#include <iostream>
#include <string>


//
// Preprocessor macros
//

// If debug macros are not desired, they can be completely disabled by not
// setting the DEBUGMACRO preprocessor macro.
#ifdef DEBUGMACROS

  /*!
    @brief Preprocessor macro DINIT is used to read a debug macro configuration
           file and to set up the associated output streams and internal data
           structures.

    Configuration files have to adhere to the following syntactical and
    semantical rules:
    -# Empty lines and comments (starting with '<tt>#</tt>' or '<tt>//</tt>'
       going until the end of the line) are ignored.
    -# A line can contain the case-insensitive command
       '<tt>showEnterExit = { on | off }</tt>' <br />
       Using this command, the output of messages when entering or leaving a
       function to be debugged can be turned on or off (default = <tt>on</tt>).
    -# A line can contain the case-insensitive command
       '<tt>ignoreFunctionName = { on | off }</tt>' <br />
       Using this command, it can be turned on or off whether function names
       shall be ignored so that debug output is always produced (default =
       <tt>off</tt>).
    -# In all other cases, the content of a configuration file's line (without
       leading and trailing white spaces) denotes case-sensitive activation
       names of functions for which debug output shall be produced.

    @param[in] CONFIGFILE A const reference to a string holding the name of the
                          configuration file to be read.
    @param[in] LOGFILE A const reference to a string holding the name of the
                       output stream to which debug output will be sent.
 */
  #define DINIT( CONFIGFILE, LOGFILE ) USEFUL::dInitFile( CONFIGFILE, LOGFILE )

  /*!
    @brief Preprcessor macro DSTART starts debugging for the currently active
           C++ scope.

    @param[in] FUNCTIONNAME A const reference to a string holding the name of
                            the function to be debugged.

    DSTART uses the specified function name as activation name, i.e., whenever
    DSTART is executed and the given function name occurs in some debug macro
    configuration file (cf. preprocessor macro DINIT), all debug code following
    DSTART in the current C++ scope will be activated and produces debug output.

    Furthermore, DSTART uses the specified function name in the debug output in
    order to properly display which function produces debug output.

    DSTART should thus be called at the beginning of every function which
    produces debug output, and DSTART in the end calls function dStart() below.
    All executed occurrences of dStart() are internally organized on a stack, so
    that every invocation of dStart() must be accompanied by some corresponding
    call of dEnd().

    In C++ code, the "Autodebug" feature makes dStart() and dEnd() calls
    triggered via local struct objects in RAII-style. This is the only way to
    make the debug macros C++ exception-safe, since it will automatically call
    the desired dEnd()s when an exception is thrown.
  */
  #define DSTART( FUNCTIONNAME ) \
    volatile const USEFUL::AutoEnd __autodebug( FUNCTIONNAME )

  /*!
    @brief Preprcessor macro DSTART_EXT starts debugging for the currently
           active C++ scope.

    @param[in] FUNCTIONNAME A const reference to a string holding the name of
                            the function to be debugged.
    @param[in] ACTIVATIONNAME A const reference to a string holding the
                              activation name of the function to be debugged.

    Whenever DSTART_EXT is executed and the given activation name occurs in some
    debug macro configuration file (cf. preprocessor macro DINIT), all debug
    code following DSTART_EXT in the current C++ scope will be activated and
    produces debug output.

    Furthermore, DSTART_EXT uses the specified function name in the debug output
    in order to properly display which function produces debug output.

    DSTART_EXT should thus be called at the beginning of every function which
    produces debug output, and DSTART_EXT in the end calls function dStart()
    below. All executed occurrences of dStart() are internally organized on a
    stack, so that every invocation of dStart() must be accompanied by some
    corresponding call of dEnd().

    In C++ code, the "Autodebug" feature makes dStart() and dEnd() calls
    triggered via local struct objects in RAII-style. This is the only way to
    make the debug macros C++ exception-safe, since it will automatically call
    the desired dEnd()s when an exception is thrown.
  */
  #define DSTART_EXT( FUNCTIONNAME, ACTIVATIONNAME ) \
    volatile const USEFUL::AutoEnd __autodebug( FUNCTIONNAME, ACTIVATIONNAME )

  /*!
    @brief Preprocessor macro DOUT is used to produce debug output for the
           currently active C++ scope.

    Whenever DOUT is executed and the activation name lying on top of the
    current stack refers to a function to be debugged, the arguments passed to
    DOUT are sent to the output stream associated with the current top element
    of the stack. Otherwise, DOUT produces no output.
  */
  #define DOUT( ... ) \
    do { \
      if ( USEFUL::dTest( 1 ) ) \
        USEFUL::dOut() << __VA_ARGS__; \
    } while ( 0 )

  /*!
    @brief Preprocessor macro DOUTL is used to produce debug output for the
           currently active C++ scope, depending on a user-provided debug level.

    @param[in] ARGS A C++ iostream compliant expression producing output that is
                    forwarded to the output stream associated with the currently
                    active function.
    @param[in] DEBUGLEVEL A signed integer that specifies a user-provided debug
                          level.

    DOUT generally produces debug output if the activation name lying on top of
    the current stack refers to a function to be debugged according to the debug
    macro configuration files processed by DINIT. If such a configuration file
    contains an 'ignorefunctionname = on' command and the user-provided debug
    level is 0, then debug output is always produced, irrespective of whether
    the top activation name on the stack refers to a function to be debugged or
    not. For negative debug levels provided by the user, however, no debug
    output is produced.
  */
  #define DOUTL( ARGS, DEBUGLEVEL ) \
    do { \
      if ( USEFUL::dTest( DEBUGLEVEL ) ) \
        USEFUL::dOut() << ARGS; \
    } while ( 0 )

  /*!
    @brief Preprocessor macro DCHECK is used to print name and decimal value of
           a variable.

    @param[in] VARIABLE A variable to be debugged.

    Whenever DCHECK is executed and the activation name lying on top of the
    current stack refers to a function to be debugged, the variable passed to
    DCHECK is sent to the output stream associated with the current top element
    of the stack. Otherwise, DCHECK produces no output.
  */
  #define DCHECK( VARIABLE ) \
    do { \
      if ( USEFUL::dTest( 1 ) ) \
        USEFUL::dOut() << "[Check] " << # VARIABLE << ": " << std::dec \
                       << VARIABLE << std::endl; \
    } while ( 0 )

  /*!
    @brief Preprocessor macro DCHECKX is used to print name and hexadecimal
           value of a variable.

    @param[in] VARIABLE A variable to be debugged.

    Whenever DCHECKX is executed and the activation name lying on top of the
    current stack refers to a function to be debugged, the variable passed to
    DCHECKX is sent to the output stream associated with the current top element
    of the stack. Otherwise, DCHECKX produces no output.
  */
  #define DCHECKX( VARIABLE ) \
    do { \
      if ( USEFUL::dTest( 1 ) ) \
        USEFUL::dOut() << "[Check] " << # VARIABLE << ": " << std::hex \
                       << VARIABLE << std::endl; \
    } while ( 0 )

  /*!
    @brief Preprocessor macro DDECLARE can be used to place any arbitrary
           declarations of local variables required for debugging in application
           code.

    @param[in] DECLARATION A C++ declaration.

    DDECLARE does not perform any active debugging actions, the given
    declaration is simply kept in the source code using this preprocessor macro.
    If however, the debug macros are disabled (ifndef DEBUGMACROS), then the
    given declaration is omitted so that debugging code and helper variables are
    entirely eliminated when no debug macros are used.

    DDECLARE and its declarations are useful for active debugging code coming
    with preprocessor macros DACTION and friends.
  */
  #define DDECLARE( DECLARATION ) DECLARATION

  /*!
    @brief Preprocessor macro DACTION is used to execute any arbitrary debugging
           code for the currently active C++ scope.

    @param[in] ACTION Any kind of C++ code used for debugging.

    Whenever DACTION is executed and the activation name lying on top of the
    current stack refers to a function to be debugged, the given debug code is
    executed. Otherwise, DACTION does not execute the specified debug code.
  */
  #define DACTION( ACTION ) \
    do { \
      if ( USEFUL::dTest( 1 ) ) { \
        ACTION; \
      } \
    } while ( 0 )

  /*!
    @brief Preprocessor macro DACTIONL is used to execute any arbitrary
           debugging code for the currently active C++ scope, depending on a
           user-provided debug level.

    @param[in] ACTION Any kind of C++ code used for debugging.
    @param[in] DEBUGLEVEL A signed integer that specifies a user-provided debug
                          level.

    DACTIONL generally executes the given debug code if the activation name
    lying on top of the current stack refers to a function to be debugged
    according to the debug macro configuration files processed by DINIT. If such
    a configuration file contains an 'ignorefunctionname = on' command and the
    user-provided debug level is 0, then the debug code is always executed,
    irrespective of whether the top activation name on the stack refers to a
    function to be debugged or not. For negative debug levels provided by the
    user, however, the debug code is not executed.
  */
  #define DACTIONL( ACTION, DEBUGLEVEL ) \
    do { \
      if ( USEFUL::dTest( DEBUGLEVEL ) ) { \
        ACTION; \
      } \
    } while ( 0 )

  /*!
    @brief Preprocessor macro DACTIONELSE is used to execute different arbitrary
           code depending on whether the currently active C++ scope lying on top
           of the stack shall be debugged or not.

    @param[in] ACTION Any kind of C++ code used for debugging which is executed
                      when debugging of the current C++ scope is active.
    @param[in] ALTACTION Any kind of alternative C++ code which is executed when
                         debugging of the current C++ scope is inactive.

    Whenever DACTIONELSE is executed and the activation name lying on top of the
    current stack refers to a function to be debugged, the given debug code is
    executed. Otherwise, DACTIONELSE executes the specified alternative action
    code.

    @note: DACTIONELSE leaves the ALTACTION code in the user's C++ code base
           even if the debug macros are completely disabled via preprocessor
           macro DEBUGMACROS! However, this behavior is desired, since the user
           wants to execute an explicit alternative action when debugging is
           actually DISabled. Otherwise, DACTION would be the user's prefered
           choice.
  */
  #define DACTIONELSE( ACTION, ALTACTION ) \
    do { \
      if ( USEFUL::dTest( 1 ) ) { \
        ACTION; \
      } else { \
        ALTACTION; \
      } \
    } while ( 0 )

  /*!
    @brief Preprocessor macro DIFTHEN is used to execute any arbitrary debugging
           code for the currently active C++ scope, depending on a user-provided
           test condition.

    @param[in] CONDITION Any kind of C++ code specifying a Boolean condition
                         that must hold in order to execute debug code.
    @param[in] ACTION Any kind of C++ code used for debugging.

    Whenever DIFTHEN is executed and the activation name lying on top of the
    current stack refers to a function to be debugged and the user-provided
    Boolean condition holds, the given debug code is executed. Otherwise,
    DIFTHEN does not execute the specified debug code.
  */
  #define DIFTHEN( CONDITION, ACTION ) \
    do { \
      if ( USEFUL::dTest( 1 ) && ( CONDITION ) ) { \
        ACTION; \
      } \
    } while ( 0 )

  /*!
    @brief Preprocessor macro DIFTHENL is used to execute any arbitrary
           debugging code for the currently active C++ scope, depending on a
           user-provided test condition as well as a user-provided debug level.

    @param[in] CONDITION Any kind of C++ code specifying a Boolean condition
                         that must hold in order to execute debug code.
    @param[in] ACTION Any kind of C++ code used for debugging.
    @param[in] DEBUGLEVEL A signed integer that specifies a user-provided debug
                          level.

    DIFTHENL generally executes the given debug code if the activation name
    lying on top of the current stack refers to a function to be debugged
    according to the debug macro configuration files processed by DINIT and if
    the user-provided Boolean condition holds. If such a configuration file
    contains an 'ignorefunctionname = on' command and the user-provided debug
    level is 0 and the user-provided Boolean condition holds, then the debug
    code is always executed, irrespective of whether the top activation name on
    the stack refers to a function to be debugged or not. For negative debug
    levels provided by the user or whenever the user-provided Boolean condition
    is not satisfied, however, the debug code is not executed.
  */
  #define DIFTHENL( CONDITION, ACTION, DEBUGLEVEL ) \
    do { \
      if ( USEFUL::dTest( DEBUGLEVEL ) && ( CONDITION ) ) { \
        ACTION; \
      } \
    } while ( 0 )

  /*!
    @brief Preprocessor macro WAITFORKEY is used to wait for a key press by the
           user, depending on a user-provided debug level.

    @param[in] DEBUGLEVEL A signed integer that specifies a user-provided debug
                          level.

    WAITFORKEY generally prints a short message and waits until a key press by
    the user if the activation name lying on top of the current stack refers to
    a function to be debugged according to the debug macro configuration files
    processed by DINIT. If the user presses 'x', the program terminates.
    Otherwise, it continues. If a configuration file contains an
    'ignorefunctionname = on' command and the user-provided debug level is 0,
    then the message is always printed, irrespective of whether the top
    activation name on the stack refers to a function to be debugged or not. For
    negative debug levels provided by the user, however, no message is printed
    and the program does not wait for a key press.
  */
  #define WAITFORKEY( DEBUGLEVEL ) \
    do { \
      if ( USEFUL::dTest( DEBUGLEVEL ) ) \
        USEFUL::dUserInteraction(); \
    } while ( 0 )

  /*!
    @brief Preprocessor macro DEMPTY checks whether the current stack contains
           functions to be debugged or not.

    @return true if the current stack of functions to be debugged in empty,
            false otherwise.
  */
  #define DEMPTY() (USEFUL::dEmpty())

  /*!
    @brief Preprocessor macro DSIZE returns the number of functions to be
           debugged currently lying on the stack.

    @return An unsigned integer denoting the stack's current size.
  */
  #define DSIZE() (USEFUL::dSize())

  /*!
    @brief Preprocessor macro DDUMP prints the current stack of active C++
           scopes to cout.
  */
  #define DDUMP() (USEFUL::dDump())

  /*!
    @brief Preprocessor macro DTEST returns whether debug output shall be
           produced for the currently active C++ scope.

    @return true if debug output shall be produced, false otherwise.

    DTEST indicates to produce debug output if the activation name lying on top
    of the current stack refers to a function to be debugged according to the
    debug macro configuration files processed by DINIT.
  */
  #define DTEST() USEFUL::dTest( 1 )

  /*!
    @brief Preprocessor macro DDESTROY destroys all internal data structures,
           closes all output streams opened for debugging and makes persistent
           that the debug macros remain fully deactivated.
  */
  #define DDESTROY() (USEFUL::dDestroy())

  /*!
    @brief Preprocessor macro DCLOSE clears all internal data structures and
           closes all output streams opened for debugging.
  */
  #define DCLOSE() (USEFUL::dClose())

#else

  // Here come the empty, dummy debug macro declarations.

  #define DINIT( CONFIGFILE, LOGFILE ) \
    do {} while ( 0 )

  #define DSTART( FUNCTIONNAME ) \
    do {} while ( 0 )

  #define DSTART_EXT( FUNCTIONNAME, ACTIVATIONNAME ) \
    do {} while ( 0 )

  #define DOUT( ... ) \
    do {} while ( 0 )

  #define DOUTL( ... ) \
    do {} while ( 0 )

  #define DCHECK( VARIABLE ) \
    do {} while ( 0 )

  #define DCHECKX( VARIABLE ) \
    do {} while ( 0 )

  #define DDECLARE( DECLARATION ) \
    do {} while ( 0 )

  #define DACTION( ACTION ) \
    do {} while ( 0 )

  #define DACTIONL( ACTION, DEBUGLEVEL ) \
    do {} while ( 0 )

  #define DACTIONELSE( ACTION, ALTACTION ) \
    do { ALTACTION; } while ( 0 )

  #define DIFTHEN( CONDITION, ACTION ) \
    do {} while ( 0 )

  #define DIFTHENL( CONDITION, ACTION, DEBUGLEVEL ) \
    do {} while ( 0 )

  #define WAITFORKEY( DEBUGLEVEL ) \
    do {} while ( 0 )

  #define DEMPTY() \
    do {} while ( 0 )

  #define DSIZE() \
    do {} while ( 0 )

  #define DDUMP() \
    do {} while ( 0 )

  #define DTEST() \
    false

  #define DDESTROY() \
    do {} while ( 0 )

  #define DCLOSE() \
    do {} while ( 0 )

#endif


//
// Header section
//

namespace USEFUL {

/*!
  @brief dInitFile reads a debug macro configuration file and sets up its
         associated output streams and internal data structures.

  @param[in] configFile A const reference to a string holding the name of the
                        configuration file to be read.
  @param[in] logFile A const reference to a string holding the name of the
                     output stream to which debug output will be sent.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
void dInitFile( const std::string &configFile, const std::string &logFile );


/*!
  @brief dStart starts debugging for the currently active C++ scope.

  @param[in] functionName A const reference to a string holding the name of the
                          function to be debugged as it is displayed in the
                          debug macros' output.
  @param[in] activationName A const reference to a string holding the activation
                            name of the function to be debugged.

  Whenever dStart is executed and the given activation name occurs in some debug
  macro configuration file (cf. preprocessor macro DINIT), all debug code
  following dStart in the current C++ scope will be activated and produces debug
  output.

  Furthermore, dStart uses the specified function name in the debug output in
  order to properly display which function produces debug output.

  dStart should thus be called at the beginning of every function which produces
  debug output. All executed occurrences of dStart() are internally organized on
  a stack, so that every invocation of dStart() must be accompanied by some
  corresponding call of dEnd().
*/
void dStart( const std::string &functionName,
             const std::string &activationName );

/*!
  @brief dOut determines the output stream used to produce debug output for the
         currently executed function.

  @return A reference to the stream associated with debug output of the
          currently executed function.
*/
std::ostream &dOut( void );

/*!
  @brief dUserInteraction prints a short message and waits until a key press by
         the user.

  If the user presses 'x', the program terminates. Otherwise, it continues.
*/
void dUserInteraction( void );

/*!
  @brief dEmpty checks whether the current stack contains functions to be
         debugged or not.

  @return true if the current stack of functions to be debugged in empty, false
          otherwise.
*/
bool dEmpty( void );

/*!
  @brief dSize returns the number of functions to be debugged currently lying on
         the stack.

  @return An unsigned integer denoting the stack's current size.
*/
unsigned int dSize( void );

/*!
  @brief dDump prints the current stack of active C++ scopes to cout.
*/
void dDump( void );

/*!
  @brief dTest returns whether debug output shall be produced for the currently
         active C++ scope., depending on a user-provided debug level.

  @param[in] debugLevel A signed integer that specifies a user-provided debug
                        level.
  @return true if debug output shall be produced, false otherwise.

  dTest generally indicates to produce debug output if the activation name lying
  on top of the current stack refers to a function to be debugged according to
  the debug macro configuration files processed by dInitFile. If such a
  configuration file contains an 'ignorefunctionname = on' command and the
  user-provided debug level is 0, then debug output is always produced,
  irrespective of whether the top activation name on the stack refers to a
  function to be debugged or not. For negative debug levels provided by the
  user, however, no debug output is produced.
*/
bool dTest( int debugLevel );

/*!
  @brief dEnd ends debugging for the currently active C++ scope.

  Whenever dEnd is executed, the activation name lying on top of the current
  debug macro stack is removed so that this C++ scope no longer produces debug
  output.

  While leaving the top scope of the stack, dEnd uses its function name in order
  to properly display which debugged function is currently left.

  dEnd has to be called at the end of every function/scope which produces debug
  output. This is realized by the C++ "Autodebug" feature which makes dStart and
  dEnd calls triggered via local struct objects in RAII-style. Since dEnd is
  invoked automatically by class AutoEnd, there is no preprocessor macro like,
  e.g., DEND in order to explicitly leave a scope in C++ application code to be
  debugged.
*/
void dEnd( void );

/*!
  @brief dDestroy destroys all internal data structures, closes all output
         streams opened for debugging and makes persistent that the debug macros
         remain fully deactivated.
*/
void dDestroy( void );

/*!
  @brief dClose clears all internal data structures and closes all output
         streams opened for debugging.
*/
void dClose( void );


/*!
  @brief AutoEnd is a helper class that automatically calls dStart and dEnd in a
         C++ scope.
*/
class AutoEnd
{

  public:

    /*!
      @brief Default constructor for the DSTART macro.

      @param[in] functionName A const reference to a string holding the name of
                              the function to be debugged as it is displayed in
                              the debug macros' output.
    */
    inline explicit AutoEnd( const std::string &functionName )
    {
      dStart( functionName, functionName );
    };

    /*!
      @brief Default constructor for the DSTART_EXT macro.

      @param[in] functionName A const reference to a string holding the name of
                              the function to be debugged as it is displayed in
                              the debug macros' output.
      @param[in] activationName A const reference to a string holding the
                                activation name of the function to be debugged.
    */
    inline AutoEnd( const std::string &functionName,
                    const std::string &activationName )
    {
      dStart( functionName, activationName );
    };

    /*!
      @brief Destructor.
    */
    inline ~AutoEnd( void )
    {
      dEnd();
    };
};

}       // namespace USEFUL

#endif  // _DEBUGMACROS_H
