#include "cardjail.h"

#include "gameexceptions.h"
#include "player.h"
#include "gametable.h"
#include "gamecycle.h"

CardJail::CardJail(Game *game, int id, CardSuit suit, CardRank rank):
        TableCard(game, id, CARD_JAIL, suit, rank)
{
}

CardJail::~CardJail()
{
}

void CardJail::play()
{
    gameCycle()->assertDraw();
    assertOnTable();
    owner()->predrawCheck(PredrawCheck);
    gameCycle()->checkDeck(owner(), this, *CardJail::checkJail, this);
}


void CardJail::play(Player* targetPlayer)
{
    gameCycle()->assertTurn();
    assertInHand();
    if (targetPlayer->role() == ROLE_SHERIFF)
        throw BadTargetPlayerException();

    if (targetPlayer->hasIdenticalCardOnTable(this)) {
        throw TwoSameOnTableException();
    }
    gameTable()->playerPlayCardOnTable(this, targetPlayer);
}

void CardJail::checkResult(bool result)
{            
    gameTable()->playerDiscardCard(this);
    if (!result) {
        gameCycle()->skipPlayersTurn();
    }
}


void CardJail::registerPlayer(Player* player)
{
    player->registerPredrawCheck(PredrawCheck);
}

void CardJail::unregisterPlayer(Player* player)
{
    player->unregisterPredrawCheck(PredrawCheck);
}

bool CardJail::checkJail(PlayingCard* card)
{
    return (card->suit() == SUIT_HEARTS);
}
