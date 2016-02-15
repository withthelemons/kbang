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

#include <QImage>

#include "publicplayerview.h"
#include "player.h"
#include "playingcard.h"

PublicPlayerView::PublicPlayerView(Player* player): mp_player(player)
{
}

PublicPlayerView::~ PublicPlayerView()
{
}

int PublicPlayerView::id() const
{
    return this ? mp_player->id() : 0;
}

QString PublicPlayerView::name() const
{
    return mp_player->name();
}

bool PublicPlayerView::isCreator() const
{
    return mp_player->isCreator();
}

bool PublicPlayerView::isSheriff() const
{
    return (mp_player->role() == ROLE_SHERIFF);
}

bool PublicPlayerView::isAlive() const
{
    return mp_player->isAlive();
}

bool PublicPlayerView::isWinner() const
{
    return mp_player->isWinner();
}

int PublicPlayerView::lifePoints() const
{
    return mp_player->lifePoints();
}

int PublicPlayerView::maxLifePoints() const
{
    return mp_player->maxLifePoints();
}

int PublicPlayerView::handSize() const
{
    return mp_player->handSize();
}

CharacterType PublicPlayerView::character() const
{
    return mp_player->characterType();
}

/**
 * Returns the role of the player. In case
 * the player is still alive and is not sheriff,
 * the unknown role is returned.
 */
PlayerRole PublicPlayerView::role() const
{
    if (mp_player->isPublicRole())
        return mp_player->role();
    return ROLE_UNKNOWN;
}

QList<PlayingCard*> PublicPlayerView::table() const
{
    return mp_player->table();
}

PublicPlayerData PublicPlayerView::publicPlayerData() const
{
    PublicPlayerData res;
    res.id          = mp_player->id();
    res.name        = mp_player->name();
    res.character   = mp_player->characterType();
    res.lifePoints  = lifePoints();
    res.isSheriff   = isSheriff();
    res.handSize    = handSize();
    res.avatar      = mp_player->avatar();
    foreach (PlayingCard* card, table()) {
        res.table.append(card->cardData());
    }
    res.hasPassword = !mp_player->password().isEmpty();
    res.hasController = mp_player->gameEventListener() != 0;
    res.isAI = mp_player->isAI();
    res.isAlive = mp_player->isAlive();
    res.isWinner = mp_player->isWinner();
    res.role = role();
    return res;
}

PlayingCard* PublicPlayerView::cardFromTable(PlayingCardType cardType) const
{
    if (table().size() == 0) {
        return 0;
    }

    if (cardType == CARD_UNKNOWN) {
        return table()[qrand() % table().size()];
    }

    foreach (PlayingCard* card, table()) {
        if (card->type() == cardType)
            return card;
    }
    return 0;
}
