SUBDIRS = src

#######################################
# Qt make
all-am:
	$(QMAKE) -set QT_SRC_DIR $(top_srcdir)
	$(QMAKE) -set QT_BUILD_DIR $(top_builddir)
	$(AM_V_GEN)$(QMAKE) $(top_srcdir)/main/dialog.pro
