/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2007 - 2026, Heiko Falk.

*/

/*!
  @file debugmacros.cc
  @brief This file implements all useful C++ debug macros.
*/


//
// Include section
//

// Include standard headers
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>

// Include useful headers
#include "debugmacros.h"
#include "indentedostream.h"
#include "io.h"
#include "stringtools.h"


//
// Preprocessor macros
//

// Define for debugging these debug functions.
// #define DEBUGDEBUG

// Define for tabbing second row.
// #define TABROW


//
// Code section
//

namespace USEFUL {


using namespace std;


//
// Global data structures
//

/*!
  @brief InitializingPointer is a helper pointer class that initializes the
         pointed-to object upon its first usage.

  This is needed to initialize the various static objects of the debug macros'
  central data structures. Having them as plain pointers is clumsy, since
  effectively we never have nullpointers here, since we want to initialize when
  they are first used.

  Having the static variables as plain data objects bears the problem that then,
  other static initializers which try to use the debug macros, will find the
  static members not yet initialized (static initialization order problem).

  Therefore, we use pointers but wrap them intelligently here.
*/
template<class T>
class InitializingPointer
{

  public:

    InitializingPointer( void ) :
      mPtr( nullptr ), mWasDeleted( false )
    {};

    void clear( void )
    {
      delete( mPtr );
      mPtr = nullptr;
      mWasDeleted = true;
    };

    inline T &operator * ( void )
    {
      check();
      return( *mPtr );
    };

    inline T *operator -> ( void )
    {
      check();
      return( mPtr );
    };


  private:

    void check( void )
    {
      if ( !mPtr ) {
        if ( mWasDeleted )
          throw std::runtime_error(
            "Attempt to initialize the debug macros a second time: This can "
            "only happen if a debug macro function is called after dClose() or "
            "dDestroy() were invoked!" );
        else
          mPtr = new T();
      }
    };

    T *mPtr;

    bool mWasDeleted;

};


//! configFileHashes collects the IDs of all debug macro configuration files.
static InitializingPointer<set<size_t>> configFileHashes;

/*!
  @brief streamsOfLogHash maps the log file hashes to the ostreams to which
         debug output is written.
*/
static InitializingPointer<map<size_t,
                               pair<ostream *,
                                    indented_ostream *>>> streamsOfLogHash;

/*!
  @brief activationHashesToDebug stores the hashes of those activation strings
         to debug and this way manages those functions with enabled debug
         output.
*/
static InitializingPointer<set<size_t>> activationHashesToDebug;

/*!
  @brief streamHashOfActivationHash maps the debug macros' activation hashes to
         the hashes of the output streams/files to which debug output is
         written.
*/
static InitializingPointer<map<size_t, size_t>> streamHashOfActivationHash;

//! functionCalls stores the names of all active functions.
static InitializingPointer<list<string>> functionCalls;

/*!
  @brief stackActivationHashes stores the activation name hashes of all active
         functions in a stack.
*/
static InitializingPointer<stack<size_t>> stackActivationHashes;

#ifdef DEBUGDEBUG
/*!
  @brief streamNameOfActivationHash maps the debug macros' activation hashes to
         the names of the output streams/files to which debug output is written.
*/
static InitializingPointer<map<size_t, string>> streamNameOfActivationHash;
#endif


//
// Flags to control the debug macros' internal behavior
//

/*!
  @brief bShowFuncStart denotes whether explicit enter and exit message shall
         be written in the context of the dStart or dEnd macros.
*/
bool bShowFuncStart = true;

/*!
  @brief bIgnoreFunctionName denotes whether function names shall be ignored so
         that debug output is always produced.
*/
bool bIgnoreFunctionName = false;

/*!
  @brief bDoAutoDestruction denotes whether the debug macros shall clean up
         everything and destroy themselves automatically during dEnd.
*/
static bool bDoAutoDestruction = false;

#ifdef TABROW
//! iMaxFunctionNameLength is used to indent trailing lines of debug output.
unsigned int iMaxFunctionNameLength = 6;
#endif


//
// Debug macro functions
//

/*
  dInitFile reads a debug macro configuration file and sets up its associated
  output streams and internal data structures.
*/
void dInitFile( const string &configFile, const string &logFile )
{
  hash<string> stringHasher;

  // Set up data structures if they do not yet exist.
  if ( streamsOfLogHash->empty() )
    streamsOfLogHash->insert(
      { stringHasher( "cout" ), { &cout, new indented_ostream( cout ) } } );

  auto configFileHash = stringHasher( configFile );

  // If we have already processed the configFile, simply skip it now.
  if ( configFileHashes->count( configFileHash ) )
    return;

  configFileHashes->insert( configFileHash );

  // Open configuration file.
  #ifdef DEBUGDEBUG
  ufDebugMsg << ufFile( configFile )
             << "Reading debug macro configuration file." << endl;
  #endif
  ifstream ifs( configFile );
  if ( ifs.fail() ) {
    #ifdef DEBUGDEBUG
    ufWarnMsg << ufFile( configFile ) << "Failed to open file, ignoring."
              << endl;
    #endif
    ifs.close();
    return;
  }

  // Open dedicated streams for debug output if necessary.
  auto logHash = stringHasher( trim( logFile ) );
  if ( !streamsOfLogHash->count( logHash ) ) {
    auto *l = new ofstream( logFile, ios::trunc );

    if ( !l ) {
      #ifdef DEBUGDEBUG
      ufWarnMsg << ufFile( logFile ) << "Failed to open debug output stream, "
                << "ignoring debugging for this module." << endl;
      #endif
      ifs.close();
      return;
    }

    auto *io = new indented_ostream( *l );
    streamsOfLogHash->insert( { logHash, { l, io } } );

    #ifdef DEBUGDEBUG
    ufDebugMsg << ufFile( logFile ) << "Opened streams for debug log output."
               << endl;
    #endif
  }
  #ifdef DEBUGDEBUG
  else
    ufDebugMsg << ufFile( logFile ) << "Streams for debug log output already "
               << "opened, skipping." << endl;
  #endif

  // Read the configuration file line by line.
  string line;
  #ifdef DEBUGDEBUG
  unsigned int lineNo = 0;
  #endif

  do {
    #ifdef DEBUGDEBUG
    ++lineNo;
    #endif

    if ( !getline( ifs, line ) )
      break;
    line = trim( line );

    // Skip empty lines.
    if ( line.empty() )
      continue;

    // Skip lines starting with comment signs '#' or '//'.
    if ( ( line.substr( 0, 1 ) == "#" ) || ( line.substr( 0, 2 ) == "//" ) ) {
      #ifdef DEBUGDEBUG
      ufDebugMsg << ufFile( configFile, lineNo )
                 << "Skipping comment '" + line + "'." << endl;
      #endif
      continue;
    }

    // Check whether the current line is a command.
    bool isCommand = false;

    auto n = line.find( '=' );
    if ( ( n != string::npos ) && ( n > 0 ) && ( n == line.rfind( '=' ) ) ) {
      string key, value;

      key.assign( line, 0, n );
      value.assign( line, n + 1, line.size() );

      key = trim( key );
      value = trim( value );

      // Convert both key and value to lower case.
      transform(
        key.begin(), key.end(), key.begin(),
        []( unsigned char c ){ return( tolower( c ) ); } );

      transform(
        value.begin(), value.end(), value.begin(),
        []( unsigned char c ){ return( tolower( c ) ); } );

      // Check for showenterexit and ignorefunctionname commands.
      if ( ( key == "showenterexit" ) &&
           ( ( value == "on" ) || ( value == "off" ) ) ) {
        isCommand = true;
        bShowFuncStart = ( value == "on" ) ? true : false;

        #ifdef DEBUGDEBUG
        ufDebugMsg << ufFile( configFile, lineNo )
                   << "Processed command '" + key + " = " + value + "'."
                   << endl;
        #endif
      } else

      if ( ( key == "ignorefunctionname" ) &&
           ( ( value == "on" ) || ( value == "off" ) ) ) {
        isCommand = true;
        bIgnoreFunctionName = ( value == "on" ) ? true : false;

        #ifdef DEBUGDEBUG
        ufDebugMsg << ufFile( configFile, lineNo )
                   << "Processed command '" + key + " = " + value + "'."
                   << endl;
        #endif
      }
    }

    // Process regular debug macro activation names.
    if ( !isCommand ) {
      auto aHash = stringHasher( line );
      activationHashesToDebug->insert( aHash );
      ( *streamHashOfActivationHash )[ aHash ] = stringHasher( logFile );

      #ifdef DEBUGDEBUG
      ( *streamNameOfActivationHash )[ aHash ] = logFile;
      ufDebugMsg << ufFile( configFile, lineNo )
                 << "Function '" + line + "' produces debug output in '" +
                    logFile + "'." << endl;
      #endif
    }
  } while ( 1 );

  ifs.close();
};


/*
  dStart starts debugging for the currently active C++ scope.

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
             const std::string &activationName )
{
  if ( streamsOfLogHash->empty() ) {
    #ifdef DEBUGDEBUG
    ufDebugMsg << ufFile() << "No log files initialized, calling dInitFile."
               << endl;
    #endif
    dInitFile( "devnull", "devnull" );
  }

  hash<string> stringHasher;
  auto fName = trim( functionName );
  auto aName = trim( activationName );
  auto aHash = stringHasher( aName );

  #ifdef DEBUGDEBUG
  ufDebugMsg << ufFile()
             << "Entered dStart( '" + fName + "', '" + aName
             + "' ), writing to '" + ( *streamNameOfActivationHash )[ aHash ]
             + "'." << endl;
  #endif

  // Add a new function call to the data structures.
  functionCalls->push_front( fName );
  stackActivationHashes->push( aHash );

  // Test if this is a function not to debug.
  if ( !activationHashesToDebug->count( aHash ) )
    return;

  // Get the stream to which debug output will be written.
  auto &out =
    *(streamsOfLogHash->at( streamHashOfActivationHash->at( aHash ) ).second);

  if ( bShowFuncStart ) {
    // Write the dStart enter message.
    out << flush << bold_stdout() << greenfg_stdout() << "--> enter "
        << reset_stdout() << flush;

    out << fName << flush;

    #ifdef TABROW
    out << string( '-', iMaxFunctionNameLength - fName.length() + 2 ) << flush;
    #endif

    out << " (" << stackActivationHashes->size() - 1 << ")" << endl << flush;
  }

  out.indent( +2 );
};


/*
  dOut determines the output stream used to produce debug output for the
  currently executed function.
*/
ostream &dOut( void )
{
  #ifdef DEBUGDEBUG
  if ( activationHashesToDebug->empty() )
    ufDebugMsg << ufFile() << "No functions activated for debugging." << endl;
  if ( stackActivationHashes->empty() )
    ufDebugMsg << ufFile()
               << "No function/scope currently entered for debugging." << endl;
  #endif

  if ( streamsOfLogHash->empty() ) {
    #ifdef DEBUGDEBUG
    ufDebugMsg << ufFile() << "No output streams set up for debugging, falling "
               << "back to 'devnull'." << endl;
    #endif
    dInitFile( "devnull", "devnull" );
  }

  // Return the output stream associated with the function at the top of the
  // stack.
  return(
    *(streamsOfLogHash->at(
        streamHashOfActivationHash->at(
          stackActivationHashes->top() ) ).second) );
};


/*
  dUserInteraction prints a short message and waits until a key press by the
  user.

  If the user presses 'x', the program terminates. Otherwise, it continues.
*/
void dUserInteraction( void )
{
  string event;

  cout << "Press 'x' to exit, any other key to continue  ";
  cin >> event;

  if ( event.compare( "x" ) == 0 )
    exit( -1 );
};


/*
  dEmpty checks whether the current stack contains functions to be debugged or
  not.
*/
bool dEmpty( void )
{
  return( stackActivationHashes->empty() );
};


/*
  dSize returns the number of functions to be debugged currently lying on the
  stack.
*/
unsigned int dSize( void )
{
  return( stackActivationHashes->size() );
};


/*
  dDump prints the current stack of active C++ scopes to cout.
*/
void dDump( void )
{
  for ( auto &s : *functionCalls )
    cout << s << endl;
};


/*
  dTest returns whether debug output shall be produced for the currently
  active C++ scope., depending on a user-provided debug level.

  dTest generally indicates to produce debug output if the activation name lying
  on top of the current stack refers to a function to be debugged according to
  the debug macro configuration files processed by dInitFile. If such a
  configuration file contains an 'ignorefunctionname = on' command and the
  user-provided debug level is 0, then debug output is always produced,
  irrespective of whether the top activation name on the stack refers to a
  function to be debugged or not. For negative debug levels provided by the
  user, however, no debug output is produced.
*/
bool dTest( int debugLevel )
{
  // If function names are ignored, then a debugLevel = 0 produces output.
  if ( ( debugLevel == 0 ) && bIgnoreFunctionName )
    return( true );

  // For negative given debug levels, we never produce debug output.
  if ( debugLevel < 0 )
    return( false );

  // Test if the top function on the stack is a function to debug.
  if ( !stackActivationHashes->empty() &&
       activationHashesToDebug->count( stackActivationHashes->top() ) )
    return( true );

  return( false );
};


/*
  dEnd ends debugging for the currently active C++ scope.

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
void dEnd( void )
{
  if ( stackActivationHashes->empty() ) {
    ufErrMsg << ufFile() << "Debug macro stack empty but dEnd() was called."
             << endl;
    #ifdef DEBUGDEBUG
    exit( -1 );
    #endif
    return;
  }

  #ifdef DEBUGDEBUG
  ufDebugMsg << ufFile()
             << "Entered dEnd( stack=" + stackActivationHashes->size() + ")."
             << endl;
  #endif

  auto aHash = stackActivationHashes->top();

  // Test if this is a function not to debug.
  if ( !activationHashesToDebug->count( aHash ) ) {
    functionCalls->pop_front();
    stackActivationHashes->pop();

    return;
  }

  auto &out =
    *(streamsOfLogHash->at( streamHashOfActivationHash->at( aHash ) ).second);

  if ( stackActivationHashes->size() > 0 ) {
    auto &fName = functionCalls->front();

    // Write the dEnd exit message.
    out.indent( -2 );

    if ( bShowFuncStart && dTest( 1 ) ) {
      out << flush << bold_stdout() << redfg_stdout() << "<--  exit "
          << reset_stdout() << flush;

      out << fName;

      #ifdef TABROW
      out << string( '-', iMaxFunctionNameLength - fName.length() + 2 );
      #endif

      out << " (" << stackActivationHashes->size() - 1 << ")" << endl << flush;
    }

    #ifdef DEBUGDEBUG
    ufDebugMsg << ufFile() << "Removed function '" + fName + "' from stack ("
               << stackActivationHashes->size() << ")." << endl;
    #endif

    functionCalls->pop_front();
    stackActivationHashes->pop();
  }

  // Flush buffer to ensure everything is written before exit.
  hash<string> stringHasher;
  if ( ( *streamHashOfActivationHash )[ aHash ] != stringHasher( "cout" ) )
    out.flush();

  if ( bDoAutoDestruction )
    dDestroy();
};


/*
  dDestroy destroys all internal data structures, closes all output streams
  opened for debugging and makes persistent that the debug macros remain fully
  deactivated.
*/
void dDestroy( void )
{
  dClose();

  bDoAutoDestruction = true;
};


/*
  dClose clears all internal data structures and closes all output streams
  opened for debugging.
*/
void dClose( void )
{
  for ( auto &mapped_logfile : *streamsOfLogHash ) {
    #ifdef DEBUGDEBUG
    ufDebugMsg << ufFile( mapped_logfile.first ) << "Closing debug output "
               << "stream." << endl;
    #endif

    // Flush all output buffers.
    mapped_logfile.second.second->flush();
    mapped_logfile.second.first->flush();

    // Delete all output buffers except cout.
    hash<string> stringHasher;
    delete mapped_logfile.second.second;
    if ( mapped_logfile.first != stringHasher( "cout" ) )
      delete mapped_logfile.second.first;
  }

  // Clear all internal data structures.
  streamsOfLogHash.clear();
  streamHashOfActivationHash.clear();
  activationHashesToDebug.clear();
  stackActivationHashes.clear();
  functionCalls.clear();
  configFileHashes.clear();
  #ifdef DEBUGDEBUG
  streamNameOfActivationHash.clear();
  #endif
};

}       // namespace USEFUL
