#ifndef CHARACTERKITCARLSON_H
#define CHARACTERKITCARLSON_H

#include "characterbase.h"
#include "reactionhandler.h"

class CharacterKitCarlson : public CharacterBase, public ReactionHandler
{
Q_OBJECT
public:
    CharacterKitCarlson(QObject *parent);
    virtual void useAbility();
    virtual void draw(bool specialDraw);

    virtual void respondCard(PlayingCard* targetCard);
    virtual ReactionType reactionType() const { return REACTION_KITCARLSON; }
    virtual Player* causedBy() const { return 0; }
};

#endif // CHARACTERKITCARLSON_H
