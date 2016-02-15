#include "playerevent.h"
#include "game.h"

using namespace client;

PlayerEvent::PlayerEvent(Game* game):
        GameEvent(game)
{
}

PlayerEvent* PlayerEvent::playerJoined(const PublicPlayerData& publicPlayerData)
{
    m_type = PlayerJoined;
    m_publicPlayerData = publicPlayerData;
    return this;
}

PlayerEvent* PlayerEvent::playerLeaved(int playerId)
{
    m_type = PlayerLeaved;
    m_playerId = playerId;
    return this;
}

PlayerEvent* PlayerEvent::playerUpdate(const PublicPlayerData& publicPlayerData)
{
    m_type = PlayerUpdate;
    m_publicPlayerData = publicPlayerData;
    return this;
}


PlayerEvent::~PlayerEvent()
{
}

void PlayerEvent::run()
{
    GameEvent::run();
    switch(m_type) {
    case PlayerJoined:
        mp_game->playerJoinedGame(m_publicPlayerData);
        break;
    case PlayerLeaved:
        mp_game->playerLeavedGame(m_playerId);
        break;
    case PlayerUpdate:
        mp_game->playerUpdate(m_publicPlayerData);
        break;
    }
    GameEvent::finish();
}

