#ifndef GAMEEVENTQUEUE_H
#define GAMEEVENTQUEUE_H

#include <QObject>
#include <QQueue>

namespace client
{
class GameEvent;

class GameEventQueue : public QObject
{
Q_OBJECT;
public:
    GameEventQueue(QObject* parent);
    ~GameEventQueue();

    void add(GameEvent*);

    void pause();
    void resume();
    void clear();

public slots:
    void onGameEventFinished(GameEvent*);

private slots:
    void runGameEvent();

private:
    QQueue<GameEvent*> m_queue;
    bool m_paused;
    bool m_eventOnHold;
    bool m_aboutToDelete;

};
}

#endif // GAMEEVENTQUEUE_H
