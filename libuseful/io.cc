/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2017 - 2026, Heiko Falk.

*/

/*!
  @file io.cc
  @brief This file implements several I/O-related utility functions for printing
         error and warning messages etc.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_useful.h>
#endif

// Include standard headers
#include <iostream>
#include <string>
#include <cstring>
#include <sys/ioctl.h>
#include <ctime>
#include <unistd.h>
#include <vector>

// Include libuseful headers
#include "debugmacros.h"
#include "io.h"


//
// Preprocessor macros
//

#define STDOUT_SMSO 0
#define STDOUT_BOLD 1
#define STDOUT_RESET 2
#define STDOUT_BLACKFG 3
#define STDOUT_REDFG 4
#define STDOUT_GREENFG 5
#define STDOUT_YELLOWFG 6
#define STDOUT_BLUEFG 7
#define STDOUT_CYANFG 8
#define STDOUT_MAGENTAFG 9
#define STDOUT_WHITEFG 10
#define STDOUT_TUHHFG 11
#define STDOUT_BLACKBG 12
#define STDOUT_REDBG 13
#define STDOUT_GREENBG 14
#define STDOUT_YELLOWBG 15
#define STDOUT_BLUEBG 16
#define STDOUT_CYANBG 17
#define STDOUT_MAGENTABG 18
#define STDOUT_WHITEBG 19

#define STDERR_SMSO 20
#define STDERR_BOLD 21
#define STDERR_RESET 22
#define STDERR_BLACKFG 23
#define STDERR_REDFG 24
#define STDERR_GREENFG 25
#define STDERR_YELLOWFG 26
#define STDERR_BLUEFG 27
#define STDERR_CYANFG 28
#define STDERR_MAGENTAFG 29
#define STDERR_WHITEFG 30
#define STDERR_TUHHFG 31
#define STDERR_BLACKBG 32
#define STDERR_REDBG 33
#define STDERR_GREENBG 34
#define STDERR_YELLOWBG 35
#define STDERR_BLUEBG 36
#define STDERR_CYANBG 37
#define STDERR_MAGENTABG 38
#define STDERR_WHITEBG 39


//
// Variable declarations section
//


using namespace std;


static bool isInitialized = false;
static bool stdoutHasColors = false;
static bool stderrHasColors = false;

// escSequences contains pointers to the various different escape sequences.
static vector< std::string > escSequences {
  string(), string(), string(), string(),
  string(), string(), string(), string(),
  string(), string(), string(), string(),
  string(), string(), string(), string(),
  string(), string(), string(), string(),
  string(), string(), string(), string(),
  string(), string(), string(), string(),
  string(), string(), string(), string(),
  string(), string(), string(), string(),
  string(), string(), string(), string() };

static string msgPackage { "" };

// ufBannerMsg is a global useful stream for banner messages.
ufBannerStream ufBannerMsg;

// ufDebugMsg is a global useful stream for debug messages.
ufDebugStream ufDebugMsg;

// ufErrMsg is a global useful stream for error messages.
ufErrStream ufErrMsg;

// ufFatalMsg is a global useful stream for fatal error messages.
ufFatalStream ufFatalMsg;

// ufInfoMsg is a global useful stream for info messages.
ufInfoStream ufInfoMsg;

// ufNoteMsg is a global useful stream for note messages.
ufNoteStream ufNoteMsg;

// ufProgrMsg is a global useful stream for progress messages.
ufProgrStream ufProgrMsg;

// ufWarnMsg is a global useful stream for warning messages.
ufWarnStream ufWarnMsg;


//
// Private functions
//

/*!
  @brief IO_Init initializes the I/O module by checking some console-related
         properties.

  IO_Init must be called once before using any other of LIBUSEFUL's I/O
  functions.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
void IO_Init( void )
{
  if ( isInitialized )
    return;

  // Check whether stdout and stderr are attached to ttys which probably have
  // colors.
  stdoutHasColors = isatty( STDOUT_FILENO );
  stderrHasColors = isatty( STDERR_FILENO );

  // This lambda helps to call getEscSequence and to assign the resulting
  // pointers properly
  auto assignEscSeq = [&]( const string &s,
                           size_t i1, size_t i2 ) {
    if ( stdoutHasColors || stderrHasColors ) {
      if ( stdoutHasColors ) {
        if( escSequences[ i1 ].empty() ) {
          escSequences[ i1 ] = s;
        }
      }

      if ( stderrHasColors ) {
        if( escSequences[ i2 ].empty() ) {
          escSequences[ i2 ] = s;
        }
      }
    }
  };

  // Assign escape sequences for bold and normal output.
  assignEscSeq( "\33[7m", STDOUT_SMSO, STDERR_SMSO );
  assignEscSeq( "\33[1m", STDOUT_BOLD, STDERR_BOLD );
  assignEscSeq( "\33(B\33[m", STDOUT_RESET, STDERR_RESET );

  // Determine escape sequences for foreground colors.
  assignEscSeq( "\33[30m", STDOUT_BLACKFG, STDERR_BLACKFG );
  assignEscSeq( "\33[31m", STDOUT_REDFG, STDERR_REDFG );
  assignEscSeq( "\33[32m", STDOUT_GREENFG, STDERR_GREENFG );
  assignEscSeq( "\33[33m", STDOUT_YELLOWFG, STDERR_YELLOWFG );
  assignEscSeq( "\33[34m", STDOUT_BLUEFG, STDERR_BLUEFG );
  assignEscSeq( "\33[35m", STDOUT_CYANFG, STDERR_CYANFG );
  assignEscSeq( "\33[36m", STDOUT_MAGENTAFG, STDERR_MAGENTAFG );
  assignEscSeq( "\33[37m", STDOUT_WHITEFG, STDERR_WHITEFG );
  assignEscSeq( "\033[38;2;45;198;214m", STDOUT_TUHHFG, STDERR_TUHHFG);

  // Determine escape sequences for background colors.
  assignEscSeq( "\33[40m", STDOUT_BLACKBG, STDERR_BLACKBG );
  assignEscSeq( "\33[41m", STDOUT_REDBG, STDERR_REDBG );
  assignEscSeq( "\33[42m", STDOUT_GREENBG, STDERR_GREENBG );
  assignEscSeq( "\33[43m", STDOUT_YELLOWBG, STDERR_YELLOWBG );
  assignEscSeq( "\33[44m", STDOUT_BLUEBG, STDERR_BLUEBG );
  assignEscSeq( "\33[45m", STDOUT_CYANBG, STDERR_CYANBG );
  assignEscSeq( "\33[46m", STDOUT_MAGENTABG, STDERR_MAGENTABG );
  assignEscSeq( "\33[47m", STDOUT_WHITEBG, STDERR_WHITEBG );

  isInitialized = true;
};


/*!
  @brief setFile is an internal helper function used by the stream manipulators
         in order to set file names and line numbers of useful message streams.

  @param[in] s A reference to a stream for which file/line information is
               specified.
  @param[in] file A const reference to a string holding the file name.
  @param[in] line An unsigned integer denoting the line number.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
ios_base &setFile( ios_base &s, const std::string &file, unsigned int line )
{
  auto *p = dynamic_cast<ufStream *>( &s );
  if ( p != nullptr )
    p->setFile( file, line );

  return( s );
};


/*!
  assertMsg prints a failed assertion message in a standardized formatted way.
*/
void assertMsg( const std::string &f, unsigned int l, std::ostream &str )
{
  cout << flush;
  cerr << flush;

  if ( !msgPackage.empty() )
    cerr << msgPackage << ": ";

  cerr << bold_stderr() << whitefg_stderr() << redbg_stderr()
       << "Failed Assertion:" << reset_stderr() << " In '" << f << "', line "
       << dec << l << ": ";

  time_t rawtime;
  time( &rawtime );
  char *buf = new char[26];
  ctime_r( &rawtime, buf );
  // Strip away trailing \n.
  buf[ strlen( buf ) - 1 ] = '\0';
  cerr << "[" << buf << "]:" << endl << string( 8, ' ' ) << bold_stderr();

  string s = dynamic_cast<ostringstream &>( str ).str();

  if ( !stderrHasColors )
    cerr << s;
  else {
    // Determine current width of terminal.
    unsigned int numberOfColumns = 80;
    unsigned int cursorPos = 9;
    struct winsize ws;

    if ( ioctl( 0, TIOCGWINSZ, &ws ) == 0 )
      numberOfColumns = ws.ws_col;

    // Process all words in the stream.
    string currentWord;
    istringstream tmpStr( s );
    bool firstWord = true;

    while ( tmpStr >> currentWord ) {
      // Add word to current line if terminal width is not exceeded.
      if ( cursorPos + currentWord.size() + ( firstWord ? 0 : 1 ) <=
             numberOfColumns + 1 ) {
        if ( !firstWord ) {
          cerr << " ";
          ++cursorPos;
        }
        cerr << currentWord;
        cursorPos += currentWord.size();
      } else {
        // If the line is full, start a new indented line.
        cerr << "\n" << string( 8, ' ' ) << currentWord;
        cursorPos = currentWord.size() + 9;
      }

      firstWord = false;
    }
  }

  cerr << reset_stderr() << endl << flush;

  delete[] buf;
};


//
// Public functions
//

/*
  setMsgPackage sets the package name to be emitted at the very beginning of
  error/warning messages.
*/
void setMsgPackage( const std::string &p )
{
  msgPackage = p;
};


/*
  getMsgPackage returns the package name to be emitted at the very beginning of
  error/warning messages.
*/
const std::string &getMsgPackage( void )
{
  return( msgPackage );
};


/*
  __ufAssertT prints an error message as well as a formatted assertion message
  and dumps core.
*/
void __ufAssertT( const string &assertion, const string &file,
                  unsigned int line, const string &function,
                  const string &text ) throw()
{
  // Flush all debugmacro outputs before throwing an assertion.
  DDESTROY();

  auto slashPos = file.rfind( '/' );
  string baseFileName = file;
  if ( slashPos != string::npos )
    baseFileName = file.substr( slashPos + 1 );

  // Strip eventual [with xx = yy] from function name.
  auto withPos = function.rfind( "[with " );
  string baseFunctionName = function;
  if ( withPos != string::npos )
    baseFunctionName = function.substr( 0, withPos - 1 );

  ostringstream stream;
  stream << "In function '" << baseFunctionName << "': " << assertion;
  assertMsg( baseFileName, line, stream );

  // Print detailed failure message.
  if ( !text.empty() )
    cerr << string( 8, ' ' ) << bold_stderr() << text << reset_stderr() << flush
        << endl;

  abort();
};


/*
  ufFile is a manipulator for useful streams in order to specify file name and
  line number associated with a message.

  ufFile resets a stream such that it assumes that a completely new message is
  beginning. If you use a useful stream without invoking this stream
  manipulator, the currently used message stream is continued. Actually, use
  ufFile whenever you want a fresh header string like, e.g., "Info:" or
  "Warning:" to appear.

  If no file name is specified, no file/line information is displayed at all. If
  no line number is specified, only the file name is written.
*/
smanip ufFile( const char *file, unsigned int line )
{
  return( smanip( setFile, string( file ), line ) );
};


/*
  ufFile is a manipulator for useful streams in order to specify file name and
  line number associated with a message.

  ufFile resets a stream such that it assumes that a completely new message is
  beginning. If you use a useful stream without invoking this stream
  manipulator, the currently used message stream is continued. Actually, use
  ufFile whenever you want a fresh header string like, e.g., "Info:" or
  "Warning:" to appear.

  If no file name is specified, no file/line information is displayed at all. If
  no line number is specified, only the file name is written.
*/
smanip ufFile( const std::string &file, unsigned int line )
{
  return( smanip( setFile, file, line ) );
};


ufStream & ufStream::operator << ( bool v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( char v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( signed char v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( unsigned char v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( short v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( unsigned short v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( int v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( unsigned int v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( long v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( unsigned long v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( long long v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( unsigned long long v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( float v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( double v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( long double v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( const char *v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( const signed char *v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( const unsigned char *v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( void *v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( std::streambuf *v )
{
  startMsg();
  stringstream str;
  str << v;
  formatMsg( str.str() );
  return( *this );
};


ufStream & ufStream::operator << ( const std::string &v )
{
  startMsg();
  formatMsg( v );
  return( *this );
};


ufStream & ufStream::operator << ( std::ostream & (*pf)( std::ostream & ) )
{
  startMsg();
  mStream << pf;
  return( *this );
};


ufStream & ufStream::operator << ( std::ios & (*pf)( std::ios & ) )
{
  startMsg();
  mStream << pf;
  return( *this );
};


ufStream & ufStream::operator << ( std::ios_base & (*pf)( std::ios_base & ) )
{
  startMsg();
  mStream << pf;
  return( *this );
};


ufStream & ufStream::operator << ( const smanip &m )
{
  return( dynamic_cast<ufStream &>( m.f( *this, m.file, m.line ) ) );
};


/*
  Default constructor creating a useful stream that is attached to the given
  standard output stream.

  This constructor also takes care of the overall initialization of the
  libuseful I/O mechanisms.
*/
ufStream::ufStream( std::ostream &s, bool useStdout ) :
  ostream(),
  mCursorPos { 1 },
  mStream { s },
  mOmitHeader { false },
  mPrintLongTime { false },
  mPrintShortTime { false },
  mPrintBoldMessage { true },
  mPrintBoldTime { true },
  mFilename { "" },
  mLine { 0 },
  mReset { true },
  mFirstWord { true },
  mUseStdout { useStdout }
{
  IO_Init();
};


/*
  startMsg starts emitting a formatted message by writing the package name, the
  formatted header, and file/line information if specified.
*/
void ufStream::startMsg( void )
{
  if ( mReset ) {
    mReset = false;

    cout << std::flush;
    cerr << std::flush;
    mCursorPos = 1;

    if ( mOmitHeader )
      return;

    if ( !msgPackage.empty() ) {
      mStream << msgPackage << ": ";
      mCursorPos += msgPackage.size() + 2;
    }

    printHeader();

    bool printTime = mPrintLongTime || mPrintShortTime;

    if ( !mFilename.empty() ) {
      mStream << " In '" << mFilename << "'";
      if ( mLine != 0 )
        mStream << ", line " << std::dec << mLine;
      mStream << ":";
    }

    if ( printTime ) {
      time_t rawtime;
      time( &rawtime );
      char *buf = new char[26];;
      ctime_r( &rawtime, buf );

      if ( mPrintLongTime ) {
        // Strip away trailing \n.
        buf[ strlen( buf ) - 1 ] = '\0';
        mStream << " [" << buf << "]:";
        mCursorPos += strlen( buf ) + 4;
      } else {
        // Strip away trailing \n.
        buf[ strlen( buf ) - 6 ] = '\0';
        if ( mPrintBoldTime )
          mStream << bold_stdout() << bluefg_stdout();
        mStream << " [" << &buf[ 11 ]
                << "]:" << reset_stdout();
        mCursorPos += strlen( &buf[ 11 ] ) + 4;
      }

      delete[] buf;
    }

    if ( !mFilename.empty() ) {
      mStream << endl << string( 8, ' ' );
      mCursorPos = 9;
    } else {
      mStream << " ";
      ++mCursorPos;
    }

    mFirstWord = true;
  }
};


/*
  formatMsg formats the message text such that the current terminal width is
  considered and appropriate line breaks and indentations are produced.
*/
void ufStream::formatMsg( const std::string &str )
{
  bool formatOutput = ( mUseStdout ? stdoutHasColors : stderrHasColors );
  if ( !formatOutput ) {
    mStream << str;
    return;
  }

  if ( mPrintBoldMessage ) {
    if ( mUseStdout )
      mStream << bold_stdout();
    else
      mStream << bold_stderr();
  }

  // Determine current width of terminal.
  unsigned int numberOfColumns = 80;
  struct winsize ws;

  if ( ioctl( 0, TIOCGWINSZ, &ws ) == 0 )
    numberOfColumns = ws.ws_col;

  // Process all words in the string.
  string currentWord;
  istringstream tmpStr( str );

  while ( tmpStr >> currentWord ) {
    // Add word to current line if terminal width is not exceeded.
    if ( mCursorPos + currentWord.size() + ( mFirstWord ? 0 : 1 ) <=
           numberOfColumns + 1 ) {
      if ( !mFirstWord ) {
        mStream << string( " " );
        ++mCursorPos;
      }
      mStream << currentWord;
      mCursorPos += currentWord.size();
    } else {
      // If the line is full, start a new indented line.
      mStream << "\n" << string( 8, ' ' ) << currentWord;
      mCursorPos = currentWord.size() + 9;
    }

    mFirstWord = false;
  }

  if ( mUseStdout )
    mStream << reset_stdout();
  else
    mStream << reset_stderr();

  mStream << std::flush;
};


/*
  setFile sets a useful stream's associated file name and line number
  information.
*/
void ufStream::setFile( const std::string &f, unsigned int l )
{
  mFilename = f;
  mLine = l;
  mReset = true;
};


/*
  Default constructor associating cout with banner messages.
*/
ufBannerStream::ufBannerStream( void ) :
  ufStream( cout, true )
{
  mOmitHeader = true;
};


/*
  printHeader is not used for banner messages.
*/
void ufBannerStream::printHeader( void )
{
};


/*
  Default constructor associating cout with debug messages.
*/
ufDebugStream::ufDebugStream( void ) :
  ufStream( cout, true )
{
  mPrintShortTime = true;
  mPrintBoldMessage = false;
  mPrintBoldTime = false;
};


/*
  printHeader is used to print the header string "Debug:" in a formatted style.

  If terminal capabilities allow, the header is printed in cyan font.
*/
void ufDebugStream::printHeader( void )
{
  mStream << cyanfg_stdout() << "Debug:";
  mCursorPos += 6;
};


/*
  Default constructor associating cerr with error messages.
*/
ufErrStream::ufErrStream( void ) :
  ufStream( cerr, false )
{
};


/*
  printHeader is used to print the header string "Error:" in a formatted style.

  If terminal capabilities allow, the header is printed in bold red font.
*/
void ufErrStream::printHeader( void )
{
  mStream << bold_stderr() << redfg_stderr() << "Error:" << reset_stderr();
  mCursorPos += 6;
};


/*
  Default constructor associating cerr with fatal error messages.
*/
ufFatalStream::ufFatalStream( void ) :
  ufStream( cerr, false )
{
  mPrintLongTime = true;
};


/*
  printHeader is used to print the header string "Fatal Error:" in a formatted
  style.

  If terminal capabilities allow, the header is printed in bold white font on
  red background.
*/
void ufFatalStream::printHeader( void )
{
  mStream << bold_stderr() << whitefg_stderr() << redbg_stderr()
          << "Fatal Error:" << reset_stderr();
  mCursorPos += 12;
};


/*
  Default constructor associating cout with info messages.
*/
ufInfoStream::ufInfoStream( void ) :
  ufStream( cout, true )
{
};


/*
  printHeader is used to print the header string "Info:" in a formatted style.

  If terminal capabilities allow, the header is printed in bold green font.
*/
void ufInfoStream::printHeader( void )
{
  mStream << bold_stdout() << tuhhfg_stdout() << "Info:" << reset_stdout();
  mCursorPos += 5;
};


/*
  Default constructor associating cout with note messages.
*/
ufNoteStream::ufNoteStream( void ) :
  ufStream( cout, true )
{
};


/*
  printHeader is used to print the header string "Note:" in a formatted style.

  If terminal capabilities allow, the header is printed in bold green font.
*/
void ufNoteStream::printHeader( void )
{
  mStream << bold_stdout() << tuhhfg_stdout() << "Note:" << reset_stdout();
  mCursorPos += 5;
};


/*
  Default constructor associating cout with progress messages.
*/
ufProgrStream::ufProgrStream( void ) :
  ufStream( cout, true )
{
  mPrintShortTime = true;
};


/*
  printHeader is used to print the header string "Progress:" in a formatted
  style.
*/
void ufProgrStream::printHeader( void )
{
  mStream << reset_stdout() << "Progress:";
  mCursorPos += 9;
};


/*
  Default constructor associating cerr with warning messages.
*/
ufWarnStream::ufWarnStream( void ) :
  ufStream( cerr, false )
{
};


/*
  printHeader is used to print the header string "Warning:" in a formatted
  style.

  If terminal capabilities allow, the header is printed in bold yellow font.
*/
void ufWarnStream::printHeader( void )
{
  mStream << bold_stderr() << yellowfg_stderr() << "Warning:" << reset_stderr();
  mCursorPos += 8;
};


/*
  smso_stdout returns a C-string containing the escape sequence for
  standard-mode output on stdout.
*/
const char *smso_stdout( void )
{
  return( escSequences[ STDOUT_SMSO ].c_str() );
};


/*
  bold_stdout returns a C-string containing the escape sequence for bold output
  on stdout.
*/
const char *bold_stdout( void )
{
  return( escSequences[ STDOUT_BOLD ].c_str() );
};


/*
  reset_stdout returns a C-string containing the escape sequence for resetting
  all output formats on stdout.
*/
const char *reset_stdout( void )
{
  return( escSequences[ STDOUT_RESET ].c_str() );
};


/*
  blackfg_stdout returns a C-string containing the escape sequence for black
  foreground output on stdout.
*/
const char *blackfg_stdout( void )
{
  return( escSequences[ STDOUT_BLACKFG ].c_str() );
};


/*
  redfg_stdout returns a C-string containing the escape sequence for red
  foreground output on stdout.
*/
const char *redfg_stdout( void )
{
  return( escSequences[ STDOUT_REDFG ].c_str() );
};


/*
  greenfg_stdout returns a C-string containing the escape sequence for green
  foreground output on stdout.
*/
const char *greenfg_stdout( void )
{
  return( escSequences[ STDOUT_GREENFG ].c_str() );
};


/*
  yellowfg_stdout returns a C-string containing the escape sequence for yellow
  foreground output on stdout.
*/
const char *yellowfg_stdout( void )
{
  return( escSequences[ STDOUT_YELLOWFG ].c_str() );
};


/*
  bluefg_stdout returns a C-string containing the escape sequence for blue
  foreground output on stdout.
*/
const char *bluefg_stdout( void )
{
  return( escSequences[ STDOUT_BLUEFG ].c_str() );
};


/*
  cyanfg_stdout returns a C-string containing the escape sequence for cyan
  foreground output on stdout.
*/
const char *cyanfg_stdout( void )
{
  return( escSequences[ STDOUT_CYANFG ].c_str() );
};


/*
  magentafg_stdout returns a C-string containing the escape sequence for magenta
  foreground output on stdout.
*/
const char *magentafg_stdout( void )
{
  return( escSequences[ STDOUT_MAGENTAFG ].c_str() );
};


/*
  whitefg_stdout returns a C-string containing the escape sequence for white
  foreground output on stdout.
*/
const char *whitefg_stdout( void )
{
  return( escSequences[ STDOUT_WHITEFG ].c_str() );
};


/*
  tuhhfg_stdout returns a C-string containing the escape sequence for tuhh
  cyan foreground output on stdout.
 */
const char *tuhhfg_stdout( void )
{
  return( escSequences[ STDOUT_TUHHFG ].c_str() );
};



/*
  blackbg_stdout returns a C-string containing the escape sequence for black
  background output on stdout.
*/
const char *blackbg_stdout( void )
{
  return( escSequences[ STDOUT_BLACKBG ].c_str() );
};


/*
  redbg_stdout returns a C-string containing the escape sequence for red
  background output on stdout.
*/
const char *redbg_stdout( void )
{
  return( escSequences[ STDOUT_REDBG ].c_str() );
};


/*
  greenbg_stdout returns a C-string containing the escape sequence for green
  background output on stdout.
*/
const char *greenbg_stdout( void )
{
  return( escSequences[ STDOUT_GREENBG ].c_str() );
};


/*
  yellowbg_stdout returns a C-string containing the escape sequence for yellow
  background output on stdout.
*/
const char *yellowbg_stdout( void )
{
  return( escSequences[ STDOUT_YELLOWBG ].c_str() );
};


/*
  bluebg_stdout returns a C-string containing the escape sequence for blue
  background output on stdout.
*/
const char *bluebg_stdout( void )
{
  return( escSequences[ STDOUT_BLUEBG ].c_str() );
};


/*
  cyanbg_stdout returns a C-string containing the escape sequence for cyan
  background output on stdout.
*/
const char *cyanbg_stdout( void )
{
  return( escSequences[ STDOUT_CYANBG ].c_str() );
};


/*
  magentabg_stdout returns a C-string containing the escape sequence for magenta
  background output on stdout.
*/
const char *magentabg_stdout( void )
{
  return( escSequences[ STDOUT_MAGENTABG ].c_str() );
};


/*
  whitebg_stdout returns a C-string containing the escape sequence for white
  background output on stdout.
*/
const char *whitebg_stdout( void )
{
  return( escSequences[ STDOUT_WHITEBG ].c_str() );
};


/*
  smso_stderr returns a C-string containing the escape sequence for
  standard-mode output on stderr.
*/
const char *smso_stderr( void )
{
  return( escSequences[ STDERR_SMSO ].c_str() );
};


/*
  bold_stderr returns a C-string containing the escape sequence for bold output
  on stderr.
*/
const char *bold_stderr( void )
{
  return( escSequences[ STDERR_BOLD ].c_str() );
};


/*
  reset_stderr returns a C-string containing the escape sequence for resetting
  all output formats on stderr.
*/
const char *reset_stderr( void )
{
  return( escSequences[ STDERR_RESET ].c_str() );
};


/*
  blackfg_stderr returns a C-string containing the escape sequence for black
  foreground output on stderr.
*/
const char *blackfg_stderr( void )
{
  return( escSequences[ STDERR_BLACKFG ].c_str() );
};


/*
  redfg_stderr returns a C-string containing the escape sequence for red
  foreground output on stderr.
*/
const char *redfg_stderr( void )
{
  return( escSequences[ STDERR_REDFG ].c_str() );
};


/*
  greenfg_stderr returns a C-string containing the escape sequence for green
  foreground output on stderr.
*/
const char *greenfg_stderr( void )
{
  return( escSequences[ STDERR_GREENFG ].c_str() );
};


/*
  yellowfg_stderr returns a C-string containing the escape sequence for yellow
  foreground output on stderr.
*/
const char *yellowfg_stderr( void )
{
  return( escSequences[ STDERR_YELLOWFG ].c_str() );
};


/*
  bluefg_stderr returns a C-string containing the escape sequence for blue
  foreground output on stderr.
*/
const char *bluefg_stderr( void )
{
  return( escSequences[ STDERR_BLUEFG ].c_str() );
};


/*
  cyanfg_stderr returns a C-string containing the escape sequence for cyan
  foreground output on stder.
*/
const char *cyanfg_stderr( void )
{
  return( escSequences[ STDERR_CYANFG ].c_str() );
};


/*
  magentafg_stderr returns a C-string containing the escape sequence for magenta
  foreground output on stderr.
*/
const char *magentafg_stderr( void )
{
  return( escSequences[ STDERR_MAGENTAFG ].c_str() );
};


/*
  whitefg_stderr returns a C-string containing the escape sequence for white
  foreground output on stderr.
*/
const char *whitefg_stderr( void )
{
  return( escSequences[ STDERR_WHITEFG ].c_str() );
};

/*
  tuhhfg_stdout returns a C-string containing the escape sequence for tuhh
  cyan foreground output on stderr.
 */
const char *tuhhfg_stderr( void )
{
  return( escSequences[ STDERR_TUHHFG ].c_str() );
};


/*
  blackbg_sterr returns a C-string containing the escape sequence for black
  background output on stderr.
*/
const char *blackbg_stderr( void )
{
  return( escSequences[ STDERR_BLACKBG ].c_str() );
};


/*
  redbg_stderr returns a C-string containing the escape sequence for red
  background output on stderr.
*/
const char *redbg_stderr( void )
{
  return( escSequences[ STDERR_REDBG ].c_str() );
};


/*
  greenbg_stderr returns a C-string containing the escape sequence for green
  background output on stderr.
*/
const char *greenbg_stderr( void )
{
  return( escSequences[ STDERR_GREENBG ].c_str() );
};


/*
  yellowbg_stderr returns a C-string containing the escape sequence for yellow
  background output on stderr.
*/
const char *yellowbg_stderr( void )
{
  return( escSequences[ STDERR_YELLOWBG ].c_str() );
};


/*
  bluebg_stderr returns a C-string containing the escape sequence for blue
  background output on stderr.
*/
const char *bluebg_stderr( void )
{
  return( escSequences[ STDERR_BLUEBG ].c_str() );
};


/*
  cyanbg_stderr returns a C-string containing the escape sequence for cyan
  background output on stderr.
*/
const char *cyanbg_stderr( void )
{
  return( escSequences[ STDERR_CYANBG ].c_str() );
};


/*
  magentabg_stderr returns a C-string containing the escape sequence for magenta
  background output on stderr.
*/
const char *magentabg_stderr( void )
{
  return( escSequences[ STDERR_MAGENTABG ].c_str() );
};


/*
  whitebg_stderr returns a C-string containing the escape sequence for white
  background output on stderr.
*/
const char *whitebg_stderr( void )
{
  return( escSequences[ STDERR_WHITEBG ].c_str() );
};
