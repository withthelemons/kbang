#ifndef CHARACTERJOURDONNAIS_H
#define CHARACTERJOURDONNAIS_H

#include "characterbase.h"

class CardBarrel;

class CharacterJourdonnais : public CharacterBase
{
Q_OBJECT
public:
    CharacterJourdonnais(QObject *parent);
    virtual void useAbility();

private:
    CardBarrel* mp_integratedBarrel;
};

#endif // CHARACTERJOURDONNAIS_H
