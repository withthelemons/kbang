/***************************************************************************
 *   Copyright (C) 2008 by MacJariel                                       *
 *   echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil"                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "gameserver.h"
#include "console.h"
#include "common.h"
#include <QCoreApplication>
#include "util.h"

#include <cstdlib>
#include <time.h>

#include "parser/parserstructs.h"

#define KBANG_SERVER_VERSION_MAJOR 0
#define KBANG_SERVER_VERSION_MINOR 1
#define KBANG_SERVER_VERSION_REVISION 3

int main(int argc, char* argv[])
{
    time_t sec;
    time(&sec);
    qsrand((unsigned int) sec);
    QCoreApplication app(argc, argv);
    GameServer& server = GameServer::instance();
    server.setVersion(QString("%1.%2.%3").arg(KBANG_SERVER_VERSION_MAJOR).
                                          arg(KBANG_SERVER_VERSION_MINOR).
                                          arg(KBANG_SERVER_VERSION_REVISION));

    Console* console = new Console(&server, stdin, stdout);
    console->start();
    server.listen();
//    QObject::connect(&app, SIGNAL(aboutToQuit()),
//                     console, SLOT(terminate()));
    return app.exec();
}

