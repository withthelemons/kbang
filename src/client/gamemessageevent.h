#ifndef GAMEMESSAGEEVENT_H
#define GAMEMESSAGEEVENT_H

#include "gameevent.h"
#include "parser/parserstructs.h"

namespace client
{
class GameMessageEvent: public GameEvent
{
Q_OBJECT;
public:
    GameMessageEvent(Game* game, const GameMessage&);
    virtual ~GameMessageEvent();
    virtual void run();

private:
    QString cardToString(const CardData&, bool withRankAndSuit = 0);
    QString cardListWidgetToString(QList<CardData>);
    QString decoratePlayerName(const QString& playerName, bool isTarget = 0);

    GameMessage m_gameMessage;

};
}

#endif // GAMEMESSAGEEVENT_H
