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
#ifndef GAME_H
#define GAME_H

#include <QtCore>
#include "publicgameview.h"
#include "parser/parserstructs.h"

class Client;
class Player;
class GameServer;
class Game;
class PlayingCard;
class CardPlayable;
class GameEventListener;
class GameEventManager;
class GameLogger;

class PublicPlayerView;


class GameInfo;
class GameTable;
class GameCycle;
class BeerRescue;
class PlayerReaper;

/**
 * The Game class represents a bang game. Because there is a lot
 * of stuff to handle, the Game class delegates the problems
 * to its parts rather than managing everything itself.
 *
 * The list of tasks that are managed directly by this class includes
 * creating and removing players, starting and finishing games and
 * sending messages. The Game class also takes care about the list
 * of players in game (including its order) and provides "next player"
 * and "distance between players" capabilities. Finally it contains
 * the getters for its component objects.
 *
 * @author MacJariel <macjariel@users.sourceforge.net>
 */
class Game: public QObject
{
    Q_OBJECT;
public:

    /**
     * Creates an instance of Game. The GameServer class is responsible
     * for creating instances of Game.
     * \param parent parent of the Game is a GameServer
     * \param structGame the game structure from parser
     * \see GameServer::createGame()
     */
    Game(GameServer* parent, int gameId, const CreateGameData&);
    ~Game();

  /////////////
 // GETTERS //
/////////////

    /**
     * Returns the id of the game. Zero value is reserved and interpreted
     * as an invalid game.
     */
    inline int id() const { return m_id; }

    /**
     * Returns the count of players in the game.
     */
    inline int playersCount() const { return m_playerList.size(); }

    int alivePlayersCount() const;

    /**
     * Returns the count of spectators in the game.
     */
    inline int spectatorsCount() const { return 0; } /// @todo implement spectators

    inline GameState& gameState() { return m_state; }
    inline GameInfo& gameInfo()   { return *mp_gameInfo;  }
    inline GameCycle& gameCycle() { return *mp_gameCycle; }
    inline GameTable& gameTable() { return *mp_gameTable; }
    inline GameEventManager&
            gameEventManager(){ return *mp_gameEventManager; }
    inline BeerRescue* beerRescue() { return mp_beerRescue; }

    inline const PublicGameView& publicGameView() const
                                  { return m_publicGameView; }

    bool canBeStarted() const { return m_startable; }
    bool isFinished() const { return m_state == GAMESTATE_FINISHED; }

    /**
     * Returns the list of players, respecting the order
     * of players sitting around the table.
     */
    QList<Player*> playerList() const { return m_playerList; }

    /**
     * This method is provided for convenience.
     * @see Game::playerList()
     */
    QList<PublicPlayerView*> publicPlayerList() const
        { return m_publicPlayerList; }

    /**
     * Looks up and returns the Player according to its id.
     * Returns 0 if player doesn't exist.
     */
    Player* player(int playerId);

    /**
     * Returns the first player that succeeds the currentPlayer
     * and is alive.
     */
    Player* nextPlayer(Player* currentPlayer);

    /**
     * Returns the distance from one player to another. The effect
     * of "horse" cards is considered and therefore this function
     * does not have to be symmetric and generally getDistance(A,B) !=
     * getDistance(B,A).
     */
    int getDistance(Player* fromPlayer, Player* toPlayer) const;


  ///////////////
 // MODIFIERS //
///////////////

    /**
     * Creates a new player, adds him into the list of players and
     * returns the pointer to it. Creating players is reasonable only
     * before the game is started.
     */
    Player* createPlayer(const CreatePlayerData&, GameEventListener*);

    void createAI(int count);

    void replacePlayer(Player*, const CreatePlayerData&, GameEventListener*);

    /**
     * Removes a player from the game. Removing players is reasonable only
     * before the game is started.
     */
    void removePlayer(Player* player);

    void buryPlayer(Player* player, Player* killer);

    void winningSituation(PlayerRole winners);

    void setPlayerReaper(PlayerReaper*);
    void unsetPlayerReaper();

    /**
     * Tries to start the game.
     * @param player The player that tries to start the game, can be 0 to skip
     *               the creator testing.
     * @throws BadPlayerException player is not the game creator
     * @throws BadGameStateException game cannot be started now
     */
    void startGame(Player* player);

    /**
     * Sends a chat message by name of player.
     */
    void sendChatMessage(Player* player, const QString& message);

private:
    void checkStartable();
    void shufflePlayers();
    void setRolesAndCharacters();
    QList<PlayerRole> getRoleList();


private:
    int       m_id;
    GameState m_state;

    GameInfo*             mp_gameInfo;
    GameTable*            mp_gameTable;
    GameCycle*            mp_gameCycle;
    GameEventManager*     mp_gameEventManager;

    PublicGameView      m_publicGameView;
    int                 m_nextUnusedPlayerId;
    QMap<int, Player*>  m_playerMap;
    QList<Player*>      m_playerList;
    bool                m_startable;
    QList<PublicPlayerView*>
                        m_publicPlayerList;
    int                 m_goodGuysCount;
    int                 m_outlawsCount;
    int                 m_renegadesCount;
    BeerRescue*         mp_beerRescue;
    PlayerReaper*       mp_defaultPlayerReaper;
    PlayerReaper*       mp_playerReaper;
    GameLogger*         mp_gameLogger;
};

#endif
