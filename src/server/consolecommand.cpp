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

#include <QTextStream>
#include <QStringList>
#include <QTcpSocket>

#include "consolecommand.h"
#include "console.h"
#include "gameserver.h"
#include "game.h"
#include "gameinfo.h"
#include "player.h"
#include "client.h"

#include "voidai.h"



#define CMDDEF(name) bool name(const QStringList& args, Console& console)
#define CMDREG(name, function) cmdHash.insert(name, function)
#define CMDALIAS(name, alias) cmdHash.insert(name, cmdHash[alias])
#define CMD_ASSERT_ARG_CNT(cnt) if (args.size() != cnt) { console.cout() << QString("Bad arguments count, expected %1.").arg(cnt); return 1; }



QHash<QString, ConsoleCmd*> cmdHash;


ConsoleCmd* console_get_command(const QString& cmdName)
{
    if (cmdHash.contains(cmdName))
    {
        return cmdHash.value(cmdName);
    }
    return 0;
}

CMDDEF(console_help)
{
    (void) args;
    console.cout() << "Help" << endl;
    return 1;
}

CMDDEF(console_quit)
{
    (void) args;
    (void) console;
    console.disable();
    GameServer::instance().exit();
    return 1;
}

CMDDEF(console_list_games)
{
    Q_UNUSED(args);
    foreach(Game* game, GameServer::instance().gameList()) {
        console.cout() << "--------------------------------------------------------------------------------" << endl;
        console.cout() << " " << game->gameInfo().name() << " (id: " << game->id() << ")" << endl;
        console.cout() << "--------------------------------------------------------------------------------" << endl;
        foreach(Player* player, game->playerList()) {
            console.cout() << "  " << player->id() << ": " << player->name() << endl;
        }
        console.cout() << endl << endl;
    }
    return 0;
}

CMDDEF(console_list_clients)
{
    Q_UNUSED(args);
    foreach(Client* client, GameServer::instance().clientList()) {
        console.cout() << "  " << client->id() << ", " << client->address() << ", ";
        if (client->gameId() == 0) {
            console.cout() << "not in game";
        } else {
            Game* game = GameServer::instance().game(client->gameId());
            Q_ASSERT(game);
            console.cout() << QString("in %1 (%2)").arg(game->gameInfo().name()).arg(game->id());
            if (client->playerId()) {
                Player* player = game->player(client->playerId());
                console.cout() << QString(" as %1 (%2)").arg(player->name()).arg(player->id());
            }
        }
        console.cout() << endl;
    }
    return 0;
}

CMDDEF(console_dump_clients)
{
    Q_UNUSED(args);
    QString dumpFileName = QString("client-dump-%1.log").
                          arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QFile dumpFile(dumpFileName);
    dumpFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream out(&dumpFile);

    out.setPadChar('*');
    out << qSetFieldWidth(80) << center << " KBang Server: client dump " <<
            reset << endl;
    out.setPadChar('*');
    out << qSetFieldWidth(80) << center << QDateTime::currentDateTime().
            toString("yyyy/MM/dd hh:mm:ss") << reset << endl << endl;

    out.setPadChar('-');
    out << qSetFieldWidth(80) << "";
    out << reset << endl;

    out << "| " << qSetFieldWidth(4) << center << "Id" << reset << " | ";
    out << qSetFieldWidth(16) << center << "Ip Address" << reset << " | ";
    out << qSetFieldWidth(6) << center << "GameId" << reset << " | ";
    out << qSetFieldWidth(11) << center << "SocketState" << reset << " | ";
    out << qSetFieldWidth(27) << center << "SocketError" << reset << " |";
    out << endl;

    out.setPadChar('-');
    out << qSetFieldWidth(80) << "";
    out << reset << endl;

    foreach(Client* client, GameServer::instance().clientList()) {
        out << "| " << qSetFieldWidth(4) << center << client->id() << reset << " | ";
        out << qSetFieldWidth(16) << center << client->address() << reset << " | ";
        out << qSetFieldWidth(6) << center << client->gameId() << reset << " | ";
        out << qSetFieldWidth(11) << center << client->socket()->state() << reset << " | ";
        out << qSetFieldWidth(27) << center << QString("%1, %2").
                arg(client->socket()->error()).
                arg(client->socket()->errorString()) << reset << " |";
        out << reset << endl;
    }
    return 0;
}


CMDDEF(console_set_player_password)
{
    CMD_ASSERT_ARG_CNT(3);
    int gameId = args[0].toInt();
    int playerId = args[1].toInt();

    Game* game = GameServer::instance().game(gameId);
    if (game == 0) {
        console.cout() << QString("The game id %1 does not exist.").arg(gameId);
        return 1;
    }
    Player* player = game->player(playerId);
    if (player == 0) {
        console.cout() << QString("The player id %1 in game %2 does not exist.").arg(playerId).arg(game->gameInfo().name());
        return 1;
    }
    player->setPassword(args[2]);
    return 0;
}

void console_register_commands()
{
    CMDREG("help", &console_help);
    CMDREG("quit", &console_quit);
    CMDREG("list-games", &console_list_games);
    CMDREG("list-clients", &console_list_clients);
    CMDREG("dump-clients", &console_dump_clients);
    CMDREG("set-player-password", &console_set_player_password);


    CMDALIAS("exit", "quit");
}


/*
QMap<QString, ConsoleCmd*> ConsoleCmd::mp_commands;
ConsoleCmd* ConsoleCmd::mp_cmdNotFound = 0;

void ConsoleCmd::initialize()
{
    ConsoleCmdGet::initialize();
    ConsoleCmdNotFound::initialize();
    ConsoleCmdHelp::initialize();
    ConsoleCmdQuit::initialize();
}

QPair<QString, QStringList> ConsoleCmd::parse(QString command)
{
    QPair<QString, QStringList> aResult;
    QTextStream ss(&command, QIODevice::ReadOnly);
    QString buffer;
    ss >> aResult.first;
    while (ss.status() == QTextStream::Ok)
    {
        ss >> buffer;
        if (buffer.size()) aResult.second.push_back(buffer);
    }
    return aResult;
    //int i;
    //for(i=0;i<m_params.size();++i)
    //    QTextStream(stdout) << m_params[i] << endl;
}

QString ConsoleCmdNotFound::execute(const QString& cmdName, const QStringList &, const GameServer*) const
{
    if (cmdName.length() == 0) return QString("");
    return QString(cmdName + ": command not found\n");
}

ConsoleCmdHelp::ConsoleCmdHelp()
{
    m_cmdName = "help";
    m_help = "Displays this help";
}

QString ConsoleCmdHelp::execute(const QString&, const QStringList& attributes, const GameServer*) const
{
    const QMap<QString, ConsoleCmd*>& map = ConsoleCmd::commands();
    QMapIterator<QString, ConsoleCmd*> i(map);
    QStringList rows;
    while(i.hasNext())
    {
        i.next();
        rows.push_back(i.key()  + ": " + i.value()->help());
    }
    return rows.join("\n");

}

void ConsoleCmdHelp::initialize()
{
    ConsoleCmd::appendCommand("help", new ConsoleCmdHelp());
}

QString ConsoleCmdGet::execute(const QString&, const QStringList& params, const GameServer* gs) const
{
    if (params.count() == 0) return QString("use: get [VAR]");
    if (params[0] == "name") return gs->name();

    return QString("This is the get command.");
}

void ConsoleCmdHelp::quit()
{
    ConsoleCmd::appendCommand("quit", new ConsoleCmdQuit());
}
*/



