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
#include "chatwidget.h"

#include <QPainter>
#include <QPaintEvent>


using namespace client;

ChatWidget::ChatWidget(QWidget *parent)
 : QWidget(parent)
{
    setupUi(this);
    setContentsMargins(5, 5, 5, 5);
    /*
    QPalette palette = QApplication::palette();
    palette.setColor(QPalette::Base, QColor(0, 0, 0, 16));
    setPalette(palette);
    */

    mp_chatView->setFocusPolicy(Qt::NoFocus);
    mp_messageBox->setFocusPolicy(Qt::ClickFocus);
    connect(mp_messageBox, SIGNAL(returnPressed()),
            this, SLOT(sendMessage()));
}


ChatWidget::~ChatWidget()
{
}

void ChatWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.fillRect(event->rect().intersect(contentsRect()), QColor(0, 0, 0, 32));
}

void ChatWidget::clear()
{
    mp_chatView->clear();
}

void ChatWidget::incomingMessage(int, const QString& senderName, const QString& message)
{
    mp_chatView->append(QString("<b>%1:</b> %2").arg(Qt::escape(senderName)).arg(Qt::escape(message)));
}

void ChatWidget::sendMessage()
{
    const QString& message = mp_messageBox->text();
    mp_messageBox->clear();
    emit outgoingMessage(message);
}


