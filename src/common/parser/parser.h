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
#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QTimer>

#include "parserstructs.h"
#include "queryget.h"
#include "queryresult.h"

class IOProxy;
class QIODevice;
class QXmlStreamReader;
class QXmlStreamWriter;
class XmlNode;

/**
 * The Parser class provides for both client and server a way to communicate
 * with each other. An instance of this class is created above the socket to
 * the other side and then messages are sent to other side by calling methods.
 * As soon as a recognized message is recieved from other side, according signal
 * is emitted.
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class Parser: public QObject
{
Q_OBJECT
public:
    //Parser(QObject* parent);
    Parser(QObject* parent, QIODevice* socket);
    virtual ~Parser();

    typedef QString QueryId;

    bool isKeepAlive() const;
    void setKeepAlive(bool keepAlive);

public slots:
    void attachSocket(QIODevice* socket);
    void detachSocket();
    void terminate();
    void ping();
    void sendKeepAlive();

signals:
    void streamInitialized();
    void terminated();
    void incomingData(const QByteArray&);
    void outgoingData(const QByteArray&);

public:
    static QString protocolVersion();
signals:
    void pong(int latency);


    ////////////
    // CLIENT //
    ////////////
public:
    void initializeStream();
    QueryGet* queryGet();

    void actionCreateGame(const CreateGameData&, const CreatePlayerData&);
    void actionJoinGame(int gameId, int playerId, const QString& gamePassword, const CreatePlayerData&);
    void actionLeaveGame();
    void actionStartGame();
    void actionChatMessage(const QString& message);
    void actionDrawCard();
    void actionPlayCard(const ActionPlayCardData&);
    void actionUseAbility(const ActionUseAbilityData&);
    void actionEndTurn();
    void actionPass();
    void actionDiscard(int cardId);

signals:
    void sigEventEnterGameMode(int gameId, const QString& gameName, ClientType);
    void sigEventExitGameMode();
    void sigEventPlayerJoinedGame(const PublicPlayerData&);
    void sigEventPlayerLeavedGame(int playerId);
    void sigEventPlayerUpdate(const PublicPlayerData&);
    void sigEventGameCanBeStarted(bool canBeStarted);
    void sigEventGameStateChange(const GameState&);
    void sigEventGameContextChange(const GameContextData&);
    void sigEventGameSync(const GameSyncData&);
    void sigEventLifePointsChange(int playerId, int lifePoints);
    void sigEventPlayerDied(int playerId, PlayerRole role);
    void sigEventCardMovement(const CardMovementData&);
    void sigEventChatMessage(int senderId, const QString& senderName, const QString& message);
    void sigEventGameMessage(const GameMessage&);

    ////////////
    // SERVER //
    ////////////
public:
    void eventEnterGameMode(int gameId, const QString& gameName, ClientType);
    void eventExitGameMode();
    void eventPlayerJoinedGame(const PublicPlayerData&);
    void eventPlayerLeavedGame(int playerId);
    void eventPlayerUpdate(const PublicPlayerData&);
    void eventChatMessage(int senderId, const QString& senderName, const QString& message);
    void eventGameMessage(const GameMessage&);
    void eventGameStateChange(const GameState&);
    void eventGameContextChange(const GameContextData&);
    void eventGameSync(const GameSyncData&);
    void eventCardMovement(const CardMovementData&);
    void eventLifePointsChange(int playerId, int lifePoints);
    void eventPlayerDied(int playerId, PlayerRole role);
    void eventGameCanBeStarted(bool canBeStarted);

signals:
    void sigQueryServerInfo(const QueryResult&);
    void sigQueryGameInfoList(const QueryResult&);
    void sigQueryGameInfo(int gameId, const QueryResult&);

    void sigActionCreateGame(const CreateGameData&, const CreatePlayerData&);
    void sigActionJoinGame(int gameId, int playerId, const QString& gamePassword, const CreatePlayerData&);
    void sigActionLeaveGame();
    void sigActionStartGame();
    void sigActionChatMessage(const QString& message);
    void sigActionDrawCard();
    void sigActionPlayCard(const ActionPlayCardData&);
    void sigActionUseAbility(const ActionUseAbilityData&);
    void sigActionEndTurn();
    void sigActionPass();
    void sigActionDiscard(int cardId);


private slots:
    void readData();
    void writeData(const QByteArray&);

private:
    void stateStart();
    void stateReady();
    void stateStanza();
    void processStanza();
    void sendInitialization();
    void sendTermination();
    void eventStart();
    void eventEnd();
    void actionStart();
    void actionEnd();
    void streamError();

private:
    typedef enum {
        S_Start = 0,
        S_Ready,
        S_Stanza,
        S_Terminated,
        S_Error
    } ReaderState;
    IOProxy*          mp_ioProxy;
    QIODevice*        mp_socket;
    QXmlStreamReader* mp_streamReader;
    QXmlStreamWriter* mp_streamWriter;
    bool              m_streamInitialized;
    ReaderState       m_readerState;
    int               m_readerDepth;
    XmlNode*          mp_parsedStanza;
    XmlNode*          mp_parsedXmlElement;
    QHash<QString, QueryGet*> m_getQueries;
    QueryGet*         mp_queryGet;
    QTimer            m_keepAliveTimer;
};

#endif
