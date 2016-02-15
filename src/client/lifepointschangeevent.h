#ifndef LIFEPOINTSCHANGEEVENT_H
#define LIFEPOINTSCHANGEEVENT_H

#include "gameevent.h"

namespace client
{

class LifePointsChangeEvent : public GameEvent
{
Q_OBJECT;
public:
    LifePointsChangeEvent(Game* game, int playerId, int lifePoints);
    virtual ~LifePointsChangeEvent();
    virtual void run();
    virtual void stop();

private:
    int m_playerId;
    int m_lifePoints;
};
}
#endif // LIFEPOINTSCHANGEEVENT_H
