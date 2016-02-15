#include "characterpropertychange.h"
#include "player.h"

CharacterPropertyChange::CharacterPropertyChange(QObject* parent, Type type):
        CharacterBase(parent, CHARACTER_UNKNOWN),
        m_type(type)
{
    switch(type) {
    case RoseDoolan:
        setCharacterType(CHARACTER_ROSE_DOOLAN);
        break;
    case PaulRegret:
        setCharacterType(CHARACTER_PAUL_REGRET);
        break;
    case WillyTheKid:
        setCharacterType(CHARACTER_WILLY_THE_KID);
        break;
    case SlabTheKiller:
        setCharacterType(CHARACTER_SLAB_THE_KILLER);
        break;
    }
}

int CharacterPropertyChange::maxLifePoints() const
{
    if (m_type == PaulRegret)
        return 3;
    else
        return 4;
}

void CharacterPropertyChange::setPlayer(Player* player)
{
    CharacterBase::setPlayer(player);
    switch(m_type) {
    case RoseDoolan:
        player->modifyDistanceOut(1);
        break;
    case PaulRegret:
        player->modifyDistanceIn(1);
        break;
    case WillyTheKid:
        player->modifyUnlimitedBangs(1);
        break;
    case SlabTheKiller:
        player->setBangPower(2);
        break;
    }
}
