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

#ifndef GAMEEVENTHANDLER_H
#define GAMEEVENTHANDLER_H

#include <QString>
#include <QList>
#include "parser/parserstructs.h"


class PlayingCard;
class PublicPlayerView;
class PublicGameView;
class PlayerCtrl;

/**
 * Abstract class that defines the interface to notify back PlayerControllers
 * about events.
 *
 * \note This class could be renamed. Candidates are:
 *   * GameEventsHandler
 */
class GameEventListener
{
public:
    virtual bool isAI() = 0;

    /**
     * This method is created right after the Player is created.
     * The controlling client gets the PlayerCtrl instance that
     * is used to control player and can do some caching of persistent
     * members (for example PublicGameView, PrivatePlayerView objects).
     */
    virtual void onHandlerRegistered(const PublicGameView* publicGameView, PlayerCtrl* playerCtrl) = 0;
    virtual void onHandlerUnregistered() = 0;


    virtual void onGameStartabilityChanged(bool isStartable) = 0;

    /**
     * This method is called when someone sends a chat message.
     * \param player The publicPlayerView of sender.
     * \param playerName The name of the sender.
     * \param message The chat message.
     */
    virtual void onChatMessage(PublicPlayerView& publicPlayerView, const QString& message) = 0;

    virtual void onGameSync() = 0;

    virtual void onPlayerJoinedGame(PublicPlayerView&) = 0;

    virtual void onPlayerLeavedGame(PublicPlayerView&) = 0;

    virtual void onPlayerUpdated(PublicPlayerView&) = 0;

    virtual void onPlayerDied(PublicPlayerView&, PublicPlayerView* causedBy) = 0;

    virtual void onGameStarted() = 0;

    virtual void onGameFinished() = 0;

    /**
     * This method is called when a player draws a card. If the controlled player draws a card, the
     * card attribute points to that card, otherwise card is null.
     */
    virtual void onPlayerDrawFromDeck(PublicPlayerView&, QList<const PlayingCard*> cards, bool revealCards) = 0;

    virtual void onPlayerDrawFromGraveyard(PublicPlayerView&, const PlayingCard* card, const PlayingCard* nextCard) = 0;

    virtual void onPlayerDiscardCard(PublicPlayerView&, const PlayingCard* card, PocketType pocket) = 0;

    virtual void onPlayerPlayCard(PublicPlayerView&, const PlayingCard* card) = 0;

    virtual void onPlayerPlayCard(PublicPlayerView&, const PlayingCard* card, PublicPlayerView& target) = 0;

    virtual void onPlayerPlayCard(PublicPlayerView&, const PlayingCard* card, const PlayingCard* target, PublicPlayerView* targetPlayer) = 0;

    virtual void onPlayerPlayCardOnTable(PublicPlayerView&, const PlayingCard* card, PublicPlayerView& target) = 0;

    virtual void onPassTableCard(PublicPlayerView&, const PlayingCard* card, PublicPlayerView& targetPlayer) = 0;

    virtual void onPlayerRespondWithCard(PublicPlayerView&, const PlayingCard* card) = 0;

    virtual void onPlayerPass(PublicPlayerView&) = 0;

    virtual void onDrawIntoSelection(QList<const PlayingCard*> cards) = 0;

    virtual void onPlayerPickFromSelection(PublicPlayerView&, const PlayingCard* card) = 0;

    virtual void onUndrawFromSelection(const PlayingCard* card) = 0;

    virtual void onPlayerCheckDeck(PublicPlayerView&, const PlayingCard* checkedCard, const PlayingCard* causedBy, bool checkResult) = 0;

    virtual void onPlayerStealCard(PublicPlayerView&, PublicPlayerView& targetPlayer, PocketType pocketFrom, const PlayingCard* card) = 0;

    virtual void onCancelCard(PocketType pocketFrom, const PlayingCard* card, PublicPlayerView* targetPlayer, PublicPlayerView* causedBy) = 0;

    virtual void onGameContextChange(const GameContextData&) = 0;

    virtual void onLifePointsChange(PublicPlayerView&, int lifePoints, PublicPlayerView* causedBy) = 0;

    virtual void onDeckRegenerate() = 0;

    virtual void onPlayerUseAbility(PublicPlayerView&) = 0;

    virtual void onActionRequest(ActionRequestType requestType) = 0;

    virtual ~GameEventListener() {}
};


#endif // GameEventListener_H
