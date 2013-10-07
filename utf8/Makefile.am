#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libutf8formw.la

#######################################
# Build information for each library

# Sources for libsqlite3formw
libutf8formw_la_SOURCES = utf8.c

# Linker options libsqlite3formw
libutf8formw_la_LDFLAGS = 

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
include_HEADERS = utf8.h

libutf8formw_la_CPPFLAGS = -I$(top_srcdir)/utf8

