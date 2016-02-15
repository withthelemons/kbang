#ifndef CHARACTERSIDKETCHUM_H
#define CHARACTERSIDKETCHUM_H

#include "characterbase.h"

class CharacterSidKetchum : public CharacterBase
{
Q_OBJECT
public:
    CharacterSidKetchum(QObject *parent);
    virtual void useAbility(QList<PlayingCard*> cards);
};

#endif // CHARACTERSIDKETCHUM_H
