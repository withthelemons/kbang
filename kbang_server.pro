TEMPLATE = app
UI_DIR = uics/server
MOC_DIR = mocs/server
OBJECTS_DIR = obj/server
QT += network xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000

CONFIG += release

INCLUDEPATH += src/server src/common
DEPENDPATH += src/common

HEADERS += \ 
    src/server/player.h \
    src/server/gameserver.h \
    src/server/game.h \
    src/server/console.h \
    src/server/consolecommand.h \
    src/server/client.h \
    src/server/tcpserver.h \
    src/server/common.h \
    src/server/publicgameview.h \
    src/server/publicplayerview.h \
    src/server/privateplayerview.h \
    src/server/playingcard.h \
    src/server/reactioncard.h \
    src/server/tablecard.h \
    src/server/weaponcard.h \
    src/server/cardbang.h \
    src/server/cardmissed.h \
    src/server/cards.h \
    src/server/gamecycle.h \
    src/server/gameexceptions.h \
    src/server/gameinfo.h \
    src/server/gametable.h \
    src/server/playerctrl.h \
    src/server/gameeventlistener.h \
    src/server/voidai.h \
    src/server/cardbeer.h \
    src/server/cardhorse.h \
    src/server/cardfactory.h \
    src/server/carddrawcards.h \
    src/server/cardmultishoot.h \
    src/server/cardduel.h \
    src/server/cardgeneralstore.h \
    src/server/cardjail.h \
    src/server/reactionhandler.h \
    src/server/carddynamite.h \
    src/server/cardtaker.h \
    src/server/cardbarrel.h \
    src/server/gameeventmanager.h \
    src/server/gamelogger.h \
    src/server/characterbase.h \
    src/server/charactercalamityjanet.h \
    src/server/characterblackjack.h \
    src/server/characterlist.h \
    src/server/charactercassidygringo.h \
    src/server/charactersuzylafayette.h \
    src/server/characterpropertychange.h \
    src/server/characterjourdonnais.h \
    src/server/characterpedroramirez.h \
    src/server/characterjessejones.h \
    src/server/charactersidketchum.h \
    src/server/characterkitcarlson.h \
    src/server/playerreaper.h \
    src/server/charactervulturesam.h \
    src/server/checkdeckresulthandler.h \
    src/server/characterluckyduke.h

    
SOURCES += \
    src/server/main.cpp \
    src/server/player.cpp \
    src/server/gameserver.cpp \
    src/server/game.cpp \
    src/server/console.cpp \
    src/server/consolecommand.cpp \
    src/server/client.cpp \
    src/server/tcpserver.cpp \
    src/server/common.cpp \
    src/server/publicgameview.cpp \
    src/server/publicplayerview.cpp \
    src/server/privateplayerview.cpp \
    src/server/playingcard.cpp \
    src/server/reactioncard.cpp \
    src/server/tablecard.cpp \
    src/server/weaponcard.cpp \
    src/server/cardbang.cpp \
    src/server/cardmissed.cpp \
    src/server/gamecycle.cpp \
    src/server/gameinfo.cpp \
    src/server/gametable.cpp \
    src/server/playerctrl.cpp \
    src/server/voidai.cpp \
    src/server/gameeventlistener.cpp \
    src/server/cardbeer.cpp \
    src/server/cardhorse.cpp \
    src/server/cardfactory.cpp \
    src/server/carddrawcards.cpp \
    src/server/cardmultishoot.cpp \
    src/server/cardduel.cpp \
    src/server/cardgeneralstore.cpp \
    src/server/cardjail.cpp \
    src/server/reactionhandler.cpp \
    src/server/carddynamite.cpp \
    src/server/cardtaker.cpp \
    src/server/cardbarrel.cpp \
    src/server/gameeventmanager.cpp \
    src/server/gamelogger.cpp \
    src/server/characterbase.cpp \
    src/server/charactercalamityjanet.cpp \
    src/server/characterblackjack.cpp \
    src/server/characterlist.cpp \
    src/server/charactercassidygringo.cpp \
    src/server/charactersuzylafayette.cpp \
    src/server/characterpropertychange.cpp \
    src/server/characterjourdonnais.cpp \
    src/server/characterpedroramirez.cpp \
    src/server/characterjessejones.cpp \
    src/server/charactersidketchum.cpp \
    src/server/characterkitcarlson.cpp \
    src/server/playerreaper.cpp \
    src/server/charactervulturesam.cpp \
    src/server/characterluckyduke.cpp

unix {
    LIBPATH += lib
    TARGETDEPS += lib/libkbang_common.a
}

win32 {
    debug:LIBPATH += debug/lib
    release:LIBPATH += release/lib
}

LIBS += -lkbang_common

TARGET = kbang-server
QMAKE_CXXFLAGS_DEBUG += -Wall
