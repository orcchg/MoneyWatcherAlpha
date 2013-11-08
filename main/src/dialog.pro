QT += widgets

HEADERS     = $$[QT_APP_DIR]/main/src/app_dialog.h
SOURCES     = $$[QT_APP_DIR]/main/src/app_dialog.cc \
			  $$[QT_APP_DIR]/main/src/main.cc

QMAKE_CXXFLAGS += -std=c++0x -fPIC
