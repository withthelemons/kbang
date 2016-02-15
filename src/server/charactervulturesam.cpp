#include "charactervulturesam.h"
#include "player.h"
#include "game.h"
#include "gametable.h"


CharacterVultureSam::CharacterVultureSam(QObject* parent):
        CharacterBase(parent, CHARACTER_VULTURE_SAM)
{
}


void CharacterVultureSam::setPlayer(Player* player)
{
    CharacterBase::setPlayer(player);
    player->game()->setPlayerReaper(this);
}

void CharacterVultureSam::playerDied()
{
    mp_player->game()->unsetPlayerReaper();
}

void CharacterVultureSam::cleanUpCards(Player* corpse)
{
    notifyAbilityUse();
    foreach(PlayingCard* card, corpse->hand())
        mp_player->game()->gameTable().playerStealCard(mp_player, card);

    foreach(PlayingCard* card, corpse->table())
        mp_player->game()->gameTable().playerStealCard(mp_player, card);
}
