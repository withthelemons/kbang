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
#ifndef PLAYINGCARD_H
#define PLAYINGCARD_H

#include <QObject>
#include "parser/parserstructs.h"

class Game;
class Player;
class GameTable;
class GameCycle;

/**
 * The PlayingCard is the base class for all playing cards. The each playing
 * card has the following attributes:
 *  * id   - the unique identifactor of the card
 *  * type - e.g. Bang!, Missed, ...
 *  * suit - e.g. Diamonds, Spades, ...
 *  * number - e.g. 7, K, A
 *
 * Moreover, every card also rembembers where it's located. This is sufficiently
 * represented by storing the owner and the pocket. The class that moves cards (the
 * GameTable class) is responsible to maintain these values correct.
 *
 * Despite the differences between playing cards in Bang, every card can be played
 * somehow. By playing it's meant taking the card from the player's hand and putting it
 * to the discard pile, in front of the player, in front of another player, etc. In some
 * cases it's necessery to add some extra information while playing the card. It can be
 * the another card or another player. For this action the PlayingCard provides the
 * "play" interface:
 *  * virtual void play();
 *  * virtual void play(Player* targetPlayer);
 *  * virtual void play(PlayingCard* targetCard);
 *
 * Not every card (in fact none card) has to support all the methods. In case of using improper
 * method, the BadUsageException should be returned.
 *
 * @author MacJariel <MacJariel@gmail.com>
 */
class PlayingCard: public QObject
{
friend class GameTable;
Q_OBJECT
public:
    PlayingCard(Game *game, int id, PlayingCardType, CardSuit, CardRank);
    virtual ~PlayingCard();

    inline int              id()        const { return m_id;        }
    inline PlayingCardType  type()      const { return m_type;      }
    inline CardSuit         suit()      const { return m_suit;      }
    inline CardRank         rank()      const { return m_rank;      }
    inline Player*          owner()     const { return mp_owner;    }
    inline PocketType       pocket()    const { return m_pocket;    }
    inline bool             isVirtual() const { return m_isVirtual; }
    inline PlayingCard*     master()    const { Q_ASSERT(mp_master != 0); return mp_master;   }

    CardData cardData() const;

    virtual void play();
    virtual void play(Player* targetPlayer);
    virtual void play(PlayingCard* targetCard);

    void assertInHand() const;
    void assertOnTable() const;

    void setVirtual(PlayingCard* master);
    void setVirtual(Player* owner, PocketType pocket);

protected:
    Game*       game()      const { return mp_game; }
    GameTable*  gameTable() const;
    GameCycle*  gameCycle() const;


    void setType(PlayingCardType type);

private:
    inline void setOwner(Player *owner)             { mp_owner = owner; }
    inline void setPocket(const PocketType& pocket) { m_pocket = pocket; }

private:
    int             m_id;
    PlayingCardType m_type;
    CardSuit        m_suit;
    CardRank        m_rank;
    Player*         mp_owner;
    PocketType      m_pocket;
    Game*           mp_game;
    bool            m_isVirtual;
    PlayingCard*    mp_master;
};

#endif
