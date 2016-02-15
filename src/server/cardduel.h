#ifndef CARDDUEL_H
#define CARDDUEL_H

#include "reactioncard.h"

class Player;

class CardDuel : public ReactionCard
{
Q_OBJECT;
public:
    CardDuel(Game* game, int id, CardSuit, CardRank);
    ~CardDuel();
    virtual void play(Player* targetPlayer);

    virtual void respondPass();
    virtual void respondCard(PlayingCard* targetCard);
    virtual ReactionType reactionType() const { return REACTION_DUEL; }
    virtual Player* causedBy() const { return mp_shootingPlayer; }

private:
    void requestNext();

    Player* mp_initialPlayer;
    Player* mp_shootingPlayer;
    Player* mp_requestedPlayer;
};


#endif // CARDDUEL_H
