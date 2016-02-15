#ifndef REACTIONHANDLER_H
#define REACTIONHANDLER_H

#include <QObject>
#include "parser/parserstructs.h"

class PlayingCard;
class Player;

class ReactionHandler
{
public:
    virtual ~ReactionHandler() {}
    virtual void respondPass();
    virtual void respondCard(PlayingCard* targetCard);

    virtual void dismiss();

    virtual ReactionType reactionType() const = 0;
    virtual Player* causedBy() const= 0;
};

#endif // REACTIONHANDLER_H
