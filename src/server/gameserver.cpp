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
#include "game.h"
#include "tcpserver.h"
#include "client.h"
#include "common.h"

#include "gameinfo.h"
#include "cardfactory.h"
#include "config.h"

#include <QTcpSocket>
#include <QXmlStreamWriter>

#ifdef Q_OS_UNIX
#include <signal.h>
#endif

GameServer* GameServer::sm_instance = 0;

GameServer::GameServer():
    QObject(0),
    m_nextClientId(0),
    m_nextGameId(0),
    m_maxClientCount(1)
{
#ifdef Q_OS_UNIX
    // ignore broken-pipe signal eventually caused by sockets
    signal(SIGPIPE, SIG_IGN);
#endif

    mp_tcpServer = new TcpServer(this);
    m_serverInfoData.name =
            Config::instance().readString("network", "server_name");
    m_serverInfoData.description =
            Config::instance().readString("network", "server_description");
    mp_cardFactory = new CardFactory();
}

GameServer::~GameServer()
{
    delete mp_cardFactory;
    QCoreApplication::quit();
}

GameServer& GameServer::instance()
{
    if (!sm_instance) sm_instance = new GameServer();
    return *sm_instance;
}

ServerInfoData GameServer::serverInfo() const
{
    return m_serverInfoData;
}

void GameServer::setVersion(QString version)
{
    m_version = version;
}

Game* GameServer::createGame(const CreateGameData& createGameData)
{
    while (!m_nextGameId || m_games.contains(m_nextGameId)) m_nextGameId++;
    int gameId = m_nextGameId++;
    Game* newGame = new Game(this, gameId, createGameData);
    m_games[gameId] = newGame;
    return newGame;
}

void GameServer::removeGame(Game* game)
{
    m_games.remove(game->id());
    game->deleteLater();
}

QList<Game*> GameServer::gameList()
{
    return m_games.values();
}

Game* GameServer::game(int id)
{
    if (m_games.contains(id)) return m_games[id];
    return 0;
}

QList<Client*> GameServer::clientList()
{
    return m_clients.values();
}

Client* GameServer::client(int id)
{
    if (m_clients.contains(id)) return m_clients[id];
    return 0;
}



bool GameServer::listen()
{
    if (!mp_tcpServer->isListening() && !mp_tcpServer->listen())
    {
        qCritical("ERROR: Unable to listen on %s:%d", mp_tcpServer->hostAddressString().toAscii().data(),
                  mp_tcpServer->port());
        return 0;
    }
    else
    {
        qWarning("Listening on %s:%d", mp_tcpServer->hostAddressString().toAscii().data(), mp_tcpServer->port());
    }
    return 1;
}

void GameServer::exit()
{
    deleteLater();
}

void GameServer::createClient()
{
    if (!mp_tcpServer->hasPendingConnections()) return;
    while (!m_nextClientId || m_clients.contains(m_nextClientId)) m_nextClientId++;
    int clientId = m_nextClientId++;
    m_clients.insert(clientId, 0);
    QTcpSocket* socket = mp_tcpServer->nextPendingConnection();
    m_clients[clientId] = new Client(this, clientId, socket);
    connect(m_clients[clientId], SIGNAL(disconnected(int)),
            this, SLOT(deleteClient(int)));
}

void GameServer::deleteClient(int clientId)
{
    m_clients.remove(clientId);
}

/*
void GameServer::queryGame(int gameId, QueryResult result)
{
    if (m_games.contains(gameId))
    {
        result.sendData(m_games[gameId]->gameInfo().structGame(), m_games[gameId]->structPlayerList());
    }
    else
    {
        //TODO: send error
    }
}

void GameServer::queryGameList(QueryResult result)
{
    StructGameList x;
    foreach(const Game* game, m_games.values())
    {
        x.append(game->gameInfo().structGame());
    }
    result.sendData(x);
}
*/

