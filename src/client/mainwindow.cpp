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
#include "common.h"
#include "config.h"
#include "mainwindow.h"
#include "connecttoserverdialog.h"
#include "creategamedialog.h"
#include "joingamedialog.h"
#include "aboutdialog.h"
#include "logwidget.h"
#include "chatwidget.h"
#include "opponentwidget.h"
#include "localplayerwidget.h"
#include "parser/queryget.h"
#include "game.h"
#include "card.h"
#include "cardwidgetsizemanager.h"

#include <QPainter>
#include <QPaintEvent>

using namespace client;

MainWindow::MainWindow():
        mp_connectToServerDialog(0),
        mp_createGameDialog(0),
        mp_joinGameDialog(0),
        mp_aboutDialog(0),
        m_serverConnection(this),
        mp_game(0)
{
    setupUi(this);
    setStyleSheet(styleSheet() + "\n"
        "#mp_centralWidget {\n"
        "   background-image: url(\"" + Config::dataPathString() + "gfx/misc/bang-artwork.png\");\n"
        "}\n\n");
    Card::loadDefaultRuleset();
    mp_cardWidgetSizeManager = new CardWidgetSizeManager(this);

    createActions();
    createMenu();
    createWidgets();
    updateActions();

    connect(&m_serverConnection, SIGNAL(statusChanged()),
            this, SLOT(serverConnectionStatusChanged()));
    connect(&m_serverConnection, SIGNAL(enterGameMode(int, const QString&, ClientType)),
            this,                SLOT(enterGameMode(int, const QString&, ClientType)));
    connect(&m_serverConnection, SIGNAL(exitGameMode()),
            this,                SLOT(exitGameMode()));
}


MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    QRadialGradient g(width() / 2, height() / 2, width() / 2, width() / 2 , height() / 2);
    g.setColorAt(0, QColor(239, 215, 179));
    g.setColorAt(0.5 , QColor(211, 179, 140));
    painter.fillRect(e->rect(), g);
    QMainWindow::paintEvent(e);
}

void MainWindow::showConnectToServerDialog()
{
    if (!mp_connectToServerDialog)
    {
        mp_connectToServerDialog = new ConnectToServerDialog(this);
        connect(mp_connectToServerDialog, SIGNAL(connectToServer(QString, int)),
                &m_serverConnection, SLOT(connectToServer(QString, int)));
    }
    mp_connectToServerDialog->show();
}

void MainWindow::disconnectFromServer()
{
    if (mp_game != 0)
        exitGameMode();
    m_serverConnection.disconnectFromServer();
}

void MainWindow::showCreateGameDialog()
{
    if (!mp_createGameDialog) {
        mp_createGameDialog = new CreateGameDialog(this);
        connect(mp_createGameDialog,SIGNAL(createGame(CreateGameData,CreatePlayerData)),
                &m_serverConnection, SLOT(createGame(CreateGameData,CreatePlayerData)));
    }
    mp_createGameDialog->show();
}


void MainWindow::showJoinGameDialog()
{
    if (!mp_joinGameDialog) {
        mp_joinGameDialog = new JoinGameDialog(this, &m_serverConnection);
        connect(mp_joinGameDialog, SIGNAL(joinGame(int,int,QString,CreatePlayerData)),
                &m_serverConnection, SLOT(joinGame(int,int,QString,CreatePlayerData)));
    }
    mp_joinGameDialog->show();
}

void MainWindow::leaveGame()
{
    if (m_serverConnection.isConnected()) {
        m_serverConnection.leaveGame();
    } else {
        exitGameMode();
    }
}

void MainWindow::showAboutDialog()
{
    if (!mp_aboutDialog) {
        mp_aboutDialog = new AboutDialog(this);
    }
    mp_aboutDialog->show();
}

void MainWindow::enterGameMode(int gameId, const QString& gameName, ClientType clientType)
{
    Q_ASSERT(mp_game == 0);
    GameWidgets x(mp_centralWidget, mp_middleWidget, mp_localPlayerWidget, m_opponentWidgets, mp_statusLabel);
    mp_game = new Game(this, gameId, clientType, &m_serverConnection, x);
    connect(mp_game, SIGNAL(emitLogMessage(const QString&)),
            mp_logWidget, SLOT(appendLogMessage(const QString&)));
    mp_logWidget->appendLogMessage(tr("You have joined <i>%1</i>.").arg(gameName));
    updateActions();
}

void MainWindow::exitGameMode()
{
    mp_game->clear();
    mp_game->deleteLater();
    mp_game = 0;
    mp_chatWidget->clear();
    updateActions();
}

void MainWindow::serverConnectionStatusChanged()
{
    updateActions();
}


void MainWindow::createActions()
{
    connect(mp_actionConnectToServer, SIGNAL(triggered()),
            this, SLOT(showConnectToServerDialog()));
    connect(mp_actionDisconnectFromServer, SIGNAL(triggered()),
            this, SLOT(disconnectFromServer()));
    connect(actionCreateGame, SIGNAL(triggered()),
            this, SLOT(showCreateGameDialog()));
    connect(mp_actionJoinGame, SIGNAL(triggered()),
            this, SLOT(showJoinGameDialog()));
    connect(mp_actionLeaveGame, SIGNAL(triggered()),
            this, SLOT(leaveGame()));
    connect(mp_actionAbout, SIGNAL(triggered()),
            this, SLOT(showAboutDialog()));
}


void MainWindow::createMenu()
{
}

void MainWindow::createWidgets()
{
    m_opponentWidgets.append(mp_opponent1);
    m_opponentWidgets.append(mp_opponent2);
    m_opponentWidgets.append(mp_opponent3);
    m_opponentWidgets.append(mp_opponent4);
    m_opponentWidgets.append(mp_opponent5);
    m_opponentWidgets.append(mp_opponent6);

    connect(mp_chatWidget, SIGNAL(outgoingMessage(const QString&)),
            &m_serverConnection, SLOT(sendChatMessage(const QString&)));
    connect(&m_serverConnection, SIGNAL(incomingChatMessage(int, const QString&, const QString&)),
            mp_chatWidget, SLOT(incomingMessage(int, const QString&, const QString&)));

    connect(&m_serverConnection, SIGNAL(logMessage(QString)),
            mp_logWidget, SLOT(appendLogMessage(QString)));
    connect(&m_serverConnection, SIGNAL(incomingData(const QByteArray&)),
            mp_logWidget, SLOT(appendIncomingData(const QByteArray&)));
    connect(&m_serverConnection, SIGNAL(outgoingData(const QByteArray&)),
            mp_logWidget, SLOT(appendOutgoingData(const QByteArray&)));

}

void MainWindow::updateActions()
{
    if (m_serverConnection.isConnected())
    {
        mp_actionConnectToServer->setEnabled(0);
        mp_actionDisconnectFromServer->setEnabled(1);
        if (mp_game != 0)
        {
            actionCreateGame->setEnabled(0);
            mp_actionJoinGame->setEnabled(0);
            mp_actionLeaveGame->setEnabled(1);
        }
        else
        {
            actionCreateGame->setEnabled(1);
            mp_actionJoinGame->setEnabled(1);
            mp_actionLeaveGame->setEnabled(0);
        }
    }
    else
    {
        mp_actionConnectToServer->setEnabled(1);
        mp_actionDisconnectFromServer->setEnabled(0);
        actionCreateGame->setEnabled(0);
        mp_actionJoinGame->setEnabled(0);
        mp_actionLeaveGame->setEnabled(mp_game != 0);
    }
}
