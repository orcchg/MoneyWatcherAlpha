QT += widgets

INCLUDEPATH = $$[QT_SRC_DIR]/include

HEADERS     = $$[QT_SRC_DIR]/main/include/app_dialog.h

SOURCES     = $$[QT_SRC_DIR]/main/app_dialog.cc \
			  $$[QT_SRC_DIR]/main/main.cc
			  
LIBS += $$[QT_SRC_LIB_DIR]/libmoneywatcher.so \
        $$[QT_SQLITE3_LIB_DIR]/libsqlite3formw.so
        
QMAKE_CXXFLAGS += -std=c++11 -fPIC -I$$[QT_BUILD_DIR]
