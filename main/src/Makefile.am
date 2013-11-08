#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libmwengineapp.la

#######################################
# Build information for each library

# Sources for libmwengineapp
libmwengineapp_la_SOURCES = engine.cc

# Linker options libmwengineapp
libmwengineapp_LDFLAGS =

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmwengineapp_la_CPPFLAGS = -std=c++11 \
                             -fPIC \
                             -I$(top_srcdir)/include \
                             -I$(top_srcdir)/sqlite3 \
                             -I$(top_srcdir)/main/include

libmwengineapp_la_LIBADD = $(top_builddir)/src/libmoneywatcher.la \
					       $(top_builddir)/sqlite3/libsqlite3formw.la


#######################################
# Qt make
all:
	$(QMAKE) -set QT_APP_DIR $(top_srcdir)
	$(AM_V_GEN)$(QMAKE) $(top_srcdir)/main/src/dialog.pro
