
#include "gameeventhandler.h"
#include "gameeventqueue.h"
#include "game.h"

#include "cardmovementevent.h"
#include "gamecontextchangeevent.h"
#include "gamesyncevent.h"
#include "lifepointschangeevent.h"
#include "playerdiedevent.h"
#include "playerevent.h"
#include "gamemessageevent.h"

using namespace client;

GameEventHandler::GameEventHandler(Game* game):
        QObject(game),
        mp_game(game)
{
    mp_queue = new GameEventQueue(this);
}

void GameEventHandler::connectSlots(QObject* signalEmitter)
{
    connect(signalEmitter,      SIGNAL(sigEventCardMovement(const CardMovementData&)),
            this,               SLOT(onCardMovementEvent(const CardMovementData&)));
    connect(signalEmitter,      SIGNAL(sigEventGameContextChange(const GameContextData&)),
            this,               SLOT(onGameContextChangeEvent(const GameContextData&)));
    connect(signalEmitter,      SIGNAL(sigEventGameSync(const GameSyncData&)),
            this,               SLOT(onGameSyncEvent(const GameSyncData&)));
    connect(signalEmitter,      SIGNAL(sigEventLifePointsChange(int, int)),
            this,               SLOT(onLifePointsChangeEvent(int, int)));
    connect(signalEmitter,      SIGNAL(sigEventPlayerDied(int, PlayerRole)),
            this,               SLOT(onPlayerDiedEvent(int, PlayerRole)));
    connect(signalEmitter,      SIGNAL(sigEventPlayerJoinedGame(const PublicPlayerData&)),
            this,               SLOT(onPlayerJoinedEvent(const PublicPlayerData&)));
    connect(signalEmitter,      SIGNAL(sigEventPlayerLeavedGame(int)),
            this,               SLOT(onPlayerLeavedEvent(int)));
    connect(signalEmitter,      SIGNAL(sigEventPlayerUpdate(const PublicPlayerData&)),
            this,               SLOT(onPlayerUpdateEvent(const PublicPlayerData&)));
    connect(signalEmitter,      SIGNAL(sigEventGameMessage(const GameMessage&)),
            this,               SLOT(onGameMessageEvent(const GameMessage&)));
}

void GameEventHandler::pause()
{
    mp_queue->pause();
}

void GameEventHandler::resume()
{
    mp_queue->resume();
}

void GameEventHandler::clear()
{
    mp_queue->clear();
}


void GameEventHandler::onCardMovementEvent(const CardMovementData& cardMovementData)
{
    mp_queue->add(new CardMovementEvent(mp_game, cardMovementData));
}

void GameEventHandler::onGameContextChangeEvent(const GameContextData& gameContextData)
{
    mp_queue->add(new GameContextChangeEvent(mp_game, gameContextData));
}

void GameEventHandler::onGameSyncEvent(const GameSyncData& gameSyncData)
{
    mp_queue->add(new GameSyncEvent(mp_game, gameSyncData));
}

void GameEventHandler::onLifePointsChangeEvent(int playerId, int lifePoints)
{
    mp_queue->add(new LifePointsChangeEvent(mp_game, playerId, lifePoints));
}

void GameEventHandler::onPlayerDiedEvent(int playerId, PlayerRole role)
{
    mp_queue->add(new PlayerDiedEvent(mp_game, playerId, role));
}

void GameEventHandler::onPlayerJoinedEvent(const PublicPlayerData& player)
{
    mp_queue->add((new PlayerEvent(mp_game))->playerJoined(player));
}

void GameEventHandler::onPlayerLeavedEvent(int playerId)
{
    mp_queue->add((new PlayerEvent(mp_game))->playerLeaved(playerId));
}
void GameEventHandler::onPlayerUpdateEvent(const PublicPlayerData& player)
{
    mp_queue->add((new PlayerEvent(mp_game))->playerUpdate(player));
}

void GameEventHandler::onGameMessageEvent(const GameMessage& gameMessage)
{
    mp_queue->add(new GameMessageEvent(mp_game, gameMessage));
}
