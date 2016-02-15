#include "cardduel.h"
#include "gametable.h"
#include "gamecycle.h"
#include "game.h"
#include "player.h"
#include "gameexceptions.h"

CardDuel::CardDuel(Game* game, int id, CardSuit cardSuit, CardRank cardRank):
        ReactionCard(game, id, CARD_DUEL, cardSuit, cardRank)
{
}

CardDuel::~CardDuel()
{
}

void CardDuel::play(Player* targetPlayer)
{
    gameCycle()->assertTurn();
    assertInHand();

    /* don't allow shoot yourself */
    if (owner() == targetPlayer)
        throw BadTargetPlayerException();

    mp_initialPlayer = owner();
    // the positions will swap in requestNext(), therefore
    // are swapped here
    mp_requestedPlayer = owner();
    mp_shootingPlayer  = targetPlayer;

    gameCycle()->setCardEffect(1);
    gameTable()->playerPlayCard(this, targetPlayer);
    requestNext();
}

void CardDuel::respondPass()
{
    game()->gameCycle().unsetResponseMode();
    gameTable()->playerPass(mp_requestedPlayer);
    mp_requestedPlayer->modifyLifePoints(-1, mp_initialPlayer);
    gameCycle()->setCardEffect(0);
}

void CardDuel::respondCard(PlayingCard* targetCard)
{
    targetCard->assertInHand();
    switch(targetCard->type()) {
    case CARD_BANG:
        gameTable()->playerRespondWithCard(targetCard);
        game()->gameCycle().unsetResponseMode();
        requestNext();
        return;
    default:
        break;
    }
    throw BadCardException();
}

void CardDuel::requestNext()
{
    Player* tmp = mp_requestedPlayer;
    mp_requestedPlayer = mp_shootingPlayer;
    mp_shootingPlayer = tmp;
    game()->gameCycle().setResponseMode(this, mp_requestedPlayer);
}


