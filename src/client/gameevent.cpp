#include "gameevent.h"
#include "game.h"

using namespace client;

GameEvent::GameEvent(Game* game):
        QObject(game),
        mp_game(game),
        m_isRunning(0)
{
}

GameEvent::~GameEvent()
{
}

void GameEvent::run()
{
    m_isRunning = 1;
}

void GameEvent::stop()
{
}

void GameEvent::finish()
{
    m_isRunning = 0;
    emit finished(this);
}

bool GameEvent::isReadyRun()
{
    return 1;
}

bool GameEvent::isRunning()
{
    return m_isRunning;
}
