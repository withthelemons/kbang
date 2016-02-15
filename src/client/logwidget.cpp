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
#include "logwidget.h"

#include <QPainter>
#include <QPaintEvent>

using namespace client;

LogWidget::LogWidget(QWidget *parent)
: QWidget(parent), m_dataType(0)
{
    setupUi(this);
    mp_xmlView->setFontPointSize(8);
}


LogWidget::~LogWidget()
{
}

void LogWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.fillRect(event->rect().intersect(contentsRect()), QColor(0, 0, 0, 32));
}


void LogWidget::appendLogMessage(QString message)
{
    mp_logView->append(message);
}


void LogWidget::appendIncomingData(const QByteArray & data)
{
    QTextCursor x = mp_xmlView->textCursor();
    x.movePosition(QTextCursor::End);
    mp_xmlView->setTextCursor(x);
    if (m_dataType == 2)
    {
        mp_xmlView->append("");
    }
    if (m_dataType != 1)
    {
        mp_xmlView->setTextColor(Qt::yellow);
    }
    mp_xmlView->insertPlainText(data);
    m_dataType = 1;
    x = mp_xmlView->textCursor();
    x.movePosition(QTextCursor::End);
    mp_xmlView->setTextCursor(x);
}

void LogWidget::appendOutgoingData(const QByteArray & data)
{
    QTextCursor x = mp_xmlView->textCursor();
    x.movePosition(QTextCursor::End);
    mp_xmlView->setTextCursor(x);
    if (m_dataType == 1)
    {
        mp_xmlView->append("");
    }
    if (m_dataType != 2)
    {
        mp_xmlView->setTextColor(Qt::white);
    }
    mp_xmlView->setFontPointSize(9);
    mp_xmlView->insertPlainText(data);
    m_dataType = 2;
    x = mp_xmlView->textCursor();
    x.movePosition(QTextCursor::End);
    mp_xmlView->setTextCursor(x);
}
