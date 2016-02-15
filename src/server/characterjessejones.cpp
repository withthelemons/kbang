#include "characterjessejones.h"
#include "gametable.h"
#include "gamecycle.h"
#include "player.h"
#include "gameexceptions.h"

CharacterJesseJones::CharacterJesseJones(QObject *parent):
        CharacterBase(parent, CHARACTER_JESSE_JONES)
{
}

void CharacterJesseJones::useAbility(Player* targetPlayer)
{
    mp_targetPlayer = targetPlayer;
    gameCycle().draw(mp_player, 1);
}

void CharacterJesseJones::draw(bool specialDraw)
{
    if (specialDraw) {
        PlayingCard* targetCard = mp_targetPlayer->getRandomCardFromHand();
        if (targetCard == 0)
            throw BadTargetPlayerException();
        notifyAbilityUse();
        gameTable().playerStealCard(mp_player, targetCard);
        gameTable().playerDrawFromDeck(mp_player, 1, 0);
    } else {
        CharacterBase::draw(0);
    }
}
