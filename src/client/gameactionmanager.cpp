#include "gameactionmanager.h"
#include "game.h"
#include "playerwidget.h"
#include "serverconnection.h"
#include "cardwidget.h"
#include "cardzoomwidget.h"

using namespace client;

GameActionManager::GameActionManager(Game* game):
        m_state(STATE_MAIN),
        mp_game(game),
        mp_activeCard(0)
{
}



bool GameActionManager::isClickable(CardWidget*)
{
    return 1;
}

bool GameActionManager::onCardClicked(CardWidget* cardWidget)
{
    if (!mp_game->isAbleToRequest())
        return 0;

    switch(m_state) {
    case STATE_DISCARD:
        if (cardWidget->pocketType() != POCKET_HAND ||
            cardWidget->ownerId()    != mp_game->playerId())
            return 0;
        mp_game->serverConnection()->discardCard(cardWidget->cardData().id);
        return 1;

    case STATE_SELECT_PLAYER:
        unsetActiveCard();
        return 1;

    case STATE_SELECT_CARDS:
        Q_ASSERT(mp_activeCard != 0);
        if (cardWidget == mp_activeCard) {
            unsetActiveCard();
            return 1;
        }
        if (m_cardSelection.contains(cardWidget)) {
            removeFromSelection(cardWidget);
            return 1;
        }

        addToSelection(cardWidget);
        if (m_cardSelection.size() == m_selectionSize) {
            if (mp_activeCard->type() == Card::Character) {
                useAbilityWithCards();
            } else {
                playWithCards();
            }
            unsetActiveCard();
        }
        return 1;

    case STATE_MAIN:
        if (cardWidget->type() == Card::Character &&
            cardWidget->ownerId() == mp_game->playerId()) {
            onCharacterClicked(cardWidget);
            return 1;
        }

        if (cardWidget->pocketType() == POCKET_DECK) {
            mp_game->serverConnection()->drawCard();
            return 1;
        }

        if (cardWidget->cardData().id == 0)
            return 0;

        if (mp_game->gamePlayState() == GAMEPLAYSTATE_RESPONSE) {
            mp_game->serverConnection()->playCard(cardWidget->cardData().id);
            return 1;
        }

        if (cardWidget->pocketType() == POCKET_HAND && cardWidget->ownerId() == mp_game->playerId())
            onMainCardClicked(cardWidget);
        else
            mp_game->serverConnection()->playCard(cardWidget->cardData().id);
        return 1;
    }

    return 1;
}

void GameActionManager::onCardRightClicked(CardWidget* cardWidget)
{
    new CardZoomWidget(mp_game, cardWidget);
}

void GameActionManager::onPlayerClicked(PlayerWidget* playerWidget)
{
    if (!mp_game->isAbleToRequest() || m_state != STATE_SELECT_PLAYER ||
        playerWidget->id() == 0     || mp_activeCard == 0)
        return;

    if (mp_activeCard->type() == Card::Character) {
        mp_game->serverConnection()->useAbility(playerWidget->id());
    } else {
        mp_game->serverConnection()->playCardWithPlayer(mp_activeCard->cardData().id, playerWidget->id());
    }
    unsetActiveCard();
}

void GameActionManager::onEndTurnClicked()
{
    unsetActiveCard();
    mp_game->serverConnection()->endTurn();
}

void GameActionManager::onPassClicked()
{
    unsetActiveCard();
    mp_game->serverConnection()->pass();
}

void GameActionManager::setDiscardMode(bool inDiscardMode)
{
    unsetActiveCard();
    m_state = inDiscardMode ? STATE_DISCARD : STATE_MAIN;
}

void GameActionManager::onMainCardClicked(CardWidget* cardWidget)
{
    switch (cardWidget->cardData().type) {
        case CARD_BANG:
        case CARD_DUEL:
        case CARD_JAIL:
                selectPlayer(cardWidget);
                break;
        case CARD_PANIC:
        case CARD_CATBALOU:
                selectCards(cardWidget, 1);
                break;
        default:
            if (cardWidget->cardData().type == CARD_MISSED &&
                mp_game->character() == CHARACTER_CALAMITY_JANET) {
                selectPlayer(cardWidget);
                break;
            }
            mp_game->serverConnection()->playCard(cardWidget->cardData().id);
            break;
    }
}

void GameActionManager::onCharacterClicked(CardWidget* cardWidget)
{
    switch(mp_game->character()) {
    case CHARACTER_JESSE_JONES:
        selectPlayer(cardWidget);
        break;
    case CHARACTER_SID_KETCHUM:
        selectCards(cardWidget, 2);
        break;
    default:
        mp_game->serverConnection()->useAbility();
        break;
    }
}

void GameActionManager::selectPlayer(CardWidget* activeCard)
{
    Q_ASSERT(mp_activeCard == 0);
    Q_ASSERT(m_state == STATE_MAIN);
    mp_activeCard = activeCard;
    m_state = STATE_SELECT_PLAYER;
    mp_activeCard->setHighlight(1);
}

void GameActionManager::selectCards(CardWidget* activeCard, int cardCount)
{
    Q_ASSERT(mp_activeCard == 0);
    Q_ASSERT(m_state == STATE_MAIN);
    mp_activeCard = activeCard;
    m_state = STATE_SELECT_CARDS;
    m_cardSelection.clear();
    m_selectionSize = cardCount;
    mp_activeCard->setHighlight(1);
}

void GameActionManager::unsetActiveCard()
{
    if (m_state == STATE_SELECT_CARDS) {
        foreach (CardWidget* c, m_cardSelection)
            removeFromSelection(c);
    }
    if (mp_activeCard != 0) {
        mp_activeCard->setHighlight(0);
        mp_activeCard = 0;
    }
    m_state = STATE_MAIN;
}


void GameActionManager::addToSelection(CardWidget* card)
{
    m_cardSelection.append(card);
    if (m_cardSelection.size() != m_selectionSize)
        card->setHighlight(1);
}

void GameActionManager::removeFromSelection(CardWidget* card)
{
    m_cardSelection.removeAll(card);
    card->setHighlight(0);
}

void GameActionManager::useAbilityWithCards()
{
    QList<int> cards;
    foreach (CardWidget* card, m_cardSelection)
        cards.append(card->cardData().id);

    mp_game->serverConnection()->useAbility(cards);
}

void GameActionManager::playWithCards()
{
    if (m_cardSelection.size() == 1) {
        CardWidget* card = m_cardSelection[0];
        if (card->pocketType() == POCKET_HAND) {
            mp_game->serverConnection()->playCardWithPlayer(mp_activeCard->cardData().id, card->ownerId());
        } else {
            mp_game->serverConnection()->playCardWithCard(mp_activeCard->cardData().id, card->cardData().id);
        }
    } else {
        NOT_REACHED();
    }
}


void GameActionManager::debug(const QString& msg)
{
    qDebug(qPrintable(QString("[Click]         %1").arg(msg)));
}
