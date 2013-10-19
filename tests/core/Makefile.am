noinst_PROGRAMS = core_tests_all

core_tests_all_SOURCES = test_all.cc

core_tests_all_LDFLAGS =

include_HEADERS = test_fixture.h test_include_all.h

core_tests_all_CPPFLAGS = -std=c++11 -fPIC -I$(top_srcdir)/include -I$(top_srcdir)/tests/google

core_tests_all_LDADD = $(top_builddir)/src/libmoneywatcher.la \
					   $(top_builddir)/sqlite3/libsqlite3formw.la \
					   $(top_builddir)/tests/google/lib_gtest.la