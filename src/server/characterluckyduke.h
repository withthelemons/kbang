#ifndef CHARACTERLUCKYDUKE_H
#define CHARACTERLUCKYDUKE_H

#include "characterbase.h"
#include "reactionhandler.h"

class CheckDeckResultHandler;
class PlayingCard;

class CharacterLuckyDuke : public CharacterBase, public ReactionHandler
{
Q_OBJECT
public:
    CharacterLuckyDuke(QObject *parent);
    virtual void checkDeck(PlayingCard* causedBy, bool (*checkFunc)(PlayingCard*), CheckDeckResultHandler*);

    virtual void respondCard(PlayingCard* targetCard);
    virtual ReactionType reactionType() const { return REACTION_LUCKYDUKE; }
    virtual Player* causedBy() const { return 0; }

private:
    PlayingCard*            mp_causedBy;
    CheckDeckResultHandler* mp_resultHandler;
    bool                    (*mp_checkFunc)(PlayingCard*);
};

#endif // CHARACTERLUCKYDUKE_H
