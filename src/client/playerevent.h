#ifndef PLAYEREVENT_H
#define PLAYEREVENT_H

#include "parser/parserstructs.h"
#include "gameevent.h"

namespace client
{
class Game;

class PlayerEvent : public GameEvent
{
Q_OBJECT;
public:
    enum Type {
        PlayerJoined,
        PlayerLeaved,
        PlayerUpdate
    };

    PlayerEvent(Game* game);

    PlayerEvent* playerJoined(const PublicPlayerData&);
    PlayerEvent* playerLeaved(int playerId);
    PlayerEvent* playerUpdate(const PublicPlayerData&);

    virtual ~PlayerEvent();
    virtual void run();
private:
    Type                m_type;
    int                 m_playerId;
    PublicPlayerData    m_publicPlayerData;
};
}

#endif // PLAYEREVENT_H
