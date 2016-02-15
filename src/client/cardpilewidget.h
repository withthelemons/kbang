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
#ifndef CARDPILEWIDGET_H
#define CARDPILEWIDGET_H

#include <QWidget>
#include <QStack>
#include <QSize>

#include "cardpocket.h"
#include "cardwidget.h"





namespace client
{

/**
 * The CardPileWidget presents a kind of CardPocket (the place for holding a bunch
 * of playing cards), that keeps its cards on a pile. An example of CardPileWidget can
 * be the deck or the graveyard.
 * @author MacJariel <MacJariel@gmail.com>
 */
class CardPileWidget : public CardPocket
{
Q_OBJECT
public:
    CardPileWidget(QWidget *parent = 0);
    ~CardPileWidget();

    virtual CardWidget* peek();
    virtual CardWidget* pop();
    virtual void push(CardWidget* card);
    virtual QPoint newCardPosition() const;

protected:
    QStack<CardWidget*> m_cards;
    CardWidget::Size m_cardWidgetSize;

private:
    QSize            m_padding;   
};
}
#endif
