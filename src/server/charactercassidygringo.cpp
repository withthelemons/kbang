#include "charactercassidygringo.h"
#include "gametable.h"
#include "player.h"

CharacterCassidyGringo::CharacterCassidyGringo(QObject* parent, Type type):
    CharacterBase(parent, CHARACTER_UNKNOWN),
    m_type(type)
{
    if (type == BartCassidy)
        setCharacterType(CHARACTER_BART_CASSIDY);
    else if (type == ElGringo)
        setCharacterType(CHARACTER_EL_GRINGO);
}

void CharacterCassidyGringo::setPlayer(Player* player)
{
    CharacterBase::setPlayer(player);
    connect(player, SIGNAL(onHit(int,Player*)),
            this,   SLOT(onHit(int,Player*)));
}

int CharacterCassidyGringo::maxLifePoints() const
{
    if (m_type == BartCassidy)
        return 4;
    else
        return 3;
}

void CharacterCassidyGringo::onHit(int lifePoints, Player* causedBy)
{
    if (m_type == BartCassidy) {
        notifyAbilityUse();
        gameTable().playerDrawFromDeck(mp_player, lifePoints, 0);
        return;
    }
    if (m_type == ElGringo) {
        if (causedBy == 0 || causedBy == mp_player) return;
        notifyAbilityUse();
        for (int i = 0; i < lifePoints; ++i) {
            PlayingCard* targetCard = causedBy->getRandomCardFromHand();
            if (targetCard == 0) return;
            gameTable().playerStealCard(mp_player, targetCard);
        }
        return;
    }
}
