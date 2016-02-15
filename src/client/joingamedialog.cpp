/***************************************************************************
 *   Copyright (C) 2008 by MacJariel                                       *
 *   echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil"                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
c *   (at your option) any later version.                                   *
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

#include <QInputDialog>
#include <QHeaderView>

#include "joingamedialog.h"
#include "serverconnection.h"

#include "parser/queryget.h"
#include "util.h"
#include "config.h"

using namespace client;

JoinGameDialog::JoinGameDialog(QWidget *parent, ServerConnection* serverConnection):
        QDialog(parent),
        mp_serverConnection(serverConnection),
        m_currentGameId(0),
        m_currentPlayerId(0)
{
    setupUi(this);
    connect(pushButtonRefresh, SIGNAL(clicked()),
            this, SLOT(refreshGameList()));

    connect(lineEditPlayerName, SIGNAL(textChanged(QString)),
            this, SLOT(setButtonsState()));
    //playerListView->header()->setResizeMode(0, QHeaderView::Stretch);
    playerListView->header()->setStretchLastSection(0);
    playerListView->setHeaderLabels(QStringList() << tr("Player") << "" << "" << "");
    playerListView->header()->setResizeMode(0, QHeaderView::Stretch);
    playerListView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    playerListView->header()->setResizeMode(2, QHeaderView::ResizeToContents);
    playerListView->header()->setResizeMode(3, QHeaderView::ResizeToContents);
    /*
    playerListView->header()->resizeSection(0, 16);
    playerListView->header()->resizeSection(1, 16);
    playerListView->header()->resizeSection(2, 16);
    playerListView->header()->resizeSection(3, 16);
    */

    updateGameView();
    updateGameListView();
    refreshGameList();
    loadConfigValues();
}


JoinGameDialog::~JoinGameDialog()
{
}

QString gameState(GameState g)
{
    switch(g) {
        case GAMESTATE_WAITINGFORPLAYERS: return QObject::tr("waiting");
        case GAMESTATE_PLAYING: return QObject::tr("playing");
        case GAMESTATE_FINISHED: return QObject::tr("finished");
        default: break;
    }
    return "";
}


void setGameItem(const GameInfoData& gameInfo, QTreeWidgetItem* item)
{
    item->setData(0, Qt::UserRole, gameInfo.id);
    item->setText(0, gameInfo.name);
    switch(gameInfo.state) {
        case GAMESTATE_WAITINGFORPLAYERS:
            item->setText(1, "Waiting");
            item->setText(2, QString("%1 / %2").arg(gameInfo.totalPlayersCnt).arg(gameInfo.maxPlayers));
            break;
        case GAMESTATE_PLAYING:
            item->setText(1, "Playing");
            item->setText(2, QString("%1 / %2").arg(gameInfo.alivePlayersCnt).arg(gameInfo.totalPlayersCnt));
            break;
        case GAMESTATE_FINISHED:
            item->setText(1, "Finished");
            item->setText(2, "");
            break;
        default:
            NOT_REACHED();
    }
}



void JoinGameDialog::updateGameListView()
{
    bool isCurrentGameInList = 0;
    gameListView->clear();
    foreach(const GameInfoData& gameInfo, m_gameList) {
        QTreeWidgetItem* item = new QTreeWidgetItem(gameListView);
        item->setData(0, Qt::UserRole, gameInfo.id);
        item->setText(0, gameInfo.name);
        switch(gameInfo.state) {
            case GAMESTATE_WAITINGFORPLAYERS:
                item->setText(1, "Waiting");
                item->setText(2, QString("%1 / %2").arg(gameInfo.totalPlayersCnt).arg(gameInfo.maxPlayers));
                break;
            case GAMESTATE_PLAYING:
                item->setText(1, "Playing");
                item->setText(2, QString("%1 / %2").arg(gameInfo.alivePlayersCnt).arg(gameInfo.totalPlayersCnt));
                break;
            case GAMESTATE_FINISHED:
                item->setText(1, "Finished");
                item->setText(2, "");
                break;
            default:
                NOT_REACHED();
        }
        if (gameInfo.id == m_currentGameId) {
            gameListView->setCurrentItem(item);
            isCurrentGameInList = 1;
        }
    }
    if (!isCurrentGameInList)
        m_currentGameId = 0;
    setButtonsState();
}

void JoinGameDialog::updateGameView()
{
    const GameInfoData* gameInfo = 0;
    foreach(const GameInfoData& g, m_gameList) {
        if (g.id == m_currentGameId) {
            gameInfo = &g;
            break;
        }
    }
    if (gameInfo == 0) {
        labelName->setText("");
        labelDescription->setText("");
        labelState->setText("");
        labelPlayers->setText("");
        labelAIPlayers->setText("");
        labelPassword->setText("");
        playerListView->clear();
        return;
    }

    labelName->setText(gameInfo->name);
    labelDescription->setText(gameInfo->description);
    labelState->setText(gameState(gameInfo->state));
    labelPlayers->setText(gameInfo->minPlayers + " - " + gameInfo->maxPlayers);
    labelAIPlayers->setText(QString::number(gameInfo->AIPlayersCnt));
    if (gameInfo->hasPlayerPassword)
        labelPassword->setText(QObject::tr("players need password"));
    else if (gameInfo->hasSpectatorPassword)
        labelPassword->setText(QObject::tr("spectators need password"));
    else
        labelPassword->setText(QObject::tr("no password necessary"));

    playerListView->clear();
    foreach(const PlayerInfoData& p, gameInfo->players) {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setData(0, Qt::UserRole, p.id);
        if (p.id == m_currentPlayerId)
            playerListView->setCurrentItem(item);
        item->setText(0, p.name);
        if (p.hasController) {
            item->setIcon(1, QIcon(":/icons/connect.png"));
            item->setData(1, Qt::ToolTipRole, tr("Connected"));
        } else {
            item->setIcon(1, QIcon(":/icons/disconnect.png"));
            item->setData(1, Qt::ToolTipRole, tr("Disconnected"));
        }

        if (p.isAI) {
            item->setIcon(2, QIcon(":/icons/ai.png"));
            item->setData(2, Qt::ToolTipRole, tr("Artifical Intelligence"));
        } else {
            item->setIcon(2, QIcon(":/icons/human.png"));
            item->setData(2, Qt::ToolTipRole, tr("Human Player"));
        }

        if (p.isAlive) {
            item->setIcon(3, QIcon(":/icons/alive.png"));
            item->setData(3, Qt::ToolTipRole, tr("Player is Alive"));
        } else {
            item->setIcon(3, QIcon(":/icons/dead.png"));
            item->setData(3, Qt::ToolTipRole, tr("Player is Dead"));
        }

        if (p.hasController && !p.isAI) {
            item->setDisabled(1);
        } else {
            item->setDisabled(0);
        }
        playerListView->addTopLevelItem(item);
    }
    if (gameInfo->state == GAMESTATE_WAITINGFORPLAYERS &&
            gameInfo->totalPlayersCnt < gameInfo->maxPlayers) {
        QTreeWidgetItem* item = new QTreeWidgetItem(playerListView);
        item->setData(0, Qt::UserRole, -1);
        item->setText(0, QObject::tr("Create new player"));
        QFont font;
        font.setItalic(1);
        item->setData(0, Qt::FontRole, font);
        if (m_currentPlayerId <= 0) {
            m_currentPlayerId = -1;
            playerListView->setCurrentItem(item);
        }
    }
    setButtonsState();
}


void JoinGameDialog::updateGameList(const GameInfoListData& gameList)
{
    m_gameList = gameList;
    updateGameListView();
    updateGameView();
}

void JoinGameDialog::updateGame(const GameInfoData& game)
{
    QMutableListIterator<GameInfoData> it(m_gameList);
    bool gameIsInList = 0;
    while(it.hasNext()) {
        if (it.next().id == game.id) {
            it.setValue(game);
            gameIsInList = 1;
            break;
        }
    }
    if (gameIsInList == 0)
        m_gameList.append(game);

    updateGameListView();
    updateGameView();
}


void JoinGameDialog::refreshGameList()
{
    QueryGet* query = mp_serverConnection->queryGet();
    connect(query,  SIGNAL(result(const GameInfoListData&)),
            this,   SLOT(updateGameList(const GameInfoListData&)));
    query->getGameInfoList();
}

void JoinGameDialog::refreshGame(int gameId)
{
    QueryGet* query = mp_serverConnection->queryGet();
    connect(query, SIGNAL(result(const GameInfoData&)),
            this, SLOT(updateGame(const GameInfoData&)));
    query->getGameInfo(gameId);
}

void JoinGameDialog::selectGame(int gameId)
{
    if (m_currentGameId != gameId)
        m_currentPlayerId = 0;
    m_currentGameId = gameId;
    updateGameView();
    //refreshGame(gameId);
}


void JoinGameDialog::show()
{
    refreshGameList();
    QDialog::show();
}

void JoinGameDialog::on_gameListView_itemClicked(QTreeWidgetItem* item, int)
{
    int gameId = item->data(0, Qt::UserRole).toUInt();
    selectGame(gameId);
}

void JoinGameDialog::on_playerListView_itemClicked(QTreeWidgetItem * item, int)
{
    m_currentPlayerId = item->data(0, Qt::UserRole).toUInt();
    setButtonsState();
}

void JoinGameDialog::setButtonsState()
{
    bool enableSpectate = (m_currentGameId != 0);
    bool enablePlay = enableSpectate && (m_currentPlayerId != 0) &&
                        !lineEditPlayerName->text().isEmpty();
    pushButtonSpectate->setEnabled(enableSpectate);
    pushButtonPlay->setEnabled(enablePlay);
}

void JoinGameDialog::on_pushButtonPlay_clicked()
{
    Q_ASSERT(m_currentGameId != 0);
    Q_ASSERT(m_currentPlayerId != 0);

    const GameInfoData* game = gameInfoData(m_currentGameId);
    Q_ASSERT(game != 0);

    QString password;
    if (game->hasPlayerPassword) {
        bool ok;
        password = QInputDialog::getText(this, tr("Enter password"),
                                          tr("The game is protected by password. Please enter the password:"),
                                          QLineEdit::Password, "", &ok);
         if (!ok || password.isEmpty())
             return;
    }

    if (m_currentPlayerId == -1)
        m_currentPlayerId = 0;

    CreatePlayerData playerData;
    playerData.name     = lineEditPlayerName->text();
    playerData.password = lineEditPlayerPassword->text();
    playerData.avatar   = selectPlayerIconWidget->image();

    emit joinGame(m_currentGameId, m_currentPlayerId, password, playerData);
    saveConfigValues();
    close();
}

// TODO: SPECTATE
void JoinGameDialog::on_pushButtonSpectate_clicked()
{
    /*
    Q_ASSERT(mp_gameListView->currentItem() != 0);
    int gameId = mp_gameListView->currentItem()->data(0, Qt::UserRole).toUInt();
    QString gameName = mp_gameListView->currentItem()->text(0);
    emit joinGame(gameId, "", "Player"); // TODO: gamePassword, playerName
    close();
    */
}

const GameInfoData* JoinGameDialog::gameInfoData(int gameId)
{
    foreach(const GameInfoData& gameInfo, m_gameList) {
        if (gameInfo.id == gameId)
            return &gameInfo;
    }
    return 0;
}

void JoinGameDialog::loadConfigValues()
{
    Config& cfg = Config::instance();
    cfg.refresh();
    lineEditPlayerName->setText(cfg.readString("player", "name"));
    lineEditPlayerPassword->setText(cfg.readString("player", "password"));
    selectPlayerIconWidget->setImageFileName(cfg.readString("player", "image"));
    setButtonsState();
}

void JoinGameDialog::saveConfigValues()
{
    Config& cfg = Config::instance();
    cfg.writeString("player", "name", lineEditPlayerName->text());
    cfg.writeString("player", "password", lineEditPlayerPassword->text());
    cfg.writeString("player", "image", selectPlayerIconWidget->imageFileName());
    cfg.store();
}


