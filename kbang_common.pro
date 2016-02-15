TEMPLATE = lib
UI_DIR = uics/common
MOC_DIR = mocs/common
OBJECTS_DIR = obj/common

CONFIG += staticlib release
#CONFIG += staticlib debug

QT += xml
greaterThan(QT_MAJOR_VERSION, 4): DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000
INCLUDEPATH += src/common
HEADERS += src/common/parser/ioproxy.h \
    src/common/parser/parser.h \
    src/common/parser/parserstructs.h \
    src/common/parser/queryget.h \
    src/common/parser/queryresult.h \
    src/common/parser/xmlnode.h \
    src/common/config.h \
    src/common/util.h \
    src/common/gameenums.h
SOURCES += src/common/parser/ioproxy.cpp \
    src/common/parser/parser.cpp \
    src/common/parser/parserstructs.cpp \
    src/common/parser/queryget.cpp \
    src/common/parser/queryresult.cpp \
    src/common/parser/xmlnode.cpp \
    src/common/config.cpp \
    src/common/util.cpp \
    src/common/gameenums.cpp
TARGET = lib/kbang_common
QMAKE_CXXFLAGS_DEBUG += -Wall
