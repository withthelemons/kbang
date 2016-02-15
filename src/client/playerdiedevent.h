#ifndef PLAYERDIEDEVENT_H
#define PLAYERDIEDEVENT_H

#include "parser/parserstructs.h"
#include "gameevent.h"



namespace client
{
class Game;

class PlayerDiedEvent : public GameEvent
{
Q_OBJECT;
public:
    PlayerDiedEvent(Game* game, int playerId, PlayerRole role);
    virtual ~PlayerDiedEvent();
    virtual void run();
private:
    int m_playerId;
    PlayerRole m_role;
};
}
#endif // GAMEFOCUSCHANGEEVENT_H
