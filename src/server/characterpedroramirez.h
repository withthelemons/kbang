#ifndef CHARACTERPEDRORAMIREZ_H
#define CHARACTERPEDRORAMIREZ_H

#include "characterbase.h"

class CharacterPedroRamirez : public CharacterBase
{
Q_OBJECT
public:
    CharacterPedroRamirez(QObject *parent);
    virtual void useAbility();
    virtual void draw(bool specialDraw);
};


#endif // CHARACTERPEDRORAMIREZ_H
