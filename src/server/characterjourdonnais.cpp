#include "characterjourdonnais.h"
#include "cardbarrel.h"
#include "player.h"
#include "gamecycle.h"

CharacterJourdonnais::CharacterJourdonnais(QObject* parent):
        CharacterBase(parent, CHARACTER_JOURDONNAIS),
        mp_integratedBarrel(0)
{
}

void CharacterJourdonnais::useAbility()
{
    if (mp_integratedBarrel == 0) {
        mp_integratedBarrel = new CardBarrel(mp_player->game(), 0, SUIT_CLUBS, 2);
        mp_integratedBarrel->setVirtual(mp_player, POCKET_TABLE);
    }
    gameCycle().playCard(mp_player, mp_integratedBarrel);
}

