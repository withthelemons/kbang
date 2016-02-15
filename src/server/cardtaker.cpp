#include "cardtaker.h"
#include "player.h"
#include "gamecycle.h"
#include "gametable.h"
#include "gameexceptions.h"
#include "game.h"

CardTaker::CardTaker(Game* game, int id, CardTaker::Type type, CardSuit cardSuit, CardRank cardRank):
        PlayingCard(game, id, CARD_UNKNOWN, cardSuit, cardRank),
        m_type(type)
{
    if (m_type == Panic)
        setType(CARD_PANIC);
    else
        setType(CARD_CATBALOU);
}

CardTaker::~CardTaker()
{
}

void CardTaker::play(Player* targetPlayer)
{
    gameCycle()->assertTurn();
    assertInHand();

    /* allow steel from himself only if has more than one card in hand */
    if (owner() == targetPlayer && owner()->handSize() < 2)
        throw BadTargetPlayerException();

    
    PlayingCard* targetCard;

    do {
        targetCard = targetPlayer->getRandomCardFromHand();
    } while (targetCard == this); // pick other than this card

    if (targetCard == 0)
        throw BadTargetPlayerException();

     play(targetCard);
}

void CardTaker::play(PlayingCard* targetCard)
{
    gameCycle()->assertTurn();
    assertInHand();

    Player* o = owner();

    if (m_type == Panic) {
        /* distance check */
        if (game()->getDistance(owner(), targetCard->owner()) > 1)
            throw PlayerOutOfRangeException();
        gameCycle()->setCardEffect(1);
        gameTable()->playerPlayCard(this, targetCard);
        gameTable()->playerStealCard(o, targetCard);
        gameCycle()->setCardEffect(0);
    } else {
        gameCycle()->setCardEffect(1);
        gameTable()->playerPlayCard(this, targetCard);
        gameTable()->cancelCard(targetCard, o);
        gameCycle()->setCardEffect(0);
    }
}

