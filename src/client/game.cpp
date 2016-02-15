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
#include "game.h"
#include "serverconnection.h"
#include "parser/queryget.h"
#include "localplayerwidget.h"
#include "opponentwidget.h"
#include "cardlistwidget.h"
#include "gameeventhandler.h"
#include "deckwidget.h"
#include "graveyardwidget.h"
#include "cardwidgetsizemanager.h"

#include <QtDebug>
#include <QBoxLayout>

using namespace client;

Game::Game(QObject* parent, int gameId, ClientType clientType,
           ServerConnection* serverConnection, const GameWidgets& gameWidgets):
        QObject(parent),
        m_gameId(gameId),
        m_playerId(0),
        m_isCreator(0),
        m_gameState(GAMESTATE_INVALID),
        m_interface(NoInterface),
        mp_serverConnection(serverConnection),
        mp_localPlayerWidget(gameWidgets.localPlayerWidget),
        m_opponentWidgets(gameWidgets.opponentWidget),
        mp_mainWidget(gameWidgets.mainWidget),
        mp_middleWidget(gameWidgets.middleWidget),
        mp_statusLabel(gameWidgets.statusLabel),
        mp_startButton(0),
        mp_deck(0),
        mp_graveyard(0),
        mp_selection(0),
        m_cardWidgetFactory(this),
        m_gameActionManager(this)

{
    Q_UNUSED(clientType);
    mp_localPlayerWidget->enterGameMode(this);
    foreach(OpponentWidget* opponentWidget, m_opponentWidgets) {
        opponentWidget->enterGameMode(this);
    }
    mp_gameEventHandler = new GameEventHandler(this);
    mp_gameEventHandler->connectSlots(mp_serverConnection->parser());
}

Game::~Game()
{
}


void Game::setPlayerId(int playerId)
{
    m_playerId = playerId;
}

void Game::setGameState(const GameState& gameState)
{
    m_gameState = gameState;
}

void Game::setGameContext(const GameContextData& gameContextData)
{
    if (m_gameState != GAMESTATE_PLAYING && m_gameState != GAMESTATE_FINISHED)
        return;

    PlayerWidget* currentPlayer = playerWidget(currentPlayerId());
    PlayerWidget* requestedPlayer = playerWidget(requestedPlayerId());
    if (currentPlayer) {
        currentPlayer->setCurrent(0);
        currentPlayer->update();
    }

    if (requestedPlayer) {
        requestedPlayer->setRequested(0);
        requestedPlayer->update();
    }
    m_gameContextData = gameContextData;
    currentPlayer = playerWidget(currentPlayerId());
    requestedPlayer = playerWidget(requestedPlayerId());

    if (currentPlayer) {
        currentPlayer->setCurrent(1);
        currentPlayer->update();
    } else {
        qWarning("Cannot set active player.");
    }

    if (requestedPlayer) {
        requestedPlayer->setRequested(1);
        requestedPlayer->update();
    } else {
        qWarning("Cannot set requested player.");
    }

    mp_localPlayerWidget->setFromContext(gameContextData);


    if (gamePlayState() == GAMEPLAYSTATE_DRAW) {
        emit emitLogMessage("<br />");
    }


    if (!requestedPlayer || !requestedPlayer->isLocalPlayer()) {
        unsetTextInfo();
        return;
    }



    if (gamePlayState() == GAMEPLAYSTATE_RESPONSE) {
        PlayerWidget* causedBy = gameContextData.causedBy ? playerWidget(gameContextData.causedBy) : 0;
        QString causedByName = causedBy ? causedBy->name() : "";
        QString message;
        switch(gameContextData.reactionType) {
        case REACTION_BANG:
            message = tr("<i>%1</i> played Bang! on you!").arg(causedByName);
            break;
        case REACTION_DUEL:
            message = tr("You are in duel with <i>%1</i>.").arg(causedByName);
            break;
        case REACTION_GATLING:
            message = tr("<i>%1</i> played Gatling.").arg(causedByName);
            break;
        case REACTION_GENERALSTORE:
            message = tr("Shopping time. Please pick a card.").arg(causedByName);
            break;
        case REACTION_INDIANS:
            message = tr("The Indians lead by <i>%1</i> are in town.").arg(causedByName);
            break;
        case REACTION_LASTSAVE:
            message = tr("You've just got the fatal shot. Ya have beer?");
            break;
        case REACTION_KITCARLSON:
            message = tr("Pick two cards from selection.");
            break;
        case REACTION_LUCKYDUKE:
            message = tr("Feelin' lucky? Pick a card to respond with.");
            break;
        case REACTION_NONE:
            NOT_REACHED();
        }
        setTextInfo(message);
    } else if (gamePlayState() == GAMEPLAYSTATE_DRAW) {
        setTextInfo(tr("It's your turn. Good luck!"));
    } else if (gamePlayState() == GAMEPLAYSTATE_DISCARD) {
        setTextInfo(tr("You need to discard some cards!"));
    } else{
        unsetTextInfo();
    }
}

void Game::setSelection(QList<CardData> cards)
{
    if (m_gameState != GAMESTATE_PLAYING && m_gameState != GAMESTATE_FINISHED)
        return;

    mp_selection->clear();
    foreach(const CardData& card, cards) {
        CardWidget* cardWidget = m_cardWidgetFactory.createPlayingCard(0);
        cardWidget->setCardData(card);
        mp_selection->push(cardWidget);
    }
}

void Game::setIsCreator(bool isCreator)
{
    m_isCreator = isCreator;
}

void Game::setGraveyard(const CardData& data)
{
    if (m_interface != GameInterface)
        return;
    mp_graveyard->setFirstCard(data);
}

void Game::validate()
{
    switch(m_gameState) {
    case GAMESTATE_INVALID:
    case GAMESTATE_WAITINGFORPLAYERS:
        if (m_isCreator) {
            if (m_interface == CreatorInterface) return;
            unloadInterface();
            loadCreatorInterface();
        } else {
            unloadInterface();
        }
        break;
    case GAMESTATE_PLAYING:
    case GAMESTATE_FINISHED:
        unloadInterface();
        loadGameInterface();
        break;
    }
}

void Game::clear()
{
    unloadInterface();
    mp_localPlayerWidget->leaveGameMode();
    foreach(OpponentWidget* opponentWidget, m_opponentWidgets)
        opponentWidget->leaveGameMode();
    unsetTextInfo();
    mp_gameEventHandler->clear();
}

void Game::setTextInfo(const QString& text)
{
    mp_statusLabel->setText(text);
}

void Game::unsetTextInfo()
{
    mp_statusLabel->setText("");
}

void Game::loadCreatorInterface()
{
    Q_ASSERT(mp_startButton == 0);
    Q_ASSERT(mp_middleWidget->layout() == 0);
    mp_startButton = new QPushButton(mp_middleWidget);
    QBoxLayout* l = new QBoxLayout(QBoxLayout::LeftToRight);
    mp_startButton->setText(tr("Start game"));
    mp_startButton->setEnabled(0);
    connect(mp_startButton, SIGNAL(clicked()),
            this, SLOT(startButtonClicked()));
    connect(mp_serverConnection, SIGNAL(gameCanBeStarted(bool)),
            this, SLOT(gameCanBeStarted(bool)));
    l->addStretch(1);
    l->addWidget(mp_startButton);
    l->addStretch(1);
    mp_middleWidget->setLayout(l);
    m_interface = CreatorInterface;
}

void Game::unloadCreatorInterface()
{
    Q_ASSERT(mp_startButton != 0);
    Q_ASSERT(mp_middleWidget->layout() != 0);
    mp_startButton->deleteLater();
    mp_startButton = 0;
    delete mp_middleWidget->layout();
    m_interface = NoInterface;
}

void Game::loadGameInterface()
{
    Q_ASSERT(mp_deck == 0);
    Q_ASSERT(mp_graveyard == 0);
    Q_ASSERT(mp_selection == 0);
    Q_ASSERT(mp_middleWidget->layout() == 0);
    qDebug("---- loading game interface ----");
    mp_deck = new DeckWidget(0);
    mp_deck->init(&m_cardWidgetFactory);
    mp_graveyard = new GraveyardWidget(0);
    mp_graveyard->init(&m_cardWidgetFactory);
    mp_selection = new CardListWidget(0);
    mp_selection->setPocketType(POCKET_SELECTION);
    mp_selection->setCardSize(CardWidget::SIZE_NORMAL);
    mp_selection->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mp_selection->setHasBox(0);

    QBoxLayout* l = new QBoxLayout(QBoxLayout::LeftToRight);
    l->addStretch(3);
    l->addWidget(mp_graveyard);
    l->addStretch(1);
    l->addWidget(mp_deck);
    l->addStretch(3);

    QBoxLayout* l2 = new QBoxLayout(QBoxLayout::LeftToRight);
    l2->addStretch(3);
    l2->addWidget(mp_selection);
    l2->addStretch(3);

    QBoxLayout* l3 = new QVBoxLayout();
    l3->addLayout(l);
    l3->addStretch(1);
    l3->addLayout(l2);
    mp_middleWidget->setLayout(l3);
    m_interface = GameInterface;
}

void Game::unloadGameInterface()
{
    Q_ASSERT(mp_deck != 0);
    Q_ASSERT(mp_graveyard != 0);
    Q_ASSERT(mp_selection != 0);
    Q_ASSERT(mp_middleWidget->layout() != 0);
    qDebug("---- unloading game interface ----");
    mp_deck->deleteLater();
    mp_graveyard->deleteLater();
    mp_selection->deleteLater();
    delete mp_middleWidget->layout();
    m_interface = NoInterface;
    mp_deck = 0;
    mp_graveyard = 0;
    mp_selection = 0;
}

void Game::unloadInterface()
{
    switch(m_interface) {
    case NoInterface:
        return;
    case CreatorInterface:
        unloadCreatorInterface();
        return;
    case GameInterface:
        unloadGameInterface();
        return;
    }
}

void Game::playerJoinedGame(const PublicPlayerData& player)
{
    int i;
    for(i = 0; i < m_opponentWidgets.count(); ++i)
    {
        OpponentWidget* w = m_opponentWidgets[i];
        if (w->isVoid()) {
            w->setFromPublicData(player);
            m_players[player.id] = w;
            break;
        }
    }
    if (i == m_opponentWidgets.count()) {
        qCritical("Too many players connected to the game.");
    }
}

void Game::playerLeavedGame(int playerId)
{
    if (m_players.contains(playerId))
    {
        m_players[playerId]->clear();
        m_players.remove(playerId);
    }
}

void Game::playerUpdate(const PublicPlayerData& player)
{

    if (m_players.contains(player.id)) {
        m_players[player.id]->setFromPublicData(player);
    }

}

void Game::gameCanBeStarted(bool canBeStarted)
{
    mp_startButton->setEnabled(canBeStarted);
}

void Game::startButtonClicked()
{
    disconnect(mp_startButton, SIGNAL(clicked()),
               this, SLOT(startButtonClicked()));
    mp_serverConnection->startGame();
}

//void Game::gameStarted(const StructGame&, const StructPlayerList&)
//{
//    /*
//    int pI;
//    for(pI = 0; pI < playerList.count(); pI++)
//    {
//        if (playerList[pI].id == m_playerId) break;
//    }
//    qDebug() << "player: " << pI;
//    //mp_localPlayerWidget->setPlayer(playerList[pI]);
//    foreach(OpponentWidget* w, m_opponentWidgets)
//    {
//        w->clear();
//    }
//    for (int i = pI + 1; i < playerList.count(); ++i)
//    {
//        int pos = i - (pI + 1);
//        m_players[playerList[i].id] = m_opponentWidgets[pos];
//        m_opponentWidgets[pos]->setPlayer(playerList[i]);
//    }
//    for (int i = pI - 1; i >= 0; --i)
//    {
//        int pos = i - pI + 6;
//        m_players[playerList[i].id] = m_opponentWidgets[pos];
//        m_opponentWidgets[pos]->setPlayer(playerList[i]);
//    }*/
//    if (mp_startButton)
//    {
//        mp_startButton->hide();
//        mp_startButton->deleteLater();
//    }
//
//    mp_deck = new DeckWidget(0);
//    mp_deck->init(&m_cardWidgetFactory);
//    mp_graveyard = new CardPileWidget(0);
//    mp_graveyard->setPocketType(POCKET_GRAVEYARD);
//    mp_selection = new CardListWidget(0);
//    mp_selection->setPocketType(POCKET_SELECTION);
//    mp_selection->setCardSize(CardWidget::SIZE_NORMAL);
//    mp_selection->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//
//
//    if (mp_middleWidget->layout() != 0) {
//        delete mp_middleWidget->layout();
//    }
//
//    QBoxLayout* l = new QBoxLayout(QBoxLayout::LeftToRight);
//    l->addStretch(3);
//    l->addWidget(mp_graveyard);
//    l->addStretch(1);
//    l->addWidget(mp_deck);
//    l->addStretch(3);
//
//    QBoxLayout* l2 = new QBoxLayout(QBoxLayout::LeftToRight);
//    l2->addStretch(3);
//    l2->addWidget(mp_selection);
//    l2->addStretch(3);
//
//    QBoxLayout* l3 = new QVBoxLayout();
//    l3->addLayout(l);
//    l3->addStretch(1);
//    l3->addLayout(l2);
//    mp_middleWidget->setLayout(l3);
//}

CharacterType Game::character() const
{
    return mp_localPlayerWidget->characterWidget()->character();
}


void Game::assignPlayerWidget(int playerId, PlayerWidget* playerWidget)
{
    m_players[playerId] = playerWidget;
}

void Game::sendLogMessage(const QString& message)
{
    emit emitLogMessage(message);
}

void Game::pauseGameEvents()
{
    mp_gameEventHandler->pause();
}

void Game::resumeGameEvents()
{
    mp_gameEventHandler->resume();
}

