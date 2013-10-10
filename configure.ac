dnl Process this file with autoconf to produce a configure script

AC_PREREQ(2.61)
AC_INIT(MoneyWatcherAlpha, 1.0)

dnl With support of SQLite 3.8.0.2, http://www.sqlite.org
AC_CONFIG_SRCDIR([sqlite3/sqlite3.c])

dnl Check for required programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_LIBTOOL
AC_PROG_MKDIR_P

AC_CANONICAL_SYSTEM
AC_CONFIG_HEADERS([config.h])

dnl Initialize and use automake
AM_INIT_AUTOMAKE([foreign])

AC_SYS_LARGEFILE

dnl Check for library functions that SQLite can optionally use
AC_CHECK_FUNCS([fdatasync usleep fullfsync localtime_r gmtime_r])
AC_FUNC_STRERROR_R

dnl This allows us specify individual liking flags for each target
AM_PROG_CC_C_O 

dnl Initialize Libtool
LT_INIT

dnl Check if Libtool is present
dnl Libtool is used for building share libraries 
AC_PROG_LIBTOOL

dnl Enable pkg-config      
PKG_PROG_PKG_CONFIG

dnl Link with Makefile.am in source directories
AC_CONFIG_FILES(Makefile
                sqlite3/Makefile
                src/Makefile
                main/Makefile
                include/Makefile)
                
AC_SUBST([BUILD_CFLAGS])
#-----------------------------------------------------------------------

dnl MoneyWatcher-specific options
#-----------------------------------------------------------------------
#   --enable-caching
#
AC_ARG_ENABLE(caching, [AS_HELP_STRING(
  [--enable-caching],
  [use RAM caching of recent data from the database (yes, no) [default=no]])],
  [case "${enableval}" in
    yes) caching=true ;;
    no) caching=false ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-caching]) ;;
   esac], [enable_caching=no])
if  test x"$enable_caching" != xno ; then
  # Enable data caching into RAM from database for more rapid access
  AC_DEFINE([__MW_ENABLE_CACHING__], [true], [Enabled caching from database])
  CFLAGS="$CFLAGS -DENABLE_CACHING"
  CXXFLAGS="$CXXFLAGS -DENABLE_CACHING"
  MW_VAR_ENABLE_CACHING=true
  AC_MSG_NOTICE([Enabled caching from database])
else
  AC_DEFINE([__MW_ENABLE_CACHING__], [false], [Disabled caching from database])
  MW_VAR_ENABLE_CACHING=false
  AC_MSG_NOTICE([Disabled caching from database])
fi
AM_CONDITIONAL([ENABLE_CACHING], [test x$enable_caching != xno])
AC_SUBST([MW_VAR_ENABLE_CACHING])
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
#   --enable-logging
#
AC_ARG_ENABLE(logging, [AS_HELP_STRING(
  [--enable-logging],
  [record log messages of core sources for debug (yes, no) [default=no]])],
  [case "${enableval}" in
    yes) logging=true ;;
    no) logging=false ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-logging]) ;;
   esac], [enable_logging=no])
AS_IF([test "x$enable_logging" != "xno"],
  [dnl Enable logging system for core sources debug
  CFLAGS="$CFLAGS -DENABLE_LOGGING -DMW_LOG_LEVEL=$MW_LOG_LEVEL"
  CXXFLAGS="$CXXFLAGS -DENABLE_LOGGING -DMW_LOG_LEVEL=$MW_LOG_LEVEL"
  AC_DEFINE([__MW_ENABLE_LOGGING__], [true], [Enabled logging in source code])
  MW_VAR_ENABLE_LOGGING=true
  AC_MSG_NOTICE([Enabled logging in source code])
])
AS_IF([test "x$enable_logging" == "xno"],
  [AC_DEFINE([__MW_ENABLE_LOGGING__], [false], [Disabled logging in source code])
  MW_VAR_ENABLE_LOGGING=false
  AC_MSG_NOTICE([Disabled logging in source code])
])
AM_CONDITIONAL([ENABLE_LOGGING], [test x$enable_logging != xno])
AM_COND_IF([ENABLE_LOGGING], [
  AC_DEFINE([__MW_LOG_LEVEL__], [MW_LOG_LEVEL], [Logging level])
  AC_MSG_NOTICE([Logging level set to $MW_LOG_LEVEL])
], [
  AC_DEFINE([__MW_LOG_LEVEL__], [-1], [Logging level])
])
AC_SUBST([MW_VAR_ENABLE_LOGGING])
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
#
#   --disable-tests
#
AC_ARG_ENABLE(tests, [AS_HELP_STRING(
  [--disable-tests],
  [disable building unit-tests (yes, no) [default=no]])],
  [], [disable_tests=no])
AS_IF([test "x$disable_tests" == "xyes"],
  [dnl Disable building unit-tests
  AC_MSG_NOTICE([Disabled building unit-tests])
])
AS_IF([test "x$disable_tests" != "xyes"],
  [AC_MSG_NOTICE([Enabled building unit-tests])
])
AM_CONDITIONAL([ENABLE_TESTS], [test "x$disable_tests" != "xyes"])
AM_COND_IF([ENABLE_TESTS], [dnl action-if-true
    MW_VAR_ENABLE_TESTS=true
	MW_VAR_SUBDIR_TESTS=tests
	AC_CONFIG_FILES(tests/Makefile
					tests/google/Makefile
					tests/core/Makefile)
	], [dnl action-if-false
	MW_VAR_ENABLE_TESTS=false
	MW_VAR_SUBDIR_TESTS=
])
AC_SUBST([MW_VAR_ENABLE_TESTS])
AC_SUBST([MW_VAR_SUBDIR_TESTS])
#-----------------------------------------------------------------------

dnl SQLite-specific options
#-----------------------------------------------------------------------
#   --enable-readline
#
AC_ARG_ENABLE(readline, [AS_HELP_STRING(
  [--enable-readline], 
  [use readline in shell tool (yes, no) [default=yes]])], 
  [], [enable_readline=yes])
if test x"$enable_readline" != xno ; then
  sLIBS=$LIBS
  LIBS=""
  AC_SEARCH_LIBS(tgetent, curses ncurses ncursesw, [], [])
  AC_SEARCH_LIBS(readline, readline, [], [enable_readline=no])
  AC_CHECK_FUNCS(readline, [], [])
  READLINE_LIBS=$LIBS
  LIBS=$sLIBS
fi
AC_SUBST(READLINE_LIBS)
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
#   --enable-threadsafe
#
AC_ARG_ENABLE(threadsafe, [AS_HELP_STRING(
  [--enable-threadsafe], [build a thread-safe library [default=yes]])], 
  [], [enable_threadsafe=yes])
THREADSAFE_FLAGS=-DSQLITE_THREADSAFE=0
if test x"$enable_threadsafe" != "xno"; then
  THREADSAFE_FLAGS="-D_REENTRANT=1 -DSQLITE_THREADSAFE=1"
  AC_SEARCH_LIBS(pthread_create, pthread)
fi
AC_SUBST(THREADSAFE_FLAGS)
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
#   --enable-dynamic-extensions
#
AC_ARG_ENABLE(dynamic-extensions, [AS_HELP_STRING(
  [--enable-dynamic-extensions], [support loadable extensions [default=yes]])], 
  [], [enable_dynamic_extensions=yes])
if test x"$enable_dynamic_extensions" != "xno"; then
  AC_SEARCH_LIBS(dlopen, dl)
else
  DYNAMIC_EXTENSION_FLAGS=-DSQLITE_OMIT_LOAD_EXTENSION=1
fi
AC_MSG_CHECKING([for whether to support dynamic extensions])
AC_MSG_RESULT($enable_dynamic_extensions)
AC_SUBST(DYNAMIC_EXTENSION_FLAGS)
#-----------------------------------------------------------------------

AC_CHECK_FUNCS(posix_fallocate)

dnl Check for Boost libraries
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([boost/foreach.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_LANG_POP([C++])

dnl Security xFLAGS
SECURITYFLAGS="-fstack-protector --param=ssp-buffer-size=4 -D_FORTIFY_SOURCE=2 -Wall -Wno-comment -Wformat-security"
CXXFLAGS="$CXXFLAGS $SECURITYFLAGS"
CFLAGS="$CFLAGS $SECURITYFLAGS"
LDFLAGS="$LDFLAGS -Wl,-z,relro -Wl,--hash-style=gnu"

dnl Compile with -rdynamic
LDFLAGS="$LDFLAGS -rdynamic"

dnl Do not link with unnecessary libraries
LDFLAGS="$LDFLAGS -Wl,--as-needed"

dnl Use the latest C++ standard 
CXXFLAGS="$CXXFLAGS -std=c++0x"

dnl Warn of unused precompiled headers
CXXFLAGS="$CXXFLAGS -Winvalid-pch"

dnl Use the modern C standard
CFLAGS="$CFLAGS -std=c99"

AC_OUTPUT
