#include "carddynamite.h"
#include "gamecycle.h"
#include "gameexceptions.h"
#include "gametable.h"
#include "game.h"
#include "player.h"

CardDynamite::CardDynamite(Game *game, int id, CardSuit suit, CardRank rank):
        TableCard(game, id, CARD_DYNAMITE, suit, rank)
{
}

CardDynamite::~CardDynamite()
{
}

void CardDynamite::play()
{
    if (gameCycle()->isDraw()) {
        assertOnTable();
        owner()->predrawCheck(PredrawCheck);
        gameCycle()->checkDeck(owner(), this, *CardDynamite::checkDynamite, this);
        return;
    } else {
        gameCycle()->assertTurn();
        assertInHand();
        if (owner()->hasIdenticalCardOnTable(this)) {
            throw TwoSameOnTableException();
        }
        gameTable()->playerPlayCardOnTable(this);
    }
}

void CardDynamite::checkResult(bool result)
{
    if (result) { // luck
        gameTable()->passTableCard(this, game()->nextPlayer(owner()));
    } else {
        Player* player = owner();
        gameTable()->playerDiscardCard(this);
        player->modifyLifePoints(-3, 0);
    }
}


void CardDynamite::registerPlayer(Player* player)
{
   player->registerPredrawCheck(PredrawCheck);
}

void CardDynamite::unregisterPlayer(Player* player)
{
   player->unregisterPredrawCheck(PredrawCheck);
}


bool CardDynamite::checkDynamite(PlayingCard* card)
{
    return  (card->suit() != SUIT_SPADES || card->rank() > 9);
}
