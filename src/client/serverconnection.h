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
#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include "gameenums.h"

#include <QObject>
#include <QList>

class QTcpSocket;
class QByteArray;
class CreatePlayerData;
class CreateGameData;
class ServerInfoData;;
class CardMovementData;
class Parser;
class QueryGet;

namespace client {

/**
 *	@author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class ServerConnection : public QObject
{
Q_OBJECT
public:
    ServerConnection(QObject *parent);
    virtual ~ServerConnection();

    QueryGet* queryGet();
    bool isConnected() const;
    QString serverName() const;
    QString hostName() const;

    QObject* parser() const;

public slots:
    void connectToServer(QString serverHost, int serverPort);
    void disconnectFromServer();

    void createGame(const CreateGameData&, const CreatePlayerData&);
    void joinGame(int gameId, int playerId, const QString& gamePassword, const CreatePlayerData&);
    void leaveGame();
    void startGame();
    void sendChatMessage(const QString& message);


    void drawCard();
    void playCard(int cardId);
    void playCardWithPlayer(int cardId, int playerId);
    void playCardWithCard(int cardId, int otherCardId);

    void useAbility();
    void useAbility(int playerId);
    void useAbility(QList<int> cards);

    void endTurn();
    void pass();
    void discardCard(int cardId);




private slots:
    void connected();
    void disconnected();

    void recievedServerInfo(const ServerInfoData&);


private:
    void initializeParserConnections();



private:
    QTcpSocket*         mp_tcpSocket;
    Parser*             mp_parser;

    QString             m_serverHost;
    QString             m_serverName;
    QString             m_serverDescription;

signals:
    void statusChanged();//bool connected, QString serverHost, QString serverName, QString serverDescription);
    void logMessage(QString message);

    void incomingData(const QByteArray&);
    void outgoingData(const QByteArray&);

    void incomingChatMessage(int senderId, const QString& senderName,const QString& message);

    void enterGameMode(int gameId, const QString& gameName, const ClientType&);
    void exitGameMode();
    void gameCanBeStarted(bool canBeStarted);
    void eventCardMovement(const CardMovementData&);


};
}
#endif
