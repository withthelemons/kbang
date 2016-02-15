#ifndef CHARACTERCALAMITYJANET_H
#define CHARACTERCALAMITYJANET_H

#include "characterbase.h"

class CharacterCalamityJanet : public CharacterBase
{
Q_OBJECT;
public:
    CharacterCalamityJanet(QObject *parent);

    virtual void playCard(PlayingCard* card);
    virtual void playCard(PlayingCard* card, Player* targetPlayer);
    virtual void playCard(PlayingCard* card, PlayingCard* targetCard);
    virtual void respondCard(ReactionHandler*, PlayingCard* targetCard);

private:
    PlayingCard* swapCards(PlayingCard* card);

    PlayingCard* mp_bang;
    PlayingCard* mp_missed;

};

#endif // CHARACTERCALAMITYJANET_H
