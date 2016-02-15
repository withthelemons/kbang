#include "cardwidgetsizemanager.h"

using namespace client;

qreal CardWidgetSizeManager::sm_cardRatio = 20 / 31;
qreal CardWidgetSizeManager::sm_lifePointCardRatio[] = {0, 19/75, 31/75, 44/75, 57/75, 68/75};
int CardWidgetSizeManager::sm_initialHeight = 75;

CardWidgetSizeManager::CardWidgetSizeManager(QObject* parent):
        QObject(parent)
{
    m_normalSize = QSize(width(sm_initialHeight), sm_initialHeight);
    m_zoomSize = QSize(200, 310);
}


int CardWidgetSizeManager::lifeLevel(int lifePoints) const
{
    if (lifePoints < 0 || lifePoints > 5)
        return 0;
    return (int)(m_normalSize.height() * sm_lifePointCardRatio[lifePoints]);
}

QSize CardWidgetSizeManager::initialSize()
{
    return QSize(width(sm_initialHeight), sm_initialHeight);
}

int CardWidgetSizeManager::width(int height)
{
    return (int)(height * sm_cardRatio);
}
