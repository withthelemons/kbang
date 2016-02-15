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
#ifndef TABLECARD_H
#define TABLECARD_H

#include "playingcard.h"
#include "parser/parserstructs.h"

class Player;
class GameTable;

/**
 * The TableCard is the base class for all playing cards, that can be put in front of a
 * player. In Bang! it represents the blue cards. This class provides no extra public
 * methods, but declares two important methods: reigsterPlayer and unregisterPlayer.
 *
 * @author MacJariel <MacJariel@gmail.com>
 */
class TableCard: public PlayingCard
{
friend class GameTable;
Q_OBJECT;
public:
    TableCard(Game *game, int id, PlayingCardType, CardSuit, CardRank);
    virtual ~TableCard();

protected:
    /**
     * This method is called (by GameTable) when the card is
     * put on the player's table.
     */
    virtual void registerPlayer(Player*) {};

    /**
     * This method is called (by GameTable) when the card is
     * removed from the player's table.
     */
    virtual void unregisterPlayer(Player*) {};
};

#endif
