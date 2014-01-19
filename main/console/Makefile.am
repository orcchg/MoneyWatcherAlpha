SUBDIRS = src

noinst_PROGRAMS = mw_console

mw_console_SOURCES = main.cc

mw_console_LDFLAGS =

include_HEADERS = src/engine.h

mw_console_CPPFLAGS = -std=c++11 \
                      -fPIC \
                      -I$(top_srcdir)/include

mw_console_LDADD = $(top_builddir)/src/libmoneywatcher.la \
				   $(top_builddir)/sqlite3/libsqlite3formw.la \
				   $(top_builddir)/main/console/src/libmwengineconsoleapp.la
