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
#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QWidget>
#include <QHash>
#include <QList>
#include <QQueue>
#include <QPushButton>
#include "parser/parserstructs.h"
#include "common.h"
#include "playerwidget.h"

#include "cardwidgetfactory.h"
#include "gameactionmanager.h"

namespace client {

class ServerConnection;
class GameEventHandler;
class CardWidgetSizeManager;

class DeckWidget;
class GraveyardWidget;

/**
 * @author MacJariel <MacJariel@gmail.com>
 */
class Game: public QObject {
Q_OBJECT;
public:
    Game(QObject* parent, int gameId, ClientType, ServerConnection*, const GameWidgets&);
    virtual ~Game();

    void setPlayerId(int playerId);
    void setGameState(const GameState&);
    void setGameContext(const GameContextData&);
    void setSelection(QList<CardData>);
    void setIsCreator(bool isCreator);
    void setGraveyard(const CardData&);
    void validate();
    void clear();

    void setTextInfo(const QString&);
    void unsetTextInfo();


    void playerJoinedGame(const PublicPlayerData& player);
    void playerLeavedGame(int playerId);
    void playerUpdate(const PublicPlayerData& player);
public slots:
    void gameCanBeStarted(bool);
    void startButtonClicked();

public:
    inline DeckWidget*     deck() const               { return mp_deck; }
    inline GraveyardWidget*graveyard() const          { return mp_graveyard; }
    inline CardListWidget*       selection() const          { return mp_selection; }
    inline PlayerWidget*   playerWidget(int id) const { return m_players.contains(id) ? m_players[id] : 0; }
    inline LocalPlayerWidget*
                           localPlayerWidget() const  { return mp_localPlayerWidget; }
    inline QWidget*        mainWidget() const         { return mp_mainWidget; }

    inline int             currentPlayerId() const    { return m_gameContextData.currentPlayerId; }
    inline int             requestedPlayerId() const  { return m_gameContextData.requestedPlayerId; }
    inline GamePlayState   gamePlayState() const      { return m_gameContextData.gamePlayState; }
    inline bool            isCreator() const          { return m_isCreator; }
    inline bool            isAbleToRequest() const    { return requestedPlayerId() == m_playerId; }
    inline bool            isPlaying() const          { return m_gameState == GAMESTATE_PLAYING; }
    inline bool            isFinished() const         { return m_gameState == GAMESTATE_FINISHED; }

    inline bool            gameInterfaceLoaded() const { return m_interface == GameInterface; }

    inline int playerId() const { return m_playerId; }
    CharacterType character() const;

    inline OpponentWidget* opponentWidget(int index) { return m_opponentWidgets[index]; }

    void assignPlayerWidget(int playerId, PlayerWidget*);

    inline GameActionManager* gameActionManager() { return &m_gameActionManager; }
    inline CardWidgetFactory*       cardWidgetFactory()  { return &m_cardWidgetFactory; }

    inline ServerConnection* serverConnection() { return mp_serverConnection; }

    void sendLogMessage(const QString&);

    void pauseGameEvents();
    void resumeGameEvents();

signals:
    void emitLogMessage(const QString&);

private:
    void loadCreatorInterface();
    void unloadCreatorInterface();
    void loadGameInterface();
    void unloadGameInterface();
    void unloadInterface();


private:
    int             m_gameId;
    int             m_playerId;
    bool            m_isCreator;
    GameState       m_gameState;
    GameContextData m_gameContextData;

    enum {
        NoInterface = 0,
        CreatorInterface,
        GameInterface
    } m_interface;

    ServerConnection* mp_serverConnection;


/*  Visual elements - provided by MainWindow */
    LocalPlayerWidget*        mp_localPlayerWidget;
    QList<OpponentWidget*>    m_opponentWidgets;
    QWidget*                  mp_mainWidget;
    QWidget*                  mp_middleWidget;
    QLabel*                   mp_statusLabel;



    QHash<int, PlayerWidget*> m_players;
    QPushButton*              mp_startButton;
    DeckWidget*               mp_deck;
    GraveyardWidget*          mp_graveyard;
    CardListWidget*                 mp_selection;
    QQueue<CardMovementData> m_cardMovementQueue;

    GameEventHandler*         mp_gameEventHandler;
    CardWidgetFactory         m_cardWidgetFactory;
    GameActionManager    m_gameActionManager;



};
}
#endif
