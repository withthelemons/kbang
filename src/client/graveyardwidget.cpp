#include "graveyardwidget.h"
#include "cardwidget.h"
#include "cardwidgetfactory.h"

using namespace client;

GraveyardWidget::GraveyardWidget(QWidget* parent):
        CardPileWidget(parent),
        mp_firstCard(0),
        mp_secondCard(0)
{
    setPocketType(POCKET_GRAVEYARD);
}

GraveyardWidget::~GraveyardWidget()
{
}

void GraveyardWidget::init(CardWidgetFactory* cardWidgetFactory)
{
    mp_cardWidgetFactory = cardWidgetFactory;
}

CardWidget* GraveyardWidget::pop()
{
    CardWidget* res = mp_firstCard;
    mp_firstCard = mp_secondCard;
    return res;
}

void GraveyardWidget::push(CardWidget* card)
{
    CardPocket::push(card);
    mp_secondCard->deleteLater();
    mp_secondCard = 0;
    mp_firstCard = card;
    card->setParent(this);
    card->setSize(m_cardWidgetSize);
    card->validate();
    card->move(newCardPosition());
    card->raise();
    card->show();
}

void GraveyardWidget::setFirstCard(const CardData& cardData)
{
    if (cardData.id == 0)
        return;
    if (mp_firstCard) mp_firstCard->deleteLater();
    mp_firstCard = setCard(cardData);
    mp_firstCard->show();
}

void GraveyardWidget::setSecondCard(const CardData& cardData)
{
    Q_ASSERT(mp_cardWidgetFactory != 0);
    Q_ASSERT(mp_secondCard == 0);
    Q_ASSERT(mp_firstCard != 0);
    if (cardData.id == 0)
        return;

    mp_secondCard = setCard(cardData);
    mp_secondCard->stackUnder(mp_firstCard);
    mp_secondCard->show();
}

CardWidget* GraveyardWidget::setCard(const CardData& cardData)
{
    CardWidget* res = mp_cardWidgetFactory->createPlayingCard(this);
    res->setSize(m_cardWidgetSize);
    res->setCardData(cardData);
    res->validate();
    res->move(newCardPosition());
    res->setOwnerId(0);
    res->setPocketType(POCKET_GRAVEYARD);
    return res;
}
