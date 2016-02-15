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
#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QSize>
#include "cardwidget.h"

namespace client {

class CharacterWidget: public QWidget
{
Q_OBJECT
public:
    CharacterWidget(QWidget *parent = 0);
    virtual ~CharacterWidget();

    void init(CardWidgetFactory* cardWidgetFactory);


    inline CharacterType character() const { return m_character; }
    inline int     lifePoints() const { return m_lifePoints; }

    void setOwnerId(int ownerId);
    void setCharacter(CharacterType character);
    void setLifePoints(int lifePoints);
    bool isEmpty() const;

private:
    void lifePointsChanged();

private slots:
    void onTimeout();

signals:
    void animationFinished();

private:
    CharacterType m_character;
    int m_lifePoints;
    int m_sourceY;
    int m_targetY;

    CardWidget* mp_backCard;
    CardWidget* mp_characterCard;

    bool          m_isAnimating;

    static QTimer sm_timer;
    static int    sm_countAnimaton;
    //QSize         m_sizeHint;

    CardWidgetFactory*  mp_cardWidgetFactory;
    QTime         m_time;
};

}
#endif
