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
#include "playerwidget.h"
#include "characterwidget.h"
#include "game.h"
#include "gameactionmanager.h"
#include "cardlistwidget.h"
#include "parser/parserstructs.h"
#include "config.h"

#include <QPainter>
#include <QMouseEvent>


using namespace client;

PlayerWidget::PlayerWidget(QWidget* parent):
        QWidget(parent),
        m_id(0),
        m_hasController(1),
        m_isAI(0),
        m_isAlive(1),
        m_isSheriff(0),
        m_isWinner(0),
        m_isCurrent(0),
        m_isRequested(0),
        m_playerRole(ROLE_UNKNOWN),
        mp_winnerIcon(0),
        mp_game(0),
        mp_cardWidgetSizeManager(0)
{
}

PlayerWidget::~PlayerWidget()
{
}

void PlayerWidget::setup(CardWidgetSizeManager* cardWidgetSizeManager)
{
    mp_cardWidgetSizeManager = cardWidgetSizeManager;
}

void PlayerWidget::enterGameMode(Game* game)
{
    mp_game = game;
    characterWidget()->init(game->cardWidgetFactory());
}

void PlayerWidget::leaveGameMode()
{
    mp_game = 0;
    clear();
}

void PlayerWidget::clear()
{
    m_id = m_isAI = m_isSheriff = m_isWinner = m_isCurrent = m_isRequested = 0;
    m_name = QString();
    m_isAlive = m_hasController = 1;
    m_avatar = QPixmap();
    m_playerRole = ROLE_UNKNOWN;

    characterWidget()->setCharacter(CHARACTER_UNKNOWN);
    hand()->clear();
    table()->clear();

    clearWidgets();
}

void PlayerWidget::setCurrent(bool isCurrent)
{
    m_isCurrent = isCurrent;
}

void PlayerWidget::setRequested(bool isRequested)
{
    m_isRequested = isRequested;
}

void PlayerWidget::setFromPublicData(const PublicPlayerData& publicPlayerData)
{
    qDebug() << this << "setFromPublicData()";
    m_id            = publicPlayerData.id;
    m_name          = publicPlayerData.name;
    m_hasController = publicPlayerData.hasController;
    m_isAI          = publicPlayerData.isAI;
    m_isAlive       = publicPlayerData.isAlive;
    m_isSheriff     = publicPlayerData.isSheriff;
    m_isWinner      = publicPlayerData.isWinner;

    setRoleFromPublicData(publicPlayerData.role);

    characterWidget()->setCharacter(publicPlayerData.character);
    characterWidget()->setLifePoints(publicPlayerData.lifePoints);
    characterWidget()->setOwnerId(m_id);

    hand()->setOwnerId(m_id);
    table()->setOwnerId(m_id);

    if (!publicPlayerData.avatar.isNull()) {
        m_avatar = QPixmap::fromImage(publicPlayerData.avatar).
                   scaled(avatarLabel()->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else {
        m_avatar = QPixmap();
    }

    setHandSize(publicPlayerData.handSize);
    setTable(publicPlayerData.table);
    updateWidgets();
}

void PlayerWidget::dieAndRevealRole(PlayerRole playerRole)
{
    m_isAlive = 0;
    m_playerRole = playerRole;
    updateWidgets();
}

void PlayerWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setClipRect(event->rect());
    painter.fillRect(contentsRect(), QColor(0, 0, 0, 32));

    if (m_isCurrent) {
        painter.setPen(Qt::blue);
        painter.drawRect(contentsRect().adjusted(0, 0, -1, -1));
    }

    if (m_isRequested) {
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(2);
        pen.setStyle(Qt::DashLine);
        painter.setPen(pen);
        painter.drawRect(rect().adjusted(1, 1, -2, -2));
    }
}

void PlayerWidget::mousePressEvent(QMouseEvent* event)
{
    if (gameActionManager() && event->button() == Qt::LeftButton) {
        gameActionManager()->onPlayerClicked(this);
    }
}

GameActionManager* PlayerWidget::gameActionManager() const
{
    if (mp_game == 0) {
        return 0;
    } else {
        return mp_game->gameActionManager();
    }
}

CardWidgetFactory* PlayerWidget::cardWidgetFactory() const
{
    if (mp_game == 0) {
        return 0;
    } else {
        return mp_game->cardWidgetFactory();
    }
}

void PlayerWidget::setRoleFromPublicData(PlayerRole playerRole)
{
    m_playerRole = playerRole;
}

void PlayerWidget::setHandSize(int handSize)
{
    hand()->clear();
    for(int i = 0; i < handSize; ++i) {
        CardWidget* card = mp_game->cardWidgetFactory()->createPlayingCard(this);
        card->validate();
        hand()->push(card);
    }
}

void PlayerWidget::setTable(QList<CardData> tableCards)
{
    table()->clear();
    foreach (const CardData& cardData, tableCards) {
        CardWidget* card = mp_game->cardWidgetFactory()->createPlayingCard(this);
        card->setCardData(cardData);
        card->validate();
        table()->push(card);
    }
}

void PlayerWidget::clearWidgets()
{
    playerNameLabel()->setText("");

    if (mp_winnerIcon)
        mp_winnerIcon->hide();

    avatarLabel()->setPixmap(QPixmap());
}

void PlayerWidget::updateWidgets()
{
    playerNameLabel()->setText(m_name);
    updateWinnerIcon();
    updateAvatarLabel();
}

void PlayerWidget::updateAvatarLabel()
{
    QPixmap avatar = m_avatar;
    if (!m_hasController && !avatar.isNull()) {
        QPainter painter(&avatar);
        painter.fillRect(avatar.rect(), QColor(0, 0, 0, 128));
    }
    avatarLabel()->setPixmap(avatar);
}

void PlayerWidget::createWinnerIcon()
{
    Q_ASSERT(mp_winnerIcon == 0);
    QPixmap winnerPixmap(Config::dataPathString() + "gfx/misc/winner-sign.png");
    mp_winnerIcon = new QLabel(this);
    mp_winnerIcon->setPixmap(winnerPixmap);
    mp_winnerIcon->resize(winnerPixmap.size());
    mp_winnerIcon->setToolTip(tr("This player is winner."));
}

void PlayerWidget::updateWinnerIcon()
{
    if (m_isWinner && mp_game && mp_game->isFinished()) {
        if (mp_winnerIcon == 0) {
            createWinnerIcon();
        }
        moveWinnerIcon();
        mp_winnerIcon->show();
        mp_winnerIcon->raise();
    } else {
        if (mp_winnerIcon) {
            mp_winnerIcon->hide();
        }
    }
}


