#ifndef GAMECYCLE_H
#define GAMECYCLE_H

#include "parser/parserstructs.h"

#include <QObject>
#include <QQueue>

class Game;
class Player;
class PlayingCard;
class ReactionHandler;
class CheckDeckResultHandler;


class GameCycle: public QObject
{
Q_OBJECT
public:
    GameCycle(Game* game);
    ~GameCycle();

    inline  GamePlayState   gamePlayState()   const { return m_state; }
    inline  Player*         currentPlayer()   const { return mp_currentPlayer; }
    inline  Player*         requestedPlayer() const { return mp_requestedPlayer; }
    inline  int             turnNumber()      const { return m_turnNum; }

    inline bool isDraw()     const { return m_state == GAMEPLAYSTATE_DRAW; }
    inline bool isTurn()     const { return m_state == GAMEPLAYSTATE_TURN; }
    inline bool isResponse() const { return m_state == GAMEPLAYSTATE_RESPONSE; }
    inline bool isDiscard()  const { return m_state == GAMEPLAYSTATE_DISCARD; }

    void assertDraw() const;
    void assertTurn() const;
    void assertResponse() const;
    void assertDiscard() const;

    inline ReactionHandler* reactionHandler() const { return isResponse() ? m_reactionHandlers.head() : 0; }

    GameContextData gameContextData() const;

    void start();


    /* Methods accessible from DRAW phase */

    /** The current player will <emph>draw</emph> cards. By default
     * the player will draw two cards from deck.
     *
     * \throws BadPlayerException       called by non-playing player.
     * \throws BadStateException        called in other than Draw state or requesting too many cards
     * \throws PrematureDrawException   missed card-effect - BOOM/PRISON (todo)
     *
     * \param player The calling player
     */
    void draw(Player* player, bool specialDraw = 0);

    void skipPlayersTurn();

    void finishTurn(Player* player);

    /* Methods accessible from TURN phase */
    // + seznam karet
    void discardCard(Player* player, PlayingCard* card);


    void playCard(Player* player, PlayingCard* card);
    void playCard(Player* player, PlayingCard* card, Player* targetPlayer);
    void playCard(Player* player, PlayingCard* card, PlayingCard* targetCard);
    void pass(Player* player);

    void useAbility(Player* player);
    void useAbility(Player* player, Player* targetPlayer);
    void useAbility(Player* player, QList<PlayingCard*> cards);

    void checkDeck(Player* player, PlayingCard* causedBy, bool (*checkFunc)(PlayingCard*), CheckDeckResultHandler*);

    void setResponseMode(ReactionHandler* reactionHandler, Player* requestedPlayer, bool skipQueue = 0);
    void unsetResponseMode();

    void setCardEffect(bool isCardEffect);
    bool isCardEffect() const;

signals:
    void cardEffectOver();

private:
    Game*                       mp_game;
    GamePlayState               m_state;
    GamePlayState               m_lastState;
    Player*                     mp_currentPlayer;
    Player*                     mp_requestedPlayer;
    QQueue<ReactionHandler*>    m_reactionHandlers;
    QQueue<Player*>             m_reactionPlayers;

    int     m_drawCardCount;
    int     m_drawCardMax;
    int     m_turnNum;
    bool    m_contextDirty;
    bool    m_isCardEffect;

private:
    void    sendRequest();
    void    checkPlayerAndState(Player* player, GamePlayState state);
    void    startTurn(Player* player);
    int     needDiscard(Player* player);
    void    announceContextChange();
};

#endif // GAMECYCLE_H
