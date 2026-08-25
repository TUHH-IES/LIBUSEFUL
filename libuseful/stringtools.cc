/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2007 - 2026, Heiko Falk.

*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_useful.h>
#endif

// Include standard headers
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h> // PATH_MAX

// Include libuseful headers
#include "debugmacros.h"
#include "io.h"
#include "stringtools.h"


//
// Code section
//


using namespace std;


/*
  stripPath removes all "/../" from a given path and returns an absolute path
  even if the paramater was a relative one.
*/
string stripPath( const string &_p )
{
  string path = _p;

  // We have to add the path to current dir if the supplied path is relative.
  if ( path[ 0 ] != '/' ) {

    // Determine the current path.
    char buffer[ PATH_MAX ];
    char *result = getcwd( buffer, PATH_MAX );
    ufAssertT( result, "Buffer overflow." );
    string currentPath = buffer;

    path = currentPath + "/" + path;
  }

  if ( path.find( "../" ) == std::string::npos )
    return( path );

  size_t stopPos = path.find( "/../" );

  do {
    // Copy the part until the first "/../".
    string part = path.substr( 0, stopPos );

    // Copy the rest of the path without the first "/../".
    path = path.substr( stopPos + 3, path.length() );

    // Remove the string after the last slash.
    size_t removePos = part.rfind( "/" );
    part = part.substr( 0, removePos );

    // Concatenate the two parts.
    path = part + path;

    stopPos = path.find( "/../" );
  } while ( stopPos != std::string::npos );

  return( path );
};


/*
  getNextElement is used for parsing lines like values in a csv spreadsheet.

  The parameter "line" is scanned beginning at postition pos until a specified
  "delimiter" occurs. It returns the part string found and updates the position
  "pos".
*/
string getNextElement( const string &line, char delimiter, size_t &pos )
{
  // Return if pos is behind the end of line.
  if ( pos >= line.length() )
    return( string( "" ) );

  size_t nextPos = line.find( delimiter, pos );

  string element = line.substr( pos, nextPos );
  pos = nextPos + 1;
  return( element );
};


/*
  toLong converts the given string to an integral long value.

  If the specified string cannot be converted properly to an integer long value,
  the return value is undefined.
*/
unsigned long toLong( const std::string &s )
{
  DSTART( "long unsigned int toLong(const string&)" );

  unsigned long res;
  istringstream iss( s );

  iss >> dec;
  if ( s[ 0 ] == '0' ) {
    // Octal value found.
    iss >> oct;

    if ( ( s[ 1 ] == 'x' )  || ( s[ 1 ] == 'X' ) )
      // Opps, not octal but hex instead.
      iss >> hex;
  }
  iss >> res;

  return( res );
};


/*
  toDouble converts the given string to a long double value.

  If the specified string cannot be converted properly to a long double value,
  the return value is undefined.
*/
long double toDouble( const std::string &s )
{
  DSTART( "long double toDouble(const string&)" );

  long double res;
  istringstream iss( s );

  iss >> res;

  return( res );
};


/*
  toFraction interpretes the given string as a fraction and converts it to a
  long double value.

  If the specified string cannot be converted properly to a long double value,
  the return value is undefined.
*/
long double toFraction( const std::string &s )
{
  DSTART( "long double toFraction(const string&)" );

  long double res = 0.0L;
  string::size_type n;

  if ( ( ( n = s.find( ':' ) ) != string::npos ) && ( n > 0 ) &&
         ( s.find( ':' ) == s.rfind( ':' ) ) ) {
    string numerator, denominator;

    numerator.assign( s, 0, n );
    denominator.assign( s, n + 1, s.size() );

    numerator = trim( numerator );
    denominator = trim( denominator );

    long double nu = toDouble( numerator );
    long double d = toDouble( denominator );
    res = nu / d;
  }

  return( res );
};


/*
  charToOctal interpretes the given char as decimal value and converts it to a
  string in its octal representation.
*/
std::string charToOctal( unsigned char c )
{
  DSTART( "string charToOctal(unsigned char)" );

  string res = "\\";
  stringstream sstr;

  unsigned int digit = c / 64;
  sstr << digit;
  c -= digit * 64;

  digit = c / 8;
  sstr << digit;
  c -= digit * 8;

  digit = c;
  sstr << digit;

  res += sstr.str();

  return( res );
};


/*
  trim removes leading and trailing whitespaces from a string.
*/
std::string trim( const std::string &s )
{
  DSTART( "string trim(const string&)" );

  static const string whitespace = " \t\r";
  string res( s );
  string::size_type n;

  // Remove leading whitespaces.
  if ( ( n = res.find_first_not_of( whitespace ) ) != string::npos )
    res.erase( 0, n );

  // Remove trailing whitespaces.
  if ( ( n = res.find_last_not_of( whitespace ) ) != string::npos )
    res.erase( n + 1 );

  return( res );
};


void replaceAll( std::string &str, const std::string &from,
                 const std::string &to )
{
  DSTART( "void replaceAll(string&, const string&, const string&)" );

  if ( from.empty() )
    return;

  size_t start_pos = 0;
  while ( ( start_pos = str.find( from, start_pos ) ) != std::string::npos ) {
    str.replace( start_pos, from.length(), to );
    start_pos += to.length();
  }
};
