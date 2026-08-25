# LIBUSEFUL

This package contains the USEFUL library, a collection of useful routines and classes for compiler development and debugging.


## Attribution/Citation:

This code is © Heiko Falk, 2007 - 2026, and it is made available under the MIT license enclosed with the software.

Over and above the legal restrictions imposed by this license, if you use this software for an academic publication then you are obliged to provide proper attribution. This shall come in the form of a citation in your publication such as

  H. Falk. LIBUSEFUL: A Collection of Useful Routines and Classes for Compiler Development and Debugging, v1.1 (2026).
  github.com/cknight/acds, Zenodo DOI.


## Directory Structure

The current folder contains the following sub-folders:

* doc: Contains the entire documentation of the USEFUL library in the form of the API documentation generated dynamically using doxygen (doc/html/index.html).
* m4: Contains some required autoconf macros.
* libuseful: Contains all C++ headers and implementations coming with LIBUSEFUL.


## Documentation

The folder doc contains the documentation for LIBUSEFUL.


## Build & Installation

See chapters 2-5 (Build & Installation Guide) of file doc/wir.pdf.

### Bootstrapping

If you just have checked out this software distribution from its repository, run

```
  gmake -f Makefile.bootstrap
```

at first. This will run autoconf and automake in order to generate the build environment. The LIBUSEFUL build environment is known to work with

  autoconf 2.59
  automake 1.8.4 and
  libtool 1.5.18

### Configuration, Build and Installation

Please refer to the procedures described in file INSTALL in the top-level directory of this software distribution once bootstrapping has been done as described before.
