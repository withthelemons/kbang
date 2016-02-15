#include "gameinfo.h"

GameInfo::GameInfo(const CreateGameData& createGameData):
    m_name(createGameData.name),
    m_description(createGameData.description),
    m_creatorId(0),
    m_minPlayers(createGameData.minPlayers),
    m_maxPlayers(createGameData.maxPlayers),
    m_maxSpectators(createGameData.maxSpectators),
    m_AIPlayers(createGameData.AIPlayers),
    m_playerPassword(createGameData.playerPassword),
    m_spectatorPassword(createGameData.spectatorPassword),
    m_shufflePlayersFlag(createGameData.flagShufflePlayers)
{
    Q_ASSERT(!m_name.isEmpty());
    Q_ASSERT(m_minPlayers <= m_maxPlayers);
    Q_ASSERT(m_spectatorPassword.isNull() || !m_playerPassword.isNull()); // observerPassword implies playerPassword
}


//StructGame GameInfo::structGame() const
//{
//    StructGame x;
//    x.id = m_id;
//    x.creatorId = m_creatorId;
//    x.name = m_name;
//    x.description = m_description;
//    x.minPlayers = m_minPlayers;
//    x.maxPlayers = m_maxPlayers;
//    x.maxSpectators = m_maxSpectators;
//    x.playerPassword = m_playerPassword;
//    x.spectatorPassword = m_spectatorPassword;
//    x.hasPlayerPassword = (!m_playerPassword.isNull());
//    x.hasSpectatorPassword = (!m_spectatorPassword.isNull());
//    x.flagShufflePlayers = m_shufflePlayersFlag;
//    return x;
//}
