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
#include "weaponcard.h"
#include "gameexceptions.h"
#include "player.h"
#include "gametable.h"
#include "gamecycle.h"

WeaponCard::WeaponCard(Game *game, int id, int range, CardSuit suit, CardRank rank):
        TableCard(game, id, CARD_UNKNOWN, suit, rank),
        m_range(range)
{
    switch(m_range) {
    case 1:
        setType(CARD_VOLCANIC);
        break;
    case 2:
        setType(CARD_SCHOFIELD);
        break;
    case 3:
        setType(CARD_REMINGTON);
        break;
    case 4:
        setType(CARD_CARABINE);
        break;
    case 5:
        setType(CARD_WINCHESTER);
        break;
    }
}

WeaponCard::~WeaponCard()
{
}

void WeaponCard::play()
{
    gameCycle()->assertTurn();

    if (this->pocket() != POCKET_HAND)
        throw BadUsageException();

    foreach(PlayingCard* card, owner()->table()) {
        WeaponCard* weaponCard = qobject_cast<WeaponCard*>(card);
        if (weaponCard == 0)
            continue;
        if (weaponCard->type() == type())
            throw TwoSameOnTableException();
        gameTable()->playerDiscardCard(weaponCard);
    }
    gameTable()->playerPlayCardOnTable(this);
}

void WeaponCard::registerPlayer(Player* player)
{
    player->setWeaponRange(m_range);
    if (m_range == 1) {
        player->modifyUnlimitedBangs(1);
    }
}

void WeaponCard::unregisterPlayer(Player* player)
{
    player->setWeaponRange(1);
    if (m_range == 1) {
        player->modifyUnlimitedBangs(-1);
    }
}


