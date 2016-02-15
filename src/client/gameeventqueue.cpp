#include <QTimer>
#include <QQueue>
#include <QtDebug>
#include "gameeventqueue.h"
#include "gameevent.h"


using namespace client;

GameEventQueue::GameEventQueue(QObject* parent):
        QObject(parent),
        m_paused(0),
        m_eventOnHold(0),
        m_aboutToDelete(0)
{
}

GameEventQueue::~GameEventQueue()
{
}

void GameEventQueue::add(GameEvent* gameEvent)
{
    if (m_queue.isEmpty()) {
        m_queue.enqueue(gameEvent);
        runGameEvent();
    } else {
        m_queue.enqueue(gameEvent);
    }
}


void GameEventQueue::pause()
{
    m_paused = 1;
}

void GameEventQueue::resume()
{
    m_paused = 0;
    if (m_eventOnHold) {
        m_eventOnHold = 0;
        runGameEvent();
    }
}

void GameEventQueue::clear()
{
    m_aboutToDelete = 1;
    if (!m_queue.isEmpty() && m_queue.head()->isRunning()) {
        m_queue.head()->stop();
    }
}

void GameEventQueue::runGameEvent()
{
    if (m_aboutToDelete) {
        return;
    }
    if (m_paused) {
        m_eventOnHold = 1;
        return;
    }
    GameEvent* gameEvent = m_queue.head();
    Q_ASSERT(!gameEvent->isRunning());
    if (gameEvent->isReadyRun()) {
        connect(gameEvent, SIGNAL(finished(GameEvent*)),
                this,      SLOT(onGameEventFinished(GameEvent*)));
        gameEvent->run();
    } else {
        QTimer::singleShot(10, this, SLOT(runGameEvent()));
    }
}

void GameEventQueue::onGameEventFinished(GameEvent* gameEvent)
{
    Q_ASSERT(gameEvent == m_queue.head());
    m_queue.dequeue()->deleteLater();
    if (!m_queue.isEmpty())
        runGameEvent();
    else
        qDebug("Event queue is empty!");
}
