#include "cardmultishoot.h"
#include "gametable.h"
#include "gamecycle.h"
#include "game.h"
#include "player.h"
#include "gameexceptions.h"
#include "cardbarrel.h"

CardMultiShoot::CardMultiShoot(Game* game, int id, CardMultiShoot::Type type, CardSuit cardSuit, CardRank cardRank):
        ReactionCard(game, id, CARD_UNKNOWN, cardSuit, cardRank),
        m_type(type),
        mp_requestedPlayer(0)
{
    switch(m_type) {
    case Indians:
        setType(CARD_INDIANS);
        break;
    case Gatling:
        setType(CARD_GATLING);
        break;
    }
}

CardMultiShoot::~CardMultiShoot()
{
}

void CardMultiShoot::play()
{
    gameCycle()->assertTurn();
    assertInHand();
    mp_shootingPlayer = owner();
    mp_requestedPlayer = owner();
    m_usedBarrels.clear();
    gameTable()->playerPlayCard(this);
    requestNext();
}

void CardMultiShoot::respondPass()
{
    gameCycle()->unsetResponseMode();
    gameTable()->playerPass(mp_requestedPlayer);
    mp_requestedPlayer->modifyLifePoints(-1, mp_shootingPlayer);
    requestNext();
}

void CardMultiShoot::respondCard(PlayingCard* targetCard)
{
    switch(targetCard->type()) {
    case CARD_BANG:
        if (m_type != Indians)
            break;
        targetCard->assertInHand();
        gameTable()->playerRespondWithCard(targetCard);
        game()->gameCycle().unsetResponseMode();
        requestNext();
        return;

    case CARD_MISSED:
        if (m_type != Gatling)
            break;
        targetCard->assertInHand();
        gameTable()->playerRespondWithCard(targetCard);
        game()->gameCycle().unsetResponseMode();
        requestNext();
        return;
    case CARD_BARREL: {
        if (m_type != Gatling)
            break;
        if (m_usedBarrels.contains(targetCard))
            break;
        targetCard->assertOnTable();
        m_usedBarrels.append(targetCard);
        CardBarrel* barrel = qobject_cast<CardBarrel*>(targetCard);
        barrel->check(this);
        return;
    }
    default:
        break;
    }
    throw BadCardException();
}

void CardMultiShoot::checkResult(bool result)
{
    Q_ASSERT(m_type == Gatling);
    if (result) {
        game()->gameCycle().unsetResponseMode();
        requestNext();
    }
}


ReactionType CardMultiShoot::reactionType() const
{
    return (m_type == Indians) ? REACTION_INDIANS : REACTION_GATLING;
}

void CardMultiShoot::requestNext()
{
    mp_requestedPlayer = game()->nextPlayer(mp_requestedPlayer);
    if (mp_requestedPlayer == mp_shootingPlayer) return;
    game()->gameCycle().setResponseMode(this, mp_requestedPlayer);
}
