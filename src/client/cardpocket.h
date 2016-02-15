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
#ifndef CARDPOCKET_H
#define CARDPOCKET_H

#include <QFrame>
#include <QPoint>

#include "parser/parserstructs.h"

namespace client
{
class CardWidget;

/**
 * @author MacJariel <MacJariel@gmail.com>
 */
class CardPocket : public QFrame
{
Q_OBJECT
public:
    CardPocket(QWidget *parent);
    virtual ~CardPocket();
    virtual QPoint newCardPosition() const = 0;
    virtual void push(CardWidget*);
    void setPocketType(const PocketType&);
    void setOwnerId(int);

signals:
    void newCardPositionChanged(const QPoint&);

protected:
    PocketType m_pocketType;
    int        m_ownerId;
};
}
#endif
