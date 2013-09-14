#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libsqlite3formw.la

#######################################
# Build information for each library

# Sources for libmoneywatcher
libsqlite3formw_la_SOURCES = sqlite3.c

# Linker options libTestProgram
libsqlite3formw_la_LDFLAGS = 

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
include_HEADERS = sqlite3.h

libsqlite3formw_la_CPPFLAGS = -I$(top_srcdir)/sqlite3

