SUBDIRS = src

#######################################
# Qt make
all-am:
	$(QMAKE) -set QT_SRC_DIR $(top_srcdir)
	$(QMAKE) -set QT_BUILD_DIR $(top_builddir)
	$(QMAKE) -set QT_SRC_LIB_DIR $(top_builddir)/src/.libs/
	$(QMAKE) -set QT_SQLITE3_LIB_DIR $(top_builddir)/sqlite3/.libs/
	$(AM_V_GEN)$(QMAKE) $(top_srcdir)/main/dialog.pro
