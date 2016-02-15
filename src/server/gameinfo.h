/***************************************************************************
 *   Copyright (C) 2008 by MacJariel                                       *
 *   echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil"                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef GAMEINFO_H
#define GAMEINFO_H

#include "parser/parserstructs.h"



class GameInfo
{
public:
    GameInfo(const CreateGameData&);

    inline QString  name()                  const { return m_name; }
    inline QString  description()           const { return m_description; }
    inline int      minPlayers()            const { return m_minPlayers; }
    inline int      maxPlayers()            const { return m_maxPlayers; }
    inline int      maxSpectators()         const { return m_maxSpectators; }
    inline int      AIPlayers()             const { return m_AIPlayers; }
    inline bool     hasPlayerPassword()     const { return !m_playerPassword.isNull(); }
    inline bool     hasSpectatorPassword()  const { return !m_spectatorPassword.isNull(); }
    inline int      creatorId()             const { return m_creatorId; }
    inline bool     hasShufflePlayers()     const { return m_shufflePlayersFlag; }

    inline bool comparePlayerPassword(const QString& password) const {
        return password == m_playerPassword;
    }

    inline bool compareSpectatorPassword(const QString& password) const {
        return (password == m_playerPassword || password == m_spectatorPassword);
    }

    //StructGame structGame() const;

    void setCreatorId(int creatorId) {
        m_creatorId = creatorId;
    }


private:
    QString m_name;
    QString m_description;
    int     m_creatorId;
    int     m_minPlayers;
    int     m_maxPlayers;
    int     m_maxSpectators;
    int     m_AIPlayers;
    QString m_playerPassword;
    QString m_spectatorPassword;
    bool    m_shufflePlayersFlag;
};

#endif // GAMEINFO_H
