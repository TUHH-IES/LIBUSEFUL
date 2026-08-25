/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2009 - 2026, Heiko Falk.

*/


#ifndef _INDENTED_OSTREAM_H
#define _INDENTED_OSTREAM_H

// Include standard headers
#include <limits>
#include <iostream>
#include <streambuf>
#include <iomanip>
#include <string>

//! A buffer for indenting a std::ostreams outputs
/*!
 * This class buffers whole lines as characters are read into the buffer one
 * by one and outputs the content of the buffer when a newline is seen or when
 * a predefined width is exceeded.
 */
class indented_streambuf : public std::streambuf {

  public:

    //! Constructor with defined maximum output width
    indented_streambuf( std::size_t width, std::streambuf* wrapped_buf );

    //! Destructor (finishes last line)
    ~indented_streambuf();

    //! Get the indent level to use
    std::size_t get_indent();

    //! Set the indent level to use
    void set_indent( std::size_t  w );

  protected:

    //! Output operator for single characters
    int_type overflow(int_type c);

    //! Flushes the current line
    int_type flush();

    //! The number of indentation spaces currently used
    std::size_t indent_width;
    //! The number of characters left for user content on each line
    std::size_t width;
    //! The original width (with zero indentation)
    std::size_t def_width;
    //! The number of characters printed on the current line
    std::size_t count;
    //! The tab size in spaces
    static const int tab_width = 8;
    //! The whitespace which is the current indent
    std::string prefix;
    //! The wrapped streambuf object
    std::streambuf* sbuf;
    //! The current line
    std::string buffer;
};

//! A wrapper stream which uses a configurable indentation level
class indented_ostream : public std::ostream
{
    indented_streambuf buf;

  public:

    //! Default constructor
    indented_ostream( std::ostream &os,
                      std::size_t width = std::numeric_limits<std::size_t>::max() );

    //! Change the indentation level (positive or negative offset)
    indented_ostream &indent( std::size_t w );

    //! Get the indent level to use
    std::size_t get_indent();

    //! Set the indent level to use
    void set_indent( std::size_t  w );

    //! Reset the indentation level to zero
    indented_ostream &reset_indent();

};

#endif
