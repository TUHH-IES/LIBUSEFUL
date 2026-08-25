/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2009 - 2026, Heiko Falk.

*/


//
// Include section
//

// Include local headers
#include "indentedostream.h"

//
// Class section
//


indented_streambuf::indented_streambuf( size_t w, std::streambuf* s ) :
    indent_width( 0 ), width( w ), def_width( w ), count( 0 ), sbuf( s )
{
}


indented_streambuf::~indented_streambuf()
{
  flush();
}


size_t indented_streambuf::get_indent()
{
  return indent_width;
}


void indented_streambuf::set_indent( size_t w )
{
  indent_width = w;
  prefix = std::string( indent_width, ' ' );
  width = def_width - w;
}


indented_streambuf::int_type indented_streambuf::flush()
{
  count = 0;
  sbuf->sputn( prefix.c_str(), indent_width );
  int_type rc = sbuf->sputn( buffer.c_str(), buffer.size() );
  buffer.clear();
  return rc;
}


indented_streambuf::int_type
indented_streambuf::overflow( int_type c )
{
  // This is basically a line-buffering stream buffer.
  // The algorithm is:
  // - Explicit end of line ("\r" or "\n"): we flush our buffer
  //   to the underlying stream's buffer, and set our record of
  //   the line length to 0.
  // - An "alert" character: sent to the underlying stream
  //   without recording its length, since it doesn't normally
  //   affect the a appearance of the output.
  // - tab: treated as moving to the next tab stop, which is
  //   assumed as happening every tab_width characters.
  // - Everything else: really basic buffering with word wrapping.
  //   We try to add the character to the buffer, and if it exceeds
  //   our line width, we search for the last space/tab in the
  //   buffer and break the line there. If there is no space/tab,
  //   we break the line at the limit.
  if ( traits_type::eq_int_type( traits_type::eof(), c ) )
    return traits_type::not_eof( c );

  switch ( c ) {
    case '\n':
    case '\r': {
      buffer += c;
      return flush();
    }

    case '\a':
      return sbuf->sputc( c );

    case '\t':
      buffer += c;
      count += tab_width - count % tab_width;
      return c;

    default:
      if ( count >= width ) {
        size_t wpos = buffer.find_last_of( " \t" );
        if ( wpos != std::string::npos ) {
          sbuf->sputn( prefix.c_str(), indent_width );
          sbuf->sputn( buffer.c_str(), wpos );
          count = buffer.size() - wpos - 1;
          buffer = std::string( buffer, wpos + 1 );
        }
        else {
          sbuf->sputn( prefix.c_str(), indent_width );
          sbuf->sputn( buffer.c_str(), buffer.size() );
          buffer.clear();
          count = 0;
        }
        sbuf->sputc( '\n' );
      }
      buffer += c;
      ++count;
      return c;
  }
}


indented_ostream::indented_ostream( std::ostream &os, size_t width ) :
    std::ostream( &buf ), buf( width, os.rdbuf() )
{
}


indented_ostream& indented_ostream::indent( size_t w )
{
  buf.set_indent( buf.get_indent() + w );
  return *this;
}


size_t indented_ostream::get_indent()
{
  return buf.get_indent();
}


void indented_ostream::set_indent( size_t  w )
{
  buf.set_indent( w );
}


indented_ostream &indented_ostream::reset_indent()
{
  buf.set_indent( 0 );
  return *this;
}
