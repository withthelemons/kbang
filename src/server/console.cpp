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
#include "console.h"
#include "consolecommand.h"
#include "gameserver.h"
#include <QIODevice>

Console::Console(GameServer* gameServer, FILE* in, FILE* out):
        QThread(gameServer),
        m_isEnabled(1),
        m_cin(in, QIODevice::ReadOnly),
        m_cout(out, QIODevice::WriteOnly),
        mp_gameServer(gameServer)
{
}

void Console::disable()
{
    m_isEnabled = 0;
}

Console::~Console()
{
    terminate();
    wait(2000);
}

void Console::run()
{
    init();
    doLoop();
}

void Console::init()
{
    console_register_commands();
    m_cout << QString("Welcome to KBang Server version %1.").arg(mp_gameServer->version()) << endl;
    Console::msleep(500);
}

void Console::doLoop()
{
    while (m_isEnabled) {
        writePrompt();
        QString line = m_cin.readLine();
        if (!line.isNull()) {
            execLine(line);
        } else {
            break;
        }
    }
}

/**
 *
 * @param cmdString
 */
void Console::execLine(QString cmdString)
{
    QString cmdName;
    QStringList cmdArgs;
    QTextStream ss(&cmdString, QIODevice::ReadOnly);
    QString buffer;
    ss >> cmdName;
    while (ss.status() == QTextStream::Ok)
    {
        ss >> buffer;
        if (buffer.size()) cmdArgs.push_back(buffer);
    }
    ConsoleCmd* cmd = console_get_command(cmdName);
    if (!cmd)
    {
        m_cout << cmdName << ": bad command" << endl;
        return;
    }
    (*cmd)(cmdArgs, *this); 
}

void Console::writePrompt()
{
    m_cout << "> " << flush;
}

