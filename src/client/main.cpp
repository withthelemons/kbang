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

#include <QApplication>
#include <QDialog>
#include <cstdlib>
#include <time.h>
#include "mainwindow.h"
#include "config.h"

#ifdef Q_OS_UNIX
#include <signal.h>
#endif

#define KBANG_CLIENT_VERSION_MAJOR 0
#define KBANG_CLIENT_VERSION_MINOR 1
#define KBANG_CLIENT_VERSION_REVISION 3

using namespace client;

int main(int argc, char *argv[])
{
    time_t sec;
    time(&sec);
    qsrand((unsigned int) sec);

#ifdef Q_OS_UNIX
    // ignore broken-pipe signal eventually caused by sockets
    signal(SIGPIPE, SIG_IGN);
#endif

    QApplication app(argc, argv);
    app.setApplicationName("KBang Client");
    app.setApplicationVersion(QString("%1.%2.%3").arg(KBANG_CLIENT_VERSION_MAJOR).
                                                  arg(KBANG_CLIENT_VERSION_MINOR).
                                                  arg(KBANG_CLIENT_VERSION_REVISION));
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}

