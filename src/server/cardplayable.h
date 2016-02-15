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
#ifndef CARDPLAYABLE_H
#define CARDPLAYABLE_H

#include "playingcard.h"

/**
 * This is an abstract class for all cards that can be played, which means
 * that player can use this card by putting it on the top of the graveyard.
 * Some of these cards can also require to specify a target player or target
 * card.
 * @author MacJariel <MacJariel@gmail.com>
 */
class CardPlayable: public PlayingCard
{
Q_OBJECT
public:
    CardPlayable(Game* game, int id);
    virtual ~CardPlayable();

    virtual bool play();
    virtual bool play(Player* targetPlayer) = 0;
    virtual bool play(PlayingCard* targetCard) = 0;

    virtual void respondPass() = 0;
    virtual void respondCard(PlayingCard* targetCard) = 0;
};

#endif
