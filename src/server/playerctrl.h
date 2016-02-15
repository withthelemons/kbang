#ifndef PLAYERCTRL_H
#define PLAYERCTRL_H

#include <QObject>

#include "gameexceptions.h"
#include "parser/parserstructs.h"

class Player;
class GameEventListener;
class PublicGameView;
class PublicPlayerView;
class PrivatePlayerView;
class PlayingCard;

/**
 * This class provides the only "correct" form to
 * control a player. It means that any code that
 * wants to control a player needs to do it throught
 * this API. It's used primarily by AIs and the
 * adaptor between this class and xml parser.
 */
class PlayerCtrl :  QObject
{
    Q_OBJECT;
    friend class Player;
public slots:
    /**
     * Disconnects the player from the game, or possibly subtitutes player with
     * AI. This method should never be called by AIs
     * Typically you want to call this method after the remote client
     * disconnected or requested a disconnection.
     */
    void disconnect();

    /**
     * Starts the yet not started game.
     * \throws BadPlayerException The player does not have the permission to
     *         start the game.
     * \throws BadGameStateException Either the game is already started, or
     *         the right conditions are not met, eg. there is not enough
     *         players in the game.
     */
    void startGame();


    /**
     * Player draws two cards in his draw phase.
     * \throws BadGameStateException It's not right time to draw cards.
     */
    void draw();

    /**
     * Finishes the player's turn.
     * \throws TooManyCardsInHandException The player must discard some cards,
     *         before he can finish his turn.
     * \throws BadGameStateException The player cannot finish his turn, because
     *         it's not the right moment.
     */
    void finishTurn();

    /**
     * Discards a card.
     * \param cardId The id of the card.
     * \throws BadCardException There is no card with given id in player's hand.
     * \throws BadGameStateException There is no right moment for discarding cards.
     */
    void discardCard(PlayingCard* card);


    void useAbility();
    void useAbility(const PublicPlayerView* targetPlayer);
    void useAbility(QList<PlayingCard*> cards);

    /**
     * Plays a card.
     */
    void playCard(PlayingCard* card);
    void playCard(PlayingCard* card, const PublicPlayerView* player);
    void playCard(PlayingCard* card, PlayingCard* targetCard);
    void pass();


    void sendChatMessage(const QString&);

public:
    PlayingCard* card(int cardId) const;


    const PublicGameView& publicGameView() const;
    const PublicPlayerView& publicPlayerView(int playerId = 0) const;
    const PrivatePlayerView& privatePlayerView() const;



    /**
     * Creates a new game and automatically puts the player in. This should be used
     * exclusively by nonAI client classes, as AIs typically don't create games.
     * \note The client class gets the PlayerCtrl instance through the
     *       GameEventListener::onPlayerInit(PlayerCtrl*) method.
     * \param game The game structure.
     * \param player The player structure.
     */
    static void createGame(const CreateGameData&, const CreatePlayerData&, GameEventListener*);

    /**
     * Puts the player to the game.
     * \note The client class gets the PlayerCtrl instance through the
     *       GameEventListener::onPlayerInit(PlayerCtrl*) method.
     *
     * \param gameId The id of the game.
     * \param player The player structure.
     * \throws BadGameException The game does not exist.
     * \throws BadGameStateException The game has already started.
     */
    static void joinGame(int gameId, const QString& gamePassword, const CreatePlayerData&, GameEventListener*);

    static void replacePlayer(int gameId, int playerId, const QString& password, const CreatePlayerData&, GameEventListener*);


    /**
     * Returns the StructServerInfo struct.
     */
    static ServerInfoData serverInfo();

    /**
     * Returns the list of PublicGameViews.
     */
    static QList<const PublicGameView*> publicGameList();

    /**
     * Returns the PublicGameView of the game specified by gameId.
     */
    static const PublicGameView& publicGameView(int gameId);


private:
    PlayerCtrl(Player* player);
    PlayerCtrl(const PlayerCtrl&): QObject(0) {}
    PlayerCtrl& operator=(const PlayerCtrl&) { return *this; }

    Player*                     mp_player;

};

#endif // PLAYERCTRL_H
