#ifndef CHARACTERJESSEJONES_H
#define CHARACTERJESSEJONES_H

#include "characterbase.h"

class Player;

class CharacterJesseJones : public CharacterBase
{
Q_OBJECT
public:
    CharacterJesseJones(QObject *parent);
    virtual void useAbility(Player* targetPlayer);
    virtual void draw(bool specialDraw);
private:
    Player* mp_targetPlayer;
};

#endif // CHARACTERJESSEJONES_H
