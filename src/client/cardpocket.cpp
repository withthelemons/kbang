#include "cardpocket.h"
#include "cardwidget.h"

using namespace client;

CardPocket::CardPocket(QWidget *parent):
        QFrame(parent),
        m_pocketType(POCKET_INVALID),
        m_ownerId(0)
{
}

CardPocket::~CardPocket()
{
}

void CardPocket::push(CardWidget* cardWidget)
{
    cardWidget->setOwnerId(m_ownerId);
    cardWidget->setPocketType(m_pocketType);
}

void CardPocket::setPocketType(const PocketType& pocketType)
{
    m_pocketType = pocketType;
}

void CardPocket::setOwnerId(int ownerId)
{
    m_ownerId = ownerId;
}

