#ifndef CARDDRAWCARDS_H
#define CARDDRAWCARDS_H

#include "playingcard.h"

class Player;

class CardDrawCards : public PlayingCard
{
Q_OBJECT;
public:
    enum Type {
        Diligenza,
        WellsFargo
    };
    CardDrawCards(Game* game, int id, Type, CardSuit, CardRank);
    ~CardDrawCards();
    virtual void play();

private:
    Type m_type;
    int  m_cardCount;
};

#endif // CARDDRAWCARDS_H
