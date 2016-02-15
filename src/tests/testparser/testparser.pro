TEMPLATE = app

SOURCES += mocksocket.cpp \
           testparser.cpp
HEADERS += mocksocket.h \
           testparser.h
CONFIG += qtestlib

QT += xml

INCLUDEPATH += ../../common

LIBS += ../../common/libcommon.a

TARGETDEPS += ../../common/libcommon.a

