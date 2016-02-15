#include "gamesyncevent.h"
#include "game.h"
#include "localplayerwidget.h"
#include "opponentwidget.h"


using namespace client;

GameSyncEvent::GameSyncEvent(Game* game, const GameSyncData& gameSyncData):
        GameEvent(game),
        m_gameSyncData(gameSyncData)
{
}

GameSyncEvent::~GameSyncEvent()
{
}

void GameSyncEvent::run()
{
    qDebug("GameSyncEvent");
    int index = 0;
    Q_ASSERT(m_gameSyncData.players.size() <= 7);
    while (index < m_gameSyncData.players.size() &&
           m_gameSyncData.players[index].id != m_gameSyncData.localPlayer.id)
        ++index;
    Q_ASSERT(index < m_gameSyncData.players.size());


    mp_game->setPlayerId(m_gameSyncData.localPlayer.id);
    mp_game->setGameState(m_gameSyncData.state);
    mp_game->setIsCreator(m_gameSyncData.isCreator);
    mp_game->validate();

    mp_game->setGraveyard(m_gameSyncData.graveyard);

    mp_game->localPlayerWidget()->setFromPublicData(m_gameSyncData.players[index]);
    mp_game->localPlayerWidget()->setFromPrivateData(m_gameSyncData.localPlayer);
    mp_game->assignPlayerWidget(m_gameSyncData.localPlayer.id, mp_game->localPlayerWidget());

    int localPlayerIndex = index;
    index++;

    int opponentIndex = 0;
    while (index < m_gameSyncData.players.size()) {
        mp_game->opponentWidget(opponentIndex)->setFromPublicData(m_gameSyncData.players[index]);
        mp_game->assignPlayerWidget(m_gameSyncData.players[index].id, mp_game->opponentWidget(opponentIndex));
        index++;
        opponentIndex++;
    }
    int firstUnusedOpponentIndex = opponentIndex;

    index = localPlayerIndex - 1;
    opponentIndex = 5;
    while (index >= 0) {
        mp_game->opponentWidget(opponentIndex)->setFromPublicData(m_gameSyncData.players[index]);
        mp_game->assignPlayerWidget(m_gameSyncData.players[index].id, mp_game->opponentWidget(opponentIndex));
        index--;
        opponentIndex--;
    }
    int lastUnusedOpponentIndex = opponentIndex;
    for (int i = firstUnusedOpponentIndex; i <= lastUnusedOpponentIndex; ++i) {
        mp_game->opponentWidget(i)->clear();
    }

    mp_game->setGameContext(m_gameSyncData.gameContext);
    mp_game->setSelection(m_gameSyncData.selection);

    emit finished(this);
}


bool GameSyncEvent::isReadyRun()
{
    return 1;
}


void GameSyncEvent::setLocalPlayer(int index)
{
    mp_game->localPlayerWidget()->setFromPublicData(m_gameSyncData.players[index]);
    mp_game->localPlayerWidget()->setFromPrivateData(m_gameSyncData.localPlayer);
}

