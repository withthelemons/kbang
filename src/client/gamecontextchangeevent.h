#ifndef GAMEFOCUSCHANGEEVENT_H
#define GAMEFOCUSCHANGEEVENT_H

#include "parser/parserstructs.h"
#include "gameevent.h"



namespace client
{
class Game;

class GameContextChangeEvent : public GameEvent
{
Q_OBJECT;
public:
    GameContextChangeEvent(Game* game, const GameContextData&);
    virtual ~GameContextChangeEvent();
    virtual void run();
private:
    GameContextData m_gameContextData;
};
}
#endif // GAMEFOCUSCHANGEEVENT_H
