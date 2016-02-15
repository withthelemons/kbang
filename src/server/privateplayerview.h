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
#ifndef PRIVATEPLAYERVIEW_H
#define PRIVATEPLAYERVIEW_H

#include <publicplayerview.h>

class PlayingCard;


/**
 *
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class PrivatePlayerView: public PublicPlayerView
{
friend class Player;
private:
    PrivatePlayerView(Player* player);
    virtual ~PrivatePlayerView();

public:
    /**
     * Returns the role of the player. The valid role
     * is returned even if the player is still alive
     * and is not sheriff.
     */
    virtual PlayerRole role() const;

    QString password() const;

    QList<PlayingCard* > hand() const;

    /**
     * Returns the number of cards of given type in player's hand.
     */
    int handSize(PlayingCardType) const;

    PrivatePlayerData privatePlayerData() const;

    PlayingCard* card(int cardId) const;


    // implement method cardsInHand
};

#endif
