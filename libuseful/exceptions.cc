/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2017 - 2026, Heiko Falk.

*/


// Standard headers
#include <stdexcept>
#include <string>

// Local headers
#include "exceptions.h"
#include "io.h"

using namespace std;


const char *usefulError::what() const throw()
{
  mErrorString.clear();

  if ( !mFile.empty() ) {
    mErrorString = "File: " + mFile;

    if ( mLine != 0 )
      mErrorString += ", line: " + std::to_string( mLine );

    mErrorString += ": ";
  }

  mErrorString += std::runtime_error::what();

  return( mErrorString.c_str() );
};


void usefulError::print() const throw()
{
  if ( mFatalStream )
    ufFatalMsg << ufFile( mFile, mLine ) << std::runtime_error::what() << endl;
  else
    ufErrMsg << ufFile( mFile, mLine ) << std::runtime_error::what() << endl;
};


usefulError::usefulError( const std::string &file, const unsigned int line,
                          const char *what, bool f ) :
  std::runtime_error( what ),
  mFile( file ),
  mLine( line ),
  mErrorString( "" ),
  mFatalStream( f )
{};


usefulError::usefulError( const std::string &file, const unsigned int line,
                          const std::string &what, bool f ) :
  std::runtime_error( what ),
  mFile( file ),
  mLine( line ),
  mErrorString( "" ),
  mFatalStream( f )
{};



ufError::ufError( const char *what, bool f ) :
  usefulError( "", 0, what, f )
{};


ufError::ufError( const std::string &what, bool f ) :
  usefulError( "", 0, what, f )
{};


ufError::ufError( const std::string &file, const unsigned int line,
                  const std::string &msg, bool f ) :
  usefulError( file, line, msg, f )
{};



ufFatalError::ufFatalError( const char *what, bool f ) :
  usefulError( "", 0, what, f )
{};


ufFatalError::ufFatalError( const std::string &what, bool f ) :
  usefulError( "", 0, what, f )
{};


ufFatalError::ufFatalError( const std::string &file, const unsigned int line,
                            const std::string &msg, bool f ) :
  usefulError( file, line, msg, f )
{};
