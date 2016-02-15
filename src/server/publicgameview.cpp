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
#include "publicgameview.h"
#include "publicplayerview.h"
#include "game.h"
#include "gameinfo.h"
#include "player.h"
#include "gamecycle.h"
#include "gametable.h"
#include "reactioncard.h"

PublicGameView::PublicGameView(Game* game):
    mp_game(game)
{
}


PublicGameView::~PublicGameView()
{
}

int PublicGameView::id() const
{
    return mp_game->id();
}

QString PublicGameView::name() const
{
    return mp_game->gameInfo().name();
}

QString PublicGameView::description() const
{
    return mp_game->gameInfo().description();
}

int PublicGameView::playersCount() const
{
    return mp_game->playersCount();
}

bool PublicGameView::canBeStarted() const
{
    return mp_game->canBeStarted();
}

GameInfoData PublicGameView::gameInfoData() const
{
    GameInfoData res;
    res.id          = id();
    res.name        = name();
    res.description = description();
    res.minPlayers  = mp_game->gameInfo().minPlayers();
    res.maxPlayers  = mp_game->gameInfo().maxPlayers();
    res.maxSpectators = mp_game->gameInfo().maxSpectators();
    res.totalPlayersCnt = mp_game->playersCount();
    res.alivePlayersCnt = mp_game->alivePlayersCount();
    res.spectatorsCnt   = mp_game->spectatorsCount();
    //res.AIPlayersCnt    todo
    res.hasPlayerPassword = mp_game->gameInfo().hasPlayerPassword();
    res.hasSpectatorPassword = mp_game->gameInfo().hasSpectatorPassword();
    res.state           = mp_game->gameState();
    foreach(Player* p, mp_game->playerList()) {
        PlayerInfoData playerInfo;
        playerInfo.id = p->id();
        playerInfo.name = p->name();
        playerInfo.isAlive = p->isAlive();
        playerInfo.hasController = (p->gameEventListener() != 0);
        playerInfo.isAI = p->isAI();
        playerInfo.hasPassword = 0; // todo
        res.players.append(playerInfo);
    }
    return res;
}

GameContextData PublicGameView::gameContextData() const
{
    return mp_game->gameCycle().gameContextData();
}

GameState PublicGameView::gameState() const
{
    return mp_game->gameState();
}

PlayingCard* PublicGameView::graveyardTop() const
{
    return mp_game->gameTable().graveyardTop();
}
/*
const PlayingCard* PublicGameView::reactionCard() const
{
        return qobject_cast<ReactionCard*>(mp_game->gameCycle().reactionHandler());
}
*/

QList<PlayingCard*> PublicGameView::selection() const
{
    return mp_game->gameTable().selection();
}

QList<PublicPlayerView*>  PublicGameView::publicPlayerList() const
{
    return mp_game->publicPlayerList();
}

PublicPlayerView* PublicGameView::publicPlayerView(int playerId) const
{
    Player* player = mp_game->player(playerId);
    if (player == 0) return 0;
    return &player->publicView();
}

QList<PublicPlayerView*> PublicGameView::neighbors(PublicPlayerView* source, int distance) const
{
    QList<PublicPlayerView*> result;
    foreach (PublicPlayerView* p, mp_game->publicPlayerList()) {
        if (p->id() == source->id()) continue;
        if (mp_game->getDistance(Player::player(source), Player::player(p)) <= distance)
            result.append(p);
    }
    return result;
}
