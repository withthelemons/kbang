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
#ifndef PUBLICPLAYERVIEW_H
#define PUBLICPLAYERVIEW_H

#include <QList>

#include "common.h"
#include "parser/parserstructs.h"

class Player;
class PlayingCard;

/**
 *
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
*/
class PublicPlayerView {
friend class Player;
protected:
    PublicPlayerView(Player* player);
    virtual ~PublicPlayerView();

public:
    int                  id()                const;
    QString              name()              const;
    bool                 isCreator()         const;
    bool                 isSheriff()         const;
    bool                 isAlive()           const;
    bool                 isWinner()          const;
    int                  lifePoints()        const;
    int                  maxLifePoints()     const;
    int                  handSize()          const;
    CharacterType        character()         const;
    virtual PlayerRole   role()              const;
    QList<PlayingCard*>  table()             const;
    PublicPlayerData     publicPlayerData()  const;

    /**
     * Returns a random card of given type from player's table.
     */
    PlayingCard* cardFromTable(PlayingCardType cardType = CARD_UNKNOWN) const;

protected:
    Player* mp_player;

private:
    PublicPlayerView(const PublicPlayerView&);
    PublicPlayerView& operator=(const PublicPlayerView&);
};

#endif
