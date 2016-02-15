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

#include "serverconnection.h"
#include "parser/parser.h"
#include "parser/queryget.h"

#include <QTcpSocket>

using namespace client;

ServerConnection::ServerConnection(QObject *parent)
 : QObject(parent), mp_parser(0)
{
    mp_tcpSocket = new QTcpSocket();
    connect(mp_tcpSocket, SIGNAL(connected()),
            this, SLOT(connected()));
    connect(mp_tcpSocket, SIGNAL(disconnected()),
            this, SLOT(disconnected()));
}


ServerConnection::~ServerConnection()
{
}

void ServerConnection::connectToServer(QString serverHost, int serverPort)
{
    if (mp_tcpSocket->state() == QAbstractSocket::UnconnectedState)
    {
        emit logMessage(tr("Connecting to <i>%1</i>...").arg(serverHost));
        mp_tcpSocket->connectToHost(serverHost, serverPort);
        m_serverHost = serverHost;
    }
}

void ServerConnection::disconnectFromServer()
{
    if (mp_parser == 0)
        return;
    if (mp_tcpSocket->state() != QAbstractSocket::UnconnectedState)
    {
        mp_parser->terminate();
    }
}

void ServerConnection::connected()
{
    emit statusChanged();
    emit logMessage(tr("Connected to <i>%1</i>.").arg(m_serverHost));
    mp_parser = new Parser(this, mp_tcpSocket);
    initializeParserConnections();
    mp_parser->initializeStream();

    QueryGet* query = queryGet();
    connect(query, SIGNAL(result(const ServerInfoData&)),
            this, SLOT(recievedServerInfo(const ServerInfoData&)));
    query->getServerInfo();
}

void ServerConnection::disconnected()
{
    if (mp_parser == 0)
        return;
    mp_parser->deleteLater();
    mp_parser = 0;
    emit statusChanged();
    emit logMessage(tr("Disconnected from <i>%1</i>.").arg(m_serverHost));
    m_serverHost = "";
}

void ServerConnection::recievedServerInfo(const ServerInfoData& serverInfo)
{
    m_serverName = serverInfo.name;
    m_serverDescription = serverInfo.description;
    emit statusChanged();
}

void ServerConnection::createGame(const CreateGameData& game, const CreatePlayerData& player)
{
    if (mp_parser == 0)
        return;
    emit logMessage(tr("Creating game \"%1\".").arg(game.name));
    mp_parser->actionCreateGame(game, player);
}

void ServerConnection::joinGame(int gameId, int playerId, const QString& gamePassword, const CreatePlayerData& player)
{
    if (mp_parser == 0)
        return;
    emit logMessage(tr("Joining game."));
    mp_parser->actionJoinGame(gameId, playerId, gamePassword, player);
}

void ServerConnection::leaveGame()
{
    if (mp_parser == 0)
        return;
    mp_parser->actionLeaveGame();
}

void ServerConnection::startGame()
{
    if (mp_parser == 0)
        return;
    mp_parser->actionStartGame();
}


void ServerConnection::sendChatMessage(const QString& message)
{
    if (mp_parser == 0)
        return;
    mp_parser->actionChatMessage(message);
}

void ServerConnection::drawCard()
{
    if (mp_parser == 0)
        return;
    mp_parser->actionDrawCard();
}

void ServerConnection::playCard(int cardId)
{
    if (mp_parser == 0)
        return;
    ActionPlayCardData actionPlayCardData;
    actionPlayCardData.playedCardId = cardId;
    actionPlayCardData.type = ActionPlayCardData::PLAYCARD_SIMPLE;
    mp_parser->actionPlayCard(actionPlayCardData);
}

void ServerConnection::playCardWithPlayer(int cardId, int playerId)
{
    if (mp_parser == 0)
        return;
    ActionPlayCardData actionPlayCardData;
    actionPlayCardData.playedCardId = cardId;
    actionPlayCardData.type = ActionPlayCardData::PLAYCARD_PLAYER;
    actionPlayCardData.targetPlayerId = playerId;
    mp_parser->actionPlayCard(actionPlayCardData);
}

void ServerConnection::playCardWithCard(int cardId, int otherCardId)
{
    if (mp_parser == 0)
        return;
    ActionPlayCardData actionPlayCardData;
    actionPlayCardData.playedCardId = cardId;
    actionPlayCardData.type = ActionPlayCardData::PLAYCARD_CARD;
    actionPlayCardData.targetCardId = otherCardId;
    mp_parser->actionPlayCard(actionPlayCardData);
}


void ServerConnection::useAbility()
{
    if (mp_parser == 0)
        return;
    ActionUseAbilityData actionUseAbilityData;
    actionUseAbilityData.type = ActionUseAbilityData::TypeSimple;
    mp_parser->actionUseAbility(actionUseAbilityData);
}

void ServerConnection::useAbility(int playerId)
{
    if (mp_parser == 0)
        return;
    ActionUseAbilityData actionUseAbilityData;
    actionUseAbilityData.type = ActionUseAbilityData::TypePlayer;
    actionUseAbilityData.targetPlayerId = playerId;
    mp_parser->actionUseAbility(actionUseAbilityData);
}

void ServerConnection::useAbility(QList<int> cards)
{
    if (mp_parser == 0)
        return;
    ActionUseAbilityData actionUseAbilityData;
    actionUseAbilityData.type = ActionUseAbilityData::TypeCards;
    actionUseAbilityData.targetCardsId = cards;
    mp_parser->actionUseAbility(actionUseAbilityData);
}


void ServerConnection::endTurn()
{
    if (mp_parser == 0)
        return;
    mp_parser->actionEndTurn();
}

void ServerConnection::pass()
{
    if (mp_parser == 0)
        return;
    mp_parser->actionPass();
}

void ServerConnection::discardCard(int cardId)
{
    if (mp_parser == 0)
        return;
    mp_parser->actionDiscard(cardId);
}




QueryGet* ServerConnection::queryGet()
{
    Q_ASSERT(mp_parser != 0);
    return mp_parser->queryGet();
}
/*
void ServerConnection::recievedEventJoinGame(int gameId, const StructPlayer& player, bool other)
{
    if (!other)
    {
        m_gameId = gameId;
        emit statusChanged();
        emit logMessage(tr("You have entered the game."));
    }
    else
    {
        emit logMessage(tr("%1 has entered the game.").arg(player.name));
    }
}

void ServerConnection::recievedEventLeaveGame(int gameId, const StructPlayer& player, bool other)
{
    if (!other)
    {
        m_gameId = 0;
        emit statusChanged();
        emit logMessage(tr("You have left the game."));
    }
    else
    {
        emit logMessage(tr("%1 has left the game.").arg(player.name));
    }
}
*/

void ServerConnection::initializeParserConnections()
{
    connect(mp_parser, SIGNAL(sigEventEnterGameMode(int,QString,ClientType)),
            this,      SIGNAL(enterGameMode(int,QString,ClientType)));
    connect(mp_parser, SIGNAL(sigEventExitGameMode()),
            this,      SIGNAL(exitGameMode()));
    connect(mp_parser, SIGNAL(sigEventChatMessage(int, const QString&, const QString&)),
            this, SIGNAL(incomingChatMessage(int, const QString&, const QString&)));
    connect(mp_parser, SIGNAL(sigEventGameCanBeStarted(bool)),
            this, SIGNAL(gameCanBeStarted(bool)));
    connect(mp_parser, SIGNAL(incomingData(const QByteArray&)),
            this, SIGNAL(incomingData(const QByteArray&)));
    connect(mp_parser, SIGNAL(outgoingData(const QByteArray&)),
            this, SIGNAL(outgoingData(const QByteArray&)));
    connect(mp_parser, SIGNAL(sigEventCardMovement(const CardMovementData&)),
            this, SIGNAL(eventCardMovement(const CardMovementData&)));
}

bool ServerConnection::isConnected() const
{
    return (mp_parser != 0);
}

QString ServerConnection::serverName() const
{
    return m_serverName;
}

QString ServerConnection::hostName() const
{
    return m_serverHost;
}

QObject* ServerConnection::parser() const
{
    return mp_parser;
}




