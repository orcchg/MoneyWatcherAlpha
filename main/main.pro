QT += widgets

HEADERS     = include/engine.h
SOURCES     = src/engine.cc \
              main.cc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/layouts/basiclayouts
INSTALLS += target
