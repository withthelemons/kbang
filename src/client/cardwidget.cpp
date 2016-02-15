/***************************************************************************
 *   Copyright (C) 2008 by MacJariel                                       *
 *   echo "dmailet@gbalt.dob" | tr "edibmlt" "ecrmjil"                   *
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
#include <QtCore>
#include <QtDebug>
#include <QPainter>
#include <QMouseEvent>


#include "card.h"
#include "cardwidget.h"
#include "cardactionswidget.h"
#include "gameactionmanager.h"

using namespace client;


const QSize CardWidget::sm_qsizeSmall(48, 75);
const QSize CardWidget::sm_qsizeNormal(100, 155);
const QSize CardWidget::sm_qsizeBig(300, 465);

int CardWidget::sm_lifeLevels[6] = {0, 19, 31, 44, 57, 68};
//int CardWidget::sm_lifeLevels[6] = {0, 21, 34, 49, 64, 77};

//const QSize  margins(10,10);
//const QPoint padding(5,5);

CardWidget::CardWidget(QWidget* parent, Card::Type cardType):
        QLabel(parent),
        m_cardType(cardType),
        m_pocket(POCKET_INVALID),
        m_ownerId(0),
        m_playerRole(ROLE_UNKNOWN),
        m_characterType(CHARACTER_UNKNOWN),
        m_qsize(sm_qsizeSmall),
        m_shadowMode(0),
        m_hasHighlight(0),
        m_isEmpty(0),
        mp_gameActionManager(0)
{
    show();
}

CardWidget::~ CardWidget()
{
}

void CardWidget::setGameActionManager(GameActionManager* gameActionManager)
{
    mp_gameActionManager = gameActionManager;
}

void CardWidget::setType(Card::Type cardType)
{
    m_cardType = cardType;
}

void CardWidget::clone(CardWidget* other)
{
    m_cardType = other->type();
    m_playerRole = other->m_playerRole;
    m_characterType = other->character();
    m_cardData = other->cardData();
}

void CardWidget::setCardData(const CardData& cardData)
{
    m_cardData = cardData;
    m_isEmpty = 0;
}

void CardWidget::setSize(Size size)
{
    m_size = size;
    m_qsize = qSize(size);
}

void CardWidget::validate()
{
    if (m_isEmpty) {
        setPixmap(QPixmap());
    } else {
        const Card* card;
        switch(m_cardType) {
        case Card::Playing:
            card = Card::findPlayingCard(m_cardData.type);
            break;
        case Card::Role:
            card = Card::findRoleCard(m_playerRole);
            break;
        case Card::Character:
            card = Card::findCharacterCard(m_characterType);
            break;
        }
        if (card == 0) {
            qFatal("Cannot look-up the card!");
        }
        if (card->image().isNull()) {
            qWarning(qPrintable(QString("Card '%1' has null pixmap.").arg(card->name())));
        }
        if (m_cardType == Card::Playing && m_cardData.type != CARD_UNKNOWN) {
            setPixmap(card->image(m_cardData.suit, m_cardData.rank).
                      scaled(m_qsize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        } else {
            setPixmap(card->image().scaled(m_qsize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
    }
    setMinimumSize(m_qsize);
    setMaximumSize(m_qsize);
    resize(m_qsize);
}

void CardWidget::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    if (m_hasHighlight) {
        QPainter painter(this);
        painter.fillRect(this->rect(), QBrush(QColor(0,0,0,128)));
    }
}

QSize CardWidget::qSize(Size size)
{
    switch(size) {
        case SIZE_SMALL:  return sm_qsizeSmall;  break;
        case SIZE_NORMAL: return sm_qsizeNormal; break;
        case SIZE_BIG:    return sm_qsizeBig;    break;
    }
    Q_ASSERT(0);
    return QSize(); // avoid warning
}

int CardWidget::lifeLevel(int lifePoints)
{
    if (lifePoints < 0 || lifePoints > 5)
        return 0;
    return sm_lifeLevels[lifePoints];
}




void CardWidget::setPocketType(const PocketType& pocket)
{
    m_pocket = pocket;
}

void CardWidget::setOwnerId(int ownerId)
{
    m_ownerId = ownerId;
}

void CardWidget::setPlayerRole(PlayerRole playerRole)
{
    m_playerRole = playerRole;
    m_isEmpty = 0;
}

void CardWidget::setCharacterType(CharacterType characterType)
{
    m_characterType = characterType;
    m_isEmpty = 0;
}

void CardWidget::setEmpty()
{
    m_isEmpty = 1;
}

void CardWidget::setHighlight(bool hasHighlight)
{
    m_hasHighlight = hasHighlight;
    update();
}

QPoint CardWidget::center() const
{
    return QPoint((int)(QWidget::size().width() / 2),
                          (int)(QWidget::size().height() / 2));
}

void CardWidget::mousePressEvent(QMouseEvent* e)
{
    if (mp_gameActionManager) {
        switch(e->button()) {
        case Qt::LeftButton: {
            bool handled = mp_gameActionManager->onCardClicked(this);
            if (!handled)
                QLabel::mousePressEvent(e);
            break;
        }
        case Qt::RightButton:
            mp_gameActionManager->onCardRightClicked(this);
            break;
        default:
            break;
        }
    }
}


