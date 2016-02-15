#include <QTimer>

#include "gamecontextchangeevent.h"
#include "game.h"
#include "playerwidget.h"

using namespace client;

GameContextChangeEvent::GameContextChangeEvent(Game* game, const GameContextData& gameContextData):
        GameEvent(game),
        m_gameContextData(gameContextData)
{
}

GameContextChangeEvent::~GameContextChangeEvent()
{
}

void GameContextChangeEvent::run()
{
    GameEvent::run();
    mp_game->setGameContext(m_gameContextData);
    QTimer::singleShot(100, this, SLOT(finish()));
}

