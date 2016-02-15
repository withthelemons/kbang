#ifndef CHARACTERSUZYLAFAYETTE_H
#define CHARACTERSUZYLAFAYETTE_H

#include "characterbase.h"

class CharacterSuzyLafayette : public CharacterBase
{
Q_OBJECT
public:
    CharacterSuzyLafayette(QObject* parent);
    virtual void setPlayer(Player* player);

public slots:
    void onEmptyHand();


};

#endif // CHARACTERSUZYLAFAYETTE_H
