#ifndef CHARACTERVULTURESAM_H
#define CHARACTERVULTURESAM_H

#include "characterbase.h"
#include "playerreaper.h"

class CharacterVultureSam : public CharacterBase, public PlayerReaper
{
Q_OBJECT;
public:
    CharacterVultureSam(QObject* parent);
    virtual void setPlayer(Player* player);
    virtual void playerDied();
    virtual void cleanUpCards(Player* corpse);
};

#endif // CHARACTERVULTURESAM_H
