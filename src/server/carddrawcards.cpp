#include "carddrawcards.h"
#include "gametable.h"
#include "gamecycle.h"

CardDrawCards::CardDrawCards(Game* game, int id, CardDrawCards::Type type, CardSuit cardSuit, CardRank cardRank):
        PlayingCard(game, id, CARD_UNKNOWN, cardSuit, cardRank),
        m_type(type)
{
    switch(m_type) {
    case Diligenza:
        setType(CARD_DILIGENZA);
        m_cardCount = 2;
        break;
    case WellsFargo:
        setType(CARD_WELLSFARGO);
        m_cardCount = 3;
        break;
    }
}


CardDrawCards::~CardDrawCards()
{
}

void CardDrawCards::play()
{
    gameCycle()->assertTurn();
    assertInHand();
    Player* player = owner();
    gameCycle()->setCardEffect(1);
    gameTable()->playerPlayCard(this);
    gameTable()->playerDrawFromDeck(player, m_cardCount);
    gameCycle()->setCardEffect(0);
}
