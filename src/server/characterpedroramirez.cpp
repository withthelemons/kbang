#include "characterpedroramirez.h"
#include "gamecycle.h"
#include "playingcard.h"
#include "gametable.h"
#include "gameexceptions.h"

CharacterPedroRamirez::CharacterPedroRamirez(QObject *parent):
        CharacterBase(parent, CHARACTER_PEDRO_RAMIREZ)
{
}

void CharacterPedroRamirez::useAbility()
{
    gameCycle().draw(mp_player, 1);
}

void CharacterPedroRamirez::draw(bool specialDraw)
{
    if (specialDraw) {
        if (gameTable().isEmptyGraveyard())
            throw BadGameStateException(); // @todo: maybe throw different exception
        notifyAbilityUse();
        gameTable().playerDrawFromGraveyard(mp_player);
        gameTable().playerDrawFromDeck(mp_player, 1, 0);
    } else {
        CharacterBase::draw(0);
    }
}
