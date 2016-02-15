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
#include "privateplayerview.h"
#include "player.h"
#include "playingcard.h"
#include "gametable.h"
#include "game.h"

PrivatePlayerView::PrivatePlayerView(Player* player)
 : PublicPlayerView(player)
{
}


PrivatePlayerView::~PrivatePlayerView()
{
}

PlayerRole PrivatePlayerView::role() const
{
    return mp_player->role();
}

QString PrivatePlayerView::password() const
{
    return mp_player->password();
}

QList<PlayingCard* > PrivatePlayerView::hand() const
{
    return mp_player->hand();
}

int PrivatePlayerView::handSize(PlayingCardType cardType) const
{
    int res = 0;
    foreach (PlayingCard* card, mp_player->hand()) {
        if (card->type() == cardType)
            res++;
    }
    return res;
}

PrivatePlayerData PrivatePlayerView::privatePlayerData() const
{
    PrivatePlayerData res;
    res.id          = id();
    res.role        = role();
    foreach (PlayingCard* card, hand()) {
        res.hand.append(card->cardData());
    }
    return res;
}

PlayingCard* PrivatePlayerView::card(int cardId) const
{
    PlayingCard* res = mp_player->game()->gameTable().card(cardId);
    if (res == 0 || res->owner() == mp_player)
        return res;
    if (res->pocket() == POCKET_HAND || res->pocket() == POCKET_DECK)
        return 0;
    return res;
}
