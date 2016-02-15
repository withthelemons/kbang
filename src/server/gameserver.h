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
#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QtCore>
#include "parser/parser.h"
#include "parser/parserstructs.h"
#include "util.h"

class Game;
class Client;
class TcpServer;
class QXmlStreamWriter;
class CardFactory;

/**
 * The GameServer class is a singleton class that represent the state of the whole
 * server. It holds all the clients and games created on the server, as well as some
 * attributes of the server such as the server name, description, etc.
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class GameServer: public QObject, private NonCopyable
{
    Q_OBJECT;
public:
    /**
     * This method returns reference to the GameServer (singleton) instance.
     */
    static GameServer&  instance();

    ServerInfoData      serverInfo() const;
    QString             version() const { return m_version; }
    void                setVersion(QString version);


    /**
     * Creates new game and returns a pointer to it.
     */
    Game*               createGame(const CreateGameData&);
    void                removeGame(Game*);

    /**
     * Returns a list of games (Game instances).
     */
    QList<Game*>        gameList();

    /**
     * Returns a pointer to the Game with given id.
     */
    Game*               game(int id);

    /**
     * Returns a list of clients.
     */
    QList<Client*>      clientList();

    /**
     * Returns a pointer to the Client with given id.
     */
    Client*             client(int id);

    CardFactory* cardFactory() const { return mp_cardFactory; }

    /**
     * Tells the GameServer to listen for incoming connections.
     * @returns false in case of error, otherwise true
     */
    bool                listen();

    /**
     * Prepares GameServer for exiting.
     */
    void                exit();

public slots:
    /**
     * If there is a pending incoming connection, creates a new
     * client that is serving that connection.
     */
    void                createClient();

    /**
     * Removes the client from the clients list.
     */
    void                deleteClient(int clientId);

private:
    GameServer();
    ~GameServer();

private:
    static GameServer*       sm_instance;
    QString                 m_version;
    QHash<int, Game*>   m_games;
    QHash<int, Client*>      m_clients;

    int m_nextClientId;
    int m_nextGameId;
    int m_maxClientCount; // is this necessary?
    ServerInfoData m_serverInfoData;
    TcpServer* mp_tcpServer;
    CardFactory* mp_cardFactory;
};

#endif
