#
# Target to build
#

.SILENT:

lib_LTLIBRARIES 		= libuseful.la


#
# Source files of the target
#

libuseful_la_SOURCES		= auxfuncs.cc auxfuncs.h \
				  debugmacros.cc debugmacros.h \
				  exceptions.cc exceptions.h \
				  indentedostream.cc indentedostream.h \
				  io.cc io.h \
				  useful.h \
				  stringtools.cc stringtools.h

usefulincludedir		= ${includedir}/libuseful

usefulinclude_HEADERS		= auxfuncs.h \
				  debugmacros.h \
				  exceptions.h \
				  indentedostream.h \
				  io.h \
				  useful.h \
				  stringtools.h


#
# Compiler and linker flags
#

libuseful_la_LDFLAGS		= -avoid-version
