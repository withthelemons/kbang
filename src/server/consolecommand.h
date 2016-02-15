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
#ifndef CONSOLECMD_H
#define CONSOLECMD_H

#include <QtCore>

class Console;

typedef bool (ConsoleCmd)(const QStringList&, Console&);

void console_register_commands();
ConsoleCmd* console_get_command(const QString&);



/*

class GameServer;
class ConsoleCmd
{
protected:
    ConsoleCmd() {}
    ConsoleCmd(const ConsoleCmd&) { qFatal("Trying to use ConsoleCmd(const ConsoleCmd&)"); }
    const ConsoleCmd& operator=(const ConsoleCmd&) { qFatal("Trying to use const ConsoleCmd& operator=(const ConsoleCmd&)"); return *this; }
    virtual ~ConsoleCmd() {}
    static void appendCommand(const QString& cmdName, ConsoleCmd* cmdptr) {
        mp_commands[cmdName] = cmdptr;
    }
    static void setCmdNotFound(ConsoleCmd* cmdptr)
    {
        mp_cmdNotFound = cmdptr;
    }

public:
    inline QString cmdName() const {
        return m_cmdName;
    }
    inline QString help() const {
        return m_help;
    }
    virtual QString execute(const QString&, const QStringList&, const GameServer*) const = 0;
    inline static const ConsoleCmd& instance(const QString& commandName) {
        ConsoleCmd* cmdptr = mp_commands[commandName];
        if (cmdptr)
        {
            return *cmdptr;
        }
        else
        {
            return *mp_cmdNotFound;
        }
    }
    inline static const QMap<QString, ConsoleCmd*>& commands()
    {
        return mp_commands;
    }
    static void initialize();
    static QPair<QString, QStringList> parse(QString);

protected:
    QString m_cmdName;
    QString m_help;

private:
    static QMap<QString, ConsoleCmd*> mp_commands;
    static ConsoleCmd* mp_cmdNotFound;
};

class ConsoleCmdNotFound: public ConsoleCmd
{
public:
    virtual QString execute(const QString&, const QStringList&, const GameServer*) const;
    inline static void initialize()
    {
        ConsoleCmd::setCmdNotFound(new ConsoleCmdNotFound());
    }
};

class ConsoleCmdGet: public ConsoleCmd
{
protected:
    ConsoleCmdGet()
    {
        m_cmdName = "get";
        m_help = "returns ....";
    }

public:
    virtual QString execute(const QString&, const QStringList& params, const GameServer* gs) const;
    inline static void initialize()
    {
        ConsoleCmd::appendCommand("get", new ConsoleCmdGet());
    }
};

class ConsoleCmdHelp: public ConsoleCmd
{
protected:
    ConsoleCmdHelp();
public:
    virtual QString execute(const QString&, const QStringList&, const GameServer*) const;
    static void initialize();
};

class ConsoleCmdQuit: public ConsoleCmd
{
protected:
    ConsoleCmdQuit();
public:
    virtual QString execute(const QString&, const QStringList&, const GameServer*) const;
    static void initialize();
};

*/




#endif
