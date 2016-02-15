#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <QtCore>

class PlayingCard;
class TableCard;
class Player;
class Game;
class CardFactory;
class CheckDeckResultHandler;


class GameTable
{
    friend class Game;
public:
    GameTable(Game* game);


    /**
     * Player draws specified amount of cards to his hands. If revealCards
     * is set, the other players can see the cards.
     */
    QList<const PlayingCard*> playerDrawFromDeck(Player*, int count = 1, bool revealCards = 0);

    void playerDrawFromGraveyard(Player*);

    /**
     * Card's owner discards a specified card. The card must be either in
     * player's hand or on player's table.
     */
    void playerDiscardCard(PlayingCard*);

    /**
     * Card's owner plays a card (puts it into graveyard) without specifying any target.
     */
    void playerPlayCard(PlayingCard*);

    /**
     * Card's owner plays a card (puts it into graveyard) with specifying target player.
     */
    void playerPlayCard(PlayingCard*, Player* targetPlayer);

    /**
     * Card's owner plays a card (puts it into graveyard) with specifying target card.
     */
    void playerPlayCard(PlayingCard*, PlayingCard* targetCard);

    /**
     * Card's owner plays a card on his (or other player's) table. If targetPlayer is
     * not NULL, the card is played on that player's table (e.g. Jail).
     */
    void playerPlayCardOnTable(TableCard*, Player* targetPlayer = 0);

    /**
     * The specified card is passed to the table of targetPlayer. The specified
     * card must be already on table. (e.g. used by Dynamite passing)
     */
    void passTableCard(TableCard*, Player* targetPlayer);

    void playerPass(Player* player);

    void playerRespondWithCard(PlayingCard* card);

    /**
     * The specified amount of cards is drawn to the card selection. If selectionOwner
     * is NULL, the cards are revealed to everyone. Otherwise only for that player.
     */
    void drawIntoSelection(int count, Player* selectionOwner = 0);

    /**
     * The player picks a card from selection.
     */
    void playerPickFromSelection(Player*, PlayingCard*);

    void undrawFromSelection(PlayingCard* card);

    /**
     * The player checks the deck and returns the result.
     */
    //void playerCheckDeck(Player*, PlayingCard*, bool (*)(PlayingCard*), CheckDeckResultHandler*);

    PlayingCard* checkDeck();

    /**
     * The player steals a card from other player.
     */
    void playerStealCard(Player*, PlayingCard*);

    /**
     * The target card is canceled. If player is not NULL,
     * he is the one that initiated the cancelation.
     */
    void cancelCard(PlayingCard*, Player* player = 0);

    void cancelSelection();

    bool isEmptyGraveyard() const;

    PlayingCard* graveyardTop() const;

    PlayingCard* card(int cardId) const {
        return (m_cards.contains(cardId)) ? m_cards[cardId] : 0;
    }

    QList<PlayingCard*> selection() const { return m_selection; }

private:
    void generateCards(CardFactory*);
    void shuffleDeck();
    void dealCards();
    void regenerateDeck();
    void moveCardToGraveyard(PlayingCard*);
    inline PlayingCard* popCardFromDeck();
    inline void putCardToDeck(PlayingCard*);
    inline void putCardToGraveyard(PlayingCard*);

protected:
   /**
    * Prepares the game.
    */
    void prepareGame(CardFactory*);

private:
    Game*                          mp_game;
    QList<PlayingCard*>            m_deck;
    QList<PlayingCard*>            m_graveyard;
    QList<PlayingCard*>            m_selection;
    QMap<int, PlayingCard*>        m_cards;
};

#endif // GAMETABLE_H
