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
#include "deckwidget.h"
#include "cardwidgetfactory.h"

using namespace client;

DeckWidget::DeckWidget(QWidget *parent):
        CardPileWidget(parent),
        mp_cardWidgetFactory(0)
{
    setPocketType(POCKET_DECK);
}

DeckWidget::~DeckWidget()
{
}

void DeckWidget::init(CardWidgetFactory* cardWidgetFactory)
{
    mp_cardWidgetFactory = cardWidgetFactory;
    m_cards.push(newCard());
}

CardWidget* DeckWidget::pop()
{
    return newCard();
}

void DeckWidget::push(CardWidget* card)
{
    card->hide();
    card->deleteLater();
}

CardWidget* DeckWidget::newCard()
{
    Q_ASSERT(mp_cardWidgetFactory != 0);
    CardWidget* w = mp_cardWidgetFactory->createPlayingCard(this);
    w->setSize(m_cardWidgetSize);
    w->validate();
    w->raise();
    w->move(newCardPosition());
    w->show();
    w->setOwnerId(0);
    w->setPocketType(POCKET_DECK);
    return w;
}

