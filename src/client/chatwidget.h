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
#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include "ui_chatwidget.h"

namespace client
{

/**
 *@author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class ChatWidget : public QWidget, public Ui::ChatWidget
{
Q_OBJECT
public:
    ChatWidget(QWidget *parent = 0);
    ~ChatWidget();
    virtual void paintEvent(QPaintEvent* event);
    void clear();

public slots:
    void incomingMessage(int senderId, const QString& senderName, const QString& message);
    void sendMessage();

signals:
    void outgoingMessage(const QString& message);

};
}
#endif
