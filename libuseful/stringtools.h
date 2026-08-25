/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2007 - 2026, Heiko Falk.

*/


#ifndef _STRINGTOOLS_H
#define _STRINGTOOLS_H


//
// Include section
//

// Include standard headers
#include <cstddef>
#include <istream>
#include <string>


//
// Header section
//

/*!
  @brief stripPath removes all "/../" from a given path and returns an absolute
         path even if the paramater was a relative one.
*/
std::string stripPath( const std::string & );

/*!
  @brief getNextElement is used for parsing lines like values in a csv
         spreadsheet.

  The parameter "line" is scanned beginning at postition pos until a specified
  "delimiter" occurs. It returns the part string found and updates the position
  "pos".
*/
std::string getNextElement( const std::string &, char, std::size_t & );

/*!
  @brief toLong converts the given string to an integral long value.

  @param[in] s The string to be converted.
  @return Upon successful conversion, the corresponding long value.

  If the specified string cannot be converted properly to an integer long
  value, the return value is undefined.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
unsigned long toLong( const std::string &s );

/*!
  @brief toDouble converts the given string to a long double value.

  @param[in] s The string to be converted.
  @return Upon successful conversion, the corresponding long double value.

  If the specified string cannot be converted properly to a long double
  value, the return value is undefined.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
long double toDouble( const std::string &s );

/*!
  @brief toFraction interpretes the given string as a fraction and converts
          it to a long double value.

  @param[in] s The string to be converted.
  @return Upon successful conversion, the corresponding long double value.

  If the specified string cannot be converted properly to a long double
  value, the return value is undefined.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
long double toFraction( const std::string &s );

/*!
  @brief charToOctal interpretes the given char as decimal value and converts it
         to a string in its octal representation.

  @param[in] c The char to be converted.
  @return A string containing the octal representation of the specified char.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
std::string charToOctal( unsigned char c );

/*!
  @brief trim removes leading and trailing whitespaces from a string.

  @param[in] s A const reference to a string where whitespaces shall be removed.
  @return The trimmed string without leading and heading whitespaces.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
std::string trim( const std::string &s );

/*!
  @brief replaceAll replaces all occurrences of one sub-string in one other
         string by some third string.

  @param[in,out] str A reference to the string in which replacements shall be
                     carried out.
  @param[in] from A const reference to a string to be replaced.
  @param[in] to A const reference to a string acting as replacement.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
void replaceAll( std::string &str, const std::string &from,
                 const std::string &to );

#endif  // _STRINGTOOLS_H
