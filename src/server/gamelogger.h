#ifndef GAMELOGGER_H
#define GAMELOGGER_H

#include <fstream>

#include "gameeventlistener.h"


class GameLogger : public GameEventListener
{
public:
    GameLogger();

    virtual bool isAI() { return 1; }

    virtual void onHandlerRegistered(const PublicGameView* publicGameView, PlayerCtrl* playerCtrl);
    virtual void onHandlerUnregistered();

    virtual void onGameStartabilityChanged(bool) {}

    virtual void onChatMessage(PublicPlayerView&, const QString&) {}
    virtual void onGameSync() {}
    virtual void onPlayerJoinedGame(PublicPlayerView&) {}
    virtual void onPlayerLeavedGame(PublicPlayerView&) {}
    virtual void onPlayerUpdated(PublicPlayerView&) {}
    virtual void onPlayerDied(PublicPlayerView&, PublicPlayerView* causedBy);
    virtual void onGameStarted();
    virtual void onGameFinished();
    virtual void onPlayerDrawFromDeck(PublicPlayerView&, QList<const PlayingCard*>, bool);
    virtual void onPlayerDrawFromGraveyard(PublicPlayerView&, const PlayingCard*, const PlayingCard*);
    virtual void onPlayerDiscardCard(PublicPlayerView&, const PlayingCard*, PocketType);
    virtual void onPlayerPlayCard(PublicPlayerView&, const PlayingCard*);
    virtual void onPlayerPlayCard(PublicPlayerView&, const PlayingCard*, PublicPlayerView&);
    virtual void onPlayerPlayCard(PublicPlayerView&, const PlayingCard*, const PlayingCard*, PublicPlayerView*);
    virtual void onPlayerPlayCardOnTable(PublicPlayerView&, const PlayingCard*, PublicPlayerView&);
    virtual void onPassTableCard(PublicPlayerView&, const PlayingCard*, PublicPlayerView&);
    virtual void onPlayerRespondWithCard(PublicPlayerView&, const PlayingCard*);
    virtual void onPlayerPass(PublicPlayerView&);
    virtual void onDrawIntoSelection(QList<const PlayingCard*>);
    virtual void onPlayerPickFromSelection(PublicPlayerView&, const PlayingCard*);
    virtual void onUndrawFromSelection(const PlayingCard* card);
    virtual void onPlayerCheckDeck(PublicPlayerView&, const PlayingCard*, const PlayingCard*, bool);
    virtual void onPlayerStealCard(PublicPlayerView&, PublicPlayerView&, PocketType, const PlayingCard*);
    virtual void onCancelCard(PocketType, const PlayingCard*, PublicPlayerView*,  PublicPlayerView*);
    virtual void onGameContextChange(const GameContextData&);
    virtual void onLifePointsChange(PublicPlayerView&, int, PublicPlayerView*);
    virtual void onDeckRegenerate();
    virtual void onPlayerUseAbility(PublicPlayerView&);

    virtual void onActionRequest(ActionRequestType) {}

private:

    QString cardToString(const PlayingCard*);
    QString cardListToString(QList<const PlayingCard*>);

    const PublicGameView* mp_publicGameView;
    std::ofstream m_logFile;
};

#endif // GAMELOGGER_H
