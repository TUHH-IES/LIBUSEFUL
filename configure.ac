#
#
#  This script belongs to the
#
#    WCC Intermediate Representation (WIR) Framework
#
#  Permission to use this software is granted according to the license provided
#  with this software distribution.
#
#  Copyright 2007 - 2026, Heiko Falk.
#
#

#
# Default initialization stuff
#

AC_INIT([libuseful], [1.1])

AC_CONFIG_MACRO_DIRS([m4])

AC_CANONICAL_TARGET

AC_CONFIG_HEADERS(config_useful.h)
AC_CONFIG_SRCDIR([libuseful/useful.h])
AM_INIT_AUTOMAKE([foreign])

# Allow for very compact build output
AM_SILENT_RULES

ac_cv_cppflags=""
ac_cv_cxxflags=""
ac_cv_cflags=""
ac_cv_ldflags=""


#
# Determine some important paths about source and build tree
#

current_configure=$0
relative_srcdir=`dirname $0`
absolute_srcdir=`cd $relative_srcdir; pwd`


#
# Define special command line parameters
#

# CXXFLAGS and Release-Quality
AC_ARG_WITH(release-quality,
        AS_HELP_STRING([--with-release-quality],
               [set compiler flags to highest optimization level (default=no)]),
                ac_cv_release_quality="$withval", ac_cv_release_quality=no)

AX_CHECK_COMPILE_FLAG([-Og], [og="yes"],[og="no"])

if test "$ac_cv_release_quality" = yes; then
  if test -z "${ac_cv_cxxflags}"; then
    ac_cv_cxxflags="-O3"
  else
    ac_cv_cxxflags="-O3 ${ac_cv_cxxflags}"
  fi
  if test -z "${ac_cv_cflags}"; then
    ac_cv_cflags="-O3"
  else
    ac_cv_cflags="-O3 ${ac_cv_cflags}"
  fi
else
  if test -z "${ac_cv_cxxflags}"; then
    ac_cv_cxxflags="-g -Wall -Wextra"
  else
    ac_cv_cxxflags="-g -Wall -Wextra ${ac_cv_cxxflags}"
  fi
  if test -z "${ac_cv_cflags}"; then
    ac_cv_cflags="-g -Wall -Wextra"
  else
    ac_cv_cflags="-g -Wall -Wextra ${ac_cv_cflags}"
  fi
  if [[ "$og" == "yes" ]]; then
    ac_cv_cxxflags="-Og ${ac_cv_cxxflags}"
    ac_cv_cflags="-Og ${ac_cv_cflags}"
  else
    ac_cv_cxxflags="-O0 ${ac_cv_cxxflags}"
    ac_cv_cflags="-O0 ${ac_cv_cflags}"
  fi
fi


# Check if 64-bit build should be enabled.
AC_ARG_ENABLE(x64,
        AS_HELP_STRING([--enable-x64],
               [enable x64 build (default=yes)]),
                ac_cv_x64="$enableval", ac_cv_x64=yes)

if test "$ac_cv_x64" = yes -a `uname` = Linux -a `uname -m` = "x86_64"; then
  if test -z "${CXXFLAGS}"; then
    CXXFLAGS="-m64"
  else
    CXXFLAGS="-m64 ${CXXFLAGS}"
  fi
  if test -z "${CFLAGS}"; then
    CFLAGS="-m64"
  else
    CFLAGS="-m64 ${CFLAGS}"
  fi
else
  if test -z "${CXXFLAGS}"; then
    CXXFLAGS="-m32"
  else
    CXXFLAGS="-m32 ${CXXFLAGS}"
  fi
  if test -z "${CFLAGS}"; then
    CFLAGS="-m32"
  else
    CFLAGS="-m32 ${CFLAGS}"
  fi
  ac_cv_x64=no
fi

# Enabling memory debugging mode
AC_ARG_ENABLE(memory_debugging,
          AS_HELP_STRING([--enable-memory-debugging],
                 [enable GCC stack protection and libstdc++ debug mode and address sanitizer (default=no)]),
          ac_cv_memory_debugging="$enableval", ac_cv_memory_debugging=no)

if test "$ac_cv_memory_debugging" = "yes"; then
  if test -z "${ac_cv_cxxflags}"; then
    ac_cv_cxxflags="-fstack-protector-all -fsanitize=address -Wstack-protector -D_GLIBCXX_DEBUG"
  else
    ac_cv_cxxflags="-fstack-protector-all -fsanitize=address -Wstack-protector -D_GLIBCXX_DEBUG ${ac_cv_cxxflags}"
  fi
  if test -z "${ac_cv_cflags}"; then
    ac_cv_cflags="-fstack-protector-all -fsanitize=address -Wstack-protector -D_GLIBCXX_DEBUG"
    else
    ac_cv_cflags="-fstack-protector-all -fsanitize=address -Wstack-protector -D_GLIBCXX_DEBUG ${ac_cv_cflags}"
  fi
fi


 # Enabling memory protector mode
AC_ARG_ENABLE(memory_protection,
        AS_HELP_STRING([--enable-memory-protection],
               [enable GCC stack protection and libstdc++ debug mode (default=no)]),
                ac_cv_memory_protection="$enableval", ac_cv_memory_protection=no)

if test "$ac_cv_memory_protection" = "yes"; then
  if test -z "${ac_cv_cxxflags}"; then
    ac_cv_cxxflags="-fstack-protector-all -Wstack-protector -D_GLIBCXX_DEBUG"
  else
    ac_cv_cxxflags="-fstack-protector-all -Wstack-protector -D_GLIBCXX_DEBUG ${ac_cv_cxxflags}"
  fi
  if test -z "${ac_cv_cflags}"; then
    ac_cv_cflags="-fstack-protector-all -Wstack-protector -D_GLIBCXX_DEBUG"
  else
    ac_cv_cflags="-fstack-protector-all -Wstack-protector -D_GLIBCXX_DEBUG ${ac_cv_cflags}"
  fi
fi



# Configuration for LIBUSEFUL within WCC
AC_ARG_WITH(wcc,
        AS_HELP_STRING([--with-wcc],
               [Configure LIBUSEFUL for usage within WCC compiler (default=no)]),
                ac_cv_wcc="$withval", ac_cv_wcc=no)

if test "$ac_cv_wcc" = yes; then
  if test "$enable_shared" = no -a "$enable_static" = yes; then
    prefix=`echo $PWD`/libuseful/.libs
  else
    libdir=${prefix}/lib
    prefix=`echo $PWD`/libuseful/.libs
  fi
fi


# Activate building of DOXYGEN documentation
AC_ARG_WITH(doxygen-doc,
        AS_HELP_STRING([--with-doxygen-doc],
               [build doxygen documentation (default=yes)]),
                ac_cv_doxygen_doc="$withval", ac_cv_doxygen_doc=yes)

if test ! -f $absolute_srcdir/doc/doxygen.conf.in; then
  ac_cv_doxygen_doc=no
fi


#
# Checking the existence of build programs
#

# C++ compiler
AC_LANG([C++])
AC_PROG_CXX

# Set correct language standard - we use C++17
AX_CHECK_COMPILE_FLAG([-std=c++17], [ac_cv_cxxflags="${ac_cv_cxxflags} -std=c++17"], [AC_MSG_ERROR([Compiler does not support C++17 mode])] )

# Libtool
AC_DISABLE_STATIC
LT_INIT

if test "$enable_shared" = no -a "$enable_static" = yes; then
  libdir=`echo $PWD`/.libs

  if test `uname` = "SunOS" -a `uname -r` = "5.10" -a ${CXX} = "g++"; then
    THEGCC=`which $CXX`
    GCCBINDIR=`dirname $THEGCC`
    GCCLIBDIR=`dirname $GCCBINDIR`/lib

    if test ! -d ${PWD}/__lib; then
      mkdir ${PWD}/__lib
    fi
    if test ! -h ${PWD}/__lib/libstdc++.a; then
      $LN_S ${GCCLIBDIR}/libstdc++.a ${PWD}/__lib
    fi

    if test -z "${ac_cv_ldflags}"; then
      ac_cv_ldflags="-static -static-libgcc -L${PWD}/__lib"
    else
      ac_cv_ldflags="${ac_cv_ldflags} -static -static-libgcc -L${PWD}/__lib"
    fi
  else
    if test -z "${ac_cv_ldflags}"; then
      ac_cv_ldflags="-all-static"
    else
      ac_cv_ldflags="${ac_cv_ldflags} -all-static"
    fi
  fi

  AC_DEFINE(STATIC_LINKING, 1, [Define to 1 if you do a static build.])
fi

# Install
AC_PROG_INSTALL


#
# Checking the existence of miscellaneous programs
#

AC_CHECK_PROG(DIFF, diff, diff, notfound)
if test "$DIFF" = notfound; then
  AC_MSG_ERROR([*** diff not found - please install first ***])
fi

if test $ac_cv_doxygen_doc = yes; then
  AC_CHECK_PROG(DOT, dot, dot, notfound)
  if test $DOT = notfound; then
    HAVE_DOT=NO
  else
    HAVE_DOT=YES
  fi
  AC_SUBST(DOT,[${HAVE_DOT}])

  AC_CHECK_PROG(DOXYGEN, doxygen, doxygen, notfound)
  AM_CONDITIONAL(HAVE_DOXYGEN, test ! $DOXYGEN = notfound)
else
  HAVE_DOT=NO
  AC_SUBST(DOT,[${HAVE_DOT}])

  DOXYGEN=notfound
  AM_CONDITIONAL(HAVE_DOXYGEN, test ! $DOXYGEN = notfound)
fi

AC_CHECK_PROG(UNAME, uname, uname, notfound)
if test "$UNAME" = notfound; then
  AC_MSG_ERROR([*** uname not found - please install first ***])
fi


#
# Determine Build System
#

if test "${target_os}" = "mingw32msvc"; then
  AC_DEFINE(HAVE_WIN32, 1, [Define to 1 if you're cross-compiling for Windows systems.])
elif test `$UNAME` = SunOS; then
  AC_DEFINE(HAVE_SUNOS, 1, [Define to 1 if you're compiling on a Solaris system.])
elif test `$UNAME` = Linux; then
  AC_DEFINE(HAVE_LINUX, 1, [Define to 1 if you're compiling on a Linux system.])
fi

AM_CONDITIONAL([HAVE_WIN32], [test "${target_os}" = "mingw32msvc"])
AM_CONDITIONAL([HAVE_SUNOS], [test `$UNAME` = SunOS])
AM_CONDITIONAL([HAVE_LINUX], [test `$UNAME` = Linux])


# Extract some release numbers for doxygen
USEFUL_RELEASE="$(sed -n '/^release/h;$x;$p' ${absolute_srcdir}/Releases)"
AC_SUBST(USEFUL_RELEASE, [${USEFUL_RELEASE}])
AC_DEFINE_UNQUOTED([LIBUSEFUL_VERSION], ["${USEFUL_RELEASE}"], [Defines version of LIBUSEFUL module.])
AC_DEFINE([LIBUSEFUL_MODULE], "libuseful", [Defines name of LIBUSEFUL module.])

# Tools for generating documentation
AC_CHECK_PROG(PDFLATEX, pdflatex, pdflatex, notfound)
if test "$PDFLATEX" = notfound; then
  AC_MSG_ERROR([*** pdflatex not found - please install first ***])
fi


#
# Propagate configured parameters into Makefiles
#

AM_CONDITIONAL(WITH_DOXYGEN_DOC, test $ac_cv_doxygen_doc = yes)


#
# Prepare output
#

AC_SUBST(AM_CPPFLAGS, [${ac_cv_cppflags}])
AC_SUBST(AM_CFLAGS, [${ac_cv_cflags}])
AC_SUBST(AM_CXXFLAGS, [${ac_cv_cxxflags}])
AC_SUBST(AM_LDFLAGS, [${ac_cv_ldflags}])

AC_CONFIG_FILES([Makefile
                 libuseful/Makefile
                 doc/Makefile])

if test $ac_cv_doxygen_doc = yes; then
  if test ! ${DOXYGEN} = notfound; then
    AC_CONFIG_FILES([doc/doxygen.conf])
  fi
fi

AC_OUTPUT


#
# Generate status output
#

echo ""
echo "LIBUSEFUL Configuration:"
echo "  Install path:                 ${prefix}"
echo "  C++ Compiler flags:           ${ac_cv_cxxflags} ${CXXFLAGS}"
echo "  C Compiler flags:             ${ac_cv_cflags} ${CFLAGS}"
echo "  C Preprocessor flags:         ${ac_cv_cppflags} ${CPPFLAGS}"
echo "  Build shared libraries:       ${enable_shared}"
echo "  Build static libraries:       ${enable_static}"
echo "  Build 64-bit:                 ${ac_cv_x64}"
echo ""
echo "  Build DOXYGEN documentation:  ${ac_cv_doxygen_doc}"
echo ""
