QT += widgets

HEADERS     = $$[QT_APP_DIR]/main/app_dialog.h
SOURCES     = $$[QT_APP_DIR]/main/app_dialog.cc \
			  $$[QT_APP_DIR]/main/main.cc

QMAKE_CXXFLAGS += -std=c++11 -fPIC
