#ifndef GAMEEVENTHANDLER_H
#define GAMEEVENTHANDLER_H

#include <QObject>
#include "parser/parserstructs.h"

namespace client
{
class GameEventQueue;
class Game;

class GameEventHandler: public QObject
{
Q_OBJECT;
public:
    GameEventHandler(Game* game);
    void connectSlots(QObject* signalEmitter);

    void pause();
    void resume();
    void clear();

public slots:
    void onCardMovementEvent(const CardMovementData&);
    void onGameContextChangeEvent(const GameContextData&);
    void onGameSyncEvent(const GameSyncData&);
    void onLifePointsChangeEvent(int playerId, int lifePoints);
    void onPlayerDiedEvent(int playerId, PlayerRole role);

    void onPlayerJoinedEvent(const PublicPlayerData&);
    void onPlayerLeavedEvent(int playerId);
    void onPlayerUpdateEvent(const PublicPlayerData&);

    void onGameMessageEvent(const GameMessage&);



private:
    Game*           mp_game;
    GameEventQueue* mp_queue;
};

}
#endif // GAMEEVENTHANDLER_H
