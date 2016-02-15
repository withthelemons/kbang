#ifndef CARDTAKER_H
#define CARDTAKER_H

#include "playingcard.h"

class CardTaker : public PlayingCard
{
public:
    enum Type {
        Panic,
        CatBalou
    };

    CardTaker(Game *game, int id, Type, CardSuit, CardRank);
    ~CardTaker();

    virtual void play(Player* targetPlayer);
    virtual void play(PlayingCard* targetCard);

private:
    Type m_type;
};

#endif // CARDTAKER_H
