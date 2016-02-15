#include "characterlist.h"

#include "characterbase.h"
#include "characterblackjack.h"
#include "charactercalamityjanet.h"
#include "charactercassidygringo.h"
#include "characterjessejones.h"
#include "characterjourdonnais.h"
#include "characterkitcarlson.h"
#include "characterluckyduke.h"
#include "characterpedroramirez.h"
#include "characterpropertychange.h"
#include "charactersidketchum.h"
#include "charactersuzylafayette.h"
#include "charactervulturesam.h"
#include "util.h"

QList<CharacterType> CharacterList::sm_characterTypes;

CharacterList::CharacterList(QObject* parent, int size):
        QList<CharacterBase*>()
{
    if (sm_characterTypes.size() == 0)
        initCharacterTypes();
    Q_ASSERT(sm_characterTypes.size() != 0);
    shuffleList(sm_characterTypes);
    for (int i = 0; i < size; ++i) {
        append(createCharacter(parent, sm_characterTypes[i % sm_characterTypes.size()]));
    }
}

CharacterBase* CharacterList::createCharacter(QObject* parent, CharacterType type)
{
    switch(type) {
    case CHARACTER_BART_CASSIDY:
        return new CharacterCassidyGringo(parent, CharacterCassidyGringo::BartCassidy);
    case CHARACTER_BLACK_JACK:
        return new CharacterBlackJack(parent);
    case CHARACTER_CALAMITY_JANET:
        return new CharacterCalamityJanet(parent);
    case CHARACTER_EL_GRINGO:
        return new CharacterCassidyGringo(parent, CharacterCassidyGringo::ElGringo);
    case CHARACTER_JESSE_JONES:
        return new CharacterJesseJones(parent);
    case CHARACTER_JOURDONNAIS:
        return new CharacterJourdonnais(parent);
    case CHARACTER_KIT_CARLSON:
        return new CharacterKitCarlson(parent);
    case CHARACTER_LUCKY_DUKE:
        return new CharacterLuckyDuke(parent);
    case CHARACTER_PAUL_REGRET:
        return new CharacterPropertyChange(parent, CharacterPropertyChange::PaulRegret);
    case CHARACTER_PEDRO_RAMIREZ:
        return new CharacterPedroRamirez(parent);
    case CHARACTER_ROSE_DOOLAN:
        return new CharacterPropertyChange(parent, CharacterPropertyChange::RoseDoolan);
    case CHARACTER_SID_KETCHUM:
        return new CharacterSidKetchum(parent);
    case CHARACTER_SLAB_THE_KILLER:
        return new CharacterPropertyChange(parent, CharacterPropertyChange::SlabTheKiller);
    case CHARACTER_SUZY_LAFAYETTE:
        return new CharacterSuzyLafayette(parent);
    case CHARACTER_VULTURE_SAM:
        return new CharacterVultureSam(parent);
    case CHARACTER_WILLY_THE_KID:
        return new CharacterPropertyChange(parent, CharacterPropertyChange::WillyTheKid);
    case CHARACTER_UNKNOWN:
        NOT_REACHED();
    }
    return new CharacterBase(parent, CHARACTER_UNKNOWN);
}

void CharacterList::initCharacterTypes()
{
    Q_ASSERT(sm_characterTypes.size() == 0);
    sm_characterTypes.append(CHARACTER_BART_CASSIDY);
    sm_characterTypes.append(CHARACTER_BLACK_JACK);
    sm_characterTypes.append(CHARACTER_CALAMITY_JANET);
    sm_characterTypes.append(CHARACTER_EL_GRINGO);
    sm_characterTypes.append(CHARACTER_JESSE_JONES);
    sm_characterTypes.append(CHARACTER_JOURDONNAIS);
    sm_characterTypes.append(CHARACTER_KIT_CARLSON);
    sm_characterTypes.append(CHARACTER_LUCKY_DUKE);
    sm_characterTypes.append(CHARACTER_PAUL_REGRET);
    sm_characterTypes.append(CHARACTER_PEDRO_RAMIREZ);
    sm_characterTypes.append(CHARACTER_ROSE_DOOLAN);
    sm_characterTypes.append(CHARACTER_SID_KETCHUM);
    sm_characterTypes.append(CHARACTER_SLAB_THE_KILLER);
    sm_characterTypes.append(CHARACTER_SUZY_LAFAYETTE);
    sm_characterTypes.append(CHARACTER_VULTURE_SAM);
    sm_characterTypes.append(CHARACTER_WILLY_THE_KID);
}
