#ifndef CHARACTERBLACKJACK_H
#define CHARACTERBLACKJACK_H

#include "characterbase.h"

class CharacterBlackJack : public CharacterBase
{
Q_OBJECT;
public:
    CharacterBlackJack(QObject *parent);
    virtual void draw(bool specialDraw);
};

#endif // CHARACTERBLACKJACK_H
