#include <QTimer>

#include "playerdiedevent.h"
#include "game.h"
#include "playerwidget.h"

using namespace client;

PlayerDiedEvent::PlayerDiedEvent(Game* game, int playerId, PlayerRole role):
        GameEvent(game),
        m_playerId(playerId),
        m_role(role)
{
}

PlayerDiedEvent::~PlayerDiedEvent()
{
}

void PlayerDiedEvent::run()
{
    GameEvent::run();
    PlayerWidget* player = mp_game->playerWidget(m_playerId);
    player->dieAndRevealRole(m_role);
    QTimer::singleShot(1000, this, SLOT(finish()));
}
