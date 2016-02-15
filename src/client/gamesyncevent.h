#ifndef GAMESYNCEVENT_H
#define GAMESYNCEVENT_H

#include "gameevent.h"
#include "parser/parserstructs.h"

namespace client
{

class GameSyncEvent : public GameEvent
{
public:
    GameSyncEvent(Game* game, const GameSyncData& gameSyncData);
    virtual ~GameSyncEvent();
    virtual void run();
    virtual bool isReadyRun();


private:
    void setLocalPlayer(int index);


    GameSyncData m_gameSyncData;
};
}

#endif // GAMESYNCEVENT_H
