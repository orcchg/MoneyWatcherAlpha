#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libmwengineconsoleapp.la

#######################################
# Build information for each library

# Sources for libmwengineapp
libmwengineconsoleapp_la_SOURCES = engine.cc

# Linker options libmwengineapp
libmwengineconsoleapp_la_LDFLAGS =

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmwengineconsoleapp_la_CPPFLAGS = -std=c++11 \
		                            -fPIC \
		                            -I$(top_srcdir)/include \
		                            -I$(top_srcdir)/sqlite3 \
		                            -I$(top_srcdir)/main/console/include

libmwengineconsoleapp_la_LIBADD = $(top_builddir)/src/libmoneywatcher.la \
					       		  $(top_builddir)/sqlite3/libsqlite3formw.la
