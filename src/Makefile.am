#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libmoneywatcher.la

#######################################
# Build information for each library

# Sources for libmoneywatcher
libmoneywatcher_la_SOURCES = daily_table.cc datetime.cc entry.cc record.cc status.cc

# Linker options libTestProgram
libmoneywatcher_la_LDFLAGS =

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmoneywatcher_la_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/sqlite3

