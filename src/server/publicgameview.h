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

#ifndef PUBLICGAMEVIEW_H
#define PUBLICGAMEVIEW_H

#include "common.h"
#include "parser/parserstructs.h"

#include <QList>

class Game;
class PublicPlayerView;
class PlayingCard;

/**
 * The PublicGameView class provides an interface for quering public information
 * from games. The instances of this class are used by player controllers to get
 * the information about the "world around". Every game has exactly one PublicGameView
 * instance and is responsible for its lifetime.
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class PublicGameView {
    friend class Game;
private:
    /**
     * Constructs the PublicGameView instance. Instances can be constructed only
     * from the Game class, which is declared as friend class.
     */
    PublicGameView(Game* game);
    /**
     * Copying PublicGameView objects is not allowed.
     */
    PublicGameView(const PublicGameView&);

    /**
     * Assignment into PublicGameView instances is not allowed.
     */
    const PublicGameView& operator=(const PublicGameView&);

    ~PublicGameView();

public:
    /**
     * Returns the id of the game.
     */
    int id() const;

    /**
     * Returns name of the game.
     */
    QString name() const;

    /**
     * Returns description of the game.
     */
    inline QString description() const;

    /**
     * Returns count of players.
     */
    inline int playersCount() const;

    bool canBeStarted() const;

    GameInfoData    gameInfoData() const;
    GameContextData gameContextData() const;
    GameState       gameState() const;
    PlayingCard*    graveyardTop() const;

    QList<PlayingCard*> selection() const;

    /**
     * Returns the public list of players.
     */
    QList<PublicPlayerView*>  publicPlayerList() const;

    PublicPlayerView* publicPlayerView(int playerId) const;

    QList<PublicPlayerView*> neighbors(PublicPlayerView* source, int distance) const;

private:
    Game* mp_game;
};

#endif
