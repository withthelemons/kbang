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
#ifndef CARDMOVEMENTEVENT_H
#define CARDMOVEMENTEVENT_H

#include "gameevent.h"
#include "parser/parserstructs.h"

#include <QObject>
#include <QPoint>
#include <QList>
#include <QBasicTimer>
#include <QTime>


namespace client
{
class Game;
class CardPocket;
class CardWidget;

/**
 * This class handles the animation of cards in the game. Whenever there is a need
 * to move a card to a pocket, an instance of this class must be created. The instance
 * itself controls its lifetime, so you only need to create it with <b>new</b> operator
 * and leave it.
 *
 * @author MacJariel <MacJariel@gmail.com>
 */
class CardMovementEvent : public GameEvent
{
Q_OBJECT
public:
    /**
     * Creates an instance of CardMovementEvent class which starts the animation.
     * @param mainWidget The main widget of the window.
     * @param cardWidget The card widget that will be moved.
     * @param destination The target pocket.
     */
    CardMovementEvent(Game* game, const CardMovementData& cardMovementData);
    virtual ~CardMovementEvent();
    virtual void run();
    virtual void stop();
    virtual bool isReadyRun();

private:
    void setCardAndPocket();
    void startTransition();
    void timerEvent(QTimerEvent*);
    void stopTransition();

private slots:
    void unrevealCard();

private:
    CardMovementData    m_cardMovementData;
    CardWidget*         mp_card;
    CardPocket*         mp_destPocket;
    QPoint              m_srcPos;
    QPoint              m_currPos;
    QPoint              m_destPos;
    qreal               m_length;
    int                 m_tick;
    bool                m_cardAndPocketIsSet;
    static QBasicTimer  sm_timer;
    QTime               m_time;
};
}
#endif
