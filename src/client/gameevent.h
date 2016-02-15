#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <QObject>

namespace client
{

class Game;

/**
 * This class introduces a base game event. Game events come from the server
 * through the parser to the client in random times. Some events take some
 * time to show them - for example a movement of a card can be animated and
 * it can take a little while to move a card from one spot to another.
 * The concept of game events allows client to queue incoming events and
 * execute them one after another.
 */

class GameEvent: public QObject
{
Q_OBJECT;
public:
    /**
     * Constructs a GameEvent.
     */
    GameEvent(Game* game);
    virtual ~GameEvent();
    virtual void run();
    virtual void stop();
    virtual bool isReadyRun();
    bool isRunning();

protected slots:
    virtual void finish();

protected:
    Game* mp_game;

private:
    bool m_isRunning;

signals:
    void finished(GameEvent*);
};
}

#endif // GAMEEVENT_H
