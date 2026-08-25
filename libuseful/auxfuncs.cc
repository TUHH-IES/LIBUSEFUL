/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2007 - 2026, Heiko Falk.

*/


#ifdef HAVE_CONFIG_H
#include <config_useful.h>
#endif


// Include private headers
#include "auxfuncs.h"

using namespace std;

/*
   Return name of current module.
*/
string libusefulModule()
{
  string mod( LIBUSEFUL_MODULE );

  return mod;
};


/*
   Return version information.
*/
string libusefulVersion()
{
  string version( LIBUSEFUL_VERSION );

  return version;
};
