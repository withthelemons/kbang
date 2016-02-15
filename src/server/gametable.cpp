#include "gametable.h"
#include "common.h"
#include "player.h"
#include "util.h"
#include "gameeventmanager.h"
#include "gameexceptions.h"
#include "game.h"
#include "tablecard.h"
#include "cardfactory.h"
#include "checkdeckresulthandler.h"


GameTable::GameTable(Game* game):
        mp_game(game)
{
}


void GameTable::prepareGame(CardFactory* cardFactory) {
    generateCards(cardFactory);
    mp_game->gameEventManager().onGameSync();
    dealCards();
}



QList<const PlayingCard*> GameTable::playerDrawFromDeck(Player* player, int count, bool revealCards)
{
    QList<const PlayingCard*> drawedCards;
    for(int i = 0; i < count; ++i)
    {
        PlayingCard* card = popCardFromDeck();
        Q_ASSERT(!card->isVirtual());
        player->appendCardToHand(card);
        card->setOwner(player);
        card->setPocket(POCKET_HAND);
        drawedCards.append(card);
    }
    mp_game->gameEventManager().onPlayerDrawFromDeck(player, drawedCards, revealCards);
    return drawedCards;
}

void GameTable::playerDrawFromGraveyard(Player* player)
{
    if (m_graveyard.size() == 0)
        throw BadGameStateException();
    PlayingCard* card = m_graveyard.takeLast();
    Q_ASSERT(!card->isVirtual());
    player->appendCardToHand(card);
    card->setOwner(player);
    card->setPocket(POCKET_HAND);
    PlayingCard* nextCard = m_graveyard.isEmpty() ? 0 : m_graveyard.first();
    mp_game->gameEventManager().onPlayerDrawFromGraveyard(player, card, nextCard);
}

void GameTable::playerDiscardCard(PlayingCard* card)
{
    Q_ASSERT(!card->isVirtual());
    Player*     owner  = card->owner();
    PocketType  pocket = card->pocket();

    Q_ASSERT(pocket == POCKET_HAND || pocket == POCKET_TABLE);
    moveCardToGraveyard(card);

    mp_game->gameEventManager().onPlayerDiscardCard(owner, card, pocket);
    owner->checkEmptyHand();
}


void GameTable::playerPlayCard(PlayingCard* card)
{
    if (card->isVirtual())
        card = card->master();
    Q_ASSERT(card->pocket() == POCKET_HAND);
    Player* owner = card->owner();
    moveCardToGraveyard(card);
    mp_game->gameEventManager().onPlayerPlayCard(owner, card);
    owner->checkEmptyHand();
}

void GameTable::playerPlayCard(PlayingCard* card, Player* targetPlayer)
{
    if (card->isVirtual())
        card = card->master();
    Q_ASSERT(card->pocket() == POCKET_HAND);
    Player* owner = card->owner();
    moveCardToGraveyard(card);
    mp_game->gameEventManager().onPlayerPlayCard(owner, card, targetPlayer);
    owner->checkEmptyHand();
}

void GameTable::playerPlayCard(PlayingCard* card, PlayingCard* targetCard)
{
    if (card->isVirtual())
        card = card->master();
    Q_ASSERT(card->pocket() == POCKET_HAND);
    Q_ASSERT(!targetCard->isVirtual());
    Player* owner = card->owner();
    moveCardToGraveyard(card);
    mp_game->gameEventManager().onPlayerPlayCard(owner, card, targetCard);
    owner->checkEmptyHand();
}

void GameTable::playerPlayCardOnTable(TableCard* card, Player* targetPlayer)
{
    if (card->isVirtual())
        card = qobject_cast<TableCard*>(card->master());
    Q_ASSERT(card != 0);
    Q_ASSERT(card->pocket() == POCKET_HAND);
    Player* owner = card->owner();

    if (targetPlayer == 0)
        targetPlayer = owner;

    owner->removeCardFromHand(card);

    targetPlayer->appendCardToTable(card);
    card->setOwner(targetPlayer);
    card->setPocket(POCKET_TABLE);
    card->registerPlayer(targetPlayer);

    mp_game->gameEventManager().onPlayerPlayCardOnTable(owner, card, targetPlayer);
    owner->checkEmptyHand();
}

void GameTable::passTableCard(TableCard* card, Player* targetPlayer)
{
    Q_ASSERT(card->pocket() == POCKET_TABLE);
    if (card->isVirtual())
        card = qobject_cast<TableCard*>(card->master());
    Q_ASSERT(card != 0);
    Player* owner = card->owner();

    owner->removeCardFromTable(card);
    card->unregisterPlayer(owner);

    targetPlayer->appendCardToTable(card);
    card->setOwner(targetPlayer);
    card->setPocket(POCKET_TABLE);
    card->registerPlayer(targetPlayer);
    mp_game->gameEventManager().onPassTableCard(owner, card, targetPlayer);
}

void GameTable::playerPass(Player* player)
{
    mp_game->gameEventManager().onPlayerPass(player);
}

void GameTable::playerRespondWithCard(PlayingCard* card)
{
    if (card->isVirtual())
        card = card->master();
    Q_ASSERT(card->pocket() == POCKET_HAND);
    Player* owner = card->owner();
    moveCardToGraveyard(card);
    mp_game->gameEventManager().onPlayerRespondWithCard(owner, card);
    owner->checkEmptyHand();
}

void GameTable::drawIntoSelection(int count, Player* selectionOwner)
{
    Q_ASSERT(m_selection.isEmpty());
    QList<const PlayingCard*> drawedCards;
    for(int i = 0; i < count; ++i)
    {
        PlayingCard* card = popCardFromDeck();
        Q_ASSERT(!card->isVirtual());
        m_selection.append(card);
        card->setOwner(selectionOwner);
        card->setPocket(POCKET_SELECTION);
        drawedCards.append(card);
    }
    mp_game->gameEventManager().onDrawIntoSelection(selectionOwner, drawedCards);
}

void GameTable::playerPickFromSelection(Player* player, PlayingCard* card)
{
    if (card->isVirtual())
        card = card->master();
    bool revealCard = (card->owner() != player);
    m_selection.removeAll(card);
    player->appendCardToHand(card);
    card->setOwner(player);
    card->setPocket(POCKET_HAND);
    mp_game->gameEventManager().onPlayerPickFromSelection(player, card, revealCard);
}

void GameTable::undrawFromSelection(PlayingCard* card)
{
    Q_ASSERT(m_selection.contains(card));
    Player* owner = card->owner();
    m_selection.removeAll(card);
    putCardToDeck(card);
    card->setOwner(0);
    card->setPocket(POCKET_DECK);
    mp_game->gameEventManager().onUndrawFromSelection(card, owner);
}

/*
void GameTable::playerCheckDeck(Player* player, PlayingCard* causedBy, bool (*checkFunc)(PlayingCard*),
                                CheckDeckResultHandler* resultHandler)
{
    if (causedBy->isVirtual())
        causedBy = 0;
    PlayingCard* checkedCard = popCardFromDeck();
    Q_ASSERT(!checkedCard->isVirtual());
    putCardToGraveyard(checkedCard);
    checkedCard->setOwner(0);
    checkedCard->setPocket(POCKET_GRAVEYARD);
    bool checkResult = (*checkFunc)(checkedCard);
    mp_game->gameEventManager().onPlayerCheckDeck(player, checkedCard, causedBy, checkResult);
    resultHandler->checkResult(checkResult);
}
*/

PlayingCard* GameTable::checkDeck()
{
    PlayingCard* checkedCard = popCardFromDeck();
    Q_ASSERT(!checkedCard->isVirtual());
    putCardToGraveyard(checkedCard);
    checkedCard->setOwner(0);
    checkedCard->setPocket(POCKET_GRAVEYARD);
    mp_game->gameEventManager().onCancelCard(0, POCKET_DECK, checkedCard, 0);
    return checkedCard;
}

void GameTable::playerStealCard(Player* player, PlayingCard* card)
{
    Q_ASSERT(!card->isVirtual());
    PocketType pocket = card->pocket();
    Player*    owner  = card->owner();

    switch(pocket) {
    case POCKET_HAND:
        owner->removeCardFromHand(card);
        break;
    case POCKET_TABLE:
        owner->removeCardFromTable(card);
        (qobject_cast<TableCard*>(card))->unregisterPlayer(owner);
        break;
    default:
        NOT_REACHED();
    }

    player->appendCardToHand(card);
    card->setOwner(player);
    card->setPocket(POCKET_HAND);

    mp_game->gameEventManager().onPlayerStealCard(player, owner, pocket, card);
    owner->checkEmptyHand();
}


void GameTable::cancelCard(PlayingCard* card, Player* player)
{
    Q_ASSERT(!card->isVirtual());
    PocketType pocket = card->pocket();
    Player*    owner  = card->owner();

    moveCardToGraveyard(card);

    mp_game->gameEventManager().onCancelCard(owner, pocket, card, player);
    if (owner != 0 && pocket == POCKET_HAND)
        owner->checkEmptyHand();
}

void GameTable::cancelSelection()
{
    while(!m_selection.isEmpty()) {
        cancelCard(m_selection.first());
    }
}

bool GameTable::isEmptyGraveyard() const
{
    return m_graveyard.isEmpty();
}

PlayingCard* GameTable::graveyardTop() const
{
    if (m_graveyard.isEmpty()) return 0;
    return m_graveyard.last();
}

void GameTable::generateCards(CardFactory* cardFactory)
{
    m_cards = cardFactory->generateCards(mp_game);
    m_deck << m_cards.values();
    shuffleDeck();
}

void GameTable::shuffleDeck()
{
    shuffleList(m_deck);
}


void GameTable::dealCards()
{
    int cardCount = 0, players = 0;
    do {
        players = 0;
        foreach(Player* p, mp_game->playerList())
        {
            if (p->initialCardCount() > cardCount)
            {
                playerDrawFromDeck(p);
                players++;
            }
        }
        cardCount++;
    } while(players != 0);
}

void GameTable::regenerateDeck()
{
    Q_ASSERT(m_deck.isEmpty());
    Q_ASSERT(!m_graveyard.isEmpty());
    m_deck << m_graveyard;
    m_graveyard.clear();
    m_graveyard << m_deck.takeLast();
    shuffleDeck();
    mp_game->gameEventManager().onDeckRegenerate();
}

void GameTable::moveCardToGraveyard(PlayingCard* card)
{
    Player* owner = card->owner();
    switch(card->pocket()) {
    case POCKET_HAND:
        owner->removeCardFromHand(card);
        break;
    case POCKET_TABLE:
        owner->removeCardFromTable(card);
        (qobject_cast<TableCard*>(card))->unregisterPlayer(owner);
        break;
    case POCKET_SELECTION:
        m_selection.removeAll(card);
        break;
    case POCKET_DECK:
        if (!m_deck.isEmpty() && m_deck.first() == card) {
            popCardFromDeck();
        }
    default:
        NOT_REACHED();
    }

    putCardToGraveyard(card);
    card->setOwner(0);
    card->setPocket(POCKET_GRAVEYARD);
}


PlayingCard* GameTable::popCardFromDeck()
{
    if (m_deck.isEmpty()) regenerateDeck();
    return m_deck.takeFirst();
}

void GameTable::putCardToDeck(PlayingCard* card)
{
    m_deck.push_front(card);
}

void GameTable::putCardToGraveyard(PlayingCard* card)
{
    m_graveyard.push_back(card);
}
