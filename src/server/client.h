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
#ifndef CLIENT_H
#define CLIENT_H

#include "parser/parser.h"
#include "playerctrl.h"
#include "gameeventlistener.h"

#include <QObject>
#include <QPointer>

class QTcpSocket;


/**
 * The client class provides a adaptor between the parser and the player
 * control. The remote client sends requests that are handled and provided
 * by the Parser class and this class uses these requests to control the player
 * through the PlayerCtrl interface.
 *
 * Lifetime is controled by object itself.
 *
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class Client : public QObject, public GameEventListener
{
Q_OBJECT
public:
    /**
     * Constructs the client object.
     * \param parent The parent QObject.
     * \param id The client id.
     * \param socket The opened QTcpSocket to the client.
     */
    Client(QObject* parent, int id, QTcpSocket* socket);
    virtual ~Client();

    virtual bool isAI() { return 0; }

    /**
     * Returns the id of the client.
     * \note The id = 0 is reserved and cannot be used.
     */
    int id() const;

    /**
     * If the client is in a game, returns the id of the game,
     * otherwise returns 0.
     */
    int gameId() const;

    /**
     * If the client is a player in a game, returns the id of that
     * player, otherwise returns 0.
     */
    int playerId() const;

    QString address() const;

    QTcpSocket* socket();


public slots: // These slots are connected to parser
    void onActionCreateGame(const CreateGameData&, const CreatePlayerData&);
    void onActionJoinGame(int gameId, int playerId, QString gamePassword, const CreatePlayerData&);
    void onActionLeaveGame();
    void onActionStartGame();
    void onActionDrawCard();
    void onActionPlayCard(const ActionPlayCardData&);
    void onActionUseAbility(const ActionUseAbilityData&);
    void onActionEndTurn();
    void onActionPass();
    void onActionDiscard(int cardId);
    void onActionChatMessage(const QString&);

    void onQueryServerInfo(QueryResult result);
    void onQueryGameInfo(int gameId, QueryResult result);
    void onQueryGameInfoList(QueryResult result);

    void onParserTerminated();

public: /* The GameEventListener interface */
    virtual void onHandlerRegistered(const PublicGameView* publicGameView, PlayerCtrl* playerCtrl);
    virtual void onHandlerUnregistered();
    virtual void onGameStartabilityChanged(bool isStartable);

    virtual void onChatMessage(PublicPlayerView& publicPlayerView, const QString& message);
    virtual void onGameSync();
    virtual void onPlayerJoinedGame(PublicPlayerView&);
    virtual void onPlayerLeavedGame(PublicPlayerView&);
    virtual void onPlayerUpdated(PublicPlayerView&);
    virtual void onPlayerDied(PublicPlayerView&, PublicPlayerView* causedBy);
    virtual void onGameStarted();
    virtual void onGameFinished();

    virtual void onPlayerDrawFromDeck(PublicPlayerView&, QList<const PlayingCard*> cards, bool revealCards);
    virtual void onPlayerDrawFromGraveyard(PublicPlayerView&, const PlayingCard* card, const PlayingCard* nextCard);
    virtual void onPlayerDiscardCard(PublicPlayerView&, const PlayingCard* card, PocketType pocket);
    virtual void onPlayerPlayCard(PublicPlayerView&, const PlayingCard* card);
    virtual void onPlayerPlayCard(PublicPlayerView&, const PlayingCard* card, PublicPlayerView& target);
    virtual void onPlayerPlayCard(PublicPlayerView&, const PlayingCard* card, const PlayingCard* target, PublicPlayerView* targetPlayer);
    virtual void onPlayerPlayCardOnTable(PublicPlayerView&, const PlayingCard* card, PublicPlayerView& target);
    virtual void onPassTableCard(PublicPlayerView&, const PlayingCard* card, PublicPlayerView& targetPlayer);
    virtual void onPlayerRespondWithCard(PublicPlayerView&, const PlayingCard* card);
    virtual void onPlayerPass(PublicPlayerView&);
    virtual void onDrawIntoSelection(QList<const PlayingCard*> cards);
    virtual void onPlayerPickFromSelection(PublicPlayerView&, const PlayingCard* card);
    virtual void onUndrawFromSelection(const PlayingCard* card);
    virtual void onPlayerCheckDeck(PublicPlayerView&, const PlayingCard* checkedCard, const PlayingCard* causedBy, bool checkResult);
    virtual void onPlayerStealCard(PublicPlayerView&, PublicPlayerView& targetPlayer, PocketType pocketFrom, const PlayingCard* card);
    virtual void onCancelCard(PocketType pocketFrom, const PlayingCard* card, PublicPlayerView* targetPlayer, PublicPlayerView* p);
    virtual void onGameContextChange(const GameContextData&);
    virtual void onLifePointsChange(PublicPlayerView&, int lifePoints, PublicPlayerView* causedBy);
    virtual void onDeckRegenerate();
    virtual void onPlayerUseAbility(PublicPlayerView&);

    virtual void onActionRequest(ActionRequestType requestType);

signals:
    void disconnected(int clientId);


private:
    inline PublicPlayerView*    getPlayer(int playerId);
    inline PlayingCard*         getCard(int cardId);
    inline QList<PlayingCard*>  getCards(QList<int>);

    bool isInGame() const;




private:
    const int             m_id;
    Parser*               mp_parser;
    PlayerCtrl*           mp_playerCtrl;
    const PublicGameView* mp_publicGameView;
    QTcpSocket*           mp_socket;
};

#endif
