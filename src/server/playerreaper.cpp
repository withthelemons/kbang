#include "playerreaper.h"
#include "player.h"
#include "game.h"
#include "gametable.h"

PlayerReaper::PlayerReaper(Game* game):
        mp_game(game)
{
}


void PlayerReaper::cleanUpCards(Player* corpse)
{
    foreach(PlayingCard* card, corpse->hand())
        mp_game->gameTable().cancelCard(card);

    foreach(PlayingCard* card, corpse->table())
        mp_game->gameTable().cancelCard(card);
}
